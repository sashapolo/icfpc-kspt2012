/* 
 * File:   LifterEffects.h
 * Author: kirill
 *
 * Created on December 1, 2012, 11:14 PM
 */

#ifndef LIFTEREFFECTS_H
#define	LIFTEREFFECTS_H

#include "ShaderParallaxCallback.h"


class LifterEffects {
public:
    LifterEffects();
    virtual ~LifterEffects();
    void init(IrrlichtDevice *device_, IVideoDriver* driver_, ISceneManager* smgr_);
    
    scene::ISceneNode* createAnimation(ITexture* diffuse, ITexture* normal, u32 wFrames, u32 hFrames,float Size, Point pos,u32 time);
    void update();
private:
    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    std::list<AnimSprite*> animNodes;
};

#endif	/* LIFTEREFFECTS_H */

