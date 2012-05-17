#include "RoleLayer.h"
#include "ClientKernel.h"
#include "GameScene.h"

#include <sstream>

class  Action_Move_Src_To : public Action_Interval
{
public:
    Action_Move_Src_To(float d, Point end, int danwei)
        :Action_Interval(d),
        end_(end), danwei_(danwei)
    {
    }
    virtual ~Action_Move_Src_To()
    {}

public:
    virtual void update(float time)
    {
        Point pt(start_.x_+delta_.x_*time, start_.y_+delta_.y_*time);

        int x = pt.x_;
        int y = pt.y_;

        y %= danwei_;
        //y += danwei_;
        //y %= danwei_;

        pt.x_ = x;
        pt.y_= y;

        target_->set_src_position(pt);
    }
    virtual void start_with_target(Node *target)
    {
        start_ = target->src_position();
        delta_ = end_ - start_;

        __super::start_with_target(target);
    }

protected:
    Point start_;
    Point end_;
    Point delta_;
    int danwei_;
};

class  Action_Move_Src_Acceleration_To : public Action_Interval
{
public:
    Action_Move_Src_Acceleration_To(float d, Point end, int danwei)
        :Action_Interval(d),
        end_(end), danwei_(danwei)
    {
    }
    virtual ~Action_Move_Src_Acceleration_To()
    {}

public:
    virtual void update(float time)
    {
        Point pt(start_.x_+delta_.x_*std::sqrt(time), start_.y_+delta_.y_*std::sqrt(time));

        int x = pt.x_;
        int y = pt.y_;

        y %= danwei_;
        y += danwei_;
        y %= danwei_;
        pt.x_ = x;
        pt.y_= y;

        target_->set_src_position(pt);
    }
    virtual void start_with_target(Node *target)
    {
        start_ = target->src_position();
        delta_ = end_ - start_;

        __super::start_with_target(target);
    }

protected:
    Point start_;
    Point end_;
    Point delta_;
    int danwei_;
};

////////////////////////////////////////////////////////////
CRoleLayer::CRoleLayer()
:m_wChairID(INVALID_CHAIR),
m_sprGold(5)
{	
    m_wChairID = INVALID_CHAIR;
    m_wFishGold = 0;
    m_nFireCount = 0;
    m_bDelayBarEnough = false;
    m_CannonType = CGameCore::CannonTypeCount; 
    m_bShowRewardNubmer = false;
    m_nRewardNumber = 0;
    m_nCookFish = 0;
    m_dwCookStartTime = 0;

    m_sprLaserBean = new Sprite(Root::instance()->imageset_manager()->imageset("task")->image("laserUp"));
    add_child(m_sprLaserBean);
    m_sprLaserBean->set_visible(false);

    m_sprBombFuse  = new Sprite;
    add_child(m_sprBombFuse);
    m_sprBombFuse->set_scale(Size(1.0, 1.9));
    m_sprBombFuse->set_visible(false);

    m_sprGunBase = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("gun_base"));
    m_sprGunOver = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("gun_over"));
    m_sprGunWing1 = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("gun_wing1"));
    m_sprGunWing2 = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("gun_wing2"));

    m_sprGun = new Sprite;
    m_sprGunLight = new Sprite;

    m_sprShotNumBack = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("shot_num_back"));
    m_sprNet = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("net_0"));
    m_sprShotGold = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("gold_0"));

    add_child(m_sprGunWing1);
    add_child(m_sprGunWing2);
    add_child(m_sprGunBase);

    add_child(m_sprGunLight);
    add_child(m_sprGun);

    add_child(m_sprGunOver);

    add_child(m_sprShotNumBack);
    add_child(m_sprNet);
    add_child(m_sprShotGold);

    for (int i=0; i<5; i++)
    {
        m_sprGold[i] = new Sprite();
        add_child(m_sprGold[i]);
    }

    m_sprCannon = new Sprite;
    add_child(m_sprCannon);

    m_sprUserInfo = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("btn_other_info_image"));
    add_child(m_sprUserInfo);
    m_sprUserInfo->set_visible(false);

    m_sprWaringBullet  = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("warning_bullet"));
    add_child(m_sprWaringBullet);
    m_sprWaringBullet->set_visible(false);


    m_Font = Root::instance()->font_manager()->create_font_ttf("simsun", "fish\\simsun.ttc");
    m_Font->set_size(Size(15,16));

    m_sprGunLead = new Sprite(Root::instance()->imageset_manager()->imageset("ui_game")->image("gun_lead"));
    m_sprBuyLead = new Sprite(Root::instance()->imageset_manager()->imageset("ui_game")->image("buy_lead"));
    m_sprPlayLead = new Sprite(Root::instance()->imageset_manager()->imageset("ui_game")->image("play_lead"));
    m_sprLeftKey = new Sprite(Root::instance()->imageset_manager()->imageset("ui_game")->image("left_key_0"));
    m_sprRighKey = new Sprite(Root::instance()->imageset_manager()->imageset("ui_game")->image("right_key_0"));
    add_child(m_sprGunLead);
    add_child(m_sprBuyLead);
    add_child(m_sprPlayLead);
    add_child(m_sprLeftKey);
    add_child(m_sprRighKey);

    m_sprGunLead->set_visible(false);
    m_sprBuyLead->set_visible(false);
    m_sprPlayLead->set_visible(false);
    m_sprLeftKey->set_visible(false);
    m_sprRighKey->set_visible(false);



    m_sprWaringTime  = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("warning_time"));
    add_child(m_sprWaringTime);
    m_sprWaringTime->set_visible(false);

    std::ostringstream ostr;
    for (int i=0; i<10; i++)
    {
        ostr.str("");
        ostr << "time_" << i ;
        m_imgTimer[i] = Root::instance()->imageset_manager()->imageset("account")->image(ostr.str());
    }

    for (int i=0; i<10; i++)
    {
        ostr.str("");
        ostr << "task_number_" << i ;
        m_imgTaskNumber[i] = Root::instance()->imageset_manager()->imageset("task")->image(ostr.str());
    }

    for (int i=0; i<10; i++)
    {
        ostr.str("");
        ostr << "number_" << i ;
        m_imgCookGold[i] = Root::instance()->imageset_manager()->imageset("role")->image(ostr.str());
    }


    for (int i=0; i<11; i++)
    {
        ostr.str("");
        ostr << "fish_num_" << i ;
        m_imgCookFishNumber[i] = Root::instance()->imageset_manager()->imageset("cook")->image(ostr.str());
    }

    for (int i=0; i<11; i++)
    {
        ostr.str("");
        ostr << "time_" << i ;
        m_imgCookTime[i] = Root::instance()->imageset_manager()->imageset("cook")->image(ostr.str());
    }

    m_sprLaserBeanCannon = new Sprite(Root::instance()->imageset_manager()->imageset("laserBean")->image("0"));
    add_child(m_sprLaserBeanCannon);
    m_sprLaserBeanCannon->set_visible(false);

    m_sprBombCannon = new Sprite(Root::instance()->imageset_manager()->imageset("bomb")->image("0"));
    add_child(m_sprBombCannon);
    m_sprBombCannon->set_visible(false);

    m_sprBomb = new Sprite;
    add_child(m_sprBomb);
    m_sprBomb->set_visible(false);

    m_sprExplorer = new Sprite;
    add_child(m_sprExplorer);
    m_sprExplorer->set_visible(false);

    m_sprReward = new Sprite(Root::instance()->imageset_manager()->imageset("get_reward")->image("4"));
    add_child(m_sprReward);
    m_sprReward->set_visible(false);

    m_sprCumBar = new Sprite(Root::instance()->imageset_manager()->imageset("task")->image("cum_bar"));
    add_child(m_sprCumBar);

    m_sprShowBar = new Sprite;
    add_child(m_sprShowBar);

    for (int i=0; i<3; i++)
    {
        m_sprBarBlank[i] = new Sprite(Root::instance()->imageset_manager()->imageset("task")->image("bar_blank"));
        m_sprBar[i] = new Sprite;
        add_child(m_sprBarBlank[i]);
        add_child(m_sprBar[i]);
    }

    m_sprRealFly = new Sprite;
    m_sprRealFly->set_visible(false);
    add_child(m_sprRealFly);

    m_sprReelBack = new Sprite(Root::instance()->imageset_manager()->imageset("task")->image("reel_back"));
    m_sprReelBack->set_visible(false);
    add_child(m_sprReelBack);

    m_sprReelEff = new Sprite();
    m_sprReelEff->set_visible(false);
    add_child(m_sprReelEff);

    for (int i=0; i<3; i++)
    {
        m_sprReelNormal[i] = new Sprite(Root::instance()->imageset_manager()->imageset("task")->image("reel_normal"));
        m_sprReelNormal[i]->set_visible(false);
        add_child(m_sprReelNormal[i]);

        m_sprReelFast[i] = new Sprite(Root::instance()->imageset_manager()->imageset("task")->image("reel_fast"));
        m_sprReelFast[i]->set_visible(false);
        add_child(m_sprReelFast[i]);
    }

    for (int i=0; i<4; i++)
    {
        m_sprTaskName[i] = new Sprite;
        m_sprTaskName[i] ->set_visible(false);
        add_child(m_sprTaskName[i]);
        m_sprTaskName[i]->set_visible(false);
    }

    m_sprChangeEffect = new Sprite;
    add_child(m_sprChangeEffect);
    m_sprChangeEffect->set_visible(false);

    m_sprBarEnough = new Sprite;
    add_child(m_sprBarEnough);
    m_sprChangeEffect->set_visible(false);

    m_sprCookSayBack  = new Sprite(Root::instance()->imageset_manager()->imageset("cook")->image("say_back"));
    m_sprCookSayBack->set_visible(false);
    add_child(m_sprCookSayBack);

    m_sprCookSay = new Sprite();
    m_sprCookSay->set_visible(false);
    add_child(m_sprCookSay);

    m_sprCookBack = new Sprite(Root::instance()->imageset_manager()->imageset("cook")->image("cook_back"));
    m_sprCookBack->set_visible(false);
    add_child(m_sprCookBack);

    m_sprCookNormal = new Sprite(Root::instance()->imageset_manager()->imageset("cook")->image("cook_normal_14"));
    m_sprCookNormal->set_visible(false);
    add_child(m_sprCookNormal);

    m_sprCookFish= new Sprite();
    m_sprCookFish->set_visible(false);
    add_child(m_sprCookFish);

    m_sprCookComplete= new Sprite(Root::instance()->imageset_manager()->imageset("cook")->image("quan"));
    m_sprCookComplete->set_visible(false);
    add_child(m_sprCookComplete);

    m_sprCookGold = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("gold_0"));
    m_sprCookGold->set_visible(false);
    add_child(m_sprCookGold);

    m_sprCookLost= new Sprite();
    m_sprCookLost->set_visible(false);
    add_child(m_sprCookLost);

    m_sprCookWin= new Sprite();
    m_sprCookWin->set_visible(false);
    add_child(m_sprCookWin);
}

CRoleLayer::~CRoleLayer()
{
}

void CRoleLayer::update(float dt)
{
    __super::update(dt);
}

void CRoleLayer::draw()
{
    __super::draw();

    if (m_sprUserInfo->visible())
    {
        CGameScene *pGameScene = (CGameScene *)parent();
        CClientKernel *pClientKernel = pGameScene->GetClientKernel();
        if ( !pClientKernel)
            return ;

        const tagUserData * pUserData = pClientKernel->GetUserInfo(m_wChairID);

        if (pUserData)
        {
            Point pt = m_sprUserInfo->position_absolute();
            pt.x_ -= 26;
            pt.y_ -= 42;

            m_Font->draw_string(pt, pUserData->szName, Color(176,222,238));

            pt.y_ += 24;

            std::ostringstream ostr;
            ostr << pUserData->lScore ;

            m_Font->draw_string(pt, ostr.str().c_str(), Color(176,222,238));

            std::string szRank;
            if (pUserData->lScore>0 && pUserData->lScore<1000)
            {
                szRank = "Óæ·ò";
            }
            else
            {
                szRank = "´¬³¤";
            }

            pt.y_ += 24;
            m_Font->draw_string(pt, szRank.c_str(), Color(176,222,238));
        }


    }

    if (m_sprWaringTime->visible())
    {
        Point pt(m_sprWaringTime->position_absolute());
        Point ptDraw;
        std::ostringstream ostr;
        DWORD dwTime = (DWORD)time(NULL) - m_dwStartTime;

        if (dwTime >= 30)
        {
            CGameScene *pGameScene = (CGameScene *)parent();


            pGameScene->window_closed(0);
            Root::instance()->queue_end_rendering();		
        }
        else
        {
            ostr.str("");
            ostr << 30-dwTime; 

            ptDraw.x_ = pt.x_ -8;
            ptDraw.y_ = pt.y_ + 5;

            DrawTimer(ostr.str(), ptDraw);
        }
    }

    if (m_sprReward->visible() && m_bShowRewardNubmer)
    {
        Point pt(m_sprReward->position_absolute());
        Point ptDraw;
        ptDraw.x_ = pt.x_-46;
        ptDraw.y_ = pt.y_;

        std::ostringstream ostr;
        ostr.str("");
        ostr << m_nRewardNumber; 
        DrawTaskNumber(ostr.str(),ptDraw);
    }

    if (m_TaskDate.m_enType == CTaskDate::TYPE_COOK && m_sprCookFish->visible())
    {
        Point pt(m_sprCookNormal->position_absolute());
        Point ptDraw;

        DWORD dwTime = (DWORD)time(NULL) - m_dwCookStartTime;

        CGameScene *pGameScene = (CGameScene *)parent();
        if (pGameScene && pGameScene->GetMeChairID() == m_wChairID && (dwTime >= m_TaskDate.m_nDuration))
        {
            FireCook();
        }

		if (dwTime >= m_TaskDate.m_nDuration)
			dwTime = m_TaskDate.m_nDuration;

		ptDraw.x_ = pt.x_-60 ;
		ptDraw.y_ = pt.y_-130 ;

		DrawCookTimer(m_TaskDate.m_nDuration-dwTime, ptDraw);

		ptDraw.x_ = pt.x_ - 40;
		ptDraw.y_ = pt.y_  ;

		DrawFishNumber(ptDraw);

		ptDraw.x_ = pt.x_ - 26;
		ptDraw.y_ = pt.y_ + 36 ;

		DrawCookGold(ptDraw);

    }
}

void CRoleLayer::DrawTimer(std::string &number, const Point &pt)
{
    std::ostringstream ostr;
    Point ptDraw;
    ptDraw = pt - Point(number.size()/2.0*24.0, 0);

    for (int i=0; i<number.size(); i++)
    {
        ostr.str("");
        ostr << number[i];
        int nIndex = std::atoi(ostr.str().c_str());
        m_imgTimer[nIndex]->draw(ptDraw);

        ptDraw.x_ += 24;
    }

}

void CRoleLayer::DrawCookTimer(DWORD dwTime, const Point &pt)
{
    Point ptDraw(pt);

    int nMinute = dwTime/60;
    int nSecond1 = dwTime%60/10;
    int nSecond2 = dwTime%60%10;

    m_imgCookTime[nMinute]->draw(ptDraw);
    ptDraw.x_ += 28;

    m_imgCookTime[10]->draw(ptDraw);
    ptDraw.x_ += 28;

    m_imgCookTime[nSecond1]->draw(ptDraw);
    ptDraw.x_ += 28;

    m_imgCookTime[nSecond2]->draw(ptDraw);
}

void CRoleLayer::DrawCookGold(const Point &pt)
{
    Point ptDraw(pt);

    int nHundred = m_TaskDate.m_nBonus/100;
    int nTen = m_TaskDate.m_nBonus%100/10;
    int nUnits = m_TaskDate.m_nBonus%10;

    m_imgCookGold[nHundred]->draw(ptDraw);
    ptDraw.x_ += 24;

    m_imgCookGold[nTen]->draw(ptDraw);
    ptDraw.x_ += 24;

    m_imgCookGold[nUnits]->draw(ptDraw);
    ptDraw.x_ += 24;
}

void CRoleLayer::DrawFishNumber(const Point &pt)
{
    Point ptDraw(pt);

    int nCookFish1 = m_nCookFish/10;
    int nCookFish2 = m_nCookFish%10;
    int nFishCount1 = m_TaskDate.m_nFishCount/10;
    int nFishCount2 = m_TaskDate.m_nFishCount%10;

    m_imgCookFishNumber[nCookFish1]->draw(ptDraw);
    ptDraw.x_ += 16;

    m_imgCookFishNumber[nCookFish2]->draw(ptDraw);
    ptDraw.x_ += 16;

    m_imgCookFishNumber[10]->draw(ptDraw);
    ptDraw.x_ += 16;

    m_imgCookFishNumber[nFishCount1]->draw(ptDraw);
    ptDraw.x_ += 16;

    m_imgCookFishNumber[nFishCount2]->draw(ptDraw);
}

void CRoleLayer::DrawTaskNumber(std::string &number, const Point &pt)
{
    std::ostringstream ostr;
    Point ptDraw;
    ptDraw = pt - Point(number.size()/2.0*53.0, 0);

    for (int i=0; i<number.size(); i++)
    {
        ostr.str("");
        ostr << number[i];
        int nIndex = std::atoi(ostr.str().c_str());
        m_imgTaskNumber[nIndex]->draw(ptDraw);

        ptDraw.x_ += 53.0;
    }

}

void CRoleLayer::SetChairID(WORD wChairID)
{
    if (m_wChairID == wChairID)
        return;

    m_wChairID = wChairID;

    if (wChairID == INVALID_CHAIR)
        return;

    if (m_wChairID==0 || m_wChairID==1)
    {
        m_sprGunWing1->set_position(Point(262,38));
        m_sprGunWing2->set_position(Point(38,38));
        m_sprGunBase->set_position(Point(150,48));
        m_sprGunOver->set_position(Point(150,20));
        m_sprGun->set_position(Point(150,20));
        m_sprGunLight->set_position(Point(150,32));

        m_sprCannon->set_position(Point(150,26));
        m_sprLaserBeanCannon->set_position(Point(150,26));
        m_sprBombCannon->set_position(Point(150,60));
        m_sprBombFuse->set_position(Point(150,60));
        m_sprLaserBean->set_position(Point(150,26));
        m_sprReward->set_position(Point(180,140));

        m_sprGunWing1->set_rotation(M_PI);
        m_sprGunWing2->set_rotation(M_PI);
        m_sprGunBase->set_rotation(M_PI);
        m_sprGunOver->set_rotation(M_PI);
        m_sprGun->set_rotation(M_PI);
        m_sprGunLight->set_rotation(M_PI);
        m_sprCannon->set_rotation(M_PI);
        m_sprLaserBeanCannon->set_rotation(M_PI);
        m_sprBombCannon->set_rotation(M_PI);
        m_sprLaserBean->set_rotation(M_PI);



        m_sprUserInfo->set_position(Point(150,100));
        m_sprWaringBullet->set_position(Point(150,150));
        m_sprWaringTime->set_position(Point(150,200));

        m_sprGunLead ->set_position(Point(150,180));
        m_sprBuyLead->set_position(Point(150,180));
        m_sprPlayLead->set_position(Point(150,180));
        m_sprLeftKey->set_position(Point(280,200));
        m_sprRighKey->set_position(Point(280,200));

        m_sprCumBar->set_position(Point(150,0));
        m_sprShowBar->set_position(Point(150,0));

        m_sprBarBlank[0]->set_position(Point(88,0));
        m_sprBarBlank[1]->set_position(Point(150,0));
        m_sprBarBlank[2]->set_position(Point(212,0));
        m_sprBar[0]->set_position(Point(88,-1));
        m_sprBar[1]->set_position(Point(150,-1));
        m_sprBar[2]->set_position(Point(212,-1));



        m_sprTaskName[0]->set_position(Point(30,100));
        m_sprTaskName[1]->set_position(Point(115,160));
        m_sprTaskName[2]->set_position(Point(205,160));
        m_sprTaskName[3]->set_position(Point(290,100));

        m_sprChangeEffect->set_rotation(M_PI);
        m_sprChangeEffect->set_position(Point(150,100));

    }
    else
    {
        m_sprGunWing1->set_position(Point(38,58));
        m_sprGunWing2->set_position(Point(262,58));
        m_sprGunBase->set_position(Point(150,48));
        m_sprGunOver->set_position(Point(150,70));
        m_sprGun->set_position(Point(38,58));
        m_sprGunLight->set_position(Point(150,64));

        m_sprCannon->set_position(Point(150,70));
        m_sprLaserBeanCannon->set_position(Point(150,70));
        m_sprBombCannon->set_position(Point(150,30));
        m_sprBombFuse->set_position(Point(150,30));
        m_sprLaserBean->set_position(Point(150,70));
        m_sprReward->set_position(Point(180,-140));

        m_sprUserInfo->set_position(Point(150,0));

        m_sprWaringBullet->set_position(Point(150,-60));
        m_sprWaringTime->set_position(Point(150,-100));

        m_sprGunLead ->set_position(Point(150,-100));
        m_sprBuyLead->set_position(Point(150,-100));
        m_sprPlayLead->set_position(Point(150,-100));
        m_sprLeftKey->set_position(Point(280,-80));
        m_sprRighKey->set_position(Point(280,-80));

        m_sprCumBar->set_position(Point(150,96));
        m_sprShowBar->set_position(Point(150,96));

        m_sprBarBlank[0]->set_position(Point(88,96));
        m_sprBarBlank[1]->set_position(Point(150,96));
        m_sprBarBlank[2]->set_position(Point(212,96));
        m_sprBar[0]->set_position(Point(88,95));
        m_sprBar[1]->set_position(Point(150,95));
        m_sprBar[2]->set_position(Point(212,95));



        m_sprTaskName[0]->set_position(Point(30,-20));
        m_sprTaskName[1]->set_position(Point(115,-80));
        m_sprTaskName[2]->set_position(Point(205,-80));
        m_sprTaskName[3]->set_position(Point(290,-20));


        m_sprChangeEffect->set_position(Point(150,-20));
    }

    Point ptExplorer(640, 369);
    ptExplorer -= position();

    m_sprExplorer->set_position(ptExplorer);

    if (m_wChairID==0)
    {
        m_sprShotNumBack->set_position(Point(-106,34));
        m_sprNet->set_position(Point(-180,34));
        m_sprShotGold->set_position(Point(-146,34));

        m_sprGold[0]->set_position(Point(-90-24,34));
        m_sprGold[1]->set_position(Point(-90,34));
        m_sprGold[2]->set_position(Point(-90+24,34));
        m_sprGold[3]->set_position(Point(-90+48,34));
        m_sprGold[4]->set_position(Point(-90+72,34));

        m_sprRealFly->set_position(Point(-40,180));
        m_sprReelBack->set_position(Point(-40,180));
        for (int i=0; i<3; i++)
        {
            m_sprReelNormal[i]->set_position(Point(-92+i*51,180));
            m_sprReelFast[i]->set_position(Point(-92+i*51,180));
        }

        m_sprReelEff->set_position(Point(-40,180));
        m_sprBombFuse->set_rotation(M_PI_4+M_PI_2);

        m_sprCookNormal->set_flip_x(true);
        m_sprCookBack->set_position(Point(-120,200));
        m_sprCookNormal->set_position(Point(-120,200));
        m_sprCookSay->set_position(Point(0,100));
        m_sprCookSayBack->set_position(Point(-10,100));
        m_sprCookFish->set_position(Point(-120,160));
        m_sprCookGold->set_position(Point(-160,250));
        m_sprCookComplete->set_position(Point(-120,140));
        m_sprCookWin->set_position(Point(-120,200));
        m_sprCookLost->set_position(Point(-120,200));

    }
    else if (m_wChairID==1)
    {
        m_sprShotNumBack->set_position(Point(406,34));
        m_sprNet->set_position(Point(332,34));
        m_sprShotGold->set_position(Point(366,34));

        m_sprGold[0]->set_position(Point(422-24,34));
        m_sprGold[1]->set_position(Point(422,34));
        m_sprGold[2]->set_position(Point(422+24,34));
        m_sprGold[3]->set_position(Point(422+48,34));
		m_sprGold[4]->set_position(Point(422+72,34));


        m_sprRealFly->set_position(Point(340,180));
        m_sprReelBack->set_position(Point(340,180));
        for (int i=0; i<3; i++)
        {
            m_sprReelNormal[i]->set_position(Point(288+i*51,180));
            m_sprReelFast[i]->set_position(Point(288+i*51,180));
        }
        m_sprReelEff->set_position(Point(340,180));
        m_sprBombFuse->set_rotation(M_PI_4+M_PI);

        m_sprCookSayBack->set_flip_x(true);
        m_sprCookBack->set_position(Point(420,200));
        m_sprCookNormal->set_position(Point(420,200));
        m_sprCookSay->set_position(Point(290,100));
        m_sprCookSayBack->set_position(Point(290,100));
        m_sprCookFish->set_position(Point(420,160));
        m_sprCookGold->set_position(Point(380,250));
        m_sprCookComplete->set_position(Point(120,140));
        m_sprCookWin->set_position(Point(420,200));
        m_sprCookLost->set_position(Point(420,200));

    }
    else if (m_wChairID==2)
    {
        m_sprShotNumBack->set_position(Point(406,64));
        m_sprNet->set_position(Point(332,64));
        m_sprShotGold->set_position(Point(366,64));

        m_sprGold[0]->set_position(Point(422-24,64));
        m_sprGold[1]->set_position(Point(422,64));
        m_sprGold[2]->set_position(Point(422+24,64));
        m_sprGold[3]->set_position(Point(422+48,64));
        m_sprGold[4]->set_position(Point(422+72,64));

        m_sprRealFly->set_position(Point(340,-100));
        m_sprReelBack->set_position(Point(340,-100));
        for (int i=0; i<3; i++)
        {
            m_sprReelNormal[i]->set_position(Point(288+i*51,-102));
            m_sprReelFast[i]->set_position(Point(288+i*51,-102));
        }

        m_sprReelEff->set_position(Point(340,-100));
        m_sprBombFuse->set_rotation(-M_PI_4);

        m_sprCookSayBack->set_flip_x(true);
        m_sprCookBack->set_position(Point(420,-100));
        m_sprCookNormal->set_position(Point(420,-100));
        m_sprCookSay->set_position(Point(290,-210));
        m_sprCookSayBack->set_position(Point(290,-210));
        m_sprCookFish->set_position(Point(420,-140));
        m_sprCookGold->set_position(Point(380,-50));
        m_sprCookComplete->set_position(Point(120,-140));
        m_sprCookWin->set_position(Point(420,-100));
        m_sprCookLost->set_position(Point(420,-100));

    }
    else
    {
        m_sprShotNumBack->set_position(Point(-106,64));
        m_sprNet->set_position(Point(-180,64));
        m_sprShotGold->set_position(Point(-146,64));

        m_sprGold[0]->set_position(Point(-90-24,64));
        m_sprGold[1]->set_position(Point(-90,64));
        m_sprGold[2]->set_position(Point(-90+24,64));
        m_sprGold[3]->set_position(Point(-90+48,64));
        m_sprGold[4]->set_position(Point(-90+72,64));

        m_sprRealFly->set_position(Point(-40,-100));
        m_sprReelBack->set_position(Point(-40,-100));
        for (int i=0; i<3; i++)
        {
            m_sprReelNormal[i]->set_position(Point(-92+i*51,-102));
            m_sprReelFast[i]->set_position(Point(-92+i*51,-102));
        }

        m_sprReelEff->set_position(Point(-40,-100));

        m_sprBombFuse->set_rotation(M_PI_4);

        m_sprCookNormal->set_flip_x(true);
        m_sprCookBack->set_position(Point(-120,-100));
        m_sprCookNormal->set_position(Point(-120,-100));
        m_sprCookSay->set_position(Point(0,-210));
        m_sprCookSayBack->set_position(Point(-10,-210));
        m_sprCookFish->set_position(Point(-120,-140));
        m_sprCookGold->set_position(Point(-160,-50));
        m_sprCookComplete->set_position(Point(-120,-140));
        m_sprCookWin->set_position(Point(-120,-100));
        m_sprCookLost->set_position(Point(-120,-100));
    }

    std::ostringstream ostr;
    ostr << "gun_light_" << (int)wChairID << "_0";
    m_sprGunLight->set_display_image(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));

    Animation *aniShotGold = Root::instance()->animation_manager()->animation("shot_gold");
    Root::instance()->action_manager()->remove_all_action_from_target(m_sprShotGold);
    Action *actShotGold = new Action_Repeat_Forever(new Action_Animation(0.06, aniShotGold, false));
    m_sprShotGold->run_action(actShotGold);

    ostr.str("");
    ostr << "bar_" << (int)wChairID;
    m_sprBar[0]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image(ostr.str()));
    m_sprBar[1]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image(ostr.str()));
    m_sprBar[2]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image(ostr.str()));

}

void CRoleLayer::SetCannonType(CGameCore::enCannonType CannonType) 
{
    if (m_CannonType == CannonType)
        return;

    m_CannonType = CannonType;

    Root::instance()->action_manager()->remove_all_action_from_target(m_sprCannon);

    std::ostringstream ostr;
    ostr << "gun" << (int)CannonType ;
    m_sprCannon->set_display_image(Root::instance()->imageset_manager()->imageset(ostr.str())->image("0"));

    CGameScene *pGameScene = (CGameScene *)parent();

    if (m_wChairID == pGameScene->GetMeChairID())
    {
        CMD_C_Change_Cannon ChangeCannon;
        ChangeCannon.wStyle = m_CannonType;

        pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_CHANGE_CANNON, &ChangeCannon, sizeof(CMD_C_Change_Cannon));
    }

}

void CRoleLayer::SetFishGold(WORD wFishGold)
{
    //if (wFishGold == m_wFishGold)
    //    return;

    m_wFishGold = wFishGold; 

    bool bGotHead = false;
    int nSpritePosition = 0;
    int nSingleNumber = 0;
    std::ostringstream ostr;

    nSingleNumber = (int)(m_wFishGold%100000/10000);
    if (nSingleNumber>0 || bGotHead )
    {
        bGotHead = true;
		ostr.str("");
        ostr << "number_" << nSingleNumber;
        m_sprGold[nSpritePosition++]->set_display_image(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }

    nSingleNumber = (int)(m_wFishGold%100000%10000/1000);
    if (nSingleNumber>0 || bGotHead )
    {
        bGotHead = true;
		ostr.str("");
        ostr << "number_" << nSingleNumber;
        m_sprGold[nSpritePosition++]->set_display_image(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }


    nSingleNumber = (int)(m_wFishGold%100000%10000%1000/100);
    if (nSingleNumber>0 || bGotHead )
    {
        bGotHead = true;
        ostr.str("");
        ostr << "number_" << nSingleNumber;
        m_sprGold[nSpritePosition++]->set_display_image(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }

    nSingleNumber = (int)(m_wFishGold%100000%10000%100/10);
    if (nSingleNumber>0 || bGotHead )
    {
        bGotHead = true;
        ostr.str("");
        ostr << "number_" << nSingleNumber;
        m_sprGold[nSpritePosition++]->set_display_image(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }
 

    nSingleNumber = (int)(m_wFishGold%100000%10000%10);
    ostr.str("");
    ostr << "number_" << nSingleNumber;
    m_sprGold[nSpritePosition++]->set_display_image(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));

    for (int i=nSpritePosition; i<5; i++)
    {
        m_sprGold[i]->set_display_image(0);
    }


}

void CRoleLayer::ShowFishGoldEmpty(bool bShow)
{

    m_sprWaringBullet->set_visible(bShow);

}

void CRoleLayer::UpdateGold(WORD wFishGold)
{
    /*m_wFishGold += wFishGold;*/

    int nOffset;
    std::string strPrex;
    bool bGotHead = false;
    int nSpritePosition = 1;
    int nSingleNumber = 0;
    std::ostringstream ostr;
    Point pt;
    Sprite *sprValue[5];

    CGameScene *pGameScene = (CGameScene *)parent();
    if (m_wChairID==pGameScene->GetMeChairID())
    {
        nOffset = 33;
        strPrex = "gold_number_";
    }
    else
    {
        nOffset = 28;
        strPrex = "silver_number_";
    }   
    ostr << strPrex << 10;

    sprValue[0] = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    sprValue[0]->set_visible(false);
    Action *actFunc = new Action_Func(rak::mem_fn(this, &CRoleLayer::UpdateGoldReal));
    actFunc->set_tag(wFishGold);
    Action *act = new Action_Sequence(new Action_Delay(2), new Action_Show(), new Action_Func(rak::mem_fn(this, &CRoleLayer::UpdateGoldSound)), new Action_Scale_To(0.25, 1.25), new Action_Scale_To(0.25, 1.0), new Action_Delay(0.5), actFunc, 0);
    sprValue[0]->run_action(act);   

    nSingleNumber = (int)(wFishGold/1000);

    if (nSingleNumber>0)
    {
        bGotHead = true;
        ostr.str("");
        ostr << strPrex << nSingleNumber;
        sprValue[nSpritePosition++]= new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }

    nSingleNumber = (int)(wFishGold%1000/100);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << strPrex << nSingleNumber;
        sprValue[nSpritePosition++]= new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }

    nSingleNumber = (int)(wFishGold%100/10);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << strPrex << nSingleNumber;
        sprValue[nSpritePosition++]= new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }

    nSingleNumber = (int)(wFishGold%10);
    ostr.str("");
    ostr << strPrex << nSingleNumber;
    sprValue[nSpritePosition++]= new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));

    if (m_wChairID==0)
    {
        sprValue[0]->set_position(Point( -180,100));
    }
    else if (m_wChairID==1)
    {
        sprValue[0]->set_position(Point(330, 100));

    }
    else if (m_wChairID==2)
    {
        sprValue[0]->set_position(Point(330,0));
    }
    else
    {
        sprValue[0]->set_position(Point(-180,0));
    }

    pt = sprValue[0]->position();

    for (int i=0; i<nSpritePosition; i++)
    {
        if (sprValue[i] != 0)
        {
            pt.x_ += nOffset;
            sprValue[i]->set_position(pt);
            sprValue[i]->set_visible(false);
            add_child(sprValue[i]);

            Action *act = new Action_Sequence(new Action_Delay(2), new Action_Show()/*, new Action_Func(rak::mem_fn(this, &CRoleLayer::UpdateGoldSound))*/, new Action_Scale_To(0.25, 1.25), new Action_Scale_To(0.25, 1.0), new Action_Delay(0.5), new Action_Func(rak::mem_fn(this, &CRoleLayer::UpdateGoldEnd)), 0);
            sprValue[i]->run_action(act);   
        }

    }
}

bool CRoleLayer::UpdateGoldEnd(Node *node, int tag)
{
    if (tag==1000)
    {
        SetFishGold(m_wFishGold);
    }

    remove_child(node);
    delete node;

    return true;
}

bool CRoleLayer::UpdateGoldReal(Node *node, int tag)
{
    SetFishGold(m_wFishGold+tag);

    remove_child(node);
    delete node;

    return true;
}


bool CRoleLayer::UpdateGoldSound(Node *node, int tag)
{

    try
    {
        Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(15);
        pSound->play(false, true);
    }
    catch(...)
    {
    }


    return true;
}

void CRoleLayer::SetCannonRatation(const Point &ptMouse)
{
    float fRorate;
    Point pt(ptMouse);



    if (m_wChairID<2)
    {
        pt = pt-m_sprCannon->position_absolute();
        if (pt.y_>=0)
        {
            fRorate= std::acos(pt.x_/std::sqrt(pt.x_*pt.x_+pt.y_*pt.y_))+M_PI_2;
        }
        else
        {
            if (pt.x_ >= 0)
                fRorate= M_PI_2;
            else
                fRorate = M_PI_2+M_PI;
        }

    }
    else
    {
        pt = m_sprCannon->position_absolute() - pt;
        if (pt.y_>=0)
        {
            fRorate= std::acos(pt.x_/std::sqrt(pt.x_*pt.x_+pt.y_*pt.y_))-M_PI_2;
        }
        else
        {
            if (pt.x_ <= 0)
                fRorate= M_PI_2;
            else
                fRorate = M_PI_2+M_PI;
        }

    }

    m_sprCannon->set_rotation(fRorate);
    m_sprLaserBeanCannon->set_rotation(fRorate);
}

void CRoleLayer::SetCannonRatation(float fRate)
{

    m_sprLaserBeanCannon->set_rotation(fRate);
    m_sprCannon->set_rotation(fRate);
}

void CRoleLayer::GunAnimation()
{
    std::ostringstream ostr;
    ostr << "gun" << (int)m_CannonType;
    Animation *aniGun = Root::instance()->animation_manager()->animation(ostr.str());

    Root::instance()->action_manager()->remove_all_action_from_target(m_sprCannon);
    m_sprCannon->run_action(new Action_Animation(0.06,aniGun,true));

}

void CRoleLayer::GunBaseAnimation()
{
    std::ostringstream ostr;
    ostr << "gun_light_" << (int)m_wChairID;
    Animation *aniGunLight = Root::instance()->animation_manager()->animation(ostr.str());

    Root::instance()->action_manager()->remove_all_action_from_target(m_sprGunLight);
    m_sprGunLight->run_action(new Action_Animation(0.06,aniGunLight,true));
}

void CRoleLayer::RewardAnimation(int nRewardNumber, bool bDelay)
{
    m_nRewardNumber = nRewardNumber;

    Animation *ani = Root::instance()->animation_manager()->animation("get_reward");

    Root::instance()->action_manager()->remove_all_action_from_target(m_sprReward);

    if (bDelay)
        m_sprReward->run_action(new Action_Sequence(new Action_Delay(2), new Action_Show(), new Action_Animation(0.2,ani,true), new Action_Func(rak::mem_fn(this, &CRoleLayer::RewardAnimationEnd1)), new Action_Delay(2) , new Action_Func(rak::mem_fn(this, &CRoleLayer::RewardAnimationEnd2)), 0));
    else
        m_sprReward->run_action(new Action_Sequence(new Action_Show(), new Action_Animation(0.2,ani,true), new Action_Func(rak::mem_fn(this, &CRoleLayer::RewardAnimationEnd1)), new Action_Delay(2) , new Action_Func(rak::mem_fn(this, &CRoleLayer::RewardAnimationEnd2)), 0));
}

bool CRoleLayer::RewardAnimationEnd1(Node *node, int tag)
{
    m_bShowRewardNubmer = true;
    //((Sprite*)node)->set_display_image(Root::instance()->imageset_manager()->imageset("get_reward")->image("8"));
    return true;
}

bool CRoleLayer::RewardAnimationEnd2(Node *node, int tag)
{
    node->set_visible(false);
    m_bShowRewardNubmer = false;
    return true;
}

void CRoleLayer::ShowBarAnimation()
{                                  
    Animation *aniShowBar= Root::instance()->animation_manager()->animation("show_bar");
    Action *actShowBar = new Action_Repeat_Forever(new Action_Animation(0.2,aniShowBar,false));
    actShowBar->set_tag(0);         
    m_sprShowBar->run_action(actShowBar);
}

void CRoleLayer::DestroyShowBarAnimation()
{
    Root::instance()->action_manager()->remove_all_action_from_target(m_sprShowBar);
    m_sprShowBar->set_display_image(0);
}

void CRoleLayer::ShowUserInfo(bool bVisible)
{
    m_sprUserInfo->set_visible(bVisible);
}

void CRoleLayer::ShowGunLead(bool bShow)
{
    m_sprGunLead ->set_visible(bShow);
    m_sprLeftKey->set_visible(bShow);
}

void CRoleLayer::ShowBuyLead(bool bShow)
{
    m_sprBuyLead->set_visible(bShow);
}

void CRoleLayer::ShowPlayLead(bool bShow)
{
    m_sprPlayLead->set_visible(bShow);
    m_sprRighKey->set_visible(bShow);
}

void CRoleLayer::ShowWaringTime(bool bShow)
{
    if (bShow)
        m_dwStartTime = (DWORD)time(NULL);

    m_sprWaringTime->set_visible(bShow);
}

bool CRoleLayer::IsShowWaringTime() 
{
    return m_sprWaringTime->visible();
}

bool CRoleLayer::CommandTaskStart(CMD_S_Task *pTask)
{
    m_dwCookStartTime = (DWORD)time(NULL);
    m_TaskDate.m_enType =  (CTaskDate::Type)pTask->nTask;
    m_TaskDate.m_enState = CTaskDate::STATE_PREPARE1;
    m_TaskDate.m_nStartWheel[0] = pTask->nStartWheel[0];
    m_TaskDate.m_nStartWheel[1] = pTask->nStartWheel[1];
    m_TaskDate.m_nStartWheel[2] = pTask->nStartWheel[2];
    m_TaskDate.m_nEndWheel[0] = pTask->nEndWheel[0];
    m_TaskDate.m_nEndWheel[1] = pTask->nEndWheel[1];
    m_TaskDate.m_nEndWheel[2] = pTask->nEndWheel[2];
    m_TaskDate.m_nDuration = pTask->nDuration;
    m_TaskDate.m_nFishType = pTask->cbFishType;
    m_TaskDate.m_nFishCount = pTask->cbFishCount;
    m_TaskDate.m_nBonus = pTask->nBonus;

    int nLevel = pTask->cbLevel;

    Point ptBarEnough;
    if (m_wChairID==0 || m_wChairID==1)
    {
        ptBarEnough.x_ = 104+nLevel*62;
        ptBarEnough.y_ = 0;
    }
    else
    {
        ptBarEnough.x_ = 104+nLevel*62;
        ptBarEnough.y_ = 96;
    }

    m_sprBarEnough->set_visible(true);
    m_sprBarEnough->set_position(ptBarEnough);

    Animation *aniBarEnough = Root::instance()->animation_manager()->animation("bar_enough");

    Action *actBarEnough = new Action_Sequence(new Action_Animation(0.06, aniBarEnough, false), new Action_Hide(), new Action_Func(rak::mem_fn(this, &CRoleLayer::BarEnoughEnd)), 0);
    m_sprBarEnough->run_action(actBarEnough);

	try
	{
		Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(18);
		pSound->play(false, true);
	}
	catch(...)
	{
	}

    return true;
}

bool CRoleLayer::BarEnoughEnd(Node *node, int tag)
{
    DestroyShowBarAnimation();

    Animation *aniRealFly = Root::instance()->animation_manager()->animation("real_fly");
    Action *actRealFly  = new Action_Sequence(new Action_Show(), new Action_Animation(0.06, aniRealFly, false), new Action_Hide(), new Action_Func(rak::mem_fn(this, &CRoleLayer::RealFlyEnd)), 0);
    m_sprRealFly->run_action(actRealFly);


	try
	{
		Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(19);
		pSound->play(false, true);
	}
	catch(...)
	{
	}
    return true;
}

bool CRoleLayer::RealFlyEnd(Node *node, int tag)
{
    m_sprReelBack->set_visible(true);

    for (int i=0; i<3; i++)
    {
        int nWheel = m_TaskDate.m_nStartWheel[i];
        int nEndWheel = m_TaskDate.m_nEndWheel[i];
        int nDistance = nEndWheel-nWheel;
        if (nDistance<=0)
            nDistance += 4;

        m_sprReelNormal[i]->set_visible(true);

        m_sprReelNormal[i]->set_content_size(Size(39,46));
        m_sprReelNormal[i]->set_src_size(Size(39,46));

        m_sprReelNormal[i]->set_src_position(Point(0,nWheel*46));
        Action *actReelNormal;
        if (i==2)
        {
            actReelNormal = new Action_Sequence(new Action_Delay(1.0), new Action_Hide(), new Action_Delay(3.0+i), new Action_Show(), new Action_Move_Src_Acceleration_To(5.0, Point(0,46*((4-nWheel)-nDistance-4)), 184), new Action_Delay(3.0), new Action_Func(rak::mem_fn(this, &CRoleLayer::ReelNormalEnd)), 0);
        }
        else
        {
            actReelNormal = new Action_Sequence(new Action_Delay(1.0), new Action_Hide(), new Action_Delay(3.0+i), new Action_Show(), new Action_Move_Src_Acceleration_To(5.0, Point(0,46*((4-nWheel)-nDistance-4)), 184),  0);
        }

        m_sprReelNormal[i]->run_action(actReelNormal);

        m_sprReelFast[i]->set_content_size(Size(39,46));
        m_sprReelFast[i]->set_src_size(Size(39,46));

        m_sprReelFast[i]->set_src_position(Point(0,80*nWheel));
        Action *actReelFas = new Action_Sequence(new Action_Delay(1.0), new Action_Show(), new Action_Move_Src_To(3.0+i, Point(0,80*(nWheel+10)), 320), new Action_Hide(), 0);
        m_sprReelFast[i]->run_action(actReelFas);
    }

    if (m_TaskDate.m_enType != CTaskDate::TYPE_NULL)
    {
        Animation *aniReelEff = Root::instance()->animation_manager()->animation("reel_eff");
        Action *actReelEff = new Action_Sequence(new Action_Delay(11.0), new Action_Show(), new Action_Func(rak::mem_fn(this, &CRoleLayer::ReelEffEnd)), new Action_Animation(0.1, aniReelEff, false) , 0);
        m_sprReelEff->run_action(actReelEff);
    }



    return true;
}

bool CRoleLayer::ReelEffEnd(Node *node, int tag)
{
		try
	{
		Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(23);
		pSound->play(false, true);
	}
	catch(...)
	{
	}

	return true;
}

bool CRoleLayer::ReelNormalEnd(Node *node, int tag)
{
    m_sprReelEff->set_visible(false);
    m_sprReelBack->set_visible(false);

    for (int i=0; i<3; i++)
    {
        m_sprReelFast[i]->set_visible(false);
        m_sprReelNormal[i]->set_visible(false);
    }

    if (m_TaskDate.m_enType == CTaskDate::TYPE_BEAN)
    {
        m_sprTaskName[0]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("laser_bean_0"));
        m_sprTaskName[1]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("laser_bean_1"));
        m_sprTaskName[2]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("laser_bean_2"));
        m_sprTaskName[3]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("laser_bean_3"));
    }
    else if (m_TaskDate.m_enType == CTaskDate::TYPE_BOMB)
    {
        m_sprTaskName[0]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("bomb_0"));
        m_sprTaskName[1]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("bomb_1"));
        m_sprTaskName[2]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("bomb_2"));
        m_sprTaskName[3]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("bomb_3"));
    }
    else if (m_TaskDate.m_enType == CTaskDate::TYPE_COOK)
    {
        m_sprTaskName[0]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("cook_0"));
        m_sprTaskName[1]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("cook_1"));
        m_sprTaskName[2]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("cook_2"));
        m_sprTaskName[3]->set_display_image(Root::instance()->imageset_manager()->imageset("task")->image("cook_3"));
    }
    else if (m_TaskDate.m_enType == CTaskDate::TYPE_BONUS)
    {
        FireBonus();
    }
    else
    {
    }

    if (m_TaskDate.m_enType > CTaskDate::TYPE_BONUS)
	{
		Action *func = new Action_Func(rak::mem_fn(this, &CRoleLayer::TaskNameEnd));
		func->set_tag(0);
		Action *act = new Action_Sequence(new Action_Delay(0.2),  new Action_Show(), func, 0);
		m_sprTaskName[0]->run_action(act);

		func = new Action_Func(rak::mem_fn(this, &CRoleLayer::TaskNameEnd));
		func->set_tag(1);
		act = new Action_Sequence(new Action_Delay(0.4),  new Action_Show(), func,0);
		m_sprTaskName[1]->run_action(act);

		func = new Action_Func(rak::mem_fn(this, &CRoleLayer::TaskNameEnd));
		func->set_tag(2);
		act = new Action_Sequence(new Action_Delay(0.6),  new Action_Show(), func,0);
		m_sprTaskName[2]->run_action(act);

		func = new Action_Func(rak::mem_fn(this, &CRoleLayer::TaskNameEnd));
		func->set_tag(3);
		act = new Action_Sequence(new Action_Delay(0.8),  new Action_Show(), new Action_Delay(0.8), func, 0);
		m_sprTaskName[3]->run_action(act);

		m_TaskDate.m_enState = CTaskDate::STATE_PREPARE2;    

		try
		{
			Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(20);
			pSound->play(false, true);
		}
		catch(...)
		{
		}
	}

	SetFireCountDelay(false);

    return true;
}

bool CRoleLayer::TaskNameEnd(Node *node, int tag)
{
	if (tag == 3)
	{
		m_sprTaskName[0]->set_visible(false);
		m_sprTaskName[1]->set_visible(false);
		m_sprTaskName[2]->set_visible(false);
		m_sprTaskName[3]->set_visible(false);

		if (m_TaskDate.m_enType > CTaskDate::TYPE_COOK)
		{

			try
			{
				Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(24);
				pSound->play(false, true);
			}
			catch(...)
			{
			}

			Animation *ani = Root::instance()->animation_manager()->animation("change_eff");
			Action *act = new Action_Sequence(new Action_Animation(0.06, ani, false),  new Action_Func(rak::mem_fn(this, &CRoleLayer::ChangeEffectEnd)), 0);
			m_sprChangeEffect->run_action(act);
			m_sprChangeEffect->set_visible(true);
		}
		else if (m_TaskDate.m_enType == CTaskDate::TYPE_COOK)
		{
			m_sprCookBack->set_visible(true);
			m_sprCookNormal->set_visible(true);
			m_sprCookSayBack->set_visible(true);
			m_sprCookSay->set_visible(true);

			m_sprCookSay->set_color(Color_Rect());
			m_sprCookSayBack->set_color(Color_Rect());
			m_sprCookSay->set_display_image(Root::instance()->imageset_manager()->imageset("cook")->image("say_1"));

			Animation *ani = Root::instance()->animation_manager()->animation("cook_normal");
			Action *act = new Action_Sequence(new Action_Animation(0.2, ani, true),  new Action_Func(rak::mem_fn(this, &CRoleLayer::CookNormalEnd)), new Action_Fade_Out(1.0), new Action_Func(rak::mem_fn(this, &CRoleLayer::CookNormalFadeOutEnd)), 0);
			m_sprCookNormal->run_action(act);
		}
	}
	else
	{
		try
		{
			Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(20);
			pSound->play(false, true);
		}
		catch(...)
		{
		}
	}

    return true;
}

bool CRoleLayer::CookNormalEnd(Node *node, int tag)
{
    Action *actCookSay = new Action_Fade_Out(1.0);
    m_sprCookSay->run_action(actCookSay);

    Action *actCookSayBack = new Action_Fade_Out(1.0);
    m_sprCookSayBack->run_action(actCookSayBack);

    std::ostringstream ostr;
    ostr << "fish_" << m_TaskDate.m_nFishType;

    m_sprCookFish->set_display_image(Root::instance()->imageset_manager()->imageset("cook")->image(ostr.str()));
    m_sprCookFish->set_visible(true);
    m_sprCookGold->set_visible(true);

    return true;
}

bool CRoleLayer::CookNormalFadeOutEnd(Node *node, int tag)
{

    CGameScene *pGameScene = (CGameScene *)parent();

    if (pGameScene->GetMeChairID() == m_wChairID)
    {
        CMD_C_Task_Prepared TaskPrepared;
        TaskPrepared.nTask = m_TaskDate.m_enType;

        pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_TASK_PREPARED, &TaskPrepared, sizeof(CMD_C_Task_Prepared));
    }

     m_TaskDate.m_enState =  CTaskDate::STATE_RUNNING;

    return true;
}

bool CRoleLayer::ChangeEffectEnd(Node *node, int tag)
{
    m_sprChangeEffect->set_visible(false);

    if (m_TaskDate.m_enType==CTaskDate::TYPE_BEAN )
    {
        m_TaskDate.m_enState =  CTaskDate::STATE_RUNNING;
        m_sprLaserBeanCannon->set_visible(true);

    }
    else if (m_TaskDate.m_enType==CTaskDate::TYPE_BOMB)
    {
        m_sprBombCannon->set_visible(true);
        m_sprCannon->set_visible(false);

        m_sprBomb->set_visible(true);
        m_sprBomb->set_position(m_sprBombCannon->position());
        Root::instance()->action_manager()->remove_all_action_from_target(m_sprBomb);

        Animation *ani = Root::instance()->animation_manager()->animation("bomb_task");
        Action *act = new Action_Repeat_Forever(new Action_Animation(0.1, ani, false));
        m_sprBomb->run_action(act);

        Point pt(640, 369);
        pt -= position();

        act = new Action_Sequence(new Action_Move_To(0.5, pt), new Action_Func(rak::mem_fn(this, &CRoleLayer::BombMoveEnd)), 0);
        m_sprBomb->run_action(act);
    }

    return true;
}

bool CRoleLayer::BombMoveEnd(Node *node, int tag)
{
    m_sprBombFuse->set_visible(true);

    Animation *ani = Root::instance()->animation_manager()->animation("bomb_fuse");
    Action *act = new Action_Repeat_Forever(new Action_Animation(0.1, ani, false));
    m_sprBombFuse->run_action(act);

    m_TaskDate.m_enState =  CTaskDate::STATE_RUNNING;

    return true;
}

void CRoleLayer::SetFireCount(int nFireCount) 
{ 
    m_nFireCount = nFireCount;

    if (m_bDelayBarEnough)
    {
        return;   
    }

    int nCount = m_nFireCount/1000+1;
    float fPercentage = m_nFireCount%1000/1000.0;

    Point pt;
    if (m_wChairID==0 || m_wChairID==1)
    {
        pt.x_ = 88;
        pt.y_ = -1;
    }
    else
    {
        pt.x_ = 88;
        pt.y_ = 95;
    }

    for (int i=0; i<3; i++)
    {
        m_sprBar[i]->set_position(Point(pt.x_+i*62,pt.y_));
        m_sprBar[i]->set_content_size(Size(1, 1));
        m_sprBar[i]->set_src_size(Size(1, 1));
    }

    for (int i=0; i<nCount&&i<3; i++)
    {
        if (i==(nCount-1))
        {
            m_sprBar[i]->set_position(Point(pt.x_+i*62-(31.0-31.0*fPercentage),pt.y_));
            m_sprBar[i]->set_content_size(Size(62*fPercentage, 9));
            m_sprBar[i]->set_src_size(Size(62*fPercentage, 9));
        }
        else
        {
            m_sprBar[i]->set_position(Point(pt.x_+i*62,pt.y_));
            m_sprBar[i]->set_content_size(Size(62, 9));
            m_sprBar[i]->set_src_size(Size(62, 9));
        }
    }


    if (m_nFireCount%1000>970)
    {
        ShowBarAnimation();
    }
} 

void CRoleLayer::FireBomb() 
{
    m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
    m_TaskDate.m_enState = CTaskDate::STATE_NULL;

    m_sprExplorer->set_visible(true);
    m_sprExplorer->set_scale(Size(5.0,5.0));

    Root::instance()->action_manager()->remove_all_action_from_target(m_sprBomb);
    Root::instance()->action_manager()->remove_all_action_from_target(m_sprBomb);

    m_sprBombCannon->set_visible(false);
    m_sprCannon->set_visible(true);
    m_sprBomb->set_visible(false);
    m_sprBombFuse->set_visible(false);

    Animation *ani = Root::instance()->animation_manager()->animation("explorer");
    Action *act = new Action_Sequence(new Action_Animation(0.06, ani, false),new Action_Func(rak::mem_fn(this, &CRoleLayer::FireBombEnd)), 0);
    m_sprExplorer->run_action(act);

    Point ptFish;
    Rect rcScreen(0,0,1280,738);
    CGameScene *pGameScene = (CGameScene *)parent();
    CMD_C_Bomb Bomb;
    Bomb.cbCount = 0;

    Nodes::iterator j=pGameScene->m_layFishObject->child_begin();
    for (; j!=pGameScene->m_layFishObject->child_end(); ++j)
    {
		if (Bomb.cbCount >= MAX_FISH_IN_NET)
			break;

        CFishObjectExtend *pFishObjectExtend = (CFishObjectExtend *)(*j)->node_extend();

        if (pFishObjectExtend->wID == INVALID_WORD)
            continue;

        ptFish = (*j)->position();
        if (rcScreen.pt_in_rect(ptFish))
        {
            Bomb.FishNetObjects[Bomb.cbCount].wID = pFishObjectExtend->wID;
            Bomb.FishNetObjects[Bomb.cbCount].wRoundID = pFishObjectExtend->wRoundID;
            Bomb.FishNetObjects[Bomb.cbCount].wType = pFishObjectExtend->FishType;
            Bomb.FishNetObjects[Bomb.cbCount].dwTime = pFishObjectExtend->dwTime;

            Bomb.cbCount++;

        }
    }

    pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_BOMB, &Bomb, sizeof(CMD_C_Bomb)-(MAX_FISH_IN_NET-Bomb.cbCount)*sizeof(Fish_Net_Object));

		try
		{
			Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(21);
			pSound->play(false, true);
		}
		catch(...)
		{
		}
}

void CRoleLayer::FireLaserBean()
{
    m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
    m_TaskDate.m_enState = CTaskDate::STATE_NULL;

    m_sprLaserBean->set_visible(true);
    m_sprLaserBean->set_rotation(m_sprLaserBeanCannon->rotation());
    m_sprLaserBean->set_scale(Size(2.0, 1.0));

    Action *act = new Action_Sequence(new Action_Scale_To(0.3, 2.0, 6.0), new Action_Scale_To(0.8, 0.1, 6.0), new Action_Func(rak::mem_fn(this, &CRoleLayer::FireLaserBeanEnd)), 0);
    m_sprLaserBean->run_action(act);

	Point ptNet;
    Point ptTNet;
    Point ptFish;
	float sint, cost;


    Rect rcScreen(0,0,1280,738);
    CGameScene *pGameScene = (CGameScene *)parent();
    CMD_C_Laser_Bean LaserBean;
    LaserBean.cbCount = 0;
	LaserBean.fRote = m_sprLaserBeanCannon->rotation();

	ptNet = m_sprLaserBean->position_absolute();
	cost = std::cosf(m_sprLaserBean->rotation());
	sint = std::sinf(m_sprLaserBean->rotation());
	Rect rcNet(Point(0,0), Size(100*2, 1000*2)); 
    Nodes::iterator j=pGameScene->m_layFishObject->child_begin();
    for (; j!=pGameScene->m_layFishObject->child_end(); ++j)
    {
        if (LaserBean.cbCount >= MAX_FISH_IN_NET)
            break;

        CFishObjectExtend *pFishObjectExtend = (CFishObjectExtend *)(*j)->node_extend();

        if (pFishObjectExtend->wID == INVALID_WORD)
            continue;

        ptFish = (*j)->position();

		ptTNet.x_ = (ptFish.x_-ptNet.x_)*cost + (ptFish.y_-ptNet.y_)*sint;
		ptTNet.y_ = -(ptFish.x_-ptNet.x_)*sint + (ptFish.y_-ptNet.y_)*cost;

        if (CFishObjectExtend::ComputeCollision(150, 1500, 100, ptTNet.x_, ptTNet.y_ ))
        {
            LaserBean.FishNetObjects[LaserBean.cbCount].wID = pFishObjectExtend->wID;
            LaserBean.FishNetObjects[LaserBean.cbCount].wRoundID = pFishObjectExtend->wRoundID;
            LaserBean.FishNetObjects[LaserBean.cbCount].wType = pFishObjectExtend->FishType;
            LaserBean.FishNetObjects[LaserBean.cbCount].dwTime = pFishObjectExtend->dwTime;

            LaserBean.cbCount++;

        }
    }

    pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_LASER_BEAN, &LaserBean, sizeof(CMD_C_Laser_Bean)-(MAX_FISH_IN_NET-LaserBean.cbCount)*sizeof(Fish_Net_Object));

		try
		{
			Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(22);
			pSound->play(false, true);
		}
		catch(...)
		{
		}
}

void CRoleLayer::FireBonus()
{
    m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
    m_TaskDate.m_enState = CTaskDate::STATE_NULL;

    CGameScene *pGameScene = (CGameScene *)parent();

    if (pGameScene->GetMeChairID() == m_wChairID)
    {
        CMD_C_Bonus Bonus;
        Bonus.nBonus = m_TaskDate.m_nBonus;

        pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_BONUS, &Bonus, sizeof(CMD_C_Bonus));
    }
}

void CRoleLayer::FireCook()
{
    m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
    m_TaskDate.m_enState = CTaskDate::STATE_NULL;

    CGameScene *pGameScene = (CGameScene *)parent();

    if (pGameScene->GetMeChairID() == m_wChairID)
    {
        CMD_C_Cook Cook;
        Cook.nBonus = m_TaskDate.m_nBonus;

        pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_COOK, &Cook, sizeof(CMD_C_Cook));
    }

    if (m_nCookFish >= m_TaskDate.m_nFishCount)
    {
        m_sprCookComplete->set_visible(false);
        m_sprCookFish->set_visible(false);
        m_sprCookGold->set_visible(false);

        m_sprCookSay->set_color(Color_Rect());
        m_sprCookSay->set_display_image(Root::instance()->imageset_manager()->imageset("cook")->image("say_0"));

        Action *actCookSay = new Action_Sequence(new Action_Delay(1.6), new Action_Fade_Out(1.0), 0);
        m_sprCookSay->run_action(actCookSay);

        m_sprCookSayBack->set_color(Color_Rect());
        Action *actCookSayBack = new Action_Sequence(new Action_Delay(1.6), new Action_Fade_Out(1.0), 0);
        m_sprCookSayBack->run_action(actCookSayBack);

        m_sprCookWin->set_visible(true);
        Animation *aniCookWin = Root::instance()->animation_manager()->animation("cook_win");
        Action *actCookWin = new Action_Sequence(new Action_Animation(0.2, aniCookWin, false), new Action_Func(rak::mem_fn(this, &CRoleLayer::FireCookEnd)), 0);
        m_sprCookWin->run_action(actCookWin);
    }
    else
    {
        m_sprCookComplete->set_visible(false);
        m_sprCookFish->set_visible(false);
        m_sprCookGold->set_visible(false);

        m_sprCookSay->set_color(Color_Rect());
        m_sprCookSay->set_display_image(Root::instance()->imageset_manager()->imageset("cook")->image("say_3"));

        Action *actCookSay = new Action_Sequence(new Action_Delay(1.6), new Action_Fade_Out(1.0), 0);
        m_sprCookSay->run_action(actCookSay);

        m_sprCookSayBack->set_color(Color_Rect());
        Action *actCookSayBack = new Action_Sequence(new Action_Delay(1.6), new Action_Fade_Out(1.0), 0);
        m_sprCookSayBack->run_action(actCookSayBack);

        m_sprCookLost->set_visible(true);
        Animation *aniCookLost = Root::instance()->animation_manager()->animation("cook_lost");
        Action *actCookLost = new Action_Sequence(new Action_Animation(0.2, aniCookLost, false), new Action_Delay(1.2), new Action_Func(rak::mem_fn(this, &CRoleLayer::FireCookEnd)), 0);
        m_sprCookLost->run_action(actCookLost);
    }

    m_nCookFish = 0;
}

bool CRoleLayer::FireCookEnd(Node *node, int tag)
{
    m_sprCookSay->set_color(Color_Rect());
    m_sprCookSay->set_visible(false);
    m_sprCookSayBack->set_color(Color_Rect());
    m_sprCookSayBack->set_visible(false);
    m_sprCookLost->set_visible(false);
    m_sprCookLost->set_color(Color_Rect());
    m_sprCookWin->set_visible(false);
    m_sprCookWin->set_color(Color_Rect());
    m_sprCookNormal->set_color(Color_Rect());
    m_sprCookNormal->set_visible(false);

    m_sprCookBack->set_visible(false);

    return true;
}

void CRoleLayer::NetFireBomb() 
{
    m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
    m_TaskDate.m_enState = CTaskDate::STATE_NULL;

    m_sprExplorer->set_visible(true);
    m_sprExplorer->set_scale(Size(3.0,3.0));

    Root::instance()->action_manager()->remove_all_action_from_target(m_sprBomb);
    Root::instance()->action_manager()->remove_all_action_from_target(m_sprBomb);

    m_sprBombCannon->set_visible(false);
    m_sprCannon->set_visible(true);
    m_sprBomb->set_visible(false);
    m_sprBombFuse->set_visible(false);

    Animation *ani = Root::instance()->animation_manager()->animation("explorer");
    Action *act = new Action_Sequence(new Action_Animation(0.06, ani, false),new Action_Func(rak::mem_fn(this, &CRoleLayer::FireBombEnd)), 0);
    m_sprExplorer->run_action(act);

		try
		{
			Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(21);
			pSound->play(false, true);
		}
		catch(...)
		{
		}
}

void CRoleLayer::NetFireLaserBean(float fRote)
{
    m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
    m_TaskDate.m_enState = CTaskDate::STATE_NULL;

	m_sprCannon->set_rotation(fRote);
    m_sprLaserBeanCannon->set_rotation(fRote);
    m_sprLaserBean->set_visible(true);
    m_sprLaserBean->set_rotation(fRote);
    m_sprLaserBean->set_scale(Size(2.0, 1.0));

    Action *act = new Action_Sequence(new Action_Scale_To(0.3, 2.0, 6.0), new Action_Scale_To(0.8, 0.1, 6.0), new Action_Func(rak::mem_fn(this, &CRoleLayer::FireLaserBeanEnd)), 0);
    m_sprLaserBean->run_action(act);

		try
		{
			Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(22);
			pSound->play(false, true);
		}
		catch(...)
		{
		}
}

void CRoleLayer::NetFireBouns()
{
    m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
    m_TaskDate.m_enState = CTaskDate::STATE_NULL;

}

void CRoleLayer::NetFireCook()
{
    m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
    m_TaskDate.m_enState = CTaskDate::STATE_NULL;

    if (m_nCookFish >= m_TaskDate.m_nFishCount)
    {
        m_sprCookComplete->set_visible(false);
        m_sprCookFish->set_visible(false);
        m_sprCookGold->set_visible(false);

        m_sprCookSay->set_color(Color_Rect());
        m_sprCookSay->set_display_image(Root::instance()->imageset_manager()->imageset("cook")->image("say_0"));

        Action *actCookSay = new Action_Sequence(new Action_Delay(1.6), new Action_Fade_Out(1.0), 0);
        m_sprCookSay->run_action(actCookSay);

        m_sprCookSayBack->set_color(Color_Rect());
        Action *actCookSayBack = new Action_Sequence(new Action_Delay(1.6), new Action_Fade_Out(1.0), 0);
        m_sprCookSayBack->run_action(actCookSayBack);

        m_sprCookWin->set_visible(true);
        Animation *aniCookWin = Root::instance()->animation_manager()->animation("cook_win");
        Action *actCookWin = new Action_Sequence(new Action_Animation(0.2, aniCookWin, false), new Action_Func(rak::mem_fn(this, &CRoleLayer::FireCookEnd)), 0);
        m_sprCookWin->run_action(actCookWin);
    }
    else
    {
        m_sprCookComplete->set_visible(false);
        m_sprCookFish->set_visible(false);
        m_sprCookGold->set_visible(false);

        m_sprCookSay->set_color(Color_Rect());
        m_sprCookSay->set_display_image(Root::instance()->imageset_manager()->imageset("cook")->image("say_3"));

        Action *actCookSay = new Action_Sequence(new Action_Delay(1.6), new Action_Fade_Out(1.0), 0);
        m_sprCookSay->run_action(actCookSay);

        m_sprCookSayBack->set_color(Color_Rect());
        Action *actCookSayBack = new Action_Sequence(new Action_Delay(1.6), new Action_Fade_Out(1.0), 0);
        m_sprCookSayBack->run_action(actCookSayBack);

        m_sprCookLost->set_visible(true);
        Animation *aniCookLost = Root::instance()->animation_manager()->animation("cook_lost");
        Action *actCookLost = new Action_Sequence(new Action_Animation(0.2, aniCookLost, false), new Action_Delay(1.2), new Action_Func(rak::mem_fn(this, &CRoleLayer::FireCookEnd)), 0);
        m_sprCookLost->run_action(actCookLost);
    }

    m_nCookFish = 0;

}

bool CRoleLayer::FireLaserBeanEnd(Node *node, int tag)
{
    m_sprLaserBean->set_visible(false);
    m_sprLaserBeanCannon->set_visible(false);

    return true;
}

bool CRoleLayer::FireBombEnd(Node *node, int tag)
{
    m_sprExplorer->set_visible(false);
    return true;
}

void CRoleLayer::AddCookFish(int nFishType)
{
    if (m_TaskDate.m_enType == CTaskDate::TYPE_COOK && nFishType==m_TaskDate.m_nFishType && 
        m_TaskDate.m_enState == CTaskDate::STATE_RUNNING)
    {
        m_nCookFish ++;
        CGameScene *pGameScene = (CGameScene *)parent();
        if ((pGameScene && pGameScene->GetMeChairID() == m_wChairID) && (m_nCookFish >= m_TaskDate.m_nFishCount && pGameScene))
        {
            FireCook();
        }
    }
}

void CRoleLayer::ClearUp()
{
	m_dwStartTime = 0;

   m_sprBarEnough->set_visible(false);
    m_sprRealFly->set_visible(false);
    m_sprReelBack->set_visible(false);
	for (int i=0; i<3; i++)
       m_sprReelNormal[i]->set_visible(false);

	for (int i=0; i<3; i++)
       m_sprReelFast[i]->set_visible(false);

   m_sprReelEff->set_visible(false);

   for (int i=0; i<4; i++)
      m_sprTaskName[i]->set_visible(false);

    m_sprChangeEffect->set_visible(false);

    m_sprLaserBeanCannon->set_visible(false);
    m_sprLaserBean->set_visible(false);

    m_sprBombCannon->set_visible(false);
    m_sprBomb->set_visible(false);
    m_sprBombFuse->set_visible(false);
    m_sprExplorer->set_visible(false);

    m_sprReward->set_visible(false);

    m_sprCookBack->set_visible(false);
    m_sprCookNormal->set_visible(false);
    m_sprCookLost->set_visible(false);
    m_sprCookWin->set_visible(false);
    m_sprCookSay->set_visible(false);
    m_sprCookFish->set_visible(false);
    m_sprCookSayBack->set_visible(false);
    m_sprCookComplete->set_visible(false);
    m_sprCookGold->set_visible(false);

	int m_nFireCount = 0;
    m_bDelayBarEnough = false;
    m_bShowRewardNubmer = false;
    m_nRewardNumber = 0;
    m_nCookFish = 0;
    m_dwCookStartTime = 0;

	m_TaskDate.m_enType = CTaskDate::TYPE_NULL;
}

////////////////////////////////////////////////////////////