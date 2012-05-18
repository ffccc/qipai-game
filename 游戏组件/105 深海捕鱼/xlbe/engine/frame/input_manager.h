#ifndef __XLB_INPUT_MANAGER_H__
#define __XLB_INPUT_MANAGER_H__

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "xlbe.h"

///////////////////////////////////////////////////////////////////
//namespace xlbe {

///////////////////////////////////////////////////////////////////
class Input_Listener
{
public:
    virtual bool keyPressed( const OIS::KeyEvent &arg ) =0 ;
    virtual bool keyReleased( const OIS::KeyEvent &arg ) =0 ;

    virtual bool mouseMoved( const OIS::MouseEvent &arg ) =0 ;
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) =0 ;
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) =0 ;
};

///////////////////////////////////////////////////////////////////
class Xlbe_Export Input_Manager :  public OIS::KeyListener,
    public OIS::MouseListener
{
public:
    typedef std::vector<Input_Listener *> Input_Listeners;

public:
    Input_Manager();
    virtual ~Input_Manager();

public:
    void initialise();

    void add_input_listener(Input_Listener * listener);
    void remove_input_listener(Input_Listener * listener);

    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    OIS::InputManager* input_manager() { return input_manager_; }
    OIS::Mouse* mouse()                { return mouse_; }
    OIS::Keyboard* keyboard()          { return keyboard_; }

private:
    OIS::InputManager* input_manager_;
    OIS::Mouse* mouse_;
    OIS::Keyboard* keyboard_;

    Input_Listeners input_listeners_;

};

///////////////////////////////////////////////////////////////////
//}
#endif