#ifndef __FISH_APPLICATION_H__
#define __FISH_APPLICATION_H__

#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"

///////////////////////////////////////////////////////////
class CFishApplication : public Application 
{
public:
    CFishApplication (const std::string &title, const std::string &config, const std::string &cmdline);
    virtual ~CFishApplication ();

public:
	virtual double GetMasterVolume();
	virtual double GetMusicVolume();
	virtual double GetSoundVolume();

	virtual void SetMasterVolume(double theVolume);
	virtual void SetMusicVolume(double theVolume);
	virtual void SetSoundVolume(double theVolume);	

protected:
    virtual void initialise() ;
    virtual void destroy();

    virtual void loading_thread_proc() ;

	virtual void save_config();
    virtual void restore_config();

private:
	double m_SoundVolume;
	double m_MusicVolume;

};

extern CFishApplication *theApp;

///////////////////////////////////////////////////////////////////////////////////
#endif
