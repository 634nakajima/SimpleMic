/*
 *  SawAM.cpp
 *  
 *
 *  Created by kannolab on 12/07/13.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "SawAM.h"

int SawAM::stream(const char   *path, 
				 const char   *types, 
				 lo_arg       **argv, 
				 int          argc,
				 void         *data, 
				 void         *user_data)
{
	SawAM *sawAM = (SawAM *)user_data;
	
	lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b)/sizeof(short);
    sawAM->numPackets = size;
    
    memset(sawAM->buf, 0, sawAM->numPackets*sizeof(float));
    memset(sawAM->output, 0, sawAM->numPackets*sizeof(short));
    
    int i;
    for (i=0;i<sawAM->numPackets;i++) {
        float a,b,f;
        int l;
        
        b = sawAM->location - (unsigned)sawAM->location;
        a = 1.0-b;
        l = (unsigned)sawAM->location;
        f = (float)sawAM->fTable[sawAM->freq];
        sawAM->currentLevel = a*sawAM->sample[l]+b*sawAM->sample[l+1];
        
        sawAM->fVal = f*0.0002 + sawAM->fVal*0.9998;
        sawAM->buf[i] = (float)dp[i]/32767.0*sawAM->currentLevel;
        
        sawAM->location += sawAM->fVal*sawAM->packetCount/sawAM->sampleRate;
        
        while (sawAM->location >= sawAM->packetCount-1) {
            sawAM->location = sawAM->location - (float)sawAM->packetCount+1;
        }
        if (sawAM->location < 0) sawAM->location = 0;
    }
    
    //クリッピング
    for (i=0; i<sawAM->numPackets;i++) {
        if (sawAM->buf[i]>1) {
            sawAM->buf[i] = 1;
        }
        if (sawAM->buf[i]<-1) {
            sawAM->buf[i] = -1;
        }
        sawAM->output[i] = sawAM->buf[i]*32767;
    }
    
    //データ送信
    sawAM->sendAudioTo(sawAM->output, sawAM->numPackets*sizeof(short), 0);
    return 0;
}

int SawAM::data1(const char   *path, 
				const char   *types, 
				lo_arg       **argv, 
				int          argc,
				void         *data, 
				void         *user_data)
{
    SawAM *sawAM = (SawAM *)user_data;
    int f = argv[0]->i;
    if (f > 0)
        sawAM->freq = f;
    
    return 0;
}

void SawAM::level(void *user_data) {
    SawAM *sawAM = (SawAM *)user_data;
    sawAM->sendDataTo((int)(sawAM->currentLevel*255), 0, 1);
}

SawAM::SawAM(Server *s, const char *osc) : Module(s, osc) {
    Module::init(s, osc);
    sampleRate	= SAMPLE_RATE;
    numPackets	= 1024;
    interval	= (double)numPackets/sampleRate;
    
    buf			= (float *)malloc(numPackets*sizeof(float));
    output		= (short *)malloc(numPackets*sizeof(short));
    
    prepareAudioResources();
    initWave();
    s->setDataCallback(level, 0.1, this);
    
    addMethodToServer("/AudioIn", "b", SawAM::stream, this);
    addMethodToServer("/Freq", "ii", SawAM::data1, this);
    
    setOutNum(2);//AudioOut, CurrentLevel
}

void SawAM::init(Server *s, const char *osc) {
    Module::init(s, osc);
    sampleRate	= SAMPLE_RATE;
    numPackets	= 4192;
    interval	= (double)numPackets/sampleRate;
    
    buf			= (float *)malloc(numPackets*sizeof(float));
    output		= (short *)malloc(numPackets*sizeof(short));
    
    prepareAudioResources();
    initWave();
    s->setDataCallback(level, 0.05, this);
    
    addMethodToServer("/AudioIn", "b", SawAM::stream, this);
    addMethodToServer("/Freq", "ii", SawAM::data1, this);
    
    setOutNum(2);//AudioOut, CurrentLevel
}

void SawAM::prepareAudioResources()
{
	int i;
    for (i=0; i<256; i++) {
        fTable[i] = 25.0 * pow(2.0, (i-127)/38.0);
    }
	
    packetCount = 8192*16;
    sample	= (float *)malloc(packetCount*sizeof(float));
    
    for (i=0; i<packetCount; i++) {
        sample[i] = 1.0 - 1.0*(float)i/(float)packetCount;
    }
}

void SawAM::initWave()
{
	freq = 0;
    fVal = 0.0;
    location = 0.0;
}

SawAM::~SawAM()
{
	free(buf);
	free(output);
    st->removeDataCallback(level, this);
	deleteMethodFromServer("/AudioIn", "b");
    deleteMethodFromServer("/Freq", "ii");
}
