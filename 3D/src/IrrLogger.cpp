#include "stdinclude.h"
#include "IrrLogger.h"

IrrLogger::IrrLogger() {
    pListbox=0;
    alpha=255;
}

IrrLogger::~IrrLogger() {
}

void IrrLogger::Init(irr::gui::IGUIListBox * pListbox_)
{
    pListbox=pListbox_;
}

void IrrLogger::Close()
{
}

void IrrLogger::OnMessageOut(LOGGER_MSG& Msg)
{
    tm * timeinfo;
    timeinfo = localtime ( &Msg.Time );
    
    
    int buflen=snprintf(0,0,"%d:%d:%d %s",timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,Msg.String.c_str());
    buflen++;
    wchar_t* pBuf=new wchar_t[buflen];
    swprintf(pBuf,buflen,L"%d:%d:%d %s",timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,Msg.String.c_str());
    
    int i0=0;
    int i1=0;
    for(i1=0;i1<buflen;i1++)
    {
        if(((pBuf[i1]=='\n') || (i1==buflen-1)) && ((i1-i0)>0))
        {
            wchar_t* tmp=new wchar_t[(i1-i0)+1];
            wcsncpy(tmp,pBuf+i0,i1-i0);
            tmp[(i1-i0)]='\0';
            addItem(tmp,Msg.MsgType);
            wprintf(tmp);
            i0=i1+1;
            delete [] tmp;
        }
    }

    delete [] pBuf;
}

void IrrLogger::addItem(wchar_t* pStr, UINT Type)
{
    int nItem=pListbox->addItem(pStr);
    
    switch(Type)
    {
    case LOGGER_MSG_TYPE_INFO:          pListbox->setItemOverrideColor(nItem,irr::video::SColor(alpha,0,255,0)); break;
    case LOGGER_MSG_TYPE_WARNING:	pListbox->setItemOverrideColor(nItem,irr::video::SColor(alpha,255,255,0)); break;
    case LOGGER_MSG_TYPE_ERROR:         pListbox->setItemOverrideColor(nItem,irr::video::SColor(alpha,255,0,0)); break;
    default:                            pListbox->setItemOverrideColor(nItem,irr::video::SColor(alpha,255,0,255)); break;
    };
        
    pListbox->setSelected(nItem);
}