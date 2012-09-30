/* 
 * File:   BaseLogger.cpp
 * Author: kirill
 * 
 * Created on September 26, 2012, 6:02 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "Logger.h"


//////////LOGGER EXTERNAL//////////////////////////////
Logger* pCurrentLogger=0;

void SetLogger(Logger* pLogger)
{
    pCurrentLogger=pLogger;
};

bool LogError(char* Msg,...)
{
    if(!pCurrentLogger) return false;
    va_list args;
    va_start(args, Msg);
    pCurrentLogger->BaseOutV(LOGGER_MSG_TYPE_ERROR,Msg,args);
    va_end(args);
};

bool LogWarning(char* Msg,...)
{
    if(!pCurrentLogger) return false;
    va_list args;
    va_start(args, Msg);
    pCurrentLogger->BaseOutV(LOGGER_MSG_TYPE_WARNING,Msg,args);
    va_end(args);
};

bool LogInfo(char* Msg,...)
{
    if(!pCurrentLogger) return false;
    va_list args;
    va_start(args, Msg);
    pCurrentLogger->BaseOutV(LOGGER_MSG_TYPE_INFO,Msg,args);
    va_end(args);
};

///////////////////////////////////////////////////////

Logger::Logger() {
    WarningsCount=0;
    ErrorsCount=0;
    MessagesCount=0;

    MaxMessageCnt=512;
}

Logger::~Logger() {
}

void Logger::BaseOut(UINT MsgType,const char* Txt,...)
{
    pthread_mutex_lock(&CS2);
    va_list args;
    va_start(args, Txt);
    BaseOutV(MsgType,Txt,args);
    va_end(args);
    pthread_mutex_unlock(&CS2);
}

void Logger::BaseOutV(UINT MsgType,const char* Txt,va_list args)
{
    if (!Txt) return;
    pthread_mutex_lock(&CS1);

    if(MsgType==LOGGER_MSG_TYPE_INFO) MessagesCount++;
    else if(MsgType==LOGGER_MSG_TYPE_WARNING) WarningsCount++;
    else if(MsgType==LOGGER_MSG_TYPE_ERROR) ErrorsCount++;

    va_list arglen;
    va_copy(arglen,args);
    int len=vsnprintf(0,0,Txt, arglen)+ 1;
    va_end(arglen);
     
    char* buffer = (char*)malloc( len * sizeof(char) );
    vsnprintf(buffer, len, Txt, args);

    LOGGER_MSG Msg;
    time(&Msg.Time);
    Msg.String=buffer;
    Msg.MsgType=MsgType;

    if((MaxMessageCnt==0) || (MsgList.size()<MaxMessageCnt)) MsgList.push_back(Msg);

    free( buffer );
    pthread_mutex_unlock(&CS1);

    OnMessageOut(Msg);
}

void Logger::OnMessageOut(LOGGER_MSG& Msg)
{
    printf("%s",Msg.String.c_str());
}

bool Logger::ReadFirstMsg(LOGGER_MSG* pMsg)
{
    if(pMsg==NULL) return true;
    if(MsgList.size()==0) return false;

    std::list<LOGGER_MSG>::iterator it=MsgList.begin();
    (*pMsg)=*it;
    MsgList.pop_front();
    return true;
}

bool Logger::ReadLastMsg(LOGGER_MSG* pMsg)
{
    if(pMsg==NULL) return true;
    if(MsgList.size()==0) return false;

    std::list<LOGGER_MSG>::iterator it=MsgList.end();
    it--;
    (*pMsg)=*it;
    MsgList.pop_back();
    return true;
}

void Logger::Message(const char* str,...)
{
    pthread_mutex_lock(&CS2);
    va_list args;
    va_start(args, str);
    BaseOutV(LOGGER_MSG_TYPE_INFO,str,args);
    va_end(args);
    pthread_mutex_unlock(&CS2);
}

void Logger::Warning(const char* str,...)
{
    pthread_mutex_lock(&CS2);
    va_list args;
    va_start(args, str);
    BaseOutV(LOGGER_MSG_TYPE_WARNING,str,args);
    va_end(args);
    pthread_mutex_unlock(&CS2);
}

void Logger::Error(const char* str,...)
{
    pthread_mutex_lock(&CS2);
    va_list args;
    va_start(args, str);
    BaseOutV(LOGGER_MSG_TYPE_ERROR,str,args);
    va_end(args);
    pthread_mutex_unlock(&CS2);
}