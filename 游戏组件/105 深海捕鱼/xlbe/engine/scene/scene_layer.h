#ifndef __XLBE_SCENE_LAYER_H__
#define __XLBE_SCENE_LAYER_H__

#include "xlbe.h"

#include "frame/input_manager.h"

#include "scene_node.h"

///////////////////////////////////////////////////////////
//namespace xlbe{

///////////////////////////////////////////////////////////
class Xlbe_Export Scene_Layer : public Scene_Node,
                                public Input_Listener
{		
public:
    Scene_Layer();
    virtual ~Scene_Layer();

public:
    virtual bool keyPressed( const OIS::KeyEvent &arg ) { return true; }
    virtual bool keyReleased( const OIS::KeyEvent &arg ) { return true; }

    virtual bool mouseMoved( const OIS::MouseEvent &arg ) { return true; }
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) { return true; }
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) { return true; }
};

///////////////////////////////////////////////////////////
//}


#endif