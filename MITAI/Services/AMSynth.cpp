//
//  AMSynth.cpp
//  MITAI
//
//  Created by Musashi Nakajima on 2015/07/26.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "AMSynth.h"

int AMSynth::audio(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    AMSynth *ams = (AMSynth *)user_data;
    lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
    ams->sendAudioTo(dp, lo_blob_datasize(b), 0);
    
    return 0;
}

int AMSynth::level(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    AMSynth *ams = (AMSynth *)user_data;
    if(ams->as.isPlaying) {
        ams->sendDataTo(argv[0]->i, 0, 1);
        if (ams->sent0 == 5) ams->sent0 = 0;
    }
    else {
        if(ams->sent0 != 5) {
            ams->sendDataTo(0, 0, 1);
            ams->sent0++;
        }
    }
    return 0;
}

int AMSynth::playstop(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    AMSynth *ams = (AMSynth *)user_data;
    int v = argv[0]->i;
    if (v < 1) {
        ams->as.isPlaying = false;
        ams->as.location = 0.0;
        ams->as.volume.setVol(0);
    } else {
        ams->as.isPlaying = true;
        ams->as.volume.setVol(110);
    }
    
    return 0;
}

int AMSynth::freq(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    AMSynth *ams = (AMSynth *)user_data;
    ams->saw.freq = ams->saw.fTable[argv[0]->i];
    return 0;
}

AMSynth::AMSynth(Server *s, const char *osc) : Module(s,osc) {
    Module::init(s, osc);
    char addr[64];
    strcpy(addr, OSCAddr);
    strcat(addr, "/AS");
    as.init(s, addr);

    as.prepareAudioSource("Synth.wav");
    
    strcpy(addr, OSCAddr);
    strcat(addr, "/Saw");
    saw.init(s, addr);
    
    addMethodToServer("/Level", "ii", level, this);
    addMethodToServer("/PlayStop", "ii", playstop, this);
    addMethodToServer("/Freq", "ii", freq, this);
    addMethodToServer("/AudioIn", "b", audio, this);
    
    setOutNum(2);
    
    as.connectTo(&saw, "/AudioIn", 0);
    saw.connectTo(this, "/AudioIn", 0);
    saw.connectTo(this, "/Level", 1);
    as.useTimer();
    as.isLooping = true;
    as.isPlaying = true;
    sent0 = 0;
}

void AMSynth::init(Server *s, const char *osc) {   
    // Insert code here to initialize your application
    Module::init(s, osc);
    char addr[64];
    strcpy(addr, OSCAddr);
    strcat(addr, "/AS");
    as.init(s, addr);
    as.isLooping = true;
    as.isPlaying = true;
    as.prepareAudioSource("Synth.wav");

    strcpy(addr, OSCAddr);
    strcat(addr, "/Saw");
    saw.init(s, addr);

    addMethodToServer("/Level", "ii", level, this);
    addMethodToServer("/PlayStop", "ii", playstop, this);
    addMethodToServer("/Freq", "ii", freq, this);
    addMethodToServer("/AudioIn", "b", audio, this);
    setOutNum(2);
    
    as.connectTo(&saw, "/AudioIn", 0);
    saw.connectTo(this, "/AudioIn", 0);
    saw.connectTo(this, "/Level", 1);
    as.useTimer();
}

AMSynth::~AMSynth() {
    as.stopTimer();
    deleteMethodFromServer("/Level", "ii");
    deleteMethodFromServer("/PlayStop", "ii");
    deleteMethodFromServer("/Freq", "ii");
    deleteMethodFromServer("/AudioIn", "b");
}
