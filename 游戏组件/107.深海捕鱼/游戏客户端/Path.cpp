#include "path.h"

///////////////////////////////////////////////////////////
CPath::CPath (const std::string &file)
{
    Pack_File *pf = pfopen(file.c_str(), "rb");
    if (pf == 0)
    {
        throw "Path read error";
    }

    int x, y;
    pfread(&m_nCount, sizeof(int), 1, pf);

    for (int i=0; i<m_nCount*4; i++)
    {
        pfread(&x, sizeof(int), 1, pf);
        pfread(&y, sizeof(int), 1, pf);

        Point pt(x, y);
        push_back(pt);
    }

    pfclose(pf);
}

CPath::~CPath (void)
{
}



////////////////////////////////////////////////////////////////