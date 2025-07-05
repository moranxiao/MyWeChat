#pragma once

#include <brpc/channel.h>
#include <mutex>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "logger.hpp"

namespace common {
    //单个服务的信道管理类
    //一个服务可能有多个节点
    class ServiceChannel{
    public:
        using ChannelPtr = std::shared_ptr<brpc::Channel>; 
        using ptr = std::shared_ptr<ServiceChannel>;
    public:
        ServiceChannel(const std::string& service_name)
        :_service_name(service_name),_idx(0)
        {}
        void Append(const std::string& host)
        {
            ChannelPtr channel = std::make_shared<brpc::Channel>();
            brpc::ChannelOptions options;
            options.connect_timeout_ms = -1;
            options.timeout_ms = -1;
            options.max_retry = 3;
            options.protocol = "baidu_std";
            if(channel->Init(host.c_str(),&options) == -1)
            {
                LOG_WARN("新增主机失败:{}-{}",_service_name,host);
                return;
            }
            std::unique_lock<std::mutex>(_mtx);
            _channels.push_back(channel);
            _host_to_channel[host] = channel;
        }
        void Remove(const std::string& host)
        {
            std::unique_lock<std::mutex>(_mtx);
            auto it = _host_to_channel.find(host);
            if(it == _host_to_channel.end())
            {
                LOG_WARN("找不到要删除的主机:{}-{}",_service_name,host);
                return;
            }
            for(auto vit = _channels.begin(); vit != _channels.end(); vit++)
            {
                if(*vit == it->second)
                {
                    _channels.erase(vit);
                    break;
                }
            }
            _host_to_channel.erase(it);
        }
        ChannelPtr Choose()
        {
            std::unique_lock<std::mutex>(_mtx);
            if(_channels.size() == 0)
            {
                LOG_ERROR("没有能提供服务的主机：{}", _service_name);
                return {};
            }
            _idx = (_idx+1)%_channels.size();
            return _channels[_idx];
        }
    private:
        std::mutex _mtx;
        uint16_t _idx;  //当前轮转下表计数器
        std::string _service_name; //服务名称
        std::vector<ChannelPtr> _channels; //当前服务所有信道
        std::unordered_map<std::string,ChannelPtr> _host_to_channel; //主机与信道映射
    };

    //服务管理类
    class ServiceManager{
    public:
        using ptr = std::shared_ptr<ServiceManager>;
        ServiceManager(){}
        ~ServiceManager(){}
        ServiceChannel::ChannelPtr Choose(const std::string& service_name)
        {
            std::unique_lock<std::mutex>(_mtx);
            auto it = _service_to_channel.find(service_name);
            if(it == _service_to_channel.end())
            {
                LOG_WARN("{}服务没有对应的信道", service_name);
                return {};
            }
            return it->second->Choose();
        } 
        //放弃关心服务，不关心的服务就不用管理了
        void Abandon(const std::string& service_name)
        {
            std::unique_lock<std::mutex>(_mtx);
            _attention_services.erase(service_name);
            _service_to_channel.erase(service_name);
        }
        //关心服务
        void AddAttenion(const std::string& service_name)
        {
            std::unique_lock<std::mutex>(_mtx);
            _attention_services.insert(service_name);
        }
        void ServiceOnlineCallback(const std::string& service_instance,const std::string& host)
        {
            std::string service_name = _GetServiceName(service_instance);
            ServiceChannel::ptr channel;
            {
                std::unique_lock<std::mutex>(_mtx);
                if(_attention_services.count(service_name) == 0) 
                {
                    LOG_DEBUG("{}-{} 服务上线了，但是当前并不关心！", service_name, host);
                    return;
                }
                auto it = _service_to_channel.find(service_name);
                if(it == _service_to_channel.end())
                {
                    channel = std::make_shared<ServiceChannel>(service_name);
                    _service_to_channel.insert(std::make_pair(service_name,channel));
                }
                else channel = it->second;
            }
            if(!channel) 
            {
                LOG_ERROR("新增服务管理节点失败：{}",service_name);
                return;
            }
            channel->Append(host);
            LOG_DEBUG("{}-{} 服务上线新节点成功,进行添加管理",service_name,host);
        }
        void ServiceOfflineCallback(const std::string& service_instance,const std::string& host)
        {
            std::string service_name = _GetServiceName(service_instance);
            ServiceChannel::ptr channel;
            {
                std::unique_lock<std::mutex>(_mtx);

                if(_attention_services.count(service_name) == 0) 
                {
                    LOG_DEBUG("{}-{} 服务下线了，但是当前并不关心！", service_name, host);
                    return;
                }
                auto it = _service_to_channel.find(service_name);
                if(it == _service_to_channel.end())
                {
                    LOG_WARN("删除{}服务节点时，没有找到管理对象", service_name);
                    return;
                }
                channel = it->second;
            }
            channel->Remove(host);
            LOG_DEBUG("{}-{} 服务下线节点,进行删除管理",service_name,host);
        }
    private:
        std::string _GetServiceName(const std::string& key)
        {
            size_t pos = key.find_last_of('/');
            return key.substr(0,pos);
        }
    private:
        std::mutex _mtx;
        //关心哪些服务，只处理对应的服务上下线
        std::unordered_set<std::string> _attention_services;
        //服务名称映射服务信道
        std::unordered_map<std::string,ServiceChannel::ptr> _service_to_channel;
    };

}