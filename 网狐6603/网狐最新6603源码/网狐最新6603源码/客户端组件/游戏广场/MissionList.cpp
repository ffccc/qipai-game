#include "StdAfx.h"
#include "MissionList.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//激活任务
#define IDI_AVTIVE_MISSION				100								//激活任务
#define TIME_AVTIVE_MISSION				600000L							//激活任务

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CMissionList * CMissionList::m_pMissionList=NULL;						//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionList, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMissionList::CMissionList()
{
	//设置变量
	m_bUpdateOnLine=false;

	//设置对象
	ASSERT(m_pMissionList==NULL);
	if (m_pMissionList==NULL) m_pMissionList=this;

	return;
}

//析构函数
CMissionList::~CMissionList()
{
	//释放对象
	ASSERT(m_pMissionList==this);
	if (m_pMissionList==this) m_pMissionList=NULL;

	return;
}

//连接事件
bool CMissionList::OnEventMissionLink(INT nErrorCode)
{
	//错误判断
	if (nErrorCode!=0L)
	{
		//状态效验
		if ((m_KindIDMission.GetCount()==0)&&(m_bUpdateOnLine==false))
		{
			ASSERT(FALSE);
			return false;
		}

		//创建窗口
		if (m_hWnd==NULL)
		{
			CRect rcCreate(0,0,0,0);
			Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
		}

		//设置时间
		SetTimer(IDI_AVTIVE_MISSION,TIME_AVTIVE_MISSION,NULL);

		return true;
	}

	//更新人数
	if (m_bUpdateOnLine==true)
	{
		//设置变量
		m_bUpdateOnLine=false;

		//变量定义
		CMD_GP_GetOnline GetOnline;
		ZeroMemory(&GetOnline,sizeof(GetOnline));

		//变量定义
		POSITION Position=NULL;
		CMissionManager * pMissionManager=GetMissionManager();
		CServerListData * pServerListData=CServerListData::GetInstance();

		do
		{
			//获取房间
			CGameServerItem * pGameServerItem=pServerListData->EmunGameServerItem(Position);

			//溢出判断
			if (GetOnline.wServerCount>=CountArray(GetOnline.wOnLineServerID))
			{
				ASSERT(FALSE);
				break;
			}

			//设置房间
			if (pGameServerItem!=NULL)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				GetOnline.wOnLineServerID[GetOnline.wServerCount++]=pGameServer->wServerID;
			}

		} while (Position!=NULL);

		//发送数据
		WORD wHeadSize=sizeof(GetOnline)-sizeof(GetOnline.wOnLineServerID);
		WORD wSendSize=wHeadSize+sizeof(GetOnline.wOnLineServerID[0])*GetOnline.wServerCount;
		pMissionManager->SendData(MDM_GP_SERVER_LIST,SUB_GP_GET_ONLINE,&GetOnline,wSendSize);
	}

	//列表任务
	if (m_KindIDMission.GetCount()>0L)
	{
		//变量定义
		WORD wKindIDItem[MAX_KIND];
		WORD wKindIDCount=__min(CountArray(wKindIDItem),m_KindIDMission.GetCount());

		//变量定义
		WORD wKindIDKey=0;
		POSITION KindIDPos=m_KindIDMission.GetStartPosition();

		//构造数据
		for (WORD i=0;i<wKindIDCount;i++) 
		{
			m_KindIDMission.GetNextAssoc(KindIDPos,wKindIDKey,wKindIDItem[i]);
			m_KindIDPending[wKindIDItem[i]]=wKindIDItem[i];
		}

		//删除任务
		m_KindIDMission.RemoveAll();

		//发送数据
		CMissionManager * pMissionManager=GetMissionManager();
		pMissionManager->SendData(MDM_GP_SERVER_LIST,SUB_GP_GET_SERVER,wKindIDItem,wKindIDCount*sizeof(WORD));
	}

	return true;
}

//关闭事件
bool CMissionList::OnEventMissionShut(BYTE cbShutReason)
{
	//列表任务
	if (m_KindIDPending.GetCount()>0)
	{
		//变量定义
		WORD wKindIDKey=0,wKindID=0;
		POSITION Position=m_KindIDPending.GetStartPosition();

		//储备任务
		while (Position!=NULL)
		{
			//枚举任务
			m_KindIDPending.GetNextAssoc(Position,wKindIDKey,wKindID);

			//加入任务
			m_KindIDMission[wKindIDKey]=wKindID;
		}

		//删除任务
		m_KindIDPending.RemoveAll();
	}

	//继续任务
	if ((m_KindIDMission.GetCount()>0)||(m_bUpdateOnLine==true))
	{
		//创建窗口
		if (m_hWnd==NULL)
		{
			CRect rcCreate(0,0,0,0);
			Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
		}

		//设置时间
		SetTimer(IDI_AVTIVE_MISSION,TIME_AVTIVE_MISSION,NULL);
	}

	return true;
}

//读取事件
bool CMissionList::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_SERVER_LIST)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_LIST_TYPE:		//种类信息
			{
				//效验参数
				ASSERT(wDataSize%sizeof(tagGameType)==0);
				if (wDataSize%sizeof(tagGameType)!=0) return false;

				//变量定义
				WORD wItemCount=wDataSize/sizeof(tagGameType);
				tagGameType * pGameType=(tagGameType *)(pData);

				//获取对象
				ASSERT(CServerListData::GetInstance()!=NULL);
				CServerListData * pServerListData=CServerListData::GetInstance();

				//更新数据
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGameType(pGameType++);
				}

				return true;
			}
		case SUB_GP_LIST_KIND:		//类型信息
			{
				//效验参数
				ASSERT(wDataSize%sizeof(tagGameKind)==0);
				if (wDataSize%sizeof(tagGameKind)!=0) return false;

				//变量定义
				WORD wItemCount=wDataSize/sizeof(tagGameKind);
				tagGameKind * pGameKind=(tagGameKind *)(pData);

				//获取对象
				ASSERT(CServerListData::GetInstance()!=NULL);
				CServerListData * pServerListData=CServerListData::GetInstance();

				//更新数据
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGameKind(pGameKind++);
				}

				return true;
			}
		case SUB_GP_LIST_NODE:		//节点信息
			{
				//效验参数
				ASSERT(wDataSize%sizeof(tagGameNode)==0);
				if (wDataSize%sizeof(tagGameNode)!=0) return false;

				//变量定义
				WORD wItemCount=wDataSize/sizeof(tagGameNode);
				tagGameNode * pGameNode=(tagGameNode *)(pData);

				//获取对象
				ASSERT(CServerListData::GetInstance()!=NULL);
				CServerListData * pServerListData=CServerListData::GetInstance();

				//更新数据
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGameNode(pGameNode++);
				}

				return true;
			}
		case SUB_GP_LIST_PAGE:		//页面信息
			{
				//效验参数
				ASSERT(wDataSize%sizeof(tagGamePage)==0);
				if (wDataSize%sizeof(tagGamePage)!=0) return false;

				//变量定义
				WORD wItemCount=wDataSize/sizeof(tagGamePage);
				tagGamePage * pGamePage=(tagGamePage *)(pData);

				//获取对象
				CServerListData * pServerListData=CServerListData::GetInstance();

				//更新数据
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGamePage(pGamePage++);
				}

				return true;
			}
		case SUB_GP_LIST_SERVER:	//房间信息
			{
				//效验参数
				ASSERT(wDataSize%sizeof(tagGameServer)==0);
				if (wDataSize%sizeof(tagGameServer)!=0) return false;

				//变量定义
				WORD wItemCount=wDataSize/sizeof(tagGameServer);
				tagGameServer * pGameServer=(tagGameServer *)(pData);

				//获取对象
				CServerListData * pServerListData=CServerListData::GetInstance();

				//更新数据
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGameServer(pGameServer++);
				}

				return true;
			}
		case SUB_GP_LIST_FINISH:	//列表信息
			{
				//列表完成
				ASSERT(CServerListData::GetInstance()!=NULL);
				if (CServerListData::GetInstance()!=NULL) CServerListData::GetInstance()->OnEventListFinish();

				return true;
			}
		case SUB_GP_SERVER_FINISH:	//房间完成
			{
				//效验数据
				ASSERT(wDataSize%sizeof(WORD)==0);
				if (wDataSize%sizeof(WORD)!=0) return false;

				//获取对象
				ASSERT(CServerListData::GetInstance()!=NULL);
				CServerListData * pServerListData=CServerListData::GetInstance();

				//任务处理
				for (UINT i=0;i<(wDataSize/sizeof(WORD));i++)
				{
					m_KindIDPending.RemoveKey(((WORD *)pData)[i]);
					pServerListData->OnEventKindFinish(((WORD *)pData)[i]);
				}

				//终止任务
				if ((m_KindIDPending.GetCount()==0L)&&(m_bUpdateOnLine==false))
				{
					CMissionManager * pMissionManager=GetMissionManager();
					if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);
				}

				return true;
			}
		case SUB_GR_KINE_ONLINE:	//类型在线
			{
				//变量定义
				CMD_GP_KindOnline * pKindOnline=(CMD_GP_KindOnline *)pData;
				WORD wHeadSize=(sizeof(CMD_GP_KindOnline)-sizeof(pKindOnline->OnLineInfoKind));

				//效验数据
				ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pKindOnline->wKindCount*sizeof(tagOnLineInfoKind))));
				if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pKindOnline->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

				//获取对象
				CServerListData * pServerListData=CServerListData::GetInstance();

				//设置人数
				for (WORD i=0;i<pKindOnline->wKindCount;i++)
				{
					tagOnLineInfoKind * pOnLineInfoKind=&pKindOnline->OnLineInfoKind[i];
					pServerListData->SetKindOnLineCount(pOnLineInfoKind->wKindID,pOnLineInfoKind->dwOnLineCount);
				}

				return true;
			}
		case SUB_GR_SERVER_ONLINE:	//房间在线
			{
				//变量定义
				CMD_GP_ServerOnline * pServerOnline=(CMD_GP_ServerOnline *)pData;
				WORD wHeadSize=(sizeof(CMD_GP_ServerOnline)-sizeof(pServerOnline->OnLineInfoServer));

				//效验数据
				ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pServerOnline->wServerCount*sizeof(tagOnLineInfoServer))));
				if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pServerOnline->wServerCount*sizeof(tagOnLineInfoServer)))) return false;

				//获取对象
				CServerListData * pServerListData=CServerListData::GetInstance();

				//设置人数
				for (WORD i=0;i<pServerOnline->wServerCount;i++)
				{
					tagOnLineInfoServer * pOnLineInfoServer=&pServerOnline->OnLineInfoServer[i];
					pServerListData->SetServerOnLineCount(pOnLineInfoServer->wServerID,pOnLineInfoServer->dwOnLineCount);
				}

				//终止任务
				if ((m_KindIDPending.GetCount()==0L)&&(m_bUpdateOnLine==false))
				{
					CMissionManager * pMissionManager=GetMissionManager();
					if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);
				}

				return true;
			}
		}
	}

	return true;
}

//更新人数
VOID CMissionList::UpdateOnLineInfo()
{
	//效验状态
	ASSERT(m_bUpdateOnLine==false);
	if (m_bUpdateOnLine==true) return;

	//设置变量
	m_bUpdateOnLine=true;

	//关闭窗口
	if (m_hWnd!=NULL) DestroyWindow();

	//激活连接
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//设置地址
	LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
	if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

	//激活任务
	pMissionManager->AvtiveMissionItem(this,false);

	return;
}

//更新类型
VOID CMissionList::UpdateServerInfo(WORD wKindID)
{
	//关闭窗口
	if (m_hWnd!=NULL) DestroyWindow();

	//设置变量
	m_KindIDMission[wKindID]=wKindID;

	//激活连接
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//设置地址
	LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
	if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

	//激活任务
	pMissionManager->AvtiveMissionItem(this,false);
	
	return;
}

//时间消息
VOID CMissionList::OnTimer(UINT nIDEvent)
{
	//激活任务
	if (nIDEvent==IDI_AVTIVE_MISSION)
	{
		//关闭窗口
		DestroyWindow();

		//激活连接
		ASSERT(GetMissionManager()!=NULL);
		CMissionManager * pMissionManager=GetMissionManager();

		//设置地址
		LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
		if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

		//激活任务
		pMissionManager->AvtiveMissionItem(this,false);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
