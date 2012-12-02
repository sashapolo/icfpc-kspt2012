/* 
 * File:   LifterEffects.cpp
 * Author: kirill
 * 
 * Created on December 1, 2012, 11:14 PM
 */
#include "stdinclude.h"
#include "LifterEffects.h"
#include "AnimSprite.h"

LifterEffects::LifterEffects() {
}

LifterEffects::~LifterEffects() {
}

void LifterEffects::init(IrrlichtDevice *device_, IVideoDriver* driver_, ISceneManager* smgr_)
{
    device=device_;
    driver=driver_;
    smgr=smgr_;
}

scene::ISceneNode* LifterEffects::createAnimation(ITexture* diffuse, ITexture* normal, u32 wFrames, u32 hFrames,float Size, Point pos,u32 time)
{
    AnimSprite* myNode = new AnimSprite(smgr->getRootSceneNode(), smgr);
    myNode->Init(wFrames,hFrames,time,Size,false);
    myNode->setPosition(vector3df(pos.x*10,pos.y*10,0));
    
    
    myNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    myNode->setMaterialFlag(video::EMF_LIGHTING, false);
    myNode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    myNode->setMaterialTexture(0,diffuse);
    myNode->setMaterialTexture(1,normal);
    
    
    
   
   
    animNodes.push_back(myNode);
    return myNode;
}

void LifterEffects::update()
{
    std::list<AnimSprite*>::iterator it=animNodes.begin();
    timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC,&currentTime);
    
    while(it!=animNodes.end())
    {
        AnimSprite* pNode=*it;
        
        if(pNode->isDead())
        {
            pNode->remove();
            delete pNode;
            it=animNodes.erase(it);
        }
        else it++;
    }
}