//
//  DelayManager.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/09/13.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#include "Delay.h"

Delay::Delay(Server *s, const char *osc) : Module(s,osc) {
    Delay::init(s, osc);
}

void Delay::init(Server *s, const char *osc) {
    Module::init(s, osc);
    addMethodToServer("/AudioIn", "b", stream, this);
    addMethodToServer("/Duration", "ii", data1, this);
    addMethodToServer("/Gain", "ii", data2, this);
    addMethodToServer("/Bypass", "ii", data3, this);
    
    maxbs       = sizeof(float)*44100;
    t           = 250;
    g           = 0.2;
    dp          = 0;
    ps          = 44.1*t;
    isBypassing = false;
    output      = (short *)calloc(4192,sizeof(short));
    buf         = (float *)calloc(maxbs, 1);
}

int Delay::stream(const char   *path,
                  const char   *types,
                  lo_arg       **argv,
                  int          argc,
                  void         *data,
                  void         *user_data) {
    
	Delay *d = (Delay *)user_data;

	lo_blob b = (lo_blob)argv[0];
    short *p = (short *)lo_blob_dataptr(b);
    int dsize = lo_blob_datasize(b);
    
    short   sout;
    float   fout;
    memset(d->output, 0, dsize);
    
    if (!d->isBypassing) {
        for (int i=0;i<dsize/sizeof(short);i++) {
            fout = d->buf[d->dp] + (float)p[i]/32767.0;
            if (fout>1.0) fout = 1.0;
            if (fout<-1.0) fout = -1.0;
            sout = fout*32767;
            d->buf[d->dp] = (float)p[i]*d->g/32767.0 + fout*d->g;
            d->output[i] = sout;
            d->dp = (d->dp <= d->ps-1 ? d->dp+1 : 0);
        }
    } else {
		for (int i=0;i<dsize/sizeof(short);i++) {
			d->output[i] = p[i];
        }
    }
    d->sendAudioTo(d->output, dsize, 0);
	return 0;
}

int Delay::data1(const char   *path,
                 const char   *types,
                 lo_arg       **argv,
                 int          argc,
                 void         *data,
                 void         *user_data) {
    
    Delay *d = (Delay *)user_data;
    d->setDt(argv[0]->i);
	return 0;
}

int Delay::data2(const char   *path,
                 const char   *types,
                 lo_arg       **argv,
                 int          argc,
                 void         *data,
                 void         *user_data) {
    
    Delay *d = (Delay *)user_data;
    d->setGain(argv[0]->i);
	return 0;
}

int Delay::data3(const char   *path,
                 const char   *types,
                 lo_arg       **argv,
                 int          argc,
                 void         *data,
                 void         *user_data) {
    
    Delay *d = (Delay *)user_data;
    d->bypass(argv[0]->i);
	return 0;
}

void Delay::bypass(int data) {
    if (data == -1) return;
    
    isBypassing = (data < 1 ? true : false);
    memset(buf, 0, maxbs);
}

void Delay::setDt(int data) {
    if (data == -1) return;
    
    t = (float)data*500.0/255.0;
    ps = 44.1*t;
}

void Delay::setGain(int data) {
    if (data == -1) return;
    g = (float)data/255.0;
}

Delay::~Delay() {
    free(buf);
    free(output);
	
	deleteMethodFromServer("/AudioIn", "b");
    deleteMethodFromServer("/Duration", "ii");
	deleteMethodFromServer("/Gain", "ii");
    deleteMethodFromServer("/Bypass", "ii");
}