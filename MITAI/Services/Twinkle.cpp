//
//  Twinkle.cpp
//  RTSOSCApp
//
//  Created by kannolab1 on 2015/04/18.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "Twinkle.h"

void Twinkle::step(void *user_data) {
    Twinkle *t = (Twinkle *)user_data;
    if(t->loop == 0) return;
    else {
        t->tmp++;
        if(t->tmp >= t->loop) t->tmp = 0;
        if (t->tmp == 0) {
            if(-1 < t->time && t->time < 48 && t->tw[t->time] != 6) {
                t->as[t->tw[t->time]].location = 0;
                t->as[t->tw[t->time]].isPlaying = true;
            }
            t->time = (t->time+1)%48;
        }
    }
}

int Twinkle::stst(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    Twinkle *t = (Twinkle *)user_data;
    if(argv[0]->i > 0) {
        t->loop = 80;
    }else {
        t->loop = 0;
        //t->time = 0;
    }
    return 0;
}

int Twinkle::step(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    Twinkle *t = (Twinkle *)user_data;
    t->loop = (280 - argv[0]->i)/2;
    if (argv[0]->i == 0) t->loop = 0;
    return 0;
}

int Twinkle::audio(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    Twinkle *tw = (Twinkle *)user_data;
    lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
    tw->sendAudioTo(dp, lo_blob_datasize(b), 0);

    return 0;
}

Twinkle::Twinkle(Server *s, const char *osc) : Module(s,osc) {
    Twinkle::init(s, osc);
}

void Twinkle::init(Server *s, const char *osc) {
    Module::init(s, osc);
    time = 0;
    loop = 0;
    tmp = 0;
    // Insert code here to initialize your application
    char addr[64];
    strcpy(addr, OSCAddr);
    strcat(addr, "/ASA");
    as[0].init(s, addr);
    as[0].prepareAudioSource("C.wav");

    strcpy(addr, OSCAddr);
    strcat(addr, "/ASB");
    as[1].init(s, addr);
    as[1].prepareAudioSource("D.wav");

    strcpy(addr, OSCAddr);
    strcat(addr, "/ASC");
    as[2].init(s, addr);
    as[2].prepareAudioSource("E.wav");

    strcpy(addr, OSCAddr);
    strcat(addr, "/ASD");
    as[3].init(s, addr);
    as[3].prepareAudioSource("F.wav");

    strcpy(addr, OSCAddr);
    strcat(addr, "/ASE");
    as[4].init(s, addr);
    as[4].prepareAudioSource("G.wav");

    strcpy(addr, OSCAddr);
    strcat(addr, "/ASF");
    as[5].init(s, addr);
    as[5].prepareAudioSource("A.wav");

    for (int i=0; i<6; i++) {
        as[i].isPlaying = false;
        as[i].isLooping = false;
    }
    st->setDataCallback(step, 0.01, this);

    for (int i=0; i<5; i++) {
        as[i].connectTo(&as[i+1], "/AudioIn", 0);

    }
    as[5].connectTo(this, "/AudioIn", 0);
    
    addMethodToServer("/Tempo", "ii", step, this);
    addMethodToServer("/StartStop", "ii", stst, this);
    addMethodToServer("/AudioIn", "b", audio, this);
    as[0].useTimer();

}

Twinkle::~Twinkle() {
    st->removeDataCallback(step, this);
    for (int i=0; i<5; i++) {
        as[i].disconnectFrom(&as[i+1], "/AudioIn", 0);
    }
    as[5].disconnectFrom(this, "/AudioIn", 0);
    as[0].stopTimer();
    deleteMethodFromServer("/Tempo", "ii");
    deleteMethodFromServer("/StartStop", "ii");
    deleteMethodFromServer("/AudioIn", "b");
}
