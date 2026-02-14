#pragma once

//框架提供的专门服务发布rpc服务的网络对象类
#include <google/protobuf/service.h>
class RpcProvider{
public:
    void notifyService(google::protobuf::Service* service);

    //启动rpc服务节点，
    void Run();
private:
};