#include <iostream>

#include "../../comm/etcd.hpp"


int main()
{
    common::init_logger(false,"",spdlog::level::level_enum::debug);
    common::Registry reg("127.0.0.1:2379");
    reg.registry("/service/user/001","127.0.0.1:8080");
    reg.registry("/service/login/001","127.0.0.1:8081");
    reg.registry("/service/user/002","127.0.0.1:8083");

    return 0;
}