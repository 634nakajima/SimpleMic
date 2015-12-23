//
//  DACManager.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/08/15.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#include "DAC.h"

void DAC::dBTimer(void *userData) {
    DAC *dac = (DAC *)userData;
    int dB = dac->dBTransform(dac->buffer.getCurrentdB());
    dac->sendDataTo(dB, 0, 1);
}

int DAC::dBTransform(float dB) {
    float retVal;
    if(dB < -60) retVal = -60.0;
    else if(dB > 0) retVal = 0.0;
    else retVal = dB;
    retVal = (retVal+60.0)*255.0/70.0;

    return (int)retVal;
}

int DAC::stream(const char   *path,
                const char   *types,
                lo_arg       **argv,
                int          argc,
                void         *data,
                void         *user_data)
{
    DAC *dac = (DAC *)user_data;
    lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b)/sizeof(short);
    int d = (int)dac->wp - (int)dac->rp;
    if (d < 0)  d = d + MAX_PACKET;
    
    dac->sendAudioTo(dp, lo_blob_datasize(b), 0);
    dac->buffer.pushMono(dp, size);

    return 0;
}

int DAC::stream2(const char   *path,
                 const char   *types,
                 lo_arg       **argv,
                 int          argc,
                 void         *data,
                 void         *user_data)
{
    DAC *dac = (DAC *)user_data;
    
    lo_blob b   = (lo_blob)argv[0];
    lo_blob b2  = (lo_blob)argv[1];
    short *dp   = (short *)lo_blob_dataptr(b);
    short *dp2  = (short *)lo_blob_dataptr(b2);
    int size = lo_blob_datasize(b)/sizeof(short);
    int d = (int)dac->wp - (int)dac->rp;
    if (d < 0)  d = d + MAX_PACKET;
    
    if (dac->isPlaying) {
        for(int i=0; i<size; i++){
            
            if (d++ <= dac->bs+size*2) {
                dac->buf[dac->wp] = *dp++;
                dac->buf2[dac->wp] = *dp2++;
                dac->wp = (dac->wp != MAX_PACKET-1 ? dac->wp+1 : 0);
            } else {
                printf("//\n");
                break;
            }
        }
        
    } else {
        
        for(int i=0; i<size; i++){
            dac->buf[dac->wp] = *dp++;
            dac->buf2[dac->wp] = *dp2++;
            dac->wp = (dac->wp != MAX_PACKET-1 ? dac->wp+1 : 0);
        }
        if (d+size > dac->bs && !dac->isPlaying) {
			dac->vVal = dac->vValcpy;
            dac->start();
            printf("start\n");
        }
    }
    return 0;
}

int DAC::data1(const char   *path,
               const char   *types,
               lo_arg       **argv,
               int          argc,
               void         *data,
               void         *user_data)
{
    DAC *dac = (DAC *)user_data;
    dac->buffer.volume.setVol(argv[0]->i/2);
    return 0;
}

int DAC::data2(const char   *path,
               const char   *types,
               lo_arg       **argv,
               int          argc,
               void         *data,
               void         *user_data)
{
    DAC *dac = (DAC *)user_data;
    dac->vVal = argv[0]->i;
	dac->vValcpy = argv[0]->i;
	dac->buffer.bs = 32*argv[2]->i;
    
    return 0;
}

int DAC::outputCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData)
{
    DAC *dac = (DAC *)userData;
    short *out = (short *)outputBuffer;
    
    int d = (int)dac->wp - (int)dac->rp;
    if (d < 0)  d = d + MAX_PACKET;

    dac->buffer.pop(out, (int)framesPerBuffer);
	return paContinue;
}

DAC::DAC(Server *s, const char *osc) : Module(s, osc)
{
    Module::init(s, osc);
    addMethod();
    initDAC();
    setOutNum(2);
    if(preparePa()) printf("err_preparePa\n");
}

void DAC::init(Server *s, const char *osc) {
    Module::init(s, osc);
    addMethod();
    initDAC();
    setOutNum(2);
    if(preparePa()) printf("err_preparePa\n");
}

void DAC::addMethod()
{
    addMethodToServer("/AudioIn", "b", stream, this);
    addMethodToServer("/AudioIn", "bb", stream2, this);
    addMethodToServer("/Volume", "ii", data1, this);
	addMethodToServer("/Data", "iiii", data2, this);
}

void DAC::initDAC()
{
    for (int i=0; i<128; i++) {
        vTable[i] = 1.0 - logf((float)(127-i))/logf(127.0);
    }
    
    vTable[127] = 1.0;
    buffer.volume.setVol(127);
	vValcpy		= 64;
    vol         = 1.0;
    isPlaying   = false;
    sampleRate  = 44100.0;
    rp          = 0;
    wp          = 0;
    bs          = 32*16;
	ch			= 2;
    buf         = (short *)calloc(sizeof(short), MAX_PACKET);
    buf2         = (short *)calloc(sizeof(short), MAX_PACKET);
    
    st->setDataCallback(dBTimer, 0.1, this);
    
}

int DAC::preparePa()
{
    err = Pa_Initialize();
    if( err != paNoError ) {
        Pa_Terminate();
        return 1;
    }
    
    outputParameters.device = Pa_GetDefaultOutputDevice(); // デフォルトアウトプットデバイス
    if (outputParameters.device == paNoDevice) {
        printf("Error: No default output device.\n");
        Pa_Terminate();
        return 1;
    }
    
    outputParameters.channelCount = 2;       // ステレオアウトプット
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    err = Pa_OpenStream(&paStream,
                        NULL,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0, // paClipOff,    we won't output out of range samples so don't bother clipping them
                        outputCallback,
                        this);
    
    if( err != paNoError ) {
        Pa_Terminate();
        printf("err_open\n");
    }
    start();
    return 0;
}

int DAC::start()
{
    err = Pa_StartStream( paStream );
    isPlaying = true;
    return 0;
}

int DAC::stop()
{
    err = Pa_StopStream( paStream );
    isPlaying = false;
    return 0;
}

void DAC::deleteMethod()
{
    deleteMethodFromServer("/AudioIn", "b");
    deleteMethodFromServer("/Audio", "bb");
    deleteMethodFromServer("/Volume", "ii");
	deleteMethodFromServer("/Data", "ii");
}

void DAC::controlRate(short *inData, short *outData, int inPacketSize, int outPacketSize)
{
    /*float rate, a, b, location;//,f;
    int	l;
    
    rate  = (float)inPacketSize/(float)outPacketSize;
    b = location - (unsigned)location;
    a = 1.0-b;
    l = location;
    
    buf[i] += a*inData[l];
    buf[i] += b*outData[l+1];
    
    location += rate;*/
}

void DAC::finishPa()
{
	free(buf);
    free(buf2);
    stop();
    Pa_CloseStream( paStream );
    Pa_Terminate();
}

DAC::~DAC()
{
    deleteMethod();
    finishPa();
    st->removeDataCallback(dBTimer, this);
}