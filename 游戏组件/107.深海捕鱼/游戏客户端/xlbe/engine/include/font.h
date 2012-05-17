#ifndef __XLBE_FONT_H__
#define __XLBE_FONT_H__

#include "prereqs.h"

#include "color.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Font 
{		
public:
    Font() { }
    virtual ~Font() { }

public:
    virtual std::string name() const = 0;

    virtual int height()  =0 ;
    virtual int string_width(const std::string &s)  =0;

    virtual void set_size(const Size &size) =0 ;
    virtual void set_color(const Color &color) =0 ;

    virtual int set_align(int align) = 0;

    virtual void draw_string(const Point &pt, const std::string &s, const Color &color) =0 ;

};

///////////////////////////////////////////////////////////
//}


#endif