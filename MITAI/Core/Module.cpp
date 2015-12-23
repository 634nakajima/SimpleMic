//
//  Module.cpp
//  System1
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "Module.h"

void Module::setTID(int t)
{
    tID = t;
}

void Module::setOutNum(int on) {
    outNum = on;
    for(int i=1;i<outNum;i++) {
        RoutingTable *r = new RoutingTable();
        r->aNum = 0;
        rts.push_back(r);
    }
}

int Module::addRoute(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data)
{
    Module *mod = (Module *)user_data;
    mod->addRoute((char *)argv[0], (char *)argv[1], argv[2]->i);
    return 0;
}

int Module::deleteRoute(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data)
{
    Module *mod = (Module *)user_data;
    mod->deleteRoute((char *)argv[0], (char *)argv[1], argv[2]->i);
    return 0;
}

Module::Module(){}

Module::Module(Server *s, const char *osc) {
    init(s, osc);
}

void Module::init(Server *s, const char *osc) {
    st = s;
    setOSCAddr(osc);
    rTable = new RoutingTable();
    rTable->aNum = 0;
    rts.push_back(rTable);
    tID = -1;
    outNum = 1;
    
    addMethodToTCPServer("/AddRoute", "ss", Module::addRoute, this);
    addMethodToTCPServer("/AddRoute", "ssi", Module::addRoute, this);
    addMethodToTCPServer("/DeleteRoute", "ss", Module::deleteRoute, this);
    addMethodToTCPServer("/DeleteRoute", "ssi", Module::deleteRoute, this);
}

void Module::sendSetMdtkn() {
    lo_address addr = lo_address_new_with_proto(LO_TCP, CoIP, "6341");
    lo_send(addr,
            "/Coordinator/SetMdtkn",
            "si",
            OSCAddr,
            tID);
    lo_address_free(addr);
}

void Module::sendDeleteMdtkn() {
    lo_address addr = lo_address_new_with_proto(LO_TCP, CoIP, "6341");
    lo_send(addr,
            "/Coordinator/DeleteMdtkn",
            "si",
            OSCAddr,
            tID);
    lo_address_free(addr);
}

void Module::module_send_b(lo_blob b, lo_address lo_ip, const char *osc) {
    void *data;
    unsigned long d_len;

    lo_message m = lo_message_new();
    lo_message_add_blob(m, b);
    data = lo_message_serialise(m, osc, NULL, NULL);
    d_len = lo_message_length(m, osc);

    if (strcmp("localhost",lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st), data, d_len);
    }else {
        lo_send_message(lo_ip, osc, m);
    }

    lo_message_free(m);
    free(data);
}

void Module::module_send_i(int value, int dataID, lo_address lo_ip, const char *osc) {
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_int32(m, value);
    lo_message_add_int32(m, dataID);
    data = lo_message_serialise(m, osc, NULL, NULL);
    d_len = lo_message_length(m, osc);
    
    if (strcmp("localhost",lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st), data, d_len);
    }else {
        lo_send_message(lo_ip, osc, m);
    }
    
    lo_message_free(m);
    free(data);
}

void Module::module_send_f(float value, int dataID, lo_address lo_ip, const char *osc) {
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_float(m, value);
    lo_message_add_int32(m, dataID);
    data = lo_message_serialise(m, osc, NULL, NULL);
    d_len = lo_message_length(m, osc);
    
    if (strcmp("localhost",lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st), data, d_len);
    }else {
        lo_send_message(lo_ip, osc, m);
    }
    
    lo_message_free(m);
    free(data);
}

void Module::sendDataTo(int value, int outID) {
    sendDataTo(value, 0, outID);
}

void Module::sendDataTo(int value, int dataID, int outID) {
    if(outNum <= outID) {printf("too many outID!\n"); return;}
    
    for (int i=0; i<rts[outID]->aNum; i++) {
        module_send_i(value, dataID, rts[outID]->loAddr[i], rts[outID]->oscAddr[i]);
    }
}

void Module::sendDataTo(float value, int outID) {
    sendDataTo(value, 0, outID);
}

void Module::sendDataTo(float value, int dataID, int outID) {
    for (int i=0; i<rts[outID]->aNum; i++) {
        module_send_f(value, dataID, rts[outID]->loAddr[i], rts[outID]->oscAddr[i]);
    }
}

void Module::sendAudioTo(short *a, unsigned long l, int outID) {
    lo_blob b = lo_blob_new((int)l, a);
    for (int i=0; i<rts[outID]->aNum; i++)
        module_send_b(b, rts[outID]->loAddr[i], rts[outID]->oscAddr[i]);
    lo_blob_free(b);
}

void Module::setOSCAddr(const char *osc) {
    strcpy(OSCAddr, osc);
}

void Module::addMethodToServer(const char *path, const char *type, lo_method_handler h, void *user_data) {
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_add_method(st->st, p, type, h, user_data);
}

void Module::addMethodToTCPServer(const char *path, const char *type, lo_method_handler h, void *user_data) {
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_add_method(st->st_tcp, p, type, h, user_data);
}

void Module::deleteMethodFromServer(const char *path, const char *type) {
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_del_method(st->st, p, type);
}

void Module::deleteMethodFromTCPServer(const char *path, const char *type) {
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_del_method(st->st_tcp, p, type);
}

void Module::addRoute(char *ip, char *osc, int outID) {
    rts[outID]->setRoute(ip, osc);
}

void Module::deleteRoute(char *ip,char *osc, int outID) {
    rts[outID]->deleteRoute(ip, osc);
}

void Module::connectTo(Module *m, const char *input, int outID) {
    char p[64];
    strcpy(p, m->OSCAddr);
    strcat(p, input);
    char ip[10] = "localhost";
    addRoute(ip, p, outID);
}

void Module::disconnectFrom(Module *m, const char *input, int outID) {
    char p[64];
    strcpy(p, m->OSCAddr);
    strcat(p, input);
    char ip[10] = "localhost";
    deleteRoute(ip, p, outID);
}

char* Module::getSenderIP() {
    lo_server_thread lst = st->st;
    lo_server ls = lo_server_thread_get_server(lst);
    struct sockaddr_in *sin = (struct sockaddr_in *)lo_server_get_addr(ls);
    
    return inet_ntoa(sin->sin_addr);
}

char* Module::getSenderTCPIP() {
    lo_server_thread lst = st->st_tcp;
    lo_server ls = lo_server_thread_get_server(lst);
    struct sockaddr_in *sin = (struct sockaddr_in *)lo_server_get_addr(ls);

    return inet_ntoa(sin->sin_addr);
}

void Module::setCoIP() {
    strcpy(CoIP, getSenderTCPIP());
}

Module::~Module() {
	deleteMethodFromTCPServer("/AddRoute", "ss");
    deleteMethodFromTCPServer("/AddRoute", "ssi");
    deleteMethodFromTCPServer("/DeleteRoute", "ss");
    deleteMethodFromTCPServer("/DeleteRoute", "ssi");

    auto it = rts.begin();
    while(it != rts.end()) {
        RoutingTable *r = *it;
        delete r;
        it = rts.erase(it);
    }
}