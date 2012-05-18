#ifndef __XLBE_SPRITE_FRAME_H__
#define __XLBE_SPRITE_FRAME_H__

#include "xlbe.h"
#include "utility/rect.h"

#include "render/render_system.h"

//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
class Xlbe_Export Sprite_Frame
{
public:
    Sprite_Frame(Render_Texture *tex);
    Sprite_Frame(Render_Texture *tex, const Rect &rect);
    ~Sprite_Frame();

public:
    Rect rect() const                { return rect_; }
    void set_rect(const Rect &rect)  { rect_ = rect; }

    Render_Texture *texture()             { return texture_; }
    void set_texture(Render_Texture *tex) { texture_ = tex; }

public:
    Rect rect_;
    Render_Texture *texture_;

};

//////////////////////////////////////////////////////////////
//}
#endif