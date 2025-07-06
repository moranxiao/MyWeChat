#include <iostream>

#include "../../comm/etcd.hpp"
#include "../../comm/channel.hpp"

int main()
{
    common::init_logger(false,"",spdlog::level::level_enum::debug);
    common::ServiceManager service_manager;
    service_manager.AddAttenion("/service/usr/");
    common::Discovery disc("127.0.0.1:2379", \
        "/service/usr/",\
        std::bind(&common::ServiceManager::ServiceOnlineCallback,&service_manager,placeholders::_1,placeholders::_2), \
        std::bind(&common::ServiceManager::ServiceOfflineCallback,&service_manager,placeholders::_1,placeholders::_2));
    std::this_thread::sleep_for(std::chrono::seconds(100));
    return 0;
}