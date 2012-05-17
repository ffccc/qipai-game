#ifndef __XLB_COLOR_H__
#define __XLB_COLOR_H__

#include "prereqs.h"
///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Xlbe_Export Color 
{
public:
    Color();
    Color(int color);
    Color(int color, int alpha);
    Color(int red, int green, int blue);
    Color(int red, int green, int blue, int alpha);

public:
    int red() const;
    int green() const;
    int blue() const;
    int alpha() const;

    unsigned long toint() const;

    int& operator[] (int index);
    int operator[] (int index) const;	

public:
    int red_;
    int green_;
    int blue_;
    int alpha_;
};

//////////////////////////////////////////////////////////////////
bool operator== (const Color& color1, const Color& color2);
bool operator!= (const Color& color1, const Color& color2);

///////////////////////////////////////////////////////////////////
//}
#endif
