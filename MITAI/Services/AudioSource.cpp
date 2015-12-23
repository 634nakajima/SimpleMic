//
//  AudioSourceManager.cpp
//  RTSOSCApp
//
//  Created by kannolab1 on 2014/08/22.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#include "AudioSource.h"

#pragma mark ----- AudioSource -----

void AudioSource::render(void *userData)
{
	AudioSource *audio = (AudioSource *)userData;
    if (audio == NULL) return;
    if (audio->rec.data == NULL) return;
	int i;

	memset(audio->output, 0, audio->numPackets*sizeof(short));
    
	if (audio->isPlaying) {
        float *tmp = (float *)calloc(audio->numPackets, sizeof(float));
		for (i=0;i<audio->numPackets;i++) {
			float a,b, da, db;
			int	l;
            
			b = audio->location - (int)audio->location;
			a = 1.0-b;
            l = audio->location;
            
            da = audio->rec.data[l];
            if (l >= audio->rec.packetSize-1) {
                db = audio->rec.data[l-audio->rec.packetSize+1];
            }
            else db = audio->rec.data[l+1];
                
			tmp[i] = a*da + b*db;
			audio->location += audio->rate;
            
			if (audio->location >= audio->rec.packetSize-1) {
				if (audio->isLooping) {
					audio->location = audio->location - (float)audio->rec.packetSize+1;
				} else {
					audio->location = 0.0;
					audio->isPlaying = false;
					break;
				}
			}else if (audio->location <= -1) {
                if (audio->isLooping) {
					audio->location = audio->location + (float)audio->rec.packetSize;
				} else {
					audio->location = 0.0;
					audio->isPlaying = false;
					break;
				}
            }
		}
        audio->volume.processAudio(tmp, tmp, audio->numPackets);
        for (i=0;i<audio->numPackets;i++) {
            audio->buf[i] += (float)tmp[i];
        }
        free(tmp);
	}
    float max = 1.0;
	//クリッピング
	for (i=0; i<audio->numPackets;i++) {
        if (max < audio->buf[i]) {
            max = audio->buf[i];
            printf("max:%f, %d\n",max, i);
        }else if (-max > audio->buf[i]) {
            max = -audio->buf[i];
            printf("max:%f, %d\n",max, i);
        }
        audio->output[i] = audio->buf[i]*32767.0/max;
	}
    
    //データ送信
    audio->sendAudioTo(audio->output, audio->numPackets*sizeof(short), 0);
    memset(audio->buf, 0, audio->numPackets*sizeof(float));
}

int AudioSource::audio(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	AudioSource *audio = (AudioSource *)user_data;
	lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b)/sizeof(short);
    audio->numPackets = size;
    
    if (audio->rec.isRecording)
        audio->rec.pushData(dp, size);
    
	for(int i=0; i<audio->numPackets; i++){
		audio->buf[i] = *dp++ /32767.0;
	}
	
	audio->render(audio);
	
    return 0;
}

int AudioSource::data1(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	AudioSource *audio = (AudioSource *)user_data;
    int v = argv[0]->i;
	if (v < 1) {
		audio->isPlaying = false;
		audio->location = 0.0;
        audio->volume.setVol(0);
	} else {
		audio->isPlaying = true;
        audio->volume.setVol(110);
	}
	return 0;
}
int AudioSource::data2(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
    AudioSource *audio = (AudioSource *)user_data;
    
    float f = (float)argv[0]->i;
	
	if (f > 64.0) {
		if (!audio->isPlaying) audio->isPlaying = true;
        audio->rate = f/64.0;
    }
    else if (f > 0.0) {
		if (!audio->isPlaying) audio->isPlaying = true;
        audio->rate = (63.0+f)/127.0;
    }
	else {
		audio->rate = 1.0;
        audio->isPlaying = false;
		audio->location = 0.0;
	}
    return 0;
}

int AudioSource::data3(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
    AudioSource *audio = (AudioSource *)user_data;
    
	int v = argv[0]->i;
	
	if (v < 1) {
        audio->rec.stopRecording();
        audio->location = 0.0;
        audio->isPlaying = true;
	} else {
        audio->isPlaying = false;
        audio->rec.startRecording();
	}
	return 0;
}
int AudioSource::data4(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
    AudioSource *audio = (AudioSource *)user_data;
    
    int v = argv[0]->i;
    
    if (v < 1) {
        audio->isLooping = false;
    } else {
        audio->isLooping = true;
    }
    return 0;
}

int AudioSource::fdata(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
    AudioSource *audio = (AudioSource *)user_data;
    
	int v = argv[0]->i;
	
    if (!audio->isPlaying) audio->isPlaying = true;
    audio->rate = v;
	return 0;
}



AudioSource::AudioSource(Server *s, const char *osc) : Module(s, osc)
{
    Module::init(s, osc);
    //add methods
    addMethodToServer("/Audio", "b", AudioSource::audio, this);
    addMethodToServer("/Data1", "ii", AudioSource::data1, this);
    addMethodToServer("/Data2", "ii", AudioSource::data2, this);
    addMethodToServer("/Data3", "ii", AudioSource::data3, this);
    addMethodToServer("/Data4", "ii", AudioSource::data3, this);
    addMethodToServer("/fData", "fi", AudioSource::fdata, this);
    
    sampleRate	= SAMPLE_RATE;
    numPackets	= 512;
    interval	= (double)numPackets/sampleRate;
    
    buf			= (float *)calloc(numPackets, sizeof(float));
    output		= (short *)calloc(numPackets, sizeof(short));
    isUsingTimer = false;
    initAudioInfo();
}

void AudioSource::init(Server *s, const char *osc) {
    Module::init(s, osc);
    //add methods
    addMethodToServer("/AudioIn", "b", AudioSource::audio, this);
    addMethodToServer("/Data1", "ii", AudioSource::data1, this);
    addMethodToServer("/Data2", "ii", AudioSource::data2, this);
    addMethodToServer("/Data3", "ii", AudioSource::data3, this);
    addMethodToServer("/Data4", "ii", AudioSource::data3, this);
    addMethodToServer("/fData", "fi", AudioSource::fdata, this);
    
    sampleRate	= SAMPLE_RATE;
    numPackets	= 512;
    interval	= (double)numPackets/sampleRate;

    buf			= (float *)calloc(numPackets, sizeof(float));
    output		= (short *)calloc(numPackets, sizeof(short));
    isUsingTimer = false;
    initAudioInfo();
}

void AudioSource::initAudioInfo()
{
	rate = 1.0;
    location = 0.0;
	isPlaying = true;
	isLooping = true;
    volume.setVol(100);
}

int AudioSource::prepareAudioSource(const char *sound) {
    char p[128];
    strcpy(p, "/Users/kannolab_mbp1/Desktop/MITAI.app/Contents/Resources/");
    int retVal = rec.prepareAudioSource(strcat(p, sound));
    
    if (!retVal) {
        //isPlaying = true;
    }
    return retVal;
}

void AudioSource::useTimer() {
    st->setAudioCallback(render, numPackets, this);
    isUsingTimer = true;
}

void AudioSource::stopTimer() {
    st->removeAudioCallback(render, this);
    isUsingTimer = false;
}

AudioSource::~AudioSource()
{
	free(buf);
	free(output);
	
	deleteMethodFromServer("/AudioIn", "b");
	deleteMethodFromServer("/Data1", "ii");
	deleteMethodFromServer("/Data2", "ii");
	deleteMethodFromServer("/Data3", "ii");
    deleteMethodFromServer("/Data4", "ii");
    deleteMethodFromServer("/fData", "fi");
    if(isUsingTimer)
        stopTimer();
}

#pragma mark ----- AudioSourceManager -----
void AudioSourceManager::init(Server *s, const char *osc, const char *coAddr) {
    ModuleManager<AudioSource>::init(s, osc, coAddr);
}

void AudioSourceManager::init(Server *s, const char *osc) {
    ModuleManager<AudioSource>::init(s, osc, "");
    local = false;
}

void AudioSourceManager::setFileName(const char *fname) {
    strcpy(file, fname);
}

AudioSource *AudioSourceManager::initModule(Server *s, const char *osc) {
    AudioSource *as = new AudioSource(s, osc);
    as->prepareAudioSource(file);
    as->useTimer();
    return as;
}
