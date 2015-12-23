//
//  AMSynth.h
//  MITAI
//
//  Created by Musashi Nakajima on 2015/07/26.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef __MITAI__AMSynth__
#define __MITAI__AMSynth__

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "ModuleManager.h"
#include "AudioSource.h"
#include "SawAM.h"
#include "DAC.h"
class AMSynth : public Module
{
public:
    AudioSource as;
    SawAM       saw;
    int        sent0;
    AMSynth(){};
    AMSynth(Server *s, const char *osc);
    ~AMSynth();
        
    void init(Server *s, const char *osc);
    static int audio(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int level(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int playstop(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int freq(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
};
#endif /* defined(__MITAI__AMSynth__) */
