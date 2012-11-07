#pragma once

struct sMeshBufferAdjarency
{
    sMeshBufferAdjarency()
    {
        top=false;
        bottom=false;
        left=false;
        right=false;
    }
    bool top;
    bool bottom;
    bool left;
    bool right;
    
    void operator = (sMeshBufferAdjarency& other)
    {
        top=other.top;
        bottom=other.bottom;
        left=other.left;
        right=other.right;
    }
};

class MeshBuffer {
public:
    MeshBuffer();
    virtual ~MeshBuffer();
    
    void create(int width_, int height_, float cellsize_,float texScaleX, float texScaleY, char* array, IVideoDriver *driver, ISceneManager* smgr);
    void update(char* array);
    int addCubes(int& x_start, int& y_start,int max_cnt, u32 bufNum,char* array);
    void setIndices(char* array);
    sMeshBufferAdjarency getAdjarency(int x, int y,char* array); 
    
    SMesh* mesh;
private:
    
    int width;
    int height;
    
    float cellsize;
    float texscaleX;
    float texscaleY;
    
    u32 maxPrimitives;
    u32 maxBuffers;
    u32 maxCubes;
};
