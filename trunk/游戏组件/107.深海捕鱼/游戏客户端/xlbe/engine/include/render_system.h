#ifndef __XLBE_RENDER_SYSTEM_H__
#define __XLBE_RENDER_SYSTEM_H__

#include <string>

#include "prereqs.h"

#include "config_option.h"

#include "render_primitive.h"
#include "render_window.h"
#include "render_texture.h"

/////////////////////////////////////////////////////////////////
//namespace xlbe{

/////////////////////////////////////////////////////////////////
class Xlbe_Export Render_System
{
public:
    Render_System() {}
    virtual ~Render_System() {}

public:
    virtual Config_Option_Map &config_options() =0;
    virtual void set_config_options(const std::string &name, const std::string &value) =0;

    virtual Render_Window *initialise(const std::string &title) =0;
    virtual void shutdown() =0;

    virtual bool zbuffer() = 0;
    virtual void set_zbuffer(bool zbuffer)= 0;

    virtual bool texture_filter()= 0;
    virtual void set_texture_filter(bool filter)= 0;

    virtual Rect caption_rect() const= 0;
    virtual void set_caption_rect(const Rect &rc)= 0; 

    virtual bool hide_mouse() const = 0;
    virtual void set_hide_mouse(bool hide)= 0;

    virtual bool deactivated_on_focus_change() const= 0;
    virtual void set_deactivated_on_focus_change(bool deactivated)= 0;

    virtual void set_world_matrix()= 0;
    virtual void set_view_matrix(float width, float height)= 0;
    virtual void set_projection_matrix(int width, int height)= 0;

    virtual bool begin_scene() =0;
    virtual void end_scene() =0;
    virtual void clear(unsigned long color) =0;

    virtual void render_line(Render_Line *line) =0;
    virtual void render_triple(Render_Triple *triple) =0;
    virtual void render_quad(Render_Quad *quad) =0;

    virtual Render_Texture *add_texture(const std::string &name, const std::string &file) =0;
    virtual void remove_texture(Render_Texture *tex) =0;
    virtual void remove_texture(const std::string &name) =0;
    virtual void remove_all_texture() =0;
};

/////////////////////////////////////////////////////////////////

//}
#endif