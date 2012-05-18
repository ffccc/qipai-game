#ifndef __XLBE_RESOURCE_MANAGER_H__
#define __XLBE_RESOURCE_MANAGER_H__

#include <set>
#include <map>
#include <list>
#include <string>

#include <tinyxml.h>

#include "xlbe.h"

#include "utility/utility.h"
#include "scene/sprite_frame.h"
#include "scene/animation.h"
#include "scene/animation_cache.h"
#include "render/render_system.h"

////////////////////////////////////////////////////////////
//namespace xlbe{

////////////////////////////////////////////////////////////
class Xlbe_Export Resource_Manager
{
public:
    enum Res_Type
    {
        RES_TYPE_TEXTURE,
        RES_TYPE_SPRITE_FRAME,
        RES_TYPE_ANIMATION
    };

    struct Base_Res
    {
        Res_Type type_;
        std::string id_;
        std::string group_;

        virtual ~Base_Res() {}
        virtual void delete_resource() { }
    };

    struct Texture_Res : public Base_Res
    {
        std::string path_;

        Render_Texture* tex_;

        Texture_Res() { type_ = RES_TYPE_TEXTURE;  tex_=0; }
        virtual ~Texture_Res() { }
        virtual void delete_resource() { if (tex_) { Render_Texture_Cache::instance()->remove_texture(path_); } }
    };

    struct Sprite_Frame_Res : public Base_Res
    {
        Rect rect_;
        std::string path_;

        Sprite_Frame *frame_;

        Sprite_Frame_Res() { type_ = RES_TYPE_SPRITE_FRAME;  frame_=0; }
        virtual ~Sprite_Frame_Res() {}
        virtual void delete_resource() { Render_Texture_Cache::instance()->remove_texture(path_); if (frame_) { delete frame_; frame_=0; } }
    };

    struct Animation_Res : public Base_Res
    {
        std::string path_;

        Animation *animation_;

        Animation_Res() { type_ = RES_TYPE_ANIMATION;  animation_=0; }
        virtual ~Animation_Res() {}
        virtual void delete_resource() { if (animation_) { Animation_Cache::instance()->remove_animation(path_); } }
    };

    typedef std::map<std::string, Base_Res*> Res_Map;
    typedef std::list<Base_Res*> Res_List;
    typedef std::map<std::string, Res_List, String_Less_No_Case> Res_Group_Map;

public:
    Resource_Manager();
    virtual ~Resource_Manager();

public:
    void prase_resources_file(const std::string &file);
    int number_resources(const std::string &group) { return number_texture(group)+number_sprite_frame(group)+number_animation(group); }

    void start_load_resources(const std::string &group);
    bool load_next_resource();
    void remove_resources(const std::string &group);

    void load_resources(const std::string &group);

    int number_texture(const std::string &group)   { return number_resources(group, texture_map_); }
    Render_Texture *texture(const std::string &id);
    void replace_texture(const std::string &id, Render_Texture *tex);
    void remove_texture(const std::string &id);

    int number_sprite_frame(const std::string &group)    { return number_resources(group, sprite_frame_map_); }
    Sprite_Frame *sprite_frame(const std::string &id);
    void replace_sprite_frame(const std::string &id, Sprite_Frame *frame);
    void remove_sprite_frame(const std::string &id);

    int number_animation(const std::string &group)    { return number_resources(group, animation_map_); }
    Animation *animation(const std::string &id);
    void replace_animation(const std::string &id, Animation *animation);
    void remove_animation(const std::string &id);

    bool is_group_loaded(const std::string &group);  
    std::string current_resource_group()          { return cur_res_group_; }
    Res_List *current_resource_group_list()       { return cur_res_group_list_; }

private:
    int number_resources(const std::string &group, Res_Map &resmap);
    void remove_resources(Res_Map &resmap, const std::string &group);

    void do_load_textrue(Texture_Res *res);
    void do_load_sprite_frame(Sprite_Frame_Res *res);
    void do_load_animation(Animation_Res *res);

    void prase_resources(TiXmlElement* element);

    void prase_set_defualts(TiXmlElement *element);
    void parse_common_resource(TiXmlElement *element, Base_Res *baseres, Res_Map &resmap);

    void prase_texture_resources(TiXmlElement *element);
    void prase_sprite_frame_resources(TiXmlElement *element);
    void prase_animation_resources(TiXmlElement *element);

    void delete_map(Res_Map &resmap);

private:
    Res_Map texture_map_;
    Res_Map sprite_frame_map_;
    Res_Map animation_map_;

    std::string cur_res_group_;
    std::string default_path_;

    Res_Group_Map res_group_map_;
    Res_List* cur_res_group_list_;
    Res_List::iterator cur_res_group_listor_;

    std::set<std::string,String_Less_No_Case> loaded_groups_;

    TiXmlDocument *xml_doc_;
};

/////////////////////////////////////////////////////////
//}
#endif