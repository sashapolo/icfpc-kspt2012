/* 
 * File:   ShaderBumpCallback.h
 * Author: kirill
 *
 * Created on November 6, 2012, 4:40 PM
 */

#pragma once

class ShaderBumpCallback : public video::IShaderConstantSetCallBack
{
public:
    virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
    {
        irr::s32 colorMap=0;
        irr::s32 bumpMap=1;
        irr::s32 specularMap=2;
        irr::s32 glowMap=3;
        
        services->setPixelShaderConstant("colorMap",reinterpret_cast<irr::f32*>(&colorMap), 1);
        services->setPixelShaderConstant("bumpMap",reinterpret_cast<irr::f32*>(&bumpMap), 1);      
        services->setPixelShaderConstant("specularMap",reinterpret_cast<irr::f32*>(&specularMap), 1);  
        services->setPixelShaderConstant("glowMap",reinterpret_cast<irr::f32*>(&glowMap), 1);  
    }

};
