#pragma once
#include <fstream>

class HTMLLogger : public Logger
{
public:
	HTMLLogger(void);
	~HTMLLogger(void);

	void Init(char* FilePath, char* LogName);
	void Close();
	void OnMessageOut(LOGGER_MSG& Msg);
private:

	void  Write(const char* color, const char* string);
	void StartBanner(const char *logname);
	void EndBanner();
	void WriteTime(time_t time);

	std::ofstream  LogFile;
	bool bLogInit;
};

