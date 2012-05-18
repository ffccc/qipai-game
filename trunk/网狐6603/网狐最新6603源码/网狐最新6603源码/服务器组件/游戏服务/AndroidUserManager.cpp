#include "StdAfx.h"
#include "AndroidUserManager.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define ANDROID_LOGON_COUNT			32									//��¼��Ŀ

//ʱ���ʶ
#define IDI_ANDROID_INOUT			(IDI_REBOT_MODULE_START+0)			//����ʱ��
#define IDI_ANDROID_PULSE			(IDI_REBOT_MODULE_START+1)			//����ʱ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserManager::CAndroidUserManager()
{
	//ϵͳ����
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//����ӿ�
	m_pITimerEngine=NULL;
	m_pIServerUserManager=NULL;
	m_pIGameServiceManager=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//���ñ���
	m_wAutoAndroidCount=ANDROID_LOGON_COUNT;

	//������
	m_wStockCount=0;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	//��������
	m_AndroidUserItemMap.InitHashTable(PRIME_ANDROID_USER);

	return;
}

//��������
CAndroidUserManager::~CAndroidUserManager()
{
	//�������
	ASSERT(m_AndroidUserItemMap.GetCount()==0L);
	ASSERT(m_AndroidUserItemArray.GetCount()==0L);
	ASSERT(m_AndroidUserItemBuffer.GetCount()==0L);

	return;
}

//�ӿڲ�ѯ
VOID * CAndroidUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserManager,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CAndroidUserManager::StartService()
{
	//����ʱ��
	m_pITimerEngine->SetTimer(IDI_ANDROID_INOUT,TIME_ANDROID_INOUT*1000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_ANDROID_PULSE,TIME_ANDROID_PULSE*1000L,TIMES_INFINITY,0);

	return true;
}

//ֹͣ����
bool CAndroidUserManager::ConcludeService()
{
	//ɾ���洢
	for (INT_PTR i=0;i<m_AndroidUserItemArray.GetCount();i++)
	{
		SafeRelease(m_AndroidUserItemArray[i]);
	}

	//��������
	m_AndroidUserItemMap.RemoveAll();
	m_AndroidUserItemArray.RemoveAll();
	m_AndroidUserItemBuffer.RemoveAll();

	//����û�
	m_wStockCount=0;
	ZeroMemory(&m_AndroidParameter,sizeof(m_AndroidParameter));

	return true;
}

//�û�״��
WORD CAndroidUserManager::GetAndroidUserInfo(tagAndroidUserInfo & AndroidUserInfo)
{
	//���ñ���
	ZeroMemory(&AndroidUserInfo,sizeof(AndroidUserInfo));

	//��������
	POSITION Position=m_AndroidUserItemMap.GetStartPosition();

	//ö�ٶ���
	while (Position!=NULL)
	{
		//��ȡ����
		DWORD dwUserID=0L;
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

		//Ч�����
		ASSERT((dwUserID!=0L)&&(pAndroidUserItem!=NULL));
		if ((dwUserID==0L)||(pAndroidUserItem==NULL)) break;

		//���ж�
		if (pAndroidUserItem->m_pIServerUserItem==NULL) continue;

		//��������
		IServerUserItem * pIServerUserItem=pAndroidUserItem->m_pIServerUserItem;

		//״̬�ж�
		switch (pIServerUserItem->GetUserStatus())
		{
		case US_FREE:
			{
				ASSERT(AndroidUserInfo.wFreeUserCount<CountArray(AndroidUserInfo.pIAndroidUserFree));
				AndroidUserInfo.pIAndroidUserFree[AndroidUserInfo.wFreeUserCount++]=pAndroidUserItem;
				break;
			}
		case US_SIT:
		case US_READY:
			{
				ASSERT(AndroidUserInfo.wSitdownUserCount<CountArray(AndroidUserInfo.pIAndroidUserSitdown));
				AndroidUserInfo.pIAndroidUserSitdown[AndroidUserInfo.wSitdownUserCount++]=pAndroidUserItem;
				break;
			}
		case US_PLAYING:
		case US_OFFLINE:
			{
				ASSERT(AndroidUserInfo.wPlayUserCount<CountArray(AndroidUserInfo.pIAndroidUserPlay));
				AndroidUserInfo.pIAndroidUserPlay[AndroidUserInfo.wPlayUserCount++]=pAndroidUserItem;
				break;
			}
		}
	};

	return AndroidUserInfo.wFreeUserCount+AndroidUserInfo.wPlayUserCount+AndroidUserInfo.wSitdownUserCount;
}

//�������
bool CAndroidUserManager::InitAndroidUser(tagAndroidUserParameter & AndroidUserParameter)
{
	//���ñ���
	m_pGameParameter=AndroidUserParameter.pGameParameter;
	m_pGameServiceAttrib=AndroidUserParameter.pGameServiceAttrib;
	m_pGameServiceOption=AndroidUserParameter.pGameServiceOption;

	//����ӿ�
	m_pITimerEngine=AndroidUserParameter.pITimerEngine;
	m_pIServerUserManager=AndroidUserParameter.pIServerUserManager;
	m_pIGameServiceManager=AndroidUserParameter.pIGameServiceManager;
	m_pITCPNetworkEngineEvent=AndroidUserParameter.pITCPNetworkEngineEvent;

	return true;
}

//���ÿ��
bool CAndroidUserManager::SetAndroidStock(tagAndroidParameter AndroidParameter[], WORD wStockCount)
{
	//�����û�
	for (WORD i=0;i<wStockCount;i++)
	{
		//��ĿЧ��
		ASSERT(m_wStockCount<CountArray(m_AndroidParameter));
		if (m_wStockCount>=CountArray(m_AndroidParameter)) break;

		//���ñ���
		CAndroidUserItem * pAndroidUserItem=NULL;
		m_AndroidUserItemMap.Lookup(m_AndroidParameter[i].dwUserID,pAndroidUserItem);

		//���ñ���
		if (pAndroidUserItem==NULL)
		{
			m_AndroidParameter[m_wStockCount++]=AndroidParameter[i];
		}
	}

	return true;
}

//ɾ������
bool CAndroidUserManager::DeleteAndroidUserItem(DWORD dwAndroidID)
{
	//��ȡ����
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);

	//����Ч��
	ASSERT((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRoundID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRoundID!=HIWORD(dwAndroidID))) return false;

	//�ر��¼�
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
	FreeAndroidUserItem(pAndroidUserItem->GetUserID());

	return true;
}

//���һ���
IAndroidUserItem * CAndroidUserManager::SearchAndroidUserItem(DWORD dwUserID, DWORD dwContextID)
{
	//���һ���
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemMap[dwUserID];

	//�����ж�
	if (pAndroidUserItem!=NULL)
	{
		WORD wRoundID=pAndroidUserItem->m_wRoundID;
		WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
		if (MAKELONG(wAndroidIndex,wRoundID)==dwContextID) return pAndroidUserItem;
	}

	return NULL;
}

//��������
IAndroidUserItem * CAndroidUserManager::CreateAndroidUserItem(tagAndroidParameter & AndroidParameter)
{
	//Ч�����
	ASSERT(AndroidParameter.dwUserID!=0L);
	if (AndroidParameter.dwUserID==0L) return NULL;

	//��������
	CAndroidUserItem * pAndroidUserItem=ActiveAndroidUserItem(AndroidParameter);
	if (pAndroidUserItem==NULL) return NULL;

	//���Ա���
	WORD wRoundID=pAndroidUserItem->m_wRoundID;
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
		//�������
		ASSERT(FALSE);

		//�ͷ��û�
		FreeAndroidUserItem(AndroidParameter.dwUserID);

		return NULL;
	}

	//��������
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//�汾��Ϣ
	LogonUserID.dwPlazaVersion=VERSION_PLAZA;
	LogonUserID.dwFrameVersion=VERSION_FRAME;
	LogonUserID.dwProcessVersion=m_pGameServiceAttrib->dwClientVersion;

	//�û���Ϣ
	LogonUserID.dwUserID=AndroidParameter.dwUserID;
	lstrcpyn(LogonUserID.szPassword,TEXT(""),CountArray(LogonUserID.szPassword));

	//��������
	TCP_Command Command;
	Command.wMainCmdID=MDM_GR_LOGON;
	Command.wSubCmdID=SUB_GR_LOGON_USERID;

	//��Ϣ����
	try
	{
		//��������
		if (m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(MAKELONG(wAndroidIndex,wRoundID),Command,&LogonUserID,sizeof(LogonUserID))==false)
		{
			throw 0;
		}
	}
	catch (...) 
	{
		//�������
		ASSERT(FALSE);

		//ɾ������
		DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

		return NULL;
	}

	return pAndroidUserItem;
}

//�����¼�
bool CAndroidUserManager::OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter)
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
			if (CServerRule::IsAllowAndroidUser(m_pGameServiceOption->dwServerRule)==true)
			{
				//��洦��
				if ((m_wStockCount>0)&&(nActiveCount<m_wAutoAndroidCount))
				{
					//��ȡ�û�
					WORD wIndex=rand()%m_wStockCount;

					//��������
					if (CreateAndroidUserItem(m_AndroidParameter[wIndex])!=NULL)
					{
						//ɾ�����
						m_wStockCount--;
						m_AndroidParameter[wIndex]=m_AndroidParameter[m_wStockCount];
					}

					return true;
				}
			}

			//��������
			DWORD dwCurrentTime=(DWORD)time(NULL);
			DWORD dwServerRule=m_pGameServiceOption->dwServerRule;

			//�˳�����
			while (Position!=NULL)
			{
				//��ȡ����
				CAndroidUserItem * pAndroidUserItem=NULL;
				m_AndroidUserItemMap.GetNextAssoc(Position,dwUserID,pAndroidUserItem);

				//�˳��ж�
				if (pAndroidUserItem->m_pIServerUserItem!=NULL)
				{
					//��������
					IServerUserItem * pIServerUserItem=pAndroidUserItem->m_pIServerUserItem;

					//����״̬
					BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
					if ((cbUserStatus!=US_FREE)&&(cbUserStatus!=US_SIT)) continue;

					//����ʱ��
					DWORD dwLogonTime=pIServerUserItem->GetLogonTime();
					DWORD dwReposeTime=pAndroidUserItem->m_dwReposeTime;
					if (((dwLogonTime+dwLogonTime)>dwCurrentTime)&&(CServerRule::IsAllowAndroidUser(dwServerRule)==true)) continue;

					//ɾ���û�
					WORD wRoundID=pAndroidUserItem->m_wRoundID;
					WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
					DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));

					return true;
				}
			}

			return true;
		}
	case IDI_ANDROID_PULSE:		//�û�����
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
					DeleteAndroidUserItem(MAKELONG(pAndroidUserItem->m_wAndroidIndex,pAndroidUserItem->m_wRoundID));
				}
			}

			return true;
		}
	}

	return false;
}

//��������
bool CAndroidUserManager::SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	for (INT_PTR i=0;i<m_AndroidUserItemArray.GetCount();i++)
	{
		//��ȡ����
		CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemArray[i];

		//״̬�ж�
		if (pAndroidUserItem->m_pIServerUserItem==NULL) continue;
		if (pAndroidUserItem->m_AndroidParameter.dwUserID==0L) continue;

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
			WORD wRoundID=pAndroidUserItem->m_wRoundID;
			WORD wAndroidIndex=pAndroidUserItem->m_wAndroidIndex;
			DeleteAndroidUserItem(MAKELONG(wAndroidIndex,wRoundID));
		}
	}

	return true;
}

//��������
bool CAndroidUserManager::SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��ȡ����
	WORD wIndex=LOWORD(dwAndroidID);
	CAndroidUserItem * pAndroidUserItem=GetAndroidUserItem(wIndex);

	//����Ч��
	ASSERT((pAndroidUserItem!=NULL)&&(pAndroidUserItem->m_wRoundID==HIWORD(dwAndroidID)));
	if ((pAndroidUserItem==NULL)||(pAndroidUserItem->m_wRoundID!=HIWORD(dwAndroidID))) return false;

	//��Ϣ����
	try
	{
		if (pAndroidUserItem->OnSocketRead(wMainCmdID,wSubCmdID,pData,wDataSize)==false)
		{
			ASSERT(FALSE);
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
bool CAndroidUserManager::SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	TCP_Command Command;
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
	ASSERT((wIndex-INDEX_ANDROID)<m_AndroidUserItemArray.GetCount());
	if (((wIndex-INDEX_ANDROID)>=m_AndroidUserItemArray.GetCount())) return NULL;

	//��ȡ����
	WORD wBufferIndex=wIndex-INDEX_ANDROID;
	CAndroidUserItem * pAndroidUserItem=m_AndroidUserItemArray[wBufferIndex];

	return pAndroidUserItem;
}

//�ͷŶ���
VOID CAndroidUserManager::FreeAndroidUserItem(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0);
	if (dwUserID==0) return;

	//��������
	CAndroidUserItem * pAndroidUserItem=NULL;
	m_AndroidUserItemMap.Lookup(dwUserID,pAndroidUserItem);

	//�����ж�
	if (pAndroidUserItem==NULL)
	{
		ASSERT(FALSE);
		return;
	}

	//��λ����
	pAndroidUserItem->RepositUserItem();

	//��������
	m_AndroidUserItemMap.RemoveKey(dwUserID);
	m_AndroidUserItemBuffer.Add(pAndroidUserItem);

	return;
}

//�������
CAndroidUserItem * CAndroidUserManager::ActiveAndroidUserItem(tagAndroidParameter & AndroidParameter)
{
	//��������
	CAndroidUserItem * pAndroidUserItem=NULL;
	INT_PTR nFreeItemCount=m_AndroidUserItemBuffer.GetCount();

	//��ȡ����
	if (nFreeItemCount>0)
	{
		//��ȡ����
		INT_PTR nItemPostion=nFreeItemCount-1;
		pAndroidUserItem=m_AndroidUserItemBuffer[nItemPostion];

		//ɾ������
		m_AndroidUserItemBuffer.RemoveAt(nItemPostion);
	}

	//��������
	if (pAndroidUserItem==NULL)
	{
		//��Ŀ�ж�
		if (m_AndroidUserItemArray.GetCount()>=MAX_ANDROID)
		{
			ASSERT(FALSE);
			return NULL;
		}

		try
		{
			//��������
			IAndroidUserItemSink * pIAndroidUserItemSink=(IAndroidUserItemSink *)
				m_pIGameServiceManager->CreateAndroidUserItemSink(IID_IAndroidUserItemSink,VER_IAndroidUserItemSink);

			//��������
			try
			{
				pAndroidUserItem=new CAndroidUserItem;
			}
			catch (...)
			{
				//�������
				ASSERT(FALSE);

				//ɾ������
				SafeRelease(pIAndroidUserItemSink);
			}

			//�����ж�
			if (pAndroidUserItem==NULL)
			{
				//�������
				ASSERT(FALSE);

				//ɾ������
				SafeRelease(pIAndroidUserItemSink);

				return NULL;
			}

			//�����û�
			if ((pIAndroidUserItemSink!=NULL)&&(pIAndroidUserItemSink->Initialization(pAndroidUserItem)==false))
			{
				//�������
				ASSERT(FALSE);

				//ɾ������
				SafeRelease(pAndroidUserItem);
				SafeRelease(pIAndroidUserItemSink);

				return NULL;
			}

			//��������
			WORD wCurrentIndex=(WORD)m_AndroidUserItemArray.Add(pAndroidUserItem);

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

	//����ʱ��
	if ((AndroidParameter.dwMaxReposeTime!=0L)&&(AndroidParameter.dwMinReposeTime!=0L))
	{
		//��������
		DWORD dwMaxReposeTime=AndroidParameter.dwMaxReposeTime;
		DWORD dwMinReposeTime=AndroidParameter.dwMinReposeTime;

		//����ʱ��
		if ((dwMaxReposeTime-dwMinReposeTime)>0L)
		{
			pAndroidUserItem->m_dwReposeTime=dwMinReposeTime+rand()%(dwMaxReposeTime-dwMinReposeTime);
		}
		else
		{
			pAndroidUserItem->m_dwReposeTime=dwMinReposeTime;
		}
	}
	else
	{
		pAndroidUserItem->m_dwReposeTime=TIME_ANDROID_REPOSE_TIME;
	}

	//���ñ���
	pAndroidUserItem->m_AndroidParameter=AndroidParameter;

	//��������
	m_AndroidUserItemMap[AndroidParameter.dwUserID]=pAndroidUserItem;

	return pAndroidUserItem;
}

//////////////////////////////////////////////////////////////////////////////////
