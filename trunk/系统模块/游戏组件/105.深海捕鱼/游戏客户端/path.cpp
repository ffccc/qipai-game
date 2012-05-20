#include "path.h"


#include <stdio.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////
//namespace xlbe {	

///////////////////////////////////////////////////////////////
PathManager *PathManager::instance_ = 0;

///////////////////////////////////////////////////////////////
PathManager* PathManager::instance()
{
    if (instance_ == 0)
    {
        instance_ = new PathManager();
    }

    return instance_;
}

///////////////////////////////////////////////////////////
Path::Path (const std::string &file)
{
    std::ifstream ifs(file.c_str(), std::ios::binary);

    if (ifs.bad())
    {
        throw "Path::Path  - load flle failed";
    }

    int _size;
    int x, y;
    ifs.read((char*)&_size, sizeof(int));

    for (int i=0; i<_size; i++)
    {
        ifs.read((char*)&x, sizeof(int));
        ifs.read((char*)&y, sizeof(int));
                  
        Point pt(x, y);
        push_back(pt);
    }
}

Path::~Path (void)
{
}

////////////////////////////////////////////////////////////////
PathManager::PathManager()
{
}

PathManager::~PathManager()
{
    for (iterator i=begin(); i!=end(); ++i)
    {
        delete (*i);
    }
}

const Path *PathManager::get_path(int index)
{
    if (index<0 || index>size())
        throw "PathManager::path index failed";

    return at(index);
}

void PathManager::add_path(const std::string &file)
{
    Path *path = new Path(file);
    
    push_back(path);
}

void PathManager::remove_path(int index)
{
    //erase((iterator)index);
}

////////////////////////////////////////////////////////////////