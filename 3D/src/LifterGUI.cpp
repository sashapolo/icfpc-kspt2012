/* 
 * File:   LifterGUI.cpp
 * Author: kirill
 * 
 * Created on November 10, 2012, 10:27 PM
 */

#include "stdinclude.h"
#include "LifterGUI.h"

LifterGUI::LifterGUI() {
    pReceiver=0;
    imgListOffsetY=0;
    pMarkImage=0;
    bImmediateSteps=true;
}

LifterGUI::~LifterGUI() {
}

void LifterGUI::initGUI(IrrlichtDevice *device, IVideoDriver* driver_,ISceneManager* smgr_,IGUIEnvironment* guienv_)
{
    release();
    driver=driver_;
    smgr=smgr_;
    guienv=guienv_;
    
    pMarkTex=driver->getTexture("3D/res/textures/GUI/mov/mark.png");
    
    pClockTex=driver->getTexture("3D/res/textures/GUI/mov/clock-icon32.png");
    pUpTex=driver->getTexture("3D/res/textures/GUI/mov/up-icon32.png");
    pDownTex=driver->getTexture("3D/res/textures/GUI/mov/down-icon32.png");
    pLeftTex=driver->getTexture("3D/res/textures/GUI/mov/left-icon32.png");
    pRightTex=driver->getTexture("3D/res/textures/GUI/mov/right-icon32.png");
    
    pEatLambdaTex=driver->getTexture("3D/res/textures/GUI/mov/present-icon32.png");
    pDestroyedTex=driver->getTexture("3D/res/textures/GUI/mov/delete-icon32.png");
    pFinishedTex=driver->getTexture("3D/res/textures/GUI/mov/tick-icon32.png");
    pAbortedTex=driver->getTexture("3D/res/textures/GUI/mov/trash-icon32.png");
    pWrongStepTex=driver->getTexture("3D/res/textures/GUI/mov/block-icon32.png");
    pErrorTex=driver->getTexture("3D/res/textures/GUI/mov/warning-icon32.png");
    
    window_dim=driver->getScreenSize ();
    
    pReceiver=new MyEventReceiver();
    pReceiver->pLifterGUI=this;
    device->setEventReceiver(pReceiver);
    
    IGUISkin* skin = guienv->getSkin();
    IGUIFont* font = guienv->getFont("3D/res/fonts/fontlucida.png");
    IGUIButton* button=0;
    if (font)
            skin->setFont(font);

    skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);
    
    toolBar = guienv->addToolBar();
    rect<s32> barRect=toolBar->getRelativePosition();
    barRect.LowerRightCorner.Y=75;
    toolBar->setRelativePosition(barRect);
    video::ITexture* image = driver->getTexture("3D/res/textures/GUI/map64.png");
    toolBar->addButton(GUI_ID_BUTTON_OPEN_MAP, 0, L"Open a map",image, 0, false, true);
    image = driver->getTexture("3D/res/textures/GUI/footsteps64.png");
    toolBar->addButton(GUI_ID_BUTTON_OPEN_MAP, 0, L"Open a path",image, 0, false, true);
    
    image = driver->getTexture("3D/res/textures/GUI/play64.png");
    video::ITexture* image2=driver->getTexture("3D/res/textures/GUI/pause64.png");
    toolBar->addButton(GUI_ID_BUTTON_PLAY, 0, L"Play/Pause",image, image2, true, true);
    image = driver->getTexture("3D/res/textures/GUI/forward.png");
    toolBar->addButton(GUI_ID_BUTTON_NEXT, 0, L"Next step",image, 0, false, true);
    image = driver->getTexture("3D/res/textures/GUI/ufo_step64.png");
    image2 = driver->getTexture("3D/res/textures/GUI/ufo_immediate64.png");
    toolBar->addButton(GUI_ID_BUTTON_IMMEDIATE_STEPS, 0, L"Immediate//By step",image, image2, true, true);
    image = driver->getTexture("3D/res/textures/GUI/log64.png");
    toolBar->addButton(GUI_ID_BUTTON_OPEN_LOG, 0, L"Open/Close log",image, 0, false, true);
    
    image = driver->getTexture("3D/res/textures/GUI/mov/up-icon64.png");
    button= guienv->addButton(rect<s32>(64,192,128,256),0,GUI_ID_BUTTON_UP);
    button->setImage(image);
    button->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/down-icon64.png");
    button= guienv->addButton(rect<s32>(64,320,128,384),0,GUI_ID_BUTTON_DOWN);
    button->setImage(image);
    button->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/left-icon64.png");
    button= guienv->addButton(rect<s32>(0,256,64,320),0,GUI_ID_BUTTON_LEFT);
    button->setImage(image);
    button->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/right-icon64.png");
    button= guienv->addButton(rect<s32>(128,256,192,320),0,GUI_ID_BUTTON_RIGHT);
    button->setImage(image);
    button->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/clock-icon64.png");
    button= guienv->addButton(rect<s32>(64,256,128,320),0,GUI_ID_BUTTON_WAIT);
    button->setImage(image);
    button->setUseAlphaChannel(true);
        
    
    logListbox = guienv->addListBox(rect<s32>(0, 0, window_dim.Width, 150));
    logListbox->setVisible(false);
    infoText=guienv->addStaticText (L"Camera pos: 0.0, 0.0, 0.0\r\nFPS: 0",rect<s32>(0,0,window_dim.Width,40));
    
    pMarkImage=guienv->addImage(pMarkTex,core::position2d< s32 >(0,imgListOffsetY));
    
    IrrLogger* pLogger=new IrrLogger();
    pLogger->Init(logListbox);
    SetLogger(pLogger);
    
    LOGINFO("Started");
    lifterScene.init(driver,smgr);
    
    updateGUI();
}

void LifterGUI::updateImageLists()
{
    u32 screenChars=window_dim.Width/32;
    
    while(pathImageList.size()>screenChars) {pathImageList[pathImageList.size()-1]->remove(); pathImageList.pop_back();};
    while(stateImageList.size()>screenChars) {stateImageList[stateImageList.size()-1]->remove(); stateImageList.pop_back();};
    while(pathImageList.size()<screenChars) {pathImageList.push_back(guienv->addImage(pClockTex,core::position2d< s32 >(pathImageList.size()*32,imgListOffsetY+32)));};
    while(stateImageList.size()<screenChars) {stateImageList.push_back(guienv->addImage(pClockTex,core::position2d< s32 >(stateImageList.size()*32,imgListOffsetY)));};
    
    u32 nCharFirst=0;
    u32 nCharLast=robotPath.size();
    if(currentRobotStep>screenChars/2) nCharFirst=currentRobotStep-screenChars/2;
    if((nCharLast-nCharFirst)>screenChars) nCharLast=nCharFirst+screenChars;
    
    
    pMarkImage->setRelativePosition(core::position2d< s32 >((currentRobotStep-nCharFirst)*32,imgListOffsetY+32));
    if(robotPath.size()==0) pMarkImage->setVisible(false);
    else  pMarkImage->setVisible(true);
    //////////PATH////////
    u32 listPos=-1;
    for(u32 i=nCharFirst;i<nCharLast;i++)
    {
        listPos=i-nCharFirst;
        switch(robotPath[i])
        {
            case 'W': pathImageList[listPos]->setImage(pClockTex); break;
            case 'U': pathImageList[listPos]->setImage(pUpTex); break;
            case 'D': pathImageList[listPos]->setImage(pDownTex); break;
            case 'L': pathImageList[listPos]->setImage(pLeftTex); break;
            case 'R': pathImageList[listPos]->setImage(pRightTex); break;
            default:
                break;
        }
        pathImageList[listPos]->setRelativePosition(core::position2d< s32 >(listPos*32,imgListOffsetY+32));
        pathImageList[listPos]->setVisible(true);
    }
    for(u32 i=listPos+1;i<pathImageList.size();i++) pathImageList[i]->setVisible(false);
    
    //////////STATE////////
    listPos=-1;
    for(u32 i=nCharFirst;i<nCharLast;i++)
    {
        listPos=i-nCharFirst;
        if(i>=robotStates.size()) break;
        switch(robotStates[i])
        {
            case ES_NONE: stateImageList[listPos]->setVisible(false); break;
            case ES_EAT_LAMBDA: stateImageList[listPos]->setImage(pEatLambdaTex); break;
            case ES_DESTROYED: stateImageList[listPos]->setImage(pDestroyedTex); break;
            case ES_FINISHED: stateImageList[listPos]->setImage(pFinishedTex); break;
            case ES_ABORTED: stateImageList[listPos]->setImage(pAbortedTex); break;
            case ES_WRONG_STEP: stateImageList[listPos]->setImage(pWrongStepTex); break;
            case ES_ERROR: stateImageList[listPos]->setImage(pErrorTex); break;
            default:
                break;
        }
        stateImageList[listPos]->setRelativePosition(core::position2d< s32 >(listPos*32,imgListOffsetY));
        if(robotStates[i]!=ES_NONE) stateImageList[listPos]->setVisible(true);
    }
    for(u32 i=listPos+1;i<stateImageList.size();i++) stateImageList[i]->setVisible(false);
}

void LifterGUI::onFrame()
{
    lifterScene.updateScene();
}

void LifterGUI::updateGUI()
{
    rect<s32> barRect=toolBar->getRelativePosition();
    toolBar->move(vector2d<s32>(0,(window_dim.Height-barRect.getSize().Height)-barRect.UpperLeftCorner.Y));
    
    logListbox->setRelativePosition(rect<s32>(0, 0, window_dim.Width, 150));
    int infoOffset=logListbox->isVisible()? 150 : 0;
    infoText->setRelativePosition(rect<s32>(0,infoOffset,window_dim.Width,infoOffset+40));
    
    imgListOffsetY=(window_dim.Height-barRect.getSize().Height)-64;
    updateImageLists();
}

void LifterGUI::SetGUIColor(SColor col)
{
    for (u32 i=0; i<EGDC_COUNT ; ++i)
    {
        guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
}

void LifterGUI::setInfo(const wchar_t* txt)
{
    infoText->setText(txt);
}

void LifterGUI::resizeWindow(dimension2d<u32> newDim)
{
    window_dim=newDim;
    updateGUI();
}

void LifterGUI::release()
{
    lifterScene.release();
    if(pReceiver)
    {
        delete pReceiver;
        pReceiver=0;
    }
}