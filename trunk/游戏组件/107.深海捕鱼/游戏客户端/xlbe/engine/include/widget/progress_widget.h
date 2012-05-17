#ifndef __XLBE_PROGRESS_WIDGET_H__
#define __XLBE_PROGRESS_WIDGET_H__

#include "prereqs.h"

#include "imageset.h"
#include "layer.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

class Xlbe_Export Progress_Listener 
{
public:
    virtual void progress_step(int tag, int pos) =0;
};

///////////////////////////////////////////////////////////
class Xlbe_Export Progress_Widget : public Layer
{		
public:
    typedef std::vector<Progress_Listener *> Listeners;

public:
    Progress_Widget(Image* image_left, Image* image_middle, Image* image_right, Image *step_image);
    virtual ~Progress_Widget();

public:
    void range(int &lower, int &upper) const { lower = lower_; upper = upper_; }
    void set_range(int lower, int upper) { lower_ = lower; upper_ = upper; }

    int pos_range() const { return pos_range_; }
    void set_pos_range(int pos) { pos_range_ = pos; }

    void set_step(int step) { step_ = step; }
    void step() { pos_range_ += step_; }

public:
    virtual void draw();

public:
    void add_listener(Progress_Listener *listener);
    void remove_listener(Progress_Listener *listener);

private:
    int lower_;
    int upper_;
    int step_;
    int pos_range_;

    Image* image_left_;
    Image* image_middle_;
    Image* image_right_;
    Image* step_image_;

    Listeners listeners_; 
};

///////////////////////////////////////////////////////////
//}


#endif