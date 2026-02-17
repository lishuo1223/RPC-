#include <iostream>
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "user.pb.h"
int main(int argc,char** argv){

    //整个程序启动以后，调用初始化函数来享受rpc调用

    MprpcApplication::Init(argc,argv);

    //演示调用远程发布的rpc方法login
    fix_bug::UserServiceRpc_Stub stub(new MprpcChannel());
    fix_bug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    //rpc方法的响应
    fix_bug::LoginResponse response;
    
    stub.Login(nullptr,&request,&response,nullptr);
    //一次rpc调用的结果
    if(response.result().errcode() == 0){
        std::cout << "rpc login response:" << response.success() << std::endl;
    }else{
        std::cout << "rpc login response error" << response.result().errmsg() << std::endl;
    }

    fix_bug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mprpc");
    req.set_pwd("6666");
    fix_bug::RegisterResponse rsp;
    
    stub.Register(nullptr,&req,&rsp,nullptr);
    if(rsp.result().errcode() == 0){
        std::cout << "rpc register response:" << rsp.success() << std::endl;
    }else{
        std::cout << "rpc register response error" << rsp.result().errmsg() << std::endl;
    }

    return 0;
}