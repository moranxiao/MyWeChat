#pragma once
#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <etcd/Watcher.hpp>
#include <functional>

#include "logger.hpp"

class Registry{
public:
    Registry(const std::string& host_url)
    :_client(make_shared<etcd::Client>(host_url)),
    _keep_alive(_client->leasekeepalive(3).get()),
    _lease_id(_keep_alive->Lease())
    {}
    bool registry(const std::string& key,const std::string& value)
    {
        auto resp = _client->put(key,value,_lease_id).get();
        if(!resp.is_ok()) 
        {
            LOG_ERROR("{}-{}注册失败:{}",key,value,resp.error_message());
            return false;
        }
        return true;
    }
private:
    std::shared_ptr<etcd::Client> _client;
    std::shared_ptr<etcd::KeepAlive> _keep_alive;
    int64_t _lease_id;
};

class Discovery{
public:
    using notify_callback_t = std::function<void(std::string,std::string)>;
    Discovery(const std::string& host_url, \
    const std::string& base_dir ,\
    const notify_callback_t& put_cb, \
    const notify_callback_t& del_cb)
    :_client(std::make_shared<etcd::Client>(host_url)),
    _put_cb(put_cb),
    _del_cb(del_cb)
    {
        auto resp = _client->ls(base_dir).get();
        if(!resp.is_ok())
        {
            LOG_ERROR("获取服务信息数据失败：{}", resp.error_message());
        }
        else{
            int sz = resp.keys().size();
            for(int i = 0; i < sz; i++)
            {
                if(_put_cb) _put_cb(resp.key(i),resp.value(i).as_string());
            }
        }

        _watcher = std::make_shared<etcd::Watcher>(*(_client.get()), \
        base_dir, \
        std::bind(&Discovery::callback,this,placeholders::_1), \
        true);
    }

private:
    void callback(const etcd::Response& resp)
    {
        if(!resp.is_ok()) 
        {
            LOG_ERROR("收到了一个错误事件:{}", resp.error_message());
            return;
        }
        for(const auto& evt : resp.events())
        {
            if(evt.event_type() == etcd::Event::EventType::DELETE_)
            {
                if(_del_cb) _del_cb(evt.prev_kv().key(), evt.prev_kv().as_string());
                LOG_DEBUG("下线服务：{}-{}", evt.prev_kv().key(), evt.prev_kv().as_string());
            }
            else if(evt.event_type() == etcd::Event::EventType::PUT)
            {
                if (_put_cb) _put_cb(evt.kv().key(), evt.kv().as_string());
                LOG_DEBUG("新增服务：{}-{}", evt.kv().key(), evt.kv().as_string());
            }
            else{

            }
        }
    }
private:
    std::shared_ptr<etcd::Client> _client;
    std::shared_ptr<etcd::Watcher> _watcher;
    notify_callback_t _put_cb;
    notify_callback_t _del_cb;
};