#include "BuyBulletLayer.h"
#include "ClientKernel.h"
#include "FishApplication.h"
#include "GameScene.h"

#include <sstream>
////////////////////////////////////////////////////////////
CBuyBulletLayer::CBuyBulletLayer()
:m_sprFishGold(4),
m_sprGameGold(7),
m_nFishGold(0),
m_nGameGold(0),
m_bDragging(false),
m_lCellScore(1),
m_bSendMessage(false)
{	
    //m_Font = Root::instance()->font_manager()->create_font_ttf("simsun", "fish\\simsun.ttc");
    //m_Font->set_size(Size(14,15));

    m_btnCancle = new Button_Widget(Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_cancle_image"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_cancle_over"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_cancle_down"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_cancle_disable"));
    m_btnCancle->set_tag(10010);
    m_btnCancle->set_position(Point(388+56, 257+160));
    m_btnCancle->set_content_size(Size(104,52));
    m_btnCancle->add_listener(this);

    m_btnMax = new Button_Widget(Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_max_image"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_max_over"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_max_down"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_max_disable"));
    m_btnMax->set_tag(10011);
    m_btnMax->set_position(Point(388+350, 257+160));
    m_btnMax->set_content_size(Size(104,52));
    m_btnMax->add_listener(this);

    m_btnOk = new Button_Widget(Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_ok_image"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_ok_over"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_ok_down"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_ok_disable"));
    m_btnOk->set_tag(10012);
    m_btnOk->set_position(Point(388+180, 257+160));
    m_btnOk->set_content_size(Size(148,52));
    m_btnOk->add_listener(this);

   m_btnMinus = new Button_Widget(Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_minus_image"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_minus_over"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_minus_down"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_minus_disable"));
    m_btnMinus->set_tag(10013);
    m_btnMinus->set_position(Point(388+20, 257+110));
    m_btnMinus->set_content_size(Size(42,42));
    m_btnMinus->add_listener(this);

    m_btnAdd= new Button_Widget(Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_add_image"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_add_over"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_add_down"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_add_disable"));
    m_btnAdd->set_tag(10014);
    m_btnAdd->set_position(Point(388+442, 257+110));
    m_btnAdd->set_content_size(Size(42,42));
    m_btnAdd->add_listener(this);

    m_sldNumber= new Slider_Widget(0,
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_slider_thumb"),
        Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_slider_image"));
    m_sldNumber->set_tag(10015);
    m_sldNumber->set_position(Point(388+70, 257+110));
    m_sldNumber->set_content_size(Size(370,45));
    m_sldNumber->add_listener(this);

    m_sprBackground = new Sprite(Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_bullet_bg"));
    m_sprBackground->set_hot(Point(0,0));
    add_child(m_sprBackground);

    for (int i=0; i<4; i++)
    {
        m_sprFishGold[i] = new Sprite();
        add_child(m_sprFishGold[i]);
    }

    for (int i=0; i<7; i++)
    {
        m_sprGameGold[i] = new Sprite();
        add_child(m_sprGameGold[i]);
    }

    m_sprFishGold[0]->set_position(Point(98,76));
    m_sprFishGold[1]->set_position(Point(98+20,76));
    m_sprFishGold[2]->set_position(Point(98+40,76));
    m_sprFishGold[3]->set_position(Point(98+60,76));

    m_sprGameGold[0]->set_position(Point(290,76));
    m_sprGameGold[1]->set_position(Point(290+20,76));
    m_sprGameGold[2]->set_position(Point(290+40,76));
    m_sprGameGold[3]->set_position(Point(290+60,76));
    m_sprGameGold[4]->set_position(Point(290+80,76));
    m_sprGameGold[5]->set_position(Point(290+100,76));
    m_sprGameGold[6]->set_position(Point(290+120,76));

    SetFishGold(100);
    SetGameGold(100*m_lCellScore);

}

CBuyBulletLayer::~CBuyBulletLayer()
{
}

void CBuyBulletLayer::update(float dt)
{
    __super::update(dt);

}

void CBuyBulletLayer::draw()
{
    __super::draw();
}

void CBuyBulletLayer::add_widget(Node *node)
{
    node->add_child(this);
    node->add_child(m_btnCancle);
    node->add_child(m_btnMax);
    node->add_child(m_btnOk);
    node->add_child(m_btnMinus);
    node->add_child(m_btnAdd);
    node->add_child(m_sldNumber);
}

void CBuyBulletLayer::ShowWidnow(bool bShow)
{
    bool bVisible = visible();
    if (bVisible == bShow)
        return ;

    set_visible(bShow);
    m_btnCancle->set_visible(bShow);
    m_btnMax->set_visible(bShow);
    m_btnOk->set_visible(bShow);
    m_btnMinus->set_visible(bShow);
    m_btnAdd->set_visible(bShow);
    m_sldNumber->set_visible(bShow);

    if (bShow)
    {
        try
        {
            Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(16);
            pSound->play(false, true);
        }
        catch(...)
        {
        }
  
    }
}

void CBuyBulletLayer::button_press(int tag)
{
    if (10013 == tag)
    {
        m_nFishGold -= 100;
        if (m_nFishGold<100)
        {
            m_nFishGold = 100;
        }

        SetFishGold(m_nFishGold);
        SetGameGold(m_nFishGold*m_lCellScore);
        m_sldNumber->set_value((double)(m_nFishGold-100)/400);
    }
    else if (10014 == tag)
    {
        m_nFishGold += 100;
        if (m_nFishGold>500)
        {
            m_nFishGold = 500;
        }

        SetFishGold(m_nFishGold);
        SetGameGold(m_nFishGold*m_lCellScore);
        m_sldNumber->set_value((double)(m_nFishGold-100)/400);
    }
    else if (10011 == tag)
    {
        m_nFishGold = 500;
       
        SetFishGold(m_nFishGold);
        SetGameGold(m_nFishGold*m_lCellScore);
        m_sldNumber->set_value((double)(m_nFishGold-100)/400);
    }
    else if (10010 == tag)
    {
        ShowWidnow(false);
    }
    else if (10012 == tag)
    {
        CMD_C_Buy_Bullet BuyBullet;
        BuyBullet.wCount = m_nFishGold;

        CGameScene *pGameScene = (CGameScene *)parent();
        pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_BUY_BULLET, &BuyBullet, sizeof(CMD_C_Buy_Bullet));
        m_bSendMessage = true;
        ShowWidnow(false);
    }
}

void CBuyBulletLayer::slider_value(int tag, double value)
{
    if (value>=0 && value<0.25)
        m_nFishGold = 100 ;
    else if (value>=0.25 && value<0.5)
        m_nFishGold = 200 ;
    else if (value>=0.5 && value<0.75)
        m_nFishGold = 300 ;
    else if (value>=0.75 && value<1.0)
        m_nFishGold = 400 ;
    else
        m_nFishGold = 500 ;


    SetFishGold(m_nFishGold);
    SetGameGold(m_nFishGold*m_lCellScore);
}

void CBuyBulletLayer::SetFishGold(int nFishGold)
{
   //if (nFishGold == m_nFishGold)
   //     return;

    m_nFishGold = nFishGold; 


    bool bGotHead = false;
    int nSingleNumber = 0;
    std::ostringstream ostr;
    nSingleNumber = (int)(m_nFishGold/1000);
    if (nSingleNumber>0)
    {
        bGotHead = true;
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprFishGold[0]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
    else
    {
        m_sprFishGold[0]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nFishGold%1000/100);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprFishGold[1]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
    else
    {
        m_sprFishGold[1]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nFishGold%100/10);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprFishGold[2]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
        else
    {
        m_sprFishGold[2]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nFishGold%10);
    ostr.str("");
    ostr << "buy_bullet_number_" << nSingleNumber;
    m_sprFishGold[3]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
}

void CBuyBulletLayer::SetGameGold(int nGameGold)
{
    //if (nGameGold == m_nGameGold)
    //    return;

    m_nGameGold = nGameGold; 

     bool bGotHead = false;
    int nSingleNumber = 0;
    std::ostringstream ostr;
    nSingleNumber = (int)(m_nGameGold/1000000);
    if (nSingleNumber>0)
    {
        bGotHead = true;
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprGameGold[0]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
    else
    {
        m_sprGameGold[0]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nGameGold%1000000/100000);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprGameGold[1]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
        else
    {
        m_sprGameGold[1]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nGameGold%100000/10000);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprGameGold[2]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
        else
    {
        m_sprGameGold[2]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nGameGold%10000/1000);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprGameGold[3]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
    else
    {
        m_sprGameGold[3]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nGameGold%1000/100);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprGameGold[4]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
    else
    {
        m_sprGameGold[4]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nGameGold%100/10);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << "buy_bullet_number_" << nSingleNumber;
        m_sprGameGold[5]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
    }
    else
    {
        m_sprGameGold[5]->set_display_image(0);
    }

    nSingleNumber = (int)(m_nGameGold%10/1);
    ostr.str("");
    ostr << "buy_bullet_number_" << nSingleNumber;
    m_sprGameGold[6]->set_display_image(Root::instance()->imageset_manager()->imageset("ui_game")->image(ostr.str()));
}

void CBuyBulletLayer::mouse_down(const Point &pt, int num, int count)
{
    m_bDragging = true;
		m_ptDragMouse = pt;

}

void CBuyBulletLayer::mouse_drag(const Point &pt)
{
    if (m_bDragging)
    {
        Point ptNew = position_ + pt - m_ptDragMouse;
        move(ptNew);
    }
}

void CBuyBulletLayer::mouse_up(const Point &pt, int num, int count)
{
    if (m_bDragging )
    {
        m_bDragging = false;
    }
}

void CBuyBulletLayer::resize(const Point &pt, const Size &size)
{
    __super::resize(pt, size);

    set_position(pt);
    m_btnCancle->set_position(Point(pt.x_+56, pt.y_+160));
    m_btnMax->set_position(Point(pt.x_+350, pt.y_+160));
    m_btnOk->set_position(Point(pt.x_+180, pt.y_+160));
    m_btnMinus->set_position(Point(pt.x_+20, pt.y_+110));
    m_btnAdd->set_position(Point(pt.x_+442, pt.y_+110));
    m_sldNumber->set_position(Point(pt.x_+70, pt.y_+110));

}

void CBuyBulletLayer::SetCellScore(LONG lCellScore)
{
    m_lCellScore = lCellScore;

    SetFishGold(m_nFishGold);
    SetGameGold(m_nFishGold*m_lCellScore);
}

void CBuyBulletLayer::mouse_enter()
{
  /* CGameScene *pGameScene = (CGameScene *)parent();
   pGameScene->m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("role")->image("cursor_hand"));
   */
}

void CBuyBulletLayer::mouse_leave()
{/*
   CGameScene *pGameScene = (CGameScene *)parent();
   pGameScene->m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("role")->image("cursor_fire"));*/
}

////////////////////////////////////////////////////////////