/* 
 * File:   ShaderParallaxCallback.h
 * Author: kirill
 *
 * Created on November 6, 2012, 5:34 PM
 */
#pragma once

class ShaderParallaxCallback : public video::IShaderConstantSetCallBack
{
public:
    ShaderParallaxCallback() : video::IShaderConstantSetCallBack(), texOffset(0,0),texScale(1,1)
    {
        
    }
    
    ISceneManager         *SceneManager;
    vector2df texOffset;
    vector2df texScale;
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        irr::s32 colorMap=0;
        irr::s32 bumpHeightMap=1;
        irr::s32 specularMap=2;
        irr::s32 glowMap=3;
        
        services->setPixelShaderConstant("colorMap",reinterpret_cast<irr::f32*>(&colorMap), 1);
        services->setPixelShaderConstant("bumpHeightMap",reinterpret_cast<irr::f32*>(&bumpHeightMap), 1);      
        services->setPixelShaderConstant("specularMap",reinterpret_cast<irr::f32*>(&specularMap), 1);  
        services->setPixelShaderConstant("glowMap",reinterpret_cast<irr::f32*>(&glowMap), 1);  
        
        core::matrix4 mWorld = services->getVideoDriver()->getTransform ( video::ETS_WORLD );
        services->setVertexShaderConstant ( "mWorld", mWorld.pointer(), 16 );
        
        services->setPixelShaderConstant ( "TexOffset", reinterpret_cast< f32 * > ( &texOffset ), 2 );
        services->setPixelShaderConstant ( "TexScale", reinterpret_cast< f32 * > ( &texScale ), 2 );
        
        core::vector3df pos = SceneManager->getActiveCamera()->getAbsolutePosition();
        //float t=pos.z;
        //pos.z=pos.y;
        //pos.y=t;
        services->setVertexShaderConstant ( "cameraPos", reinterpret_cast< f32 * > ( &pos ), 3 );
    }

};