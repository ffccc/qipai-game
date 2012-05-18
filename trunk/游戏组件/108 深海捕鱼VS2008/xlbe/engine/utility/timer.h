#ifndef __XLBE_TIMER_H__
#define __XLBE_TIMER_H__

#include <string>
#include <windows.h>
#include <time.h>

#include "xlbe.h"
#include "utility.h"
//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
class Xlbe_Export Timer 
{
private:
    clock_t zero_clock_;
    
public:
    Timer();
    ~Timer();
    
    bool set_option( const std::string& strKey, const void* value );
    
    void reset();
    
    unsigned long get_milli_seconds();
    unsigned long get_micro_seconds();
};

///////////////////////////////////////////////////////////////
//}
#endif 