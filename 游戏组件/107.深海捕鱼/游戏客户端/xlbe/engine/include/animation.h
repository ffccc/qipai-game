#ifndef __XLBE_ANIMATION_H__
#define __XLBE_ANIMATION_H__

#include "prereqs.h"
#include "image.h"

//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
class Xlbe_Export Animation : private std::vector<Image*>
{
public:
    typedef std::vector<Image*> Base;
    using Base::at;
    using Base::push_back;
    using Base::erase;
    using Base::size;

public:
    Animation(const std::string &name);
    ~Animation();

public:
    float delay() const { return delay_; }
    void set_delay(float delay) { delay_ = delay; }

    std::string name() const { return name_; }
    void set_name(const std::string &name) { name_ = name; } 

	Image *image(int index);
	void add_image(Image *image);
	void remove_image(int index);

private:
    float delay_;
    std::string name_; 
};

//////////////////////////////////////////////////////////////
//}
#endif