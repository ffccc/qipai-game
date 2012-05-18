#ifndef __XLBE_SPRITE_H__
#define __XLBE_SPRITE_H__

#include <map>

#include "xlbe.h"

#include "render/render_system.h"

#include "animation.h"
#include "animation_cache.h"
#include "scene_node.h"

/////////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////////
class Xlbe_Export Sprite : public Scene_Node
{
public:
    typedef std::map<std::string, Animation*> Animations;

public:
    Sprite();
    Sprite(Render_Texture *tex);
    Sprite(Render_Texture *tex, const Rect &rect);
    Sprite(const std::string &file);   //texture file
    Sprite(const std::string &file, const Rect &rect); //texture file
    Sprite(Sprite_Frame *frame);
    Sprite(Animation *ani);
    virtual ~Sprite();

public:
    unsigned long color() const { return color_; }
    void set_color(unsigned long color);

    Render_Texture *texture() const { return texture_; }
    void set_texture(Render_Texture *tex, const Rect &rect);

public:
    virtual void draw(); 
    virtual void transform();

public:
    void set_display_frame(Sprite_Frame *frame);
    void set_display_frame(const std::string &animation, int index); 

    bool is_frame_displayed(Sprite_Frame *frame);
    Sprite_Frame  displayed_frame();

    Animation *animation_by_tag(int tag);
    Animation *animation_by_name(const std::string &animation);
    void add_animation(Animation* animation);

private:
    bool xflip_, yflip_;
    unsigned long color_;

    Rect rect_;
    Render_Quad quad_;
    Render_Texture *texture_;

    Animations animations_;
};

/////////////////////////////////////////////////////////
//}
#endif