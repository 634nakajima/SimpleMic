//
//  Object.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/04.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__Object__
#define __RTSOSCApp__Object__

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "ModuleManager.h"
#include "Serial.h"

class Object : public Module
{
public:
    
    Object(Server *s, const char *osc);
    ~Object();
    Data knob, ity;
    
    void (*cb)(Data *data, void *user_data);
    void *ud;
    void setCallback(void (*callback)(Data *data, void *user_data), void *user_data);
    static int something(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
};

class ObjectManager : public ModuleManager<Object>
{
public:
    ObjectManager(){};
    ~ObjectManager(){};

    int objectID;

    void init(Server *s, const char *osc);
    void setData(int d, int dataID);
    Object *initModule(Server *s, const char *osc);
    
    void (*writeData)(void *data, int s, void *user_data);
    void *ud;
    void setSerialCallback(void (*callback)(void *data, int s, void *user_data), void *user_data);
    
    static void serialCallback(void *data, int size, void *user_data);
    static void moduleCallback(Data *d, void *user_data);
};

#endif /* defined(__RTSOSCApp__Object__) */
