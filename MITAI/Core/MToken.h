//
//  MToken.h
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _MToken_h
#define _MToken_h

#include <vector>
#include <string.h>
#include "lo.h"

class MToken {
    
public:
    MToken();
    ~MToken();
    void setInputInfo(char *inInfo);
    void setInputInfo(std::vector<char *>inInfo);
    void setOutputInfo(char *outInfo);
    void setOutputInfo(std::vector<char *>outInfo);
    void setIconData(char *data, int size);
    
    char    ip[16];
    char    osc[64];
    lo_address mm_addr;
    int     tID;
    char    *iconData;
    int     iconSize;
    std::vector<char *> inputInfo;
    std::vector<char *> outputInfo;
};

#endif
