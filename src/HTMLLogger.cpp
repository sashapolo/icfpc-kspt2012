#include "HTMLLogger.h"


HTMLLogger::HTMLLogger(void)
{
	bLogInit=false;
}


HTMLLogger::~HTMLLogger(void)
{
    Close();
}

void HTMLLogger::Init(const char* FilePath, const char* LogName)
{
	Close();

	if(FilePath==NULL) LogFile.open("./LOG.html",std::ios::out | std::ios::trunc);
	else LogFile.open(FilePath,std::ios::out | std::ios::trunc);

	bLogInit=true;
    if(LogName==NULL) StartBanner("Global Log");
	else StartBanner(LogName);
};

void HTMLLogger::Close()
{
	if(bLogInit)
	{
		EndBanner();
		LogFile.close();
		bLogInit=false;
	};
};

void HTMLLogger::OnMessageOut(LOGGER_MSG& Msg)
{
	std::string MsgType;

        WriteTime(Msg.Time);
	switch(Msg.MsgType)
	{
	case LOGGER_MSG_TYPE_INFO:	Write("#00FF00",Msg.String.c_str()); break;
	case LOGGER_MSG_TYPE_WARNING:	Write("#FFFF00",Msg.String.c_str()); break;
	case LOGGER_MSG_TYPE_ERROR:	Write("#FF0000",Msg.String.c_str()); break;
	default:	Write("#FFFFFF",Msg.String.c_str()); break;
	};
}

void  HTMLLogger::Write(const char* color, const char* string)
{
	if(!bLogInit) return;
	if (!string) return;
	
    LogFile<<"<Font Size = 2 Color = "<<color
            <<" Face = Verdana>"<<string<<"</font><BR>";
	LogFile.flush();
}

void HTMLLogger::StartBanner(const char *logname)
{
	if(!bLogInit) return;
	LogFile<<"<html>";
    LogFile<<"<title>Log</title>";
    LogFile<<"<meta http-equiv=\"Content-Type\"content=\"text/html; charset=windows-1251\">";
    LogFile<<"<body bgcolor=\"#000000\" text=\"#aaaaaa\">";
    LogFile<<"<pre>";
    LogFile<<"<h1>Work Log - "<<logname<<"</h1><BR>";
    LogFile<<"<h3><NoBR>----------------------------- Begin ------------------------------</NoBR></h3>";
	LogFile.flush();
}

void HTMLLogger::EndBanner()
{
	if(!bLogInit) return;
	LogFile<<"<h3><NoBR>---------------------------- Results -----------------------------</NoBR></h3>";
	LogFile<<"<B>"<<GetNumInfos()<<" message(s), "<<GetNumErrors()<<" error(s), "<<GetNumWarnings()<<" warning(s)";
    LogFile<<"</B></pre></body></html>";
}

void HTMLLogger::WriteTime(time_t time)
{
	if(!bLogInit) return;

        tm * timeinfo;
        timeinfo = localtime ( &time );

    LogFile<<"<Font Size = 1 Color = #ABABAB Face = Verdana><B>";
    LogFile<<timeinfo->tm_hour<<":"<<timeinfo->tm_min<<":"<<timeinfo->tm_sec<<" - ";
    LogFile<<"</B></font>";
	LogFile.flush();
};