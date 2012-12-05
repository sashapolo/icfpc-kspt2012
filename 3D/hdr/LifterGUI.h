/* 
 * File:   LifterGUI.h
 * Author: kirill
 *
 * Created on November 10, 2012, 10:27 PM
 */

#include "LifterScene.h"


#pragma once

enum GUI_ID
{
    GUI_ID_BUTTON_OPEN_MAP,
    GUI_ID_BUTTON_SOLVE,
    GUI_ID_BUTTON_OPEN_LOG,
    GUI_ID_BUTTON_PLAY,
    GUI_ID_BUTTON_NEXT,
    GUI_ID_FD_MAPSELECT,
    GUI_ID_FD_PATHSELECT,
    GUI_ID_BUTTON_UP,
    GUI_ID_BUTTON_DOWN,
    GUI_ID_BUTTON_LEFT,
    GUI_ID_BUTTON_RIGHT,
    GUI_ID_BUTTON_WAIT,
    GUI_ID_BUTTON_ABORT,
    GUI_ID_END_GAME,
    GUI_ID_CHECK_BLOOM,
    GUI_ID_CHECK_BUTTONS,
    GUI_ID_CHECK_SPEED,
    GUI_ID_SCROLL_SPEED,
    GUI_ID_BUTTON_STOP_SOLVER,
    GUI_ID_WND_SOLVER
    
};
    
class MyEventReceiver;
class LifterGUI 
{
public:
    LifterGUI();
    virtual ~LifterGUI();
    
    void initGUI(IrrlichtDevice *device_, IVideoDriver* driver_,ISceneManager* smgr_,IGUIEnvironment* guienv_);
    void SetGUIColor(SColor col);
    void updateGUI();
    void updateImageLists();
    void onFrame();
    void resizeWindow(dimension2d<u32> newDim);
    
    void setInfo(const wchar_t* txt);
    LifterScene& getScene() {return lifterScene;};
    
    void onButtonOpenMap(){
        guienv->addFileOpenDialog(L"Select map file:",true,0,GUI_ID_FD_MAPSELECT);
    }
    void onButtonSolve();
    void onButtonMapSelected(wchar_t* Path);
    void onButtonPathSelected(wchar_t* Path){
        
    };
    void onButtonLog(){
        logListbox->setVisible(!logListbox->isVisible ());
        updateGUI();
    }
    void onButtonPlay()
    {
        bPlayed=!bPlayed;
    }
    void onButtonNext(){
        if(currentRobotStep<robotPath.size())
        {
                robotStates.push_back(lifterScene.step(robotPath[currentRobotStep++]));
                updateImageLists();
                updateGameInfo();
        };
    }
    void onButtonUp(){
        robotPath+='U';
        updateImageLists();
    }
    void onButtonDown(){
        robotPath+='D';
        updateImageLists();
    }
    void onButtonLeft(){
        robotPath+='L';
        updateImageLists();
    }
    void onButtonRight(){
        robotPath+='R';
        updateImageLists();
    }
    void onButtonWait(){
        robotPath+='W';
        updateImageLists();
    }
    void onButtonAbort(){
        robotPath+='A';
        updateImageLists();
    }
    
    void onButtonBloom()
    {
        bEnableBloom=!bEnableBloom;
    }
    
    void onButtonMoveButtons()
    {
        for(int i=0;i<6;i++) MoveButtons[i]->setVisible(!(MoveButtons[i]->isVisible()));
    }
    
    void onButtonAnimationSpeed()
    {
        animationScroll->setVisible(!(animationScroll->isVisible()));
    }
    
    void onSpeedScrollChanged()
    {
        lifterScene.setAnimationSpeed(animationScroll->getPos());
    }
    
    void onStopSolver();
    
    bool isBloomEnabled() {return bEnableBloom;};
    
    void updateSolverState();
    void updateGameInfo();
    void clearScene();
    void endGame(const wchar_t* reason=0);
    void release();
    
    
private:
    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;

    IGUIListBox * logListbox;
    IGUIStaticText* infoText;
    IGUIStaticText* gameInfoText;
    IGUIToolBar* toolBar;
    IGUIScrollBar* animationScroll;
    IGUICheckBox* bloomCheck;
    IGUICheckBox* buttonsCheck;
    IGUICheckBox* speedCheck;
    IGUIStaticText* bloomText;
    IGUIStaticText* buttonsText;
    IGUIStaticText* speedText;
    IGUIWindow* solverWnd;
    IGUIStaticText* solverStateTxt;
    IGUIButton* solverButton;
    
    std::string robotPath;
    std::vector<eEndState> robotStates;
    
    u32 currentRobotStep;
    IGUIImage* pMarkImage;
    std::vector<IGUIImage*> pathImageList;
    std::vector<IGUIImage*> stateImageList;
    int imgListOffsetY;
    
    dimension2d<u32> window_dim;
    
    LifterScene lifterScene;
    SolverThread solverTh;
    
    MyEventReceiver* pReceiver;
    
    ITexture* pMarkTex;
    
    ITexture* pClockTex;
    ITexture* pUpTex;
    ITexture* pDownTex;
    ITexture* pLeftTex;
    ITexture* pRightTex;
    ITexture* pAbortTex;
    
    ITexture* pEatLambdaTex;
    ITexture* pDestroyedTex;
    ITexture* pFinishedTex;
    ITexture* pAbortedTex;
    ITexture* pWrongStepTex;
    ITexture* pErrorTex;
    
    IGUIButton* MoveButtons[6];
    bool bEnableBloom;
    bool bPlayed;
};

class MyEventReceiver : public IEventReceiver
{
public:
    IGUIFileOpenDialog* dialog;
    IGUIElement* element;
    IGUIScrollBar* scroll;
    LifterGUI* pLifterGUI;
   virtual bool OnEvent(const SEvent& event)
   {
        if(event.EventType==EET_LOG_TEXT_EVENT)
        {
            switch(event.LogEvent.Level)
            {
                case ELL_INFORMATION: LogInfo("[Irrlicht] %s",event.LogEvent.Text); break;
                case ELL_WARNING: LogWarning("[Irrlicht] %s",event.LogEvent.Text); break;
                case ELL_ERROR: LogError("[Irrlicht] %s",event.LogEvent.Text); break;
                default:
                    LogInfo("[Irrlicht] %s",event.LogEvent.Text);
                    break;
            }
        }

        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();

            switch(event.GUIEvent.EventType)
            {
                case EGET_BUTTON_CLICKED:
                    switch(id)
                    {
                        case GUI_ID_BUTTON_OPEN_MAP:
                            pLifterGUI->onButtonOpenMap();
                            break;
                        case GUI_ID_BUTTON_OPEN_LOG:
                            pLifterGUI->onButtonLog();
                            break;
                        case GUI_ID_BUTTON_UP: pLifterGUI->onButtonUp(); break;
                        case GUI_ID_BUTTON_DOWN: pLifterGUI->onButtonDown(); break;
                        case GUI_ID_BUTTON_LEFT: pLifterGUI->onButtonLeft(); break;
                        case GUI_ID_BUTTON_RIGHT: pLifterGUI->onButtonRight(); break;
                        case GUI_ID_BUTTON_WAIT: pLifterGUI->onButtonWait(); break;
                        case GUI_ID_BUTTON_NEXT: pLifterGUI->onButtonNext(); break;
                        case GUI_ID_BUTTON_ABORT: pLifterGUI->onButtonAbort(); break;
                        case GUI_ID_BUTTON_PLAY: pLifterGUI->onButtonPlay(); break;
                        case GUI_ID_BUTTON_SOLVE: pLifterGUI->onButtonSolve(); break;
                        case GUI_ID_BUTTON_STOP_SOLVER: pLifterGUI->onStopSolver(); break;
                    }
                    break;
                case EGET_FILE_SELECTED:         
                    dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                    switch(dialog->getID())
                    {
                        case GUI_ID_FD_MAPSELECT: 
                            pLifterGUI->onButtonMapSelected((wchar_t*)dialog->getFileName());
                            break;
                        default: break;
                    };
                    break;
                case EGET_MESSAGEBOX_OK:
                    element = (IGUIElement*)event.GUIEvent.Caller;
                    switch(element->getID())
                    {
                        case GUI_ID_END_GAME:
                            pLifterGUI->clearScene();
                            break;
                        default: break;
                    }
                    break;
                case EGET_CHECKBOX_CHANGED:
                    element = (IGUIElement*)event.GUIEvent.Caller;
                    switch(element->getID())
                    {
                        case GUI_ID_CHECK_BLOOM:
                            pLifterGUI->onButtonBloom();
                            break;
                        case GUI_ID_CHECK_BUTTONS:
                            pLifterGUI->onButtonMoveButtons();
                            break;
                        case GUI_ID_CHECK_SPEED:
                            pLifterGUI->onButtonAnimationSpeed();
                            break;
                        default: break;
                    }
                    break;
                case EGET_SCROLL_BAR_CHANGED:
                    scroll = (IGUIScrollBar*)event.GUIEvent.Caller;
                    switch(scroll->getID())
                    {
                        case GUI_ID_SCROLL_SPEED:
                            pLifterGUI->onSpeedScrollChanged();
                            break;
                        default: break;
                    }
                    break;
                case EGET_ELEMENT_CLOSED :
                    element = (IGUIElement*)event.GUIEvent.Caller;
                    if(element->getID()==GUI_ID_WND_SOLVER)
                    {
                        pLifterGUI->onStopSolver();
                        return true;
                    }
                    break;
                default:
                    break;
            }
        }


      return false;
   }

};