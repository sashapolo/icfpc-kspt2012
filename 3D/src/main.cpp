#include "stdinclude.h"
#define UPDATE_TIME 500

#include "Logger.h"

IVideoDriver* driver=0;
ISceneManager* smgr=0;
IGUIEnvironment* guienv=0;
LifterScene lifterScene;

dimension2d<u32> window_dim;
IGUIListBox * logListbox;
IGUIStaticText* infoText;
gui::IGUIToolBar* toolBar;

enum GUI_ID
{
    GUI_ID_BUTTON_OPEN_MAP,
    GUI_ID_BUTTON_OPEN_LOG,
    GUI_ID_BUTTON_PLAY,
    GUI_ID_FD_MAPSELECT
    
};

void SetGUIColor(SColor col);
void initGUI();
void updateGUI();
void onResizeWindow(dimension2d<u32> newDim);

class MyEventReceiver : public IEventReceiver
{
public:
    IGUIFileOpenDialog* dialog;
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
                    if(dialog->getID()==GUI_ID_FD_MAPSELECT)
                        lifterScene.loadMap((wchar_t*)dialog->getFileName());
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
                        guienv->addFileOpenDialog(L"Select map file:",true,0,GUI_ID_FD_MAPSELECT);
                        break;
                    case GUI_ID_BUTTON_OPEN_LOG:
                        logListbox->setVisible(!logListbox->isVisible ());
                        updateGUI();
                        break;
                    }
                default:
                    break;
            }
        }


      return false;
   }

};

int main()
{
    wchar_t tmp[1024];
    
    window_dim=dimension2d<u32>(800,600);
    IrrlichtDevice *device =
            createDevice( video::EDT_OPENGL, window_dim,32,false,true);

    if (!device)
            return 1;
    
    device->setWindowCaption(L"Lifter 3D");
    
    MyEventReceiver receiver;
    device->setEventReceiver(&receiver);

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    device->getCursorControl()->setVisible(true);
    
    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    smgr->addCameraSceneNode();

    smgr->setAmbientLight(video::SColorf(0.3f, 0.3f, 0.3f, 1.0f)); 
    
    
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    smgr->addSkyBoxSceneNode(
                driver->getTexture("3D/res/textures/sky/posY.jpg"),
                driver->getTexture("3D/res/textures/sky/negY.jpg"),
                driver->getTexture("3D/res/textures/sky/negZ.jpg"),
                driver->getTexture("3D/res/textures/sky/posZ.jpg"),
                driver->getTexture("3D/res/textures/sky/posX.jpg"),
                driver->getTexture("3D/res/textures/sky/negX.jpg"));
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
    

    initGUI();
    
    IPostProcessBlur *Blur1 = new IPostProcessBlur(smgr->getRootSceneNode(), smgr, 666);   
    Blur1->initiate(window_dim.Width*2,window_dim.Height*2,0.008,smgr);   
    IPostProcessBlur *Blur2 = new IPostProcessBlur(smgr->getRootSceneNode(), smgr, 666);   
    Blur2->initiate(window_dim.Width*2,window_dim.Height*2,0.004,smgr);
    
    IPostProcessBloom *Bloom = new IPostProcessBloom(smgr->getRootSceneNode(), smgr, 666);   
    sPostBloomSetup setup;   
   
    setup.sampleDist=0.008;   
    setup.strength=0.2;   
    setup.multiplier=3;   
    Bloom->initiate(window_dim.Width*2,window_dim.Height*2,setup,smgr); 
    
    //SetGUIColor(SColor(128,0,0,0));
    
    IrrLogger* pLogger=new IrrLogger();
    pLogger->Init(logListbox);
    SetLogger(pLogger);
    
    LOGINFO("Started");
    lifterScene.init(driver,smgr);
    
    int lastFPS=driver->getFPS();
    vector3df lastCamPos=smgr->getActiveCamera()->getPosition();
    while(device->run())
    {
        if(window_dim!=driver->getScreenSize ())
        {
            onResizeWindow(driver->getScreenSize());
        }
        
        if((lastFPS!=driver->getFPS()) || (lastCamPos!=smgr->getActiveCamera()->getPosition()))
        {
            lastFPS=driver->getFPS();
            lastCamPos=smgr->getActiveCamera()->getPosition();
            swprintf(tmp,1024,L"Camera pos: %.2f, %.2f, %.2f\r\nFPS: %d",lastCamPos.X,lastCamPos.Y,lastCamPos.Z,lastFPS);
            infoText->setText(tmp);
        }
        
        driver->beginScene(true, true, SColor(255,100,101,140));

        driver->setRenderTarget(Blur1->rt0, true, true, video::SColor(0,0,0,0));
        smgr->drawAll();
        driver->setRenderTarget(Bloom->rt0, true, true, video::SColor(0,0,0,0)); 
        Blur1->render();
        driver->setRenderTarget(0);     
        Bloom->Material.setTexture(1,Blur1->rt0);
        Bloom->render();
//        driver->setRenderTarget(Blur1->rt0, true, true, video::SColor(0,0,0,0));
//        smgr->drawAll();
//        driver->setRenderTarget(Blur2->rt0, true, true, video::SColor(0,0,0,0));           
//        Blur1->render();                                                                                                          
//        driver->setRenderTarget(0);                                            
//        Blur2->render();
    
        guienv->drawAll();
        driver->endScene();
    }

    device->drop();

    return 0;
};

void initGUI()
{
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
    image = driver->getTexture("3D/res/textures/GUI/play64.png");
    video::ITexture* image2=driver->getTexture("3D/res/textures/GUI/pause64.png");
    toolBar->addButton(GUI_ID_BUTTON_PLAY, 0, L"Play/Pause",image, image2, true, true);
    image = driver->getTexture("3D/res/textures/GUI/forward.png");
    toolBar->addButton(GUI_ID_BUTTON_PLAY, 0, L"Next step",image, 0, false, true);
    image = driver->getTexture("3D/res/textures/GUI/log64.png");
    toolBar->addButton(GUI_ID_BUTTON_OPEN_LOG, 0, L"Open/Close log",image, 0, false, true);
        
    
    logListbox = guienv->addListBox(rect<s32>(0, 0, window_dim.Width, 150));
    logListbox->setVisible(false);
    infoText=guienv->addStaticText (L"Camera pos: 0.0, 0.0, 0.0\r\nFPS: 0",rect<s32>(0,0,window_dim.Width,40));
    
    onResizeWindow(window_dim);
}

void updateGUI()
{
    rect<s32> barRect=toolBar->getRelativePosition();
    toolBar->move(vector2d<s32>(0,(window_dim.Height-barRect.getSize().Height)-barRect.UpperLeftCorner.Y));
    
    logListbox->setRelativePosition(rect<s32>(0, 0, window_dim.Width, 150));
    int infoOffset=logListbox->isVisible()? 150 : 0;
    infoText->setRelativePosition(rect<s32>(0,infoOffset,window_dim.Width,infoOffset+40));
}

void onResizeWindow(dimension2d<u32> newDim)
{
    window_dim=newDim;
    updateGUI();
}


void SetGUIColor(SColor col)
{
    for (u32 i=0; i<EGDC_COUNT ; ++i)
    {
        guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
}
