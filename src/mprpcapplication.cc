#include "include/mprpcapplication.h"
#include "include/mprpcconfig.h"
#include <cstdlib>
#include <iostream>
#include <sys/unistd.h>

MprpcConfig MprpcApplication::m_config;

void ShowArgsHelp(){
    std::cout << "format: command -i <configfile>" << std::endl;
}
MprpcConfig& MprpcApplication::GetConfig(){
    return m_config;
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
    m_config.LoadConfigFile(configfile.c_str());

    std::cout << "rpcserverip" << m_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport" << m_config.Load("rpcserverport") << std::endl;
    std::cout << "zookeeperip" << m_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport" << m_config.Load("zookeeperport") << std::endl;

}
MprpcApplication& MprpcApplication::GetInstance(){
        static MprpcApplication app;
        return app;
}