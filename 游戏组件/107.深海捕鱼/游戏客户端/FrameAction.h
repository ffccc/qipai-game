#ifndef __FISH_FRAME_ACTION_H__
#define __FISH_FRAME_ACTION_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

///////////////////////////////////////////////////////////////////
class Key_Frame
{
public:
    Key_Frame():angle_(0) {  }
    Key_Frame(const Point &position, float angle)
        :position_(position), angle_(angle)
    {
    }
    ~Key_Frame()
    {
    }

public:
    float angle_;
    Point position_;
};

typedef std::vector<Key_Frame> Key_Frames; 

class Action_Key_Frame : public Action_Interval
{
public:
    Action_Key_Frame(float d, Key_Frames &frames, const Point &offset)
        :Action_Interval(d)
    {
        Key_Frame frame;

        for (Key_Frames::iterator i=frames.begin(); i!=frames.end(); ++i)
        {
            if (offset.x_!=0 && offset.y_!=0)
            {
               frame = *i;
               frame.position_ += offset;
               key_frames_.push_back(frame);

            }
            else
            {
                key_frames_.push_back((*i));
            }

            
        }

        duration_ = d*key_frames_.size();
    }

    virtual ~Action_Key_Frame()
    {
    }

public:

    virtual void update(float time)
    {
        float fDiff;
        float fIndex = time * key_frames_.size();
        int index = fIndex;

        fDiff = fIndex - index;

        if (index >= key_frames_.size())
        {
            index = key_frames_.size() - 1;
        }

        Key_Frame key;

        if (index<key_frames_.size()-1)
        {
            Key_Frame key1 = key_frames_.at(index);
            Key_Frame key2 = key_frames_.at(index+1);

            key.position_ = key1.position_*(1.0-fDiff)+ key2.position_*fDiff;
            key.angle_ = key1.angle_*(1.0-fDiff)+ key2.angle_*fDiff;

            if (std::abs(key1.angle_-key2.angle_) > 180.0)
            {
                key.angle_ = key1.angle_;
            }

        }
        else
        {
            key = key_frames_.at(index);
        }
 


        target_->set_position(key.position_);
        target_->set_rotation((360.0-key.angle_)*M_PI/180.0);

    }



private:
    Key_Frames key_frames_;
};

#endif