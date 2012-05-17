#ifndef __XLB_ACTION_COMBINE_H__
#define __XLB_ACTION_COMBINE_H__

#include <vector>

#include "prereqs.h"

#include "action.h"
#include "action_interval.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Sequence : public Action_Interval
{
private:
    class Sequence : public Action_Interval
    {
    public:
        Sequence(Action *act1, Action *act2);
        ~Sequence();

    public:
        virtual void update(float time);
        virtual void stop();

        virtual void start_with_target(Node *target);

    private:
        Action_Finit_Time *actions_[2];
        float split_;
        int last_;
    };

public:
    Action_Sequence(Action *act1, ...);
    virtual ~Action_Sequence();

public:
    virtual void update(float time);
    virtual void stop();

    virtual void start_with_target(Node *target);

private:
    Sequence *sequence_;

};

/////////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Spawn : public Action_Interval
{
public:
    Action_Spawn();
    Action_Spawn(Action *act1, ...);
    virtual ~Action_Spawn();

public:
    virtual void add_action(Action *act);
    virtual void remove_action(Action *act);

    virtual void update(float time);
    virtual void stop();

    virtual void start_with_target(Node *target);

private:
    std::vector<Action_Finit_Time *> actions_;
};

/////////////////////////////////////////////////////////////////////
//class Xlbe_Export Action_Repeat : public Action_Interval
//{
//public:
//    Action_Repeat(Action *a, int times);
//    virtual ~Action_Repeat();
//
//public:
//    virtual bool is_done() { return total_==times_; }
//    virtual void update(float time);
//    virtual void stop();
//
//    virtual void start_with_target(Node *target);
//
//private:
//    int times_;
//    int total_;
//    Action *other_;
//};
//
//class Xlbe_Export Action_Spawn : public Action_Interval
//{
//public:
//    Action_Spawn(Action *act1, ...);
//    virtual ~Action_Spawn();
//
//public:
//    virtual void update(float time);
//    virtual void stop();
//
//    virtual void start_with_target(Node *target);
//
//private:
//    std::vector<Action *> actions_;
//
//};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Repeat_Forever : public Action_Impl
{
public:
    Action_Repeat_Forever(Action *a);
    virtual ~Action_Repeat_Forever();

public:
    virtual bool is_done() { return stop_; }
    virtual void stop() { other_->stop(); stop_ = true; target_ = 0; }

    virtual void step(float dt);
    virtual void start_with_target(Node *target);

private:
    bool stop_;
    Action *other_;
};

///////////////////////////////////////////////////////////////////
//}
#endif