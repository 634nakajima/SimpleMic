//
//  AudioMixer.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/11.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__AudioMixer__
#define __RTSOSCApp__AudioMixer__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "ModuleManager.h"
#include "StreamingBuffer.h"

#define MAX_PACKET          44100

class AudioMixer : public Module
{
public:
    std::vector<StreamingBuffer> bufList;
    short *buf;
    
    AudioMixer(Server *s, const char *osc);
    AudioMixer(){}
    ~AudioMixer();

    void init(Server *s, const char *osc);
    void setInputNum(int n);

private:
    static int stream(const char   *path,
                      const char   *types,
                      lo_arg       **argv,
                      int          argc,
                      void         *data,
                      void         *user_data);
};

#endif /* defined(__RTSOSCApp__AudioMixer__) */
