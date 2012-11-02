#pragma once


class LifterScene {
public:
    LifterScene();
    virtual ~LifterScene();
    
    void init(IVideoDriver* driver_, ISceneManager* smgr_);
    bool loadMap(wchar_t* Path);
    
    void release();
    
    void addActor(Point pos, CellType type);
    void moveActor(Point pos);
    void removeActor(Point pos);
    
    void updateIndices();
private:
    IVideoDriver* driver;
    ISceneManager* smgr;
    
    ITexture* pWallTex;
    ITexture* pWallBump;
    ITexture* pEarthTex;
    ITexture* pEarthBump;
    ITexture* pStoneTex;
    ITexture* pStoneBump;
    ITexture* pRobotTex;
    ITexture* pRobotBump;
    ITexture* pFireTex;
    ITexture* pLiftTex;
    ITexture* pSunTex;
    
    IMesh* pStoneMesh;
    IMesh* pLambdaMesh;
    IMesh* pRobotMesh;
    IMesh* pCubeMesh;
    
    
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
};

