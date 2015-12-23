//
//  Volume.h
//  RTSOSCApp
//
//  Created by kannolab1 on 2014/08/22.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__Volume__
#define __RTSOSCApp__Volume__

#include <iostream>
#include <math.h>
class Volume
{
public:
    Volume();
    ~Volume(){};
    
    void setVol(int value);
    void processAudio(short *inData, short *outData, int fSize);
    void processAudio(float *inData, float *outData, int fSize);
    void muteON();
    void muteOFF();
    float getCurrentdB();
private:
    float vol, vTable[128], co1, co2, *dB;
    int vVal, vValcpy, wp, rp;
    short           *env;
};

#endif /* defined(__RTSOSCApp__Volume__) */
