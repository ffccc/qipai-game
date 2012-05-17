#ifndef __FISH_ME_USER_INFO_LAYER_H__
#define __FISH_ME_USER_INFO_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"
#include "GameCore.h"

#include "widget/button_widget.h"
#include "widget/slider_widget.h"

/////////////////////////////////////////////////////////////////
class CMeUserInfoLayer : public Layer, public Button_Listener
{
public:
    CMeUserInfoLayer();
    virtual ~CMeUserInfoLayer ();

    virtual void update(float dt);
    virtual void draw();

    /*virtual void mouse_move(const Point &pt);*/
    /*virtual void mouse_drag(const Point &pt);*/
    //virtual void mouse_wheel(int delta);
    /*virtual void mouse_down(const Point &pt, int num, int count);
    virtual void mouse_up(const Point &pt, int num, int count);*/

    virtual void resize(const Point &pt, const Size &size); 

public:
	  virtual void button_press(int tag);
    void add_widget(Node *node);

private:
    Font *m_Font;

    Button_Widget *m_btnAccount;
    Button_Widget *m_btnBullet;
    Button_Widget *m_btnDown;
    Button_Widget *m_btnUp;

    Sprite* m_sprBackground;


};

//////////////////////////////////////////////////////////////////////////
#endif