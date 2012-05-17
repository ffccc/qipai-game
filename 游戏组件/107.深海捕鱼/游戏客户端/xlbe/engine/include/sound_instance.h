#ifndef __XLB_SOUND_INSTANCE_H__
#define __XLB_SOUND_INSTANCE_H__

#include <string>

#include <windows.h>

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Sound_Instance
{
public:
	Sound_Instance() {}
	virtual ~Sound_Instance() {}

public:
    virtual void release() = 0;

    virtual void set_base_volume(double volume) = 0; 
    virtual void set_base_pan(int pan) = 0;

    virtual void adjust_pitch(double steps) = 0;

    virtual void set_volume(double volume) = 0; 
    virtual void set_pan(int position) = 0; 

    virtual void play(bool looping, bool autorelease) = 0;	
    virtual void stop() = 0;
    virtual bool is_playing() = 0;
    virtual bool is_released() = 0;
    virtual double volume() = 0;
};

///////////////////////////////////////////////////////////////////

//}
#endif