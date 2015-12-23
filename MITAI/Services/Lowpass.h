//
//  Lowpass.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/13.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__Lowpass__
#define __RTSOSCApp__Lowpass__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "portaudio.h"
#include "ModuleManager.h"

class Lowpass : public Module {
    
public:
    Lowpass(Server *s, const char *osc);
    Lowpass(){};
    ~Lowpass();
    
    float freq, omega, alpha, q;
    float a0, a1, a2;
    float b0, b1, b2;
    float in1, in2, out1, out2;
    short *output;
    bool  isBypassing;

    void init(Server *s, const char *osc);
    void calcCoef();
    void bypass(int data);
    void setFreq(int data);
    void setQ(int data);
    
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
#endif /* defined(__RTSOSCApp__Lowpass__) */
