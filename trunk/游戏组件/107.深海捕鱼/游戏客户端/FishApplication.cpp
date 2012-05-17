#include "FishApplication.h"

#include "LoadScene.h"
#include "GameScene.h"
#include "TestScene.h"

#include <sstream>

/////////////////////////////////////////////////////////
CFishApplication *theApp = 0;

/////////////////////////////////////////////////////////
CFishApplication::CFishApplication (const std::string &title, const std::string &config, const std::string &cmdline) 
:Application(title, config, cmdline)
{
    theApp = this;

	m_SoundVolume = 0.85;
	m_MusicVolume = 0.85;
}

CFishApplication ::~CFishApplication ()
{
}

void CFishApplication::save_config()
{
	__super::save_config();

   const char *value=0;
    TiXmlDocument doc;
    if(!doc.LoadFile(config_.c_str()))
    {
        throw root_error("CFishApplication::restore_config - TiXmlDocument load failed");
    }

    TiXmlElement *root = doc.FirstChildElement("config");
    if (root == 0)
    {
        throw root_error("CFishApplication::restore_config - TiXmlDocument conot find config element");
    }

    root->SetDoubleAttribute("sound", m_SoundVolume);
	root->SetDoubleAttribute("music", m_MusicVolume);

	root->SetAttribute("width", 1024);
	root->SetAttribute("height", 590);

	doc.SaveFile();
  
}

void CFishApplication::restore_config()
{
    __super::restore_config();

    int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
    int nFullHeight=GetSystemMetrics(SM_CYSCREEN);   

    std::ostringstream ostr;

    if (nFullWidth >= 1280)
    {
        Root::instance()->render_system()->set_config_options("width", "1280");
        Root::instance()->render_system()->set_config_options("height", "738");
    }

    const char *value=0;
    TiXmlDocument doc;
    if(!doc.LoadFile(config_.c_str()))
    {
        throw root_error("CFishApplication::restore_config - TiXmlDocument load failed");
    }

    TiXmlElement *root = doc.FirstChildElement("config");
    if (root == 0)
    {
        throw root_error("CFishApplication::restore_config - TiXmlDocument conot find config element");
    }

    if ((value=root->Attribute("sound", &m_SoundVolume)) == 0)
    {
        m_SoundVolume = 0.85;
    }

	if ((value=root->Attribute("music", &m_MusicVolume)) == 0)
    {
        m_MusicVolume = 0.85;
    }
}

void CFishApplication ::initialise() 
{
    Size szWindow = Root::instance()->render_window()->size();
    Root::instance()->render_system()->set_view_matrix(szWindow.width_/1280.0,szWindow.height_/738.0);

    pack_interface()->add_pack_file("fish\\fish.pak");
    pack_interface()->add_pack_file("fish\\path\\big.pak");
    pack_interface()->add_pack_file("fish\\path\\small.pak");
    pack_interface()->add_pack_file("fish\\path\\huge.pak");
    pack_interface()->add_pack_file("fish\\path\\special.pak");

    Root::instance()->resource_manager()->prase_resources_file("fish\\fish.resources");
    Root::instance()->resource_manager()->load_resources("Load");

    Scene *scLoad = new CLoadScene;
    Root::instance()->scene_director()->push_scene(scLoad);

    loading_thread_tasks_ = Root::instance()->resource_manager()->number_resources("Game");

    start_loading_thread();
}

void CFishApplication ::destroy()
{
}

void CFishApplication ::loading_thread_proc() 
{
    Root::instance()->resource_manager()->start_load_resources("Game");
    
    bool bLoad = true;
    while (bLoad)
    {
        if (!loading_thread_started_) return ;

        {
        Auto_Crit auto_crit(Root::instance()->sect_);
        bLoad = Root::instance()->resource_manager()->load_next_resource();
        }
       
        complete_loading_thread_tasks_++;
    }

    Auto_Crit auto_crit(Root::instance()->sect_);
    Root::instance()->scene_director()->replace_scene(new CGameScene);

    return ;
}

double CFishApplication::GetMasterVolume()
{
	return Root::instance()->sound_manager()->master_volume();
}

double CFishApplication::GetMusicVolume()
{
	return m_MusicVolume;
}

double CFishApplication::GetSoundVolume()
{
    return m_SoundVolume;
}

void CFishApplication::SetMasterVolume(double theVolume)
{
	return Root::instance()->sound_manager()->set_master_volume(theVolume);
}

void CFishApplication::SetMusicVolume(double theVolume)
{
	m_MusicVolume = theVolume;
	return Root::instance()->music_manager()->set_volume(theVolume);
}

void CFishApplication::SetSoundVolume(double theVolume)
{
	m_SoundVolume = theVolume;
	return Root::instance()->sound_manager()->set_volume(theVolume);
}