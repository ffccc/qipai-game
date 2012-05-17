#ifndef __FISH_SCENE_H__
#define __FISH_SCENE_H__

//////////////////////////////////////////////////////////////////
#include "scene/scene.h"
#include "scene/scene_layer.h"
#include "scene/sprite.h"
#include "action/action_interval.h"
#include "action/action_combine.h"
#include "action/action_instant.h"
#include "frame/root.h"
#include "frame/window_event_utilities.h"

#include "directx9GUIRenderer/d3d9renderer.h"
#include "CEGUIDefaultResourceProvider.h"
#include "CEGUI.h"
#include "CEGUIWindowManager.h"
#include "CEGUIImagesetManager.h"

#include "..\消息定义\CMD_Fish.h"
#include "GameLogic.h"
#include "ClientKernel.h"

#include <vector>

/////////////////////////////////////////////////////////////////
class CFishScene : public Scene, public CClientKernelSink, public Window_Event_Listener, public Frame_Listener
{
public:
    CFishScene();
    virtual ~CFishScene();

    //scene
protected:
    virtual void enter();
    virtual void visit(float dt);

    //frame
public:
    virtual void window_moved(Render_Window* rw);
    virtual void window_focus_change(Render_Window* rw);
    virtual void window_resized(Render_Window* rw);
    virtual void window_closed(Render_Window* rw);

public:
    virtual bool frame_started(const Frame_Event& evt);	
    virtual bool frame_render_targets(const Frame_Event& evt);
    virtual bool frame_ended(const Frame_Event& evt);		

    //input
public:
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg ) ;

    virtual bool mouseMoved( const OIS::MouseEvent &arg ) ;
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) ;

    //network
public:
    virtual void ResetGameFrame();
    virtual void CloseGameFrame();
    virtual bool OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize);
    virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize);
    virtual void OnEventUserMemberOrder(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
    virtual void OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
    virtual bool OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
    virtual bool OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUserr);
    virtual void OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

    //game message
public: 
    bool OnSubGameFire(const void * pBuffer, WORD wDataSize);
    bool OnSubGameBuyBullet(const void * pBuffer, WORD wDataSize);
    bool OnSubGameChangeCannon(const void * pBuffer, WORD wDataSize);
    bool OnSubGameAddFish(const void * pBuffer, WORD wDataSize);
    bool OnSubGameSceneAddFish(const void * pBuffer, WORD wDataSize);
    bool OnSubGameCapture(const void * pBuffer, WORD wDataSize);
    bool OnSubGameChangeScene(const void * pBuffer, WORD wDataSize);
    bool OnSubGameAccount(const void * pBuffer, WORD wDataSize);

    //send message
public:
    bool SendBuyBullet(int nBulletStyle);
    bool SendChangeCannon(int nCannonStyle);
    bool SendFireBullet();
    bool SendCaptureFish(std::vector<int> &vecFish);

    //callback
public:
    bool BulletEnd(Scene_Node *node, int tag);
    bool FishNetEnd(Scene_Node *node, int tag);
    bool FishEnd(Scene_Node *node, int tag);
    bool FishCaptureEnd(Scene_Node *node, int tag);
    bool CoinEnd(Scene_Node *node, int tag);
    bool ValueEnd(Scene_Node *node, int tag);
    bool TidewaterEnd(Scene_Node *node, int tag);

    //ui
public:
    bool HandleBtnBulletBuyBuy(const CEGUI::EventArgs& args);
    bool HandleBtnBulletBuyExit(const CEGUI::EventArgs& args);
    bool HandleBtnControl(const CEGUI::EventArgs& args);
    bool HandleCloseWin(const CEGUI::EventArgs& args);
    bool HandleCheckBullet(const CEGUI::EventArgs& args);
    bool HandleSettingOk(const CEGUI::EventArgs& args);
    bool HandleSettingCannel(const CEGUI::EventArgs& args);
    bool HandleBtnContinueGame(const CEGUI::EventArgs& args);
    bool HandleBtnExitGame(const CEGUI::EventArgs& args);

    //input
public:
    bool InputFire();

    //operate
protected:
    void CreateScene();
    void CreateUI();
    void ChangeScene();
    void ChangeCannon(int nChairID, int nCannonStyle); 
    void RotateCannon(int nChairID, const Point& ptMouse); 
    void FireBullet(int nChairID, int nStyle, float fRote);
    void FireFishnet(int nChairID, Point ptPosition);
    void AddFish(int nID, int nStyle, int nPath, int nFollowX, int nFollowY);
    void AddSceneFish(int nID, int nStyle, const Point &start, const Point &end, float fDelay);

protected:
    void PlaySample(const std::string &id);

private:
    Sprite *m_spTideWater;
    Sprite *m_spBoat[4];
    Sprite *m_spCannon[4];
    Sprite *m_spCannontag[4];
    Sprite *m_spCannonnum[4];
    Sprite *m_spWarn[4];

    Scene_Layer *m_layRole;
    Scene_Layer *m_layCoin;
    Scene_Layer *m_layFishNet;
    Scene_Layer *m_layBullet;
    Scene_Layer *m_layFish;
    Scene_Layer *m_layBackground[2];

    Timer m_Timer;
    Timer m_UITimer;
    int m_nUITime;
    int m_nScene;
    int m_nLastFire;

    CGameLogic m_GameLogic;
    CClientKernel *m_ClientKernel;
    RoleNetObject m_RoleObject[GAME_PLAYER];

    std::vector<Scene_Node *> m_NodeDelete;

    int m_nFishCount[11];

private:
    Sound_Instance*		sound_scene_[2];
    Sound_Instance*		m_TideWater;

    int m_bSound;
    int m_bMusic;
    double m_fSound;
    double m_fMusic;

private:
    CEGUI::DirectX9Renderer* m_ceguiRenderer;
    CEGUI::System* m_ceguiSystem;

    bool m_bMouseRaw;
};

//////////////////////////////////////////////////////////////////////////
#endif