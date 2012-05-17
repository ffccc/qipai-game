#include "StdAfx.h"
#include "GlobalUnits.h"
#include "GameFrameService.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameService::CGameFrameService()
{
	//界面接口
	m_pIGameFrameWnd=NULL;
	m_pIGameFrameControl=NULL;

	//设置变量
	m_pIClientKernel=NULL;
	m_pGameFrameView=NULL;
	m_pGameFrameEngine=NULL;

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_SERVICE,QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//析构函数
CGameFrameService::~CGameFrameService()
{
}

//接口查询
VOID * CGameFrameService::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameFrameService,Guid,dwQueryVer);
	QUERYINTERFACE(IClientKernelSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameService,Guid,dwQueryVer);
	return NULL;
}

//重置游戏
VOID CGameFrameService::ResetGameClient()
{
	//重置控件
	if (m_pIGameFrameControl!=NULL)
	{
		m_pIGameFrameControl->ResetControl();
	}

	//重置界面
	ASSERT(m_pGameFrameView!=NULL);
	if (m_pGameFrameView!=NULL) m_pGameFrameView->SwitchToReadyStatus();

	//重置框架
	ASSERT(m_pGameFrameEngine!=NULL);
	if (m_pGameFrameEngine!=NULL) m_pGameFrameEngine->ResetGameFrameEngine();

	return;
}

//关闭游戏
VOID CGameFrameService::CloseGameClient()
{
	//发送关闭
	AfxGetMainWnd()->PostMessage(WM_CLOSE);

	return;
}

//配置完成
bool CGameFrameService::OnSchemeFinish()
{
	//连接广告
	if (m_pIGameFrameControl!=NULL)
	{
		tagGameAttribute * pGameAttribute=m_pIClientKernel->GetGameAttribute();
		tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();
		m_pIGameFrameControl->SetGameBillInfo(pServerAttribute->wKindID,pServerAttribute->wServerID);
	}

	return true;
}

//列表配置
bool CGameFrameService::SchemeColumnItem(tagColumnItem ColumnItem[], BYTE cbColumnCount)
{
	//设置列表
	if (m_pIGameFrameControl!=NULL)
	{
		m_pIGameFrameControl->SetColumnDescribe(ColumnItem,cbColumnCount);
	}

	return true;
}

//道具配置
bool CGameFrameService::SchemePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount)
{
	//设置道具
	if (m_pIGameFrameControl!=NULL)
	{
		m_pIGameFrameControl->SetPropertyDescribe(PropertyInfo,cbPropertyCount);
	}

	return true;
}

//游戏消息
bool CGameFrameService::OnEventSocket(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//游戏消息
	if (wMainCmdID==MDM_GF_GAME)
	{
		//效验状态
		ASSERT(m_pGameFrameEngine!=NULL);
		if (m_pGameFrameEngine==NULL) return false;

		return m_pGameFrameEngine->OnEventGameMessage(wSubCmdID,pData,wDataSize);
	}

	return false;
}

//进程消息
bool CGameFrameService::OnEventProcess(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return false;
}

//旁观消息
bool CGameFrameService::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(m_pGameFrameEngine!=NULL);
	if (m_pGameFrameEngine==NULL) return false;

	return m_pGameFrameEngine->OnEventLookonMode(pData,wDataSize);
}

//场景消息
bool CGameFrameService::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(m_pGameFrameEngine!=NULL);
	if (m_pGameFrameEngine==NULL) return false;

	return m_pGameFrameEngine->OnEventSceneMessage(cbGameStatus,bLookonUser,pData,wDataSize);
}

//时钟删除
bool CGameFrameService::OnEventGameClockKill(WORD wChairID)
{
	//效验状态
	ASSERT(m_pGameFrameEngine!=NULL);
	if (m_pGameFrameEngine==NULL) return false;

	//界面时间
	if (wChairID!=INVALID_CHAIR)
	{
		ASSERT(m_pGameFrameView!=NULL);
		m_pGameFrameView->SetUserClock(m_pIClientKernel->SwitchViewChairID(wChairID),0L);
	}
	else
	{
		ASSERT(m_pGameFrameView!=NULL);
		m_pGameFrameView->SetUserClock(INVALID_CHAIR,0L);
	}

	//时间处理
	return m_pGameFrameEngine->OnEventGameClockKill(wChairID);
}

//时钟信息
bool CGameFrameService::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	//效验状态
	ASSERT(m_pGameFrameEngine!=NULL);
	if (m_pGameFrameEngine==NULL) return false;

	//界面时间
	WORD wViewChairID=m_pIClientKernel->SwitchViewChairID(wChairID);
	if (m_pGameFrameView!=NULL) m_pGameFrameView->SetUserClock(wViewChairID,nElapse);

	//时间处理
	return m_pGameFrameEngine->OnEventGameClockInfo(wChairID,nElapse,wClockID);
}

//用户进入
VOID CGameFrameService::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//视图用户
	if (bLookonUser==false)
	{
		//获取属性
		WORD wChairID=pIClientUserItem->GetChairID();
		WORD wViewChairID=m_pIClientKernel->SwitchViewChairID(wChairID);

		//设置用户
		m_pGameFrameView->SetClientUserItem(wViewChairID,pIClientUserItem);
	}

	return;
}

//用户离开
VOID CGameFrameService::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//视图用户
	if (bLookonUser==false)
	{
		//获取属性
		WORD wChairID=pIClientUserItem->GetChairID();
		WORD wViewChairID=m_pIClientKernel->SwitchViewChairID(wChairID);

		//设置用户
		m_pGameFrameView->SetClientUserItem(wViewChairID,NULL);
	}

	return;
}

//用户积分
VOID CGameFrameService::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//更新界面
	CRect rcClient;
	m_pGameFrameView->GetClientRect(&rcClient);
	m_pGameFrameView->InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//用户状态
VOID CGameFrameService::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//更新界面
	CRect rcClient;
	m_pGameFrameView->GetClientRect(&rcClient);
	m_pGameFrameView->InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//用户头像
VOID CGameFrameService::OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//更新界面
	CRect rcClient;
	m_pGameFrameView->GetClientRect(&rcClient);
	m_pGameFrameView->InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//创建引擎
bool CGameFrameService::CreateGameFrameService()
{
	//效验状态
	ASSERT((CGameFrameView::GetInstance()!=NULL)&&(CGameFrameEngine::GetInstance()!=NULL));
	if ((CGameFrameView::GetInstance()==NULL)||(CGameFrameEngine::GetInstance()==NULL)) return false;

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//设置变量
	m_pGameFrameView=CGameFrameView::GetInstance();
	m_pGameFrameEngine=CGameFrameEngine::GetInstance();

	//查询接口
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIGameFrameWnd=(IGameFrameWnd *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_WND,IID_IGameFrameWnd,VER_IGameFrameWnd);
	m_pIGameFrameControl=(IGameFrameControl *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_CONTROL,IID_IGameFrameControl,VER_IGameFrameControl);

	//注册接口
	m_pIClientKernel->SetUserEventSink(QUERY_ME_INTERFACE(IUserEventSink));
	m_pIClientKernel->SetClientKernelSink(QUERY_ME_INTERFACE(IClientKernelSink));

	//创建游戏
	if ((m_pGameFrameEngine==NULL)||(m_pGameFrameEngine->CreateGameFrameEngine()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
