#ifndef __XLBE_ROOT_H__
#define __XLBE_ROOT_H__

#include "prereqs.h"

#include "timer.h"
#include "config_option.h"

#include "crit_sect.h"
#include "auto_crit.h"

#include "frame_listener.h"

#include "render_system.h"

#include "input_manager.h"
#include "sound_instance.h"
#include "sound_manager.h"
#include "font_manager.h"

#include "imageset_manager.h"
#include "animation_manager.h"

#include "director.h"
#include "imageset.h"
#include "animation_manager.h"
#include "action_manager.h"

#include "resource_manager.h"

//////////////////////////////////////////////////////////////
//namespace xlbe{

//////////////////////////////////////////////////////////////
class Xlbe_Export Root
{
public:
    typedef std::deque<unsigned long> Event_Times_Queue;
    typedef std::vector<Frame_Listener *> Frame_Listeners;
    typedef std::vector<Input_Listener *> Input_Listeners;
 
    enum Frame_Event_Time_Type 
    {
        FETT_ANY = 0, 
        FETT_STARTED = 1, 
        FETT_QUEUED = 2, 
        FETT_ENDED = 3, 
        FETT_FPS = 4,
        FETT_COUNT = 5
    };

private:
    Root();

    Root(const Root&);
    const Root& operator=(const Root&);

public:
    ~Root();
    static Root* instance();

    Config_Option_Map& config_options();
    void set_config_options(const std::string &name, const std::string &value) ;

    bool initialised() const { return initialised_; }
    void initialise(const std::string& title);
    void destroy();

    void start_rendering();
    void render_one_frame();

    int fps();
    void set_fps(int nfs);

    void queue_end_rendering();
    unsigned long get_next_frame_number(void) const { return next_frame_; }

    void add_frame_listener(Frame_Listener *listener);
    void remove_frame_listener(Frame_Listener *listener);

    void add_input_listener(Input_Listener *listener);
    void remove_input_listener(Input_Listener *listener);

    Timer *timer()                          { return timer_; }
    Render_System *render_system()          { return render_system_; }
    Resource_Manager *resource_manager()    { return resource_manager_; }
    Render_Window *render_window()          { return render_window_; }
    Director *scene_director()              { return scene_director_; }
    Action_Manager *action_manager()        { return action_manager_; }
    Imageset_Manager *imageset_manager()    { return imageset_manager_; }
    Animation_Manager *animation_manager()  { return animation_manager_; }

    Input_Manager *input_manager()          { return input_manager_; }
    Sound_Manager *sound_manager()          { return sound_manager_; }
	Sound_Manager *music_manager()          { return music_manager_; }
    Font_Manager *font_manager()            { return font_manager_; } 

private:
    void fire_frame_started();
    void fire_frame_render_targets();
    void fire_frame_ended();

    void populate_frame_event(Frame_Event_Time_Type type, Frame_Event& evttoupdate);
    unsigned long calculate_event_time(unsigned long now, Frame_Event_Time_Type type);

private:
    Timer *timer_;
    Render_System *render_system_;
    Resource_Manager *resource_manager_;
    Render_Window *render_window_;
    Director *scene_director_;
    Action_Manager *action_manager_; 
    Imageset_Manager *imageset_manager_;
    Animation_Manager *animation_manager_;
    Input_Manager *input_manager_; 
    Sound_Manager *sound_manager_;
	Sound_Manager *music_manager_;
    Font_Manager *font_manager_;

    Frame_Listeners frame_listeners_;
    Input_Listeners input_listeners_;

    Event_Times_Queue event_times[FETT_COUNT];

    bool initialised_;
    bool queue_end_;
    unsigned long next_frame_;
    unsigned long fixed_delta_;

    Config_Option_Map options_;

    static Root *instance_;

public:
    Crit_Sect sect_;

};


//////////////////////////////////////////////////////////////
//}
#endif 