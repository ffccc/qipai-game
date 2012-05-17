#ifndef __XLBE_RENDER_PRIMITIVE_H__
#define __XLBE_RENDER_PRIMITIVE_H__

///////////////////////////////////////////////////////////////
//namespace xlbe {

class Render_Texture;

///////////////////////////////////////////////////////////////
enum Render_Blend
{
    BLEND_COLORMUL_ALPHAADD = 0,
    BLEND_COLORADD_ALPHAADD,
    BLEND_COLORMUL_ALPHABLEND,
    BLEND_COLORADD_ALPHABLEND,
};

///////////////////////////////////////////////////////////////
struct Vertex
{
    float x, y, z;	
    unsigned long color;		
    float tx, ty;		

    Vertex() { x=0; y=0; z=0; tx=0; ty=0; color=0xFFFFFFFF;  }

};

///////////////////////////////////////////////////////////////
struct Render_Line 
{
    Render_Blend blend_;

    static const int count_ = 2;
    Vertex vertex_[count_];

    Render_Texture *texture_;

    Render_Line() { blend_=BLEND_COLORMUL_ALPHABLEND; texture_=0; }
};

///////////////////////////////////////////////////////////////
struct Render_Triple 
{
    Render_Blend blend_;

    static const int count_ = 3;
    Vertex vertex_[count_];

    Render_Texture *texture_;

    Render_Triple() { blend_=BLEND_COLORMUL_ALPHABLEND; texture_=0; }
};

///////////////////////////////////////////////////////////////
struct Render_Quad 
{
    Render_Blend blend_;

    static const int count_ = 4;
    Vertex vertex_[count_];

    Render_Texture *texture_;

    Render_Quad() { blend_=BLEND_COLORMUL_ALPHABLEND; texture_=0; }
};

///////////////////////////////////////////////////////////////
//}
#endif