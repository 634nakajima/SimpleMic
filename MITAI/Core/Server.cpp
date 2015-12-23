//
//  Server.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/10/26.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "Server.h"
#include <stdio.h>

void Server::error(int num, const char *msg, const char *path) {
    printf("liblo UDP server error %d in path %s: %s\n", num, path, msg);
}

void Server::errorTCP(int num, const char *msg, const char *path) {
    printf("liblo TCP server error %d in path %s: %s\n", num, path, msg);
}

Server::Server() {
    st = lo_server_thread_new("6340", error);
    if (st) lo_server_thread_start(st);
    else { printf("err: starting Server\n"); while(1);}
    
    st_tcp = lo_server_thread_new_with_proto("6341", LO_TCP, errorTCP);
    if (st_tcp) lo_server_thread_start(st_tcp);
    else { printf("err: starting Server\n"); while(1);}
}

Server::~Server() {
    lo_server_thread_free(st);
    lo_server_thread_free(st_tcp);
}

void Server::setAudioCallback(void (*callback)(void *), int interval, void *user_data) {
    t.setAudioCallback(callback, interval, user_data);
}

void Server::setDataCallback(void (*callback)(void *), float interval, void *user_data) {
    t.setDataCallback(callback, interval, user_data);
}

void Server::removeAudioCallback(void (*callback)(void *), void *user_data) {
    t.removeAudioCallback(callback, user_data);
}

void Server::removeDataCallback(void (*callback)(void *), void *user_data) {
    t.removeDataCallback(callback, user_data);
}