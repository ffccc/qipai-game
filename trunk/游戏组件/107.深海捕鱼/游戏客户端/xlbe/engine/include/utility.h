#ifndef __XLBE_UTILITY_H__
#define __XLBE_UTILITY_H__

#include <string>

//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
const float M_PI = 3.14159265358979323846f;
const float M_PI_2 = 1.57079632679489661923f;
const float M_PI_4 = 0.785398163397448309616f;
const float M_1_PI = 0.318309886183790671538f;
const float M_2_PI = 0.636619772367581343076f;

static std::string string_to_upper(const std::string& s)
{
    std::string ts;

    for (unsigned i = 0; i < s.length(); i++)
        ts += toupper(s[i]);

    return ts;
}

//////////////////////////////////////////////////////////////
//}
#endif