#ifndef __XLB_ANIMATION_CACHE_H__
#define __XLB_ANIMATION_CACHE_H__

#include <string>
#include <map>

#include "xlbe.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

class Animation;

///////////////////////////////////////////////////////////////////
class Xlbe_Export Animation_Cache : private std::map<std::string, Animation*>
{
public:
    typedef std::map<std::string, Animation*> Base;

private:
    Animation_Cache::Animation_Cache();
    Animation_Cache(const Animation_Cache&);
    const Animation_Cache& operator=(const Animation_Cache&);

public:
    ~Animation_Cache();
    static Animation_Cache *instance();

public:
    Animation *add_animation(const std::string &name);
    void remove_animation(Animation *ani);
    void remove_animation(const std::string &name);
    void remove_all_animation();

private:
    static Animation_Cache *instance_;

};

///////////////////////////////////////////////////////////////////
//}
#endif