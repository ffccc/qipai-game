#ifndef __XLB_SOUND_MANAGER_H__
#define __XLB_SOUND_MANAGER_H__

#include <string>

#include <windows.h>

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
#define MAX_CHANNELS		  32
#define MAX_SOURCE_SOUNDS	256

class Sound_Instance;

///////////////////////////////////////////////////////////////////
class Sound_Manager
{
public:
	Sound_Manager() {}
	virtual ~Sound_Manager() {}

public:
    virtual bool initialised() = 0;
    virtual void initialise(HWND hwnd) = 0;

    virtual void load_sound(unsigned int id, const std::string& file) = 0;
    virtual int load_sound(const std::string& file) = 0;
    virtual void release_sound(unsigned int id) = 0;

    virtual void set_volume(double volume) = 0;
    virtual void set_base_volume(unsigned int id, double volume) = 0;
    virtual void set_base_pan(unsigned int id, int pan) = 0;	

    virtual Sound_Instance*	sound_instance(unsigned int id) = 0;

    virtual void release_sounds() = 0;
    virtual void release_channels() = 0;

    virtual double master_volume() = 0;
    virtual void set_master_volume(double volume) = 0;

    virtual void flush() = 0;
    virtual void set_cooperative_window(HWND hwnd, bool windowed) = 0;
    virtual void stop_all_sounds() = 0;
    virtual int free_sound_id() = 0;
    virtual int num_sounds() = 0;
};

///////////////////////////////////////////////////////////////////

//}
#endif