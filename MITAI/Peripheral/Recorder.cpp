//
//  Recorder.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/08/25.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#include "Recorder.h"

Recorder::Recorder()
{
    data = NULL;
    packetSize = 0;
    isRecording = false;
}

int Recorder::prepareAudioSource(const char *sound)
{
	int ifd = -1;
    if (data) {
        free(data);
        data = NULL;
    }
	PSF_PROPS props;
	
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
    
	ifd  = psf_sndOpen(sound,&props,0);
	if(ifd < 0){
		printf("Unable to open infile %s\n",sound);
		return 1;
	}
    
    packetSize = psf_sndSize(ifd);
	if(packetSize <= 0)
		printf("cannot find file size\n");
    
	printf("File size = %ld frames\n",packetSize);
    
	data = (float *) malloc(packetSize * sizeof(float));
	if(data == NULL){
        puts("no memory for frame buffer\n");
	}
    
    psf_sndReadFloatFrames(ifd, data, (unsigned)packetSize);
    psf_sndClose(ifd);

	return 0;
}

void Recorder::pushData(short *inData, int inDataSize)
{
    int sp = (int)packetSize;
    packetSize += inDataSize;
    
    float *tmp = (float *)realloc(data, packetSize*sizeof(float));
    if(tmp == NULL) return;
    data = tmp;
    
    for (int i=sp; i<packetSize; i++) {
        data[i] = inData[i-sp]/32768.0;
    }
}

void Recorder::startRecording()
{
    if (isRecording) return;
    resetData();
    isRecording = true;
    printf("Start Recording\n");
}

void Recorder::stopRecording()
{
    if (!isRecording) return;
    isRecording = false;
    printf("Stop Recording:%d Packets\n", (int)packetSize);
}

void Recorder::resetData()
{
    free(data);
    data = NULL;
    packetSize = 0;
}

Recorder::~Recorder()
{
    if(data)
        free(data);
}
