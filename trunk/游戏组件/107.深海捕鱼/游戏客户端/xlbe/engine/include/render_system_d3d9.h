#ifndef __XLBE_RENDER_SYSTEM_D3D9_H__
#define __XLBE_RENDER_SYSTEM_D3D9_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "prereqs.h"

#include "config_option.h"

#include "render_system.h"
#include "render_texture_d3d9.h"
#include "render_window_d3d9.h"

/////////////////////////////////////////////////////////////////
//namespace xlbe{

/////////////////////////////////////////////////////////////////
class Render_System_D3d9 : public Render_System
{
public:
    typedef std::map<std::string, Render_Texture*> Textures;

public:
    Render_System_D3d9();
    virtual ~Render_System_D3d9();

public:
    virtual Config_Option_Map& config_options();
    virtual void set_config_options(const std::string &name, const std::string &value) ;

    virtual Render_Window *initialise(const std::string &title);
    virtual void shutdown() ;

    virtual bool zbuffer()                 { return zbuffer_; }
    virtual void set_zbuffer(bool zbuffer) ;

    virtual bool texture_filter()                { return texture_filter_; }
    virtual void set_texture_filter(bool filter);

    virtual Rect caption_rect() const  { return caption_; }
    virtual void set_caption_rect(const Rect &rc) { caption_ = rc; }

    virtual bool hide_mouse() const              { return hide_mouse_; }
    virtual void set_hide_mouse(bool hide)       { hide_mouse_ = hide;  } 

    virtual bool deactivated_on_focus_change() const               { return deactivated_; }
    virtual void set_deactivated_on_focus_change(bool deactivated) { deactivated_ = deactivated; }

    virtual void set_world_matrix();
    virtual void set_view_matrix(float width, float height);
    virtual void set_projection_matrix(int width, int height);

    virtual bool begin_scene() ;
    virtual void end_scene() ;
    virtual void clear(unsigned long color) ;

    virtual void render_line(Render_Line *line) {}
    virtual void render_triple(Render_Triple *triple) {}
    virtual void render_quad(Render_Quad *quad) ;

    virtual Render_Texture *add_texture(const std::string &name, const std::string &file) ;
    virtual void remove_texture(Render_Texture *tex) ;
    virtual void remove_texture(const std::string &name) ;
    virtual void remove_all_texture();

public:
    IDirect3D9 *d3d() const            { return d3d_; }
    IDirect3DDevice9 *device() const   { return device_; }
    void set_d3d(IDirect3D9 *d3d)      { d3d_ = d3d; }
    void set_device(IDirect3DDevice9 *device)  { device_ = device; }

    void initialise_buffer();
    void destroy_buffer();

    bool is_device_lost()               { return device_lost_; }      
    void restore_lost_device();
    void notify_device_lost ()          { device_lost_ = true; }

    void set_blend_mode(Render_Blend blend);
    void set_texture(Render_Texture *tex);

    void set_render_state();
    void set_texture_state();
    void set_sampler_state();

    Render_Window *create_render_window(const std::string &title);

private:
    IDirect3D9*	d3d_;
    IDirect3DDevice9* device_;
    IDirect3DVertexBuffer9 *vertex_buffer_;
    IDirect3DIndexBuffer9  *index_buffer_;

    D3DXMATRIX world_matrix_;
    D3DXMATRIX view_matrix_;
    D3DXMATRIX projection_matrix_;

    Render_Blend blend_mode_ ;  
    Render_Texture *render_texture_;
    Textures textures_;

    Config_Option_Map options_;

    bool device_lost_;
    Render_Window_D3d9 *primary_window_;

    bool zbuffer_;
    bool texture_filter_;
    bool hide_mouse_;
    bool deactivated_;
    Rect caption_;
};

/////////////////////////////////////////////////////////////////

//}
#endif