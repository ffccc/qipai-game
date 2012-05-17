#ifndef __XLBE_FONT_TTF_H__
#define __XLBE_FONT_TTF_H__

#include "prereqs.h"

#include "color.h"
#include "render_system.h"

#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Font_Ttf : public Font
{		
private:
    struct Cache_Entry
    {
        wchar_t char_code_;

        unsigned long glyph_index_;
        unsigned long advance_;

        unsigned long width_, height_;

        unsigned long left_, top_;
        unsigned long glyph_width_, glyph_height_;
        
        Render_Texture *texture_;
        unsigned long texture_width_, texture_height_;

        Cache_Entry():texture_(0) {}
        ~Cache_Entry() { if (texture_) delete texture_; }
    };

    typedef std::map<wchar_t, Cache_Entry*> Caches;

public:
    Font_Ttf(const std::string &name, const std::string &file);
    virtual ~Font_Ttf();

public:
    virtual std::string name() const { return name_; }

    virtual int height()   ;
    virtual int string_width(const std::string &s)  ;

    virtual void set_size(const Size &size) ;
    virtual void set_color(const Color &color);

    virtual int set_align(int align) { int old=align_; align_=align;  return old; }

    virtual void draw_string(const Point &pt, const std::string &s, const Color &color) ;

private:
    void load_ft_libraray();
    void unload_ft_libraray();
    void load_font_file(const std::string &file);
    void unload_font_file();

    void update_size();

    void cache_char(wchar_t c);
    void cache_chars(const std::wstring& s);
    void cache_chars(wchar_t start, wchar_t end);
    bool is_char_cached(wchar_t c);
    void clear_cache();

    Cache_Entry* cache_entry(wchar_t c);
    void render_glyph(FT_Bitmap* glyph, Render_Texture **texture, unsigned long& width, unsigned long& height);

    int char_width(wchar_t c);
    int string_width(const std::wstring& s);

    unsigned long space_advance();

    void draw_char(wchar_t c, float x, float y);
    void draw_string(const std::wstring& s, float x, float y);

    void set_position(int x, int y, int width, int height);
    void set_texture(Render_Texture *texture_, const Rect &rect);

private:
    std::string name_;
    
    int width_;
    int height_;
    int last_width_;
    int last_height_;

    int align_;

    bool has_kerning_;
    FT_Face face_;
  
    Caches caches_;
    Render_Quad quad_;

    static int library_ref_;
    static FT_Library library_;
};

///////////////////////////////////////////////////////////
//}


#endif