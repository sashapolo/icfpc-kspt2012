#pragma once


class LifterScene {
public:
    LifterScene();
    virtual ~LifterScene();
    
    void init(IVideoDriver* driver_, ISceneManager* smgr_);
    bool loadMap(wchar_t* Path);
    
    void release();
    void clear();
    
    void addActor(Point pos, CellType type);
    void moveActor(Point pos);
    void removeActor(Point pos);
    
    void updateIndices();
    void updateScene();
private:
    IVideoDriver* driver;
    ISceneManager* smgr;
    
    ITexture* pWallTex;
    ITexture* pWallBump;
    ITexture* pWallGlow;
    ITexture* pWallSpecular;
    
    ITexture* pEarthTex;
    ITexture* pEarthBump;
    ITexture* pEarthGlow;
    ITexture* pEarthSpecular;
    
    ITexture* pStoneTex;
    ITexture* pStoneBump;
    ITexture* pStoneSpecular;
    ITexture* pRobotTex;
    ITexture* pRobotBump;
    ITexture* pFireTex;
    ITexture* pLiftTex;
    ITexture* pSunTex;
    
    ITexture* pBlackTex;
    ITexture* pWhiteTex;
    ITexture* pGreyTex;
    
    ITexture* pStoneSpriteTex;
    ITexture* pStoneSpriteBump;
    ITexture* pStoneSpriteSpecular;
    ITexture* pLambdaSpriteTex;
    
    IMesh* pStoneMesh;
    IMesh* pLambdaMesh;
    IMesh* pRobotMesh;
    
    
    scene::ISceneNode* pWallMeshBufferNode;
    scene::ISceneNode* pEarthMeshBufferNode;
    
    char* earth_ind;
    char* wall_ind;
    
    Field* pField;
    FieldSim sim;
    

    MeshBuffer mbWall;
    MeshBuffer mbEarth;
    
    std::vector<scene::ISceneNode*> StoneArr;
    std::vector<scene::ISceneNode*> LambdaArr;
    
    scene::ISceneNode* pRobotNode;
    scene::ISceneNode* pLiftNode;
    
    scene::ILightSceneNode* light1;
    
    scene::ICameraSceneNode* pCamera;
    
    s32 bumpMaterial;
    s32 parallaxMaterial;
};

