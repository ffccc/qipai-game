#ifndef __XLBE_BOUNDBOX_H__
#define  __XLBE_BOUNDBOX_H__

#include "vector2d.h"

//////////////////////////////////////////////////////////////
//namespace xlbe{

//////////////////////////////////////////////////////////////
struct Boundbox
{
    Boundbox():lower_(0.0f,0.0f), upper_(0.0f,0.0f) {}
    Boundbox(float x1, float y1, float x2, float y2):lower_(x1,y1), upper_(x2,y2) {}
    Boundbox(const Vector2D &a, const Vector2D &b):lower_(a), upper_(b) {}
    Boundbox(const Boundbox& a):lower_(a.lower_), upper_(a.upper_) {}

    void combine(const Boundbox& aabb)
    {
        lower_.x_ = ((lower_.x_<aabb.lower_.x_)?lower_.x_:aabb.lower_.x_);
        lower_.y_ = ((lower_.y_<aabb.lower_.y_)?lower_.y_:aabb.lower_.y_);
        upper_.x_ = ((upper_.x_>aabb.upper_.x_)?upper_.x_:aabb.upper_.x_);
        upper_.y_ = ((upper_.y_>aabb.upper_.y_)?upper_.y_:aabb.upper_.y_);
    }

    bool contains(const Boundbox& aabb) const
    {
        bool result = true;
        result = result && lower_.x_ <= aabb.lower_.x_;
        result = result && lower_.y_ <= aabb.lower_.y_;
        result = result && aabb.upper_.x_ <= upper_.x_;
        result = result && aabb.upper_.y_ <= upper_.y_;
        return result;
    }
    
    bool overlap(const Boundbox& a)
    {
        Vector2D d1(a.lower_), d2(lower_);
        d1 -= upper_;
        d2 -= a.upper_;
        
        if (d1.x_  > 0.0f || d1.y_  > 0.0f)
            return false;
        
        if (d2.x_  > 0.0f || d2.y_  > 0.0f)
            return false;
        
        return true;
    }
    
    Vector2D lower_;	
    Vector2D upper_;	
};

//////////////////////////////////////////////////////////////
//}
#endif 