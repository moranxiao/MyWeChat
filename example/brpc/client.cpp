#include <brpc/channel.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "main.pb.h"

void callback(brpc::Controller* cntl, example::EchoResponse* resp)
{
    std::cout << resp->message() << std::endl;
    delete cntl;
    delete resp;
}


int main()
{
    brpc::ChannelOptions options;
    options.timeout_ms = -1; //连接超时时间
    options.max_retry = 3; //最大重试次数
    options.protocol = "baidu_std"; //序列化协议
    brpc::Channel channel;
    if(channel.Init("127.0.0.1:8000", &options) != 0) {
        std::cout << "初始化信道失败" << std::endl; 
        return -1;
    }
    example::EchoService_Stub stub(&channel);
    example::EchoRequest req;
    req.set_message("hello rpc");
    
    brpc::Controller* cntl = new brpc::Controller();
    example::EchoResponse* res = new example::EchoResponse();

    // 同步调用
    // stub.Echo(cntl, &req, res, nullptr);
    // if(cntl->Failed()) {
    //     std::cout << "RPC调用失败: " << cntl->ErrorText() << std::endl;
    // } else {
    //     std::cout << "RPC调用成功: " << res->message() << std::endl;
    // }
    // std::cout << "收到响应：" << res->message() << std::endl;
    // delete cntl;
    // delete res;

    // 异步调用
    ::google::protobuf::Closure* done = ::google::protobuf::NewCallback(callback,cntl,res);
    stub.Echo(cntl, &req, res, done);
    std::this_thread::sleep_for(std::chrono::seconds(3)); // 等待异步调用完成
    return 0;
}