//
//  PlaybackRate.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/08/23.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#include "StreamingBuffer.h"

#define MAX_PACKET          44100

StreamingBuffer::StreamingBuffer()
{
    rate        = 1.0;
    rp          = 0.0;
    wp          = 0;
    bs          = 32*16;
    loop        = 0;
    co1         = 0.7;
    co2         = 0.3;
    isPlaying   = false;
    bufL        = (short *)calloc(sizeof(short), MAX_PACKET);
    bufR        = (short *)calloc(sizeof(short), MAX_PACKET);
    env         = (short *)calloc(sizeof(short), MAX_PACKET);
    dB         = (float *)calloc(sizeof(float), MAX_PACKET);
}

void StreamingBuffer::pushMono(short *inData, int inPacketSize)
{
    float d = (float)wp - rp;
    if (d < 0)  d = d + MAX_PACKET;

    for(int i=0; i<inPacketSize; i++){
        bufL[wp] = *inData++;
        bufR[wp] = bufL[wp];
        wp = (wp != MAX_PACKET-1 ? wp+1 : 0);
    }
    
    if (d+inPacketSize > bs && !isPlaying) {
        volume.muteOFF();
        isPlaying = true;
        printf("start\n");
    }
}

float StreamingBuffer::getCurrentdB() {
    return volume.getCurrentdB();
}

void StreamingBuffer::pushSt(short *lData, short *rData, int inPacketSize)
{
    float d = (float)wp - rp;
    if (d < 0)  d = d + MAX_PACKET;

    for(int i=0; i<inPacketSize; i++){
        bufL[wp] = *lData++;
        bufR[wp] = *rData++;
        wp = (wp != MAX_PACKET-1 ? wp+1 : 0);
    }
    if (d > bs*2 && !isPlaying) {
        volume.muteOFF();
        isPlaying = true;
        printf("start\n");
    }
}

void StreamingBuffer::pop(short *outData, int outPacketSize)
{
    float a, b, d, r;
    int	l, next;
    
    d = (float)wp - rp;
    if (d < 0)  d = d + MAX_PACKET;
    r  = d/(float)bs;
    if (loop++==1000) {
        //printf("Buffer:%f,%d\n", rate,(int)bs);
        loop = 0;
    }

    if(r>1.5) {
       r = 1.01;
        //printf("1.01\n");
    }else if(r<0.9) {
        r = 0.99;
        //printf("0.99\n");
    }
    else r = 1.0;
    //r = 1.0;
    for (int i=0; i<outPacketSize; i++) {
        l = rp;
        b = rp - (unsigned)rp;
        a = 1.0-b;
        
        if (l < MAX_PACKET-1) next = l+1;
        else next = l - MAX_PACKET + 1;

        outData[2*i] = 0;
        outData[2*i+1] = 0;

        if (isPlaying) {
            if (d > 0) {
                outData[2*i] = a*bufL[l]+b*bufL[next];
                outData[2*i+1] = a*bufR[l]+b*bufR[next];
                rate = 0.001*r + 0.999*rate;
                rp += rate;
                d = d-rate;
                if (rp >= MAX_PACKET) rp = rp - MAX_PACKET;
            }else {
                outData[2*i] = 0;
                outData[2*i+1] = 0;
                volume.muteON();
                isPlaying = false;
                printf("stop\n");
            }
        }else {
            outData[2*i] = 0;
            outData[2*i+1] = 0;
        }
    }
    volume.processAudio(outData, outData, outPacketSize*2);
}

void StreamingBuffer::popMono(short *outData, int outPacketSize)
{
    float a, b, d, r;
    int	l, next;
    
    d = (float)wp - rp;
    if (d < 0)  d = d + MAX_PACKET;
    r  = d/(float)bs;
    if (loop++==1000) {
        //printf("Buffer:%f,%d\n", rate,(int)bs);
        loop = 0;
    }
    
    if(r>1.5) {
        r = 1.01;
    } else r = 1.0;
    for (int i=0; i<outPacketSize; i++) {
        l = rp;
        b = rp - (unsigned)rp;
        a = 1.0-b;
        
        if (l < MAX_PACKET-1) next = l+1;
        else next = l - MAX_PACKET + 1;
        
        outData[i] = 0;
        
        if (isPlaying) {
            if (d > 0) {
                outData[i] = a*bufL[l]+b*bufL[next];
                rate = 0.001*r + 0.999*rate;
                rp += rate;
                d = d-rate;
                if (rp >= MAX_PACKET) rp = rp - MAX_PACKET;
            }else {
                outData[i] = 0;
                volume.muteON();
                isPlaying = false;
                printf("stop\n");
            }
        }else {
            outData[i] = 0;
        }
    }
    volume.processAudio(outData, outData, outPacketSize);
}

StreamingBuffer::~StreamingBuffer()
{
    free(bufL);
    free(bufR);
}