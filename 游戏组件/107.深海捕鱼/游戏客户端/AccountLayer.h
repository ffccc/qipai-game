#ifndef __FISH_ACCOUNT_LAYER_H__
#define __FISH_ACCOUNT_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"
#include "GameCore.h"

#include "widget/button_widget.h"
#include "widget/slider_widget.h"

/////////////////////////////////////////////////////////////////
class CAccountLayer : public Layer, public Button_Listener
{
public:
    CAccountLayer();
    virtual ~CAccountLayer ();

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

    void DrawNumber(std::string &number, const Point &pt);
    void DrawTimer(std::string &number, const Point &pt);

    void ShowWidnow(bool bShow);
    void DisableWindow(bool bDisable);

private:
    Font *m_Font;

    Button_Widget *m_btnAccountAndExit;
    Button_Widget *m_btnAccount;
    Button_Widget *m_btnClose;

    Sprite* m_sprBackground;
    Image* m_imgNumber[10];
    Image* m_imgTimer[10];

    DWORD m_dwStartTime;				

public:
    std::vector<int> m_CaptureFishs;


};

//////////////////////////////////////////////////////////////////////////
#endif