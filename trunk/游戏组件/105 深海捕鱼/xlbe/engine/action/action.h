#ifndef __XLB_ACTION_H__
#define __XLB_ACTION_H__

#include "xlbe.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

class Scene_Node;

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action
{
public:
    Action();
    virtual ~Action();

public:
    int tag() const         { return tag_; }
    void set_tag(int tag)   { tag_ = tag; }

    Scene_Node *target() const            { return target_; }
    void set_target(Scene_Node *target)   { target_ = target; }

    Scene_Node *origin_target() const            { return origin_target_; }
    void set_origin_target(Scene_Node *target)   { origin_target_ = target;}

    virtual bool is_done() {return true; }
    virtual void stop() { target_ = 0; }

    virtual void step(float dt);
    virtual void update(float time);

    virtual void start_with_target(Scene_Node *target);

protected:
    int	tag_;
    Scene_Node *target_;
    Scene_Node *origin_target_;

};

///////////////////////////////////////////////////////////////////
class Action_Finit_Time : public Action
{
public:
    Action_Finit_Time();
    Action_Finit_Time(float d);
    virtual ~Action_Finit_Time();

public:
    virtual float duration() const { return duration_; }
    virtual void set_duration(float d) { duration_ = d; }

    Action_Finit_Time *reverse();

protected:
    float duration_;
};

///////////////////////////////////////////////////////////////////
//}
#endif