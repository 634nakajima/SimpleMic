/*
 *  ModuleList.h
 *
 *
 *  Created by kannolab on 12/10/05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ModuleList_h
#define _ModuleList_h

#include "Module.h"
#include "MToken.h"
#include <stdlib.h>
#include <list>

class ModuleList : public Module {
    
public:
    
    ModuleList(Server *s, const char *osc);
    ~ModuleList();
    std::list<MToken*>			mList;
    char                        t;
    
    void    createModule(char *tID, MToken *ml);
    void    deleteModule(char *tID, MToken *ml);
    
    void    createModule(int tID, MToken *ml);
    void    deleteModule(int tID, MToken *ml);
    
    void    createModule(char *tID, int mlID);
    void    deleteModule(char *tID, int mlID);
    
    void    createModule(int tID, int mlID);
    void    deleteModule(int tID, int mlID);
    
    void	requestML();
    void    displayModules();
    MToken *getMtknFromID(int mlID);

    static int setMList(const char   *path,
                        const char   *types,
                        lo_arg       **argv,
                        int          argc,
                        void         *data,
                        void         *user_data);
    
    static int setMListTCP(const char   *path,
                           const char   *types,
                           lo_arg       **argv,
                           int          argc,
                           void         *data,
                           void         *user_data);
    
    static int deleteMList(const char   *path,
                           const char   *types,
                           lo_arg       **argv,
                           int          argc,
                           void         *data,
                           void         *user_data);
    
    static int deleteMListTCP(const char   *path,
                              const char   *types,
                              lo_arg       **argv,
                              int          argc,
                              void         *data,
                              void         *user_data);
};

#endif