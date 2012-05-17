#include "LoadLayer.h"

////////////////////////////////////////////////////////////
CLoadLayer::CLoadLayer()
:m_sprBg(0),
 m_Font(0),
 m_strHelps(4)
{	
    m_sprBg = new Sprite(Root::instance()->imageset_manager()->imageset("bg_load")->image("bg"));
    m_sprBg->set_hot(Point(0,0));
    add_child(m_sprBg);

    m_Font = Root::instance()->font_manager()->create_font_ttf("simsun", "fish\\simsun.ttc");
    m_Font->set_size(Size(15,16));

    m_strDateLoading = "����������.";
    m_strHelps[0] = "��ӭ����������֡�";
    m_strHelps[1] = "���¼��ɻ��ڣ����Ҽ��ɸı��ڷ��򣬿ո�ɷ���Ŷ��";
    m_strHelps[2] = "�����������ͺ���ը����Ҫ����������";
    m_strHelps[3] = "��Ϸ�ڰ���ֻ�дӴ�������Ż���֡�";

    m_strHelp = m_strHelps[rand()%4];
}

CLoadLayer::~CLoadLayer()
{
}

void CLoadLayer::update(float dt)
{
    __super::update(dt);

    if (update_count_%75 == 0)
    {
        m_strDateLoading = "����������.";
    }
    else if (update_count_%75 == 25)
    {
        m_strDateLoading = "����������..";
    }
    else if (update_count_%75 == 50)
    {
        m_strDateLoading = "����������...";
    }

}

void CLoadLayer::draw()
{
    __super::draw();

    m_Font->set_align(1);
    m_Font->draw_string(Point(640,510), m_strDateLoading, Color());

    m_Font->set_align(1);
    m_Font->draw_string(Point(640,710), m_strHelp, Color(176,222,238));
}

////////////////////////////////////////////////////////////