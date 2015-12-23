//
//  MToken.cpp
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "MToken.h"

MToken::MToken()
{
    tID = -1;
    ip[0] = NULL;
    osc[0] = NULL;
    mm_addr = NULL;
    iconData = NULL;
}

void MToken::setInputInfo(char *inInfo) {
    char *addr, *info;
    if ((addr = strtok(inInfo, ",")) == NULL) return;
    else {
        info = (char *)calloc(sizeof(addr),1);
        strcpy(info, addr);
        inputInfo.push_back(info);
        
        while ((addr = strtok(NULL, ",")) != NULL) {
            info = (char *)calloc(sizeof(addr),1);
            strcpy(info, addr);
            inputInfo.push_back(info);
        }
    }
    auto it = inputInfo.begin();
    while (it != inputInfo.end()) {
        printf("input:%s\n",(char *)*it);
        ++it;
    }
}

void MToken::setInputInfo(std::vector<char *> inInfo) {
    char *info;
    
    auto it = inInfo.begin();
    while (it != inInfo.end()) {
        info = (char *)calloc(sizeof(*it),1);
        strcpy(info, *it);
        inputInfo.push_back(info);
        printf("input:%s\n",(char *)*it);
        ++it;
    }
}

void MToken::setOutputInfo(char *outInfo) {
    char *addr, *info;
    if ((addr = strtok(outInfo, ",")) == NULL) return;
    else {
        info = (char *)calloc(sizeof(addr),1);
        strcpy(info, addr);
        outputInfo.push_back(info);
        
        while ((addr = strtok(NULL, ",")) != NULL) {
            info = (char *)calloc(sizeof(addr),1);
            strcpy(info, addr);
            outputInfo.push_back(info);
        }
    }
    auto it= outputInfo.begin();
    while (it != outputInfo.end()) {
        printf("output:%s\n",(char *)*it);
        ++it;
    }
}

void MToken::setOutputInfo(std::vector<char *> outInfo) {
    char *info;
    
    auto it = outInfo.begin();
    while (it != outInfo.end()) {
        info = (char *)calloc(sizeof(*it),1);
        strcpy(info, *it);
        inputInfo.push_back(info);
        printf("input:%s\n",(char *)*it);
        ++it;
    }
}

void MToken::setIconData(char *data, int size) {
    if(iconData) free(iconData);
    iconData = (char *)calloc(size, 1);
    memcpy(iconData, data, size);
    iconSize = size;
}

MToken::~MToken() {
    if(mm_addr) lo_address_free(mm_addr);
    if(iconData) free(iconData);
    
    auto it = inputInfo.begin();
    while (it != inputInfo.end()) {
        char *m = *it;
        free(m);
        it = inputInfo.erase(it);
    }
}