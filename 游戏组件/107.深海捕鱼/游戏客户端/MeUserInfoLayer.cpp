#include "MeUserInfoLayer.h"
#include "ClientKernel.h"
#include "FishApplication.h"
#include "GameScene.h"

#include <sstream>
////////////////////////////////////////////////////////////
CMeUserInfoLayer::CMeUserInfoLayer()
{	
    m_Font = Root::instance()->font_manager()->create_font_ttf("simsun", "fish\\simsun.ttc");
    m_Font->set_size(Size(14,15));

    m_btnAccount = new Button_Widget(Root::instance()->imageset_manager()->imageset("role")->image("btn_account_image"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_account_over"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_account_down"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_account_disable"));
    m_btnAccount->set_tag(10100);
    m_btnAccount->set_position(Point(514,560));
    m_btnAccount->set_content_size(Size(76,27));
    m_btnAccount->add_listener(this);

    m_btnBullet = new Button_Widget(Root::instance()->imageset_manager()->imageset("role")->image("btn_buy_image"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_buy_over"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_buy_down"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_buy_disable"));
    m_btnBullet->set_tag(10101);
    m_btnBullet->set_position(Point(514,560));
    m_btnBullet->set_content_size(Size(76,27));
    m_btnBullet->add_listener(this);

    m_btnDown = new Button_Widget(Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_down_image"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_down_over"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_down_down"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_down_disable"));
    m_btnDown->set_tag(10102);
    m_btnDown->set_position(Point(514,560));
    m_btnDown->set_content_size(Size(86,15));
    m_btnDown->set_visible(false);
    m_btnDown->add_listener(this);

    m_btnUp = new Button_Widget(Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_up_image"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_up_over"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_up_down"),
        Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_up_disable"));
    m_btnUp->set_tag(10103);
    m_btnUp->set_position(Point(514,560));
    m_btnUp->set_content_size(Size(86,15));
    m_btnUp->add_listener(this);

    m_sprBackground = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("btn_me_info_image"));
    m_sprBackground->set_hot(Point(0,0));
    add_child(m_sprBackground);

    m_Font = Root::instance()->font_manager()->create_font_ttf("simsun", "fish\\simsun.ttc");
    m_Font->set_size(Size(15,16));
}

CMeUserInfoLayer::~CMeUserInfoLayer()
{
}

void CMeUserInfoLayer::update(float dt)
{
    __super::update(dt);

}

void CMeUserInfoLayer::draw()
{
    __super::draw();

    CGameScene *pGameScene = (CGameScene *)parent();
    CClientKernel *pClientKernel = pGameScene->GetClientKernel();
    if ( !pClientKernel)
        return ;

    const tagUserData * pUserData = pClientKernel->GetMeUserInfo();

    if (pUserData)
    {
        Point pt = position_absolute();
        pt.x_ += 56;
        pt.y_ += 52;

        m_Font->draw_string(pt, pUserData->szName, Color(14,236,212));

        std::string szRank;
        if (pUserData->lScore>0 && pUserData->lScore<1000)
        {
            szRank = "Óæ·ò";
        }
        else
        {
            szRank = "´¬³¤";
        }

        pt.x_ += 160;
        m_Font->draw_string(pt, szRank.c_str(), Color(14,236,212));

        pt.x_ -= 160;
        pt.y_ += 28;

        std::ostringstream ostr;
        ostr << pUserData->lScore ;

        m_Font->draw_string(pt, ostr.str().c_str(), Color(14,236,212));


    }
}

void CMeUserInfoLayer::add_widget(Node *node)
{
    node->add_child(this);
    node->add_child(m_btnAccount);
    node->add_child(m_btnBullet);
    node->add_child(m_btnDown);
    node->add_child(m_btnUp);
}

//void CMeUserInfoLayer::ShowWidnow(bool bShow)
//{
//    set_visible(bShow);
//    m_btnAccount->set_visible(bShow);
//    m_btnBullet->set_visible(bShow);
//}

void CMeUserInfoLayer::button_press(int tag)
{
    if (tag == 10102)
    {
        resize(Point(514,560+148), Size(253,173));
        m_btnDown->set_visible(false);
        m_btnUp->set_visible(true);
    }
    else if (tag == 10103)
    {
        resize(Point(514,560), Size(253,173));
        m_btnDown->set_visible(true);
        m_btnUp->set_visible(false);
    }
    else if (tag == 10100)
    {
        CGameScene *pGameScene = (CGameScene *)parent();
        if (!pGameScene->m_layAccount->visible())
        {
            pGameScene->m_layBuyBulletLayer->ShowWidnow(false);
            pGameScene->m_layAccount->ShowWidnow(true);

            resize(Point(514,560+148), Size(253,173));
            m_btnDown->set_visible(false);
            m_btnUp->set_visible(true);
        }
    }
    else if (tag == 10101)
    {
        CGameScene *pGameScene = (CGameScene *)parent();
        int nFishGold = pGameScene->m_layRoles[pGameScene->GetMeChairID()]->GetFishGold() ;
        if (nFishGold <= 0 && !pGameScene->m_layAccount->visible())
        {
            if (!pGameScene->m_layBuyBulletLayer->IsSendBuyBulletMessage() )
            {
                pGameScene->m_layBuyBulletLayer->ShowWidnow(true);
                resize(Point(514,560+148), Size(253,173));
                m_btnDown->set_visible(false);
                m_btnUp->set_visible(true);
            }
        }
    }
}

void CMeUserInfoLayer::resize(const Point &pt, const Size &size)
{
    __super::resize(pt, size);

    set_position(pt);
    m_btnAccount->set_position(Point(pt.x_+34, pt.y_+140));
    m_btnBullet->set_position(Point(pt.x_+140, pt.y_+140));
    m_btnDown->set_position(Point(pt.x_+84, pt.y_+8));
    m_btnUp->set_position(Point(pt.x_+84, pt.y_+8));
}
////////////////////////////////////////////////////////////