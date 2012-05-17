#ifndef __FISH_BUY_BULLET_LAYER_H__
#define __FISH_BUY_BULLET_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"
#include "GameCore.h"

#include "widget/button_widget.h"
#include "widget/slider_widget.h"

/////////////////////////////////////////////////////////////////
class CBuyBulletLayer : public Layer, public Button_Listener, public Slider_Listener
{
public:
    CBuyBulletLayer();
    virtual ~CBuyBulletLayer ();

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

public:
    void ShowWidnow(bool bShow);
    void SetFishGold(int nFishGold);
    void SetGameGold(int nGameGold);
    void SetCellScore(LONG lCellScore); 

    LONG GetCellScore() const { return m_lCellScore; }

    bool IsSendBuyBulletMessage() const { return m_bSendMessage; }
    void SetSendBuyBulletMessage(bool bSend) { m_bSendMessage = bSend; }

private:
    

    /*Font *m_Font;*/

    Button_Widget *m_btnCancle;
    Button_Widget *m_btnMax;
    Button_Widget *m_btnOk;
    Button_Widget *m_btnMinus;
    Button_Widget *m_btnAdd;

    Slider_Widget *m_sldNumber;

    Sprite* m_sprBackground;
    std::vector<Sprite *> m_sprFishGold;
    std::vector<Sprite *> m_sprGameGold;

    int m_nFishGold;
    int m_nGameGold;

    bool m_bDragging;
    Point m_ptDragMouse;

    LONG m_lCellScore;
    bool m_bSendMessage;
};

//////////////////////////////////////////////////////////////////////////
#endif