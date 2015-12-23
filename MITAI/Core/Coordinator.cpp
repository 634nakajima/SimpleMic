//
//  Coordinator.cpp
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Coordinator.h"

Coordinator::Coordinator()
{}

Coordinator::Coordinator(Server *s, const char *osc) : Module(s,osc)
{
    init(s);
}

void Coordinator::init(Server *s)
{
    Module::init(s, "/Coordinator");
    addMethodToTCPServer("/SetMdtkn", "si", setMtkn, this);//osc,tID
    addMethodToTCPServer("/DeleteMdtkn", "si", deleteMtkn, this);//osc,tID
    
    ml = new ModuleList(s, "/ModuleList");
    tID = 0;
}

int Coordinator::setMtkn(const char   *path,
                         const char   *types,
                         lo_arg       **argv,
                         int          argc,
                         void         *data,
                         void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
    char ip[16];
    strcpy(ip, co->getSenderTCPIP());
    
    //エラー処理、既存のモジュールトークン確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (strcmp(tmp->ip,ip)==0) {
            if (strcmp(tmp->osc,(char *)argv[0])==0) {
                return 0;
            }
        }
    }
    
    //先にタイルに登録されているモジュールの確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (tmp->tID == argv[1]->i) {//先に登録されていればそのモジュールを消去
            co->deleteModule(tmp->tID);
            break;
        }
    }
    //モジュールトークンの生成
    MToken *m = new MToken();
    strcpy(m->ip, ip);
    strcpy(m->osc, (char *)argv[0]);
    
    if (argv[1]->i != -1) {//tIDが-1でなければ
        m->tID = argv[1]->i;
        m->setInputInfo(co->ml->getMtknFromID(co->mListIDArray[m->tID])->inputInfo);
        m->setOutputInfo(co->ml->getMtknFromID(co->mListIDArray[m->tID])->outputInfo);
        co->mtknMap.insert(std::map<int, MToken*>::value_type(m->tID, m));
        printf("Coordinator_Set:%s, %s, Node Index:%d\n",
               ip,
               (char *)argv[0],
               argv[1]->i);
    }
    
    return 0;
}

int Coordinator::deleteMtkn(const char   *path,
                            const char   *types,
                            lo_arg       **argv,
                            int          argc,
                            void         *data,
                            void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
    char ip[16];
    strcpy(ip, co->getSenderTCPIP());
    
    //エラー処理、既存のモジュールトークン確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (strcmp(tmp->ip,ip)==0) {
            if (strcmp(tmp->osc,(char *)argv[0])==0) {
                if (tmp->tID == argv[1]->i) {
                    printf("delete:%s,%s Node Index:%d\n",
                           tmp->ip,
                           tmp->osc,
                           tmp->tID);
                    
                    delete co->mtknMap[tmp->tID];
                    co->mtknMap.erase(tmp->tID);
                    return 0;
                }
            }
        }
    }
    
    return 0;
}

int Coordinator::createModule(int mc) {
    if(tID==128) tID = 0;
    mListIDArray[tID] = mc;
    ml->createModule(tID, mc);
    return tID++;
}

void Coordinator::deleteModule(int tID) {
    ml->deleteModule(tID, mListIDArray[tID]);
}

void Coordinator::addConnection(int tID1, int tID2, int outID, int inID)
{
    char m1OSC[64], m2OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        printf("err:connect tid1:%d, tid2:%d\n",tID1, tID2);
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
    //モジュールに対して接続するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcat(m1OSC,"/AddRoute");
    strcpy(m2OSC, m2->osc);
    strcat(m2OSC, m2->inputInfo[inID]);
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, m2->ip);
    lo_message_add_string(m, m2OSC);
    lo_message_add_int32(m, outID);
    
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    lo_send_message(lo_ip, m1OSC, m);
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void Coordinator::disconnect(int tID1, int tID2, int outID, int inID)
{
    char m1OSC[64], m2OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
    //モジュールに対して切断するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcat(m1OSC,"/DeleteRoute");
    strcpy(m2OSC, m2->osc);
    strcat(m2OSC,m2->inputInfo[inID]);
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, m2->ip);
    lo_message_add_string(m, m2OSC);
    lo_message_add_int32(m, outID);
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    lo_send_message(lo_ip, m1OSC, m);
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void Coordinator::disconnectAll(int tID, const char *t)
{
    char m1OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID)) {
        //printf("err:disconnect\n");
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID];
    
    //モジュールに対して切断するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcat(m1OSC,"/DeleteAllRoute");
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, t);
    
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    lo_send_message(lo_ip, m1OSC, m);
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

int Coordinator::mIDofOSCPath(const char *osc)
{
    for (std::list<MToken*>::iterator iter = ml->mList.begin(); iter!=ml->mList.end(); iter++) {
        MToken* mt = (*iter);
        if (strstr(mt->osc, osc)) {
            return mt->tID;
        }
    }
    return 0;
}

Coordinator::~Coordinator()
{
    for (std::map<int, MToken*>::iterator iter = mtknMap.begin(); iter!=mtknMap.end(); iter++) {
        deleteModule(iter->second->tID);
        delete iter->second;
    }
    
    mtknMap.clear();
    delete ml;
    
    deleteMethodFromTCPServer("/SetMdtkn", "si");
    deleteMethodFromTCPServer("/DeleteMdtkn", "si");
}