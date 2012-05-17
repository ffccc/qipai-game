#ifndef __XLB_ACTION_INSTANT_H__
#define __XLB_ACTION_INSTANT_H__

#include "prereqs.h"

#include "functional_fun.h"
#include "action.h"
#include "action_interval.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Instant : public Action_Finit_Time
{
public:
    Action_Instant():Action_Finit_Time(0){}
    virtual ~Action_Instant() {}

public:
    virtual void step(float dt) { update(1.0f); }
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Show : public Action_Instant
{
public:
    virtual void start_with_target(Node *target);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Hide : public Action_Instant
{
public:
    virtual void start_with_target(Node *target);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Toggle_Visibility : public Action_Instant
{
public:
    virtual void start_with_target(Node *target);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Place : public Action_Instant
{
public:
    Action_Place(Point p):position_(p) {}
    virtual void start_with_target(Node *target);

private:
    Point position_;
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Func : public Action_Instant
{
    typedef rak::function_base2<bool, Node*, int> *Slot_Func;

public:
    Action_Func(Slot_Func slot):slot_func_(slot) {}
    virtual void start_with_target(Node *target);

private:
    Slot_Func slot_func_;
};

///////////////////////////////////////////////////////////////////
//}
#endif