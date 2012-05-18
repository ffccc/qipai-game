#ifndef __XLBE_FRAME_LISTENER_H__
#define __XLBE_FRAME_LISTENER_H__

////////////////////////////////////////////////////////////
//namespace xlbe{

////////////////////////////////////////////////////////////
struct Frame_Event
{
    unsigned long time_since_last_event;
    unsigned long time_since_last_frame;
};

class Frame_Listener
{
public:
    virtual bool frame_started(const Frame_Event& evt)=0;	
    virtual bool frame_render_targets(const Frame_Event& evt)=0;
    virtual bool frame_ended(const Frame_Event& evt)=0;		
};

///////////////////////////////////////////////////////////
//}
#endif