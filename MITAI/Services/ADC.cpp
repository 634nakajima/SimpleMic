//
//  ADC.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "ADC.h"

int ADC::inputCallback(const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData)
{
    ADC *adc = (ADC *)userData;
    
    short *in = (short *)inputBuffer;
    (void) timeInfo; // Prevent unused variable warnings. 
    (void) statusFlags;
    (void) userData;
    
	for (int i=0; i<framesPerBuffer; i++) {
		adc->buf[i] = in[i];
	}
    adc->sendAudioTo(adc->buf, framesPerBuffer*sizeof(short), 0);
    return paContinue;
}

int ADC::stream(const char   *path, 
                const char   *types, 
                lo_arg       **argv, 
                int          argc,
                void         *data, 
                void         *user_data)
{
    ADC *adc = (ADC *)user_data;
    return 0;
}

int ADC::data1(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    ADC *adc = (ADC *)user_data;
    return 0;
}

ADC::ADC(Server *s, const char *osc) : Module(s, osc)
{
    Module::init(s, osc);
    addMethodToServer("/Volume", "ii", ADC::data1, this);

    isPlaying = false;
	buf = (short *)malloc(FRAMES_PER_BUFFER*sizeof(short));
    if(preparePa()) printf("err_preparePa\n");
}

int ADC::preparePa()
{
    err = Pa_Initialize();
    if( err != paNoError ) {
        Pa_Terminate();
        return 1;
    }
    int c = Pa_GetDeviceCount();
	printf("All Devices: %d\n",c);
    inputParameters.device = Pa_GetDefaultInputDevice(); // デフォルトインプットデバイス
	printf("Default InputDevice: %d\n", inputParameters.device);
    if (inputParameters.device == paNoDevice) {
        printf("Error: No default input device.\n");
        Pa_Terminate();
        return 1;
    }
    inputParameters.channelCount = 1;       // モノラルインプット
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    
    err = Pa_OpenStream(&paStream,
                        &inputParameters,
                        NULL,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0, // paClipOff,    we won't output out of range samples so don't bother clipping them 
                        inputCallback,
                        this);
    if( err != paNoError ) {
        Pa_Terminate();
        printf("err_open\n");
    }
    start();
    return 0;
}

int ADC::start()
{
    err = Pa_StartStream( paStream );
    if( err != paNoError ) {
        Pa_Terminate();
        printf("err_start\n");
        return 1;
    }
    isPlaying = true;
    return 0;
}

int ADC::stop()
{
    err = Pa_StopStream( paStream );
    if( err != paNoError ) {
		Pa_Terminate();
		return 1;
    }
    isPlaying = false;
    return 0;
}

ADC::~ADC()
{
    stop();
    Pa_CloseStream( paStream );
    Pa_Terminate();
    deleteMethodFromServer("/Volume", "ii");
}