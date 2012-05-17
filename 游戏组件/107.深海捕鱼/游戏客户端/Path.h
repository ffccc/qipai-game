#ifndef __XLBE_FISH_PAHT_H__
#define __XLBE_FISH_PAHT_H__

#include "xlbe.h"

//////////////////////////////////////////////////////////////
class CPath : private std::vector<Point>
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
    CPath (const std::string &file);
    virtual ~CPath (void);

    int GetCount() const { return m_nCount; }

private:
    int m_nCount;
};


//////////////////////////////////////////////////////////////
//}
#endif 