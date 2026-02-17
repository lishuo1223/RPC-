#include <iostream>
#include <string>
#include "../friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class FriendService: public fix_bug::FriendServiceRpc {
public:
    std::vector<std::string> GetFriendsList(uint32_t userid){
        std::cout << "do GetFriendsList service!" << userid <<  std::endl;
        std::vector<std::string> vec;

        vec.push_back("gao yang");
        vec.push_back("liu hong");
        vec.push_back("wang shuo");
        return vec;
    }
    void GetFriendsList(::google::protobuf::RpcController* controller,
                       const ::fix_bug::GetFriendsListRequest* request,
                       ::fix_bug::GetFriendsListResponse* response,
                       ::google::protobuf::Closure* done){
                           uint32_t userid =  request->userid();
                           std::vector<std::string> friendsList = GetFriendsList(userid);

                           response->mutable_result()->set_errcode(0);
                           response->mutable_result()->set_errmsg("");
                           for(std::string &name: friendsList){
                               std::string *p = response->add_friends();
                               *p = name;
                           }
                           done->Run();

                       }
private:

};
int main(int argc,char** argv){
    //框架初始化
    MprpcApplication::Init(argc,argv);
    //UserService对象发送到rpc节点上
    RpcProvider provider;
    provider.notifyService(new FriendService());

    //启动一个rpc服务节点,run以后，程序进入阻塞状态，等待远程调用
    provider.Run();
    return 0;
}