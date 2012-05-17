#ifndef _XLB_CRIT_SECT_H__
#define _XLB_CRIT_SECT_H__

#include <windows.h>

////////////////////////////////////////////////////////////////
//namespace xlbe {

////////////////////////////////////////////////////////////////
class Crit_Sect 
{
protected:
    CRITICAL_SECTION critical_section_;
    friend class Auto_Crit;

public:
    Crit_Sect ()   { InitializeCriticalSection(&critical_section_); }
    ~Crit_Sect ()  { DeleteCriticalSection(&critical_section_); }
};

/////////////////////////////////////////////////////////////////
#endif 
