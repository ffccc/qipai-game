#ifndef __XLBE_UTILITY_H__
#define __XLBE_UTILITY_H__

#include <string>
#include <sstream>

//////////////////////////////////////////////////////////////
//namespace xlbe {
//////////////////////////////////////////////////////////////

#define	MAX_FLOAT FLT_MAX
#define	EPSILON	  FLT_EPSILON

#define M_PI	3.14159265358979323846f
#define M_PI_2	1.57079632679489661923f
#define M_PI_4	0.785398163397448309616f
#define M_1_PI	0.318309886183790671538f
#define M_2_PI	0.636619772367581343076f

#pragma warning(disable: 4786)

//////////////////////////////////////////////////////////////
struct String_Less_No_Case 
{ 
    bool operator() (const std::string &s1, const std::string &s2) const 
    { return _stricmp(s1.c_str(),s2.c_str())<0; } 
};

//////////////////////////////////////////////////////////////
//}
#endif