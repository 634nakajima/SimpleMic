//
//  SerialParser.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/04.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "SerialParser.h"

void SerialParser::setCallback(void (*callback)(void *, int, void *), void *user_data) {
    mmCallback.push_back(callback);
    ud.push_back(user_data);
}

void SerialParser::serialCallback(void *data, int s, void *user_data) {
    SerialParser *sp = (SerialParser *)user_data;
    
    unsigned char *dp = (unsigned char *)data;
    for (int i = 0; i < s; i++) {
        sp->buf[sp->wp] = *dp++;
        sp->wp = (sp->wp+1)%1024;
    }
    sp->parseData();
}

void SerialParser::parseData()
{
    unsigned int object, value;
    
    if (available() > 1) {
        while (available() > 0) {
            object = readData();
            value = readData();
            if(object < 5) mmCallback[object](&value, sizeof(int), ud[object]);
        }
    }
    
}

unsigned char SerialParser::readData()
{
    unsigned char value;
    value = buf[rp];
    rp = (rp+1)%1024;
    return value;
}

int SerialParser::available()
{
    if (rp <= wp) {
        return (wp-rp);
    }else {
        return (1024+wp-rp);
    }
}