//
//  DelayManager.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/09/13.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__DelayManager__
#define __RTSOSCApp__DelayManager__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "portaudio.h"
#include "ModuleManager.h"

class Delay : public Module {
    
public:
    Delay(Server *s, const char *osc);
    Delay(){};
    ~Delay();
    
    float           *buf;
    short           *output;
    unsigned long   dp;
    float           t,g;
    int             ps,maxbs;
    bool            isBypassing;
    
    void init(Server *s, const char *osc);
    void bypass(int data);
    void setDt(int data);
    void setGain(int data);
    
    static int stream(const char   *path,
                      const char   *types,
                      lo_arg       **argv,
                      int          argc,
                      void         *data,
                      void         *user_data);
    
    static int data1(const char   *path,
                     const char   *types,
                     lo_arg       **argv,
                     int          argc,
                     void         *data,
                     void         *user_data);
    
    static int data2(const char   *path,
                     const char   *types,
                     lo_arg       **argv,
                     int          argc,
                     void         *data,
                     void         *user_data);
    
    static int data3(const char   *path,
                     const char   *types,
                     lo_arg       **argv,
                     int          argc,
                     void         *data,
                     void         *user_data);
    
};
#endif /* defined(__RTSOSCApp__DelayManager__) */
