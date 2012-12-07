#include "stdinclude.h"
#define UPDATE_TIME 500

#include "Logger.h"
#include "LifterGUI.h"

LifterGUI lifterGUI;

IrrlichtDevice *device;
IVideoDriver* driver;
ISceneManager* smgr;
IGUIEnvironment* guienv;
    
dimension2d<u32> window_dim;
IPostProcessBlur *Blur;
IPostProcessBloom *Bloom;

s32 bloomShaderID;
s32 blurShaderID;
void onResizeWindow(dimension2d<u32> newDim);

int main()
{
    wchar_t tmp[1024];
    window_dim=dimension2d<u32>(800,600);
    device = createDevice( video::EDT_OPENGL, window_dim,32,false,true);

    if (!device)
            return 1;
    
    device->setWindowCaption(L"Lifter 3D");

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
    
    Blur = new IPostProcessBlur(smgr->getRootSceneNode(), smgr, 666);  
    Bloom = new IPostProcessBloom(smgr->getRootSceneNode(), smgr, 666);
    Blur->initiate(window_dim.Width,window_dim.Height,0.008,smgr);   
    
    sPostBloomSetup setup;   
    setup.sampleDist=0.008;   
    setup.strength=0.2;   
    setup.multiplier=3;   
    Bloom->initiate(window_dim.Width,window_dim.Height,setup,smgr);
    
    lifterGUI.initGUI(device,driver,smgr,guienv);

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
            lifterGUI.setInfo(tmp);
        }
        
        lifterGUI.onFrame();
        
        driver->beginScene(true, true, SColor(255,100,101,140));

        if(lifterGUI.isBloomEnabled())
        {
            driver->setRenderTarget(Blur->rt0, true, true, video::SColor(0,0,0,0));
            smgr->drawAll();
            driver->setRenderTarget(Bloom->rt0, true, true, video::SColor(0,0,0,0)); 
            Blur->render();
            driver->setRenderTarget(0);     
            Bloom->Material.setTexture(1,Blur->rt0);
            Bloom->render();
        }
        else
        {
            smgr->drawAll();
        }
    
        guienv->drawAll();
        driver->endScene();
    }

    
    lifterGUI.release();
    device->drop();
    delete Blur;
    delete Bloom;
    return 0;
};

void onResizeWindow(dimension2d<u32> newDim)
{
    window_dim=newDim;
    smgr->getActiveCamera()->setAspectRatio(float(window_dim.Width)/float(window_dim.Height));
    Blur->resize(driver,window_dim.Width,window_dim.Height);    
    Bloom->resize(driver,window_dim.Width,window_dim.Height);
    lifterGUI.resizeWindow(window_dim);
}

