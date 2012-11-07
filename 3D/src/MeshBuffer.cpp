
#include "stdinclude.h"
#include "MeshBuffer.h"

const int cube_indices[36] = {
	 0, 3, 1,  1, 3, 2, // front
	 4, 7, 5,  5, 7, 6, // back
	 8,11, 9,  9,11,10, // top
	12,15,13, 13,15,14, // bottom
	16,19,17, 17,19,18, // left
	20,23,21, 21,23,22  // right
};

MeshBuffer::MeshBuffer() {
    mesh=new SMesh();
    width=0;
    height=0;
    cellsize=1.f;
    texscaleX=1;
    texscaleY=1;
}

MeshBuffer::~MeshBuffer() {
    mesh->drop();
}

void MeshBuffer::create(int width_, int height_, float cellsize_,float texScaleX, float texScaleY,char* array, IVideoDriver *driver, ISceneManager* smgr)
{
    width=width_;
    height=height_;
    cellsize=cellsize_;
    texscaleX=texScaleX;
    texscaleY=texScaleY;
    
    maxPrimitives = driver -> getMaximalPrimitiveCount();
    const u32 bufcubes = maxPrimitives / (24);
    maxBuffers = (width*height) / bufcubes;
    if(maxBuffers==0) maxBuffers=1;
    maxCubes = ((width*height)>bufcubes)?bufcubes:(width*height);
    
    update(array);
}

void MeshBuffer::update(char* array)
{
    int x=0;
    int y=0;
    u32 i=0;
    int nCubes=1;
    for(i=0;(i<maxBuffers) && (nCubes!=0);i++)
    {
        nCubes=addCubes(x,y,maxCubes,i,array);
    }
    
    if (i<mesh->getMeshBufferCount())
    {
            // clear the rest
            for (u32 j=i; j<mesh->getMeshBufferCount(); ++j)
            {
                    mesh->getMeshBuffer(j)->drop();
            }
            mesh->MeshBuffers.erase(i,mesh->getMeshBufferCount()-i);
    }
    
    mesh->recalculateBoundingBox();
}

int MeshBuffer::addCubes(int& x_start, int& y_start,int max_cnt, u32 bufNum,char* array)
{
    int i=0;
    int n_ind=0;
    int curr_cnt=0;
    int bFirst=0;
    int x,y;
    float size=cellsize/2;
    float texX=0;
    float texY=0;
    float texXpp=0;
    float texYpp=0;
    SMeshBufferTangents *buf = 0;
    
    if (bufNum<mesh->getMeshBufferCount())
    {
            buf = (SMeshBufferTangents*)mesh->getMeshBuffer(bufNum);
    }
    else
    {
            // create new buffer
            buf = new SMeshBufferTangents();
            mesh->addMeshBuffer(buf);
            // to simplify things we drop here but continue using buf
            buf->drop();
    }
    
    int cnt=0;
    int verts=0;
    int inds=0;
    for(y=y_start;y<height;y++)
    {
        for(x=!(bFirst++)?x_start:0;(x<width) && (cnt<=max_cnt);x++)
        {
            if(array[y*width+x]!=0) 
            {
                cnt++;
                sMeshBufferAdjarency adj=getAdjarency(x,y,array);
                verts+=8+(adj.top?0:4)+(adj.bottom?0:4)+(adj.left?0:4)+(adj.right?0:4);
                inds+=12+(adj.top?0:6)+(adj.bottom?0:6)+(adj.left?0:6)+(adj.right?0:6);
            }

        }
    };
    
    buf->Vertices.set_used(verts);
    buf->Indices.set_used(inds);
    
    bFirst=0;
    for(y=y_start;(y<height) && (curr_cnt<cnt);y++)
    {
        for(x=!(bFirst++)?x_start:0;(x<width) && (curr_cnt<cnt);x++)
        {
            if(array[y*width+x]==0) continue;
            curr_cnt++;
            
            sMeshBufferAdjarency adj=getAdjarency(x,y,array);
            
            int i0=i;
            // front
            texX=(float(x)/float(width))*texscaleX;
            texY=(float(y)/float(height))*texscaleY;
            texXpp=texX+texscaleX/float(width);
            texYpp=texY+texscaleY/float(height);
            
            // front
            buf->Vertices[i++]=S3DVertexTangents(-size, size, size,0.0f, 0.0f, 1.0f,SColor(255,255,255,255),texX,texYpp,1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, size, size,0.0f, 0.0f, 1.0f,SColor(255,255,255,255),texXpp,texYpp,1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, -size, size,0.0f, 0.0f, 1.0f,SColor(255,255,255,255),texXpp,texY,1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, -size, size,0.0f, 0.0f, 1.0f,SColor(255,255,255,255),texX,texY,1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            // back
            buf->Vertices[i++]=S3DVertexTangents(size, size, -size,0.0f, 0.0f, -1.0f,SColor(255,255,255,255),texXpp,texYpp,1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, size, -size,0.0f, 0.0f, -1.0f,SColor(255,255,255,255),texX,texYpp,1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, -size, -size,0.0f, 0.0f, -1.0f,SColor(255,255,255,255),texX,texY,1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, -size, -size,0.0f, 0.0f, -1.0f,SColor(255,255,255,255),texXpp,texY,1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);
            
            for(int j=0;j<12;j++)  buf->Indices[n_ind++]=i0+cube_indices[j];
            // top
            if(!adj.bottom) {
                int i1=i;
                buf->Vertices[i++]=S3DVertexTangents(-size, size, -size,0.0f, 1.0f, 0.0f,SColor(255,255,255,255),texX,texYpp,1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);
                buf->Vertices[i++]=S3DVertexTangents(size, size, -size,0.0f, 1.0f, 0.0f,SColor(255,255,255,255),texXpp,texYpp,1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);
                buf->Vertices[i++]=S3DVertexTangents(size, size, size,0.0f, 1.0f, 0.0f,SColor(255,255,255,255),texXpp,texY,1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);
                buf->Vertices[i++]=S3DVertexTangents(-size, size, size,0.0f, 1.0f, 0.0f,SColor(255,255,255,255),texX,texY,1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);
                for(int j=0;j<6;j++)  buf->Indices[n_ind++]=i1+cube_indices[j];
            }
            // bottom
            if(!adj.top) {
                int i1=i;
                buf->Vertices[i++]=S3DVertexTangents(size, -size, -size,0.0f, -1.0f, 0.0f,SColor(255,255,255,255),texXpp,texY,1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
                buf->Vertices[i++]=S3DVertexTangents(-size, -size, -size,0.0f, -1.0f, 0.0f,SColor(255,255,255,255),texX,texY,1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
                buf->Vertices[i++]=S3DVertexTangents(-size, -size, size,0.0f, -1.0f, 0.0f,SColor(255,255,255,255),texX,texYpp,1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
                buf->Vertices[i++]=S3DVertexTangents(size, -size, size,0.0f, -1.0f, 0.0f,SColor(255,255,255,255),texXpp,texYpp,1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
                for(int j=0;j<6;j++)  buf->Indices[n_ind++]=i1+cube_indices[j];
            }
             // right
            if(!adj.left) {
                int i1=i;
                buf->Vertices[i++]=S3DVertexTangents(-size, size, -size,1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texX,texYpp,0.0f, 0.0f, -1.0f,0.0f, -1.0f, 0.0f);
                buf->Vertices[i++]=S3DVertexTangents(-size, size, size,1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texXpp,texYpp,0.0f, 0.0f, -1.0f,0.0f, -1.0f, 0.0f);
                buf->Vertices[i++]=S3DVertexTangents(-size, -size, size,1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texXpp,texY,0.0f, 0.0f, -1.0f,0.0f, -1.0f, 0.0f);
                buf->Vertices[i++]=S3DVertexTangents(-size, -size, -size,1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texX,texY,0.0f, 0.0f, -1.0f,0.0f, -1.0f, 0.0f);
                for(int j=0;j<6;j++)  buf->Indices[n_ind++]=i1+cube_indices[j];
            }   
            // left
            if(!adj.right) {
                int i1=i;
                buf->Vertices[i++]=S3DVertexTangents(size, size, size,-1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texX,texYpp,0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f);
                buf->Vertices[i++]=S3DVertexTangents(size, size, -size,-1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texXpp,texYpp,0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f);
                buf->Vertices[i++]=S3DVertexTangents(size, -size, -size,-1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texXpp,texY,0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f);
                buf->Vertices[i++]=S3DVertexTangents(size, -size, size,-1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texX,texY,0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f);
                for(int j=0;j<6;j++)  buf->Indices[n_ind++]=i1+cube_indices[j];
            }
            
            for(int i1=i0;i1<i;i1++)
            {
                buf->Vertices[i1].Pos+=vector3df(cellsize*x,cellsize*y,0);
            }
        }
    }
    
    x_start=x;
    y_start=y-1;
    buf->recalculateBoundingBox();
    return curr_cnt;
}

sMeshBufferAdjarency MeshBuffer::getAdjarency(int x, int y,char* array)
{
    sMeshBufferAdjarency adj;
    if((x<0) || (y<0) || (x>=width) || (x>=height)) 
        return adj;
    
    if(x>0) adj.left=(bool)array[y*width+x-1];
    if(y>0) adj.top=(bool)array[(y-1)*width+x];
    if(x<(width-1)) adj.right=(bool)array[y*width+x+1];
    if(y<(height-1)) adj.bottom=(bool)array[(y+1)*width+x];
    //adj.left=false;
    //adj.top=false;
    //adj.right=true;
    //adj.bottom=true;
    return adj;
}