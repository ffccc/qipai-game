#include "TestLayer.h"

#include <sstream>

////////////////////////////////////////////////////////////
CTestLayer::CTestLayer()
{	
    std::ostringstream ostr;
    ostr << "fish_" << 2;

    m_sprFish = new Sprite(Root::instance()->imageset_manager()->imageset(ostr.str())->image("0"));
    add_child(m_sprFish);

    m_bChange = false;


}

CTestLayer::~CTestLayer()
{
}

void CTestLayer::update(float dt)
{
    __super::update(dt);

    //if (m_bChange)
    //{
    //    m_bChange = false;
    //    m_nIndex++;

    //    std::ostringstream ostr;
    //    ostr << "fish\\path\\small\\" << m_nIndex << ".dat";

    //    Action *act = new Action_Sequence( new Action_Key_Frame(0.025, ostr.str()),  new Action_Func(rak::mem_fn(this, &CTestLayer::FishEnd)), 0);
    //    m_sprFish->run_action(act);
    //}

}

void CTestLayer::draw()
{
    __super::draw();

}

void CTestLayer::enter()
{
      __super::enter();

    //  m_nIndex = 2001;
    //  m_bChange = false;

    //        std::ostringstream ostr;
    //        ostr << "fish\\path\\small\\" << m_nIndex << ".dat";

    //Action *act = new Action_Sequence( new Action_Key_Frame(0.025, ostr.str()),  new Action_Func(rak::mem_fn(this, &CTestLayer::FishEnd)), 0);
    //m_sprFish->run_action(act);
}

void CTestLayer::exit()
{
      __super::exit();
}

bool CTestLayer::FishEnd(Node *node, int tag)
{

   //m_bChange = true;
    return true;
}
////////////////////////////////////////////////////////////