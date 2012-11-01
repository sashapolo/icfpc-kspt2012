#pragma once

class MeshBuffer {
public:
    MeshBuffer();
    virtual ~MeshBuffer();
    
    void init(int width_, int height_, float cellsize_,float texScaleX, float texScaleY, IVideoDriver *driver, ISceneManager* smgr);
    void addCube(int& x_start, int& y_start,int cnt, u32 bufNum);
    void setIndices(char* array);
    
    SMesh* mesh;
private:
    
    int width;
    int height;
    int vb_width;
    int vb_height;
    
    float cellsize;
    float texscaleX;
    float texscaleY;
};
