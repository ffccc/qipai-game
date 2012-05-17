#include "FrameLayer.h"
#include "ClientKernel.h"
#include "FishApplication.h"
#include "GameScene.h"
    
#include <sstream>
////////////////////////////////////////////////////////////
CFrameLayer::CFrameLayer(int nScene)
{	
    std::string ui;

    if (nScene == 0)
    {
        ui = "ui_load";
    }
    else
    {
        ui = "ui_game";
    }

    m_nScene = nScene;

    image_tl_ = Root::instance()->imageset_manager()->imageset(ui)->image("frame_tl_");
    image_tm_ = Root::instance()->imageset_manager()->imageset(ui)->image("frame_tm_");
    image_tr_ = Root::instance()->imageset_manager()->imageset(ui)->image("frame_tr_");
    image_ml_ = Root::instance()->imageset_manager()->imageset(ui)->image("frame_ml_");
    image_mr_ = Root::instance()->imageset_manager()->imageset(ui)->image("frame_mr_");
    image_bl_ = Root::instance()->imageset_manager()->imageset(ui)->image("frame_bl_");
    image_bm_ = Root::instance()->imageset_manager()->imageset(ui)->image("frame_bm_");
    image_br_ = Root::instance()->imageset_manager()->imageset(ui)->image("frame_br_");

    m_Font = Root::instance()->font_manager()->create_font_ttf("simsun", "fish\\simsun.ttc");
    m_Font->set_size(Size(14,15));

    m_btnClose = new Button_Widget(Root::instance()->imageset_manager()->imageset(ui)->image("btn_close_image"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_close_over"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_close_down"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_close_disable"));
    m_btnClose->set_tag(10000);
    m_btnClose->set_position(Point(1280-54, 4));
    m_btnClose->set_content_size(Size(41,24));
    m_btnClose->add_listener(this);

    m_btnMax = new Button_Widget(Root::instance()->imageset_manager()->imageset(ui)->image("btn_max_image"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_max_over"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_max_down"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_max_disable"));
    m_btnMax->set_tag(10001);
    m_btnMax->set_position(Point(1280-54-30, 4));
    m_btnMax->set_content_size(Size(29,24));
    m_btnMax->add_listener(this);

    m_btnMin = new Button_Widget(Root::instance()->imageset_manager()->imageset(ui)->image("btn_min_image"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_min_over"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_min_down"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_min_disable"));
    m_btnMin->set_tag(10002);
    m_btnMin->set_position(Point(1280-54-30-33, 4));
    m_btnMin->set_content_size(Size(32,24));
    m_btnMin->add_listener(this);

    m_btnHelp = new Button_Widget(Root::instance()->imageset_manager()->imageset(ui)->image("btn_help_image"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_help_over"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_help_down"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_help_disable"));
    m_btnHelp->set_tag(10003);
    m_btnHelp->set_position(Point(1092-43, 4));
    m_btnHelp->set_content_size(Size(42,24));
    m_btnHelp->add_listener(this); 

    m_btnSetting = new Button_Widget(Root::instance()->imageset_manager()->imageset(ui)->image("btn_setting_image"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_setting_over"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_setting_down"),
        Root::instance()->imageset_manager()->imageset(ui)->image("btn_setting_disable"));
    m_btnSetting->set_tag(10004);
    m_btnSetting->set_position(Point(1092, 4));
    m_btnSetting->set_content_size(Size(42,24));
    m_btnSetting->add_listener(this); 
}

CFrameLayer::~CFrameLayer()
{
}

void CFrameLayer::update(float dt)
{
    __super::update(dt);

}

void CFrameLayer::draw()
{
    __super::draw();

    if (!visible_)
        return;

    Point pt = position_absolute();
    Size size = content_size();

    Point pt_draw;
    Size size_draw;

    //top
    pt_draw = pt;
    image_tl_->draw(pt_draw);

    pt_draw = pt+Point(5, 0);
    size_draw.width_= size.width_ - 5;
    size_draw.height_ = 32;
    image_tm_->draw(pt_draw, size_draw);

    pt_draw = pt + Point(size.width_-248, 0);
    image_tr_->draw(pt_draw);

    //middle
    pt_draw = pt+Point(0, 32);
    size_draw.width_= 5;
    size_draw.height_ = size.height_-32;
    image_ml_->draw(pt_draw, size_draw);

    pt_draw = pt+Point(1280-5, 32);
    size_draw.width_= 5;
    size_draw.height_ = size.height_-32;
    image_mr_->draw(pt_draw, size_draw);

    //bottom
    pt_draw = pt+Point(0, size.height_-5);
    image_bl_->draw(pt_draw);

    pt_draw = pt_draw+Point(5, 0);
    size_draw.width_= size.width_ - 5;
    size_draw.height_ = 5;
    image_bm_->draw(pt_draw, size_draw);

    pt_draw = pt + Point(size.width_-5, size.height_-5);
    image_br_->draw(pt_draw);

    char szWindowText[64];
    GetWindowText(Root::instance()->render_window()->window_handle(), szWindowText, 64);

    m_Font->set_align(0);
    m_Font->draw_string(Point(20,20), szWindowText, Color(252,187,123));

}

void CFrameLayer::add_widget(Node *node)
{
    node->add_child(this);
    node->add_child(m_btnClose);
    node->add_child(m_btnMax);
    node->add_child(m_btnMin);
    node->add_child(m_btnHelp);
    node->add_child(m_btnSetting);
}

void CFrameLayer::button_press(int tag)
{
	if (tag == 10000)
	{
      if (m_nScene > 0)
      {
          CGameScene *pGameScene = (CGameScene *)parent();
          if (!pGameScene->m_layAccount->visible())
          {
              pGameScene->m_layBuyBulletLayer->ShowWidnow(false);
              pGameScene->m_layAccount->ShowWidnow(true);
          }
/*           ((CGameScene*)parent())->window_closed(0);
           Root::instance()->queue_end_rendering();	*/	
      }
      else
      {
/*          theApp->stop_loading_thread();
          Root::instance()->queue_end_rendering();	*/	
      }

		   
	}
  else if (tag == 10002)
  {
      ShowWindow(Root::instance()->render_window()->window_handle(), SW_MINIMIZE);
  }
  else if (tag == 10004)
  {
	  if (m_nScene > 0)
      {
		  CGameScene *pGameScene = (CGameScene *)parent();
          if (!pGameScene->m_laySetting->visible())
          {
              pGameScene->m_laySetting->ShowWidnow(true);
          }
	  }

  }
	
}
////////////////////////////////////////////////////////////