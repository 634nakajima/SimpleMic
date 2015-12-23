//
//  Lowpass.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/13.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "Lowpass.h"

Lowpass::Lowpass(Server *s, const char *osc) : Module(s,osc) {
    Lowpass::init(s, osc);
}

void Lowpass::init(Server *s, const char *osc) {
    Module::init(s, osc);
    addMethodToServer("/AudioIn", "b", stream, this);
    addMethodToServer("/Freq", "ii", data1, this);
    addMethodToServer("/Q", "ii", data2, this);
    addMethodToServer("/Bypass", "ii", data3, this);
    
    isBypassing = false;
    freq = 1600.0;
    q = 0.8;
    in1 = 0.0;
    in2 = 0.0;
    out1 = 0.0;
    out2 = 0.0;
    output = (short *)calloc(4192,sizeof(short));
    calcCoef();
    
}

void Lowpass::calcCoef() {
    omega = 2.0f * 3.14159265f * freq/44100.0;
    alpha = sin(omega) / (2.0f * q);
    
    a0 =  1.0f + alpha;
    a1 = -2.0f * cos(omega);
    a2 =  1.0f - alpha;
    b0 = (1.0f - cos(omega)) / 2.0f;
    b1 =  1.0f - cos(omega);
    b2 = (1.0f - cos(omega)) / 2.0f;
}

int Lowpass::stream(const char   *path,
                  const char   *types,
                  lo_arg       **argv,
                  int          argc,
                  void         *data,
                  void         *user_data) {
    
    Lowpass *d = (Lowpass *)user_data;
    
    lo_blob b = (lo_blob)argv[0];
    short *p = (short *)lo_blob_dataptr(b);
    int dsize = lo_blob_datasize(b);
    int size = lo_blob_datasize(b)/sizeof(short);
    float   fout = 0.0;
    float a0 = d->a0;
    float a1 = d->a1;
    float a2 = d->a2;
    float b0 = d->b0;
    float b1 = d->b1;
    float b2 = d->b2;

    memset(d->output, 0, dsize);
    
    for(int i = 0; i < size; i++) {
        if(!d->isBypassing) {
            // 入力信号にフィルタを適用し、出力信号として書き出す。
            fout = b0/a0 * (float)p[i]/32767.0 + b1/a0 * d->in1  + b2/a0 * d->in2
            - a1/a0 * d->out1 - a2/a0 * d->out2;
            
            d->in2  = d->in1;       // 2つ前の入力信号を更新
            d->in1  = (float)p[i]/32767.0;  // 1つ前の入力信号を更新
            
            d->out2 = d->out1;      // 2つ前の出力信号を更新
            d->out1 = fout; // 1つ前の出力信号を更新
            if(fout > 1.0) fout = 1.0;
            if(fout < -1.0) fout = -1.0;
            
            d->output[i] = fout*22767;
        }else {
            d->output[i] = p[i];
        }
    }
    d->sendAudioTo(d->output, dsize, 0);
    return 0;
}

int Lowpass::data1(const char   *path,
                 const char   *types,
                 lo_arg       **argv,
                 int          argc,
                 void         *data,
                 void         *user_data) {
    
    Lowpass *d = (Lowpass *)user_data;
    d->setFreq(argv[0]->i);
    return 0;
}

int Lowpass::data2(const char   *path,
                 const char   *types,
                 lo_arg       **argv,
                 int          argc,
                 void         *data,
                 void         *user_data) {
    
    Lowpass *d = (Lowpass *)user_data;
    d->setQ(argv[0]->i);
    return 0;
}

int Lowpass::data3(const char   *path,
                 const char   *types,
                 lo_arg       **argv,
                 int          argc,
                 void         *data,
                 void         *user_data) {
    
    Lowpass *d = (Lowpass *)user_data;
    d->bypass(argv[0]->i);
    return 0;
}

void Lowpass::bypass(int data) {
    if (data == -1) return;
    isBypassing = (data < 1 ? true : false);
}

void Lowpass::setFreq(int data) {
    if (data == -1) return;
    freq = 21*pow(1.027459, (double)data);
    calcCoef();
}

void Lowpass::setQ(int data) {
    if (data == -1) return;
    q = (float)data/10.0;
}

Lowpass::~Lowpass() {
    free(output);
    
    deleteMethodFromServer("/AudioIn", "b");
    deleteMethodFromServer("/Freq", "ii");
    deleteMethodFromServer("/Q", "ii");
    deleteMethodFromServer("/Bypass", "ii");
}