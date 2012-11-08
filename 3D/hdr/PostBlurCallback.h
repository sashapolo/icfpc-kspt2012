/* 
 * File:   PostBlurCallback.h
 * Author: kirill
 *
 * Created on November 7, 2012, 12:33 AM
 */

#pragma once
 
class PostBlurCallback: public video::IShaderConstantSetCallBack 
{ 
public: 
    float sampleDist; 
    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) 
    { 
        services->setVertexShaderConstant("sampleDist", reinterpret_cast<f32*>(&sampleDist),1); 
    } 
}; 
 
 
 
 
class IPostProcessBlur : public scene::ISceneNode 
{ 
public: 
    core::aabbox3d<f32> Box; 
    video::S3DVertex Vertices[6];//the vertices for the onscreenquad 
    video::SMaterial Material; 
    video::ITexture* rt0; //the rendertarget 
    int mat; 
    PostBlurCallback* callback; 

    IPostProcessBlur(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id): scene::ISceneNode(parent, mgr, id) 
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


    void initiate(int sizeW,int sizeH,float sampleDist,scene::ISceneManager* smgr) 
    { 
        video::IVideoDriver* driver = smgr->getVideoDriver(); 
        video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices(); 
        
        callback= new PostBlurCallback; 
        callback->sampleDist=sampleDist; 
        Material.MaterialType=(E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles 
        ( 
            "3D/res/shaders/bloom.vert", "main", video::EVST_VS_1_1, 
            "3D/res/shaders/blur.frag", "main", video::EPST_PS_1_1, 
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
 