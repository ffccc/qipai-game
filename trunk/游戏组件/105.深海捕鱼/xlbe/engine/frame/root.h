#ifndef __XLBE_ROOT_H__
#define __XLBE_ROOT_H__

#include <deque>
#include <vector>
#include <string>

#include "xlbe.h"

#include "utility/timer.h"

#include "frame_listener.h"
#include "input_manager.h"

#include "render/render_system.h"

#include "scene/scene_director.h"
#include "action/action_manager.h"

#include "resource/resource_listener.h"
#include "resource/resource_manager.h"

//////////////////////////////////////////////////////////////
//namespace xlbe{

#define RENDER_SYSTEM()     Root::instance()->render_system()
#define RESOURCE_MANAGER()  Root::instance()->resource_manager()
#define RENDER_WINDOW()     Root::instance()->render_window()
#define SCENE_DIRECTOR()    Root::instance()->scene_director()
#define ACTION_MANAGER()    Root::instance()->action_manager()
#define INPUT_MANAGER()     Root::instance()->input_manager()
//////////////////////////////////////////////////////////////
class Xlbe_Export Root
{
public:
    typedef std::deque<unsigned long> Event_Times_Queue;
    typedef std::vector<Frame_Listener *> Frame_Listeners;
    typedef std::vector<Resource_Listener *> Resource_Listeners;

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
    Root(const std::string& config = "xlbe.cfg");

    Root(const Root&);
    const Root& operator=(const Root&);

public:
    ~Root();
    static Root* instance();

    bool initialised() const { return initialised_; }
    void initialise(const std::string& windowtitle);

    void save_config();
    void restore_config();

    void start_rendering();
    void render_one_frame();

    unsigned long fps()                { return 1000.0/fixed_delta_; }
    void set_fps(unsigned long nfs  )  { fixed_delta_ = 1000.0/nfs; }

    void queue_end_rendering();
    unsigned long get_next_frame_number(void) const { return next_frame_; }

    void add_frame_listener(Frame_Listener *listener);
    void remove_frame_listener(Frame_Listener *listener);

    void add_resource_listener(Resource_Listener *listener);
    void remove_resource_listener(Resource_Listener *listener);

    void loading_thread_proc();
    void loading_thread_completed();
    static void loading_thread_proc_stub(void *arg);

    void start_loading_thread();
    void wait_for_loading_thread();

    Timer *timer()                          { return timer_; }
    Render_System *render_system()          { return render_system_; }
    Resource_Manager *resource_manager()    { return resource_manager_; }
    Render_Window *render_window()          { return render_window_; }
    Scene_Director *scene_director()        { return scene_director_; }
    Action_Manager *action_manager()        { return action_manager_; }
    Input_Manager *input_manager()          { return input_manager_; }

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
    Scene_Director *scene_director_;
    Action_Manager *action_manager_; 
    Input_Manager *input_manager_; 

    Frame_Listeners frame_listeners_;
    Resource_Listeners resource_listeners_;

    Event_Times_Queue event_times[FETT_COUNT];

    bool initialised_;
    bool queue_end_;
    unsigned long next_frame_;
    unsigned long fixed_delta_;

    bool loading_thread_started_ ;
    bool loading_thread_completed_;

    std::string config_;

    static Root *instance_;
};

//////////////////////////////////////////////////////////////
//}
#endif 