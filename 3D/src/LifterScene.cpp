#include "stdinclude.h"
#include "LifterScene.h"
#include "AnimSprite.h"
#include "LifterEffects.h"
//#include "LifterEffects.h"
#define CELLSIZE 10.0f
#define BILLBOARD_DISTANCE 200
#define SCENE_UPDATE_TIME 300


LifterScene::LifterScene() {
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

    animationSpeed=500;
    clock_gettime(CLOCK_MONOTONIC,&prevAnimTime);
}


LifterScene::~LifterScene() {
}

void LifterScene::init(IrrlichtDevice *device_, IVideoDriver* driver_, ISceneManager* smgr_)
{
    device=device_;
    driver=driver_;
    smgr=smgr_;

    effects.init(device,driver,smgr);

    pExplosionTexture=driver->getTexture(L"3D/res/textures/exp.png");
    pFogTex=driver->getTexture(L"3D/res/textures/fog.png");
    pTeleTex=driver->getTexture(L"3D/res/textures/tele.tga");

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
    pSunTex=driver->getTexture(L"3D/res/textures/sun.tga");

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
    try {
        pField=new Field( mbPath);
    } catch (const Field::FieldParseException& e) {
        LOGERROR("Map load error!");
        return false;
    }

    result=sSimResult();

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

void LifterScene::onFrame()
{
    updateScene();
    effects.update();
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

    Field::FieldCache::const_iterator it=pField->getStoneCacheIt();
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
    scene::ISceneNode* pNode=0;
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

                if(res.Changes[i].cellType==LAMBDA)
                {
                    pNode=effects.createAnimation(pTeleTex, pBlackTex,5,6,CELLSIZE*1.1, res.Changes[i].pos1,animationSpeed*2);
                    //addLambdaExplosionParticles(pNode,pFogTex);
                }
                else
                {
                    pNode=effects.createAnimation(pExplosionTexture, pBlackTex,8,8,CELLSIZE*2.2, res.Changes[i].pos1,animationSpeed*3);
                    addExplosionParticles(pNode,pFogTex,pSunTex);
                }

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
        clock_gettime(CLOCK_MONOTONIC,&prevAnimTime);
        ISceneNodeAnimator * anim=smgr->createFlyStraightAnimator(
                core::vector3df(pos0.x*CELLSIZE,pos0.y*CELLSIZE,0),
                core::vector3df(pos1.x*CELLSIZE,pos1.y*CELLSIZE,0),animationSpeed);
        pNode->addAnimator(anim);
        //pNode->setPosition(vector3df(pos1.x*CELLSIZE,pos1.y*CELLSIZE,0));
        setNode(pos0,0);
        setNode(pos1,pNode);

    }
}

bool LifterScene::isAnimation()
{
    timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC,&currentTime);
    if(msecDiff(currentTime,prevAnimTime)<animationSpeed) return true;
    return false;
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
                pNode->setMaterialFlag(video::EMF_LIGHTING, false);

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

void LifterScene::addExplosionParticles(scene::ISceneNode* pNode,ITexture* pTexture1,ITexture* pTexture2)
{
    scene::IParticleSystemSceneNode* ps =
        smgr->addParticleSystemSceneNode(false,pNode);

    scene::IParticleEmitter* em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-7,0,-7,7,1,7), // emitter size
        core::vector3df(0.0f,0.006f,0.0f),   // initial direction
        20,50,                             // emit rate
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        200,1000,10,                         // min and max age, angle
        core::dimension2df(4.f,4.f),         // min size
        core::dimension2df(8.f,8.f));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setScale(core::vector3df(1,1,1));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, pTexture1);
    ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    /////////////

    ps =
        smgr->addParticleSystemSceneNode(false,pNode);

    em = ps->createSphereEmitter(
        core::vector3df(0.0f,0.0f,0.0f), // emitter size
        4,
        core::vector3df(0.0f,-0.003f,0.0f),   // initial direction
        80,100,                             // emit rate
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        800,2000,10,                         // min and max age, angle
        core::dimension2df(1.f,1.f),         // min size
        core::dimension2df(2.f,2.f));        // max size

    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    paf = ps->createFadeOutParticleAffector();

    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setScale(core::vector3df(1,1,1));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, pTexture2);
    ps->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
}

void LifterScene::addLambdaExplosionParticles(scene::ISceneNode* pNode,ITexture* pTexture1)
{
    scene::IParticleSystemSceneNode* ps =
        smgr->addParticleSystemSceneNode(false,pNode);

    scene::IParticleEmitter* em = ps->createSphereEmitter(
        core::vector3df(0.0f,0.0f,0.0f), // emitter size
        4,
        core::vector3df(0.0f,0.01f,0.0f),   // initial direction
        80,100,                             // emit rate
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        800,2000,10,                         // min and max age, angle
        core::dimension2df(3.f,3.f),         // min size
        core::dimension2df(6.f,6.f));        // max size


    ps->setEmitter(em); // this grabs the emitter
    em->drop(); // so we can drop it here without deleting it

    scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setScale(core::vector3df(1,1,1));
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, pTexture1);
    ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
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
            if((NodeArr[i]!=0) && (NodeArr[i]!=pLiftNode))
            {
                NodeArr[i]->remove();
                NodeArr[i]=0;
            }
        }

        pRobotNode=0;

        if(pLiftNode)
        {
            pLiftNode->remove();
            pLiftNode=0;
        }
        delete [] NodeArr;
        NodeArr=0;
    }

    if(pField)
    {
        delete pField;
        pField=0;
    }
}
