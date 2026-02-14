#include "include/mprpcapplication.h"
#include <cstdlib>
#include <iostream>
#include <sys/unistd.h>

void ShowArgsHelp(){
    std::cout << "format: command -i <configfile>" << std::endl;
}
void MprpcApplication::Init(int argc,char ** argv){
    if(argc < 2){
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
    std::string configfile;
    int c = 0;
    while((c = getopt(argc,argv,"i:")) != -1){
        switch (c) {
        case 'i':
            configfile = optarg;
            break;
        
        case '?':
            std::cout << "invalid args!" << std::endl;
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
    //开始加载配置文件  rpcserver_ip =    rpc_server_port  zookeeper_ip

}
MprpcApplication& MprpcApplication::GetInstance(){
        static MprpcApplication app;
        return app;
}