#ifndef __XLB_ACTION_MANAGER_H__
#define __XLB_ACTION_MANAGER_H__

#include <map>
#include <vector>

#include "prereqs.h"

#include "action.h"
#include "node.h"

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
        using Base::empty;

        bool pause_;
        Node* targe_;

        ~Action_Element();
    };

    typedef std::map<Node*, Action_Element*> Elements;

public:
    Action_Manager();
    virtual ~Action_Manager();

public:
    void update(float dt);

    void pause_target(Node *target);
    void resume_targer(Node *target);

    int number_of_running_actions_in_target(Node *target);

    Action *action_by_tag(int tag, Node *target);
    void add_action(Action *action, Node *target, bool pause);

    void remove_all_action_from_target(Node *target) ;
    void remove_action_by_tag(int tag, Node *target) ;

    void stop_all_action_from_target(Node *target);
    void stop_action_by_tag(int tag, Node *target);

private:
    Elements elements_;
};

///////////////////////////////////////////////////////////////////
//}
#endif