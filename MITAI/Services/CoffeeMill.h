//
//  CoffeeMill.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/04/11.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__CoffeeMill__
#define __RTSOSCApp__CoffeeMill__

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "ModuleManager.h"
#include "Serial.h"

class CoffeeMill : public Module
{
public:
    CoffeeMill(Server *s, const char *osc);
    ~CoffeeMill();
    int d;
    static int something(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
};

class CMManager : public ModuleManager<CoffeeMill>
{
public:
    CMManager();
    ~CMManager();
    
    Serial serial;
    char    buf[1024];
    int     rp,wp;
    int     pre;

    void init(Server *s, const char *osc);
    void setData(int d);
    void	parseData();
    char	readData();
    int		available();
    CoffeeMill *initModule(Server *s, const char *osc);
    static void serialCallback(void *data, int size, void *user_data);
};

#endif /* defined(__RTSOSCApp__CoffeeMill__) */
