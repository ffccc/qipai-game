#ifndef __XLBE_RENDER_SYSTEM_H__
#define __XLBE_RENDER_SYSTEM_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "xlbe.h"

#include "render_window.h"
#include "render_texture.h"
#include "render_texture_cache.h"
#include "render_vertex_buffer.h"

/////////////////////////////////////////////////////////////////
//namespace xlbe{

/////////////////////////////////////////////////////////////////
#define PRIM_QUADS	  	4

#define	BLEND_COLORADD		1
#define	BLEND_COLORMUL		0
#define	BLEND_ALPHABLEND	2
#define	BLEND_ALPHAADD		0
#define	BLEND_ZWRITE	  	4
#define	BLEND_NOZWRITE		0
#define BLEND_DEFAULT		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE)
#define BLEND_DEFAULT_Z		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)

//////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
struct Render_Quad
{
    Vertex v[4];
    unsigned long tex;
    int blend;
};

/////////////////////////////////////////////////////////////////
class Xlbe_Export Render_System
{
public:
    Render_System();
    ~Render_System();

public:
    IDirect3D9 *d3d() const            { return d3d_; }
    IDirect3DDevice9 *device() const   { return device_; }

    void initialise(Render_Window *win);
    void shutdown();

    void begin_scene();
    void end_scene();

    void clear(unsigned long color);
    void render_quad(Render_Quad *quad);
    void render_batch(bool endscene=false);

    void set_blend_mode(int blend);

    void restore(bool windowed);

private:
    void set_transform();
    void set_projection_matrix(int width, int height);

private:
    IDirect3D9*	d3d_;
    IDirect3DDevice9* device_;
    IDirect3DTexture9 *texture_ ;
    D3DPRESENT_PARAMETERS *d3dpp_;
    D3DPRESENT_PARAMETERS d3dppw_;
    D3DPRESENT_PARAMETERS d3dppf_;

    int blend_mode_ ;  

    int prim_;  
    Vertex*	vertarray_;

    D3DXMATRIX matview_;
    D3DXMATRIX matproj_;

    Render_Vertex_Buffer *vertex_buffer_;
};

/////////////////////////////////////////////////////////////////

//}
#endif