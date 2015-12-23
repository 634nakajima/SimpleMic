//
//  ADC.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _ADC_h
#define _ADC_h

#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include "Module.h"

#define PA_SAMPLE_TYPE      paInt16
#define FRAMES_PER_BUFFER   (64*2)

class ADC : public Module
{
public:
    PaStreamParameters inputParameters, outputParameters;
    PaStream *paStream;
    PaError err;
    short *buf;
    bool isPlaying;
    
    ADC(Server *s, const char *osc);
    ~ADC();
    
private:
    int preparePa();
    int start();
    int stop();
    
    static int inputCallback(const void            *inputBuffer,
							 void                  *outputBuffer,
							 unsigned long         framesPerBuffer,
							 const                 PaStreamCallbackTimeInfo* timeInfo,
							 PaStreamCallbackFlags statusFlags,
							 void                  *userData);
    
    static int stream(const char   *path, 
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
};

#endif
