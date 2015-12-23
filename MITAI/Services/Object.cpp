//
//  Object.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/04.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "Object.h"

#pragma mark ----- Object -----

Object::Object(Server *s, const char *osc) : Module(s,osc) {
    addMethodToServer("/Intensity", "ii", something, this);
    knob.value = 0;
    knob.dataID = 0;
    ity.value = 0;
    ity.dataID = 0;
}

int Object::something(const char   *path,
                          const char   *types,
                          lo_arg       **argv,
                          int          argc,
                          void         *data,
                          void         *user_data) {
    Object *o = (Object *)user_data;
    o->ity.value = argv[0]->i;
    o->ity.dataID = argv[1]->i;
    if (o->cb && o->ud) o->cb(&o->ity, o->ud);
    return 0;
}

void Object::setCallback(void (*callback)(Data *data, void *user_data), void *user_data) {
    cb = callback;
    ud = user_data;
}

Object::~Object(){}

#pragma mark ----- ObjectManager -----
void ObjectManager::init(Server *s, const char *osc) {
    ModuleManager::init(s, osc);
    objectID = -1;
}

Object *ObjectManager::initModule(Server *s, const char *osc) {
    Object *o = new Object(s, osc);
    o->setCallback(moduleCallback, this);
    return o;
}

void ObjectManager::setData(int d, int dataID) {
    for (auto iter = mList.begin(); iter != mList.end(); iter++) {
        Object *m = (Object *)(*iter);
        m->knob.value = d;
        m->sendDataTo(d, 0);
    }
}

void ObjectManager::setSerialCallback(void (*callback)(void *data, int s, void *user_data), void *user_data) {
    writeData = callback;
    ud = user_data;
}

void ObjectManager::serialCallback(void *data, int size, void *user_data) {
    ObjectManager *cmm = (ObjectManager *)user_data;
    int *dp = (int *)data;
    cmm->setData(*dp, cmm->objectID);
    
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

    printf("SerialCallback:%d, %d\n",*dp, cmm->objectID);
}

void ObjectManager::moduleCallback(Data *data, void *user_data) {
    ObjectManager *om = (ObjectManager *)user_data;
    
    for (auto iter = om->mList.begin(); iter != om->mList.end(); iter++) {
        Object *m = (Object *)(*iter);
        m->ity.value = data->value;
    }
    
    if(om->writeData && om->ud && (om->objectID != -1)) {
        int d = data->value;
        unsigned char data[2] = {(unsigned char)om->objectID, (unsigned char)d};
        om->writeData(data, sizeof(data), om->ud);
    }
}
