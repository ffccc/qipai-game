#include "StdAfx.h"
#include "AndroidUserItem.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CTimerItemArray CAndroidUserItem::m_TimerItemBuffer;					//库存数组

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItem::CAndroidUserItem()
{
	//索引变量
	m_wRoundID=1;
	m_wAndroidIndex=INVALID_WORD;

	//接口变量
	m_pIServerUserManager=NULL;
	m_pIAndroidUserManager=NULL;
	m_pIAndroidUserItemSink=NULL;

	//状态变量
	m_bStartClient=false;
	m_cbGameStatus=GAME_STATUS_FREE;

	//用户状态
	m_CurrentUserStatus.cbUserStatus=US_NULL;
	m_CurrentUserStatus.wChairID=INVALID_CHAIR;
	m_CurrentUserStatus.wTableID=INVALID_TABLE;

	//绑定变量
	m_dwReposeTime=0L;
	m_pIServerUserItem=NULL;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	return;
}

//析构函数
CAndroidUserItem::~CAndroidUserItem()
{
	//删除时间
	KillGameTimer(0);

	//释放对象
	SafeRelease(m_pIAndroidUserItemSink);

	return;
}

//接口查询
VOID * CAndroidUserItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItem,Guid,dwQueryVer);
	return NULL;
}

//获取 I D
DWORD CAndroidUserItem::GetUserID()
{
	return m_AndroidParameter.dwUserID;
}

//桌子号码
WORD CAndroidUserItem::GetTableID()
{
	//效验状态
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return INVALID_TABLE;

	return m_pIServerUserItem->GetTableID();
}

//椅子号码
WORD CAndroidUserItem::GetChairID()
{
	//效验状态
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return INVALID_CHAIR;

	return m_pIServerUserItem->GetChairID();
}

//获取自己
IServerUserItem * CAndroidUserItem::GetMeUserItem()
{
	return m_pIServerUserItem;
}

//游戏用户
IServerUserItem * CAndroidUserItem::GetTableUserItem(WORD wChariID)
{
	return NULL;
}

//发送函数
bool CAndroidUserItem::SendSocketData(WORD wSubCmdID)
{
	//状态效验
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//发送数据
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_GAME,wSubCmdID,NULL,0);

	return true;
}

//存入金币
bool CAndroidUserItem::PerformSaveScore(SCORE lScore)
{
	//状态效验
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//效验金币
	ASSERT(m_pIServerUserItem->GetUserScore()>=lScore);
	if (m_pIServerUserItem->GetUserScore()<lScore) return false;

	//修改金币
	m_pIServerUserItem->ModifyUserInsure(-lScore,0L,0L);

	return true;
}

//提取金币
bool CAndroidUserItem::PerformTakeScore(SCORE lScore)
{
	//状态效验
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//修改金币
	m_pIServerUserItem->ModifyUserInsure(lScore,0L,0L);

	return true;
}

//发送函数
bool CAndroidUserItem::SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//状态效验
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//发送数据
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

	return true;
}

//删除时间
bool CAndroidUserItem::KillGameTimer(UINT nTimerID)
{
	//删除时间
	if (nTimerID!=0)
	{
		//寻找子项
		for (INT_PTR i=0;i<m_TimerItemActive.GetCount();i++)
		{
			//获取时间
			tagTimerItem * pTimerItem=m_TimerItemActive[i];

			//删除判断
			if (pTimerItem->nTimerID==nTimerID)
			{
				m_TimerItemActive.RemoveAt(i);
				m_TimerItemBuffer.Add(pTimerItem);

				return true;
			}
		}
	}
	else
	{
		m_TimerItemBuffer.Append(m_TimerItemActive);
		m_TimerItemActive.RemoveAll();
	}

	return false;
}

//设置时间
bool CAndroidUserItem::SetGameTimer(UINT nTimerID, UINT nElapse)
{
	//寻找子项
	for (INT_PTR i=0;i<m_TimerItemActive.GetCount();i++)
	{
		//获取时间
		tagTimerItem * pTimerItem=m_TimerItemActive[i];

		//设置判断
		if (pTimerItem->nTimerID==nTimerID)
		{
			pTimerItem->nTimeLeave=nElapse;
			return true;
		}
	}

	//变量定义
	tagTimerItem * pTimerItem=NULL;
	WORD wStorageCount=(WORD)m_TimerItemBuffer.GetCount();

	//库存子项
	if (wStorageCount>0)
	{
		//获取对象
		pTimerItem=m_TimerItemBuffer[wStorageCount-1];

		//加入数组
		m_TimerItemActive.Add(pTimerItem);
		m_TimerItemBuffer.RemoveAt(wStorageCount-1);
	}

	//创建子项
	if (pTimerItem==NULL)
	{
		try
		{
			//创建对象
			pTimerItem=new tagTimerItem;
			if (pTimerItem==NULL) return false;

			//加入数组
			m_TimerItemActive.Add(pTimerItem);
		}
		catch (...)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//设置变量
	pTimerItem->nTimerID=nTimerID;
	pTimerItem->nTimeLeave=nElapse;

	return true;
}

//发送准备
bool CAndroidUserItem::SendUserReady(VOID * pData, WORD wDataSize)
{
	//状态效验
	ASSERT((m_pIServerUserItem!=NULL)&&(m_pIServerUserItem->GetUserStatus()==US_SIT));
	if ((m_pIServerUserItem==NULL)||(m_pIServerUserItem->GetUserStatus()!=US_SIT)) return false;

	//发送准备
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_FRAME,SUB_GF_USER_READY,pData,wDataSize);

	return true;
}

//发送聊天
bool CAndroidUserItem::SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//效验用户
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//构造信息
	CMD_GF_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//构造数据
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//发送命令
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	WORD wSendSize=sizeof(UserChat)-sizeof(UserChat.szChatString)+UserChat.wChatLength*sizeof(TCHAR);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);

	return true;
}

//时间事件
bool CAndroidUserItem::OnTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//寻找子项
	for (INT_PTR i=0;i<m_TimerItemActive.GetCount();)
	{
		//变量定义
		tagTimerItem * pTimerItem=m_TimerItemActive[i];

		//时间处理
		if (pTimerItem->nTimeLeave<=1L)
		{
			//设置数组
			m_TimerItemActive.RemoveAt(i);
			m_TimerItemBuffer.Add(pTimerItem);

			//时间通知
			if (m_pIAndroidUserItemSink!=NULL)
			{
				m_pIAndroidUserItemSink->OnEventTimer(pTimerItem->nTimerID);
			}
		}
		else
		{
			//设置变量
			i++;
			pTimerItem->nTimeLeave--;
		}
	}


	return true;
}

//网络消息
bool CAndroidUserItem::OnSocketRead(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//登录失败
	if ((wMainCmdID==MDM_GR_LOGON)&&(wSubCmdID==SUB_GR_LOGON_FAILURE))
	{
		return OnSocketSubLogonFailure(pData,wDataSize);
	}

	//登录完成
	if ((wMainCmdID==MDM_GR_LOGON)&&(wSubCmdID==SUB_GR_LOGON_FINISH))
	{
		return OnSocketSubLogonFinish(pData,wDataSize);
	}

	//用户状态
	if ((wMainCmdID==MDM_GR_USER)&&(wSubCmdID==SUB_GR_USER_STATUS))
	{
		return OnSocketSubUserStatus(pData,wDataSize);
	}

	//游戏消息
	if ((wMainCmdID==MDM_GF_GAME)||(wMainCmdID==MDM_GF_FRAME))
	{
		return OnSocketGameMessage(wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	//系统消息
	if ((wMainCmdID==MDM_CM_SYSTEM)&&(wSubCmdID==SUB_CM_SYSTEM_MESSAGE))
	{
		return OnSocketSubSystemMessage(pData,wDataSize);
	}

	return true;
}

//游戏消息
bool CAndroidUserItem::OnSocketGameMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//游戏处理
	if ((wMainCmdID==MDM_GF_GAME)&&(m_pIAndroidUserItemSink!=NULL))
	{
		return m_pIAndroidUserItemSink->OnEventGameMessage(wSubCmdID,pData,wDataSize);
	}

	//框架处理
	if (wMainCmdID==MDM_GF_FRAME)
	{
		//默认处理
		switch (wSubCmdID)
		{
		case SUB_GF_GAME_STATUS:		//游戏状态
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GF_GameStatus));
				if (wDataSize!=sizeof(CMD_GF_GameStatus)) return false;

				//消息处理
				CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)pData;

				//设置变量
				m_cbGameStatus=pGameStatus->cbGameStatus;

				return true;
			}
		case SUB_GF_GAME_SCENE:			//游戏场景
			{
				//游戏处理
				if (m_pIAndroidUserItemSink!=NULL)
				{
					bool bLookonUser=(m_pIServerUserItem->GetUserStatus()==US_LOOKON);
					return m_pIAndroidUserItemSink->OnEventSceneMessage(m_cbGameStatus,bLookonUser,pData,wDataSize);
				}

				return true;
			}
		}
	}

	return true;
}

//登录失败
bool CAndroidUserItem::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	return true;
}

//登录完成
bool CAndroidUserItem::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//设置变量
	m_cbGameStatus=GAME_STATUS_FREE;
	m_pIServerUserItem=m_pIServerUserManager->SearchUserItem(m_AndroidParameter.dwUserID);

	//状态效验
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//用户状态
	m_CurrentUserStatus.wChairID=m_pIServerUserItem->GetChairID();
	m_CurrentUserStatus.wTableID=m_pIServerUserItem->GetTableID();
	m_CurrentUserStatus.cbUserStatus=m_pIServerUserItem->GetUserStatus();

	//规则变量
	CMD_GR_UserRule UserRule;
	ZeroMemory(&UserRule,sizeof(UserRule));

	//发送规则
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GR_USER,SUB_GR_USER_RULE,&UserRule,sizeof(UserRule));

	//重入判断
	if (m_pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		StartGameClient();
	}

	return true;
}

//用户状态
bool CAndroidUserItem::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//变量定义
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;

	//玩家判断
	if (pUserStatus->dwUserID!=m_AndroidParameter.dwUserID)
	{
		return true;
	}

	//用户状态
	tagUserStatus LastUserStatus;
	CopyMemory(&LastUserStatus,&m_CurrentUserStatus,sizeof(LastUserStatus));
	CopyMemory(&m_CurrentUserStatus,&pUserStatus->UserStatus,sizeof(m_CurrentUserStatus));

	//开始切换
	if ((LastUserStatus.cbUserStatus!=US_READY)&&(m_CurrentUserStatus.cbUserStatus==US_READY))
	{
	}

	//关闭判断
	if ((m_bStartClient==true)&&(m_CurrentUserStatus.wTableID==INVALID_TABLE))
	{
		//关闭游戏
		CloseGameClient();

		return true;
	}

	//启动判断
	if ((m_bStartClient==false)&&(m_CurrentUserStatus.wTableID!=INVALID_TABLE))
	{
		//启动游戏
		StartGameClient();

		return true;
	}

	return true;
}

//系统消息
bool CAndroidUserItem::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(WCHAR)))) return false;

	//关闭处理
	if ((pSystemMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭处理
		DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
		m_pIAndroidUserManager->DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//启动游戏
VOID CAndroidUserItem::StartGameClient()
{
	//状态判断
	ASSERT((m_bStartClient==false)&&(m_pIServerUserItem!=NULL));
	ASSERT((m_pIServerUserItem->GetTableID()!=INVALID_TABLE)&&(m_pIServerUserItem->GetChairID()!=INVALID_CHAIR));

	//设置变量
	m_bStartClient=true;

	//变量定义
	CMD_GF_GameOption GameOption;
	ZeroMemory(&GameOption,sizeof(GameOption));

	//构造数据
	GameOption.cbAllowLookon=FALSE;
	GameOption.dwFrameVersion=INVALID_DWORD;
	GameOption.dwClientVersion=INVALID_DWORD;

	//发送数据
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_FRAME,SUB_GF_GAME_OPTION,&GameOption,sizeof(GameOption));

	return;
}

//关闭游戏
VOID CAndroidUserItem::CloseGameClient()
{
	//状态判断
	ASSERT((m_bStartClient==true)&&(m_pIServerUserItem!=NULL));
	ASSERT((m_pIServerUserItem->GetTableID()==INVALID_TABLE)&&(m_pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//删除时间
	KillGameTimer(0);

	//设置变量
	m_bStartClient=false;
	m_cbGameStatus=GAME_STATUS_FREE;

	//游戏复位
	if (m_pIAndroidUserItemSink!=NULL)
	{
		m_pIAndroidUserItemSink->RepositionSink();
	}

	return;
}

//复位数据
VOID CAndroidUserItem::RepositUserItem()
{
	//状态变量
	m_bStartClient=false;
	m_cbGameStatus=GAME_STATUS_FREE;

	//用户状态
	m_CurrentUserStatus.cbUserStatus=US_NULL;
	m_CurrentUserStatus.wChairID=INVALID_CHAIR;
	m_CurrentUserStatus.wTableID=INVALID_TABLE;

	//绑定变量
	m_dwReposeTime=0L;
	m_pIServerUserItem=NULL;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	//删除时间
	m_TimerItemBuffer.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	//索引变量
	m_wRoundID=__max(1,m_wRoundID+1);

	//复位游戏
	if (m_pIAndroidUserItemSink!=NULL)
	{
		m_pIAndroidUserItemSink->RepositionSink();
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
