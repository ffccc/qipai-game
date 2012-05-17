#ifndef __FISH_FRAME_LAYER_H__
#define __FISH_FRAME_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"
#include "GameCore.h"

#include "widget/button_widget.h"

/////////////////////////////////////////////////////////////////
class CFrameLayer : public Layer, public Button_Listener
{
public:
    CFrameLayer(int nScene);
    virtual ~CFrameLayer ();

    virtual void update(float dt);
    virtual void draw();

public:
	virtual void button_press(int tag);
    void add_widget(Node *node);

private:
    Image* image_tl_;
    Image* image_tm_;
    Image* image_tr_;
    Image* image_ml_;
    Image* image_mr_;
    Image* image_bl_;
    Image* image_bm_;
    Image* image_br_;

    Font *m_Font;

    Button_Widget *m_btnClose;
    Button_Widget *m_btnMax;
    Button_Widget *m_btnMin;
    Button_Widget *m_btnHelp;
    Button_Widget *m_btnSetting;

    int m_nScene;
};

//////////////////////////////////////////////////////////////////////////
#endif