#ifndef __XLBE_RENDER_TEXTURE_H__
#define __XLBE_RENDER_TEXTURE_H__

#include <string>

#include "prereqs.h"

#include "size.h"

/////////////////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////////////////
class Xlbe_Export Render_Texture
{
public:
    Render_Texture() {}
    virtual ~Render_Texture() {}

public:
    virtual std::string name() const=0;
    virtual void set_name(const std::string &name)=0;

    virtual void create(int width, int height) =0;
    virtual void load(const std::string &file) =0;
    virtual void destory() =0;

    virtual Size size() = 0;

    virtual unsigned long *lock(bool readonly, int left=0, int top=0, int width=0, int height=0)=0;
    virtual void unlock()=0;
};

/////////////////////////////////////////////////////////////////
//}
#endif
