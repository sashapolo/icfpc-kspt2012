/* 
 * File:   PostBloomCallback.h
 * Author: kirill
 *
 * Created on November 7, 2012, 1:13 AM
 */
#pragma once
 
struct sPostBloomSetup 
{ 
    float sampleDist; 
    float strength; 
    float multiplier; 
}; 
 
 
class PostBloomCallback: public video::IShaderConstantSetCallBack 
{ 
public: 
    float sampleDist; 
    float strength; 
    float multiplier; 


    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) 
    { 
        irr::s32 colorMap=0;
        irr::s32 origMap =1;
        services->setPixelShaderConstant("colorMap",reinterpret_cast<irr::f32*>(&colorMap), 1);
        services->setPixelShaderConstant("origMap",reinterpret_cast<irr::f32*>(&origMap), 1);
        services->setVertexShaderConstant("sampleDist", reinterpret_cast<f32*>(&sampleDist),1); 
        services->setVertexShaderConstant("strength", reinterpret_cast<f32*>(&strength),1); 
        services->setVertexShaderConstant("multiplier", reinterpret_cast<f32*>(&multiplier),1); 
    } 
}; 
 
 
 
 
class IPostProcessBloom : public scene::ISceneNode 
{ 

public: 
    core::aabbox3d<f32> Box; 
    video::S3DVertex Vertices[6];//the vertices for the onscreenquad 
    video::SMaterial Material; 
    video::ITexture* rt0; //the rendertarget 
    video::ITexture* orig0;
    int mat; 
    PostBloomCallback* callback; 

    IPostProcessBloom(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id): scene::ISceneNode(parent, mgr, id) 
    { 
        Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f); 
        Vertices[1] = video::S3DVertex(-1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f); 
        Vertices[2] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f); 
        Vertices[3] = video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f); 
        Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f); 
        Vertices[5] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f); 
        callback=0;
        rt0=0;
    } 


    void initiate(unsigned int sizeW,unsigned int sizeH,sPostBloomSetup data,scene::ISceneManager* smgr) 
    { 
        video::IVideoDriver* driver = smgr->getVideoDriver(); 
        video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices(); 

        callback= new PostBloomCallback(); 
        callback->sampleDist=data.sampleDist; 
        callback->strength=data.strength; 
        callback->multiplier=data.multiplier; 

        Material.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterialFromFiles 
        ( 
            "3D/res/shaders/bloom.vert", "main", video::EVST_VS_1_1, 
            "3D/res/shaders/bloom.frag", "main", video::EPST_PS_1_1, 
            callback, (video::EMT_SOLID) 
        ); 
        
        Material.Wireframe = false; 
        Material.Lighting = false; 
        resize(driver,sizeW,sizeH);
    } 
    
    void resize(video::IVideoDriver* driver,unsigned int sizeW,unsigned int sizeH)
    {
        if(rt0)
        {
            if((rt0->getSize().Width==sizeW) && (rt0->getSize().Height==sizeH)) return;
            driver->removeTexture(rt0);
        }
        rt0 = driver->addRenderTargetTexture(core::dimension2d<u32>(sizeW,sizeH));
        Material.setTexture(0,rt0);
    }

    virtual void OnPreRender(){} 

    virtual void render() 
    { 
        u16 indices[] = {0,1,2,3,4,5}; 
        video::IVideoDriver* driver = SceneManager->getVideoDriver(); 

        driver->setMaterial(Material); 
        driver->setTransform(video::ETS_WORLD, AbsoluteTransformation); 
        driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
        driver->setMaterial(SMaterial());
    } 
    //Renders the Post-Process Effect 

    virtual u32 getMaterialCount(){return 1;} 
    virtual video::SMaterial& getMaterial(s32 i){return (Material);} 
    virtual const core::aabbox3d<f32>& getBoundingBox() const{return Box;} 
}; 