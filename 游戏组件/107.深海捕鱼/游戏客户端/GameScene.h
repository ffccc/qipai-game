#ifndef __FISH_GAME_SCENE_H__
#define __FISH_GAME_SCENE_H__

//////////////////////////////////////////////////////////////////
#include "ClientKernel.h"
#include "..\消息定义\CMD_Fish.h"

#include "xlbe.h"

#include "GameCore.h"
#include "BackgroundLayer.h"
#include "RoleLayer.h"
#include "FishLayer.h"
#include "BulletLayer.h"
#include "NetLayer.h"
#include "FrameLayer.h"
#include "BuyBulletLayer.h"
#include "CoinLayer.h"
#include "MeUserInfoLayer.h"
#include "AccountLayer.h"
#include "CursorLayer.h"
#include "SettingLayer.h"

/////////////////////////////////////////////////////////////////
class CGameScene : public Scene, public CClientKernelSink,  public Window_Event_Listener
{
public:
    CGameScene();
    virtual ~CGameScene();

    //network
public:
    virtual void ResetGameFrame() {}
    virtual void CloseGameFrame() {}
    virtual bool OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize);
    virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize);
    virtual void OnEventUserMemberOrder(tagUserData * pUserData, WORD wChairID, bool bLookonUser) {}
    virtual void OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
    virtual bool OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser) ;
    virtual bool OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUserr);
    virtual void OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser) {}

    //frame
public:
    virtual void window_moved(Render_Window* rw) {}
    virtual void window_focus_change(Render_Window* rw) {}
    virtual void window_resized(Render_Window* rw) {}
    virtual void window_closed(Render_Window* rw); 

public:
    virtual void enter();
    virtual void exit();

    virtual void update(float dt);
    virtual void draw();

public:
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    virtual bool mouseMoved( const OIS::MouseEvent &arg ) ;
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) ;
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) ; 

public:
    bool OnSubGameAddLinePathFish(const void * pBuffer, WORD wDataSize);
    bool OnSubGameAddPointPathFish(const void * pBuffer, WORD wDataSize);
    bool OnSubGameAddGroupPointPathFish(const void * pBuffer, WORD wDataSize);
    bool OnSubGameBuyBulletSuccess(const void * pBuffer, WORD wDataSize);
    bool OnSubGameBuyBulletFailed(const void * pBuffer, WORD wDataSize);
    bool OnSubGameFireSuccess(const void * pBuffer, WORD wDataSize);
    bool OnSubGameFireFailed(const void * pBuffer, WORD wDataSize);
    bool OnSubGameChangeCannon(const void * pBuffer, WORD wDataSize);
    bool OnSubGameCastNetSuccess(const void * pBuffer, WORD wDataSize);
    bool OnSubGameCastNetFailed(const void * pBuffer, WORD wDataSize);
    bool OnSubGameChangeScene(const void * pBuffer, WORD wDataSize);
    bool OnSubGameAccount(const void * pBuffer, WORD wDataSize);
    bool OnSubGameAddSpecialPointPathFish(const void * pBuffer, WORD wDataSize);
    bool OnSubGameTask(const void * pBuffer, WORD wDataSize);
	bool OnSubGameLaserBeanSucess(const void * pBuffer, WORD wDataSize);
	bool OnSubGameBombSucess(const void * pBuffer, WORD wDataSize);
  bool OnSubBonus(const void * pBuffer, WORD wDataSize);
  bool OnSubGameCook(const void * pBuffer, WORD wDataSize);

public:
    WORD GetMeChairID();
	bool IsAndroidLogicChairID() { return m_bAndroidLogicChairID; }
    CClientKernel *GetClientKernel() { return m_pClientKernel; }

private:
    bool m_bIsConnect;
    CClientKernel *m_pClientKernel;

public:
    CBackgroundLayer* m_layBackground;
    CFishLayer *m_layFishObject;
    CNetLayer *m_layNetObject;
    CBulletLayer *m_layBulletObject;
    CCoinLayer *m_layCoinObject;
    std::vector<CRoleLayer*> m_layRoles;
    CMeUserInfoLayer *m_layMeUserInfo;
    CBuyBulletLayer *m_layBuyBulletLayer;
    CAccountLayer *m_layAccount;
    CFrameLayer *m_layFrame;
    CCursorLayer *m_layCursor;
	CSettingLayer *m_laySetting;

    Timer m_Timer;

    bool m_bFirstGame;
    bool m_bFireBullet;
	bool m_bAndroidLogicChairID;
};

//////////////////////////////////////////////////////////////////////////
#endif