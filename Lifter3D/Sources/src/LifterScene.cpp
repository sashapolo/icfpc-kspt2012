#include "stdinclude.h"
#include "LifterScene.h"
#define CELLSIZE 10.0f

LifterScene::LifterScene() {
    driver=0;
    smgr=0;
    pField=0;
    
    earth_ind=0;
    wall_ind=0;
    
    pWallTex=0;
    pEarthTex=0;
}


LifterScene::~LifterScene() {
}

void LifterScene::init(IVideoDriver* driver_, ISceneManager* smgr_)
{
    driver=driver_;
    smgr=smgr_;
    
    pWallTex=driver->getTexture(L"res/textures/wall.png");
    pEarthTex=driver->getTexture(L"res/textures/earth.png");
    pEarthBump=driver->getTexture(L"res/textures/earth_n.png");
    pWallBump=driver->getTexture(L"res/textures/wall_n.tga");
    pStoneTex=driver->getTexture(L"res/textures/stone.jpg");
    pRobotTex=driver->getTexture(L"res/textures/ufo.png");
    pFireTex=driver->getTexture(L"res/textures/fireball.bmp");
    
    pStoneMesh=smgr->getMesh(L"res/models/stone.3ds");
    pLambdaMesh=smgr->getMesh(L"res/models/lambda.3DS");
    pRobotMesh=smgr->getMesh(L"res/models/ufo.3DS");
}

bool LifterScene::loadMap(wchar_t* Path)
{
    release();
    int len=wcslen(Path)+1;
    char* mbPath=new char[len];
    for(int i=0;i<len-1;i++) mbPath[i]=(char)Path[i];
    mbPath[len-1]='\0';
    pField=createField( mbPath);
    
    if(!pField) return false;
  
    mbWall.init(pField->getSize().first,pField->getSize().second,CELLSIZE,
            float(pField->getSize().first)/5.f*2.f, float(pField->getSize().second)/5.f*2.f,
            driver,smgr);
    mbEarth.init(pField->getSize().first,pField->getSize().second,CELLSIZE,
            float(pField->getSize().first)/5.f*2.f, float(pField->getSize().second)/5.f*2.f,
            driver,smgr);

    earth_ind=new char[pField->getSize().first*pField->getSize().second];
    wall_ind=new char[pField->getSize().first*pField->getSize().second];

    updateIndices();
    //memset(wall_ind,0,pField->getSize().first*pField->getSize().second);

    pWallMeshBufferNode=smgr -> addMeshSceneNode(mbWall.mesh);
    pWallMeshBufferNode->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
    pWallMeshBufferNode->setMaterialFlag(video::EMF_LIGHTING, true);
    pWallMeshBufferNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    pWallMeshBufferNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    pWallMeshBufferNode->setMaterialTexture(0,pWallTex);
    pWallMeshBufferNode->setMaterialTexture(1,pWallBump);
    pWallMeshBufferNode->getMaterial(0).MaterialTypeParam = 0.035f;

    pEarthMeshBufferNode=smgr -> addMeshSceneNode(mbEarth.mesh);
    pEarthMeshBufferNode->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
    pEarthMeshBufferNode->setMaterialFlag(video::EMF_LIGHTING, true);
    pEarthMeshBufferNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    pEarthMeshBufferNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    pEarthMeshBufferNode->setMaterialTexture(0,pEarthTex);
    pEarthMeshBufferNode->setMaterialTexture(1,pEarthBump);
    pEarthMeshBufferNode->getMaterial(0).MaterialTypeParam = 0.035f;

    scene::ISceneNode* pNode;
    std::list<FieldMember*>::iterator it=pField->getStoneCacheIt();
    int x,y;
    while(it!=pField->getStoneCacheEnd())
    {
        x=(*it)->getCoordinate().x;
        y=(*it)->getCoordinate().y;
        
        pNode=smgr -> addMeshSceneNode(pStoneMesh);
        pNode->setPosition(vector3df(x*CELLSIZE,(pField->getSize().second-y)*CELLSIZE-CELLSIZE*1.5,0));
        pNode->setScale(vector3df(CELLSIZE,CELLSIZE,CELLSIZE));
        
        pNode->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
        pNode->setMaterialFlag(video::EMF_LIGHTING, true);
        pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
        pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
        pNode->setMaterialTexture(0,pStoneTex);
        
        StoneArr.push_back(pNode);
        it++;
    }
    
    it=pField->getLambdaCacheIt();
    while(it!=pField->getLambdaCacheEnd())
    {
        x=(*it)->getCoordinate().x;
        y=(*it)->getCoordinate().y;
        
        pNode=smgr -> addMeshSceneNode(pLambdaMesh);
        pNode->setPosition(vector3df(x*CELLSIZE,(pField->getSize().second-y)*CELLSIZE-CELLSIZE*1.5,0));
        pNode->setScale(vector3df(CELLSIZE,CELLSIZE,CELLSIZE));
        
        pNode->setMaterialType(video::EMT_SOLID);
        pNode->setMaterialFlag(video::EMF_LIGHTING, true);
        pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
        pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
        
        scene::ISceneNodeAnimator* anim =
                        smgr->createRotationAnimator(core::vector3df(0,-0.4f,0));
                pNode->addAnimator(anim);
        anim->drop();
        
        LambdaArr.push_back(pNode);
        it++;
    }
    
    x=pField->getRobot()->getCoordinate().x;
    y=pField->getRobot()->getCoordinate().y;
    pNode=smgr -> addMeshSceneNode(pRobotMesh);
    pNode->setPosition(vector3df(x*CELLSIZE,(pField->getSize().second-y)*CELLSIZE-CELLSIZE,0));
    pNode->setScale(vector3df(0.05,0.05,0.05));

    pNode->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
    pNode->setMaterialFlag(video::EMF_LIGHTING, true);
    pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);    
    pNode->setMaterialTexture(0,pRobotTex);
    pNode->setMaterialTexture(1,pEarthBump);
    
    scene::ISceneNodeAnimator* anim =
                        smgr->createRotationAnimator(core::vector3df(0,0.5f,0));
                pNode->addAnimator(anim);
    anim->drop();
    
    scene::IParticleSystemSceneNode* ps =
                smgr->addParticleSystemSceneNode(false, pNode);

    // create and set emitter
    scene::IParticleEmitter* em = ps->createBoxEmitter(
            core::aabbox3d<f32>(-3,0,-3,3,1,3),
            core::vector3df(0.0f,-0.03f,0.0f),
            80,110,
            video::SColor(0,255,255,255), video::SColor(0,255,255,255),
            60,180);
    em->setMinStartSize(core::dimension2d<f32>(3.0f, 4.0f));
    em->setMaxStartSize(core::dimension2d<f32>(3.0f, 4.0f));

    ps->setEmitter(em);
    em->drop();
    
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, pFireTex);
    ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

    // create and set affector
    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
    ps->addAffector(paf);
    paf->drop();    
    
    scene::ILightSceneNode* light1 =
            smgr->addLightSceneNode(0, core::vector3df((pField->getSize().first/2)*CELLSIZE,(pField->getSize().second/2)*CELLSIZE,-100),
            video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 1500);
    scene::IBillboardSceneNode* pSun=smgr->addBillboardSceneNode(light1);
    pSun->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    pSun->setMaterialFlag(video::EMF_LIGHTING, false);
    pSun->setMaterialTexture(0,driver->getTexture(L"res/textures/sun.png"));
    
    return (bool)pField;
}

void LifterScene::updateIndices()
{
   for(int j=0;j<pField->getSize().second;j++)
   {
       for(int i=0;i<pField->getSize().first;i++)
       {
           if(pField->getCellType(Point(i,(pField->getSize().second-1)-j))==EARTH)
                earth_ind[j*pField->getSize().first+i]=1;
           else 
           {
               earth_ind[j*pField->getSize().first+i]=0;
               if(pField->getCellType(Point(i,(pField->getSize().second-1)-j))==WALL)
                   wall_ind[j*pField->getSize().first+i]=1;
               else 
                   wall_ind[j*pField->getSize().first+i]=0;
           }
           
       }
   }
   mbWall.setIndices(wall_ind);
    mbEarth.setIndices(earth_ind);
}
