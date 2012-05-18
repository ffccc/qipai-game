#ifndef __FISH_APPLICATION_H__
#define __FISH_APPLICATION_H__

#include "FishScene.h"
#include "frame/application.h"

///////////////////////////////////////////////////////////
class CFishApplication : public Application 
{
public:
    CFishApplication(const std::string &title, const std::string &cmdline);

protected:
    virtual void initialise();
    virtual void destroy();

    virtual bool loading_thread_proc();	
    virtual bool loading_thread_completed();

};

extern CFishApplication *pApp;

///////////////////////////////////////////////////////////////////////////////////
#endif
