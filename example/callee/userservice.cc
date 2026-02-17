#include <iostream>
#include <string>
#include "../user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class UserService :public fix_bug::UserServiceRpc
{
public:
    bool Login(std::string name,std::string pwd){
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name " << name << "pwd:" << pwd << std::endl;
        return true;
    }
    bool Register(uint32_t id,std::string name,std::string pwd){
        std::cout << "doing local service:Register" << std::endl;
        std::cout << "id " << id << "name " << name << "pwd " << pwd << std::endl;
        return true;
    }
    //重写基类UserServiceRpc的虚函数，下面这些方法都是框架直接调用的
    void Login(::google::protobuf::RpcController* controller,
                       const ::fix_bug::LoginRequest* request,
                       ::fix_bug::LoginResponse* response,
                       ::google::protobuf::Closure* done){
                            //获取参数
                            std::string name = request->name();
                            std::string pwd = request->pwd();
                            //做本地业务
                            bool loginRes = Login(name,pwd);
                            //把响应写入
                            fix_bug::ResultCode* code = response->mutable_result();
                            code->set_errcode(0);
                            code->set_errmsg("");
                            response->set_success(loginRes);
                            //执行回调操作
                            done->Run();
                       }
    void Register(::google::protobuf::RpcController* controller,
                       const ::fix_bug::RegisterRequest* request,
                       ::fix_bug::RegisterResponse* response,
                       ::google::protobuf::Closure* done){
                           uint32_t id = request->id();
                           std::string name = request->name();
                           std::string pwd = request->pwd();
                           bool ret = Register(id,name,pwd);
                           response->mutable_result()->set_errcode(0);
                           response->mutable_result()->set_errmsg("");
                           response->set_success(ret);

                           done->Run();
                       }

};

int main(int argc,char** argv){
    //框架初始化
    MprpcApplication::Init(argc,argv);
    //UserService对象发送到rpc节点上
    RpcProvider provider;
    provider.notifyService(new UserService());

    //启动一个rpc服务节点,run以后，程序进入阻塞状态，等待远程调用
    provider.Run();
    return 0;
}