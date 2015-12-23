//
//  Coordinator.h
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Coordinator_h
#define _Coordinator_h

#include "Module.h"
#include "MToken.h"
#include "ModuleList.h"
#include <map>
#include <list>

class Coordinator : public Module {

public:
    Coordinator();
    Coordinator(Server *s, const char *osc);
    ~Coordinator();
    
    std::map<int, MToken*>      mtknMap;
    ModuleList                  *ml;
    int                         mListIDArray[128];
    void    init(Server *s);
    void    addConnection(int tID1, int tID2, int outID, int inID);
    void    disconnect(int tID1, int tID2, int outID, int inID);
	void    disconnectAll(int tID, const char *t);
    int    createModule(int mc);
    void    deleteModule(int tID);
    int     mIDofOSCPath(const char *osc);
    static int setMtkn(const char   *path,
                       const char   *types, 
                       lo_arg       **argv, 
                       int          argc,
                       void         *data, 
                       void         *user_data);
    
    static int deleteMtkn(const char   *path, 
                          const char   *types, 
                          lo_arg       **argv, 
                          int          argc,
                          void         *data, 
                          void         *user_data);
    
};

#endif
