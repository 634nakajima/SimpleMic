//
//  Timer.cpp
//  RTSOSCApp
//
//  Created by kannolab1 on 2015/07/10.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "Timer.h"

void Timer::render(PtTimestamp timestamp, void *userData) {
    Timer *t = (Timer *)userData;
    for (int i=0; i!=t->dataCallback.size(); i++) {
        int dI = t->dataInterval[i]*44100.0;
        if((*t->dtmp[i] += t->numPackets) >= dI) {
            *t->dtmp[i] -= dI;
            t->dataCallback[i](t->d_ud[i]);
        }
    }
    for (int i=0; i!=t->audioCallback.size();i++) {
        int dI = t->aInterval[i];
        if((*t->atmp[i] += t->numPackets) >= dI) {
            *t->atmp[i] -= dI;
            t->audioCallback[i](t->a_ud[i]);
        }
    }
}

Timer::Timer() {
    sampleRate      = 44100.0;
    numPackets      = 128;
    audioInterval	= (double)numPackets/sampleRate;
    Pt_Start(audioInterval, this->render, this);

}

void Timer::setAudioCallback(void (*callback)(void *), int interval, void *user_data) {
    audioCallback.push_back(callback);
    a_ud.push_back(user_data);
    aInterval.push_back(interval);
    int *a = new int(0);
    atmp.push_back(a);
}

void Timer::setDataCallback(void (*callback)(void *), float interval, void *user_data) {
    dataCallback.push_back(callback);
    d_ud.push_back(user_data);
    dataInterval.push_back(interval);
    int *d = new int(0);
    dtmp.push_back(d);
}

void Timer::removeAudioCallback(void (*callback)(void *), void *user_data) {
    for (int i=0;i<audioCallback.size();i++) {
        if (audioCallback[i] == *callback && a_ud[i] == user_data) {
            audioCallback.erase(audioCallback.begin()+i);
            a_ud.erase(a_ud.begin()+i);
            aInterval.erase(aInterval.begin()+i);
            delete atmp[i];
            atmp.erase(atmp.begin()+i);
            break;
        }
    }
}

void Timer::removeDataCallback(void (*callback)(void *), void *user_data) {
    for (int i=0;i<dataCallback.size();i++) {
        if (dataCallback[i] == *callback && d_ud[i] == user_data) {
            dataCallback.erase(dataCallback.begin()+i);
            d_ud.erase(d_ud.begin()+i);
            dataInterval.erase(dataInterval.begin()+i);
            delete dtmp[i];
            dtmp.erase(dtmp.begin()+i);
            break;
        }
    }
}

Timer::~Timer() {
    for (int i=0;i<atmp.size();++i) {
        delete atmp[i];
    }
    for (int i=0;i<dtmp.size();++i) {
        delete dtmp[i];
    }
}
