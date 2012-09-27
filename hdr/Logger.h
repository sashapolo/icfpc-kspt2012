/* 
 * File:   BaseLogger.h
 * Author: kirill
 *
 * Created on September 26, 2012, 6:02 PM
 */

#pragma once
#include <pthread.h>
#include <string>
#include <list>
#include <stdarg.h>



#define _PLOC(x) "[%s, %s:%d] "x,__FUNCTION__,__FILE__,__LINE__
#define _PFUN(x) "[%s] "x,__FUNCTION__


#define LOGGER_MSG_TYPE_UNKNOWN 0
#define LOGGER_MSG_TYPE_INFO 1
#define LOGGER_MSG_TYPE_WARNING 2
#define LOGGER_MSG_TYPE_ERROR 3

#define LOGINFO(x,s...) LogInfo(_PLOC(x),## s);
#define LOGWARNING(x,s...) LogWarning(_PLOC(x),## s);
#define LOGERROR(x,s...) LogError(_PLOC(x),## s);

typedef unsigned int UINT;
struct LOGGER_MSG
{
    LOGGER_MSG()
    {
        MsgType=LOGGER_MSG_TYPE_UNKNOWN;
        Time=0;
    };
    std::string String;
    UINT MsgType;
    time_t Time;
};

class Logger
{
public:
    Logger();
    virtual ~Logger();

    void SetMaxMessageCount(UINT Val) {MaxMessageCnt=Val;};
    UINT GetMaxMessageCount() {return MaxMessageCnt;};
    void ClearMsgStack() {MsgList.clear();};

    int GetNumWarnings() {return WarningsCount;};
    int GetNumErrors() {return ErrorsCount;};
    int GetNumInfos() {return MessagesCount;};

    bool ReadFirstMsg(LOGGER_MSG* pMsg);
    bool ReadLastMsg(LOGGER_MSG* pMsg);

    void Message(const char* str,...);
    void Warning(const char* str,...);
    void Error(const char* str,...);

    void MessageV(const char* str,va_list args);
    void WarningV(const char* str,va_list args);
    void ErrorV(const char* str,va_list args);

    void BaseOut(UINT MsgType,const char* Txt,...);
    void BaseOutV(UINT MsgType,const char* Txt,va_list args);

    virtual void OnMessageOut(LOGGER_MSG& Msg);
private:
    int WarningsCount;
    int ErrorsCount;
    int MessagesCount;

    UINT MaxMessageCnt;

    pthread_mutex_t CS1,CS2;

    std::list<LOGGER_MSG> MsgList;
};


//////////LOGGER EXTERNAL//////
void SetLogger(Logger* pLogger);
bool LogError(char* Msg,...);
bool LogWarning(char* Msg,...);
bool LogInfo(char* Msg,...);
///////////////////////////////