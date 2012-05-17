#ifndef __XLB_DSOUND_INSTANCE_H__
#define __XLB_DSOUND_INSTANCE_H__

#include <windows.h>

#include "sound_instance.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

class DSound_Manager;

///////////////////////////////////////////////////////////////////
class DSound_Instance : public Sound_Instance
{
    friend class DSound_Manager;
public:
    DSound_Instance(DSound_Manager* manager, LPDIRECTSOUNDBUFFER sourcesound);
    virtual ~DSound_Instance();

public:
    virtual void release() ;

    virtual void set_base_volume(double volume) ; 
    virtual void set_base_pan(int pan) ;

    virtual void adjust_pitch(double steps) ;

    virtual void set_volume(double volume) ; 
    virtual void set_pan(int position) ; 

    virtual void play(bool looping, bool autorelease) ;	
    virtual void stop() ;
    virtual bool is_playing() ;
    virtual bool is_released() ;
    virtual double volume() ;

protected:
    void rehup_volume();
    void rehup_pan();

protected:
    DSound_Manager* sound_manager_;
    LPDIRECTSOUNDBUFFER source_sound_buffer_;
    LPDIRECTSOUNDBUFFER sound_buffer_;
    bool auto_release_;
    bool has_played_;
    bool released_;

    int base_pan_;
    double base_volume_;

    int pan_;
    double volume_;	

    DWORD default_frequency_;
};

///////////////////////////////////////////////////////////////////

//}
#endif