#ifndef __XLB_RENDER_TEXTURE_CACHE_H__
#define __XLB_RENDER_TEXTURE_CACHE_H__

#include <string>
#include <map>

#include "xlbe.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

class Render_Texture;

///////////////////////////////////////////////////////////////////
class Xlbe_Export Render_Texture_Cache : private std::map<std::string, Render_Texture*>
{
public:
    typedef std::map<std::string, Render_Texture*> Base;

private:
    Render_Texture_Cache::Render_Texture_Cache();
    Render_Texture_Cache(const Render_Texture_Cache&);
    const Render_Texture_Cache& operator=(const Render_Texture_Cache&);

public:
    ~Render_Texture_Cache();
    static Render_Texture_Cache *instance();

public:
    Render_Texture *add_image(const std::string &name);
    void remove_texture(Render_Texture *tex);
    void remove_texture(const std::string &name);
    void remove_all_texture();

private:
    static Render_Texture_Cache *instance_;

};

///////////////////////////////////////////////////////////////////
//}
#endif