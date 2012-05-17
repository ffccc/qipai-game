#ifndef __XLBE_LAYER_H__
#define __XLBE_LAYER_H__

#include "prereqs.h"

#include "input_manager.h"
#include "node.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Layer : public Node
{		
public:
    Layer();
    virtual ~Layer();

public:
    virtual void key_char(char c);
    virtual void key_down(OIS::KeyCode key);
    virtual void key_up(OIS::KeyCode key);

    virtual bool wants_focus();
    virtual void got_focus();
    virtual void lost_focus();	

    virtual void mouse_enter();
    virtual void mouse_leave();

    virtual void mouse_move(const Point &pt);
    virtual void mouse_drag(const Point &pt);
    virtual void mouse_wheel(int delta);
    virtual void mouse_down(const Point &pt, int num, int count);
    virtual void mouse_up(const Point &pt, int num, int count);

    virtual void move(const Point &pt);
    virtual void resize(const Point &pt, const Size &size); 
};

///////////////////////////////////////////////////////////
//}


#endif