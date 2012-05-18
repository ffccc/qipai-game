#ifndef __XLBE_SCENE_DIRECTOR_H__
#define __XLBE_SCENE_DIRECTOR_H__

#include <stack>

#include "xlbe.h"
#include "scene.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Scene_Director 
{		
public:
    typedef std::stack<Scene*> Scenes;

public:
    Scene_Director();
    ~Scene_Director();

public:
    void push_scene(Scene *scene);
    void pop_scene();

    void replace_scene(Scene *scene);

    void draw_scene(unsigned long tv);

    void pause();
    void resume();

private:
    void set_next_scene();

private:
    bool paused_;
    bool new_transition_;
    bool runing_transition_;  

    Scene *running_scene_;
    Scene *next_scene_;

    Scenes scenes_;
};

///////////////////////////////////////////////////////////
//}


#endif