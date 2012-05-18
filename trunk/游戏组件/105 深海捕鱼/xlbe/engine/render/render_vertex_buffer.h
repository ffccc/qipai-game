#ifndef __XLBE_RENDER_VERTEX_BUFFER_H__
#define __XLBE_RENDER_VERTEX_BUFFER_H__

#include <d3d9.h>
#include <d3dx9.h>

#include "xlbe.h"

//////////////////////////////////////////////////////////////
struct Vertex
{
    float x, y;	
    float z;			
    unsigned long col;		
    float tx, ty;		
};

//////////////////////////////////////////////////////////////
//namespace xlbe{

class Xlbe_Export Render_System;

//////////////////////////////////////////////////////////////
class Render_Vertex_Buffer
{
public:
    Render_Vertex_Buffer(Render_System *system);
    ~Render_Vertex_Buffer();

public:
    void initialise();
    void destroy();

    unsigned int vertex_count() const  { return vertex_count_; }
    void set_vertex_count(unsigned int count) ;

    void set_indices();

    Vertex* lock();
    void unlock();

private:
    Render_System *render_system_;
    IDirect3DVertexBuffer9 *vertex_buffer_;
    IDirect3DIndexBuffer9  *index_buffer_;

    unsigned int vertex_count_;
};

//////////////////////////////////////////////////////////////
//}
#endif 