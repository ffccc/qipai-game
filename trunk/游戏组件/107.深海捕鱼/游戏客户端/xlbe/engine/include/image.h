#ifndef __XLBE_IMAGE_H__
#define __XLBE_IMAGE_H__

#include "prereqs.h"

#include "rect.h"
#include "color_rect.h"
#include "render_primitive.h"

//////////////////////////////////////////////////////////////
//namespace xlbe {

class Imageset;

//////////////////////////////////////////////////////////////
class Xlbe_Export Image
{
public:
    Image();
    Image(Imageset *owner, const std::string &name, const Rect &area);
    Image(const Image &image);
    ~Image();

public:
    std::string name() const               { return name_; }
    void set_name(const std::string &name) { name_ = name; } 

    std::string imageset_name() const ;
    const Imageset*	imageset(void) const ;

    Rect area() const                { return area_; }
    void set_area(const Rect &area)  { area_ = area; }

    float width() const { return area_.width(); }
    float height() const { return area_.height(); }

    Size texture_size() const ;

    void draw(const Point &pos);
    void draw(const Point &pos, const Color_Rect& colors, Render_Blend blend);
    void draw(const Point &pos, const Size &size);
    void draw(const Point &pos, const Size &size, const Color_Rect& colors, Render_Blend blend);
    void draw(const Point &pos, const Size &size, const Point &src_pos, const Size &src_size);
    void draw(const Point &pos, const Size &size, const Point &src_pos, const Size &src_size, const Color_Rect& colors, Render_Blend blend);

    void draw(const Point dest[4], const Point src[4], const Color_Rect& colors, Render_Blend blend);

public:
    std::string name_; 
    Rect area_;
    Imageset *owner_;

};

//////////////////////////////////////////////////////////////
//}
#endif