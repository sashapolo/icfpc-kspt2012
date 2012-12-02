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
    GUI_ID_BUTTON_OPEN_PATH,
    GUI_ID_BUTTON_OPEN_LOG,
    GUI_ID_BUTTON_PLAY,
    GUI_ID_BUTTON_NEXT,
    GUI_ID_BUTTON_IMMEDIATE_STEPS,
    GUI_ID_FD_MAPSELECT,
    GUI_ID_FD_PATHSELECT,
    GUI_ID_BUTTON_UP,
    GUI_ID_BUTTON_DOWN,
    GUI_ID_BUTTON_LEFT,
    GUI_ID_BUTTON_RIGHT,
    GUI_ID_BUTTON_WAIT,
    
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
    void onButtonOpenPath(){
        guienv->addFileOpenDialog(L"Select path file:",true,0,GUI_ID_FD_PATHSELECT);
    }
    void onButtonMapSelected(wchar_t* Path){
        getScene().loadMap(Path);
        robotPath.clear();
        robotStates.clear();
        currentRobotStep=0;
        updateGUI();
    }
    void onButtonPathSelected(wchar_t* Path){
        
    };
    void onButtonLog(){
        logListbox->setVisible(!logListbox->isVisible ());
        updateGUI();
    }
    void onButtonPlay();
    void onButtonNext(){
        if(currentRobotStep<robotPath.size())
        {
                robotStates.push_back(lifterScene.step(robotPath[currentRobotStep++]));
                updateImageLists();
        };
    }
    void onButtonUp(){
        robotPath+='U';
        updateImageLists();
        if(bImmediateSteps) onButtonNext();
    }
    void onButtonDown(){
        robotPath+='D';
        updateImageLists();
        if(bImmediateSteps) onButtonNext();
    }
    void onButtonLeft(){
        robotPath+='L';
        updateImageLists();
        if(bImmediateSteps) onButtonNext();
    }
    void onButtonRight(){
        robotPath+='R';
        updateImageLists();
        if(bImmediateSteps) onButtonNext();
    }
    void onButtonWait(){
        robotPath+='W';
        updateImageLists();
        if(bImmediateSteps) onButtonNext();
    }
    void onButtonImmediateSteps()
    {
        bImmediateSteps=!bImmediateSteps;
        if(bImmediateSteps)
        {
            robotPath.resize(currentRobotStep);
            if(robotStates.size()>currentRobotStep)
                robotStates.resize(currentRobotStep);
            updateImageLists();
        }
    };
    
    void release();
private:
    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;

    IGUIListBox * logListbox;
    IGUIStaticText* infoText;
    IGUIToolBar* toolBar;
    
    std::string robotPath;
    std::vector<eEndState> robotStates;
    
    u32 currentRobotStep;
    IGUIImage* pMarkImage;
    std::vector<IGUIImage*> pathImageList;
    std::vector<IGUIImage*> stateImageList;
    int imgListOffsetY;
    
    dimension2d<u32> window_dim;
    
    LifterScene lifterScene;
    
    MyEventReceiver* pReceiver;
    
    ITexture* pMarkTex;
    
    ITexture* pClockTex;
    ITexture* pUpTex;
    ITexture* pDownTex;
    ITexture* pLeftTex;
    ITexture* pRightTex;
    
    ITexture* pEatLambdaTex;
    ITexture* pDestroyedTex;
    ITexture* pFinishedTex;
    ITexture* pAbortedTex;
    ITexture* pWrongStepTex;
    ITexture* pErrorTex;
    
    bool bImmediateSteps;
};

class MyEventReceiver : public IEventReceiver
{
public:
    IGUIFileOpenDialog* dialog;
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
            switch(event.GUIEvent.EventType)
            {
                case EGET_FILE_SELECTED:         
                    dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                    switch(dialog->getID())
                    {
                        case GUI_ID_FD_MAPSELECT: 
                            pLifterGUI->onButtonMapSelected((wchar_t*)dialog->getFileName());
                            break;
                        case GUI_ID_FD_PATHSELECT:
                            pLifterGUI->onButtonPathSelected((wchar_t*)dialog->getFileName());
                            break;
                        default: break;
                    };
                    return true;
                
                default:
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
                        case GUI_ID_BUTTON_IMMEDIATE_STEPS: pLifterGUI->onButtonImmediateSteps(); break;
                    }
                default:
                    break;
            }
        }


      return false;
   }

};