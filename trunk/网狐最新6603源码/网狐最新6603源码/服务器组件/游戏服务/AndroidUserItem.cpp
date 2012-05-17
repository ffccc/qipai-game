#include "StdAfx.h"
#include "AndroidUserItem.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CTimerItemArray CAndroidUserItem::m_TimerItemBuffer;					//�������

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItem::CAndroidUserItem()
{
	//��������
	m_wRoundID=1;
	m_wAndroidIndex=INVALID_WORD;

	//�ӿڱ���
	m_pIServerUserManager=NULL;
	m_pIAndroidUserManager=NULL;
	m_pIAndroidUserItemSink=NULL;

	//״̬����
	m_bStartClient=false;
	m_cbGameStatus=GAME_STATUS_FREE;

	//�û�״̬
	m_CurrentUserStatus.cbUserStatus=US_NULL;
	m_CurrentUserStatus.wChairID=INVALID_CHAIR;
	m_CurrentUserStatus.wTableID=INVALID_TABLE;

	//�󶨱���
	m_dwReposeTime=0L;
	m_pIServerUserItem=NULL;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	return;
}

//��������
CAndroidUserItem::~CAndroidUserItem()
{
	//ɾ��ʱ��
	KillGameTimer(0);

	//�ͷŶ���
	SafeRelease(m_pIAndroidUserItemSink);

	return;
}

//�ӿڲ�ѯ
VOID * CAndroidUserItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItem,Guid,dwQueryVer);
	return NULL;
}

//��ȡ I D
DWORD CAndroidUserItem::GetUserID()
{
	return m_AndroidParameter.dwUserID;
}

//���Ӻ���
WORD CAndroidUserItem::GetTableID()
{
	//Ч��״̬
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return INVALID_TABLE;

	return m_pIServerUserItem->GetTableID();
}

//���Ӻ���
WORD CAndroidUserItem::GetChairID()
{
	//Ч��״̬
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return INVALID_CHAIR;

	return m_pIServerUserItem->GetChairID();
}

//��ȡ�Լ�
IServerUserItem * CAndroidUserItem::GetMeUserItem()
{
	return m_pIServerUserItem;
}

//��Ϸ�û�
IServerUserItem * CAndroidUserItem::GetTableUserItem(WORD wChariID)
{
	return NULL;
}

//���ͺ���
bool CAndroidUserItem::SendSocketData(WORD wSubCmdID)
{
	//״̬Ч��
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//��������
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_GAME,wSubCmdID,NULL,0);

	return true;
}

//������
bool CAndroidUserItem::PerformSaveScore(SCORE lScore)
{
	//״̬Ч��
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//Ч����
	ASSERT(m_pIServerUserItem->GetUserScore()>=lScore);
	if (m_pIServerUserItem->GetUserScore()<lScore) return false;

	//�޸Ľ��
	m_pIServerUserItem->ModifyUserInsure(-lScore,0L,0L);

	return true;
}

//��ȡ���
bool CAndroidUserItem::PerformTakeScore(SCORE lScore)
{
	//״̬Ч��
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//�޸Ľ��
	m_pIServerUserItem->ModifyUserInsure(lScore,0L,0L);

	return true;
}

//���ͺ���
bool CAndroidUserItem::SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//��������
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

	return true;
}

//ɾ��ʱ��
bool CAndroidUserItem::KillGameTimer(UINT nTimerID)
{
	//ɾ��ʱ��
	if (nTimerID!=0)
	{
		//Ѱ������
		for (INT_PTR i=0;i<m_TimerItemActive.GetCount();i++)
		{
			//��ȡʱ��
			tagTimerItem * pTimerItem=m_TimerItemActive[i];

			//ɾ���ж�
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

//����ʱ��
bool CAndroidUserItem::SetGameTimer(UINT nTimerID, UINT nElapse)
{
	//Ѱ������
	for (INT_PTR i=0;i<m_TimerItemActive.GetCount();i++)
	{
		//��ȡʱ��
		tagTimerItem * pTimerItem=m_TimerItemActive[i];

		//�����ж�
		if (pTimerItem->nTimerID==nTimerID)
		{
			pTimerItem->nTimeLeave=nElapse;
			return true;
		}
	}

	//��������
	tagTimerItem * pTimerItem=NULL;
	WORD wStorageCount=(WORD)m_TimerItemBuffer.GetCount();

	//�������
	if (wStorageCount>0)
	{
		//��ȡ����
		pTimerItem=m_TimerItemBuffer[wStorageCount-1];

		//��������
		m_TimerItemActive.Add(pTimerItem);
		m_TimerItemBuffer.RemoveAt(wStorageCount-1);
	}

	//��������
	if (pTimerItem==NULL)
	{
		try
		{
			//��������
			pTimerItem=new tagTimerItem;
			if (pTimerItem==NULL) return false;

			//��������
			m_TimerItemActive.Add(pTimerItem);
		}
		catch (...)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//���ñ���
	pTimerItem->nTimerID=nTimerID;
	pTimerItem->nTimeLeave=nElapse;

	return true;
}

//����׼��
bool CAndroidUserItem::SendUserReady(VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT((m_pIServerUserItem!=NULL)&&(m_pIServerUserItem->GetUserStatus()==US_SIT));
	if ((m_pIServerUserItem==NULL)||(m_pIServerUserItem->GetUserStatus()!=US_SIT)) return false;

	//����׼��
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_FRAME,SUB_GF_USER_READY,pData,wDataSize);

	return true;
}

//��������
bool CAndroidUserItem::SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//Ч���û�
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//������Ϣ
	CMD_GF_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//��������
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//��������
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	WORD wSendSize=sizeof(UserChat)-sizeof(UserChat.szChatString)+UserChat.wChatLength*sizeof(TCHAR);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);

	return true;
}

//ʱ���¼�
bool CAndroidUserItem::OnTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//Ѱ������
	for (INT_PTR i=0;i<m_TimerItemActive.GetCount();)
	{
		//��������
		tagTimerItem * pTimerItem=m_TimerItemActive[i];

		//ʱ�䴦��
		if (pTimerItem->nTimeLeave<=1L)
		{
			//��������
			m_TimerItemActive.RemoveAt(i);
			m_TimerItemBuffer.Add(pTimerItem);

			//ʱ��֪ͨ
			if (m_pIAndroidUserItemSink!=NULL)
			{
				m_pIAndroidUserItemSink->OnEventTimer(pTimerItem->nTimerID);
			}
		}
		else
		{
			//���ñ���
			i++;
			pTimerItem->nTimeLeave--;
		}
	}


	return true;
}

//������Ϣ
bool CAndroidUserItem::OnSocketRead(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��¼ʧ��
	if ((wMainCmdID==MDM_GR_LOGON)&&(wSubCmdID==SUB_GR_LOGON_FAILURE))
	{
		return OnSocketSubLogonFailure(pData,wDataSize);
	}

	//��¼���
	if ((wMainCmdID==MDM_GR_LOGON)&&(wSubCmdID==SUB_GR_LOGON_FINISH))
	{
		return OnSocketSubLogonFinish(pData,wDataSize);
	}

	//�û�״̬
	if ((wMainCmdID==MDM_GR_USER)&&(wSubCmdID==SUB_GR_USER_STATUS))
	{
		return OnSocketSubUserStatus(pData,wDataSize);
	}

	//��Ϸ��Ϣ
	if ((wMainCmdID==MDM_GF_GAME)||(wMainCmdID==MDM_GF_FRAME))
	{
		return OnSocketGameMessage(wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	//ϵͳ��Ϣ
	if ((wMainCmdID==MDM_CM_SYSTEM)&&(wSubCmdID==SUB_CM_SYSTEM_MESSAGE))
	{
		return OnSocketSubSystemMessage(pData,wDataSize);
	}

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItem::OnSocketGameMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��Ϸ����
	if ((wMainCmdID==MDM_GF_GAME)&&(m_pIAndroidUserItemSink!=NULL))
	{
		return m_pIAndroidUserItemSink->OnEventGameMessage(wSubCmdID,pData,wDataSize);
	}

	//��ܴ���
	if (wMainCmdID==MDM_GF_FRAME)
	{
		//Ĭ�ϴ���
		switch (wSubCmdID)
		{
		case SUB_GF_GAME_STATUS:		//��Ϸ״̬
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GF_GameStatus));
				if (wDataSize!=sizeof(CMD_GF_GameStatus)) return false;

				//��Ϣ����
				CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)pData;

				//���ñ���
				m_cbGameStatus=pGameStatus->cbGameStatus;

				return true;
			}
		case SUB_GF_GAME_SCENE:			//��Ϸ����
			{
				//��Ϸ����
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

//��¼ʧ��
bool CAndroidUserItem::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	return true;
}

//��¼���
bool CAndroidUserItem::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//���ñ���
	m_cbGameStatus=GAME_STATUS_FREE;
	m_pIServerUserItem=m_pIServerUserManager->SearchUserItem(m_AndroidParameter.dwUserID);

	//״̬Ч��
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//�û�״̬
	m_CurrentUserStatus.wChairID=m_pIServerUserItem->GetChairID();
	m_CurrentUserStatus.wTableID=m_pIServerUserItem->GetTableID();
	m_CurrentUserStatus.cbUserStatus=m_pIServerUserItem->GetUserStatus();

	//�������
	CMD_GR_UserRule UserRule;
	ZeroMemory(&UserRule,sizeof(UserRule));

	//���͹���
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GR_USER,SUB_GR_USER_RULE,&UserRule,sizeof(UserRule));

	//�����ж�
	if (m_pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		StartGameClient();
	}

	return true;
}

//�û�״̬
bool CAndroidUserItem::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//��������
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;

	//����ж�
	if (pUserStatus->dwUserID!=m_AndroidParameter.dwUserID)
	{
		return true;
	}

	//�û�״̬
	tagUserStatus LastUserStatus;
	CopyMemory(&LastUserStatus,&m_CurrentUserStatus,sizeof(LastUserStatus));
	CopyMemory(&m_CurrentUserStatus,&pUserStatus->UserStatus,sizeof(m_CurrentUserStatus));

	//��ʼ�л�
	if ((LastUserStatus.cbUserStatus!=US_READY)&&(m_CurrentUserStatus.cbUserStatus==US_READY))
	{
	}

	//�ر��ж�
	if ((m_bStartClient==true)&&(m_CurrentUserStatus.wTableID==INVALID_TABLE))
	{
		//�ر���Ϸ
		CloseGameClient();

		return true;
	}

	//�����ж�
	if ((m_bStartClient==false)&&(m_CurrentUserStatus.wTableID!=INVALID_TABLE))
	{
		//������Ϸ
		StartGameClient();

		return true;
	}

	return true;
}

//ϵͳ��Ϣ
bool CAndroidUserItem::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//��������
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//Ч�����
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(WCHAR)))) return false;

	//�رմ���
	if ((pSystemMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//�رմ���
		DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
		m_pIAndroidUserManager->DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//������Ϸ
VOID CAndroidUserItem::StartGameClient()
{
	//״̬�ж�
	ASSERT((m_bStartClient==false)&&(m_pIServerUserItem!=NULL));
	ASSERT((m_pIServerUserItem->GetTableID()!=INVALID_TABLE)&&(m_pIServerUserItem->GetChairID()!=INVALID_CHAIR));

	//���ñ���
	m_bStartClient=true;

	//��������
	CMD_GF_GameOption GameOption;
	ZeroMemory(&GameOption,sizeof(GameOption));

	//��������
	GameOption.cbAllowLookon=FALSE;
	GameOption.dwFrameVersion=INVALID_DWORD;
	GameOption.dwClientVersion=INVALID_DWORD;

	//��������
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRoundID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_FRAME,SUB_GF_GAME_OPTION,&GameOption,sizeof(GameOption));

	return;
}

//�ر���Ϸ
VOID CAndroidUserItem::CloseGameClient()
{
	//״̬�ж�
	ASSERT((m_bStartClient==true)&&(m_pIServerUserItem!=NULL));
	ASSERT((m_pIServerUserItem->GetTableID()==INVALID_TABLE)&&(m_pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//ɾ��ʱ��
	KillGameTimer(0);

	//���ñ���
	m_bStartClient=false;
	m_cbGameStatus=GAME_STATUS_FREE;

	//��Ϸ��λ
	if (m_pIAndroidUserItemSink!=NULL)
	{
		m_pIAndroidUserItemSink->RepositionSink();
	}

	return;
}

//��λ����
VOID CAndroidUserItem::RepositUserItem()
{
	//״̬����
	m_bStartClient=false;
	m_cbGameStatus=GAME_STATUS_FREE;

	//�û�״̬
	m_CurrentUserStatus.cbUserStatus=US_NULL;
	m_CurrentUserStatus.wChairID=INVALID_CHAIR;
	m_CurrentUserStatus.wTableID=INVALID_TABLE;

	//�󶨱���
	m_dwReposeTime=0L;
	m_pIServerUserItem=NULL;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	//ɾ��ʱ��
	m_TimerItemBuffer.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	//��������
	m_wRoundID=__max(1,m_wRoundID+1);

	//��λ��Ϸ
	if (m_pIAndroidUserItemSink!=NULL)
	{
		m_pIAndroidUserItemSink->RepositionSink();
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
