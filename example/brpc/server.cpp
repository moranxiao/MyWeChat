#include <brpc/server.h>

#include "main.pb.h"

class EchoServiceImp : public example::EchoService{
public:
    EchoServiceImp()
    {}
    ~EchoServiceImp()
    {}
    virtual void Echo(google::protobuf::RpcController* controller,
                       const ::example::EchoRequest* request,
                       ::example::EchoResponse* response,
                       ::google::protobuf::Closure* done)
    {
        brpc::ClosureGuard done_guard(done);
        response->set_message(request->message()+"----this is server");
    }
};

int main()
{
    EchoServiceImp echo;
    brpc::Server server;
    brpc::ServerOptions options;
    options.num_threads = 4;
    //SERVER_DOESNT_OWN_SERVICE是指示brpc服务器不拥有服务对象的生命周期，
    server.AddService(&echo, brpc::SERVER_DOESNT_OWN_SERVICE);

    if (server.Start(8000, &options) != 0) {
        LOG(ERROR) << "Fail to start EchoService";
        return -1;
    }
    // 等待直到收到退出信号
    server.RunUntilAskedToQuit();
    
    return 0;
}