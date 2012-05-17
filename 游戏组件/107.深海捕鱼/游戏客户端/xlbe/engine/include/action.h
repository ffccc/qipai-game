#ifndef __XLB_ACTION_H__
#define __XLB_ACTION_H__

#include "prereqs.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

class Node;

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action
{
public:
    Action() {}
    virtual ~Action() {}

public:
    virtual int tag() const = 0;
    virtual void set_tag(int tag) = 0;

    virtual bool is_done()  = 0;
    virtual void stop()  = 0;

    virtual void step(float dt) = 0;
    virtual void update(float dt) = 0;

    virtual float speed() const = 0;
    virtual void set_speed(float speed) = 0;

    virtual Node *target() const = 0;
    virtual void start_with_target(Node *target) = 0;

    //virtual Action *clone() const = 0;
    //virtual Action *reverse() const = 0;
};

///////////////////////////////////////////////////////////////////
//}
#endif