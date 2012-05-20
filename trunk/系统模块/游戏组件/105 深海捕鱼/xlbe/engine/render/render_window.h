#ifndef __XLBE_RENDER_WINDOW_H__
#define __XLBE_RENDER_WINDOW_H__

#include <string>
#include <windows.h>

#include "xlbe.h"
#include "utility/rect.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Render_Window 
{		
public:
    Render_Window();
    ~Render_Window();

    void create(const std::string& titlename);
    void destroy() ;

    HWND window() const  { return hwnd_; }

    bool closed() const  { return closed_; }
    bool visible() const { return (hwnd_ && !IsIconic(hwnd_)); }

    bool active() const  { if (!windowed_) return visible(); return active_ && visible();}                 
    void set_active(bool active) { active_ = active; }

    bool windowed() const { return windowed_; }
    void set_windowed(bool windowed);

    Size window_size() const;                  
    void set_window_size(int width, int height);

    Point window_position() const;
    void set_window_position(int left, int top);

    Size fullscreen_size() const;                  
    void set_fullscreen_size(int width, int height);

    bool hidemouse() const              { return hidemouse_; }
    void set_hidemouse(bool hide)       { if (hidemouse_ != hide) ShowCursor(hide); hidemouse_ = hide;  }  

    bool deactivated_on_focus_change() const               { return deactivated_; }
    void set_deactivated_on_focus_change(bool deactivated) { deactivated_ = deactivated; }

public:
    void display_changed();
    void window_moved_or_resized();

private:
    bool active_;
    bool closed_; 

    bool windowed_;
    bool hidemouse_;
    bool deactivated_;

    RECT rc_window_;
    RECT rc_fullscreen_;

    DWORD style_window_;
    DWORD style_fullscreen_;

    HWND hwnd_;
    HINSTANCE hinst_;
};

///////////////////////////////////////////////////////////
//}

#endif

