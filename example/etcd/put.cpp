#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Response.hpp>
#include <string>

int main()
{
    std::string etcd_url = "http://127.0.0.1:2379";
    etcd::Client client(etcd_url);
    etcd::Response resp = client.leasegrant(5).get();
    if(!resp.is_ok()) exit(-1);
    int64_t lease_id = resp.value().lease();
    resp = client.put("/service/user/yyx","shuai",lease_id).get();
    if(!resp.is_ok()) exit(-1);
    
    resp = client.put("/service/friend/little rain","mei").get();
    if(!resp.is_ok()) exit(-1);
    sleep(10);
    client.rm("/service/friend/little rain");
    return 0;
}
