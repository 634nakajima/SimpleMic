//
//  ADCManager.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/08/13.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__DACManager__
#define __RTSOSCApp__DACManager__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "portaudio.h"
#include "ModuleManager.h"
#include "StreamingBuffer.h"

#define PA_SAMPLE_TYPE      paInt16
#define FRAMES_PER_BUFFER   (64*2)
#define MAX_PACKET          44100

class DAC : public Module
{
public:
    PaStreamParameters outputParameters;
    PaStream *paStream;
    PaError err;
    StreamingBuffer buffer;
    
    bool            isPlaying;
    double          sampleRate;
    short           *buf, *buf2;
    unsigned long   bs,rp,wp,loop;
    float           vol, vTable[128];
    int             vVal, vValcpy;
    int				ch;//L:0 R:1 Stereo:2
    
    DAC(Server *s, const char *osc);
    DAC(){}
    void init(Server *s, const char *osc);
    ~DAC();
    
    int preparePa();
    int start();
    int stop();
    int dBTransform(float dB);
    static void dBTimer(void *userData);
    
    void controlRate(short *inData, short *outData, int inPacketSize, int outPacketSize);
private:
    void initDAC();
    void addMethod();
    void deleteMethod();
    void finishPa();
    
    static int stream(const char   *path,
                      const char   *types,
                      lo_arg       **argv,
                      int          argc,
                      void         *data,
                      void         *user_data);
    
    static int stream2(const char   *path,
                       const char   *types,
                       lo_arg       **argv,
                       int          argc,
                       void         *data,
                       void         *user_data);
    
    static int data1(const char   *path,
                     const char   *types,
                     lo_arg       **argv,
                     int          argc,
                     void         *data,
                     void         *user_data);
    
    static int data2(const char   *path,
                     const char   *types,
                     lo_arg       **argv,
                     int          argc,
                     void         *data,
                     void         *user_data);
    
    static int outputCallback(const void            *inputBuffer,
                              void                  *outputBuffer,
                              unsigned long         framesPerBuffer,
                              const                 PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void                  *userData);
    
};
#endif /* defined(__RTSOSCApp__DACManager__) */
