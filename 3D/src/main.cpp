#include "stdinclude.h"
#define UPDATE_TIME 500

IVideoDriver* driver=0;
ISceneManager* smgr=0;
IGUIEnvironment* guienv=0;
LifterScene lifterScene;

void SetGUIColor(SColor col);

class MyEventReceiver : public IEventReceiver
{
public:
    IGUIFileOpenDialog* dialog;
   virtual bool OnEvent(const SEvent& event)
   {
      if (event.EventType == EET_GUI_EVENT)
      {
         s32 id = event.GUIEvent.Caller->getID();

         switch(event.GUIEvent.EventType)
         {

            case EGET_BUTTON_CLICKED:
               switch(id)
               {
                  case 20:
                     //guienv->addFileOpenDialog(L"Select a file, then click OK", true, 0, 10 );
                     return true;
               }

            case EGET_FILE_SELECTED:         
               dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
               lifterScene.loadMap((wchar_t*)dialog->getFileName());
               return true;
            

         default:
            break;
         }
      }


      return false;
   }

};

int main()
{
    timeval currTime;
    timeval prevUpdateTime;
    wchar_t tmp[1024];
    
    dimension2d<u32> window_dim=dimension2d<u32>(800,600);
    IrrlichtDevice *device =
            createDevice( video::EDT_OPENGL, window_dim,32,false,true);

    if (!device)
            return 1;

    device->setWindowCaption(L"Irr test");
    
    MyEventReceiver receiver;
    device->setEventReceiver(&receiver);

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    device->getCursorControl()->setVisible(true);
    
    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    smgr->addCameraSceneNodeFPS( 0, 50.0f, 0.5f );//addCameraSceneNode(0, vector3df(0,0,400), vector3df(5,5,0));

    vector3df lightrot=vector3df(50,50,-50);
    
    
    
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
                driver->getTexture("3D/res/textures/sky/irrlicht2_up.jpg"),
                driver->getTexture("3D/res/textures/sky/irrlicht2_dn.jpg"),
                driver->getTexture("3D/res/textures/sky/irrlicht2_lf.jpg"),
                driver->getTexture("3D/res/textures/sky/irrlicht2_rt.jpg"),
                driver->getTexture("3D/res/textures/sky/irrlicht2_ft.jpg"),
                driver->getTexture("3D/res/textures/sky/irrlicht2_bk.jpg"));
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    IGUISkin* skin = guienv->getSkin();
    IGUIFont* font = guienv->getFont("3D/res/fonts/fontlucida.png");
    if (font)
            skin->setFont(font);

    skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);
    
    IGUIListBox * log_listbox = guienv->addListBox(rect<s32>(0, 0, window_dim.Width, 80));
    IGUIStaticText* text=guienv->addStaticText (L"text",rect<s32>(0,80,window_dim.Width,120));
    //SetGUIColor(SColor(128,0,0,0));
    
    IrrLogger* pLogger=new IrrLogger();
    pLogger->Init(log_listbox);
    SetLogger(pLogger);
    
    gettimeofday(&prevUpdateTime,0);
    
    
    
    LOGINFO("Started");
    lifterScene.init(driver,smgr);
    lifterScene.loadMap(L"res/maps/map1");
    IGUIFileOpenDialog* pDial=guienv->addFileOpenDialog(L"Select map file:");
    
    while(device->run())
    {
        gettimeofday(&currTime,0);
        if(abs(currTime.tv_usec-prevUpdateTime.tv_usec)>UPDATE_TIME)
        {
            swprintf(tmp,1024,L"Camera pos: %.2f, %.2f, %.2f\r\nFPS: %d",smgr->getActiveCamera()->getPosition().X,smgr->getActiveCamera()->getPosition().Y,smgr->getActiveCamera()->getPosition().Z,driver->getFPS());
            text->setText(tmp);
            prevUpdateTime.tv_usec=currTime.tv_usec;
        }
        
        lightrot.rotateXZBy(0.01);
        //light1->setPosition(lightrot);
        
        driver->beginScene(true, true, SColor(255,100,101,140));

        smgr->drawAll();

        guienv->drawAll();
        driver->endScene();
    }

    device->drop();

    return 0;
};


void SetGUIColor(SColor col)
{
    for (u32 i=0; i<EGDC_COUNT ; ++i)
    {
        guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
}
