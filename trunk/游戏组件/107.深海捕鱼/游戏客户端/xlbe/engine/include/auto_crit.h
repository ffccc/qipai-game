#ifndef __XLB_AUTOCRIT_H__
#define __XLB_AUTOCRIT_H__


#include "crit_sect.h"

/////////////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////////////
class Auto_Crit
{
    LPCRITICAL_SECTION		crit_sec_;
public:
    Auto_Crit(LPCRITICAL_SECTION critsec) : crit_sec_(critsec)
    { 
        EnterCriticalSection(crit_sec_); 
    }

    Auto_Crit(const Crit_Sect& critsec) :  crit_sec_((LPCRITICAL_SECTION) &critsec.critical_section_)
    { 
        EnterCriticalSection(crit_sec_); 
    }

    ~Auto_Crit()
    { 
        LeaveCriticalSection(crit_sec_); 
    }
};

//////////////////////////////////////////////////////////////
//}

#endif 