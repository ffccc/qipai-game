#ifndef __XLBE_DIRECTOR_H__
#define __XLBE_DIRECTOR_H__

#include <stack>

#include "prereqs.h"
///////////////////////////////////////////////////////////
//namespace xlbe{

class Scene;

///////////////////////////////////////////////////////////
class Xlbe_Export Director 
{		
public:
    typedef std::stack<Scene*> Scenes;

public:
    Director();
    ~Director();

public:
    void push_scene(Scene *scene);
    void pop_scene();

    void replace_scene(Scene *scene);

    void draw_scene(float dt);

    void pause();
    void resume();

private:
    void set_next_scene();

private:
    bool paused_;
    bool new_push_;
    bool runing_destroy_;  

    Scene *running_scene_;
    Scene *next_scene_;

    Scenes scenes_;
};

///////////////////////////////////////////////////////////
//}


#endif