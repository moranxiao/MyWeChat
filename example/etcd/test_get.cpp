#include "../../comm/etcd.hpp"
#include <thread>

void put_cb(const std::string& key,const std::string& value)
{
    std::cout << "put-" << key << ":" << value << std::endl;
}
void del_cb(const std::string& key,const std::string& value)
{
    std::cout << "del-" <<  key << ":" << value << std::endl;
    
}
int main()
{
    common::init_logger(false,"",spdlog::level::level_enum::trace);
    common::Discovery disc("http://127.0.0.1:2379","/service", put_cb,del_cb);
    while(true);
    return 0;
}