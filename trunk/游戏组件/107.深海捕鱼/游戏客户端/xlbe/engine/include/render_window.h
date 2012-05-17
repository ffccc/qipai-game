#ifndef __XLBE_RENDER_WINDOW_H__
#define __XLBE_RENDER_WINDOW_H__

#include <string>
#include <windows.h>

#include "prereqs.h"
#include "rect.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Render_Window 
{
public:
    Render_Window() {}
    virtual ~Render_Window() {}

public:
    virtual void create(const std::string& title, unsigned int width, unsigned int height, bool fullscreen) =0;
    virtual void destroy() =0;

    virtual bool visible() const = 0 ;
    virtual void set_visible(bool visible) =0;

    virtual bool active() const = 0;              
    virtual void set_active(bool active) = 0;

    virtual bool fullscreen() const =0;
    virtual void set_fullscreen(bool fullScreen) =0;

    virtual Size size() const = 0;                  
    virtual Point position() const =0;
    virtual void reposition(int left, int top) =0;

    virtual bool drag_caption() const = 0;
    virtual void set_drag_caption(bool drag) = 0;

    virtual void window_moved_or_resized() = 0;

    virtual HWND window_handle() const = 0;
};

///////////////////////////////////////////////////////////
//}

#endif

