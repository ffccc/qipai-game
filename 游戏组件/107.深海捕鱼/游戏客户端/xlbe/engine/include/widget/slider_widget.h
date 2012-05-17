#ifndef __XLBE_SLIDER_WIDGET_H__
#define __XLBE_SLIDER_WIDGET_H__

#include "prereqs.h"

#include "imageset.h"
#include "layer.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

class Xlbe_Export Slider_Listener 
{
public:
    virtual void slider_value(int tag, double value)=0;
};

///////////////////////////////////////////////////////////
class Xlbe_Export Slider_Widget : public Layer
{		
public:
    typedef std::vector<Slider_Listener *> Listeners;

public:
    Slider_Widget(Image* track_Image, Image *thumb_image, Image *slider_image);
    virtual ~Slider_Widget();

public:
    virtual void draw();

public:
    virtual void mouse_enter();
    virtual void mouse_leave();

    virtual void mouse_down(const Point &pt, int num, int count);
    virtual void mouse_up(const Point &pt, int num, int count);

    virtual void mouse_drag(const Point &pt);

public:
	virtual double value() const { return  value_; }
    virtual void set_value(double value);

    void add_listener(Slider_Listener *listener);
    void remove_listener(Slider_Listener *listener);

private:
    bool dragging_;
    double value_;
    int rel_x_;
    
    Image* track_image_;
    Image* thumb_image_;
    Image* slider_image_;

    Listeners listeners_; 
};

///////////////////////////////////////////////////////////
//}


#endif