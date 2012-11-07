
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

void MeshBuffer::init(int width_, int height_, float cellsize_,float texScaleX, float texScaleY, IVideoDriver *driver, ISceneManager* smgr)
{
    width=width_;
    height=height_;
    cellsize=cellsize_;
    vb_width=width+1;
    vb_height=height+1;
    texscaleX=texScaleX;
    texscaleY=texScaleY;
    
    
    const u32 maxprim = driver -> getMaximalPrimitiveCount();
    const u32 bufcubes = maxprim / (24);
    u32 buffers = (width*height) / bufcubes;
    if(buffers==0) buffers=1;
    const u32 maxcubes = (width*height);
    
    int x=0;
    int y=0;
    u32 i=0;
    for(i=0;i<buffers;i++)
    {
        addCube(x,y,(maxcubes>bufcubes)?bufcubes:maxcubes,i);
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
    
    //mesh=smgr->getMeshManipulator()->createMeshWithTangents(mesh);
}

void MeshBuffer::addCube(int& x_start, int& y_start,int cnt, u32 bufNum)
{
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
    
    buf->Vertices.set_used(cnt*24);
    
    int i=0;
    int curr_cnt=0;
    int bFirst=0;
    int x,y;
    float size=cellsize/2;
    float texX=0;
    float texY=0;
    float texXpp=0;
    float texYpp=0;
    for(y=y_start;(y<height) && (curr_cnt<cnt);y++)
    {
        for(x=!(bFirst++)?x_start:0;(x<width) && (curr_cnt<cnt);x++,curr_cnt++)
        {
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
            // top
            buf->Vertices[i++]=S3DVertexTangents(-size, size, -size,0.0f, 1.0f, 0.0f,SColor(255,255,255,255),texX,texYpp,1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, size, -size,0.0f, 1.0f, 0.0f,SColor(255,255,255,255),texXpp,texYpp,1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, size, size,0.0f, 1.0f, 0.0f,SColor(255,255,255,255),texXpp,texY,1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, size, size,0.0f, 1.0f, 0.0f,SColor(255,255,255,255),texX,texY,1.0f, 0.0f, 0.0f,0.0f, 0.0f, -1.0f);
            // bottom
            buf->Vertices[i++]=S3DVertexTangents(size, -size, -size,0.0f, -1.0f, 0.0f,SColor(255,255,255,255),texXpp,texY,1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, -size, -size,0.0f, -1.0f, 0.0f,SColor(255,255,255,255),texX,texY,1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, -size, size,0.0f, -1.0f, 0.0f,SColor(255,255,255,255),texX,texYpp,1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, -size, size,0.0f, -1.0f, 0.0f,SColor(255,255,255,255),texXpp,texYpp,1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
             // right
            buf->Vertices[i++]=S3DVertexTangents(-size, size, -size,1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texX,texYpp,0.0f, 0.0f, -1.0f,0.0f, -1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, size, size,1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texXpp,texYpp,0.0f, 0.0f, -1.0f,0.0f, -1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, -size, size,1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texXpp,texY,0.0f, 0.0f, -1.0f,0.0f, -1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(-size, -size, -size,1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texX,texY,0.0f, 0.0f, -1.0f,0.0f, -1.0f, 0.0f);
            // left
            buf->Vertices[i++]=S3DVertexTangents(size, size, size,-1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texX,texYpp,0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, size, -size,-1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texXpp,texYpp,0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, -size, -size,-1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texXpp,texY,0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f);
            buf->Vertices[i++]=S3DVertexTangents(size, -size, size,-1.0f, 0.0f, 0.0f,SColor(255,255,255,255),texX,texY,0.0f, 0.0f, 1.0f,0.0f, -1.0f, 0.0f);
            
            for(int i1=i0;i1<i;i1++)
            {
                buf->Vertices[i1].Pos+=vector3df(cellsize*x,cellsize*y,0);
            }
        }
    }
    x_start=x;
    y_start=y-1;
    buf->recalculateBoundingBox();
}

void MeshBuffer::setIndices(char* array)
{
    int x=0;
    int y=0;
    for(u32 z=0;z<mesh->getMeshBufferCount();z++)
    {
        SMeshBuffer *buf=(SMeshBuffer*)mesh->getMeshBuffer(z);
        int vertcnt=buf->getVertexCount();
        int cubecnt=vertcnt/24;
        int indcnt=0;
        
        int x0=x;
        int y0=y;
        for(int i=0;i<cubecnt;i++)
        {
            if(array[y0*width+x0]==1) 
            {
                indcnt+=36;
            }
            x0++;
            if(x0>=width) 
            {
                x0=0;
                y0++;
            }
        }
        
        buf->Indices.set_used(indcnt);
        
        int n_ind=0;
        for(int i=0;i<cubecnt;i++)
        {
            if(array[y*width+x]==1) 
            {
                u16 n=i*24;
                //printf("n= %d\n",n);
                for(int j=0;j<36;j++)
                {
                    buf->Indices[n_ind++]=n+cube_indices[j];
                }
            }
          
            x++;
            if(x>=width) 
            {
                x=0;
                y++;
            }
        }
        buf->recalculateBoundingBox();
    }
    
    mesh->recalculateBoundingBox();
}