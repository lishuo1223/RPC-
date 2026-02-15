#pragma once

#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <unordered_map>
#include <string>
#include <google/protobuf/descriptor.h>

//框架提供的专门服务发布rpc服务的网络对象类
#include <google/protobuf/service.h>
class RpcProvider{
public:
    void notifyService(google::protobuf::Service* service);

    //启动rpc服务节点，
    void Run();
private:
   
    muduo::net::EventLoop m_eventLoop;
    //服务类型信息
    struct ServiceInfo{
        google::protobuf::Service* m_service;  //保存服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*> m_methodMap;  //保存服务方法
    };
    //存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string,ServiceInfo> m_serviceMap;

    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer*,
                   muduo::Timestamp);
    void SendRpcResponse(const muduo::net::TcpConnectionPtr& conn,google::protobuf::Message* msg);
};