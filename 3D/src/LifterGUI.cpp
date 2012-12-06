/* 
 * File:   LifterGUI.cpp
 * Author: kirill
 * 
 * Created on November 10, 2012, 10:27 PM
 */

#include "stdinclude.h"
#include "LifterGUI.h"
#include "SolverThread.h"

#define CHECK_OFFSET 400
#define GUI_UPDATE_TIME 100

LifterGUI::LifterGUI() {
    pReceiver=0;
    imgListOffsetY=0;
    pMarkImage=0;
    bEnableBloom=true;
    bPlayed=true;
}

LifterGUI::~LifterGUI() {
}

void LifterGUI::initGUI(IrrlichtDevice *device_, IVideoDriver* driver_,ISceneManager* smgr_,IGUIEnvironment* guienv_)
{
    release();
    device=device_;
    driver=driver_;
    smgr=smgr_;
    guienv=guienv_;
    
    pMarkTex=driver->getTexture("3D/res/textures/GUI/mov/mark.png");
    
    pClockTex=driver->getTexture("3D/res/textures/GUI/mov/clock-icon32.png");
    pUpTex=driver->getTexture("3D/res/textures/GUI/mov/up-icon32.png");
    pDownTex=driver->getTexture("3D/res/textures/GUI/mov/down-icon32.png");
    pLeftTex=driver->getTexture("3D/res/textures/GUI/mov/left-icon32.png");
    pRightTex=driver->getTexture("3D/res/textures/GUI/mov/right-icon32.png");
    pAbortTex=driver->getTexture("3D/res/textures/GUI/mov/block-icon32.png");
    
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
    toolBar->addButton(GUI_ID_BUTTON_SOLVE, 0, L"Solve",image, 0, false, true);
    
    image = driver->getTexture("3D/res/textures/GUI/play64.png");
    video::ITexture* image2=driver->getTexture("3D/res/textures/GUI/pause64.png");
    toolBar->addButton(GUI_ID_BUTTON_PLAY, 0, L"Play/Pause",image2, image, true, true);
    image = driver->getTexture("3D/res/textures/GUI/forward.png");
    toolBar->addButton(GUI_ID_BUTTON_NEXT, 0, L"Next step",image, 0, false, true);
    image = driver->getTexture("3D/res/textures/GUI/log64.png");
    toolBar->addButton(GUI_ID_BUTTON_OPEN_LOG, 0, L"Open/Close log",image, 0, false, true);
    
    image = driver->getTexture("3D/res/textures/GUI/mov/up-icon64.png");
    MoveButtons[0]= guienv->addButton(rect<s32>(64,192,128,256),0,GUI_ID_BUTTON_UP);
    MoveButtons[0]->setImage(image);
    MoveButtons[0]->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/down-icon64.png");
    MoveButtons[1]= guienv->addButton(rect<s32>(64,320,128,384),0,GUI_ID_BUTTON_DOWN);
    MoveButtons[1]->setImage(image);
    MoveButtons[1]->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/left-icon64.png");
    MoveButtons[2]= guienv->addButton(rect<s32>(0,256,64,320),0,GUI_ID_BUTTON_LEFT);
    MoveButtons[2]->setImage(image);
    MoveButtons[2]->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/right-icon64.png");
    MoveButtons[3]= guienv->addButton(rect<s32>(128,256,192,320),0,GUI_ID_BUTTON_RIGHT);
    MoveButtons[3]->setImage(image);
    MoveButtons[3]->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/clock-icon64.png");
    MoveButtons[4]= guienv->addButton(rect<s32>(64,256,128,320),0,GUI_ID_BUTTON_WAIT);
    MoveButtons[4]->setImage(image);
    MoveButtons[4]->setUseAlphaChannel(true);
    image = driver->getTexture("3D/res/textures/GUI/mov/block-icon64.png");
    MoveButtons[5]= guienv->addButton(rect<s32>(0,192,64,256),0,GUI_ID_BUTTON_ABORT);
    MoveButtons[5]->setImage(image);
    MoveButtons[5]->setUseAlphaChannel(true);
        
    
    logListbox = guienv->addListBox(rect<s32>(0, 0, window_dim.Width, 150));
    logListbox->setVisible(false);
    infoText=guienv->addStaticText (L"Camera pos: 0.0, 0.0, 0.0\r\nFPS: 0",rect<s32>(0,0,window_dim.Width,40));
    gameInfoText=guienv->addStaticText (L"",rect<s32>(window_dim.Width-100,window_dim.Height-60,window_dim.Width,window_dim.Height));
    gameInfoText->setOverrideColor(SColor(255,0,255,0));
    updateGameInfo();
    
    bloomCheck=guienv->addCheckBox(true,rect<s32>(CHECK_OFFSET,window_dim.Height-70,CHECK_OFFSET+30,window_dim.Height-50),0,GUI_ID_CHECK_BLOOM);
    bloomText=guienv->addStaticText (L"Bloom",rect<s32>(CHECK_OFFSET+30,window_dim.Height-68,CHECK_OFFSET+80,window_dim.Height-50));
    
    buttonsCheck=guienv->addCheckBox(true,rect<s32>(CHECK_OFFSET,window_dim.Height-50,CHECK_OFFSET+30,window_dim.Height-30),0,GUI_ID_CHECK_BUTTONS);
    buttonsText=guienv->addStaticText (L"Buttons",rect<s32>(CHECK_OFFSET+30,window_dim.Height-48,CHECK_OFFSET+80,window_dim.Height-30));
    
    speedCheck=guienv->addCheckBox(true,rect<s32>(CHECK_OFFSET,window_dim.Height-30,CHECK_OFFSET+30,window_dim.Height-10),0,GUI_ID_CHECK_SPEED);
    speedText=guienv->addStaticText (L"Speed",rect<s32>(CHECK_OFFSET+30,window_dim.Height-28,CHECK_OFFSET+80,window_dim.Height-10));
    
    animationScroll=guienv->addScrollBar(false,rect<s32>(window_dim.Width-25,192,window_dim.Width,window_dim.Height-192),0,GUI_ID_SCROLL_SPEED);
    animationScroll->setMin(0);
    animationScroll->setMax(2000);
    animationScroll->setPos(lifterScene.getAnimationSpeed());
    
    pMarkImage=guienv->addImage(pMarkTex,core::position2d< s32 >(0,imgListOffsetY));
    
    IrrLogger* pLogger=new IrrLogger();
    pLogger->Init(logListbox);
    SetLogger(pLogger);
    
    LOGINFO("Started");
    lifterScene.init(device,driver,smgr);
    
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
            case 'A': pathImageList[listPos]->setImage(pAbortTex); break;
            default:
                break;
        }
        pathImageList[listPos]->setRelativePosition(core::position2d< s32 >(listPos*32,imgListOffsetY+32));
        pathImageList[listPos]->setVisible(true);
    }
    for(u32 i=listPos+1;i<pathImageList.size();i++) pathImageList[i]->setVisible(false);
    
    //////////STATE////////
    listPos=-1;
    const wchar_t* End=0;
    for(u32 i=nCharFirst;i<nCharLast;i++)
    {
        listPos=i-nCharFirst;
        if(i>=robotStates.size()) break;
        switch(robotStates[i])
        {
            case ES_NONE: stateImageList[listPos]->setVisible(false); break;
            case ES_EAT_LAMBDA: stateImageList[listPos]->setImage(pEatLambdaTex); break;
            case ES_DESTROYED: stateImageList[listPos]->setImage(pDestroyedTex); End=L"Destroyed"; break;
            case ES_FINISHED: stateImageList[listPos]->setImage(pFinishedTex); End=L"Finished"; break;
            case ES_ABORTED: stateImageList[listPos]->setImage(pAbortedTex); End=L"Aborted"; break;
            case ES_WRONG_STEP: stateImageList[listPos]->setImage(pWrongStepTex); break;
            case ES_ERROR: stateImageList[listPos]->setImage(pErrorTex); break;
            default:
                break;
        }
        stateImageList[listPos]->setRelativePosition(core::position2d< s32 >(listPos*32,imgListOffsetY));
        if(robotStates[i]!=ES_NONE) stateImageList[listPos]->setVisible(true);
        if(End) break;
    }
    for(u32 i=listPos+1;i<stateImageList.size();i++) stateImageList[i]->setVisible(false);
    
    if(End) endGame(End);
}

void LifterGUI::onButtonMapSelected(wchar_t* Path)
{
    clearScene();
    if(!getScene().loadMap(Path))
    {
        wchar_t tmp[1024];
        swprintf(tmp,1024,L"Can't load map from file: %ls",Path);
        guienv->addMessageBox(L"Results:", tmp,true,EMBF_OK);
    }
}

void LifterGUI::clearScene()
{
    getScene().clear();
    robotPath.clear();
    robotStates.clear();
    currentRobotStep=0;
    updateGUI();
}

void LifterGUI::endGame(const wchar_t* reason)
{
    wchar_t tmp[1024];
    swprintf(tmp,1024,L"End of Game!\nReason: %ls \nScore: %d\nSteps: %d\nLambdas harvested: %d",
            reason,lifterScene.getScore(),lifterScene.getNumSteps(),lifterScene.getNumHarvestedLambdas());
    guienv->addMessageBox(L"Results:", tmp,true,EMBF_OK,0,GUI_ID_END_GAME);
    
}

void LifterGUI::onFrame()
{
    lifterScene.onFrame();
    if(bPlayed)
    {
        if(!lifterScene.isAnimation())
        {
            onButtonNext();
        }
    }
    onUpdateGUI();
}

void LifterGUI::onUpdateGUI()
{
    timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC,&currentTime);
    if(msecDiff(currentTime,prevGUIUpdateTime)<GUI_UPDATE_TIME) return;
    
    updateSolverState();
    
    clock_gettime(CLOCK_MONOTONIC,&prevGUIUpdateTime);
}

void LifterGUI::updateSolverState()
{
    if(solverStateTxt==0) return;
    wchar_t tmp[1024];
    int Time=solverTh.getTime();
    const wchar_t* StateTxt;
    if(solverTh.isRunning()) StateTxt=L"Processed";
    else StateTxt=L"Finished";
    
    swprintf(tmp,1024,L"State: %ls\nTime: %d:%d.%d (%ds)\nLambas collected: %d\nSteps: %d\nComplete: %.1f%%",
            StateTxt,Time/60000,(Time%60000)/1000,(Time%60000)%1000,Time/1000,
            solverTh.getLambdasCollected(),solverTh.getNumberOfSteps(),solverTh.getSolvingPercentage());
    solverStateTxt->setText(tmp);
    if(!solverTh.isRunning()) 
    {
        solverTh.stop();
        solverButton->setText(L"Close");
    }
}

void LifterGUI::onButtonSolve()
{
    if(lifterScene.getField()==0) return;
    robotPath.clear();
    robotStates.clear();
    currentRobotStep=0;
    updateGUI();
    
    SignalHandler::setupSignalHandler();
    
    solverTh.start((Field*)lifterScene.getField());
    solverWnd=guienv->addWindow(
                        rect<s32>(0, 0, 300, 200),
                        true, // modal?
                        L"Solver state:",0,GUI_ID_WND_SOLVER);
    solverStateTxt=guienv->addStaticText(L"Solver state:",
                        rect<s32>(35,35,265,165),
                        false, // border?
                        false, // wordwrap?
                        solverWnd);
    solverButton=guienv->addButton(rect<s32>(35,150,265,190),solverWnd,GUI_ID_BUTTON_STOP_SOLVER,L"Stop");
    solverWnd->setRelativePosition(core::position2di(window_dim.Width/2-150,window_dim.Height/2-100));
}

void LifterGUI::onStopSolver()
{
    solverTh.stop();
    robotPath+=solverTh.getResult();
    updateImageLists();
    
    solverWnd->remove();
    solverWnd=0;
    solverStateTxt=0;
    solverButton=0;
}

void LifterGUI::updateGUI()
{
    rect<s32> barRect=toolBar->getRelativePosition();
    toolBar->move(vector2d<s32>(0,(window_dim.Height-barRect.getSize().Height)-barRect.UpperLeftCorner.Y));
    
    logListbox->setRelativePosition(rect<s32>(0, 0, window_dim.Width, 150));
    int infoOffset=logListbox->isVisible()? 150 : 0;
    infoText->setRelativePosition(rect<s32>(0,infoOffset,window_dim.Width,infoOffset+40));
    
    gameInfoText->setRelativePosition(rect<s32>(window_dim.Width-100,window_dim.Height-60,window_dim.Width,window_dim.Height));
    
    bloomCheck->setRelativePosition(rect<s32>(CHECK_OFFSET,window_dim.Height-70,CHECK_OFFSET+30,window_dim.Height-50));
    bloomText->setRelativePosition(rect<s32>(CHECK_OFFSET+30,window_dim.Height-68,CHECK_OFFSET+80,window_dim.Height-50));
    
    buttonsCheck->setRelativePosition(rect<s32>(CHECK_OFFSET,window_dim.Height-50,CHECK_OFFSET+30,window_dim.Height-30));
    buttonsText->setRelativePosition(rect<s32>(CHECK_OFFSET+30,window_dim.Height-48,CHECK_OFFSET+80,window_dim.Height-30));
    
    speedCheck->setRelativePosition(rect<s32>(CHECK_OFFSET,window_dim.Height-30,CHECK_OFFSET+30,window_dim.Height-10));
    speedText->setRelativePosition(rect<s32>(CHECK_OFFSET+30,window_dim.Height-28,CHECK_OFFSET+80,window_dim.Height-10));
    
    animationScroll->setRelativePosition(rect<s32>(window_dim.Width-25,192,window_dim.Width,window_dim.Height-192));
 
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

void LifterGUI::updateGameInfo()
{
    wchar_t tmp[1024];
    swprintf(tmp,1024,L"Score: %d\nSteps: %d\nLambdas: %d",
            lifterScene.getScore(),lifterScene.getNumSteps(),lifterScene.getNumHarvestedLambdas());
    gameInfoText->setText(tmp);
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
        device->setEventReceiver(0);
        delete pReceiver;
        pReceiver=0;
    }
}