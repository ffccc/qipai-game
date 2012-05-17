#ifndef __XLBE_H__
#define __XLBE_H__
/////////////////////////////////////////////////////

#ifndef Xlbe_Export

#ifdef  Xlbe_DLL
#define Xlbe_Export _declspec(dllexport)
#else 
#define Xlbe_Export _declspec(dllimport)
#endif

#endif
#endif