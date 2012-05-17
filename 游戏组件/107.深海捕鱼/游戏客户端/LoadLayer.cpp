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

    m_strDateLoading = "数据载入中.";
    m_strHelps[0] = "欢迎进入猎鱼高手。";
    m_strHelps[1] = "上下键可换炮，左右键可改变炮方向，空格可发射哦。";
    m_strHelps[2] = "脉冲光炮任务和海底炸弹需要触法引爆。";
    m_strHelps[3] = "游戏内帮助只有从大厅进入才会出现。";

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
        m_strDateLoading = "数据载入中.";
    }
    else if (update_count_%75 == 25)
    {
        m_strDateLoading = "数据载入中..";
    }
    else if (update_count_%75 == 50)
    {
        m_strDateLoading = "数据载入中...";
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