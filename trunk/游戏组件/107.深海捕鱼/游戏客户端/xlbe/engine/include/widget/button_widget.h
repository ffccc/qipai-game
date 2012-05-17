#ifndef __XLBE_BUTTON_WIDGET_H__
#define __XLBE_BUTTON_WIDGET_H__

#include "prereqs.h"

#include "imageset.h"
#include "layer.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

class Xlbe_Export Button_Listener 
{
public:
    virtual void button_press(int tag)=0;
};

///////////////////////////////////////////////////////////
class Xlbe_Export Button_Widget : public Layer
{		
public:
    typedef std::vector<Button_Listener *> Listeners;

public:
    Button_Widget(Image* button_image, Image *over_image, Image* down_image, Image* disabled_image);
    virtual ~Button_Widget();

public:
    virtual void draw();

public:
    virtual void mouse_enter();
    virtual void mouse_leave();

    virtual void mouse_down(const Point &pt, int num, int count);
    virtual void mouse_up(const Point &pt, int num, int count);

public:
    void add_listener(Button_Listener *listener);
    void remove_listener(Button_Listener *listener);

private:
    bool over_;
    bool down_;

    Image* button_image_;
    Image* over_image_;
    Image* down_image_;	
    Image* disabled_image_;

    Listeners listeners_; 
};

///////////////////////////////////////////////////////////
//}


#endif