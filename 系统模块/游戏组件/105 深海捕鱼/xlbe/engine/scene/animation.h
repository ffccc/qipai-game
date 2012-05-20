#ifndef __XLBE_ANIMATION_H__
#define __XLBE_ANIMATION_H__

#include <vector>
#include <string>

#include "xlbe.h"
#include "sprite_frame.h"

//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
class Xlbe_Export Animation : private std::vector<Sprite_Frame*>
{
public:
    typedef std::vector<Sprite_Frame*> Base;
    using Base::at;
    using Base::push_back;
    using Base::erase;
    using Base::size;

public:
    Animation();
    Animation(const std::string &file);
    ~Animation();

public:
    float delay() const            { return delay_; }
    void set_delay(float delay)    { delay_ = delay; }

    int tag() const                { return tag_; }
    void set_tag(int tag)          { tag_ = tag; }

    std::string name() const               { return name_; }
    void set_name(const std::string &name) { name_ = name; } 

private:
    void parse_file(const std::string &file);

private:
    int tag_;
    std::string name_;
    float delay_;
};

//////////////////////////////////////////////////////////////
//}
#endif