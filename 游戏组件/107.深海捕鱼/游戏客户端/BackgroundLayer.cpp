#include "BackgroundLayer.h"

#include<sstream>

////////////////////////////////////////////////////////////
CBackgroundLayer::CBackgroundLayer()
:m_SceneType(CGameCore::SceneTypeCount),
m_imgBackground0(0),
m_imgBackground1(0)
{	
    m_sprTideWater = new Sprite(Root::instance()->imageset_manager()->imageset("ui_game")->image("tide_water"));
    m_sprTideWater->set_hot(Point(0,0));
    m_sprTideWater->set_position(Point(1272-20, 0));
    m_sprTideWater->set_visible(false);
    add_child(m_sprTideWater);

    
    for (int i=0; i<4; i++)
    {
		try
		{
			m_BackSound[i] = Root::instance()->music_manager()->sound_instance(i);
		}
		catch(...)
		{
			m_BackSound[i]  = 0;
		}

	}

}

CBackgroundLayer::~CBackgroundLayer()
{
}

void CBackgroundLayer::SetSceneType(CGameCore::enSceneType SceneType)
{
    if (SceneType == CGameCore::SceneTypeCount)
        return ;

    //if (SceneType == m_SceneType)
    //    return;

    m_SceneType = SceneType;

    std::ostringstream ostr;
    ostr << "bg_game_" << (int)SceneType;

    m_imgBackground0 = Root::instance()->imageset_manager()->imageset(ostr.str())->image("bg");
    m_imgBackground1 = 0;

    m_sprTideWater->set_position(Point(1272-20, 0));
    m_sprTideWater->set_visible(false);

	if (m_BackSound[m_SceneType])
        m_BackSound[m_SceneType]->play(true, false);

}

void CBackgroundLayer::ChangeSceneType(CGameCore::enSceneType SceneType)
{
    if (SceneType == m_SceneType)
        return ;

	if (m_BackSound[m_SceneType])
        m_BackSound[m_SceneType]->stop();

    try
    {
        Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(4);
        pSound->play(false, true);
    }
    catch(...)
    {
    }

    m_SceneType = SceneType;

    std::ostringstream ostr;
    ostr << "bg_game_" << (int)SceneType;

    m_imgBackground1 = Root::instance()->imageset_manager()->imageset(ostr.str())->image("bg");

    m_sprTideWater->set_position(Point(1272-20, 0));
    m_sprTideWater->set_visible(true);

    Action *actWater = new Action_Sequence(new Action_Delay(1), new Action_Move_To(2.4135, Point(-283, 0)),  new Action_Func(rak::mem_fn(this, &CBackgroundLayer::ChangeSceneEnd)), 0);
    m_sprTideWater->run_action(actWater);
}

bool CBackgroundLayer::ChangeSceneEnd(Node *node, int tag)
{
    SetSceneType(m_SceneType);

    return true;
}

void CBackgroundLayer::update(float dt)
{
    __super::update(dt);
}

void CBackgroundLayer::draw()
{
    Point  pt = m_sprTideWater->position();

    if (pt.x_ < -20)
    {
        pt.x_ = -20;
    }

    if (m_imgBackground0)
    {
        m_imgBackground0->draw(position());
    }
    
    if (m_imgBackground1)
    {
        m_imgBackground1->draw(pt+position()+Point(20,0), Size(1272-pt.x_+20, m_imgBackground1->height()), pt+Point(20,0), Size(1272-pt.x_+20, m_imgBackground1->height()));
    }
    
    __super::draw(); 
}

////////////////////////////////////////////////////////////