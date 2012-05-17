#ifndef __XLB_IMAGESET_MANAGER_H__
#define __XLB_IMAGESET_MANAGER_H__

#include "prereqs.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

class Imageset;

///////////////////////////////////////////////////////////////////
class Xlbe_Export Imageset_Manager : private std::map<std::string, Imageset*>
{
public:
    typedef std::map<std::string, Imageset*> Base;
    using Base::iterator;
    using Base::find;
    using Base::erase;
    using Base::size;
    using Base::insert;
    using Base::clear;

public:
    Imageset_Manager();
    ~Imageset_Manager();

public:
    Imageset *create_imageset(const std::string& name, Render_Texture *texture);
    Imageset *imageset(const std::string &name);
    void remove_imageset(const std::string& name);

public:
	Imageset *load_from_xml(const std::string &xml);
};

///////////////////////////////////////////////////////////////////
//}
#endif