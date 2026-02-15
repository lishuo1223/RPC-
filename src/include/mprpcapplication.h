#pragma once

//框架的基础类
#include "mprpcconfig.h"
class MprpcApplication{
public:
    static void Init(int argc,char ** argv);
    static MprpcApplication& GetInstance();
    static MprpcConfig& GetConfig();
private:
    static MprpcConfig m_config;
    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&)=delete;
};