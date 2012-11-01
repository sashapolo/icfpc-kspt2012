#pragma once


class LifterScene {
public:
    LifterScene();
    virtual ~LifterScene();
    
    void init(IVideoDriver* driver_, ISceneManager* smgr_);
    bool loadMap(wchar_t* Path);
    
    void release() {};
    
    void writeActor(Point pos, CellType type);
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
    ITexture* pRobotTex;
    ITexture* pFireTex;
    
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
};

