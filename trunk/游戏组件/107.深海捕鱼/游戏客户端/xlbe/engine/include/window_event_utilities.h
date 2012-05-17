#ifndef __XLBE_WINDOW_EVENT_UTILITIES_H__
#define __XLBE_WINDOW_EVENT_UTILITIES_H__

#include <windows.h>

#include <vector>

#include "prereqs.h"
#include "render_window.h"
/////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////
class Window_Event_Listener
{
public:
    virtual void window_moved(Render_Window* rw)=0;
    virtual void window_focus_change(Render_Window* rw)=0;
    virtual void window_resized(Render_Window* rw)=0;
    virtual void window_closed(Render_Window* rw)=0;
};

class Xlbe_Export Window_Event_Utilities
{
public:
    typedef std::vector<Window_Event_Listener *> Event_Listeners;
    typedef std::vector<Render_Window *> Render_Windows;

public:
    static void add_event_listener(Window_Event_Listener *listener);
    static void remove_event_listener(Window_Event_Listener *listener);

    static void add_render_window(Render_Window *window);
    static void remove_render_window(Render_Window *window);

    static void message_pump();

    static LRESULT CALLBACK _WndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam);

    static Event_Listeners event_listeners_;
    static Render_Window  *render_window_;

    static Point drag_caption_delta_;
};


/////////////////////////////////////////////////////////////////////////////////////
//}
#endif