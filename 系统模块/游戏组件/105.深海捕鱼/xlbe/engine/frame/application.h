#ifndef __XLBE_APPLICATION_H__
#define __XLBE_APPLICATION_H__

#include "xlbe.h"

#include "root.h"

//////////////////////////////////////////////////////////////
//namespace xlbe{

//////////////////////////////////////////////////////////////
#include "frame/root.h"

//////////////////////////////////////////////////////////////
class Xlbe_Export Application : public Resource_Listener
{
public:
    Application();
    virtual ~Application();

    virtual void exec();

public:
    Root *root() { return root_; }

    const char *cmd_line() const { return cmdline_.c_str(); }

    float loading_thread_progress();

protected:
    virtual void initialise();
    virtual void destroy();

    virtual bool loading_thread_proc();	
    virtual bool loading_thread_completed();	

protected:
    Root *root_;
    std::string title_;
    std::string cmdline_;
    int loading_thread_tasks_;
    int	complete_loading_thread_tasks_;

};

//////////////////////////////////////////////////////////////
//}
#endif 