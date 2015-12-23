//
//  AudioSourceManager.h
//  RTSOSCApp
//
//  Created by kannolab1 on 2014/08/22.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__AudioSource__
#define __RTSOSCApp__AudioSource__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "portaudio.h"
#include "ModuleManager.h"
#include "Volume.h"
#include "Recorder.h"
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

class AudioSource : public Module {
    
public:
    float		rate;
    double		location;
    double		interval;
    double		sampleRate;
    unsigned	numPackets;
    float		*buf;
    short		*output;
    bool		isPlaying, isLooping, isUsingTimer;
    Volume      volume;
    Recorder    rec;
    AudioSource(){}
    AudioSource(Server *s, const char *osc);
    ~AudioSource();
    
    void init(Server *s, const char *osc);
    static int audio(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int data1(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int data2(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int data3(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int data4(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    static int fdata(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    
    int			prepareAudioSource(const char *sound);
    void		initAudioInfo();
    static void render(void *userData);
    void        useTimer();
    void        stopTimer();
};

class AudioSourceManager : public ModuleManager<AudioSource>
{
public:
    char    file[64];

    AudioSourceManager(){};
    ~AudioSourceManager(){};

    void init(Server *s, const char *osc);
    void init(Server *s, const char *osc, const char *coAddr);
    void setFileName(const char *fname);
    AudioSource *initModule(Server *s, const char *osc);
};

#endif /* defined(__RTSOSCApp__AudioSourceManager__) */
