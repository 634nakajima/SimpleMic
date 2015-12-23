//
//  RoutingTable.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _RoutingTable_h
#define _RoutingTable_h

#include "lo.h"
#include <iostream>
#include <string.h>

#define MAX_ANUM 16

class RoutingTable
{
public:
    RoutingTable();
    ~RoutingTable();
    
    void setRoute(char *ip, char *osc);
    void deleteRoute(char *ip, char *osc);
    void deleteAllRoute(char *osc);
    int numRoute(char *osc);
    int isRouting(char *ip, char *osc);
    
    lo_address  loAddr[MAX_ANUM];
    char        *oscAddr[MAX_ANUM];
    int         aNum;
    
};

#endif
