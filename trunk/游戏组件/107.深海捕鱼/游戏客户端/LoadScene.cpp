#include "LoadScene.h"

#include "FishApplication.h"

/////////////////////////////////////////////////////////////
CLoadScene::CLoadScene()
{
    m_layLoad = new CLoadLayer;
    m_layLoad->set_position(Point(4,31));
	  m_layLoad->set_content_size(Size(1272,703));
    add_child(m_layLoad);

     m_widgetProgress = new Progress_Widget(Root::instance()->imageset_manager()->imageset("ui_load")->image("progress_left"),
        Root::instance()->imageset_manager()->imageset("ui_load")->image("progress_left"),
        Root::instance()->imageset_manager()->imageset("ui_load")->image("progress_left"),
        Root::instance()->imageset_manager()->imageset("ui_load")->image("progress_step"));
    m_widgetProgress->set_tag(10001);
    m_widgetProgress->set_position(Point(462, 446));
    m_widgetProgress->set_content_size(Size(360,11));
    m_widgetProgress->set_pos_range(0);
    add_child(m_widgetProgress);

    m_layFrame = new CFrameLayer(0);
    m_layFrame->set_position(Point(0,0));
    m_layFrame->set_content_size(Size(1280, 738)); 
    m_layFrame->set_disable(true);
    m_layFrame->add_widget(this);

    m_layCursor = new CCursorLayer;
    m_layCursor->set_position(Point(0,0));
    m_layCursor->set_content_size(Size(80, 80));
    m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("ui_load")->image("cursor"));
    add_child(m_layCursor);
}

CLoadScene::~CLoadScene()
{
}

void CLoadScene::update(float dt)
{
    __super::update(dt);

    m_widgetProgress->set_pos_range(theApp->loading_thread_progress()*100);
}

bool CLoadScene::mouseMoved( const OIS::MouseEvent &arg ) 
{
    Point pt(arg.state.X.abs, arg.state.Y.abs);

    m_layCursor->set_position(pt);

     return __super::mouseMoved(arg);
}


