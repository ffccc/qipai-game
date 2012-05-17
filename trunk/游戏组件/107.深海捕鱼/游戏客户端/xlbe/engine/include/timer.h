#ifndef __XLBE_TIMER_H__
#define __XLBE_TIMER_H__

#include <time.h>

#include "prereqs.h"
#include "utility.h"
//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
class Xlbe_Export Timer 
{    
public:
    Timer()    { reset(); }
    ~Timer()   {}

    void reset()  { zero_clock_ = clock(); }

    unsigned long get_milli_seconds()
    {
        clock_t newclock = clock();
        return (unsigned long)( (float)( newclock - zero_clock_ ) / ( (float)CLOCKS_PER_SEC / 1000.0 ) ) ;
    }

    unsigned long get_micro_seconds()
    {
        clock_t newclock = clock();
        return (unsigned long)( (float)( newclock - zero_clock_ ) / ( (float)CLOCKS_PER_SEC / 1000000.0 ) ) ;
    }

private:
    clock_t zero_clock_;
};

///////////////////////////////////////////////////////////////
//}
#endif 