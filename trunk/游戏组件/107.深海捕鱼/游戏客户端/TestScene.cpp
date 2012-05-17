#include "TestScene.h"

#include "FishApplication.h"

/////////////////////////////////////////////////////////////
CTestScene::CTestScene()
{
    m_layTest = new CTestLayer;
    m_layTest->set_position(Point(0,0));
	  m_layTest->set_content_size(Size(1280,738));
    add_child(m_layTest);


}

CTestScene::~CTestScene()
{
}

void CTestScene::update(float dt)
{
    __super::update(dt);

}



