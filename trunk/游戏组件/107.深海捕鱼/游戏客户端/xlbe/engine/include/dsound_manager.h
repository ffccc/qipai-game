#ifndef __XLB_DSOUND_MANAGER_H__
#define __XLB_DSOUND_MANAGER_H__

#include <string>

#include "dsound_version.h"
#include "sound_manager.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class DSound_Instance;

class DSound_Manager : public Sound_Manager
{
    friend class DSound_Instance;
public:
    DSound_Manager();
    virtual ~DSound_Manager();

public:
    virtual bool initialised();
    virtual void initialise(HWND hwnd);

    virtual void load_sound(unsigned int id, const std::string& file);
    virtual int load_sound(const std::string& file) ;
    virtual void release_sound(unsigned int id) ;

    virtual void set_volume(double volume) ;
    virtual void set_base_volume(unsigned int id, double volume);
    virtual void set_base_pan(unsigned int id, int pan) ;	
 
    virtual Sound_Instance*	sound_instance(unsigned int id) ;

    virtual void release_sounds() ;
    virtual void release_channels() ;

    virtual double master_volume() ;
    virtual void set_master_volume(double volume) ;

    virtual void flush() ;
    virtual void set_cooperative_window(HWND hwnd, bool windowed) ;
    virtual void stop_all_sounds() ;
    virtual int free_sound_id() ;
    virtual int num_sounds() ;

protected:
    int find_free_channel();
    void release_free_channels();

    int volume_to_db(double volume);

    bool load_ogg_sound(unsigned int id, const std::string& file);
    bool load_fmod_sound(unsigned int id, const std::string& file);
    bool load_wav_sound(unsigned int id, const std::string& file);
    bool load_av_sound(unsigned int id, const std::string& file);

protected:
    LPDIRECTSOUNDBUFFER source_sounds_[MAX_SOURCE_SOUNDS];
    std::string source_file_names_[MAX_SOURCE_SOUNDS];
    LPDIRECTSOUNDBUFFER primary_buffer_;
    unsigned long source_data_sizes_[MAX_SOURCE_SOUNDS];
    double base_volumes_[MAX_SOURCE_SOUNDS];
    int base_pans_[MAX_SOURCE_SOUNDS];
    DSound_Instance* playing_sounds_[MAX_CHANNELS];	
    double master_volume_;
    unsigned long last_release_tick_;

private:
    HMODULE dsound_dll_;
    LPDIRECTSOUND direct_sound_;
    bool have_fmod_;
};

///////////////////////////////////////////////////////////////////

//}
#endif