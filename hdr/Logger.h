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
	/**
	 * Конструктор класса Logger.
	 * Задает начальное значение счетчиков предупреждений, ошибок и сообщений.
	 * Также задает максимальное значение счетчика сообщений.
	 */
	Logger();
	/**
	 * Деструктор класса Logger.
	 */
	virtual ~Logger();
	/**
	 * установка максимального значения счетчика сообщений.
	 * @param UINT Val - максимальное значение счетчика сообщений.
	 */
	void SetMaxMessageCount(UINT Val) {MaxMessageCnt=Val;};
	/**
	 * Получение максимального значения счетчика сообщений.
	 * @return максимальное значение счетчика сообщений.
	 */
	UINT GetMaxMessageCount() {return MaxMessageCnt;};
	/**
	 * Очистка стека сообщений.
	 */
	void ClearMsgStack() {MsgList.clear();};
	/**
	 * Получение значения счетчика сообщений с предупреждениями.
	 * @return значение счетчика сообщений с предупреждениями.
	 */
	int GetNumWarnings() {return WarningsCount;};
	/**
	 * Получение значения счетчика сообщений с ошибками.
	 * @return значение счетчика сообщений с ошибками.
	 */
	int GetNumErrors() {return ErrorsCount;};
	/**
	 * Получение значения счетчика информационных сообщений.
	 * @return значение счетчика информационных сообщений.
	 */
	int GetNumInfos() {return MessagesCount;};
	/**
	 * Чтение первого сообщения.
	 * @param LOGGER_MSG* pMsg - сообщение.
	 * @return false, сообщений нет; true, если pMsg нулевой или сообщение прочитано успешно.
	 */
	bool ReadFirstMsg(LOGGER_MSG* pMsg);
	/**
	 * Чтение последнего сообщения.
	 * @param LOGGER_MSG* pMsg - сообщение.
	 * @return false, сообщений нет; true, если pMsg нулевой или сообщение прочитано успешно.
	 */
	bool ReadLastMsg(LOGGER_MSG* pMsg);
	/**
	 * Информационное сообщение (аргументы передаются перечислением).
	 * @param const char* str - строка сообщения.
	 */	
	void Message(const char* str,...);
	/**
	 * Сообщение с предупреждением (аргументы передаются перечислением).
	 * @param const char* str - строка сообщения.
	 */
	void Warning(const char* str,...);
	/**
	 * Сообщение об ошибке (аргументы передаются перечислением).
	 * @param const char* str - строка сообщения.
	 */
	void Error(const char* str,...);
	/**
	 * Информационное сообщение (аргументы передаются массивом va_list).
	 * @param const char* str - строка сообщения.
	 * @param va_list args - аргументы.
	 */
	void MessageV(const char* str,va_list args);
	/**
	 * Сообщение с предупреждением (аргументы передаются массивом va_list).
	 * @param const char* str - строка сообщения.
	 * @param va_list args - аргументы.
	 */
	void WarningV(const char* str,va_list args);
	/**
	 * Сообщение об ошибке (аргументы передаются массивом va_list).
	 * @param const char* str - строка сообщения.
	 * @param va_list args - аргументы.
	 */
	void ErrorV(const char* str,va_list args);
	/**
	 * Вывод информационного сообщения в лог (аргументы передаются перечислением).
	 * @param UINT MsgType - тип сообщения.
	 * @param char* Txt - сообщение.
	 */
	void BaseOut(UINT MsgType,const char* Txt,...);
	/**
	 * Вывод информационного сообщения в лог (аргументы передаются массивом va_list).
	 * @param UINT MsgType - тип сообщения.
	 * @param char* Txt - сообщение.
	 * @param va_list args - аргументы.
	 */	
	void BaseOutV(UINT MsgType,const char* Txt,va_list args);
	/**
	 * Вывод сообщения.
	 * @param LOGGER_MSG& Msg - сообщение.
	 */
	virtual void OnMessageOut(LOGGER_MSG& Msg);
private:
	/**
	 * Счетчик сообщений с предупреждениями.
	 */
	int WarningsCount;
	/**
	 * Счетчик сообщений с ошибками.
	 */
	int ErrorsCount;
	/**
	 * Счетчик информационных сообщений.
	 */
	int MessagesCount;
	/**
	 * Максимальное число сообщений.
	 */
	UINT MaxMessageCnt;
	/**
	 * todo.
	 */
	pthread_mutex_t CS1,CS2;
	/**
	 * Список сообщений.
	 */
	std::list<LOGGER_MSG> MsgList;
};


//////////LOGGER EXTERNAL//////
/**
 * Установка текущего лога.
 * @param Logger* pLogger - идентификатор лога.
 */
void SetLogger(Logger* pLogger);
/**
 * Вывод сообщения с ошибкой в лог.
 * @param char* Msg - сообщение.
 * @return false, если это не текущий лог, в противном случае - true.
 */
bool LogError(const char* Msg,...);
/**
 * Вывод сообщения с предупреждением в лог.
 * @param char* Msg - сообщение.
 * @return false, если это не текущий лог, в противном случае - true.
 */
bool LogWarning(const char* Msg,...);
/**
 * Вывод информационного сообщения в лог.
 * @param char* Msg - сообщение.
 * @return false, если это не текущий лог, в противном случае - true.
 */
bool LogInfo(const char* Msg,...);
///////////////////////////////
