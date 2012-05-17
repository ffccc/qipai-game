#ifndef __XLB_ANIMATION_MAMAGER_H__
#define __XLB_ANIMATION_MAMAGER_H__

#include "prereqs.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

class Animation;

///////////////////////////////////////////////////////////////////
class Xlbe_Export Animation_Manager : private std::map<std::string, Animation*>
{
public:
    typedef std::map<std::string, Animation*> Base;

public:
    Animation_Manager();
    ~Animation_Manager();

public:
    Animation *animation(const std::string &name);
    void add_animation(Animation *ani);
    void remove_animation(Animation *ani);
    void remove_animation(const std::string &name);
    void remove_all_animation();

public:
	Animation *load_from_xml(const std::string &xml);
};

///////////////////////////////////////////////////////////////////
//}
#endif