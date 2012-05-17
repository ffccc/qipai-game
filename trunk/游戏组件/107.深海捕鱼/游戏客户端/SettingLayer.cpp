#include "SettingLayer.h"
#include "ClientKernel.h"
#include "FishApplication.h"
#include "GameScene.h"

#include <sstream>
////////////////////////////////////////////////////////////
CSettingLayer::CSettingLayer()
:m_bDragging(false)
{	
    m_btnCancle = new Button_Widget(Root::instance()->imageset_manager()->imageset("setting")->image("btn_cancle_image"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_cancle_over"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_cancle_down"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_cancle_disable"));
    m_btnCancle->set_tag(11010);
    m_btnCancle->set_position(Point(388+56, 257+160));
    m_btnCancle->set_content_size(Size(104,52));
    m_btnCancle->add_listener(this);

    m_btnOk = new Button_Widget(Root::instance()->imageset_manager()->imageset("setting")->image("btn_ok_image"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_ok_over"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_ok_down"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_ok_disable"));
    m_btnOk->set_tag(11012);
    m_btnOk->set_position(Point(388+180, 257+160));
    m_btnOk->set_content_size(Size(104,52));
    m_btnOk->add_listener(this);

   m_btnMinus = new Button_Widget(Root::instance()->imageset_manager()->imageset("setting")->image("btn_minus_image"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_minus_over"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_minus_down"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_minus_disable"));
    m_btnMinus->set_tag(11013);
    m_btnMinus->set_position(Point(388+20, 257+110));
    m_btnMinus->set_content_size(Size(42,42));
    m_btnMinus->add_listener(this);

    m_btnAdd= new Button_Widget(Root::instance()->imageset_manager()->imageset("setting")->image("btn_add_image"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_add_over"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_add_down"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_add_disable"));
    m_btnAdd->set_tag(11014);
    m_btnAdd->set_position(Point(388+442, 257+110));
    m_btnAdd->set_content_size(Size(42,42));
    m_btnAdd->add_listener(this);

    m_sldNumber= new Slider_Widget(0,
        Root::instance()->imageset_manager()->imageset("setting")->image("slider_thumb"),
        Root::instance()->imageset_manager()->imageset("setting")->image("slider_image"));
    m_sldNumber->set_tag(11015);
    m_sldNumber->set_position(Point(388+70, 257+110));
    m_sldNumber->set_content_size(Size(372,45));
    m_sldNumber->add_listener(this);
	m_sldNumber->set_value(theApp->GetMusicVolume());

   m_btnMinus1 = new Button_Widget(Root::instance()->imageset_manager()->imageset("setting")->image("btn_minus_image"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_minus_over"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_minus_down"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_minus_disable"));
    m_btnMinus1->set_tag(11016);
    m_btnMinus1->set_position(Point(388+20, 257+110));
    m_btnMinus1->set_content_size(Size(42,42));
    m_btnMinus1->add_listener(this);

    m_btnAdd1= new Button_Widget(Root::instance()->imageset_manager()->imageset("setting")->image("btn_add_image"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_add_over"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_add_down"),
        Root::instance()->imageset_manager()->imageset("setting")->image("btn_add_disable"));
    m_btnAdd1->set_tag(11017);
    m_btnAdd1->set_position(Point(388+442, 257+110));
    m_btnAdd1->set_content_size(Size(42,42));
    m_btnAdd1->add_listener(this);

    m_sldNumber1= new Slider_Widget(0,
        Root::instance()->imageset_manager()->imageset("setting")->image("slider_thumb"),
        Root::instance()->imageset_manager()->imageset("setting")->image("slider_image"));
    m_sldNumber1->set_tag(11018);
    m_sldNumber1->set_position(Point(388+70, 257+110));
    m_sldNumber1->set_content_size(Size(372,45));
    m_sldNumber1->add_listener(this);
	m_sldNumber1->set_value(theApp->GetSoundVolume());


    m_sprBackground = new Sprite(Root::instance()->imageset_manager()->imageset("setting")->image("bg"));
    m_sprBackground->set_hot(Point(0,0));
    add_child(m_sprBackground);
}

CSettingLayer::~CSettingLayer()
{
}

void CSettingLayer::update(float dt)
{
    __super::update(dt);

}

void CSettingLayer::draw()
{
    __super::draw();
}

void CSettingLayer::add_widget(Node *node)
{
    node->add_child(this);
    node->add_child(m_btnCancle);
    node->add_child(m_btnOk);
    node->add_child(m_btnMinus);
    node->add_child(m_btnAdd);
    node->add_child(m_sldNumber);
    node->add_child(m_btnMinus1);
    node->add_child(m_btnAdd1);
    node->add_child(m_sldNumber1);

}

void CSettingLayer::ShowWidnow(bool bShow)
{
    bool bVisible = visible();
    if (bVisible == bShow)
        return ;

    set_visible(bShow);
    m_btnCancle->set_visible(bShow);
    m_btnOk->set_visible(bShow);
    m_btnMinus->set_visible(bShow);
    m_btnAdd->set_visible(bShow);
    m_sldNumber->set_visible(bShow);
    m_btnMinus1->set_visible(bShow);
    m_btnAdd1->set_visible(bShow);
    m_sldNumber1->set_visible(bShow);

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

void CSettingLayer::button_press(int tag)
{
	if (tag==11010 || tag==11012)
	{
         ShowWidnow(false);
	}
	else if (tag==11013)
	{
		m_sldNumber->set_value(m_sldNumber->value()-0.1);
		theApp->SetMusicVolume(m_sldNumber->value());
	}
	else if (tag==11014)
	{
		m_sldNumber->set_value(m_sldNumber->value()+0.1);
		theApp->SetMusicVolume(m_sldNumber->value());
	}
	else if (tag==11016)
	{
		m_sldNumber1->set_value(m_sldNumber1->value()-0.1);
		theApp->SetSoundVolume(m_sldNumber1->value());
	}
	else if (tag==11017)
	{
		m_sldNumber1->set_value(m_sldNumber1->value()+0.1);
		theApp->SetSoundVolume(m_sldNumber1->value());
	}
}

void CSettingLayer::slider_value(int tag, double value)
{
	if (tag == 11015)
	{
		theApp->SetMusicVolume(value);
	}
	else if (tag == 11018)
	{
		theApp->SetSoundVolume(value);
	}
}


void CSettingLayer::mouse_down(const Point &pt, int num, int count)
{
    m_bDragging = true;
	m_ptDragMouse = pt;

}

void CSettingLayer::mouse_drag(const Point &pt)
{
    if (m_bDragging)
    {
        Point ptNew = position_ + pt - m_ptDragMouse;
        move(ptNew);
    }
}

void CSettingLayer::mouse_up(const Point &pt, int num, int count)
{
    if (m_bDragging )
    {
        m_bDragging = false;
    }
}

void CSettingLayer::resize(const Point &pt, const Size &size)
{
    __super::resize(pt, size);

    set_position(pt);
    m_btnCancle->set_position(Point(pt.x_+270, pt.y_+200));
    m_btnOk->set_position(Point(pt.x_+120, pt.y_+200));
    m_btnMinus->set_position(Point(pt.x_+20, pt.y_+80));
    m_btnAdd->set_position(Point(pt.x_+442, pt.y_+80));
    m_sldNumber->set_position(Point(pt.x_+66, pt.y_+80));
    m_btnMinus1->set_position(Point(pt.x_+20, pt.y_+134));
    m_btnAdd1->set_position(Point(pt.x_+442, pt.y_+134));
    m_sldNumber1->set_position(Point(pt.x_+66, pt.y_+134));
}

void CSettingLayer::mouse_enter()
{

}

void CSettingLayer::mouse_leave()
{
}

////////////////////////////////////////////////////////////