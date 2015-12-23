//
//  Module.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Module_h
#define _Module_h

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <math.h>
#include <vector>
#include "lo.h"
#include "RoutingTable.h"
#include "Server.h"
#define SAMPLE_RATE	44100.0

typedef struct Data {
    int value;
    int dataID;
}Data;

class Module
{
public:
    char                IPAddr[16];
    char                OSCAddr[64];
    char                CoIP[16];
    Server              *st;
    RoutingTable        *rTable;
    std::vector<RoutingTable *> rts;
    int					tID, outNum;
    
    Module(Server *s, const char *osc);
    Module();
    ~Module();
    
    void init(Server *s, const char *osc);
    void setTID(int t);
    void setOutNum(int on);
    
    void addMethodToServer(const char *path, const char *type, lo_method_handler h, void *user_data);
    void addMethodToTCPServer(const char *path, const char *type, lo_method_handler h, void *user_data);
    void deleteMethodFromServer(const char *path, const char *type);
    void deleteMethodFromTCPServer(const char *path, const char *type);
    
    void sendSetMdtkn();
    void sendDeleteMdtkn();
    
    void connectTo(Module *m, const char *input, int outID);
    void disconnectFrom(Module *m, const char *input, int outID);
    
    void addRoute(char *ip, char *osc, int outID);
    void deleteRoute(char *ip, char *osc, int outID);
    
    void sendDataTo(int value, int outID);
    void sendDataTo(float value, int outID);
    void sendDataTo(int value, int dataID, int outID);
    void sendDataTo(float value, int dataID, int outID);
    void sendAudioTo(short *a, unsigned long l, int outID);

    char* getSenderIP();
    char* getSenderTCPIP();
    void setCoIP();
    
private:
    void setOSCAddr(const char *osc);
    
    void module_send_b(lo_blob b, lo_address lo_ip, const char *osc);
    void module_send_f(float value, int dataID, lo_address lo_ip, const char *osc);
    void module_send_i(int value, int dataID, lo_address lo_ip, const char *osc);
    
    static int addRoute(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int deleteRoute(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
};

#endif
