#ifndef __XLBE_RESOURCE_MANAGER_H__
#define __XLBE_RESOURCE_MANAGER_H__

#include <set>
#include <map>
#include <list>
#include <string>

#include "tinyxml.h"

#include "prereqs.h"

#include "utility.h"

////////////////////////////////////////////////////////////
//namespace xlbe{

////////////////////////////////////////////////////////////
class Xlbe_Export Resource_Manager
{
public:
    enum Res_Type
    {
        RES_TYPE_IMAGESET,
        RES_TYPE_ANIMATION,
        RES_TYPE_SOUND,
        RES_TYPE_MUSIC

    };

    struct Base_Res
    {
        Res_Type type_;
        std::string name_;
        std::string path_;
        std::string group_;

        virtual ~Base_Res() {}
        virtual void delete_resource() { }
    };

    struct Imageset_Res : public Base_Res
    {
        Imageset_Res() { type_ = RES_TYPE_IMAGESET; }
        virtual ~Imageset_Res() {}
        virtual void delete_resource();
    };

    struct Animation_Res : public Base_Res
    {
        Animation_Res() { type_ = RES_TYPE_ANIMATION; }
        virtual ~Animation_Res() {}
        virtual void delete_resource(); 
    };

    struct Sound_Res : public Base_Res
    {
        Sound_Res() { type_ = RES_TYPE_SOUND; }
        virtual ~Sound_Res() {}
        virtual void delete_resource(); 
    };

	struct Music_Res : public Base_Res
    {
        Music_Res() { type_ = RES_TYPE_MUSIC; }
        virtual ~Music_Res() {}
        virtual void delete_resource(); 
    };

    typedef std::map<std::string, Base_Res*> Res_Map;
    typedef std::list<Base_Res*> Res_List;
    typedef std::map<std::string, Res_List> Res_Group_Map;

public:
    Resource_Manager();
    virtual ~Resource_Manager();

public:
    void prase_resources_file(const std::string &file);
    int number_resources(const std::string &group); 

    void start_load_resources(const std::string &group);
    bool load_next_resource();
    
    void load_resources(const std::string &group);
    void remove_resources(const std::string &group);

    bool is_group_loaded(const std::string &group);  
    std::string current_resource_group()          { return cur_res_group_; }
    Res_List *current_resource_group_list()       { return cur_res_group_list_; }

private:
    int number_resources(const std::string &group, Res_Map &resmap);
    void remove_resources(Res_Map &resmap, const std::string &group);

    void do_load_imageset(Imageset_Res *res);
    void do_load_animation(Animation_Res *res);
    void do_load_sound(Sound_Res *res);
    void do_load_music(Music_Res *res);

    void prase_resources(TiXmlElement* element);

    void prase_set_defualts(TiXmlElement *element);
    void parse_common_resource(TiXmlElement *element, Base_Res *baseres, Res_Map &resmap);

    void prase_imageset_resources(TiXmlElement *element);
    void prase_animation_resources(TiXmlElement *element);
    void prase_sound_resources(TiXmlElement *element);
	void prase_music_resources(TiXmlElement *element);

    void delete_map(Res_Map &resmap);

private:
    Res_Map imageset_map_;
    Res_Map animation_map_;
    Res_Map sound_map_;
	Res_Map music_map_;

    std::string cur_res_group_;
    std::string default_path_;

    Res_Group_Map res_group_map_;
    Res_List* cur_res_group_list_;
    Res_List::iterator cur_res_group_listor_;

    std::set<std::string> loaded_groups_;

    TiXmlDocument *xml_doc_;
};

/////////////////////////////////////////////////////////
//}
#endif