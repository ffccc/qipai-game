#ifndef __XLBE_FONT_MANAGER_H__
#define __XLBE_FONT_MANAGER_H__

#include "prereqs.h"

#include "font.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Font_Manager 
{		
public:
    typedef std::map<std::string, Font*> Fonts;

public:
    Font_Manager();
    ~Font_Manager();

public:
    Font *create_font_ttf(const std::string& name, const std::string& file);
    void destory_font_ttf(const std::string& name);
    void destory_font_ttf(Font *font);
    Font *font_ttf(const std::string& name);

private:
    Fonts font_ttfs_;

};

///////////////////////////////////////////////////////////
//}


#endif