//
//  Recorder.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/08/25.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__Recorder__
#define __RTSOSCApp__Recorder__

#include <iostream>
#include "portsf.h"

class Recorder
{
public:
    float *data;
    long packetSize;
    bool isRecording;
    
    Recorder();
    ~Recorder();
    int prepareAudioSource(const char *sound);
    void pushData(short *inData, int inDataSize);
    void startRecording();
    void stopRecording();
    void resetData();

};
#endif /* defined(__RTSOSCApp__Recorder__) */
