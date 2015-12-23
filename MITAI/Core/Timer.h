//
//  Timer.h
//  RTSOSCApp
//
//  Created by kannolab1 on 2015/07/10.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__Timer__
#define __RTSOSCApp__Timer__

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif

typedef void (*timerCallback)(void *);

class Timer {
public:
    Timer();
    ~Timer();

    void setAudioCallback(void (*callback)(void *), int interavl, void *user_data);
    void removeAudioCallback(void (*callback)(void *), void *user_data);
    void setDataCallback(void (*callback)(void *), float interavl, void *user_data);
    void removeDataCallback(void (*callback)(void *), void *user_data);
    
private:
    double      audioInterval;
    double		sampleRate;
    unsigned	numPackets;
    std::vector<timerCallback> audioCallback, dataCallback;
    std::vector<void *> a_ud, d_ud;
    std::vector<float> dataInterval;
    std::vector<int *> dtmp, atmp;
    std::vector<int> aInterval;

    static void render(PtTimestamp timestamp, void *userData);
};

#endif /* defined(__RTSOSCApp__Timer__) */
