#pragma once

class IrrLogger : public Logger
{
public:
    IrrLogger(void);
    ~IrrLogger(void);

    void Init(irr::gui::IGUIListBox * pListbox_);
    void Close();
    void OnMessageOut(LOGGER_MSG& Msg);
    void addItem(wchar_t* pStr, UINT Type);
    
    bool isInit() {return (bool)pListbox;};
private:
    irr::gui::IGUIListBox * pListbox;

    int alpha;
};
