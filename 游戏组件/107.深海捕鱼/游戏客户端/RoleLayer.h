#ifndef __FISH_ROLE_LAYER_H__
#define __FISH_ROLE_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"
#include "GameCore.h"

struct CTaskDate
{
    enum Type 
    {
        TYPE_NULL = -1,
        TYPE_BONUS = 0,
        TYPE_COOK,
        TYPE_BEAN,
        TYPE_BOMB,
        TYPE_COUNT
    };

    enum State
    {
        STATE_NULL = -1,
        STATE_PREPARE1 = 0,
        STATE_PREPARE2,
        STATE_RUNNING ,
        STATE_COMPLETE,
        STATE_COUNT
    };

    Type m_enType;
    State m_enState; 

    int m_nBonus;
    int m_nDuration;

    int m_nStartWheel[3];
    int m_nEndWheel[3];

    int m_nFishType;
    int m_nFishCount;

    CTaskDate() :m_enType(TYPE_NULL), m_enState(STATE_NULL), 
        m_nDuration(0), m_nBonus(0), m_nFishType(0), m_nFishCount(0)
    { 
    }
};
/////////////////////////////////////////////////////////////////
class CRoleLayer : public Layer
{
public:
    CRoleLayer();
    virtual ~CRoleLayer ();

public:
    WORD GetChairID() const { return m_wChairID; }
    void SetChairID(WORD wChairID);

    WORD GetFishGold() const { return m_wFishGold; }
    void SetFishGold(WORD wFishGold);

    CGameCore::enCannonType GetConnonType() const { return m_CannonType; }
    void SetCannonType(CGameCore::enCannonType CannonType) ;

    Point GetCannonPosition() { return m_sprCannon->position_absolute(); }
    float GetCannonRataion()  { return m_sprCannon->rotation(); }
    void SetCannonRatation(const Point &ptMouse);
    void SetCannonRatation(float fRate);

    void SetFireCountDelay(bool bDelay) { m_bDelayBarEnough = bDelay; }
    int GetFireCount() const { return m_nFireCount; }
    void SetFireCount(int nFireCount) ;

    void GunAnimation();
    void GunBaseAnimation();
    void RewardAnimation(int nRewardNumber, bool bDelay);

    void ShowBarAnimation();
    void DestroyShowBarAnimation();

    void UpdateGold(WORD wFishGold);
    bool UpdateGoldEnd(Node *node, int tag);
    bool UpdateGoldSound(Node *node, int tag);
    bool UpdateGoldReal(Node *node, int tag);
    void ShowFishGoldEmpty(bool bShow);

    //virtual void mouse_enter();
    //virtual void mouse_leave();

    void ShowUserInfo(bool bVisible);
    void ShowGunLead(bool bShow);
    void ShowBuyLead(bool bShow);
    void ShowPlayLead(bool bShow);

    void ShowWaringTime(bool bShow);
    bool IsShowWaringTime() ;

    virtual void update(float dt);
    virtual void draw();

    void DrawTimer(std::string &number, const Point &pt);
    void DrawCookTimer(DWORD dwTime, const Point &pt);
    void DrawCookGold(const Point &pt);
    void DrawFishNumber(const Point &pt);
    void DrawBar(int nFireCount);
    void DrawTaskNumber(std::string &number, const Point &pt);

    bool CommandTaskStart(CMD_S_Task *pTask);
    bool BarEnoughEnd(Node *node, int tag);
    bool RealFlyEnd(Node *node, int tag);
    bool TaskNameEnd(Node *node, int tag);
    bool ChangeEffectEnd(Node *node, int tag);
    bool BombMoveEnd(Node *node, int tag);
    bool ReelNormalEnd(Node *node, int tag);
    bool FireLaserBeanEnd(Node *node, int tag);
    bool RewardAnimationEnd1(Node *node, int tag);
    bool RewardAnimationEnd2(Node *node, int tag);
    bool FireBombEnd(Node *node, int tag);
    bool CookNormalEnd(Node *node, int tag);
    bool CookNormalFadeOutEnd(Node *node, int tag);
    bool FireCookEnd(Node *node, int tag);
	bool ReelEffEnd(Node *node, int tag);

    void FireBomb() ;
    void FireLaserBean();
    void FireBonus();
    void FireCook();

    void NetFireBomb() ;
    void NetFireLaserBean(float fRote);
    void NetFireBouns();
    void NetFireCook();

    void AddCookFish(int nFishType);

	void ClearUp();

private:
    Sprite *m_sprGunBase;
    Sprite *m_sprGunOver;
    Sprite *m_sprGunWing1;
    Sprite *m_sprGunWing2;

    Sprite *m_sprGun;
    Sprite *m_sprGunLight;

    Sprite *m_sprShotNumBack;
    Sprite *m_sprNet;

    Sprite *m_sprShotGold;
    std::vector<Sprite *> m_sprGold;

    Sprite *m_sprCannon;

    Sprite *m_sprUserInfo;

    Sprite *m_sprWaringBullet;
    Sprite *m_sprWaringTime;

    Sprite *m_sprGunLead;
    Sprite *m_sprBuyLead;
    Sprite  *m_sprPlayLead;
    Sprite *m_sprLeftKey;
    Sprite *m_sprRighKey;

    Sprite *m_sprCumBar;
    Sprite *m_sprShowBar;

    Font *m_Font;

    DWORD m_dwStartTime;
    Image* m_imgTimer[10];
    Image* m_imgTaskNumber[10];
    Image* m_imgCookTime[11];
    Image* m_imgCookFishNumber[11];
    Image* m_imgCookGold[10];

    Sprite *m_sprBar[3];
    Sprite *m_sprBarBlank[3];
    Sprite *m_sprBarEnough;
    Sprite *m_sprRealFly;
    Sprite *m_sprReelBack;
    Sprite *m_sprReelNormal[3];
    Sprite *m_sprReelFast[3];
    Sprite *m_sprReelEff;

    Sprite *m_sprTaskName[4];
    Sprite *m_sprChangeEffect;

    Sprite *m_sprLaserBeanCannon;
    Sprite *m_sprLaserBean;

    Sprite *m_sprBombCannon;
    Sprite *m_sprBomb;
    Sprite *m_sprBombFuse;
    Sprite *m_sprExplorer;

    Sprite *m_sprReward;

    Sprite *m_sprCookBack;
    Sprite *m_sprCookNormal;
    Sprite *m_sprCookLost;
    Sprite *m_sprCookWin;
    Sprite *m_sprCookSay;
    Sprite *m_sprCookFish;
    Sprite *m_sprCookSayBack;
    Sprite *m_sprCookComplete;
    Sprite *m_sprCookGold;


private:
    WORD m_wChairID;
    WORD m_wFishGold;
    int m_nFireCount;
    bool m_bDelayBarEnough;
    bool m_bShowRewardNubmer;
    int m_nRewardNumber;
    int m_nCookFish;
    DWORD m_dwCookStartTime;

    CGameCore::enCannonType m_CannonType; 

public:
    CTaskDate m_TaskDate;
};

//////////////////////////////////////////////////////////////////////////
#endif