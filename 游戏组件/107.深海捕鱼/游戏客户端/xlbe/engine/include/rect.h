#ifndef __XLBE_RECT_H__
#define __XLBE_RECT_H__

#include "size.h"
#include "point.h"

//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
class Rect 
{
public:
    Rect():origin_(0,0), size_(0,0) {}
    Rect(float x, float y, float width, float height):origin_(x,y), size_(width,height) {}
    Rect(const Point &point, const Size &size):origin_(point), size_(size) {}
    Rect(const Rect& rect):origin_(rect.origin_), size_(rect.size_) {}

public:
    float width() const { return size_.width_; }
    float height() const { return size_.height_; }

    Size size() const { return size_; }

    Point origin_point() const { return origin_; }
    Point center_point() const { return Point(origin_.x_+size_.width_/2.0, origin_.y_+size_.height_/2.0); }

    bool empty() const { return (size_.width_==0&&size_.height_==0); }

    bool equal_rect(const Rect &r) const { return (origin_==r.origin_&&size_==r.size_); }
    bool pt_in_rect(const Point &point) const { return (point.x_>origin_.x_ && point.y_>origin_.y_ && point.x_<size_.width_+origin_.x_ && point.y_<size_.height_+origin_.y_); }

    void set_rect(float x, float y, float width, float height) { origin_.set_point(x,y); size_.set_size(width,height); }
    void set_rect(const Point &point, const Size &size) { origin_=point; size_=size; }
    void set_rect_empty() { origin_.set_point(0,0); size_.set_size(0,0); }

    void inflate_rect(float width, float height) { size_.width_+=width; size_.height_+=height; }
    void inflate_rect(const Size &size) { size_+=size; }

    void deflate_rect(float width, float height) { size_.width_-=width; size_.height_-=height; }
    void deflate_rect(const Size &size) { size_-=size; }

    void offset_rect(float x, float y) { origin_.x_+=x; origin_.y_+=y; }
    void offset_rect(const Size &size) { origin_.x_+=size.width_; origin_.y_+=size.height_; }

    bool intersect_rect(const Rect& l, const Rect& r) {}
    bool union_rect(const Rect& l, const Rect& r) {}
    bool subtract_rect(const Rect& l, const Rect& r) {}

    bool collision(const Rect &r) const 
    {
        if (r.origin_.y_ + r.size_.height_ < origin_.y_ ||
            r.origin_.y_ > origin_.y_ + size_.height_ ||
            r.origin_.x_ + r.size_.width_ < origin_.x_ || 
            r.origin_.x_ > origin_.x_ + size_.width_)
            return false;

        return true;

    }

    Rect& operator = (const Rect& r)  { origin_=r.origin_; size_=r.size_; return *this; }

    bool operator == (const Rect& r) const { return (origin_==r.origin_&&size_==r.size_); }
    bool operator != (const Rect& r) const { return (origin_!=r.origin_||size_!=r.size_); }

public:
    Point origin_;
    Size  size_;
};

//////////////////////////////////////////////////////////////
//}
#endif