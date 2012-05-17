#include "StdAfx.h"
#include "GlobalUnits.h"
#include "GameFrameEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//标识定义
#define IDC_GAME_FRAME_ENGINE		100									//窗口标识

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CGameFrameEngine *	CGameFrameEngine::m_pGameFrameEngine=NULL;			//引擎接口

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameEngine, CWnd)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameEngine::CGameFrameEngine()
{
	//界面接口
	m_pIClientKernel=NULL;
	m_pIStringMessage=NULL;

	//设置接口
	ASSERT(m_pGameFrameEngine==NULL);
	if (m_pGameFrameEngine==NULL) m_pGameFrameEngine=this;

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_ENGINE,QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//析构函数
CGameFrameEngine::~CGameFrameEngine()
{
	//设置接口
	ASSERT(m_pGameFrameEngine==this);
	if (m_pGameFrameEngine==this) m_pGameFrameEngine=NULL;

	return;
}

//接口查询
VOID * CGameFrameEngine::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameEngine,Guid,dwQueryVer);
	return NULL;
}

//单机模式
bool CGameFrameEngine::IsSingleMode()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return true;

	//获取模式
	return m_pIClientKernel->IsSingleMode();
}

//旁观用户
bool CGameFrameEngine::IsLookonMode()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return true;

	//获取设置
	return m_pIClientKernel->IsLookonMode();
}

//允许旁观
bool CGameFrameEngine::IsAllowLookon()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//获取设置
	return m_pIClientKernel->IsAllowLookon();
}

//获取状态
BYTE CGameFrameEngine::GetGameStatus()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return GAME_STATUS_FREE;

	//获取状态
	return m_pIClientKernel->GetGameStatus();
}

//设置状态
VOID CGameFrameEngine::SetGameStatus(BYTE cbGameStatus)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return;

	//设置状态
	m_pIClientKernel->SetGameStatus(cbGameStatus);

	return;
}

//获取自己
WORD CGameFrameEngine::GetMeChairID()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return NULL;

	//获取自己
	return m_pIClientKernel->GetMeChairID();
}

//获取自己
IClientUserItem * CGameFrameEngine::GetMeUserItem()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return NULL;

	//获取自己
	return m_pIClientKernel->GetMeUserItem();
}

//游戏用户
IClientUserItem * CGameFrameEngine::GetTableUserItem(WORD wChariID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return NULL;

	//获取自己
	return m_pIClientKernel->GetTableUserItem(wChariID);
}

//游戏用户
IClientUserItem * CGameFrameEngine::SearchTableUserItem(DWORD dwUserID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return NULL;

	//搜索用户
	return m_pIClientKernel->SearchUserByUserID(dwUserID);
}

//发送函数
bool CGameFrameEngine::SendSocketData(WORD wSubCmdID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendSocketData(MDM_GF_GAME,wSubCmdID);
}

//发送函数
bool CGameFrameEngine::SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendSocketData(MDM_GF_GAME,wSubCmdID,pData,wDataSize);
}

//发送准备
bool CGameFrameEngine::SendUserReady(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendUserReady(pData,wDataSize);
}

//发送表情
bool CGameFrameEngine::SendUserExpression(DWORD dwTargetUserID, WORD wItemIndex)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendUserExpression(dwTargetUserID,wItemIndex);
}

//发送聊天
bool CGameFrameEngine::SendUserChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//发送数据
	return m_pIClientKernel->SendUserChatMessage(dwTargetUserID,pszChatMessage,crFontColor);
}

//时钟标识
UINT CGameFrameEngine::GetClockID()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//删除时钟
	return m_pIClientKernel->GetClockID();
}

//时钟位置
WORD CGameFrameEngine::GetClockChairID()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return false;

	//删除时钟
	return m_pIClientKernel->GetClockChairID();
}

//删除时钟
VOID CGameFrameEngine::KillGameClock(WORD wClockID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return;

	//删除时钟
	m_pIClientKernel->KillGameClock(wClockID);

	return;
}

//设置时钟
VOID CGameFrameEngine::SetGameClock(WORD wChairID, WORD wClockID, UINT nElapse)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return;

	//设置时钟
	m_pIClientKernel->SetGameClock(wChairID,wClockID,nElapse);

	return;
}

//激活框架
VOID CGameFrameEngine::ActiveGameFrame()
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return;

	//激活框架
	m_pIClientKernel->ActiveGameFrame();

	return;
}

//切换椅子
WORD CGameFrameEngine::SwitchViewChairID(WORD wChairID)
{
	//效验参数
	ASSERT(m_pIClientKernel!=NULL);
	if (m_pIClientKernel==NULL) return INVALID_CHAIR;

	//转换椅子
	return m_pIClientKernel->SwitchViewChairID(wChairID);
}

//播放声音
bool CGameFrameEngine::PlayGameSound(LPCTSTR pszSoundName)
{
	//播放声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayGameSound(pszSoundName);

	return true;
}

//播放声音
bool CGameFrameEngine::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//播放声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayGameSound(hInstance,pszSoundName);

	return true;
}

//重置引擎
bool CGameFrameEngine::ResetGameFrameEngine()
{
	//重置通知
	if (OnResetGameEngine()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//创建引擎
bool CGameFrameEngine::CreateGameFrameEngine()
{
	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//查询接口
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);

	//用户接口
	IUserEventSink * pIUserEventSink=QUERY_ME_INTERFACE(IUserEventSink);
	if (pIUserEventSink!=NULL) m_pIClientKernel->SetUserEventSink(pIUserEventSink);

	//创建窗口
	if (Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),AfxGetMainWnd(),IDC_GAME_FRAME_ENGINE)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建通知
	if (OnInitGameEngine()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
