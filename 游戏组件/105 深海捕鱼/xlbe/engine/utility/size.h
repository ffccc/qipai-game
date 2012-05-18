#ifndef __XLB_SIZE_H__
#define __XLB_SIZE_H__

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Size 
{
public:
    Size():width_(0), height_(0)  { }
    Size(float width, float height):width_(width), height_(height) {}
    Size(const Size &size): width_(size.width_), height_(size.height_) {}
    ~Size() {}

    bool operator == (const Size &size) const { return (size.width_==width_&&size.height_==height_); }
    bool operator != (const Size &size) const { return (size.width_!=width_||size.height_!=height_); }

    Size &operator = (const Size &size) { width_=size.width_; height_=size.height_; return *this; }

    Size& operator += (const Size &size) { width_+=size.width_; height_+=size.height_; }
    Size& operator -= (const Size &size) { width_-=size.width_; height_-=size.height_; }

    void set_size(float width, float height) { width_=width; height_=height; }

    Size operator + (const Size &size) { return Size(width_+size.width_, height_+size.height_); }
    Size operator - (const Size &size) { return Size(width_-size.width_, height_-size.height_); }
    Size operator - () { return Size(-width_, -height_); }

public:
    float width_;
    float height_;
};

///////////////////////////////////////////////////////////////////
//}
#endif
