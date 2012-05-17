#ifndef __XLBE_APPLICATION_H__
#define __XLBE_APPLICATION_H__

#include "prereqs.h"

#include "root.h"


//////////////////////////////////////////////////////////////
//namespace xlbe{

//////////////////////////////////////////////////////////////
class Xlbe_Export Application 
{
public:
    Application(const std::string &title, const std::string &config, const std::string &cmdline);
    virtual ~Application();

    virtual void exec();

public:
    virtual void initialise()=0;
    virtual void destroy()=0;

    virtual void save_config();
    virtual void restore_config();

    float loading_thread_progress();
    virtual void loading_thread_proc() { };
    //void loading_thread_completed()=0;
    static void loading_thread_proc_stub(void *arg);

    void start_loading_thread();
    void stop_loading_thread();
    void wait_for_loading_thread();

    Root *root() { return root_; }
    const char *title() const { return title_.c_str(); }
    const char *config() const { return config_.c_str(); }
    const char *cmd_line() const { return cmdline_.c_str(); }

protected:
    Root *root_;

    std::string title_;
    std::string config_;
    std::string cmdline_;   
				
    HANDLE thread_handle_;
    volatile int loading_thread_tasks_;
    volatile int complete_loading_thread_tasks_;
    volatile bool loading_thread_started_ ;
    volatile bool loading_thread_completed_;			
};

//////////////////////////////////////////////////////////////
//}
#endif 