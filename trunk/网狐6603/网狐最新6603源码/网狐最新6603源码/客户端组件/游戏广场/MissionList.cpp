#include "StdAfx.h"
#include "MissionList.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define IDI_AVTIVE_MISSION				100								//��������
#define TIME_AVTIVE_MISSION				600000L							//��������

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CMissionList * CMissionList::m_pMissionList=NULL;						//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMissionList, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMissionList::CMissionList()
{
	//���ñ���
	m_bUpdateOnLine=false;

	//���ö���
	ASSERT(m_pMissionList==NULL);
	if (m_pMissionList==NULL) m_pMissionList=this;

	return;
}

//��������
CMissionList::~CMissionList()
{
	//�ͷŶ���
	ASSERT(m_pMissionList==this);
	if (m_pMissionList==this) m_pMissionList=NULL;

	return;
}

//�����¼�
bool CMissionList::OnEventMissionLink(INT nErrorCode)
{
	//�����ж�
	if (nErrorCode!=0L)
	{
		//״̬Ч��
		if ((m_KindIDMission.GetCount()==0)&&(m_bUpdateOnLine==false))
		{
			ASSERT(FALSE);
			return false;
		}

		//��������
		if (m_hWnd==NULL)
		{
			CRect rcCreate(0,0,0,0);
			Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
		}

		//����ʱ��
		SetTimer(IDI_AVTIVE_MISSION,TIME_AVTIVE_MISSION,NULL);

		return true;
	}

	//��������
	if (m_bUpdateOnLine==true)
	{
		//���ñ���
		m_bUpdateOnLine=false;

		//��������
		CMD_GP_GetOnline GetOnline;
		ZeroMemory(&GetOnline,sizeof(GetOnline));

		//��������
		POSITION Position=NULL;
		CMissionManager * pMissionManager=GetMissionManager();
		CServerListData * pServerListData=CServerListData::GetInstance();

		do
		{
			//��ȡ����
			CGameServerItem * pGameServerItem=pServerListData->EmunGameServerItem(Position);

			//����ж�
			if (GetOnline.wServerCount>=CountArray(GetOnline.wOnLineServerID))
			{
				ASSERT(FALSE);
				break;
			}

			//���÷���
			if (pGameServerItem!=NULL)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				GetOnline.wOnLineServerID[GetOnline.wServerCount++]=pGameServer->wServerID;
			}

		} while (Position!=NULL);

		//��������
		WORD wHeadSize=sizeof(GetOnline)-sizeof(GetOnline.wOnLineServerID);
		WORD wSendSize=wHeadSize+sizeof(GetOnline.wOnLineServerID[0])*GetOnline.wServerCount;
		pMissionManager->SendData(MDM_GP_SERVER_LIST,SUB_GP_GET_ONLINE,&GetOnline,wSendSize);
	}

	//�б�����
	if (m_KindIDMission.GetCount()>0L)
	{
		//��������
		WORD wKindIDItem[MAX_KIND];
		WORD wKindIDCount=__min(CountArray(wKindIDItem),m_KindIDMission.GetCount());

		//��������
		WORD wKindIDKey=0;
		POSITION KindIDPos=m_KindIDMission.GetStartPosition();

		//��������
		for (WORD i=0;i<wKindIDCount;i++) 
		{
			m_KindIDMission.GetNextAssoc(KindIDPos,wKindIDKey,wKindIDItem[i]);
			m_KindIDPending[wKindIDItem[i]]=wKindIDItem[i];
		}

		//ɾ������
		m_KindIDMission.RemoveAll();

		//��������
		CMissionManager * pMissionManager=GetMissionManager();
		pMissionManager->SendData(MDM_GP_SERVER_LIST,SUB_GP_GET_SERVER,wKindIDItem,wKindIDCount*sizeof(WORD));
	}

	return true;
}

//�ر��¼�
bool CMissionList::OnEventMissionShut(BYTE cbShutReason)
{
	//�б�����
	if (m_KindIDPending.GetCount()>0)
	{
		//��������
		WORD wKindIDKey=0,wKindID=0;
		POSITION Position=m_KindIDPending.GetStartPosition();

		//��������
		while (Position!=NULL)
		{
			//ö������
			m_KindIDPending.GetNextAssoc(Position,wKindIDKey,wKindID);

			//��������
			m_KindIDMission[wKindIDKey]=wKindID;
		}

		//ɾ������
		m_KindIDPending.RemoveAll();
	}

	//��������
	if ((m_KindIDMission.GetCount()>0)||(m_bUpdateOnLine==true))
	{
		//��������
		if (m_hWnd==NULL)
		{
			CRect rcCreate(0,0,0,0);
			Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
		}

		//����ʱ��
		SetTimer(IDI_AVTIVE_MISSION,TIME_AVTIVE_MISSION,NULL);
	}

	return true;
}

//��ȡ�¼�
bool CMissionList::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	if (Command.wMainCmdID==MDM_GP_SERVER_LIST)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_LIST_TYPE:		//������Ϣ
			{
				//Ч�����
				ASSERT(wDataSize%sizeof(tagGameType)==0);
				if (wDataSize%sizeof(tagGameType)!=0) return false;

				//��������
				WORD wItemCount=wDataSize/sizeof(tagGameType);
				tagGameType * pGameType=(tagGameType *)(pData);

				//��ȡ����
				ASSERT(CServerListData::GetInstance()!=NULL);
				CServerListData * pServerListData=CServerListData::GetInstance();

				//��������
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGameType(pGameType++);
				}

				return true;
			}
		case SUB_GP_LIST_KIND:		//������Ϣ
			{
				//Ч�����
				ASSERT(wDataSize%sizeof(tagGameKind)==0);
				if (wDataSize%sizeof(tagGameKind)!=0) return false;

				//��������
				WORD wItemCount=wDataSize/sizeof(tagGameKind);
				tagGameKind * pGameKind=(tagGameKind *)(pData);

				//��ȡ����
				ASSERT(CServerListData::GetInstance()!=NULL);
				CServerListData * pServerListData=CServerListData::GetInstance();

				//��������
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGameKind(pGameKind++);
				}

				return true;
			}
		case SUB_GP_LIST_NODE:		//�ڵ���Ϣ
			{
				//Ч�����
				ASSERT(wDataSize%sizeof(tagGameNode)==0);
				if (wDataSize%sizeof(tagGameNode)!=0) return false;

				//��������
				WORD wItemCount=wDataSize/sizeof(tagGameNode);
				tagGameNode * pGameNode=(tagGameNode *)(pData);

				//��ȡ����
				ASSERT(CServerListData::GetInstance()!=NULL);
				CServerListData * pServerListData=CServerListData::GetInstance();

				//��������
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGameNode(pGameNode++);
				}

				return true;
			}
		case SUB_GP_LIST_PAGE:		//ҳ����Ϣ
			{
				//Ч�����
				ASSERT(wDataSize%sizeof(tagGamePage)==0);
				if (wDataSize%sizeof(tagGamePage)!=0) return false;

				//��������
				WORD wItemCount=wDataSize/sizeof(tagGamePage);
				tagGamePage * pGamePage=(tagGamePage *)(pData);

				//��ȡ����
				CServerListData * pServerListData=CServerListData::GetInstance();

				//��������
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGamePage(pGamePage++);
				}

				return true;
			}
		case SUB_GP_LIST_SERVER:	//������Ϣ
			{
				//Ч�����
				ASSERT(wDataSize%sizeof(tagGameServer)==0);
				if (wDataSize%sizeof(tagGameServer)!=0) return false;

				//��������
				WORD wItemCount=wDataSize/sizeof(tagGameServer);
				tagGameServer * pGameServer=(tagGameServer *)(pData);

				//��ȡ����
				CServerListData * pServerListData=CServerListData::GetInstance();

				//��������
				for (WORD i=0;i<wItemCount;i++)
				{
					pServerListData->InsertGameServer(pGameServer++);
				}

				return true;
			}
		case SUB_GP_LIST_FINISH:	//�б���Ϣ
			{
				//�б����
				ASSERT(CServerListData::GetInstance()!=NULL);
				if (CServerListData::GetInstance()!=NULL) CServerListData::GetInstance()->OnEventListFinish();

				return true;
			}
		case SUB_GP_SERVER_FINISH:	//�������
			{
				//Ч������
				ASSERT(wDataSize%sizeof(WORD)==0);
				if (wDataSize%sizeof(WORD)!=0) return false;

				//��ȡ����
				ASSERT(CServerListData::GetInstance()!=NULL);
				CServerListData * pServerListData=CServerListData::GetInstance();

				//������
				for (UINT i=0;i<(wDataSize/sizeof(WORD));i++)
				{
					m_KindIDPending.RemoveKey(((WORD *)pData)[i]);
					pServerListData->OnEventKindFinish(((WORD *)pData)[i]);
				}

				//��ֹ����
				if ((m_KindIDPending.GetCount()==0L)&&(m_bUpdateOnLine==false))
				{
					CMissionManager * pMissionManager=GetMissionManager();
					if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);
				}

				return true;
			}
		case SUB_GR_KINE_ONLINE:	//��������
			{
				//��������
				CMD_GP_KindOnline * pKindOnline=(CMD_GP_KindOnline *)pData;
				WORD wHeadSize=(sizeof(CMD_GP_KindOnline)-sizeof(pKindOnline->OnLineInfoKind));

				//Ч������
				ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pKindOnline->wKindCount*sizeof(tagOnLineInfoKind))));
				if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pKindOnline->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

				//��ȡ����
				CServerListData * pServerListData=CServerListData::GetInstance();

				//��������
				for (WORD i=0;i<pKindOnline->wKindCount;i++)
				{
					tagOnLineInfoKind * pOnLineInfoKind=&pKindOnline->OnLineInfoKind[i];
					pServerListData->SetKindOnLineCount(pOnLineInfoKind->wKindID,pOnLineInfoKind->dwOnLineCount);
				}

				return true;
			}
		case SUB_GR_SERVER_ONLINE:	//��������
			{
				//��������
				CMD_GP_ServerOnline * pServerOnline=(CMD_GP_ServerOnline *)pData;
				WORD wHeadSize=(sizeof(CMD_GP_ServerOnline)-sizeof(pServerOnline->OnLineInfoServer));

				//Ч������
				ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pServerOnline->wServerCount*sizeof(tagOnLineInfoServer))));
				if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pServerOnline->wServerCount*sizeof(tagOnLineInfoServer)))) return false;

				//��ȡ����
				CServerListData * pServerListData=CServerListData::GetInstance();

				//��������
				for (WORD i=0;i<pServerOnline->wServerCount;i++)
				{
					tagOnLineInfoServer * pOnLineInfoServer=&pServerOnline->OnLineInfoServer[i];
					pServerListData->SetServerOnLineCount(pOnLineInfoServer->wServerID,pOnLineInfoServer->dwOnLineCount);
				}

				//��ֹ����
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

//��������
VOID CMissionList::UpdateOnLineInfo()
{
	//Ч��״̬
	ASSERT(m_bUpdateOnLine==false);
	if (m_bUpdateOnLine==true) return;

	//���ñ���
	m_bUpdateOnLine=true;

	//�رմ���
	if (m_hWnd!=NULL) DestroyWindow();

	//��������
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//���õ�ַ
	LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
	if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

	//��������
	pMissionManager->AvtiveMissionItem(this,false);

	return;
}

//��������
VOID CMissionList::UpdateServerInfo(WORD wKindID)
{
	//�رմ���
	if (m_hWnd!=NULL) DestroyWindow();

	//���ñ���
	m_KindIDMission[wKindID]=wKindID;

	//��������
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//���õ�ַ
	LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
	if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

	//��������
	pMissionManager->AvtiveMissionItem(this,false);
	
	return;
}

//ʱ����Ϣ
VOID CMissionList::OnTimer(UINT nIDEvent)
{
	//��������
	if (nIDEvent==IDI_AVTIVE_MISSION)
	{
		//�رմ���
		DestroyWindow();

		//��������
		ASSERT(GetMissionManager()!=NULL);
		CMissionManager * pMissionManager=GetMissionManager();

		//���õ�ַ
		LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
		if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

		//��������
		pMissionManager->AvtiveMissionItem(this,false);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
