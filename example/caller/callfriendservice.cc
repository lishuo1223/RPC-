#include <iostream>
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "friend.pb.h"
#include "mprpccontroller.h"
int main(int argc,char** argv){

    //整个程序启动以后，调用初始化函数来享受rpc调用

    MprpcApplication::Init(argc,argv);

    //演示调用远程发布的rpc方法login
    fix_bug::FriendServiceRpc_Stub stub(new MprpcChannel());
    fix_bug::GetFriendsListRequest request;
    request.set_userid(1000);
    //rpc方法的响应
    fix_bug::GetFriendsListResponse response;
    MprpcController controller;
    stub.GetFriendsList(&controller,&request,&response,nullptr);
    //一次rpc调用的结果
    if(controller.Failed()){
        std::cout << controller.ErrorText() << std::endl;
    }else{
       if(response.result().errcode() == 0){
        std::cout << "rpc GetFriendsList response:"  << std::endl;
        int size = response.friends_size();
        for(int i = 0; i < size; i ++){
            std::cout << "index:" << (i + 1) << "name :" << response.friends(i) << std::endl;
        }
    }else{
        std::cout << "rpc GetFriendsList response error" << response.result().errmsg() << std::endl;
    }
    }
    return 0;
}