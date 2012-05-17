#ifndef __XLB_ACTION_INTERVAL_H__
#define __XLB_ACTION_INTERVAL_H__

#include "prereqs.h"

#include "action_impl.h"
#include "animation.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Interval : public Action_Finit_Time
{
public:
    Action_Interval(float d);
    virtual ~Action_Interval();

public:
    virtual bool is_done() { return ((elapsed_>=duration_)||stop_); }
    virtual void stop() { stop_ = true; target_ = 0; }

    virtual void step(float dt);
    virtual void update(float time);

    virtual float speed() const { return speed_; }
    virtual void set_speed(float speed) { speed_ = speed; }

    virtual void start_with_target(Node *target);

protected:
    float elapsed_;
    bool stop_;
    float speed_;

};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Delay : public Action_Interval
{
public:
    Action_Delay(float d);
    virtual ~Action_Delay();
};

////////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Move_To : public Action_Interval
{
public:
    Action_Move_To(float d, Point end);
    virtual ~Action_Move_To();

public:
    virtual void update(float time);
    virtual void start_with_target(Node *target);

protected:
    Point start_;
    Point end_;
    Point delta_;
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Move_By : public Action_Move_To
{
public:
    Action_Move_By(float d, Point delta);
    virtual ~Action_Move_By();

public:
    virtual void start_with_target(Node *target);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Bezier_By : public Action_Interval
{
public:
    Action_Bezier_By(float d, const Point &c1, const Point &c2, const Point &end);
    virtual ~Action_Bezier_By();

public:
    virtual void update(float time);
    virtual void start_with_target(Node *target);

private:
    Point start_;
    Point end_;
    Point control1_;
    Point control2_;
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Rorate_To : public Action_Interval
{
public:
    Action_Rorate_To(float d, float end);
    virtual ~Action_Rorate_To();

public:
    virtual void update(float time);
    virtual void start_with_target(Node *target);

private:
    float start_angle_;
    float end_angle_;
    float diff_angle_;
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Rorate_By : public Action_Interval
{
public:
    Action_Rorate_By(float d, float angle);
    virtual ~Action_Rorate_By();

public:
    virtual void update(float time);
    virtual void start_with_target(Node *target);

private:
    float angle_;
    float start_angle_;
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Fade_Out : public Action_Interval
{
public:
    Action_Fade_Out(float d);
    virtual ~Action_Fade_Out();

public:
    virtual void update(float time);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Fade_In : public Action_Interval
{
public:
    Action_Fade_In(float d);
    virtual ~Action_Fade_In();

public:
    virtual void update(float time);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Scale_To : public Action_Interval
{
public:
    Action_Scale_To(float d, float scale);
    Action_Scale_To(float d, float scalex, float scaley);
    virtual ~Action_Scale_To();

public:
    virtual void update(float time);
    virtual void start_with_target(Node *target);

protected:
    float start_scale_x_;
    float start_scale_y_;
    float end_scale_x_;
    float end_scale_y_;
    float delta_x_;
    float delta_y_;
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Scale_By : public Action_Scale_To
{
public:
    Action_Scale_By(float d, float scale_x, float scale_y);
    virtual ~Action_Scale_By();

public:
    virtual void start_with_target(Node *target);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Animation : public Action_Interval
{
public:
    Action_Animation(float d, Animation *animation, bool origin);
    virtual ~Action_Animation();

public:
    virtual void stop() ;
    virtual void update(float time);
    virtual void start_with_target(Node *target);

private:
    bool origin_;
    Image *origin_image_;
    Animation *animation_;
};

///////////////////////////////////////////////////////////////////
//}
#endif