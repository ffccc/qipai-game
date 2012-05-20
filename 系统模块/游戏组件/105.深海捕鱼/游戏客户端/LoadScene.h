#ifndef __LOAD_SCENE_H__
#define __LOAD_SCENE_H__

//////////////////////////////////////////////////////////////////
#include "scene/scene.h"
#include "scene/scene_layer.h"
#include "scene/sprite.h"
#include "action/action_interval.h"
#include "action/action_combine.h"
#include "action/action_instant.h"
#include "frame/root.h"

/////////////////////////////////////////////////////////////////
class CLoadScene : public Scene
{
public:
    CLoadScene();
    virtual ~CLoadScene();

public:
    virtual void enter() ;
    virtual void update(float dt);

private:
    Sprite *spProgress;
   
};

//////////////////////////////////////////////////////////////////////////
#endif