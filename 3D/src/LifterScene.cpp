#include "stdinclude.h"
#include "LifterScene.h"
#define CELLSIZE 10.0f
#define BILLBOARD_DISTANCE 200
#define SCENE_UPDATE_TIME 300

int msecDiff(timespec& t1,timespec& t2)
{
    return abs((t2.tv_sec-t1.tv_sec)*1000+(t2.tv_nsec-t1.tv_nsec)/1000);
};

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

bool LifterScene::loadMap(wchar_t* Path)
{
    clear();
    int len=wcslen(Path)+1;
    char* mbPath=new char[len];
    for(int i=0;i<len-1;i++) mbPath[i]=(char)Path[i];
    mbPath[len-1]='\0';
    pField=new Field( mbPath);
    result=sSimResult();
    
    if(!pField) return false;
    earth_ind=new char[pField->getSize().first*pField->getSize().second];
    wall_ind=new char[pField->getSize().first*pField->getSize().second];
    
    mbWall.create(pField->getSize().first,pField->getSize().second,CELLSIZE,
            float(pField->getSize().first)/18.f*2.f, float(pField->getSize().second)/18.f*2.f,
            0,driver,smgr);
    mbEarth.create(pField->getSize().first,pField->getSize().second,CELLSIZE,
            float(pField->getSize().first)/18.f*2.f, float(pField->getSize().second)/18.f*2.f,
            0,driver,smgr);

    setBaseSceneNodes();
    
    pWallMeshBufferNode=smgr -> addMeshSceneNode(mbWall.mesh);
    setParallaxMaterial(pWallMeshBufferNode,pWallTex,pWallBump,pWallSpecular,pWallGlow);
    pEarthMeshBufferNode=smgr -> addMeshSceneNode(mbEarth.mesh);
    setParallaxMaterial(pEarthMeshBufferNode,pEarthTex,pEarthBump,pEarthSpecular,pEarthGlow);
    
    float sx=pField->getSize().first*CELLSIZE;
    float sy=pField->getSize().second*CELLSIZE;
    float gip=sqrt(sx*sx+sy*sy);
    
    light1->setPosition(core::vector3df((pField->getSize().first/2)*CELLSIZE,(pField->getSize().second/2)*CELLSIZE,-gip));
    pCamera->setTarget(core::vector3df((pField->getSize().first/2)*CELLSIZE,(pField->getSize().second/2)*CELLSIZE,0));
    return (bool)pField;
}

void LifterScene::setBaseSceneNodes()
{
    u32 nStone=0;
    u32 nLambda=0;
    char cell;
    for(int j=0;j<pField->getSize().second;j++)
    {
        for(int i=0;i<pField->getSize().first;i++)
        {
            cell=pField->getXY(Point(i,(pField->getSize().second-1)-j));
            if(cell==EARTH)
                earth_ind[j*pField->getSize().first+i]=1;
            else 
                earth_ind[j*pField->getSize().first+i]=0;

            if(cell==WALL)
                wall_ind[j*pField->getSize().first+i]=1;
            else 
                wall_ind[j*pField->getSize().first+i]=0;
            
            switch(cell)
            {
                case STONE:
                        if(nStone<StoneArr.size())
                            StoneArr[nStone++]->setPosition(vector3df(i*CELLSIZE,j*CELLSIZE,0));
                        else {addActor(Point(i,j),STONE); nStone++;};
                    break;
                case LAMBDA:
                        if(nLambda<LambdaArr.size())
                            LambdaArr[nLambda++]->setPosition(vector3df(i*CELLSIZE,j*CELLSIZE,0));
                        else {addActor(Point(i,j),LAMBDA); nLambda++;};
                    break;
                case OPENED_LIFT:
                case CLOSED_LIFT:
                        if(pLiftNode)
                            pLiftNode->setPosition(vector3df(i*CELLSIZE,j*CELLSIZE,0));
                        else
                            addActor(Point(i,j),CLOSED_LIFT);
                    break;
                case ROBOT:
                        if(pRobotNode)
                            pRobotNode->setPosition(vector3df(i*CELLSIZE,j*CELLSIZE,0));
                        else
                            addActor(Point(i,j),ROBOT);
                    break;
                default:
                    break;
            }
        }
    }
    for(u32 i=StoneArr.size()-1;(i>=nStone)  && (StoneArr.size()!=0);i--) {StoneArr[i]->remove(); StoneArr.pop_back();};
    for(u32 i=LambdaArr.size()-1;(i>=nLambda) && (LambdaArr.size()!=0);i--) {LambdaArr[i]->remove(); LambdaArr.pop_back();};
    mbWall.update(wall_ind);
    mbEarth.update(earth_ind);
}

eEndState LifterScene::step(char chStep)
{
    if(!pField) return ES_NONE;
    if(result.state==ES_FINISHED) return ES_FINISHED;
    result.Changes.clear();
    const Field* pNewField=FieldSim::calcNextStateEx(pField,chStep,&result);
    delete pField;
    pField=pNewField;
    setBaseSceneNodes();
    return result.state;
}

void LifterScene::updateScene()
{
    timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC,&currentTime);
    if(msecDiff(currentTime,prevUpdateTime)<SCENE_UPDATE_TIME) return;
    prevUpdateTime=currentTime;
    
    
    scene::ICameraSceneNode* pCurrCamera=smgr -> getActiveCamera();

    for(u32 i=0;i<StoneArr.size();i++)
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
    
    for(u32 i=0;i<LambdaArr.size();i++)
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

            setBumpMaterial(pNodeLow,pStoneTex,pStoneBump,pStoneSpecular,pBlackTex);
            setBumpMaterial(pNodeHi,pStoneSpriteTex,pStoneSpriteBump,pStoneSpriteSpecular,pBlackTex);

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

            setDefaultMaterial(pNodeLow,video::EMT_SOLID);
            setDefaultMaterial(pNodeHi,video::EMT_TRANSPARENT_ALPHA_CHANNEL,pLambdaSpriteTex,pWhiteTex);

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

                setDefaultMaterial(pNode,video::EMT_NORMAL_MAP_SOLID,pRobotTex,pRobotBump);
                
                anim = smgr->createRotationAnimator(core::vector3df(0,0.5f,0));
                pNode->addAnimator(anim);
                anim->drop();
                
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

                setDefaultMaterial(pNode,video::EMT_SOLID,pLiftTex);
                
                pLiftNode=pNode;
            }
            pLiftNode->setPosition(vector3df(pos.x*CELLSIZE,pos.y*CELLSIZE,0));
            break;
        default:
            break;
    }
}

void LifterScene::setParallaxMaterial(scene::ISceneNode* pNode, ITexture* diffuse, ITexture* normal, ITexture* specular, ITexture* glow)
{
    pNode->setMaterialType((irr::video::E_MATERIAL_TYPE)parallaxMaterial);//video::EMT_NORMAL_MAP_SOLID);
    pNode->setMaterialFlag(video::EMF_LIGHTING, false);
    pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    pNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
    if(diffuse) pNode->setMaterialTexture(0,diffuse);
    if(normal) pNode->setMaterialTexture(1,normal);
    if(specular) pNode->setMaterialTexture(2,specular);
    if(glow) pNode->setMaterialTexture(3,glow);
}

void LifterScene::setBumpMaterial(scene::ISceneNode* pNode, ITexture* diffuse, ITexture* normal, ITexture* specular, ITexture* glow)
{
    pNode->setMaterialType((irr::video::E_MATERIAL_TYPE)bumpMaterial);//video::EMT_NORMAL_MAP_SOLID);
    pNode->setMaterialFlag(video::EMF_LIGHTING, false);
    pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    pNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
    if(diffuse) pNode->setMaterialTexture(0,diffuse);
    if(normal) pNode->setMaterialTexture(1,normal);
    if(specular) pNode->setMaterialTexture(2,specular);
    if(glow) pNode->setMaterialTexture(3,glow);
}

void LifterScene::setDefaultMaterial(scene::ISceneNode* pNode, E_MATERIAL_TYPE Type, ITexture* diffuse, ITexture* normal)
{
    pNode->setMaterialType(Type);
    pNode->setMaterialFlag(video::EMF_LIGHTING, true);
    pNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    pNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    if(diffuse) pNode->setMaterialTexture(0,diffuse);
    if(normal) pNode->setMaterialTexture(1,normal);
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
