#ifndef __XLBE_SCENE_H__
#define __XLBE_SCENE_H__

#include "xlbe.h"
#include "scene_node.h"

#include "frame/input_manager.h"

//////////////////////////////////////////////////////////////
//namespace xlbe {

//////////////////////////////////////////////////////////////
class Xlbe_Export Scene : public Scene_Node,
                          public Input_Listener
{
public:
    Scene();
    virtual ~Scene();

public:
    virtual bool keyPressed( const OIS::KeyEvent &arg ) { return true; }
    virtual bool keyReleased( const OIS::KeyEvent &arg ) { return true; }

    virtual bool mouseMoved( const OIS::MouseEvent &arg ) { return true; }
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) { return true; }
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) { return true; }
};

//////////////////////////////////////////////////////////////
//}
#endif