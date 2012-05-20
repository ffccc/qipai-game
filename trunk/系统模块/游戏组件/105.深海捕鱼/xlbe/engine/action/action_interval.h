#ifndef __XLB_ACTION_INTERVALH__
#define __XLB_ACTION_INTERVALH__

#include "xlbe.h"

#include "action.h"
#include "scene/animation.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Interval : public Action_Finit_Time
{
public:
    Action_Interval(float d);
    virtual ~Action_Interval();

public:
    virtual bool is_done() { return (elapsed_ >= duration_); }
    virtual void step(float dt);
    virtual void start_with_target(Scene_Node *target);

    virtual float elapsed() const { return elapsed_; }

protected:
    float elapsed_;
    bool	first_tick_;
};

////////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Move_To : public Action_Interval
{
public:
    Action_Move_To(float d, Point end);
    virtual ~Action_Move_To();

public:
    virtual void update(float time);
    virtual void start_with_target(Scene_Node *target);

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
    virtual void start_with_target(Scene_Node *target);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Bezier_By : public Action_Interval
{
public:
    Action_Bezier_By(float d, const Point &c1, const Point &c2, const Point &end);
    virtual ~Action_Bezier_By();

public:
    virtual void update(float time);
    virtual void start_with_target(Scene_Node *target);

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
    virtual void start_with_target(Scene_Node *target);

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
    virtual void start_with_target(Scene_Node *target);

private:
    float angle_;
    float start_angle_;
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
    virtual void start_with_target(Scene_Node *target);

protected:
    float scale_x_;
    float scale_y_;
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
    virtual void start_with_target(Scene_Node *target);
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Animation : public Action_Interval
{
public:
    Action_Animation(float d, Animation *animation, bool origin);
    virtual ~Action_Animation();

public:
    virtual void update(float time);
    virtual void stop();
    virtual void start_with_target(Scene_Node *target);

private:
    bool origin_;
    Sprite_Frame *origin_frame_;
    Animation *animation_;
};

///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
//}
#endif