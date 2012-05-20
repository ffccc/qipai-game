#ifndef __XLB_ACTION_MANAGER_H__
#define __XLB_ACTION_MANAGER_H__

#include <map>
#include <vector>

#include "xlbe.h"

#include "scene/scene_node.h"
#include "action.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Xlbe_Export Action_Manager 
{
public:
    struct Action_Element : private std::vector<Action*>
    {
        typedef std::vector<Action*> Base;
        using Base::push_back;
        using Base::begin;
        using Base::end;
        using Base::iterator;
        using Base::erase;
        using Base::size;

        bool pause_;
        Scene_Node* targe_;

        ~Action_Element();
    };

    typedef std::map<Scene_Node*, Action_Element*> Elements;

public:
    Action_Manager();
    virtual ~Action_Manager();

public:
    void update(float dt);

    void pause_target(Scene_Node *target);
    void resume_targer(Scene_Node *target);

    int number_of_running_actions_in_target(Scene_Node *target);

    Action *action_by_tag(int tag, Scene_Node *target);
    void add_action(Action *action, Scene_Node *target, bool pause);

    void remove_all_action_from_target(Scene_Node *target) ;
    void remove_action_by_tag(int tag, Scene_Node *target) ;

private:
    Elements elements_;
};

///////////////////////////////////////////////////////////////////
//}
#endif