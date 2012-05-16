#include "StdAfx.h"
#include "AndroidUserManager.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define ANDROID_LOGON_COUNT			128									//��¼��Ŀ

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserManager::CAndroidUserManager()
{
	//���ñ���
	m_wAutoAndroidCount=ANDROID_LOGON_COUNT;

	//����ӿ�
	m_pITimerEngine=NULL;
	m_pIServerUserManager=NULL;
	m_pIGameServiceManager=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//ϵͳ����
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//������
	m_wStockCount=0;
	ZeroMemory(m_dwAndroidStock,sizeof(m_dwAndroidStock));

	//��������
	m_AndroidUserItemMap.InitHashTable(503L);

	return;
}

//��������
CAndroidUserManager::~CAndroidUserManager()
{
	//�������
	ASSERT(m_AndroidUserItemMap.GetCount()==0L);
	ASSERT(m_AndroidUserItemFree.GetCount()==0L);
	ASSERT(m_AndroidUserItemStorage.GetCount()==0L);

	return;
}

//�ӿڲ�ѯ
VOID * __cdecl CAndroidUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserManager,Guid,dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CAndroidUserManager::StartService()
{
	//����ʱ��
	m_pITimerEngine->SetTimer(IDI_ANDROID_INOUT,TIME_ANDROID_INOUT*1000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_ANDROID_USER_PULSE,TIME_ANDROID_USER_PULSE*1000L,TIMES_INFINITY,0);

	return true;
}

//ֹͣ����
bool __cdecl CAndroidUserManager::StopService()
{
	//ɾ���洢
	for (INT_PTR i=0;i<m_AndroidUserItemStorage.GetCount();i++)
	{
		CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemStorage[i];
		SafeDelete(pAndroidUserItem);
	}

	//��������
	m_AndroidUserItemMap.RemoveAll();
	m_AndroidUserItemFree.RemoveAll();
	m_AndroidUserItemStorage.RemoveAll();

	//����û�
	m_wStockCount=0;
	ZeroMemory(m_dwAndroidStock,sizeof(m_dwAndroidStock));

	return true;
}

//���ÿ��
bool __cdecl CAndroidUserManager::SetAndroidStock(DWORD dwAndroidStock[], WORD wStockCount)
{
	//�������
	m_wStockCount=0;
	ZeroMemory(m_dwAndroidStock,sizeof(m_dwAndroidStock));

	//�����û�
	for (WORD i=0;i<wStockCount;i++)
	{
		//��ĿЧ��
		ASSERT(m_wStockCount<CountArray(m_dwAndroidStock));
		if (m_wStockCount>=CountArray(m_dwAndroidStock)) break;

		//���ñ���
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.Lookup(dwAndroidStock[i],pAndroidUserItem);

		//���ñ���
		if (pAndroidUserItem==NULL)
		{
			DWORD dwUserID=dwAndroidStock[i];
			m_dwAndroidStock[m_wStockCount++]=dwUserID;
		}
	}

	return true;
}

//ɾ��������
bool __cdecl CAndroidUserManager::DeleteAndroidUserItem(DWORD dwAndroidID)
{
	//��ȡ����
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);

	//����Ч��
	ASSERT((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRountID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRountID!=HIWORD(dwAndroidID))) return false;

	//�ر��¼�
	//bool bSuccess=false;
	try
	{
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwAndroidID,0,0L);
	}
	catch (...)
	{
		//�������
		ASSERT(FALSE);
	}

	//ɾ������
	FreeAndroidUserItem(pAndroidUserItem);

	return true;
}

//�л�������
IAndroidUserItem * __cdecl CAndroidUserManager::SwitchAndroidUserItem(IServerUserItem * pIServerUserItem)
{
	return NULL;
}

//����������
IAndroidUserItem * __cdecl CAndroidUserManager::CreateAndroidUserItem(DWORD dwUserID, TCHAR szPassword[PASS_LEN])
{
	//��������
	CAndroidUserItem * pAndroidUserItem=ActiveAndroidUserItem(dwUserID);
	if (pAndroidUserItem==NULL) return NULL;

	//���Ա���
	WORD wRoundID=pAndroidUserItem->m_wRountID;
	WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;

	//����ģ��
	try
	{
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkBind(MAKELONG(wAndroidIndex,wRoundID),0L)==false)
		{
			throw 0;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		FreeAndroidUserItem(pAndroidUserItem);
		return NULL;
	}

	//��������
	CMD_GR_LogonByUserID LogonByUserID;
	ZeroMemory(&LogonByUserID,sizeof(LogonByUserID));

	//��������
	LogonByUserID.dwUserID=dwUserID;
	lstrcpyn(LogonByUserID.szPassWord,szPassword,CountArray(LogonByUserID.szPassWord));

	//��������
	CMD_Command Command;
	Command.wMainCmdID=MDM_GR_LOGON;
	Command.wSubCmdID=SUB_GR_LOGON_USERID;

	//��Ϣ����
	try
	{
		//��������
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(MAKELONG(wAndroidIndex,wRoundID),Command,&LogonByUserID,sizeof(LogonByUserID))==false)
		{
			throw 0;
		}
	}
	catch (...) 
	{
		ASSERT(FALSE);
		DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));
		return NULL;
	}

	return pAndroidUserItem;
}

//�����¼�
bool __cdecl CAndroidUserManager::OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)
{
	switch (dwTimerID)
	{
	case IDI_ANDROID_INOUT:			//��������
		{
			//��������
			DWORD dwUserID=0L;
			INT_PTR nActiveCount=m_AndroidUserItemMap.GetCount();
			POSITION Position=m_AndroidUserItemMap.GetStartPosition();

			//��¼����
			if ((m_wStockCount>0)&&(nActiveCount<m_wAutoAndroidCount))
			{
				//��ȡ�û�
				WORD wIndex=rand()%m_wStockCount;
				DWORD dwUserID=m_dwAndroidStock[wIndex];

				//ɾ�����
				m_wStockCount--;
				m_dwAndroidStock[wIndex]=m_dwAndroidStock[m_wStockCount];

				//����������
				CreateAndroidUserItem(dwUserID,NULL);

				return true;
			}

			//�˳�����
			while (Position!=NULL)
			{
				//��ȡ����
				CAndroidUserItem * pAndroidUserItem=NULL;
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//�˳��ж�
				if (pAndroidUserItem->m_pIServerUserItem!=NULL)
				{
					//��ȡ����
					BYTE cbUserStatus=pAndroidUserItem->m_pIServerUserItem->GetUserStatus();
					DWORD dwOnlineTimeCount=pAndroidUserItem->m_pIServerUserItem->GetOnlineTimeCount();

					//״̬�ж�
					if (dwOnlineTimeCount<=TIME_ANDROID_SERVICE) continue;
					if ((cbUserStatus!=US_FREE)&&(cbUserStatus!=US_SIT)) continue;

					//ɾ���û�
					WORD wRoundID=pAndroidUserItem->m_wRountID;
					WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
					DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

					return true;
				}
			}

			return true;
		}
	case IDI_ANDROID_USER_PULSE:	//�û�����
		{
			//��������
			DWORD dwUserID=0L;
			CAndroidUserItem * pAndroidUserItem=NULL;
			POSITION Position=m_AndroidUserItemMap.GetStartPosition();

			//�û�����
			while (Position!=NULL)
			{
				//��ȡ����
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//ʱ�䴦��
				try
				{
					ASSERT(pAndroidUserItem!=NULL);
					if (pAndroidUserItem->m_pIServerUserItem!=NULL) pAndroidUserItem->OnTimerPulse(dwTimerID,dwBindParameter);
				}
				catch (...)
				{
					ASSERT(FALSE);
					DeleteAndroidUserItem(MAKELONG(pAndroidUserItem->m_wAndroidIndex,pAndroidUserItem->m_wRountID));
				}
			}

			return true;
		}
	}

	return false;
}

//��������
bool __cdecl CAndroidUserManager::SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	for (INT_PTR i=0;i<m_AndroidUserItemStorage.GetCount();i++)
	{
		//��ȡ����
		CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemStorage[i];

		//״̬�ж�
		if (pAndroidUserItem->m_dwUserID==0L) continue;
		if (pAndroidUserItem->m_pIServerUserItem==NULL) continue;

		//��Ϣ����
		try
		{
			if (pAndroidUserItem->OnSocketRead(wMainCmdID,wSubCmdID,pData,wDataSize)==false)
			{
				throw 0;
			}
		}
		catch (...) 
		{
			//�������
			ASSERT(FALSE);

			//�Ͽ��û�
			WORD wRoundID=pAndroidUserItem->m_wRountID;
			WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
			DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));
		}
	}

	return true;
}

//��������
bool __cdecl CAndroidUserManager::SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��ȡ����
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);

	//����Ч��
	ASSERT((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRountID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRountID!=HIWORD(dwAndroidID))) return false;

	//��Ϣ����
	try
	{
		if (pAndroidUserItem->OnSocketRead(wMainCmdID,wSubCmdID,pData,wDataSize)==false)
		{
			throw 0;
		}
	}
	catch (...) 
	{
		ASSERT(FALSE);
		DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//��������
bool __cdecl CAndroidUserManager::SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	CMD_Command Command;
	Command.wSubCmdID=wSubCmdID;
	Command.wMainCmdID=wMainCmdID;

	//��Ϣ����
	try
	{
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(dwAndroidID,Command,pData,wDataSize)==false)
		{
			throw 0;
		}
	}
	catch (...) 
	{
		ASSERT(FALSE);
		DeleteAndroidUserItem(dwAndroidID);
	}

	return true;
}

//��ȡ����
CAndroidUserItem * CAndroidUserManager::GetAndroidUserItem(WORD wIndex)
{
	//Ч������
	ASSERT(wIndex>=INDEX_ANDROID);
	if (wIndex<INDEX_ANDROID) return NULL;

	//Ч������
	ASSERT((wIndex-INDEX_ANDROID)<m_AndroidUserItemStorage.GetCount());
	if (((wIndex-INDEX_ANDROID)>=m_AndroidUserItemStorage.GetCount())) return NULL;

	//��ȡ����
	WORD wStorageIndex=wIndex-INDEX_ANDROID;
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemStorage[wStorageIndex];

	return pAndroidUserItem;
}

//�������
CAndroidUserItem * CAndroidUserManager::ActiveAndroidUserItem(DWORD dwUserID)
{
	//��������
	CAndroidUserItem * pAndroidUserItem=NULL;
	INT_PTR nFreeItemCount=m_AndroidUserItemFree.GetCount();

	//��ȡ����
	if (nFreeItemCount>0)
	{
		//��ȡ����
		INT_PTR nItemPostion=nFreeItemCount-1;
		pAndroidUserItem=m_AndroidUserItemFree[nItemPostion];

		//ɾ������
		m_AndroidUserItemFree.RemoveAt(nItemPostion);
	}

	//��������
	if (pAndroidUserItem==NULL)
	{
		//��Ŀ�ж�
		if (m_AndroidUserItemStorage.GetCount()>=MAX_ANDROID) return NULL;

		try
		{
			//��������
			pAndroidUserItem=new CAndroidUserItem;
			if (pAndroidUserItem==NULL)
			{
				ASSERT(FALSE);
				return NULL;
			}

			//��������
			m_AndroidUserItemStorage.Add(pAndroidUserItem);

			//��������
			WORD wCurrentIndex=(WORD)m_AndroidUserItemStorage.GetCount()-1;
			IAndroidUserItemSink * pIAndroidUserItemSink=(IAndroidUserItemSink *)
				m_pIGameServiceManager->CreateAndroidUserItemSink(IID_IAndroidUserItemSink,VER_IAndroidUserItemSink);

			//�����û�
			if ((pIAndroidUserItemSink!=NULL)&&(pIAndroidUserItemSink->InitUserItemSink(pAndroidUserItem)==false))
			{
				pIAndroidUserItemSink->Release();
				pIAndroidUserItemSink=NULL;
			}

			//���ýӿ�
			pAndroidUserItem->m_wAndroidIndex=wCurrentIndex+INDEX_ANDROID;
			pAndroidUserItem->m_pIServerUserManager=m_pIServerUserManager;
			pAndroidUserItem->m_pIAndroidUserItemSink=pIAndroidUserItemSink;
			pAndroidUserItem->m_pIAndroidUserManager=QUERY_ME_INTERFACE(IAndroidUserManager);
		}
		catch (...) 
		{ 
			ASSERT(FALSE);
			return NULL; 
		}
	}

	//���ñ���
	pAndroidUserItem->m_dwUserID=dwUserID;

	//��������
	m_AndroidUserItemMap[dwUserID]=pAndroidUserItem;

	return pAndroidUserItem;
}

//�ͷŶ���
bool CAndroidUserManager::FreeAndroidUserItem(CAndroidUserItem * pAndroidUserItem)
{
	//Ч�����
	ASSERT(pAndroidUserItem!=NULL);
	if (pAndroidUserItem==NULL) return false;

	//��������
	DWORD dwUserID=pAndroidUserItem->m_dwUserID;
	CAndroidUserItem * pAndroidUserItemMap=NULL;
	m_AndroidUserItemMap.Lookup(dwUserID,pAndroidUserItemMap);

	//�����ж�
	if (pAndroidUserItemMap==pAndroidUserItem)
	{
		//��λ����
		pAndroidUserItem->RepositUserItem();

		//��������
		m_AndroidUserItemMap.RemoveKey(dwUserID);
		m_AndroidUserItemFree.Add(pAndroidUserItem);

		return true;
	}

	//�ͷ�ʧ��
	ASSERT(FALSE);

	return false;
}

//////////////////////////////////////////////////////////////////////////
