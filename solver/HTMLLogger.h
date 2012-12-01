#pragma once

#include <fstream>

#include "Logger.h"

class HTMLLogger : public Logger
{
public:
	/**
	 * Констрктор класса HTMLLogger.
	 * Обнуляет булевую переменную инициализации логгера.
	 */
	HTMLLogger(void);
	/**
	 * Деструктор класса HTMLLogger.
	 * Закрывает текущий логгер.
	 */
	~HTMLLogger(void);
	/**
	 * Инициализация.
	 * Установка пути к файлу лога и его имени.
	 * @param char* FilePath - путь к файлу.
	 * @param char* LogName - имя файла.
	 */
	void Init(const char* FilePath, const char* LogName);
	/**
	 * Закрытие логгера.
	 * Завершает текущий лог, если он был инициализирован.
	 */
	void Close();
	/**
	 * Запись сообщений логгера.
	 * В зависимости от типа сообщения, используется свой цвет записи.
	 * @param LOGGER_MSG& Msg - сообщение логгера.
	 */
	void OnMessageOut(LOGGER_MSG& Msg);
private:
	/**
	 * Запись данных.
	 * @param char* color - цвет шрифта.
	 * @param char* string - данные для записи.
	 */
	void  Write(const char* color, const char* string);
	/**
	 * Запись заголовка.
	 * @param char *logname - имя лог-файла.
	 */
	void StartBanner(const char *logname);
	/**
	 * Запись концовки.
	 */
	void EndBanner();
	/**
	 * Запись времени.
	 * @param time_t time - время.
	 */
	void WriteTime(time_t time);

	/**
	 * Файл лога.
	 */
	std::ofstream  LogFile;
	/**
	 * Переменная инициализации текущего лога.
	 */
	bool bLogInit;
};

