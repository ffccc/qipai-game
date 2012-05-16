#include "StdAfx.h"
#include "AndroidUserItem.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
CTimerItemArray CAndroidUserItem::m_TimerItemStorage;					//�������

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItem::CAndroidUserItem()
{
	//״̬����
	m_bStartClient=false;
	m_cbGameStatus=GS_FREE;

	//�û�״̬
	m_CurrentUserStatus.cbUserStatus=US_NULL;
	m_CurrentUserStatus.wChairID=INVALID_CHAIR;
	m_CurrentUserStatus.wTableID=INVALID_TABLE;

	//�󶨱���
	m_dwUserID=0L;
	m_pIServerUserItem=NULL;

	//��������
	m_wRountID=1;
	m_wAndroidIndex=INVALID_WORD_INDEX;

	//�ӿڱ���
	m_pIServerUserManager=NULL;
	m_pIAndroidUserManager=NULL;
	m_pIAndroidUserItemSink=NULL;

	return;
}

//��������
CAndroidUserItem::~CAndroidUserItem()
{
	//ɾ��ʱ��
	KillGameTimer(0);

	//�ͷŶ���
	if (m_pIAndroidUserItemSink!=NULL)
	{
		m_pIAndroidUserItemSink->Release();
		m_pIAndroidUserItemSink=NULL;
	}

	return;
}

//�ӿڲ�ѯ
VOID * __cdecl CAndroidUserItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItem,Guid,dwQueryVer);
	return NULL;
}

//��ȡ I D
DWORD __cdecl CAndroidUserItem::GetUserID()
{
	return m_dwUserID;
}

//���Ӻ���
WORD __cdecl CAndroidUserItem::GetTableID()
{
	//Ч��״̬
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return INVALID_TABLE;

	return m_pIServerUserItem->GetTableID();
}

//���Ӻ���
WORD __cdecl CAndroidUserItem::GetChairID()
{
	//Ч��״̬
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return INVALID_CHAIR;

	return m_pIServerUserItem->GetChairID();
}

//��ȡ�Լ�
IServerUserItem * __cdecl CAndroidUserItem::GetMeUserItem()
{
	return m_pIServerUserItem;
}

//��Ϸ�û�
IServerUserItem * __cdecl CAndroidUserItem::GetTableUserItem(WORD wChariID)
{
	return NULL;
}

//���ͺ���
bool __cdecl CAndroidUserItem::SendSocketData(WORD wSubCmdID)
{
	//״̬Ч��
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//��������
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRountID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_GAME,wSubCmdID,NULL,0);

	return true;
}

//���ͺ���
bool __cdecl CAndroidUserItem::SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//��������
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRountID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

	return true;
}

//ɾ��ʱ��
bool __cdecl CAndroidUserItem::KillGameTimer(UINT nTimerID)
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
				m_TimerItemStorage.Add(pTimerItem);

				return true;
			}
		}
	}
	else
	{
		m_TimerItemStorage.Append(m_TimerItemActive);
		m_TimerItemActive.RemoveAll();
	}

	return false;
}

//����ʱ��
bool __cdecl CAndroidUserItem::SetGameTimer(UINT nTimerID, UINT nElapse)
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
	WORD wStorageCount=(WORD)m_TimerItemStorage.GetCount();

	//�������
	if (wStorageCount>0)
	{
		//��ȡ����
		pTimerItem=m_TimerItemStorage[wStorageCount-1];

		//��������
		m_TimerItemActive.Add(pTimerItem);
		m_TimerItemStorage.RemoveAt(wStorageCount-1);
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
bool __cdecl CAndroidUserItem::SendUserReady(VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT((m_pIServerUserItem!=NULL)&&(m_pIServerUserItem->GetUserStatus()==US_SIT));
	if ((m_pIServerUserItem==NULL)||(m_pIServerUserItem->GetUserStatus()!=US_SIT)) return false;

	//����׼��
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRountID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_FRAME,SUB_GF_USER_READY,pData,wDataSize);

	return true;
}

//��������
bool __cdecl CAndroidUserItem::SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)
{
	//Ч���û�
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//������Ϣ
	CMD_GF_UserChat UserChat;
	lstrcpyn(UserChat.szChatMessage,pszChatMessage,CountArray(UserChat.szChatMessage));

	//��������
	UserChat.crFontColor=crFontColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.dwSendUserID=m_pIServerUserItem->GetUserID();
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatMessage);

	//��������
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRountID);
	WORD wSendSize=sizeof(UserChat)-sizeof(UserChat.szChatMessage)+UserChat.wChatLength*sizeof(WCHAR);
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
			m_TimerItemStorage.Add(pTimerItem);

			//ʱ��֪ͨ
			ASSERT(m_pIAndroidUserItemSink!=NULL);
			if (m_pIAndroidUserItemSink!=NULL) m_pIAndroidUserItemSink->OnEventTimer(pTimerItem->nTimerID);
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
	if ((wMainCmdID==MDM_GR_LOGON)&&(wSubCmdID==SUB_GR_LOGON_ERROR))
	{
		return OnSocketSubLogonFailed(pData,wDataSize);
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

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItem::OnSocketGameMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��Ϸ����
	if (wMainCmdID==MDM_GF_GAME)
	{
		ASSERT(m_pIAndroidUserItemSink!=NULL);
		return m_pIAndroidUserItemSink->OnEventGameMessage(wSubCmdID,pData,wDataSize);
	}

	//��ܴ���
	if (wMainCmdID==MDM_GF_FRAME)
	{
		//Ĭ�ϴ���
		switch (wSubCmdID)
		{
		case SUB_GF_OPTION:				//��Ϸ״̬
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GF_Option));
				if (wDataSize!=sizeof(CMD_GF_Option)) return false;

				//��Ϣ����
				CMD_GF_Option * pOption=(CMD_GF_Option *)pData;

				//���ñ���
				m_cbGameStatus=pOption->bGameStatus;

				return true;
			}
		case SUB_GF_SCENE:				//��Ϸ����
			{
				//��Ϸ����
				bool bLookonUser=(m_pIServerUserItem->GetUserStatus()==US_LOOKON);
				return m_pIAndroidUserItemSink->OnEventGameScene(m_cbGameStatus,bLookonUser,pData,wDataSize);
			}
		}
	}

	return true;
}

//��¼ʧ��
bool CAndroidUserItem::OnSocketSubLogonFailed(VOID * pData, WORD wDataSize)
{
	//�رմ���
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRountID);
	m_pIAndroidUserManager->DeleteAndroidUserItem(dwAndroidID);

	return true;
}

//��¼���
bool CAndroidUserItem::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//���ñ���
	m_cbGameStatus=GS_FREE;
	m_pIServerUserItem=m_pIServerUserManager->SearchOnLineUser(m_dwUserID);

	//״̬Ч��
	ASSERT(m_pIServerUserItem!=NULL);
	if (m_pIServerUserItem==NULL) return false;

	//�û�״̬
	m_CurrentUserStatus.wChairID=m_pIServerUserItem->GetChairID();
	m_CurrentUserStatus.wTableID=m_pIServerUserItem->GetTableID();
	m_CurrentUserStatus.cbUserStatus=m_pIServerUserItem->GetUserStatus();

	//�������
	//CMD_GR_UserRule UserRule;
	//ZeroMemory(&UserRule,sizeof(UserRule));

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
	if (pUserStatus->dwUserID!=m_dwUserID) return true;

	//�û�״̬
	tagUserStatus LastUserStatus;
	CopyMemory(&LastUserStatus,&m_CurrentUserStatus,sizeof(LastUserStatus));

	//����״̬
	m_CurrentUserStatus.wChairID=pUserStatus->wChairID;
	m_CurrentUserStatus.wTableID=pUserStatus->wTableID;
	m_CurrentUserStatus.cbUserStatus=pUserStatus->cbUserStatus;

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

//������Ϸ
VOID CAndroidUserItem::StartGameClient()
{
	//״̬�ж�
	ASSERT((m_bStartClient==false)&&(m_pIServerUserItem!=NULL));
	ASSERT((m_pIServerUserItem->GetTableID()!=INVALID_TABLE)&&(m_pIServerUserItem->GetChairID()!=INVALID_CHAIR));

	//���ñ���
	m_bStartClient=true;

	//��������
	CMD_GF_Info Info;
	ZeroMemory(&Info,sizeof(Info));

	//��������
	Info.bAllowLookon=FALSE;

	//��������
	DWORD dwAndroidID=MAKELONG(m_wAndroidIndex,m_wRountID);
	m_pIAndroidUserManager->SendDataToServer(dwAndroidID,MDM_GF_FRAME,SUB_GF_INFO,&Info,sizeof(Info));

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
	m_cbGameStatus=GS_FREE;

	//��Ϸ��λ
	if (m_pIAndroidUserItemSink!=NULL) m_pIAndroidUserItemSink->RepositUserItemSink();

	return;
}

//��λ����
VOID CAndroidUserItem::RepositUserItem()
{
	//״̬����
	m_bStartClient=false;
	m_cbGameStatus=GS_FREE;

	//�û�״̬
	m_CurrentUserStatus.cbUserStatus=US_NULL;
	m_CurrentUserStatus.wChairID=INVALID_CHAIR;
	m_CurrentUserStatus.wTableID=INVALID_TABLE;

	//�󶨱���
	m_dwUserID=0L;
	m_pIServerUserItem=NULL;

	//ɾ��ʱ��
	m_TimerItemStorage.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	//��������
	m_wRountID=__max(1,m_wRountID+1);

	//��λ��Ϸ
	if (m_pIAndroidUserItemSink!=NULL) m_pIAndroidUserItemSink->RepositUserItemSink();

	return;
}

//////////////////////////////////////////////////////////////////////////
