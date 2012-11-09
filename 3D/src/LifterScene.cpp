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
    
    pRobotNode=0;
    pLiftNode=0;
    
    pWallMeshBufferNode=0;
    pEarthMeshBufferNode=0;
    
    pCamera=0;
}


LifterScene::~LifterScene() {
}

void LifterScene::init(IVideoDriver* driver_, ISceneManager* smgr_)
{
    driver=driver_;
    smgr=smgr_;
    
    pWallTex=driver->getTexture(L"3D/res/textures/wall.png");
    pWallBump=driver->getTexture(L"3D/res/textures/wall_n.tga");
    pWallGlow=driver->getTexture(L"3D/res/textures/wall_g.png");
    pWallSpecular=driver->getTexture(L"3D/res/textures/wall_s.png");
    
    pEarthTex=driver->getTexture(L"3D/res/textures/earth.png");
    pEarthBump=driver->getTexture(L"3D/res/textures/earth_n.tga");
    pEarthGlow=driver->getTexture(L"3D/res/textures/earth_g.png");
    pEarthSpecular=driver->getTexture(L"3D/res/textures/earth_s.png");
    
    pStoneTex=driver->getTexture(L"3D/res/textures/stone.png");
    pStoneBump=driver->getTexture(L"3D/res/textures/stone_n.png");
    pStoneSpecular=driver->getTexture(L"3D/res/textures/stone_s.png");
    pRobotTex=driver->getTexture(L"3D/res/textures/ufo.png");
    pRobotBump=driver->getTexture(L"3D/res/textures/ufo_n.png");
    pFireTex=driver->getTexture(L"3D/res/textures/fireball.png");
    pLiftTex=driver->getTexture(L"3D/res/textures/lift_closed.png");
    pSunTex=driver->getTexture(L"3D/res/textures/sun.png");
    
    pStoneSpriteTex=driver->getTexture(L"3D/res/textures/stone_sprite.png");
    pStoneSpriteBump=driver->getTexture(L"3D/res/textures/stone_sprite_n.png");
    pStoneSpriteSpecular=driver->getTexture(L"3D/res/textures/stone_sprite_s.png");
    pLambdaSpriteTex=driver->getTexture(L"3D/res/textures/lambda_sprite.png");
    
    pBlackTex=driver->getTexture(L"3D/res/textures/black.png");
    pWhiteTex=driver->getTexture(L"3D/res/textures/white.png");
    pGreyTex=driver->getTexture(L"3D/res/textures/grey.png");
    
    pStoneMesh=smgr->getMesh(L"3D/res/models/stone.3DS");
    pLambdaMesh=smgr->getMesh(L"3D/res/models/lambda.3DS");
    pRobotMesh=smgr->getMesh(L"3D/res/models/ufo.3DS");
    

    light1 = smgr->addLightSceneNode(0, core::vector3df(0,0,-50),
            video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 2500);
    
    pCamera=smgr->addCameraSceneNodeMaya(0,-150,150,150);
    pCamera->setFarValue(2000.f);
    pCamera->setTarget(core::vector3df(0,0,0));
    smgr->setActiveCamera(pCamera);
    
    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
    ShaderBumpCallback* bumpCallback = new ShaderBumpCallback();
    bumpMaterial = gpu-> addHighLevelShaderMaterialFromFiles(
            "3D/res/shaders/bump.vert","main",irr::video::EVST_VS_2_0,
            "3D/res/shaders/bump.frag","main",irr::video::EPST_PS_2_0,
            bumpCallback,irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL,1);
    
    ShaderParallaxCallback* parallaxCallback = new ShaderParallaxCallback();
    parallaxCallback->SceneManager=smgr;
    parallaxMaterial = gpu-> addHighLevelShaderMaterialFromFiles(
            "3D/res/shaders/parallax.vert","main",irr::video::EVST_VS_2_0,
            "3D/res/shaders/parallax.frag","main",irr::video::EPST_PS_2_0,
            parallaxCallback,irr::video::EMT_SOLID,1);

}

void LifterScene::release()
{
    mbWall.release();
    mbEarth.release();
    clear();
}

void LifterScene::clear()
{
    if(earth_ind) 
    {
        delete [] earth_ind;
        earth_ind=0;
    }
    if(wall_ind) 
    {
        delete [] wall_ind;
        wall_ind=0;
    }
    
    if(pWallMeshBufferNode) 
    {
        pWallMeshBufferNode->remove();
        pWallMeshBufferNode=0;
    };
    
    if(pEarthMeshBufferNode) 
    {
        pEarthMeshBufferNode->remove();
        pEarthMeshBufferNode=0;
    }
    
    if(pRobotNode)
    {
        pRobotNode->remove();
        pRobotNode=0;
    }
    
    if(pLiftNode)
    {
        pLiftNode->remove();
        pLiftNode=0;
    }
    
    for(unsigned int i=0;i<StoneArr.size();i++) StoneArr[i]->remove();
    StoneArr.clear();
    for(unsigned int i=0;i<LambdaArr.size();i++) LambdaArr[i]->remove();
    LambdaArr.clear();
}

bool LifterScene::loadMap(wchar_t* Path)
{
    clear();
    int len=wcslen(Path)+1;
    char* mbPath=new char[len];
    for(int i=0;i<len-1;i++) mbPath[i]=(char)Path[i];
    mbPath[len-1]='\0';
    pField=createField( mbPath);
    
    if(!pField) return false;
    earth_ind=new char[pField->getSize().first*pField->getSize().second];
    wall_ind=new char[pField->getSize().first*pField->getSize().second];
    updateIndices();
    
    mbWall.create(pField->getSize().first,pField->getSize().second,CELLSIZE,
            float(pField->getSize().first)/18.f*2.f, float(pField->getSize().second)/18.f*2.f,
            wall_ind,driver,smgr);
    mbEarth.create(pField->getSize().first,pField->getSize().second,CELLSIZE,
            float(pField->getSize().first)/18.f*2.f, float(pField->getSize().second)/18.f*2.f,
            earth_ind,driver,smgr);

    
    
    pWallMeshBufferNode=smgr -> addMeshSceneNode(mbWall.mesh);
    pWallMeshBufferNode->setMaterialType((irr::video::E_MATERIAL_TYPE)parallaxMaterial);//video::EMT_NORMAL_MAP_SOLID);
    pWallMeshBufferNode->setMaterialFlag(video::EMF_LIGHTING, false);
    pWallMeshBufferNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    pWallMeshBufferNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    pWallMeshBufferNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
    pWallMeshBufferNode->setMaterialTexture(0,pWallTex);
    pWallMeshBufferNode->setMaterialTexture(1,pWallBump);
    pWallMeshBufferNode->setMaterialTexture(2,pWallSpecular);
    pWallMeshBufferNode->setMaterialTexture(3,pWallGlow);

    pEarthMeshBufferNode=smgr -> addMeshSceneNode(mbEarth.mesh);
    pEarthMeshBufferNode->setMaterialType((irr::video::E_MATERIAL_TYPE)parallaxMaterial);//video::EMT_NORMAL_MAP_SOLID);
    pEarthMeshBufferNode->setMaterialFlag(video::EMF_LIGHTING, false);
    pEarthMeshBufferNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    pEarthMeshBufferNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    pEarthMeshBufferNode->setMaterialTexture(0,pEarthTex);
    pEarthMeshBufferNode->setMaterialTexture(1,pEarthBump);
    pEarthMeshBufferNode->setMaterialTexture(2,pEarthSpecular);
    pEarthMeshBufferNode->setMaterialTexture(3,pEarthGlow);

    std::list<FieldMember*>::iterator it=pField->getStoneCacheIt();
    int x,y;
    while(it!=pField->getStoneCacheEnd())
    {
        x=(*it)->getCoordinate().x;
        y=(*it)->getCoordinate().y;
        
        addActor(Point(x,(pField->getSize().second-1)-y),STONE);
        it++;
    }
    
    it=pField->getLambdaCacheIt();
    while(it!=pField->getLambdaCacheEnd())
    {
        x=(*it)->getCoordinate().x;
        y=(*it)->getCoordinate().y;
        
        addActor(Point(x,(pField->getSize().second-1)-y),LAMBDA);
        it++;
    }
    
    if(pField->isRobotAlive())
    {
        x=pField->getRobot()->getCoordinate().x;
        y=pField->getRobot()->getCoordinate().y;
        addActor(Point(x,(pField->getSize().second-1)-y),ROBOT);
    }
    
    if(pField->getLift())
    {
        x=pField->getLift()->getCoordinate().x;
        y=pField->getLift()->getCoordinate().y;
        addActor(Point(x,(pField->getSize().second-1)-y),CLOSED_LIFT);
    }
    
    float sx=pField->getSize().first*CELLSIZE;
    float sy=pField->getSize().second*CELLSIZE;
    float gip=sqrt(sx*sx+sy*sy);
    
    light1->setPosition(core::vector3df((pField->getSize().first/2)*CELLSIZE,(pField->getSize().second/2)*CELLSIZE,-gip));
    pCamera->setTarget(core::vector3df((pField->getSize().first/2)*CELLSIZE,(pField->getSize().second/2)*CELLSIZE,0));
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
                earth_ind[j*pField->getSize().first+i]=0;
           
           if(pField->getCellType(Point(i,(pField->getSize().second-1)-j))==WALL)
               wall_ind[j*pField->getSize().first+i]=1;
           else 
               wall_ind[j*pField->getSize().first+i]=0;
           
       }
   }
   //mbWall.setIndices(wall_ind);
   //mbEarth.setIndices(earth_ind);
}

#define BILLBOARD_DISTANCE 200
void LifterScene::updateScene()
{
    scene::ICameraSceneNode* pCurrCamera=smgr -> getActiveCamera();

    for(int i=0;i<StoneArr.size();i++)
    {
        if(pCurrCamera->getPosition().getDistanceFrom(StoneArr[i]->getPosition())<BILLBOARD_DISTANCE)
        {
            core::list<ISceneNode*>::ConstIterator it=StoneArr[i]->getChildren().begin();
            (*it)->setVisible(true);
            it++;
            (*it)->setVisible(false);
        }
        else
        {
            core::list<ISceneNode*>::ConstIterator it=StoneArr[i]->getChildren().begin();
            (*it)->setVisible(false);
            it++;
            (*it)->setVisible(true);
        }
    }
    
    for(int i=0;i<LambdaArr.size();i++)
    {
        if(pCurrCamera->getPosition().getDistanceFrom(LambdaArr[i]->getPosition())<BILLBOARD_DISTANCE)
        {
            core::list<ISceneNode*>::ConstIterator it=LambdaArr[i]->getChildren().begin();
            (*it)->setVisible(true);
            it++;
            (*it)->setVisible(false);
        }
        else
        {
            core::list<ISceneNode*>::ConstIterator it=LambdaArr[i]->getChildren().begin();
            (*it)->setVisible(false);
            it++;
            (*it)->setVisible(true);
        }
    }
}

void LifterScene::addActor(Point pos, CellType type)
{
    scene::ISceneNode* pNode;
    scene::ISceneNodeAnimator* anim;
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    scene::IParticleAffector* paf;
    
    scene::ISceneNode* pNodeLow;
    scene::ISceneNode* pNodeHi;
    
    switch(type)
    {
        case WALL:
           wall_ind[pos.y*pField->getSize().first+pos.x]=1; 
           break;
        case EARTH:
           earth_ind[pos.y*pField->getSize().first+pos.x]=1; 
           break;
        case STONE:
            //pNode=smgr -> addMeshSceneNode(pStoneMesh);//addOctreeSceneNode
            pNode=smgr -> addEmptySceneNode();
            pNodeLow=smgr -> addMeshSceneNode(pStoneMesh);
            pNodeHi=smgr -> addBillboardSceneNode();
            pNode->addChild(pNodeLow);
            pNode->addChild(pNodeHi);//addOctreeSceneNode
            
            pNode->setPosition(vector3df(pos.x*CELLSIZE,pos.y*CELLSIZE,0));
            pNode->setRotation(vector3df(pos.x*49,pos.y*49,pos.x*49));
            //pNode->setScale(vector3df(0.5,0.5,0.5));

            pNodeLow->setMaterialType((irr::video::E_MATERIAL_TYPE)bumpMaterial);//video::EMT_NORMAL_MAP_SOLID);
            pNodeLow->setMaterialFlag(video::EMF_LIGHTING, false);
            pNodeLow->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
            pNodeLow->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
            pNodeLow->setMaterialTexture(0,pStoneTex);
            pNodeLow->setMaterialTexture(1,pStoneBump);
            pNodeLow->setMaterialTexture(2,pStoneSpecular);
            pNodeLow->setMaterialTexture(3,pBlackTex);
    
            pNodeHi->setMaterialType((irr::video::E_MATERIAL_TYPE)bumpMaterial);//video::EMT_NORMAL_MAP_SOLID);
            pNodeHi->setMaterialFlag(video::EMF_LIGHTING, false);
            pNodeHi->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
            pNodeHi->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
            pNodeHi->setMaterialTexture(0,pStoneSpriteTex);
            pNodeHi->setMaterialTexture(1,pStoneSpriteBump);
            pNodeHi->setMaterialTexture(2,pStoneSpriteSpecular);
            pNodeHi->setMaterialTexture(3,pBlackTex);

            StoneArr.push_back(pNode);
            break;
        case LAMBDA:
            pNode=smgr -> addEmptySceneNode();
            pNodeLow=smgr -> addMeshSceneNode(pLambdaMesh);
            pNodeHi=smgr -> addBillboardSceneNode();
            pNode->addChild(pNodeLow);
            pNode->addChild(pNodeHi);
            
            pNode->setPosition(vector3df(pos.x*CELLSIZE,pos.y*CELLSIZE,0));
            pNode->setScale(vector3df(CELLSIZE,CELLSIZE,CELLSIZE));

            pNodeLow->setMaterialType(video::EMT_SOLID);
            pNodeLow->setMaterialFlag(video::EMF_LIGHTING, true);
            pNodeLow->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
            pNodeLow->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
            
            pNodeHi->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
            pNodeHi->setMaterialFlag(video::EMF_LIGHTING, true);
            pNodeHi->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
            pNodeHi->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
            pNodeHi->setMaterialTexture(0,pLambdaSpriteTex);
            pNodeHi->setMaterialTexture(1,pWhiteTex);

            anim = smgr->createRotationAnimator(core::vector3df(0,-0.4f,0));
                    pNode->addAnimator(anim);
            anim->drop();

            LambdaArr.push_back(pNode);
            break;
        case ROBOT:
            if(!pRobotNode)
            {
                pNode=smgr -> addMeshSceneNode(pRobotMesh);
                pNode->setScale(vector3df(0.05,0.05,0.05));

                pNode->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
                pNode->setMaterialFlag(video::EMF_LIGHTING, true);
                pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
                pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);    
                pNode->setMaterialTexture(0,pRobotTex);
                pNode->setMaterialTexture(1,pRobotBump);
                
                anim = smgr->createRotationAnimator(core::vector3df(0,0.5f,0));
                pNode->addAnimator(anim);
                anim->drop();
                
                //anim = smgr->createFlyStraightAnimator(core::vector3df(0,2,0),core::vector3df(0,-2,0),1000,true,true);
                //pNode->addAnimator(anim);
                //anim->drop();

                ps = smgr->addParticleSystemSceneNode(false, pNode);

                // create and set emitter
                em = ps->createBoxEmitter(
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
                paf = ps->createFadeOutParticleAffector();
                ps->addAffector(paf);
                paf->drop();    
                pRobotNode=pNode;
            }
            pRobotNode->setPosition(vector3df(pos.x*CELLSIZE,pos.y*CELLSIZE,0));
            break;
        case CLOSED_LIFT:
            if(!pLiftNode)
            {
                pNode=smgr -> addCubeSceneNode(CELLSIZE);

                pNode->setMaterialType(video::EMT_SOLID);
                pNode->setMaterialFlag(video::EMF_LIGHTING, true);
                pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
                pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);    
                pNode->setMaterialTexture(0,pLiftTex);
                
                pLiftNode=pNode;
            }
            pLiftNode->setPosition(vector3df(pos.x*CELLSIZE,pos.y*CELLSIZE,0));
            break;
            
        case OPENED_LIFT:
            if(!pLiftNode)
            {
                pNode=smgr -> addCubeSceneNode(CELLSIZE);

                pNode->setMaterialType(video::EMT_SOLID);
                pNode->setMaterialFlag(video::EMF_LIGHTING, true);
                pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
                pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);    
                pNode->setMaterialTexture(0,pLiftTex);
                
                pLiftNode=pNode;
            }
            pLiftNode->setPosition(vector3df(pos.x*CELLSIZE,pos.y*CELLSIZE,0));
            break;
        default:
            break;
    }
}
