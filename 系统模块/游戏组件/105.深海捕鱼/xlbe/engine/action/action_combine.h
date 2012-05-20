#ifndef __XLB_ACTION_COMBINE_H__
#define __XLB_ACTION_COMBINE_H__

#include <vector>

#include "xlbe.h"

#include "action.h"
#include "action_interval.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Sequence : public Action_Interval
{
public:
    Action_Sequence(Action_Finit_Time *act1, ...);
    virtual ~Action_Sequence();

public:
    virtual void update(float time);
    virtual void stop();

    virtual void start_with_target(Scene_Node *target);

private:
    float split_;
    int index_;
    std::vector<Action_Finit_Time *> actions_;
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Repeat : public Action_Interval
{
public:
    Action_Repeat(Action_Finit_Time *a, int times);
    virtual ~Action_Repeat();

public:
    virtual bool is_done() { return total_==times_; }
    virtual void update(float time);
    virtual void stop();

    virtual void start_with_target(Scene_Node *target);

private:
    int times_;
    int total_;
    Action_Finit_Time *other_;
};

class Xlbe_Export Action_Spawn : public Action_Interval
{
public:
    Action_Spawn(Action_Finit_Time *act1, ...);
    virtual ~Action_Spawn();

public:
    virtual void update(float time);
    virtual void stop();

    virtual void start_with_target(Scene_Node *target);

private:
    std::vector<Action_Finit_Time *> actions_;

};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Repeat_Forever : public Action
{
public:
    Action_Repeat_Forever(Action_Interval *a);
    virtual ~Action_Repeat_Forever();

public:
    virtual bool is_done() { return false; }
    virtual void step(float dt);
    virtual void start_with_target(Scene_Node *target);

private:
    Action_Interval *other_;
};

///////////////////////////////////////////////////////////////////
//}
#endif