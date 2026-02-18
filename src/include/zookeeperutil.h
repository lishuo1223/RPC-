#pragma once

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

class ZkClient{
public:
    ZkClient();
    ~ZkClient();
    void start();
    //在zkserver上根据指定的path创建znode节点
    void create(const char* path,const char* data,int datalen,int state=0);
    //根据参数指定的znode节点路径，或者znode节点的值
    std::string GetData(const char* path);
private:
    //zk客户端句柄
    zhandle_t* m_zhandle;
};