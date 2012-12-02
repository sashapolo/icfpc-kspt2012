#pragma once

class AnimSprite : public ISceneNode
{
private:
    core::aabbox3d<f32> Box; 
    video::S3DVertex    Vertices[4];
    u16                 Indices[12]; 
    u32                 wFrames, hFrames;
    f32                 wStep, hStep;
    timespec startTime;
    u32 lifeTime;
    video::SMaterial Material;
    bool bDead;
    bool bCycled;
    float Size;
    
public: 
    AnimSprite(ISceneNode* parent, ISceneManager* mgr): ISceneNode(parent, mgr)
    {      
        u16 ind[] = { 0,1,3, 3,1,2, 1,0,2, 2,0,3 };
        memcpy(Indices,ind,sizeof(u16)*12);      
        float x=0.5f;
        float y=0.5f;
        Vertices[0] = S3DVertex(-x,-y,0, 0,0,0,SColor(255,255,255,255),0,1);
        Vertices[1] = S3DVertex( x,-y,0, 0,0,0,SColor(255,255,255,255),1,1); 
        Vertices[2] = S3DVertex( x, y,0, 0,0,0,SColor(255,255,255,255),1,0);
        Vertices[3] = S3DVertex(-x, y,0, 0,0,0,SColor(255,255,255,255),0,0);
        
        Box.reset(Vertices[0].Pos);
        for (s32 i=1; i<4; ++i)  Box.addInternalPoint(Vertices[i].Pos);
        Material=video::IdentityMaterial;
        Size=1;
    }     

    virtual void Init(u32 wFrames_,u32 hFrames_,u32 lifeTime_,float Size_, bool bCycled_)
    {
        wFrames=wFrames_;
        hFrames=hFrames_;
        wStep=(1.0/float(wFrames));
        hStep=(1.0/float(hFrames));
        lifeTime=lifeTime_;
        clock_gettime(CLOCK_MONOTONIC,&(startTime));
        bCycled=bCycled_;
        bDead=false;
        Size=Size_;
        
        setFrame(0);
    }
    
    bool isDead()
    {
        return bDead;
    }
    
    virtual void setFrame(int n)
    {
        float x = wStep*(n % wFrames);
        float y = hStep*(n / wFrames);
        
        Vertices[0].TCoords.X = x; 
        Vertices[0].TCoords.Y = y+hStep;
        Vertices[1].TCoords.X = x+wStep; 
        Vertices[1].TCoords.Y = y+hStep;
        Vertices[2].TCoords.X = x+wStep; 
        Vertices[2].TCoords.Y = y;
        Vertices[3].TCoords.X = x; 
        Vertices[3].TCoords.Y = y;
    }

    virtual void OnRegisterSceneNode()
    { 
        if (IsVisible)    SceneManager->registerNodeForRendering(this);
        ISceneNode::OnRegisterSceneNode();
        //ISceneNode::OnPreRender();
    }

    virtual void Update()
    {
        timespec currentTime;
        clock_gettime(CLOCK_MONOTONIC,&currentTime);
    
        u32 timeLeft=msecDiff(startTime,currentTime);
        
        if(timeLeft<lifeTime)
        {
            int nFrame=(float(timeLeft)/float(lifeTime))*float(hFrames*wFrames);
            setFrame(nFrame);
            LOGINFO("Timeleft: %d, %d",timeLeft,0);
        } 
        else
        {
            if(bCycled) clock_gettime(CLOCK_MONOTONIC,&(startTime));
            else bDead=true;
        }
    }     
    
    virtual void render()
    {  
        Update();
        if(bDead) return;
        video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	// make billboard look to camera

	core::vector3df pos = getAbsolutePosition();

	core::vector3df campos = camera->getAbsolutePosition();
	core::vector3df target = camera->getTarget();
	core::vector3df up = camera->getUpVector();
	core::vector3df view = target - campos;
	view.normalize();

	core::vector3df horizontal = up.crossProduct(view);
	if ( horizontal.getLength() == 0 )
	{
		horizontal.set(up.Y,up.X,up.Z);
	}
	horizontal.normalize();
	horizontal *= 0.5f * Size;

	core::vector3df vertical = horizontal.crossProduct(view);
	vertical.normalize();
	vertical *= 0.5f * Size;

	view *= -1.0f;

	for (s32 i=0; i<4; ++i)
		Vertices[i].Normal = view;

	Vertices[0].Pos = pos + horizontal + vertical;
	Vertices[1].Pos = pos - horizontal + vertical;
	Vertices[2].Pos = pos - horizontal - vertical;
	Vertices[3].Pos = pos + horizontal - vertical;

	// draw

	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(Box, video::SColor(0,208,195,152));
	}

	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

	driver->setMaterial(getMaterial(0));

	driver->drawIndexedTriangleList(&Vertices[0], 4, &Indices[0], 4);
        ///
        //IVideoDriver* driver = SceneManager->getVideoDriver();
        //driver->setMaterial(getMaterial(0));
        //driver->setTransform(ETS_WORLD, AbsoluteTransformation);
        //driver->drawIndexedTriangleList(&Vertices[0], 4, &Indices[0], 4);
    }

    virtual const aabbox3d<f32>& getBoundingBox() const 
    { 
        return Box; 
    }
    
    virtual video::SMaterial& getMaterial(u32 num)
    {
        return Material;
    }
    virtual u32 getMaterialCount() const
    {
        return 1;
    }
}; 