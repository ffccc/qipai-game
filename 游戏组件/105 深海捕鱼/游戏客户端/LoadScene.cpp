#include "FishApplication.h"
#include "LoadScene.h"

/////////////////////////////////////////////////////////////
CLoadScene::CLoadScene()
{
    spProgress = new Sprite(RESOURCE_MANAGER()->sprite_frame("Progress"));
    spProgress->set_position(Point(16,590));
    
    add_child(spProgress);
}

CLoadScene::~CLoadScene()
{
}

void CLoadScene::enter()
{
    __super::enter();
}

void CLoadScene::update(float dt)
{
    __super::update(dt);

    float x = 1024*pApp->loading_thread_progress();

    spProgress->set_position(Point(x/2.0,590));
    spProgress->set_content_size(Size(x,32));

}
