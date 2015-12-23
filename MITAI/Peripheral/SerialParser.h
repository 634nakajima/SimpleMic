//
//  SerialParser.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/04.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef __RTSOSCApp__SerialParser__
#define __RTSOSCApp__SerialParser__

#include <stdio.h>
#include <stdlib.h>
#include <vector>

typedef void (*func)(void *, int, void *);

class SerialParser {
    
public:
    
    SerialParser(){};
    ~SerialParser(){};

    unsigned char			buf[1024];
    int						rp,wp;
    
    void            parseData();
    unsigned char	readData();
    int             available();

    std::vector<func> mmCallback;
    std::vector<void *> ud;
    void setCallback(void (*callback)(void *, int, void *), void *user_data);
    
    static void serialCallback(void *data, int s, void *user_data);

};

#endif /* defined(__RTSOSCApp__SerialParser__) */
