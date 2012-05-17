#ifndef __XLBE_RENDER_TEXTURE_H__
#define __XLBE_RENDER_TEXTURE_H__

#include <d3d9.h>
#include <d3dx9.h>

#include <string>
#include <fstream>
#include <iostream>

#include "xlbe.h"

#include "utility/size.h"
#include "utility/point.h"
#include "utility/rect.h"

/////////////////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////////////////
class Xlbe_Export Render_Texture
{
public:
    Render_Texture(int width, int height);
    Render_Texture(const std::string& filename);
    ~Render_Texture();

public:
    IDirect3DTexture9 *texture() const { return tex_; }
    void set_texture();

    void create(int width, int height);
    void load(const std::string &filename);
    void destory();

    Size get_size() const { return Size(width_, height_); }

private:
    int width_;
    int height_;
    IDirect3DTexture9* tex_;
};

/////////////////////////////////////////////////////////////////
//}
#endif
