#ifndef __XLBE_RENDER_TEXTURE_D3D9_H__
#define __XLBE_RENDER_TEXTURE_D3D9_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "render_texture.h"

/////////////////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////////////////
class Xlbe_Export Render_Texture_D3d9 : public Render_Texture
{
public:
    Render_Texture_D3d9(const std::string &name, int width, int height);
    Render_Texture_D3d9(const std::string &name, const std::string& filename);
    virtual ~Render_Texture_D3d9();

public:
    virtual std::string name() const               { return name_; }
    virtual void set_name(const std::string &name) { name_ = name; } 

    virtual void create(int width, int height);
    virtual void load(const std::string &file);
    virtual void destory();

    virtual Size size() { return Size(width_, height_); }

    virtual unsigned long *lock(bool readonly, int left=0, int top=0, int width=0, int height=0);
    virtual void unlock();

public:
    IDirect3DTexture9 *texture() const { return tex_; }

private:
    int width_;
    int height_;
    std::string name_;
    IDirect3DTexture9* tex_;
};

/////////////////////////////////////////////////////////////////
//}
#endif