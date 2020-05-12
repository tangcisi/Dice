/*
 * Copyright (C) 2019-2020 String.Empty
 * ����ʱ�¼�
 * �����ܼ�ʱ��ɵ�ָ��
 */
#pragma once
#include <string>
#include "DiceMsgSend.h"

struct DiceJobDetail {
    long long fromQQ = 0;
    chatType fromChat;
    const char* cmd_key;
    std::string strMsg;
    time_t fromTime = time(NULL);
    //��ʱ������
    map<string, string> strVar = {};
    DiceJobDetail(const char* cmd):cmd_key(cmd){}
    DiceJobDetail(long long qq, chatType ct, std::string msg = "", const char* cmd = "") 
        :fromQQ(qq), fromChat(ct), strMsg(msg),cmd_key(cmd) {

    }
};

class DiceJob : public DiceJobDetail {
    enum class Renum { NIL, Retry_For, Retry_Until };
public:
    DiceJob(DiceJobDetail detail) :DiceJobDetail(detail) {}
    Renum ren = Renum::NIL;
    void exec();
    void echo(const std::string&);
    void note(const std::string&, int);
};

class DiceScheduler {
public:
    void start();
    void end();
    void push_job(DiceJobDetail&);
};
inline DiceScheduler sch;

typedef void (*cmd)(DiceJob&);