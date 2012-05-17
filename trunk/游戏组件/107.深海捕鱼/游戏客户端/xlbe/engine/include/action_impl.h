#ifndef __XLB_ACTION_IMPL_H__
#define __XLB_ACTION_IMPL_H__

#include "prereqs.h"

#include "node.h"
#include "action.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Impl : public Action
{
public:
    Action_Impl():tag_(-1), target_(0) {}
    virtual ~Action_Impl() {}

public:
    virtual int tag() const         { return tag_; }
    virtual void set_tag(int tag)   { tag_ = tag; }

    virtual bool is_done() { return true; }
    virtual void stop() { target_ = 0; }

    virtual void step(float dt) {}
    virtual void update(float time) {}

    virtual float speed() const  { return 1.0f; }
    virtual void set_speed(float speed) { }

    virtual Node *target() const { return target_; }
    virtual void start_with_target(Node *target) { target_  = target; } 

protected:
    int	tag_;
    Node *target_;
};

///////////////////////////////////////////////////////////////////
class Action_Finit_Time : public Action_Impl
{
public:
    Action_Finit_Time(float d):duration_(d) {}
    virtual ~Action_Finit_Time() {}

public:
    virtual float duration() const { return duration_; }
    virtual void set_duration(float d) { duration_ = d; }

protected:
    float duration_;
};

///////////////////////////////////////////////////////////////////
class Action_Follow : public Action_Impl
{
public:
    Action_Follow(Node *node, const Point &offset):followed_node_(node), offset_(offset) {}
    virtual ~Action_Follow() {}

public:
    virtual bool is_done() { return !followed_node_->running(); }
    virtual void step(float dt) { target_->set_position(followed_node_->position()+offset_); }

protected:
    Point offset_;
    Node *followed_node_;
};

///////////////////////////////////////////////////////////////////
class Action_Down : public Action_Impl
{
public:
    Action_Down(float speed):speed_(speed) {}
    virtual ~Action_Down() {}

public:
    virtual bool is_done() { return false; }
    virtual void step(float dt) 
    {
        Point pt = target_->position();
        pt.y_ += speed_*dt;

        target_->set_position(pt);
    }

protected:
    float speed_;
};

///////////////////////////////////////////////////////////////////
//}
#endif