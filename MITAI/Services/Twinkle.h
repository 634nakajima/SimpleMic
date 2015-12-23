//
//  Twinkle.h
//  RTSOSCApp
//
//  Created by kannolab1 on 2015/04/18.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__Twinkle__
#define __RTSOSCApp__Twinkle__

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "ModuleManager.h" 
#include "AudioSource.h"
#include "DAC.h"
#include "Serial.h"

class Twinkle : public Module
{
public:
    AudioSource as[6];
    
    int time, loop, tmp;
    int tw[48] = {0,0,4,4,5,5,4,6,3,3,2,2,1,1,0,6,4,4,3,3,2,2,1,6,4,4,3,3,2,2,1,6,0,0,4,4,5,5,4,6,3,3,2,2,1,1,0,6};

    Twinkle(){}
    Twinkle(Server *s, const char *osc);
    ~Twinkle();
    
    void init(Server *s, const char *osc);
    static int step(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int audio(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int stst(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static void step(void *user_data);

};

#endif /* defined(__RTSOSCApp__Twinkle__) */
