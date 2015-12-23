//
//  CoffeeMill.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/04/11.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "CoffeeMill.h"

#pragma mark ----- CoffeeMill -----

CoffeeMill::CoffeeMill(Server *s, const char *osc) : Module(s,osc)
{
    d = 0;
}

int CoffeeMill::something(const char   *path,
                          const char   *types,
                          lo_arg       **argv,
                          int          argc,
                          void         *data,
                          void         *user_data)
{
    CoffeeMill *cm = (CoffeeMill *)user_data;
    cm->sendDataTo(cm->d, 0);
    return 0;
}



CoffeeMill::~CoffeeMill() {
}

#pragma mark ----- CMManager -----

CMManager::CMManager() {
}

CMManager::~CMManager() {
}

void CMManager::init(Server *s, const char *osc)
{
    rp = 0;
    wp = 0;
    pre = 0;
    ModuleManager::init(s, osc);
    serial.setCallback(serialCallback, this);
    setMInfo("/CoffeeMill", "", "/Handle", "Coffeegrinder.png");//Module Address, Callback Function
}

CoffeeMill *CMManager::initModule(Server *s, const char *osc)
{
    return new CoffeeMill(s, osc);
}

void CMManager::setData(int d)
{
    for (auto iter = this->mList.begin(); iter != this->mList.end(); iter++) {
        CoffeeMill *m = (CoffeeMill *)(*iter);
        m->d = d;
        m->sendDataTo(d, 0);
    }
}

void CMManager::parseData()
{
    unsigned int value;
    unsigned char a64[8], a16[2], data[4];
    
    if (available() > 16) {
        while (available() > 0) {
            if (readData() == 0x7E) {
                for (int i = 0; i < 2; i++) {
                    readData();
                }
                data[0] = readData();
                a64[0] = readData();
                a64[1] = readData();
                a64[2] = readData();
                a64[3] = readData();
                a64[4] = readData();
                a64[5] = readData();
                a64[6] = readData();
                a64[7] = readData();
                
                a16[0] = readData();
                a16[1] = readData();
                
                data[1] = readData();
                
                value = readData();
                
                data[2] = readData();
                
                long sum = 0;
                for (int i=0; i<2; i++) {
                    sum += data[i];
                }
                for (int i=0; i<8; i++) {
                    sum += a64[i];
                }
                for (int i=0; i<2; i++) {
                    sum += a16[i];
                }
                sum += value;
                
                data[3] = (0xFF - (sum & 0xFF));
                
                if(data[2] != data[3]) {
                    printf("err:xbee\n");
                    return;
                }
                value = value*15;
                if(value > 250) value = 250;

                if(pre != value) {
                    setData(value);
                
                    struct timeval myTime;    // time_t構造体を定義．1970年1月1日からの秒数を格納するもの
                    struct tm *time_st;       // tm構造体を定義．年月日時分秒をメンバ変数に持つ構造体
                    /* 時刻取得 */
                    gettimeofday(&myTime, NULL);    // 現在時刻を取得してmyTimeに格納．通常のtime_t構造体とsuseconds_tに値が代入される
                    time_st = localtime(&myTime.tv_sec);    // time_t構造体を現地時間でのtm構造体に変換
                    
                    printf("Date:%02d:%02d:%02d.%06d\t",     // 現在時刻
                           time_st->tm_hour,          // 時
                           time_st->tm_min,           // 分
                           time_st->tm_sec,           // 秒
                           myTime.tv_usec            // マイクロ秒
                           );
                    printf("Coffee:%d\n", value);
                }
                    pre = value;
            }
        }
    }
    
}

char CMManager::readData()
{
    char value;
    value = buf[rp];
    rp = (rp+1)%1024;
    return value;
}

int CMManager::available()
{
    if (rp <= wp) {
        return (wp-rp);
    }else {
        return (1024+wp-rp);
    }
}

void CMManager::serialCallback(void *data, int size, void *user_data) {
    CMManager *cmm = (CMManager *)user_data;
    char *dp = (char *)data;
    for (int i = 0; i < size; i++) {
        cmm->buf[cmm->wp] = *dp++;
        cmm->wp = (cmm->wp+1)%1024;
    }
    cmm->parseData();
}