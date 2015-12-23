//
//  Volume.cpp
//  RTSOSCApp
//
//  Created by kannolab1 on 2014/08/22.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#include "Volume.h"
#define MAX_PACKET          44100

Volume::Volume()
{
    for (int i=0; i<128; i++) {
        vTable[i] = powf((double)i/127.0, 2.0);
    }
    
    vVal        = 127;
	vValcpy		= 127;
    vol         = 1.0;
    co1         = 0.8;
    co2         = 0.2;
    wp          = 0;
    rp          = 0;
    env         = (short *)calloc(sizeof(short), MAX_PACKET);
    dB         = (float *)calloc(sizeof(float), MAX_PACKET);
}

void Volume::setVol(int value)
{
    vVal = value;
    vValcpy = vVal;
}

float Volume::getCurrentdB() {
    return dB[wp];
}

void Volume::processAudio(short *inData, short *outData, int fSize)
{
    for(int i=0; i<fSize; i++){

        vol = 0.005*vTable[vVal] + 0.995*vol;
        *outData = *inData++ * vol;
        
        wp = (wp != MAX_PACKET-1 ? wp+1 : 0);
        env[wp] = abs(*outData++);
        if (wp != 0) env[wp] = co1 * env[wp-1] + co2 * env[wp];
        else env[wp] = co1 * env[MAX_PACKET-1] + co2 * env[wp];
        dB[wp] = 20.0*log10((double)env[wp]/32767.0);
    }
}

void Volume::processAudio(float *inData, float *outData, int fSize)
{
    float *ti = inData;
    float *to = outData;
    for(int i=0; i<fSize; i++){
        vol = 0.01*vTable[vVal] + 0.99*vol;
        *to++ = *ti++ * vol;
    }
}

void Volume::muteON()
{
    vVal = 0;
}

void Volume::muteOFF()
{
    vVal = vValcpy;
}