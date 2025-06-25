#include "../../comm/etcd.hpp"
#include <thread>

int main()
{
    init_logger(false,"",spdlog::level::level_enum::trace);

    Registry reg("http://127.0.0.1:2379");
    reg.registry("/service/user","127.0.0.1");
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}