#ifndef __XLBE_FISH_PAHT_H__
#define __XLBE_FISH_PAHT_H__

#include <cmath>
#include <string>
#include <vector>
#include <fstream>

#include "utility/Point.h"

//////////////////////////////////////////////////////////////
#define BLOCK_SIZE   8

//////////////////////////////////////////////////////////////
class Path : private std::vector<Point>
{
public:
	typedef std::vector<Point>  base;
  using base::iterator;
  using base::size;
  using base::push_back;
  using base::begin;
  using base::end;
  using base::at;

public:
    Path (const std::string &file);
    virtual ~Path (void);
};

class PathManager : public std::vector<Path *>
{
public:
	typedef std::vector<Path*>  base;
  using base::iterator;
  using base::size;
  using base::push_back;
  using base::begin;
  using base::end;
  using base::at;

public:
    PathManager();
    ~PathManager();

    static PathManager *instance();

public:
    const Path *get_path(int index);  
    void add_path(const std::string &file);
    void remove_path(int index);

private:
    PathManager(const PathManager&);
    const PathManager& operator=(const PathManager&);

    static PathManager *instance_;
};

//////////////////////////////////////////////////////////////
//}
#endif 