#ifndef __XLBE_RENDER_WINDOW_D3D9_H__
#define __XLBE_RENDER_WINDOW_D3D9_H__

#include <string>
#include <windows.h>

#include "prereqs.h"
#include "rect.h"

#include "render_window.h"
#include "render_window_d3d9.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

class Render_System_D3d9;

///////////////////////////////////////////////////////////
class Xlbe_Export Render_Window_D3d9 : public Render_Window
{		
public:
    Render_Window_D3d9(HINSTANCE hinst, Render_System_D3d9 *rs) ;
    virtual ~Render_Window_D3d9(); 

    virtual void create(const std::string& title, unsigned int width, unsigned int height, bool fullscreen);
    virtual void destroy();

    virtual bool visible() const  { return (hwnd_ /*&& !IsIconic(hwnd_)*/); }
    virtual void set_visible(bool visible) { }

    virtual bool active() const  { if (fullscreen_) return visible(); return active_ && visible(); }                 
    virtual void set_active(bool active) { active_ = active; }

    virtual bool fullscreen() const { return fullscreen_; }
    virtual void set_fullscreen(bool fullScreen) ;

    virtual Size size() const { return Size(width_, height_); }             
    virtual Point position() const ;
    virtual void reposition(int left, int top) ;

    virtual bool drag_caption() const  { return drag_ ; }
    virtual void set_drag_caption(bool drag)  { drag_ = drag; }

    virtual void window_moved_or_resized();

    virtual HWND window_handle() const { return hwnd_; }

public:

    D3DPRESENT_PARAMETERS *presentation_parameters()  { return &d3dpp_; }

    void create_d3d_resources();
    void destroy_d3d_resources();

private:
    bool active_;
    bool fullscreen_;
    unsigned int width_;
    unsigned int height_;

    bool drag_;

    HWND hwnd_;
    HINSTANCE hinst_;

    Render_System_D3d9 *render_system_;
    D3DPRESENT_PARAMETERS d3dpp_;
};

///////////////////////////////////////////////////////////
//}

#endif