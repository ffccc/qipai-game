#ifndef __XLB_IMAGESET_H__
#define __XLB_IMAGESET_H__

#include "prereqs.h"
#include "color_rect.h"
#include "render_system.h"
#include "image.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Xlbe_Export Imageset : private std::map<std::string, Image>
{
public:
    typedef std::map<std::string, Image> Base;
    using Base::iterator;
    using Base::find;
    using Base::erase;
    using Base::size;
    using Base::insert;
    using Base::clear;

public:
    Imageset(const std::string& name, Render_Texture *texture);
    ~Imageset();

public:
    std::string name() const               { return name_; }
    void set_name(const std::string &name) { name_ = name; } 

    Render_Texture *texture() { return texture_; }
    void set_texture(Render_Texture* texture) { texture_ = texture; }

    int image_count(const std::string &name) { return size(); }

    Image *image(const std::string &name);
    bool is_image_defined(const std::string &name);
    void define_image(const std::string &name, const Rect &area);
    void undefine_image(const std::string &name);
    void undefine_all_image();

    void draw(const Point dest[4], const Point src[4], const Color_Rect& colors, Render_Blend blend);

private:
    std::string name_; 
    Render_Quad quad_;
    Render_Texture *texture_;
};

///////////////////////////////////////////////////////////////////
//}
#endif