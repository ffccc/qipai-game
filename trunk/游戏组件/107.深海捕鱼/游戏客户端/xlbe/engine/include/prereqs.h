#ifndef __XLBE_PREREQS_H__
#define __XLBE_PREREQS_H__

//////////////////////////////////////////////////////////////
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <deque>

#include "config.h"

//////////////////////////////////////////////////////////////
#define Xlbe_Export

#if defined( _MSC_VER )
#	define XLB_MSVC_COMPILER
#endif

#if defined(__WIN32__) || defined(_WIN32)
# define XLB_WIN32_PLATFORM
#		if defined( XLB_DYNAMIC_LIB )
#			undef Xlbe_Export
#			if defined( XLB_NONCLIENT_BUILD )
#				define Xlbe_Export __declspec( dllexport )
#			else
#				define Xlbe_Export __declspec( dllimport )
#			endif
#		endif
#endif


//////////////////////////////////////////////////////////////
#endif 