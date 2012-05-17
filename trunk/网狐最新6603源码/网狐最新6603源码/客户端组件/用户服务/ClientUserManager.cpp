#include "StdAfx.h"
#include "ClientUserManager.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CClientUserItemArray CPlazaUserManager::m_UserItemStorage;				//�洢����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CClientUserItem::CClientUserItem()
{
	//���ñ���
	m_cbCompanion=CP_NORMAL;
	ZeroMemory(&m_UserInfo,sizeof(m_UserInfo));
	ZeroMemory(m_szUserNote,sizeof(m_szUserNote));
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));

	return;
}

//��������
CClientUserItem::~CClientUserItem()
{
}

//�ӿڲ�ѯ
VOID * CClientUserItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientUserItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IClientUserItem,Guid,dwQueryVer);
	return NULL;
}

//��Ҫ�ȼ�
LONG CClientUserItem::GetImportOrder()
{
	//����ȼ�
	INT nOrder=0;
	if (m_cbCompanion==CP_FRIEND) nOrder+=1000;
	if (m_UserInfo.cbMemberOrder!=0) nOrder+=m_UserInfo.cbMemberOrder*100L;
	if (m_UserInfo.cbMasterOrder!=0) nOrder+=m_UserInfo.cbMasterOrder*10000L;

	return nOrder;
}

//�û�ʤ��
float CClientUserItem::GetUserWinRate()
{
	LONG lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwWinCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//�û�����
float CClientUserItem::GetUserLostRate()
{
	LONG lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwLostCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//�û�����
float CClientUserItem::GetUserDrawRate()
{
	LONG lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwDrawCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//�û�����
float CClientUserItem::GetUserFleeRate()
{
	LONG lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.dwFleeCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//���ñ�ע
VOID CClientUserItem::SetUserNoteInfo(LPCTSTR pszUserNote)
{
	//Ч�����
	ASSERT(pszUserNote!=NULL);
	if (pszUserNote==NULL) return;

	//���ñ�ע
	lstrcpyn(m_szUserNote,pszUserNote,CountArray(m_szUserNote));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CPlazaUserManager::CPlazaUserManager()
{
	//����ӿ�
	m_pIUserManagerSink=NULL;
	m_pIUserInformation=NULL;

	return;
}

//��������
CPlazaUserManager::~CPlazaUserManager()
{
	//�洢�û�
	m_UserItemStorage.Append(m_UserItemActive);

	//�����û�
	m_UserItemActive.RemoveAll();

	return;
}

//�ӿڲ�ѯ
VOID * CPlazaUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IPlazaUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IPlazaUserManager,Guid,dwQueryVer);
	return NULL;
}

//���ýӿ�
bool CPlazaUserManager::SetUserInformation(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserInformation)!=NULL);
		m_pIUserInformation=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserInformation);

		//�ɹ��ж�
		if (m_pIUserInformation==NULL) return false;
	}
	else m_pIUserInformation=NULL;

	return true;
}

//���ýӿ�
bool CPlazaUserManager::SetUserManagerSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserManagerSink)!=NULL);
		m_pIUserManagerSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserManagerSink);

		//�ɹ��ж�
		if (m_pIUserManagerSink==NULL) return false;
	}
	else m_pIUserManagerSink=NULL;

	return true;
}

//�����û�
IClientUserItem * CPlazaUserManager::ActiveUserItem(tagUserInfo & UserInfo, tagCustomFaceInfo & CustomFaceInfo)
{
	//��������
	CClientUserItem * pClientUserItem=NULL;
	INT_PTR nStorageCount=m_UserItemStorage.GetCount();

	//��ȡ�û�
	if (nStorageCount>0)
	{
		pClientUserItem=m_UserItemStorage[nStorageCount-1];
		m_UserItemStorage.RemoveAt(nStorageCount-1);
	}
	else
	{
		pClientUserItem=new CClientUserItem;
		if (pClientUserItem==NULL) return NULL;
	}

	//�����û�
	ASSERT(pClientUserItem!=NULL);
	m_UserItemActive.Add(pClientUserItem);

	//��Ϣ����
	ASSERT(m_pIUserInformation!=NULL);
	tagCompanionInfo * pCompanionInfo=m_pIUserInformation->SearchCompanionInfo(UserInfo.dwUserID);

	//��ϵ����
	if (pCompanionInfo!=NULL)
	{
		pClientUserItem->m_cbCompanion=pCompanionInfo->cbCompanion;
		lstrcpyn(pClientUserItem->m_szUserNote,pCompanionInfo->szUserNote,CountArray(pClientUserItem->m_szUserNote));
	}
	else 
	{
		pClientUserItem->m_szUserNote[0]=0;
		pClientUserItem->m_cbCompanion=CP_NORMAL; 
	}

	//��������
	CopyMemory(&pClientUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	CopyMemory(&pClientUserItem->m_CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//����֪ͨ
	ASSERT(m_pIUserManagerSink!=NULL);
	if (m_pIUserManagerSink!=NULL) m_pIUserManagerSink->OnUserItemAcitve(pClientUserItem);

	//��Ϣ����
	if (pCompanionInfo!=NULL)
	{
		//һ���ж�
		bool bSameData=(UserInfo.dwGameID==pCompanionInfo->dwGameID);
		if ((bSameData==true)&&(lstrcmp(UserInfo.szNickName,pCompanionInfo->szNickName)!=0)) bSameData=false;

		//���´���
		if (bSameData==false) m_pIUserInformation->InsertCompanionInfo(pClientUserItem,pCompanionInfo->cbCompanion);
	}

	return pClientUserItem;
}

//ɾ���û�
bool CPlazaUserManager::DeleteUserItem(IClientUserItem * pIClientUserItem)
{
	//�����û�
	CClientUserItem * pUserItemActive=NULL;
	for (INT_PTR i=0;i<m_UserItemActive.GetCount();i++)
	{
		pUserItemActive=m_UserItemActive[i];
		if (pIClientUserItem==pUserItemActive)
		{
			//ɾ���û�
			m_UserItemActive.RemoveAt(i);
			m_UserItemStorage.Add(pUserItemActive);

			//ɾ��֪ͨ
			ASSERT(m_pIUserManagerSink!=NULL);
			m_pIUserManagerSink->OnUserItemDelete(pUserItemActive);

			//��������
			pUserItemActive->m_cbCompanion=CP_NORMAL;
			ZeroMemory(&pUserItemActive->m_UserInfo,sizeof(tagUserInfo));

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//���»���
bool CPlazaUserManager::UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore)
{
	//Ч�����
	ASSERT(pUserScore!=NULL);
	ASSERT(pIClientUserItem!=NULL);

	//��ȡ�û�
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//��������
	tagUserScore UserScore;
	UserScore.lScore=pUserInfo->lScore;
	UserScore.lGrade=pUserInfo->lGrade;
	UserScore.lInsure=pUserInfo->lInsure;
	UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	UserScore.dwUserMedal=pUserInfo->dwUserMedal;
	UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.dwLoveLiness=pUserInfo->dwLoveLiness;

	//��������
	pUserInfo->lScore=pUserScore->lScore;
	pUserInfo->lGrade=pUserScore->lGrade;
	pUserInfo->lInsure=pUserScore->lInsure;
	pUserInfo->dwWinCount=pUserScore->dwWinCount;
	pUserInfo->dwLostCount=pUserScore->dwLostCount;
	pUserInfo->dwDrawCount=pUserScore->dwDrawCount;
	pUserInfo->dwFleeCount=pUserScore->dwFleeCount;
	pUserInfo->dwUserMedal=pUserScore->dwUserMedal;
	pUserInfo->dwExperience=pUserScore->dwExperience;
	pUserInfo->dwLoveLiness=pUserScore->dwLoveLiness;

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserScore);

	return true;
}

//����״̬
bool CPlazaUserManager::UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus)
{
	//Ч�����
	ASSERT(pUserStatus!=NULL);
	ASSERT(pIClientUserItem!=NULL);

	//��ȡ�û�
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//��������
	tagUserStatus UserStatus;
	UserStatus.wTableID=pUserInfo->wTableID;
	UserStatus.wChairID=pUserInfo->wChairID;
	UserStatus.cbUserStatus=pUserInfo->cbUserStatus;

	//��������
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserStatus);

	return true;
}

//����ͷ��
bool CPlazaUserManager::UpdateUserCustomFace(IClientUserItem * pIClientUserItem, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//��ȡ�û�
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//���ñ���
	pUserInfo->dwCustomID=dwCustomID;
	CopyMemory(pCustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserFaceUpdate(pIClientUserItem);

	return true;
}

//ö���û�
IClientUserItem * CPlazaUserManager::EnumUserItem(WORD wEnumIndex)
{
	if (wEnumIndex>=m_UserItemActive.GetCount()) return NULL;
	return m_UserItemActive[wEnumIndex];
}

//�����û�
IClientUserItem * CPlazaUserManager::SearchUserByUserID(DWORD dwUserID)
{
	//�û�����
	for (INT_PTR i=0;i<m_UserItemActive.GetCount();i++)
	{
		CClientUserItem * pClientUserItem=m_UserItemActive[i];
		if (pClientUserItem->m_UserInfo.dwUserID==dwUserID) return pClientUserItem;
	}

	return NULL;
}

//�����û�
IClientUserItem * CPlazaUserManager::SearchUserByGameID(DWORD dwGameID)
{
	//�û�����
	for (INT_PTR i=0;i<m_UserItemActive.GetCount();i++)
	{
		CClientUserItem * pClientUserItem=m_UserItemActive[i];
		if (pClientUserItem->m_UserInfo.dwGameID==dwGameID) return pClientUserItem;
	}

	return NULL;
}

//�����û�
IClientUserItem * CPlazaUserManager::SearchUserByNickName(LPCTSTR pszNickName)
{
	//�û�����
	for (INT_PTR i=0;i<m_UserItemActive.GetCount();i++)
	{
		CClientUserItem * pClientUserItem=m_UserItemActive[i];
		LPCTSTR pszTempNickName=pClientUserItem->GetNickName();
		if (lstrcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameUserManager::CGameUserManager()
{
	//����ӿ�
	m_pIUserManagerSink=NULL;

	//�û�����
	ZeroMemory(m_pTableUserItem,sizeof(m_pTableUserItem));

	return;
}

//��������
CGameUserManager::~CGameUserManager()
{
	//�Թ��û�
	m_UserItemStorage.Append(m_UserItemLookon);

	//��Ϸ�û�
	for (WORD i=0;i<CountArray(m_pTableUserItem);i++)
	{
		if (m_pTableUserItem[i]!=NULL) m_UserItemStorage.Add(m_pTableUserItem[i]);
	}

	//�����û�
	m_UserItemLookon.RemoveAll();

	return;
}

//�ӿڲ�ѯ
VOID * CGameUserManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameUserManager,Guid,dwQueryVer);
	return NULL;
}

//���ýӿ�
bool CGameUserManager::SetUserManagerSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserManagerSink)!=NULL);
		m_pIUserManagerSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserManagerSink);

		//�ɹ��ж�
		if (m_pIUserManagerSink==NULL) return false;
	}
	else m_pIUserManagerSink=NULL;

	return true;
}

//�ӿڲ�ѯ
VOID * CGameUserManager::GetUserManagerSink(REFGUID Guid, DWORD dwQueryVer)
{
	if (m_pIUserManagerSink==NULL) return NULL;
	return m_pIUserManagerSink->QueryInterface(Guid,dwQueryVer);
}

//�����û�
bool CGameUserManager::ResetUserItem()
{
	//�洢�û�
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		if (m_pTableUserItem[i]!=NULL) m_UserItemStorage.Add(m_pTableUserItem[i]);
	}
	m_UserItemStorage.Append(m_UserItemLookon);

	//���ñ���
	m_UserItemLookon.RemoveAll();
	ZeroMemory(m_pTableUserItem,sizeof(m_pTableUserItem));

	return true;
}

//ɾ���û�
bool CGameUserManager::DeleteUserItem(IClientUserItem * pIClientUserItem)
{
	//Ч��״̬
	ASSERT((pIClientUserItem!=NULL)&&(pIClientUserItem->GetChairID()<MAX_CHAIR));
	if ((pIClientUserItem==NULL)||(pIClientUserItem->GetChairID()>=MAX_CHAIR)) return false;

	//��Ϸ�û�
	if (pIClientUserItem->GetUserStatus()!=US_LOOKON)
	{
		//��������
		WORD wChairID=pIClientUserItem->GetChairID();
		CClientUserItem * pUserItemRemove=(CClientUserItem *)pIClientUserItem;

		//Ч���û�
		ASSERT(m_pTableUserItem[pIClientUserItem->GetChairID()]==pIClientUserItem);
		if (m_pTableUserItem[pIClientUserItem->GetChairID()]!=pIClientUserItem) return false;

		//ɾ���û�
		m_pTableUserItem[wChairID]=NULL;
		m_UserItemStorage.Add(pUserItemRemove);

		//ɾ��֪ͨ
		ASSERT(m_pIUserManagerSink!=NULL);
		m_pIUserManagerSink->OnUserItemDelete(pUserItemRemove);

		//��������
		pUserItemRemove->m_cbCompanion=CP_NORMAL;
		ZeroMemory(&pUserItemRemove->m_UserInfo,sizeof(tagUserInfo));

		return true;
	}

	//�Թ��û�
	CClientUserItem * pUserItemActive=NULL;
	for (INT_PTR i=0;i<m_UserItemLookon.GetCount();i++)
	{
		pUserItemActive=m_UserItemLookon[i];
		if (pIClientUserItem==pUserItemActive)
		{
			//ɾ���û�
			m_UserItemLookon.RemoveAt(i);
			m_UserItemStorage.Add(pUserItemActive);

			//ɾ��֪ͨ
			ASSERT(m_pIUserManagerSink!=NULL);
			m_pIUserManagerSink->OnUserItemDelete(pUserItemActive);

			//��������
			pUserItemActive->m_cbCompanion=CP_NORMAL;
			ZeroMemory(&pUserItemActive->m_UserInfo,sizeof(tagUserInfo));

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�����û�
IClientUserItem * CGameUserManager::ActiveUserItem(tagUserInfo & UserInfo, tagCustomFaceInfo & CustomFaceInfo, BYTE cbCompanion, LPCTSTR pszUserNote)
{
	//Ч��״̬
	ASSERT(UserInfo.wChairID<MAX_CHAIR);
	if (UserInfo.wChairID>=MAX_CHAIR) return NULL;

	//��������
	CClientUserItem * pClientUserItem=NULL;
	INT_PTR nStorageCount=m_UserItemStorage.GetCount();

	//��ȡ�û�
	if (nStorageCount>0)
	{
		pClientUserItem=m_UserItemStorage[nStorageCount-1];
		ASSERT(pClientUserItem!=NULL);
		m_UserItemStorage.RemoveAt(nStorageCount-1);
	}
	else
	{
		pClientUserItem=new CClientUserItem;
		ASSERT(pClientUserItem!=NULL);
		if (pClientUserItem==NULL) return false;
	}

	//�����û�
	if (UserInfo.cbUserStatus==US_LOOKON) m_UserItemLookon.Add(pClientUserItem);
	else m_pTableUserItem[UserInfo.wChairID]=pClientUserItem;

	//�û���Ϣ
	CopyMemory(&pClientUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	CopyMemory(&pClientUserItem->m_CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//������Ϣ
	pClientUserItem->m_cbCompanion=cbCompanion;
	lstrcpyn(pClientUserItem->m_szUserNote,pszUserNote,CountArray(pClientUserItem->m_szUserNote));

	//����֪ͨ
	ASSERT(m_pIUserManagerSink!=NULL);
	if (m_pIUserManagerSink!=NULL) m_pIUserManagerSink->OnUserItemAcitve(pClientUserItem);

	return pClientUserItem;
}

//���»���
bool CGameUserManager::UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore)
{
	//Ч�����
	ASSERT(pUserScore!=NULL);
	ASSERT(pIClientUserItem!=NULL);

	//��ȡ�û�
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//��������
	tagUserScore UserScore;
	UserScore.lScore=pUserInfo->lScore;
	UserScore.lGrade=pUserInfo->lGrade;
	UserScore.lInsure=pUserInfo->lInsure;
	UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	UserScore.dwUserMedal=pUserInfo->dwUserMedal;
	UserScore.dwExperience=pUserInfo->dwExperience;
	UserScore.dwLoveLiness=pUserInfo->dwLoveLiness;

	//��������
	pUserInfo->lScore=pUserScore->lScore;
	pUserInfo->lGrade=pUserScore->lGrade;
	pUserInfo->lInsure=pUserScore->lInsure;
	pUserInfo->dwWinCount=pUserScore->dwWinCount;
	pUserInfo->dwLostCount=pUserScore->dwLostCount;
	pUserInfo->dwDrawCount=pUserScore->dwDrawCount;
	pUserInfo->dwFleeCount=pUserScore->dwFleeCount;
	pUserInfo->dwUserMedal=pUserScore->dwUserMedal;
	pUserInfo->dwExperience=pUserScore->dwExperience;
	pUserInfo->dwLoveLiness=pUserScore->dwLoveLiness;

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserScore);

	return true;
}

//����״̬
bool CGameUserManager::UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus)
{
	//Ч�����
	ASSERT(pUserStatus!=NULL);
	ASSERT(pIClientUserItem!=NULL);

	//��ȡ�û�
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//��������
	tagUserStatus UserStatus;
	UserStatus.wTableID=pUserInfo->wTableID;
	UserStatus.wChairID=pUserInfo->wChairID;
	UserStatus.cbUserStatus=pUserInfo->cbUserStatus;

	//��������
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserStatus);

	return true;
}

//����ͷ��
bool CGameUserManager::UpdateUserCustomFace(IClientUserItem * pIClientUserItem, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//��ȡ�û�
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//���ñ���
	pUserInfo->dwCustomID=dwCustomID;
	CopyMemory(&pCustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserFaceUpdate(pIClientUserItem);

	return true;
}

//��Ϸ�û�
IClientUserItem * CGameUserManager::GetTableUserItem(WORD wChariID)
{
	//Ч�����
	ASSERT(wChariID<CountArray(m_pTableUserItem));
	if (wChariID>=CountArray(m_pTableUserItem)) return NULL;

	return m_pTableUserItem[wChariID];
}

//�Թ��û�
IClientUserItem * CGameUserManager::EnumLookonUserItem(WORD wEnumIndex)
{
	if (wEnumIndex>=m_UserItemLookon.GetCount()) return NULL;
	return m_UserItemLookon[wEnumIndex];
}

//�����û�
IClientUserItem * CGameUserManager::SearchUserByUserID(DWORD dwUserID)
{
	//��Ϸ�û�
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		CClientUserItem * pClientUserItem=m_pTableUserItem[i];
		if ((pClientUserItem!=NULL)&&(pClientUserItem->GetUserID()==dwUserID)) return pClientUserItem;
	}

	//�Թ��û�
	for (INT_PTR i=0;i<m_UserItemLookon.GetCount();i++)
	{
		CClientUserItem * pClientUserItem=m_UserItemLookon[i];
		if (pClientUserItem->GetUserID()==dwUserID) return pClientUserItem;
	}

	return NULL;
}

//�����û�
IClientUserItem * CGameUserManager::SearchUserByGameID(DWORD dwGameID)
{
	//��Ϸ�û�
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		CClientUserItem * pClientUserItem=m_pTableUserItem[i];
		if ((pClientUserItem!=NULL)&&(pClientUserItem->GetGameID()==dwGameID)) return pClientUserItem;
	}

	//�Թ��û�
	for (INT_PTR i=0;i<m_UserItemLookon.GetCount();i++)
	{
		CClientUserItem * pClientUserItem=m_UserItemLookon[i];
		if (pClientUserItem->GetGameID()==dwGameID) return pClientUserItem;
	}

	return NULL;
}

//�����û�
IClientUserItem * CGameUserManager::SearchUserByNickName(LPCTSTR pszNickName)
{
	//��Ϸ�û�
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		//��ȡ�û�
		CClientUserItem * pClientUserItem=m_pTableUserItem[i];

		//�û��ж�
		if (pClientUserItem!=NULL)
		{
			LPCTSTR pszTempNickName=pClientUserItem->GetNickName();
			if (lstrcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
		}
	}

	//�Թ��û�
	for (INT_PTR i=0;i<m_UserItemLookon.GetCount();i++)
	{
		//��ȡ�û�
		CClientUserItem * pClientUserItem=m_UserItemLookon[i];

		//�û��ж�
		if (pClientUserItem!=NULL)
		{
			LPCTSTR pszTempNickName=pClientUserItem->GetNickName();
			if (lstrcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(ClientUserItem);
DECLARE_CREATE_MODULE(GameUserManager);
DECLARE_CREATE_MODULE(PlazaUserManager);

//////////////////////////////////////////////////////////////////////////////////
