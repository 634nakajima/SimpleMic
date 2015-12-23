/*
 *  SawAM.h
 *  
 *
 *  Created by kannolab on 12/07/13.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SawAM_h
#define _SawAM_h

#include "Module.h"
#ifndef	_PT_
#include "porttime.h"
#define _PT_
#endif


class SawAM : public Module {

public:
	int			freq;
	float		fVal;
	float		*sample;
	double		location;
	long		packetCount;
	double		interval;
    double		sampleRate;
    unsigned	numPackets;
	float		fTable[256];
	float		*buf;
	short		*output;
    double      currentLevel;
    
	SawAM(Server *s, const char *osc);
    SawAM(){}
	~SawAM();
    
    void init(Server *s, const char *osc);
    
    static int stream(const char   *path, 
                      const char   *types, 
                      lo_arg       **argv, 
                      int          argc,
                      void         *data, 
                      void         *user_data);
    
    static int data1(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data);
	
    static void level(void *user_data);
    
	void prepareAudioResources();
	void initWave();

};

#endif