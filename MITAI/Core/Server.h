//
//  Server.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/26.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Server_h
#define _Server_h

#include "lo.h"
#include "Timer.h"

class Server
{
public:
    lo_server_thread st;
    lo_server_thread st_tcp;
    Server();
    ~Server();
    void setAudioCallback(void (*callback)(void *), int interval, void *user_data);
    void setDataCallback(void (*callback)(void *), float interval, void *user_data);
    void removeAudioCallback(void (*callback)(void *), void *user_data);
    void removeDataCallback(void (*callback)(void *), void *user_data);
private:
    Timer t;
    static void error(int num, const char *m, const char *path);
    static void errorTCP(int num, const char *m, const char *path);

};

#endif
