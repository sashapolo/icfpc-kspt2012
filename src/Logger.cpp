/* 
 * File:   BaseLogger.cpp
 * Author: kirill
 * 
 * Created on September 26, 2012, 6:02 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "Logger.h"


//////////LOGGER EXTERNAL//////////////////////////////
/**
 * Текущий логгер.
 */
Logger* pCurrentLogger=0;

/**
 * Установка текущего лога.
 * @param Logger* pLogger - идентификатор лога.
 */
void SetLogger(Logger* pLogger)
{
    pCurrentLogger=pLogger;
};

/**
 * Вывод сообщения с ошибкой в лог.
 * @param char* Msg - сообщение.
 * @return false, если это не текущий лог, в противном случае - true.
 */
bool LogError(const char* Msg,...)
{
    if(!pCurrentLogger) return false;
    va_list args;
    va_start(args, Msg);
    pCurrentLogger->BaseOutV(LOGGER_MSG_TYPE_ERROR,Msg,args);
    va_end(args);
    return true;
};

/**
 * Вывод сообщения с предупреждением в лог.
 * @param char* Msg - сообщение.
 * @return false, если это не текущий лог, в противном случае - true.
 */
bool LogWarning(const char* Msg,...)
{
    if(!pCurrentLogger) return false;
    va_list args;
    va_start(args, Msg);
    pCurrentLogger->BaseOutV(LOGGER_MSG_TYPE_WARNING,Msg,args);
    va_end(args);
    return true;
};

/**
 * Вывод информационного сообщения в лог.
 * @param char* Msg - сообщение.
 * @return false, если это не текущий лог, в противном случае - true.
 */
bool LogInfo(const char* Msg,...)
{
    if(!pCurrentLogger) return false;
    va_list args;
    va_start(args, Msg);
    pCurrentLogger->BaseOutV(LOGGER_MSG_TYPE_INFO,Msg,args);
    va_end(args);
    return true;
};

///////////////////////////////////////////////////////

/**
 * Конструктор класса Logger.
 * Задает начальное значение счетчиков предупреждений, ошибок и сообщений.
 * Также задает максимальное значение счетчика сообщений.
 */
Logger::Logger() {
    WarningsCount=0;
    ErrorsCount=0;
    MessagesCount=0;

    MaxMessageCnt=512;
    
    memset(&CS1,0,sizeof(CS1));
    memset(&CS2,0,sizeof(CS2));
}

/**
 * Деструктор класса Logger.
 */
Logger::~Logger() {
}

/**
 * Вывод информационного сообщения в лог (аргументы передаются перечислением).
 * @param UINT MsgType - тип сообщения.
 * @param char* Txt - сообщение.
 */
void Logger::BaseOut(UINT MsgType,const char* Txt,...)
{
    pthread_mutex_lock(&CS2);
    va_list args;
    va_start(args, Txt);
    BaseOutV(MsgType,Txt,args);
    va_end(args);
    pthread_mutex_unlock(&CS2);
}

/**
 * Вывод информационного сообщения в лог (аргументы передаются массивом va_list).
 * @param UINT MsgType - тип сообщения.
 * @param char* Txt - сообщение.
 * @param va_list args - аргументы.
 */
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

/**
 * Вывод сообщения.
 * @param LOGGER_MSG& Msg - сообщение.
 */
void Logger::OnMessageOut(LOGGER_MSG& Msg)
{
    printf("%s",Msg.String.c_str());
}

/**
 * Чтение первого сообщения.
 * @param LOGGER_MSG* pMsg - сообщение.
 * @return false, сообщений нет; true, если pMsg нулевой или сообщение прочитано успешно.
 */
bool Logger::ReadFirstMsg(LOGGER_MSG* pMsg)
{
    if(pMsg==NULL) return true;
    if(MsgList.size()==0) return false;

    std::list<LOGGER_MSG>::iterator it=MsgList.begin();
    (*pMsg)=*it;
    MsgList.pop_front();
    return true;
}

/**
 * Чтение последнего сообщения.
 * @param LOGGER_MSG* pMsg - сообщение.
 * @return false, сообщений нет; true, если pMsg нулевой или сообщение прочитано успешно.
 */
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

/**
 * Информационное сообщение.
 * @param const char* str - строка сообщения.
 */
void Logger::Message(const char* str,...)
{
    pthread_mutex_lock(&CS2);
    va_list args;
    va_start(args, str);
    BaseOutV(LOGGER_MSG_TYPE_INFO,str,args);
    va_end(args);
    pthread_mutex_unlock(&CS2);
}

/**
 * Сообщение с предупреждением.
 * @param const char* str - строка сообщения.
 */
void Logger::Warning(const char* str,...)
{
    pthread_mutex_lock(&CS2);
    va_list args;
    va_start(args, str);
    BaseOutV(LOGGER_MSG_TYPE_WARNING,str,args);
    va_end(args);
    pthread_mutex_unlock(&CS2);
}

/**
 * Сообщение об ошибке.
 * @param const char* str - строка сообщения.
 */
void Logger::Error(const char* str,...)
{
    pthread_mutex_lock(&CS2);
    va_list args;
    va_start(args, str);
    BaseOutV(LOGGER_MSG_TYPE_ERROR,str,args);
    va_end(args);
    pthread_mutex_unlock(&CS2);
}
