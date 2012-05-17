#include "TaskLayer.h"

#include <sstream>

////////////////////////////////////////////////////////////
CTaskLayer::CTaskLayer()
{	

}

CTaskLayer::~CTaskLayer()
{
}

void CTaskLayer::update(float dt)
{
    __super::update(dt);

}

void CTaskLayer::draw()
{
    __super::draw();
}

void CTaskLayer::enter()
{
   __super::enter();

}

void CTaskLayer::exit()
{
      __super::exit();
}

bool CTaskLayer::CommandTaskStart(WORD wChairID)
{
    m_sprBarEnough->set_position(Point(500,400));

    Animation *aniBarEnough = Root::instance()->animation_manager()->animation("bar_enough");
    Action *actBarEnough = new Action_Sequence(new Action_Animation(0.06, aniBarEnough, false), new Action_Func(rak::mem_fn(this, &CTaskLayer::BarEnoughEnd)), 0);

    m_sprBarEnough->run_action(actBarEnough);

    return true;
}

bool CTaskLayer::BarEnoughEnd(Node *node, int tag)
{
    return true;
}


////////////////////////////////////////////////////////////