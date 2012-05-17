#include "GameScene.h"


#include "FishApplication.h"

/////////////////////////////////////////////////////////////
CGameScene::CGameScene()
:m_layBackground(0),
m_layFishObject(0),
m_layRoles(GAME_PLAYER)
{
    //////////////////////////////////////////////////////////////////////
    m_pClientKernel = new CClientKernel;

    Window_Event_Utilities::add_event_listener(this);	

    m_Timer.reset();

    /////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////
    m_layBackground = new CBackgroundLayer;
    m_layBackground->set_position(Point(4,31));
    m_layBackground->set_content_size(Size(1272, 703));
    m_layBackground->set_disable(true);
    add_child(m_layBackground);

    m_layFishObject = new CFishLayer;
    m_layFishObject->set_disable(true);
    m_layFishObject->set_content_size(Size(1280, 738)); 
    add_child(m_layFishObject);

    m_layNetObject = new CNetLayer;
    m_layNetObject->set_disable(true);
    m_layNetObject->set_content_size(Size(1280, 738)); 
    add_child(m_layNetObject);

    m_layBulletObject = new CBulletLayer;
    m_layBulletObject->set_disable(true);
    m_layBulletObject->set_content_size(Size(1280, 738)); 
    add_child(m_layBulletObject);

    m_layCoinObject = new CCoinLayer;
    m_layCoinObject->set_disable(true);
    m_layCoinObject->set_content_size(Size(1280, 738)); 
    add_child(m_layCoinObject);

    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_layRoles[i] = new CRoleLayer;
        m_layRoles[i]->set_disable(true);
        m_layRoles[i]->set_content_size(Size(300,96));
        add_child(m_layRoles[i]); 

        m_layRoles[i]->set_visible(false);
    }

    m_layRoles[0]->set_position(Point(200,31+8));
    m_layRoles[1]->set_position(Point(780,31+8));
    m_layRoles[2]->set_position(Point(780,638-8));
    m_layRoles[3]->set_position(Point(200,638-8));

    m_layMeUserInfo = new CMeUserInfoLayer;
    m_layMeUserInfo->set_content_size(Size(253,173));
    m_layMeUserInfo->add_widget(this);
    m_layMeUserInfo->set_disable(true);
    m_layMeUserInfo->resize(Point(514,560+148), Size(253,173));

    m_layBuyBulletLayer = new CBuyBulletLayer;
    m_layBuyBulletLayer->set_position(Point(388,257));
    m_layBuyBulletLayer->set_content_size(Size(503, 223)); 
    m_layBuyBulletLayer->add_widget(this);
    m_layBuyBulletLayer->ShowWidnow(false);

    m_layAccount = new CAccountLayer;
    m_layAccount->set_content_size(Size(787,456));
    m_layAccount->add_widget(this);
    m_layAccount->resize(Point(246,140), Size(787,456));
    m_layAccount->ShowWidnow(false);

	m_laySetting = new CSettingLayer;
    m_laySetting->set_content_size(Size(503,265));
    m_laySetting->add_widget(this);
    m_laySetting->resize(Point(388,140), Size(503,265));
    m_laySetting->ShowWidnow(false);

    m_layFrame = new CFrameLayer(1);
    m_layFrame->set_position(Point(0,0));
    m_layFrame->set_content_size(Size(1280, 738)); 
    m_layFrame->set_disable(true);
    m_layFrame->add_widget(this);

    m_layCursor = new CCursorLayer;
    m_layCursor->set_position(Point(0,0));
    m_layCursor->set_content_size(Size(80, 80));
    m_layFrame->set_disable(true);
    m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("role")->image("cursor_fire"));
    add_child(m_layCursor);

    m_bFirstGame = true;
     m_Timer.reset();

    m_bFireBullet = false;
	m_bAndroidLogicChairID = false;
}

CGameScene::~CGameScene()
{
}

void CGameScene::enter()
{
    __super::enter();

    HWND hWnd = Root::instance()->render_window()->window_handle();  
    m_bIsConnect = m_pClientKernel->InitClientKernel(hWnd, theApp->cmd_line(), this);
}

void CGameScene::exit()
{
    __super::exit();
}

void CGameScene::window_closed(Render_Window* rw)
{
    if (m_pClientKernel)
    {
        m_bIsConnect = false;

        delete m_pClientKernel;
        m_pClientKernel = NULL;
    }
}

void CGameScene::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
}

bool CGameScene::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser) 
{
    m_layRoles[wChairID]->SetChairID(INVALID_CHAIR);
    m_layRoles[wChairID]->SetFishGold(0);
    m_layRoles[wChairID]->SetCannonType(CGameCore::CannonTypeCount);

    m_layRoles[wChairID]->set_visible(false);

	m_layRoles[wChairID]->ClearUp();

    return true;
}

bool CGameScene::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUserr)
{
    m_layRoles[wChairID]->SetChairID(wChairID);
    m_layRoles[wChairID]->SetFishGold(0);
    m_layRoles[wChairID]->SetCannonType(CGameCore::CannonType_0);
	m_layRoles[wChairID]->SetFireCount(0);

    m_layRoles[wChairID]->set_visible(true);

    if (wChairID == GetMeChairID())
        m_layRoles[wChairID]->ShowGunLead(true);

    return true;
}

bool CGameScene::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
    CMD_S_StatusFree *pStatusFree = (CMD_S_StatusFree *)pBuffer;

    m_layBackground->SetSceneType(CGameCore::ByteToSceneType(pStatusFree->cbScene));
    m_layBuyBulletLayer->SetCellScore(pStatusFree->lCellScore);

    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_layRoles[i]->SetChairID(pStatusFree->RoleObjects[i].wID); 
        m_layRoles[i]->SetFishGold(pStatusFree->RoleObjects[i].wFishGold);
        m_layRoles[i]->SetCannonType(CGameCore::WordToCannonType(pStatusFree->RoleObjects[i].wCannonType));
        m_layRoles[i]->SetFireCount(pStatusFree->RoleObjects[i].wFireCount);

        if (m_layRoles[i]->GetChairID() != INVALID_CHAIR)
            m_layRoles[i]->set_visible(true);
    }

    m_layRoles[GetMeChairID()]->ShowBarAnimation();

    return true;
}

bool CGameScene::OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize)
{
    if (MDM_GF_GAME !=Command.wMainCmdID) 
        return false;

    if (Command.wSubCmdID == SUB_S_SEND_LINE_PATH_FISH)
    {
        return OnSubGameAddLinePathFish(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_SEND_POINT_PATH_FISH)
    {
        return OnSubGameAddPointPathFish(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_BUY_BULLET_SUCCESS)
    {
        return OnSubGameBuyBulletSuccess(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_BUY_BULLET_FAILED)
    {
        return OnSubGameBuyBulletFailed(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_FIRE_SUCCESS)
    {
        return OnSubGameFireSuccess(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_FIRE_FAILED)
    {
        return OnSubGameFireFailed(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_CHANGE_CANNON)
    {
        return OnSubGameChangeCannon(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_CAST_NET_SUCCESS)
    {
        return OnSubGameCastNetSuccess(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_CAST_NET_FAILED)
    {
        return OnSubGameCastNetFailed(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_CHANGE_SCENE)
    {
        return OnSubGameChangeScene(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_ACCOUNT)
    {
        return OnSubGameAccount(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_SEND_GROUP_POINT_PATH_FISH)
    {
        return OnSubGameAddGroupPointPathFish(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_SEND_SPECIAL_POINT_PATH)
    {
        return OnSubGameAddSpecialPointPathFish(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_TASK)
    {
        return OnSubGameTask(pBuffer, wDataSize);
    }
	else if (Command.wSubCmdID == SUB_S_LASER_BEAN_SUCCESS)
    {
        return OnSubGameLaserBeanSucess(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_BOMB_SUCCESS)
    {
        return OnSubGameBombSucess(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_BONUS)
    {
        return OnSubBonus(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_COOK)
    {
        return OnSubGameCook(pBuffer, wDataSize);
    }


    return true;
}

bool CGameScene::OnSubGameAddLinePathFish(const void * pBuffer, WORD wDataSize)
{
    //if (wDataSize!=sizeof(SendLinePathFish)-(MAX_FISH_SEND-SendLinePathFish.cbCount)*sizeof(Fish_With_Line_Path)) return false;

    CMD_S_Send_Line_Path_Fish *pSendLinePathFish = (CMD_S_Send_Line_Path_Fish *)pBuffer;

    return m_layFishObject->NetAddLinePathFish(pSendLinePathFish);
}

bool CGameScene::OnSubGameAddSpecialPointPathFish(const void * pBuffer, WORD wDataSize)
{
    CMD_S_Send_Special_Point_Path *pSendSpecialPointPathFish = (CMD_S_Send_Special_Point_Path *)pBuffer;

    return m_layFishObject->NetAddSpecialPointPathFish(pSendSpecialPointPathFish);
}

bool CGameScene::OnSubGameTask(const void * pBuffer, WORD wDataSize)
{
    CMD_S_Task *pTask = (CMD_S_Task *)pBuffer;

    WORD wChairID = pTask->wChairID;

    m_layRoles[wChairID]->CommandTaskStart(pTask);

    if (pTask->nTask != CTaskDate::TYPE_NULL)
    {
        m_layRoles[wChairID]->SetFireCountDelay(true);
        m_layRoles[wChairID]->SetFireCount(0);
    }

    return true;
}

bool CGameScene::OnSubGameAddPointPathFish(const void * pBuffer, WORD wDataSize)
{
    CMD_S_Send_Point_Path_Fish *pSendPointPathFish = (CMD_S_Send_Point_Path_Fish *)pBuffer;

    return m_layFishObject->NetAddPointPathFish(pSendPointPathFish);
}

bool CGameScene::OnSubGameAddGroupPointPathFish(const void * pBuffer, WORD wDataSize)
{
    CMD_S_Send_Group_Point_Path_Fish *pSendGroupPointPathFish = (CMD_S_Send_Group_Point_Path_Fish *)pBuffer;

    return m_layFishObject->NetAddGroupPointPathFish(pSendGroupPointPathFish);
}

bool CGameScene::OnSubGameBuyBulletSuccess(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Buy_Bullet_Success)) return false;

    CMD_S_Buy_Bullet_Success *pBuyBulletSuccess = (CMD_S_Buy_Bullet_Success *)pBuffer;

    WORD wChairID = pBuyBulletSuccess->wChairID;
    WORD wCount = pBuyBulletSuccess->wCount;

    if (wChairID == GetMeChairID())
    {
        m_layBuyBulletLayer->SetSendBuyBulletMessage(false);
        if (m_bFirstGame)
        {
            m_layRoles[wChairID]->ShowPlayLead(true);
            m_layRoles[wChairID]->ShowBuyLead(false);
        }
    }

    m_layRoles[wChairID]->ShowFishGoldEmpty(false);
    m_layRoles[wChairID]->SetFishGold(m_layRoles[wChairID]->GetFishGold()+wCount);

    return true;
}

bool CGameScene::OnSubGameBuyBulletFailed(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Buy_Bullet_Failed)) return false;

    CMD_S_Buy_Bullet_Failed *pBuyBulletFailed = (CMD_S_Buy_Bullet_Failed *)pBuffer;

    WORD wChairID = pBuyBulletFailed->wChairID;

    if (wChairID == GetMeChairID())
    {
        m_layBuyBulletLayer->SetSendBuyBulletMessage(false);
    }

    return true;
}

bool CGameScene::OnSubGameFireSuccess(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Fire_Success)) return false;

    CMD_S_Fire_Success *pFireSuccess = (CMD_S_Fire_Success *)pBuffer;

    WORD wChairID = pFireSuccess->wChairID;
    float fRote = pFireSuccess->fRote;

	m_bAndroidLogicChairID = (pFireSuccess->wAndroidLogicChairID == GetMeChairID());

    if (wChairID != GetMeChairID())
    {
        m_layRoles[wChairID]->SetCannonRatation(fRote);
        m_layBulletObject->BulletFire(m_layRoles[wChairID]->GetCannonPosition(), fRote, wChairID, m_layRoles[wChairID]->GetConnonType());
        m_layRoles[wChairID]->GunAnimation();
        m_layRoles[wChairID]->GunBaseAnimation();

        WORD wFishGold = m_layRoles[wChairID]->GetConnonType()+1;
        if (wFishGold >= m_layRoles[wChairID]->GetFishGold())
        {
            wFishGold=0;
        }
        else
        {
            wFishGold = m_layRoles[wChairID]->GetFishGold() - wFishGold;
        }
        if (wFishGold <= 10)
        {
            m_layRoles[wChairID]->ShowFishGoldEmpty(true);
        }
        else
        {
            m_layRoles[wChairID]->ShowFishGoldEmpty(false);
        }
        
        m_layRoles[wChairID]->SetFishGold(wFishGold);

    }

    WORD wFishGold = m_layRoles[wChairID]->GetConnonType()+1;
	m_layRoles[wChairID]->SetFireCount(pFireSuccess->nFireCount);


    return true;
}

bool CGameScene::OnSubGameFireFailed(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Fire_Failed)) return false;

    //MessageBox(0,"OnSubGameFireFailed","OnSubGameFireFailed",0);

    return true;
}

bool CGameScene::OnSubGameChangeCannon(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Change_Cannon)) return false;

    CMD_S_Change_Cannon *pChangeCannon = (CMD_S_Change_Cannon *)pBuffer;

    WORD wChairID = pChangeCannon->wChairID;
    WORD wStyle = pChangeCannon->wStyle;

    if (wChairID != GetMeChairID())
    {
        m_layRoles[wChairID]->SetCannonType(CGameCore::WordToCannonType(wStyle));
    }

    return true;
}

bool CGameScene::OnSubGameCastNetSuccess(const void * pBuffer, WORD wDataSize)
{
    CMD_S_Cast_Net_Success *pCastNetSuccess = (CMD_S_Cast_Net_Success *)pBuffer;

    if (wDataSize!=sizeof(CMD_S_Cast_Net_Success)-(MAX_FISH_IN_NET-pCastNetSuccess->cbCount)*sizeof(Fish_Net_Object)) return false;

    BYTE cbCount = pCastNetSuccess->cbCount;
    WORD wChairID = pCastNetSuccess->wChairID;
    WORD wFishID;

    int nCoinCount = 0;

    for (BYTE i=0; i<cbCount; i++)
    {
        wFishID = pCastNetSuccess->FishNetObjects[i].wID;

        nCoinCount += m_layFishObject->FishCaptured(wFishID, wChairID);    
    }

    if (nCoinCount>0)
        m_layRoles[wChairID]->UpdateGold(nCoinCount);

    return true;

}

bool CGameScene::OnSubGameLaserBeanSucess(const void * pBuffer, WORD wDataSize)
{
	CMD_S_Laser_Bean_Success *pLaserBeanuccess = (CMD_S_Laser_Bean_Success *)pBuffer;

	if (wDataSize!=sizeof(CMD_S_Laser_Bean_Success)-(MAX_FISH_IN_NET-pLaserBeanuccess->cbCount)*sizeof(Fish_Net_Object)) return false;

	BYTE cbCount = pLaserBeanuccess->cbCount;
	WORD wChairID = pLaserBeanuccess->wChairID;
	WORD wFishID;

	int nCoinCount = 0;

  if (GetMeChairID() != pLaserBeanuccess->wChairID)
	     m_layRoles[wChairID]->NetFireLaserBean(pLaserBeanuccess->fRote);

	for (BYTE i=0; i<cbCount; i++)
	{
		wFishID = pLaserBeanuccess->FishNetObjects[i].wID;

		nCoinCount += m_layFishObject->FishCaptured(wFishID, wChairID);    
	}

  
	if (nCoinCount>0)
	{
		m_layRoles[wChairID]->UpdateGold(nCoinCount);
		m_layRoles[wChairID]->RewardAnimation(nCoinCount, true);
	}

	return true;
}

bool CGameScene::OnSubGameBombSucess(const void * pBuffer, WORD wDataSize)
{
	CMD_S_Bomb_Success *pBombSuccess = (CMD_S_Bomb_Success *)pBuffer;

	if (wDataSize!=sizeof(CMD_S_Bomb_Success)-(MAX_FISH_IN_NET-pBombSuccess->cbCount)*sizeof(Fish_Net_Object)) return false;

	BYTE cbCount = pBombSuccess->cbCount;
	WORD wChairID = pBombSuccess->wChairID;
	WORD wFishID;

	int nCoinCount = 0;

  if (GetMeChairID() != pBombSuccess->wChairID)
      m_layRoles[wChairID]->NetFireBomb();

	for (BYTE i=0; i<cbCount; i++)
	{
		wFishID = pBombSuccess->FishNetObjects[i].wID;

		nCoinCount += m_layFishObject->FishCaptured(wFishID, wChairID);    
	}

	if (nCoinCount>0)
	{
		m_layRoles[wChairID]->UpdateGold(nCoinCount);
		m_layRoles[wChairID]->RewardAnimation(nCoinCount, true);
	}

	return true;
}

bool CGameScene::OnSubBonus(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Bonus)) return false;

    CMD_S_Bonus *pBonus = (CMD_S_Bonus *)pBuffer;

    if (GetMeChairID() != pBonus->wChairID)
      m_layRoles[pBonus->wChairID]->NetFireBouns();

    m_layRoles[pBonus->wChairID]->UpdateGold(pBonus->nBonus);
    m_layRoles[pBonus->wChairID]->RewardAnimation(pBonus->nBonus, false);

    return true;
}

bool CGameScene::OnSubGameCook(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Cook)) return false;

    CMD_S_Cook *pCook = (CMD_S_Cook *)pBuffer;

    if (GetMeChairID() != pCook->wChairID)
      m_layRoles[pCook->wChairID]->NetFireCook();

    if (pCook->cbSucess)
    {
        m_layRoles[pCook->wChairID]->UpdateGold(pCook->nBonus);
        m_layRoles[pCook->wChairID]->RewardAnimation(pCook->nBonus, true);
    }

    return true;
}

bool CGameScene::OnSubGameCastNetFailed(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Cast_Net_Failed)) return false;


        return true;
}

bool CGameScene::OnSubGameChangeScene(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Change_Scene)) return false;

    CMD_S_Change_Scene *pChangeScene = (CMD_S_Change_Scene *)pBuffer;

    m_layFishObject->SpeedUpFishObject(10.0);
    m_layBackground->ChangeSceneType(CGameCore::ByteToSceneType(pChangeScene->cbScene));

        return true;
}

bool CGameScene::OnSubGameAccount(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_Account)) return false;

    CMD_S_Account *pAccount = (CMD_S_Account *)pBuffer;
    if (pAccount->wChairID == GetMeChairID())
    {
        m_layAccount->DisableWindow(false);
        m_layAccount->ShowWidnow(false);
    }

    m_layRoles[pAccount->wChairID]->SetFishGold(0);

    return true;
       
}

WORD CGameScene::GetMeChairID()
{
    if (m_bIsConnect && m_pClientKernel)
    {
        return m_pClientKernel->GetMeChairID();
    }

    return INVALID_CHAIR;
}


bool CGameScene::keyPressed( const OIS::KeyEvent &arg )
{
    return __super::keyPressed(arg);
}

bool CGameScene::keyReleased( const OIS::KeyEvent &arg )
{
     return __super::keyReleased(arg);
}

bool CGameScene::mouseMoved( const OIS::MouseEvent &arg ) 
{
    int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
    int nFullHeight=GetSystemMetrics(SM_CYSCREEN);   

    Point ptOffset1(arg.state.X.abs, arg.state.Y.abs);

    if (nFullWidth < 1280)
    {
        ptOffset1.x_ = arg.state.X.abs*(1280.0/1024.0);
        ptOffset1.y_ = arg.state.Y.abs*(738.0/590.0);
    }

    Point pt(ptOffset1);

    m_layCursor->set_position(pt);

    bool bHandle = false;
    Point ptUI = m_layBuyBulletLayer->position();
    Size szUI = m_layBuyBulletLayer->content_size();
    Rect rcUI(ptUI, szUI);
    if (m_layBuyBulletLayer->visible() && rcUI.pt_in_rect(pt))
    {
        bHandle = true;
        m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("role")->image("cursor_hand"));
    }

    rcUI.origin_ = m_layAccount->position();
    rcUI.size_ = m_layAccount->content_size();
    if (m_layAccount->visible() && rcUI.pt_in_rect(pt))
    {
        bHandle = true;
        m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("role")->image("cursor_hand"));
    }

     rcUI.origin_ = m_layMeUserInfo->position();
    rcUI.size_ = m_layMeUserInfo->content_size();
    if (m_layMeUserInfo->visible() && rcUI.pt_in_rect(pt))
    {
        bHandle = true;
        m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("role")->image("cursor_hand"));
    }

    rcUI.origin_ = m_laySetting->position();
    rcUI.size_ = m_laySetting->content_size();
    if (m_laySetting->visible() && rcUI.pt_in_rect(pt))
    {
        bHandle = true;
        m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("role")->image("cursor_hand"));
    }

    if (!bHandle)
    {
        m_layCursor->SetCursor(Root::instance()->imageset_manager()->imageset("role")->image("cursor_fire"));
    }

    if (GetMeChairID() != INVALID_CHAIR)
    {
        
        m_layRoles[GetMeChairID()]->SetCannonRatation(pt);
    }

    Rect rc;
    for (int i=0; i<GAME_PLAYER; i++)
    {
        if (GetMeChairID()!=i)
        {
            rc.origin_ =  m_layRoles[i]->position();
            rc.size_ = m_layRoles[i]->content_size();
            rc.origin_.x_ += 50;
            rc.size_.width_ -= 100;

            if (rc.pt_in_rect(pt))
            {
                m_layRoles[i]->ShowUserInfo(true);
            }
            else
            {
                m_layRoles[i]->ShowUserInfo(false);
            }
        }
          
    }

   /*  return __super::mouseMoved(arg1);*/

    if (mouse_down_)
    {
        mouse_drag(Point(pt.x_, pt.y_));

        return true;
    }

    mouse_position(Point(pt.x_, pt.y_));

    return true;
}

bool CGameScene::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) 
{
    int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
    int nFullHeight=GetSystemMetrics(SM_CYSCREEN);   

    Point ptOffset1(arg.state.X.abs, arg.state.Y.abs);


    if (nFullWidth < 1280)
    {
        ptOffset1.x_ = arg.state.X.abs*(1280.0/1024.0);
        ptOffset1.y_ = arg.state.Y.abs*(738.0/590.0);
    }

    Point pt(ptOffset1);
    Rect rcScreen(4,31,1272,703);

    Point pos;
    Node *node = node_by_position(pt, pos);

    m_bFireBullet = false;
    if (rcScreen.pt_in_rect(pt) && !node)
    {
        if (id == 0 )
        {
            m_bFireBullet = true;

            WORD wMeChairID = GetMeChairID();
            if (wMeChairID != INVALID_CHAIR)
            {
				if (!m_layAccount->visible() && (m_layRoles[wMeChairID]->m_TaskDate.m_enType==CTaskDate::TYPE_BEAN&&m_layRoles[wMeChairID]->m_TaskDate.m_enState==CTaskDate::STATE_RUNNING))
				{
					    m_layRoles[wMeChairID]->FireLaserBean();
						m_bFireBullet = false;
				}
				else if (!m_layAccount->visible() &&(m_layRoles[wMeChairID]->m_TaskDate.m_enType==CTaskDate::TYPE_BOMB&&m_layRoles[wMeChairID]->m_TaskDate.m_enState==CTaskDate::STATE_RUNNING))
				{
					    m_layRoles[wMeChairID]->FireBomb();
						m_bFireBullet = false;
				}

				else  if (m_layRoles[wMeChairID]->GetFishGold() <= 0 && !m_layAccount->visible())
                {
                    if (!m_layBuyBulletLayer->IsSendBuyBulletMessage())
                        m_layBuyBulletLayer->ShowWidnow(true);

                    if (m_bFirstGame)
                    {
                        m_layRoles[wMeChairID]->ShowBuyLead(true);
                        m_layRoles[wMeChairID]->ShowGunLead(false);
                        m_layRoles[wMeChairID]->ShowPlayLead(false);
                    }
                }
				else if (!m_layAccount->visible() && m_layRoles[wMeChairID]->GetFishGold()>0 &&
            ((m_layRoles[wMeChairID]->m_TaskDate.m_enState<CTaskDate::STATE_PREPARE2)||((m_layRoles[wMeChairID]->m_TaskDate.m_enType==CTaskDate::TYPE_COOK&&m_layRoles[wMeChairID]->m_TaskDate.m_enState==CTaskDate::STATE_RUNNING))))
                {
                    unsigned long time = m_Timer.get_milli_seconds();
                    if (time < 400)
                        return true;

                    m_Timer.reset();
                    m_layRoles[GetMeChairID()]->ShowWaringTime(false);

                    if (m_bFirstGame)
                    {
                        m_bFirstGame = false;

                        m_layRoles[wMeChairID]->ShowGunLead(false);
                        m_layRoles[wMeChairID]->ShowBuyLead(false);
                        m_layRoles[wMeChairID]->ShowPlayLead(false);

                        m_layRoles[wMeChairID]->DestroyShowBarAnimation();
                    }

                    WORD wFishGold = m_layRoles[wMeChairID]->GetConnonType()+1;
                    if (wFishGold >= m_layRoles[wMeChairID]->GetFishGold())
                    {
                         wFishGold=0;
                    }
                    else
                    {
                        wFishGold = m_layRoles[wMeChairID]->GetFishGold() - wFishGold;
                    }

                    m_layBulletObject->BulletFire(m_layRoles[wMeChairID]->GetCannonPosition(), m_layRoles[wMeChairID]->GetCannonRataion(), wMeChairID, m_layRoles[wMeChairID]->GetConnonType());
                    m_layRoles[wMeChairID]->GunAnimation();
                    m_layRoles[wMeChairID]->GunBaseAnimation();
                    if (wFishGold <= 10)
                    {
                        m_layRoles[wMeChairID]->ShowFishGoldEmpty(true);
                    }
                    else
                    {
                        m_layRoles[wMeChairID]->ShowFishGoldEmpty(false);
                    }
                    m_layRoles[wMeChairID]->SetFishGold(wFishGold);
                }
            }


        }
        else if (id == 1)
        {
            WORD wMeChairID = GetMeChairID();
            if (wMeChairID != INVALID_CHAIR && ((m_layRoles[wMeChairID]->m_TaskDate.m_enState<CTaskDate::STATE_PREPARE2)||((m_layRoles[wMeChairID]->m_TaskDate.m_enType==CTaskDate::TYPE_COOK&&m_layRoles[wMeChairID]->m_TaskDate.m_enState==CTaskDate::STATE_RUNNING))))
            {
                try
                {
                    Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(17);
                    pSound->play(false, true);
                }
                catch(...)
                {
                }
                m_layRoles[wMeChairID]->SetCannonType(CGameCore::WordToCannonType((m_layRoles[wMeChairID]->GetConnonType()+1)%CGameCore::CannonTypeCount));
            }   
        }
    }


    //return __super::mousePressed(arg1, id);


    Point mouse(ptOffset1);

    mouse_position(mouse);

    mouse_down_ = true;

    node = node_by_position(mouse, pos);

    last_down_node_ = node;

    if (node != 0)
    {
        if (((Layer*)(node))->wants_focus())
            set_focus(node);

        ((Layer*)(node))->mouse_down(pos, 0, 0);
    }

    return true;
}

bool CGameScene::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
    int nFullHeight=GetSystemMetrics(SM_CYSCREEN);   

    Point ptOffset1(arg.state.X.abs, arg.state.Y.abs);

m_bFireBullet = false;

    if (nFullWidth < 1280)
    {
        ptOffset1.x_ = arg.state.X.abs*(1280.0/1024.0);
        ptOffset1.y_ = arg.state.Y.abs*(738.0/590.0);
    }

    Point pos;
    Point mouse(ptOffset1);

    mouse_down_ = false;

    if (last_down_node_)
    {
        Node *node = last_down_node_;
        ((Layer*)(node))->mouse_up(Point(mouse-node->position()), 0, 0);
    }

    
    mouse_position(mouse);

    return true;
}

void CGameScene::update(float dt)
{
    __super::update(dt);

    unsigned long time = m_Timer.get_milli_seconds();
    if (time > 60*1000 && !m_layRoles[GetMeChairID()]->IsShowWaringTime())
    {
        m_layRoles[GetMeChairID()]->ShowWaringTime(true);
        m_Timer.reset();
    }

    const OIS::MouseState& ms = Root::instance()->input_manager()->mouse()->getMouseState();

    int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
    int nFullHeight=GetSystemMetrics(SM_CYSCREEN);   

    Point ptOffset1(ms.X.abs, ms.Y.abs);


    if (nFullWidth < 1280)
    {
        ptOffset1.x_ = ms.X.abs*(1280.0/1024.0);
        ptOffset1.y_ = ms.Y.abs*(738.0/590.0);
    }

    Point pt(ptOffset1);
    Rect rcScreen(4,31,1272,703);

    if ((Root::instance()->input_manager()->keyboard()->isKeyDown(OIS::KC_UP)||
        Root::instance()->input_manager()->keyboard()->isKeyDown(OIS::KC_SPACE)||
        (Root::instance()->input_manager()->mouse()->getMouseState().buttonDown(OIS::MB_Left))&&rcScreen.pt_in_rect(pt)&&m_bFireBullet)
		
		&& ((m_layRoles[GetMeChairID()]->m_TaskDate.m_enState<CTaskDate::STATE_PREPARE2)
		    ||((m_layRoles[GetMeChairID()]->m_TaskDate.m_enType==CTaskDate::TYPE_COOK
			&&m_layRoles[GetMeChairID()]->m_TaskDate.m_enState==CTaskDate::STATE_RUNNING))))
    {
        WORD wMeChairID = GetMeChairID();
        if (wMeChairID != INVALID_CHAIR)
        {
            if (m_layRoles[wMeChairID]->GetFishGold() <= 0 && !m_layAccount->visible())
            {
                if (!m_layBuyBulletLayer->IsSendBuyBulletMessage())
                    m_layBuyBulletLayer->ShowWidnow(true);


            }
            else if (!m_layAccount->visible())
            {
                unsigned long time = m_Timer.get_milli_seconds();
                if (time < 400)
                    return ;

                m_Timer.reset();
                m_layRoles[GetMeChairID()]->ShowWaringTime(false);


                WORD wFishGold = m_layRoles[wMeChairID]->GetConnonType()+1;
                if (wFishGold >= m_layRoles[wMeChairID]->GetFishGold())
                {
                    wFishGold=0;
                }
                else
                {
                    wFishGold = m_layRoles[wMeChairID]->GetFishGold() - wFishGold;
                }

                m_layBulletObject->BulletFire(m_layRoles[wMeChairID]->GetCannonPosition(), m_layRoles[wMeChairID]->GetCannonRataion(), wMeChairID, m_layRoles[wMeChairID]->GetConnonType());
                m_layRoles[wMeChairID]->GunAnimation();
                m_layRoles[wMeChairID]->GunBaseAnimation();
                if (wFishGold <= 10)
                {
                    m_layRoles[wMeChairID]->ShowFishGoldEmpty(true);
                }
                else
                {
                    m_layRoles[wMeChairID]->ShowFishGoldEmpty(false);
                }
                m_layRoles[wMeChairID]->SetFishGold(wFishGold);
            }
        }
    }

}

void CGameScene::draw()
{
    __super::draw();

}



