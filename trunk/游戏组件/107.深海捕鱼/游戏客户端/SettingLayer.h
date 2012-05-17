#ifndef __FISH_SETTING_LAYER_H__
#define __FISH_SETTING_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"
#include "GameCore.h"

#include "widget/button_widget.h"
#include "widget/slider_widget.h"

/////////////////////////////////////////////////////////////////
class CSettingLayer : public Layer, public Button_Listener, public Slider_Listener
{
public:
    CSettingLayer();
    virtual ~CSettingLayer ();

    virtual void update(float dt);
    virtual void draw();

    /*virtual void mouse_move(const Point &pt);*/
    virtual void mouse_drag(const Point &pt);
    //virtual void mouse_wheel(int delta);
    virtual void mouse_down(const Point &pt, int num, int count);
    virtual void mouse_up(const Point &pt, int num, int count);

    virtual void resize(const Point &pt, const Size &size); 
    virtual void mouse_enter();
    virtual void mouse_leave();
public:
	virtual void button_press(int tag);
	virtual void slider_value(int tag, double value);
	void add_widget(Node *node);
	void ShowWidnow(bool bShow);

private:
    Button_Widget *m_btnCancle;
    Button_Widget *m_btnOk;
    Button_Widget *m_btnMinus;
    Button_Widget *m_btnAdd;
    Slider_Widget *m_sldNumber;

    Button_Widget *m_btnMinus1;
    Button_Widget *m_btnAdd1;
    Slider_Widget *m_sldNumber1;

    Sprite* m_sprBackground;

    bool m_bDragging;
    Point m_ptDragMouse;
};

//////////////////////////////////////////////////////////////////////////
#endif