//
//  AudioMixer.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/11.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "AudioMixer.h"

int AudioMixer::stream(const char   *path,
                const char   *types,
                lo_arg       **argv,
                int          argc,
                void         *data,
                void         *user_data)
{
    AudioMixer *am = (AudioMixer *)user_data;
    
    lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
    //int size = lo_blob_datasize(b)/sizeof(short);
    //int d = (int)am->wp - (int)am->rp;
    //if (d < 0)  d = d + MAX_PACKET;
    
    am->sendAudioTo(dp, lo_blob_datasize(b), 0);
    
    return 0;
}

AudioMixer::AudioMixer(Server *s, const char *osc) : Module(s, osc)
{
    AudioMixer::Module::init(s, osc);
}

void AudioMixer::init(Server *s, const char *osc) {
    Module::init(s, osc);
    buf = (short *)calloc(sizeof(short), MAX_PACKET);
    setInputNum(2);
}

void AudioMixer::setInputNum(int n) {
    for (int i=0;i<n;i++) {
        bufList[i] = StreamingBuffer();
        addMethodToServer("/Audio", "b", stream, &bufList[i]);
    }
}

AudioMixer::~AudioMixer()
{
    deleteMethodFromServer("/Audio", "b");
}