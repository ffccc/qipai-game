#include "LoadScene.h"
#include "FishScene.h"
#include "FishApplication.h"

CFishApplication *pApp = 0;

////////////////////////////////////////////////////////
CFishApplication::CFishApplication(const std::string &title, const std::string &cmdline)
{
    title_ = title;
    cmdline_ = cmdline;

    pApp = this;
}

void CFishApplication::initialise()
{
    __super::initialise();

    RESOURCE_MANAGER()->prase_resources_file("properties\\resources.xml");
    RESOURCE_MANAGER()->load_resources("Load");

    loading_thread_tasks_ = RESOURCE_MANAGER()->number_resources("Game");

    CLoadScene *pLoadScene = new CLoadScene;
    root_->scene_director()->push_scene(pLoadScene);
}

void CFishApplication::destroy()
{
}

bool CFishApplication::loading_thread_proc()
{
    RESOURCE_MANAGER()->start_load_resources("Game");
    

    while (RESOURCE_MANAGER()->load_next_resource())
    {
        complete_loading_thread_tasks_++;
    }

    return true;
}

bool CFishApplication::loading_thread_completed()
{
    CFishScene *pFishScene = new CFishScene;
    root_->scene_director()->replace_scene(pFishScene);

    RESOURCE_MANAGER()->remove_resources("Load");

    return true;
}