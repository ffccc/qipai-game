#ifndef __XLBE_SCENE_H__
#define __XLBE_SCENE_H__

#include "prereqs.h"
#include "input_manager.h"
#include "node.h"

//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
class Xlbe_Export Scene : public Node, public Input_Listener
{
public:
    Scene();
    virtual ~Scene();

public:
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    virtual bool mouseMoved( const OIS::MouseEvent &arg ) ;
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) ;
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) ; 

public:
    Node *node_by_position(const Point &mouse, Point &pos);
    void mouse_position(const Point &mouse);
    void set_focus(Node *node);
    void mouse_drag(const Point &mouse);

protected:
    Node *last_down_node_;
    Node *over_node_;
    Node *focus_node_;

    Point last_mouse_;
    bool mouse_down_;
    bool key_down_[0xFF];
};

//////////////////////////////////////////////////////////////
//}
#endif