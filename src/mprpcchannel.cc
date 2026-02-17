#include <sys/types.h>
#include <sys/socket.h>
#include <error.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "mprpcchannel.h"
#include <string>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "rpcheader.pb.h"
#include "mprpcapplication.h"
#include <unistd.h>

using namespace google::protobuf;

void MprpcChannel::CallMethod(const MethodDescriptor* method,
                          RpcController* controller, const Message* request,
                          Message* response, Closure* done){
                               const google::protobuf::ServiceDescriptor* sd = method->service();
                               std::string service_name = sd->name();
                               std::string method_name = method->name();
                               //获取参数的序列化长度字符串 args_size
                               std::string args_str;
                               uint32_t args_size = 0;
                               if(request->SerializeToString(&args_str)){
                                   args_size = args_str.size();
                               }else{
                                   std::cout << "SerializeToString request error " <<args_str << std::endl;
                                   controller->SetFailed("serialize request error!");
                                   return;
                               }
                               mprpc::RpcHeader rpcHeader;
                               rpcHeader.set_service_name(service_name);
                               rpcHeader.set_method_name(method_name);
                               rpcHeader.set_args_size(args_size);
                               uint32_t header_size = 0;
                               std::string rpc_header_str;
                               if(rpcHeader.SerializeToString(&rpc_header_str)){
                                    header_size = rpc_header_str.size();
                               }else{
                                    std::cout << "Serialize rpc header error!" << std::endl;
                                    return;
                               }
                                //组织待发送的rpc请求的字符串
                                std::string send_rpc_str;
                                send_rpc_str.insert(0,std::string((char*)&header_size,4));
                                send_rpc_str += rpc_header_str; //rpc_header
                                send_rpc_str += args_str;  //args

                                
                                std::cout << "===============" << std::endl;
                                std::cout << "header_size: "<< header_size << std::endl;
                                std::cout << "rpc_header_str: "<< rpc_header_str << std::endl;
                                std::cout << "service_name:" << service_name << std::endl;
                                std::cout << "method_name:" << method_name << std::endl;
                                std::cout << "args_str:" << args_str << std::endl;
                                std::cout << "==================" << std::endl;

                                //使用Tcp编程，完成rpc方法的调用
                                int clientfd = socket(AF_INET,SOCK_STREAM,0);
                                if(clientfd == -1){
                                    
                                    char errtxt[512] = {0};
                                    sprintf(errtxt,"create socket error! errno:%d",errno);
                                    controller->SetFailed(errtxt);
                                    exit(EXIT_FAILURE);
                                }
                                std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
                                u_int16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
                                struct sockaddr_in server_addr;
                                server_addr.sin_family = AF_INET;
                                server_addr.sin_port = htons(port);
                                server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

                                if(connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
                                    char errtxt[512] = {0};
                                    sprintf(errtxt,"connect error! errno:%d",errno);
                                    controller->SetFailed(errtxt);
                                    close(clientfd);
                                    return;
                                    exit(EXIT_FAILURE);
                                }
                                //发送rpc请求
                                if(-1 == send(clientfd,send_rpc_str.c_str(),send_rpc_str.size(),0)){
                                    
                                    char errtxt[512] = {0};
                                    sprintf(errtxt,"send error! errno:%d",errno);
                                    controller->SetFailed(errtxt);
                                    close(clientfd);
                                    return;
                                }
                                //接受rpc请求的响应值
                                char recv_buf[1024] = {0};
                                int recv_size = 0;
                                if(-1 ==(recv_size = recv(clientfd,recv_buf, 1024,0))){
                                    
                                    char errtxt[512] = {0};
                                    sprintf(errtxt,"send error! errno:%s",recv_buf);
                                    controller->SetFailed(errtxt);
                                    close(clientfd);
                                    return;
                                }   
                                std::string response_str(recv_buf,0,recv_size);
                                if(!response->ParseFromArray(recv_buf,recv_size)){
                                    std::cout << "parse error! response_str:" << recv_buf << std::endl;
                                    close(clientfd);
                                    return;
                                }
                                
                                close(clientfd);

                          }