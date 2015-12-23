/*
 *  ModuleList.cpp
 *
 *
 *  Created by kannolab on 12/10/05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ModuleList.h"
#include <iostream>

ModuleList::ModuleList(Server *s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/setMList", "sssb", setMList, this);
    addMethodToTCPServer("/setMList", "sssb", setMListTCP, this);
    addMethodToServer("/deleteMList", "sss", deleteMList, this);
    addMethodToTCPServer("/deleteMList", "sss", deleteMListTCP, this);
    requestML();
}

int ModuleList::setMList(const char   *path,
                         const char   *types,
                         lo_arg       **argv,
                         int          argc,
                         void         *data,
                         void         *user_data)
{
    ModuleList *mlc = (ModuleList *)user_data;
    int i = 0;
    char ip[16];
    strcpy(ip, mlc->getSenderIP());
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = mlc->mList.begin(); iter != mlc->mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, ip)==0) {
            if (strcmp(ml->osc, (char *)argv[0])==0) {
                return 0;
            }
        }
        i++;
    }
    
    //モジュールリストの生成
    MToken *m = new MToken();
    m->tID = i;//tIDをモジュールリストのインデックスに使う
    strcpy(m->ip, ip);
    m->mm_addr = lo_address_new_with_proto(LO_TCP, m->ip, "6341");
    strcpy(m->osc, (char *)argv[0]);
    m->setInputInfo((char *)argv[1]);
    m->setOutputInfo((char *)argv[2]);
    
    //get icon data
    lo_blob b = (lo_blob)argv[3];
    m->setIconData((char *)lo_blob_dataptr(b), lo_blob_datasize(b));
    mlc->mList.push_back(m);
    
    printf("ModuleList_Set:%s %s Module Index:%d Icon size: %d\n",m->ip, m->osc, m->tID, m->iconSize);
    
    return 0;
    
}

int ModuleList::setMListTCP(const char   *path,
                            const char   *types,
                            lo_arg       **argv,
                            int          argc,
                            void         *data,
                            void         *user_data)
{
    ModuleList *mlc = (ModuleList *)user_data;
    int i = 0;
    char ip[16];
    strcpy(ip, mlc->getSenderTCPIP());
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = mlc->mList.begin(); iter != mlc->mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, ip)==0) {
            if (strcmp(ml->osc, (char *)argv[0])==0) {
                return 0;
            }
        }
        i++;
    }
    
    //モジュールリストの生成
    MToken *m = new MToken();
    m->tID = i;//tIDをモジュールリストのインデックスに使う
    strcpy(m->ip, ip);
    m->mm_addr = lo_address_new_with_proto(LO_TCP, m->ip, "6341");
    strcpy(m->osc, (char *)argv[0]);
    m->setInputInfo((char *)argv[1]);
    m->setOutputInfo((char *)argv[2]);
    
    //get icon data
    lo_blob b = (lo_blob)argv[3];
    m->setIconData((char *)lo_blob_dataptr(b), lo_blob_datasize(b));
    mlc->mList.push_back(m);
    printf("ModuleList_Set:%s %s Module Index:%d Icon size: %d\n",m->ip, m->osc, m->tID, m->iconSize);
    
    return 0;
    
}

int ModuleList::deleteMList(const char   *path,
                            const char   *types,
                            lo_arg       **argv,
                            int          argc,
                            void         *data,
                            void         *user_data)
{
    ModuleList *mlc = (ModuleList *)user_data;
    char ip[16];
    strcpy(ip, mlc->getSenderIP());
    
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = mlc->mList.begin(); iter != mlc->mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, ip)==0) {
            if (strcmp(ml->osc, (char *)argv[0])==0) {
                printf("delete:%s,%s ModuleIndex:%d\n",ml->ip, ml->osc, ml->tID);
                mlc->mList.remove(ml);
                delete ml;
                return 0;
            }
        }
    }
    
    return 0;
    
}

int ModuleList::deleteMListTCP(const char   *path,
                               const char   *types,
                               lo_arg       **argv,
                               int          argc,
                               void         *data,
                               void         *user_data)
{
    ModuleList *mlc = (ModuleList *)user_data;
    char ip[16];
    strcpy(ip, mlc->getSenderTCPIP());
    
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = mlc->mList.begin(); iter != mlc->mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, ip)==0) {
            if (strcmp(ml->osc, (char *)argv[0])==0) {
                printf("delete:%s,%s ModuleIndex:%d\n",ml->ip, ml->osc, ml->tID);
                mlc->mList.remove(ml);
                delete ml;
                return 0;
            }
        }
    }
    
    return 0;
}

void ModuleList::createModule(char *tID, MToken *ml)
{
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_int32(m, 1);
    lo_message_add_string(m, tID);
    
    data = lo_message_serialise(m, ml->osc, NULL, NULL);
    d_len = lo_message_length(m, ml->osc);
    
    lo_address addr = lo_address_new_with_proto(LO_TCP, ml->ip, "6341");
    lo_send_message(addr, ml->osc, m);
    lo_address_free(addr);
    lo_message_free(m);
    free(data);
}

void ModuleList::deleteModule(char *tID, MToken *ml)
{
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_int32(m, 0);
    lo_message_add_string(m, tID);
    
    data = lo_message_serialise(m, ml->osc, NULL, NULL);
    d_len = lo_message_length(m, ml->osc);
    lo_address addr = lo_address_new_with_proto(LO_TCP, ml->ip, "6341");
    lo_send_message(addr, ml->osc, m);
    lo_address_free(addr);
    lo_message_free(m);
    free(data);
}

void ModuleList::createModule(int tID, MToken *ml)
{
    char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
    createModule(t, ml);
}

void ModuleList::deleteModule(int tID, MToken *ml)
{
    char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
    deleteModule(t, ml);
}

void ModuleList::createModule(char *tID, int mlID)
{
    createModule(tID, getMtknFromID(mlID));
}

void ModuleList::deleteModule(char *tID, int mlID)
{
    deleteModule(tID, getMtknFromID(mlID));
}

MToken *ModuleList::getMtknFromID(int mlID) {
    for (auto iter = mList.begin(); iter != mList.end(); iter++) {
        MToken* ml = (*iter);
        if (mlID == ml->tID) {
            return ml;
        }
    }
    return NULL;
}

void ModuleList::createModule(int tID, int mlID)
{
    char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
    createModule(t, mlID);
}

void ModuleList::deleteModule(int tID, int mlID)
{
    char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
    deleteModule(t, mlID);
}

void ModuleList::requestML()
{
    int sock;
    unsigned long n, d_len;
    struct sockaddr_in addr;
    void *data;
    char path[] = "/ModuleManager/RequestML";
    char p[64];
    //int  mColor;
    strcpy(p, OSCAddr);
    
    //create lo_message
    lo_message m = lo_message_new();
    lo_message_add_int32(m, 0);
    
    
    data = lo_message_serialise(m, path, NULL, NULL);
    d_len = lo_message_length(m, path);
    
    //create socket
    int opt = 1;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(int));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6340);
    inet_pton(AF_INET, "255.255.255.255", &addr.sin_addr.s_addr);
    
    //send(念のため3回)
    srand((unsigned int)time(NULL));
    for (int j=0; j<3; j++) {
        n = sendto(sock, data, d_len, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 1) {
            perror("sendto");
        }
        usleep(1000+(rand()%100)*100);
    }
    lo_message_free(m);
    free(data);
    close(sock);
}

void ModuleList::displayModules()
{
    for (std::list<MToken*>::iterator iter = mList.begin(); iter != mList.end(); iter++) {
        MToken* ml = (*iter);
        printf("ip:%s osc:%s\n",ml->ip,ml->osc);
    }
}

ModuleList::~ModuleList()
{
    for (std::list<MToken*>::iterator iter = mList.begin(); iter!=mList.end(); iter++) {
        mList.remove(*iter);
        delete (*iter);
    }
    deleteMethodFromServer("/setMList", "sssb");
    deleteMethodFromTCPServer("/setMList", "sssb");
    deleteMethodFromServer("/deleteMList", "sss");
    deleteMethodFromTCPServer("/deleteMList", "sss");
}