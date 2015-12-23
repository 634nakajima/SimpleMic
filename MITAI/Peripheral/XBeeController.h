/*
 *  XBeeController.h
 *  
 *
 *  Created by kannolab on 12/10/11.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _XBeeController_h
#define _XBeeController_h

#include "Module.h"
#include "Coordinator.h"
#include "Serial.h"

class XBeeController {
	
public:
    XBeeController(Server *s);
    ~XBeeController();
    
    void init(Server *s);
	Serial                  *serial;
	char					buf[1024];
	int						rp,wp;
	
    void    setAlive(int tID, int mColor);
	
private:
    void    initTile(int tID, unsigned char *a64, unsigned char *a16);
	void	parseData();
	char	readData();
	int		available();
    void addMethod(Module *m);
    void deleteMethod(Module *m);
    
    static void amCallback(Module *m, int status, void *cl);
    
    static int stream(const char   *path, 
					  const char   *types, 
					  lo_arg       **argv, 
					  int          argc,
					  void         *data, 
					  void         *user_data);
};

#endif
