//
//  PlaybackRate.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/08/23.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__StreamingBuffer__
#define __RTSOSCApp__StreamingBuffer__

#include <iostream>
#include "Volume.h"

class StreamingBuffer
{
public:
    float           rate, rp, co1, co2, *dB;
    short           *bufL, *bufR, *env;
    unsigned long   bs,wp;
    bool isPlaying;
    int loop;
    Volume volume;
    
    StreamingBuffer();
    ~StreamingBuffer();
    void pushMono(short *inData, int inPacketSize);
    void pushSt(short *lData, short *rData, int inPacketSize);
    void pop(short *outData, int outPacketSize);
    void popMono(short *outData, int outPacketSize);
    float getCurrentdB();
};
#endif /* defined(__RTSOSCApp__StreamingBuffer__) */
