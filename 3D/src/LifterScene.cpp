#include "stdinclude.h"
#include "LifterScene.h"
#define CELLSIZE 10.0f
#define BILLBOARD_DISTANCE 200
#define SCENE_UPDATE_TIME 300


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
    NodeArr=0;
}


LifterScene::~LifterScene() {
}

void LifterScene::init(IrrlichtDevice *device_, IVideoDriver* driver_, ISceneManager* smgr_)
{
    device=device_;
    driver=driver_;
    smgr=smgr_;
    
    pExplosionTexture=driver->getTexture(L"3D/res/textures/exp.tga");
    
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
    NodeArr=new scene::ISceneNode*[pField->getSize().first*pField->getSize().second];
    memset(NodeArr,0,pField->getSize().first*pField->getSize().second*sizeof(scene::ISceneNode*));
    memset(earth_ind,0,pField->getSize().first*pField->getSize().second*sizeof(char));
    memset(wall_ind,0,pField->getSize().first*pField->getSize().second*sizeof(char));
    
    mbWall.create(pField->getSize().first,pField->getSize().second,CELLSIZE,
            float(pField->getSize().first)/18.f*2.f, float(pField->getSize().second)/18.f*2.f,
            0,driver,smgr);
    mbEarth.create(pField->getSize().first,pField->getSize().second,CELLSIZE,
            float(pField->getSize().first)/18.f*2.f, float(pField->getSize().second)/18.f*2.f,
            0,driver,smgr);

    char cell;
    for(int j=0;j<pField->getSize().second;j++)
    {
        for(int i=0;i<pField->getSize().first;i++)
        {
            cell=pField->getXY(Point(i,(pField->getSize().second-1)-j));
            addActor(Point(i,j),cell);
        }
    }
    mbWall.update(wall_ind);
    mbEarth.update(earth_ind);

    
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

scene::ISceneNode* LifterScene::getNode(Point p)
{
    return NodeArr[p.y*pField->getSize().first+p.x];
}

void LifterScene::setNode(Point p,scene::ISceneNode* node)
{
    NodeArr[p.y*pField->getSize().first+p.x]=node;
}


eEndState LifterScene::step(char chStep)
{
    if(!pField) return ES_NONE;
    if(result.state==ES_FINISHED) return ES_FINISHED;
    result.Changes.clear();
    const Field* pNextField=FieldSim::calcNextStateEx(pField,chStep,&result);
    applyChanges(result);
    
    delete pField;
    pField=pNextField;
    //setBaseSceneNodes();
    return result.state;
}

void LifterScene::updateScene()
{
    if(!pField) return;
    timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC,&currentTime);
    if(msecDiff(currentTime,prevUpdateTime)<SCENE_UPDATE_TIME) return;
    prevUpdateTime=currentTime;
    
    
    core::list<ISceneNode*>::ConstIterator nodeIt;
    scene::ICameraSceneNode* pCurrCamera=smgr -> getActiveCamera();

    FieldCache::const_iterator it=pField->getStoneCacheIt();
    while(it!=pField->getStoneCacheEnd())
    {
        Point pt=*(*it);
        pt.y=(pField->getSize().second-1)-pt.y;
        scene::ISceneNode* pNode=getNode(pt);
        if(!pNode) 
        {
            LOGERROR("Wrong node");
            it++;
            continue;
        }
        
        bool bVisible=false;
        if(pCurrCamera->getPosition().getDistanceFrom(pNode->getPosition())<BILLBOARD_DISTANCE) bVisible=true;
        
        nodeIt=pNode->getChildren().begin();
        if(pNode->getChildren().size()==2)
        {
            (*nodeIt)->setVisible(bVisible);
            nodeIt++;
            (*nodeIt)->setVisible(!bVisible);
        }
        
        it++;
    }
    
    it=pField->getLambdaCacheIt();
    while(it!=pField->getLambdaCacheEnd())
    {
        Point pt=*(*it);
        pt.y=(pField->getSize().second-1)-pt.y;
        scene::ISceneNode* pNode=getNode(pt);
        if(!pNode) 
        {
            LOGERROR("Wrong node");
            it++;
            continue;
        }
        
        bool bVisible=false;
        if(pCurrCamera->getPosition().getDistanceFrom(pNode->getPosition())<BILLBOARD_DISTANCE) bVisible=true;
        
        nodeIt=pNode->getChildren().begin();
        if(pNode->getChildren().size()==2)
        {
            (*nodeIt)->setVisible(bVisible);
            nodeIt++;
            (*nodeIt)->setVisible(!bVisible);
        }
        
        it++;
    }
}

void LifterScene::applyChanges(sSimResult& res)
{
    for(u32 i=0;i<res.Changes.size();i++)
    {
        res.Changes[i].pos1=Point(res.Changes[i].pos1.x,(pField->getSize().second-1)-res.Changes[i].pos1.y);
        res.Changes[i].pos2=Point(res.Changes[i].pos2.x,(pField->getSize().second-1)-res.Changes[i].pos2.y);
        switch(res.Changes[i].changeType)
        {
            case CH_MOVE:
                moveActor(res.Changes[i].pos1,res.Changes[i].pos2);
                LOGINFO("Change: move (%d,%d)->(%d,%d)",
                        res.Changes[i].pos1.x,res.Changes[i].pos1.y,
                        res.Changes[i].pos2.x,res.Changes[i].pos2.y)
                break;
            case CH_DESTROY:
                removeActor(res.Changes[i].pos1);
                LOGINFO("Change: destroy (%d,%d)",
                        res.Changes[i].pos1.x,res.Changes[i].pos1.y)
                break;
            default:
                LOGINFO("Change: none");
                break;
        }
    }
    mbWall.update(wall_ind);
    mbEarth.update(earth_ind);
}

void LifterScene::moveActor(Point pos0,Point pos1)
{
    scene::ISceneNode* pNode=getNode(pos0);
    if(pNode) 
    {
        ISceneNodeAnimator * anim=smgr->createFlyStraightAnimator(
                core::vector3df(pos0.x*CELLSIZE,pos0.y*CELLSIZE,0),
                core::vector3df(pos1.x*CELLSIZE,pos1.y*CELLSIZE,0),1000);
        pNode->addAnimator(anim);
        //pNode->setPosition(vector3df(pos1.x*CELLSIZE,pos1.y*CELLSIZE,0));
        setNode(pos0,0);
        setNode(pos1,pNode);
    }
}

void LifterScene::removeActor(Point pos)
{
    char cell=pField->getXY(Point(pos.x,(pField->getSize().second-1)-pos.y));
    
    if(cell==WALL) wall_ind[pos.y*pField->getSize().first+pos.x]=0;
    else if(cell==EARTH) earth_ind[pos.y*pField->getSize().first+pos.x]=0;
    else
    {
        scene::ISceneNode* pNode=getNode(pos);
        if(pNode) 
        {
            pNode->remove();
            setNode(pos,0);
        }
    }
}

void LifterScene::addActor(Point pos, char type)
{
    scene::ISceneNode* pNode;
    scene::ISceneNodeAnimator* anim;
    scene::IParticleSystemSceneNode* ps;
    scene::IParticleEmitter* em;
    scene::IParticleAffector* paf;
    
    scene::ISceneNode* pNodeLow;
    scene::ISceneNode* pNodeHi;
    
    
    removeActor(pos);
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
            setNode(pos,pNode);
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
            setNode(pos,pNode);
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
            setNode(pos,pRobotNode);
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
            setNode(pos,pLiftNode);
            break;
            
        case OPENED_LIFT:
            if(!pLiftNode)
            {
                pNode=smgr -> addCubeSceneNode(CELLSIZE);

                setDefaultMaterial(pNode,video::EMT_SOLID,pLiftTex);
                
                pLiftNode=pNode;
            }
            pLiftNode->setPosition(vector3df(pos.x*CELLSIZE,pos.y*CELLSIZE,0));
            setNode(pos,pLiftNode);
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
    
    if(NodeArr)
    {
        for(int i=0;i<pField->getSize().first*pField->getSize().second;i++)
        {
            if(NodeArr[i]!=0)
            {
                NodeArr[i]->remove();
                NodeArr[i]=0;
            }
        }
        
        pRobotNode=0;
        pLiftNode=0;
        delete [] NodeArr;
        NodeArr=0;
    }
    
//    if(pRobotNode)
//    {
//        pRobotNode->remove();
//        pRobotNode=0;
//    }
//    
//    if(pLiftNode)
//    {
//        pLiftNode->remove();
//        pLiftNode=0;
//    }
    
//    for(unsigned int i=0;i<StoneArr.size();i++) StoneArr[i]->remove();
//    StoneArr.clear();
//    for(unsigned int i=0;i<LambdaArr.size();i++) LambdaArr[i]->remove();
//    LambdaArr.clear();
}
