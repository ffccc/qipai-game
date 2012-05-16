#include "StdAfx.h"
#include "Resource.h"
#include "CompanionManager.h"

//////////////////////////////////////////////////////////////////////////

//�汾����
#define VER_COMPANION_MAIN			1									//���汾
#define VER_COMPANION_SUB			0									//�Ӱ汾

//�ļ�ͷ��Ϣ
struct tagCompanionFileHead
{
	WORD							wVersion;							//�汾��Ϣ
	WORD							wItemSize;							//�ṹ��С
	DWORD							dwFileSize;							//�ļ���С
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CCompanionManager::CCompanionManager(void)
{
	m_bNeedUpdate=false;
	m_DlgCompanionManager.SetCompanionManager(QUERY_ME_INTERFACE(IUnknownEx));
	return;
}

//��������
CCompanionManager::~CCompanionManager(void)
{
	//ɾ����ϵ
	tagCompanionItem * pCompanionItem=NULL;
	for (INT_PTR i=0;i<m_CompanionItemActive.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemActive[i];
		SafeDelete(pCompanionItem);
	}
	for (INT_PTR i=0;i<m_CompanionItemRelease.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemRelease[i];
		SafeDelete(pCompanionItem);
	}
	m_CompanionItemActive.RemoveAll();
	m_CompanionItemRelease.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CCompanionManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICompanionManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICompanionManager,Guid,dwQueryVer);
	return NULL;
}

//������Ϣ
bool __cdecl CCompanionManager::LoadCompanion()
{
	//���ļ�
	CFile CompanionFile;
	if (CompanionFile.Open(TEXT("Companion.DAT"),CFile::modeRead)==FALSE) return true;

	//�жϴ�С
	ULONGLONG uFileSize=CompanionFile.GetLength();
	if (uFileSize<sizeof(tagCompanionFileHead)) return true;

	//��ȡ�ļ�ͷ
	tagCompanionFileHead CompanionFileHead;
	memset(&CompanionFileHead,0,sizeof(CompanionFileHead));
	CompanionFile.Read(&CompanionFileHead,sizeof(CompanionFileHead));

	//Ч���ļ�ͷ
	if (HIBYTE(CompanionFileHead.wVersion)!=VER_COMPANION_MAIN) return false;
	ASSERT(CompanionFileHead.dwFileSize==uFileSize);
	ASSERT(((CompanionFileHead.dwFileSize-sizeof(CompanionFileHead))%CompanionFileHead.wItemSize)==0);
	if (CompanionFileHead.dwFileSize!=uFileSize) return false;
	if (((CompanionFileHead.dwFileSize-sizeof(CompanionFileHead))%CompanionFileHead.wItemSize)!=0) return false;

	//ɾ����ϵ
	tagCompanionItem * pCompanionItem=NULL;
	for (INT_PTR i=0;i<m_CompanionItemActive.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemActive[i];
		memset(pCompanionItem,0,sizeof(tagCompanionItem));
	}
	m_CompanionItemRelease.Append(m_CompanionItemActive);
	m_CompanionItemActive.RemoveAll();

	//��ȡ��Ϣ
	WORD wItemSize=__min(sizeof(tagCompanionItem),CompanionFileHead.wItemSize);
	DWORD dwItemCount=(CompanionFileHead.dwFileSize-sizeof(CompanionFileHead))/CompanionFileHead.wItemSize;
	for (DWORD i=0;i<dwItemCount;i++)
	{
		//������ϵ
		try { pCompanionItem=new tagCompanionItem; }
		catch (...) { return false; }
		if (pCompanionItem==NULL) return false;
		memset(pCompanionItem,0,sizeof(tagCompanionItem));

		//��ȡ��ϵ
		CompanionFile.Read(pCompanionItem,wItemSize);
		pCompanionItem->szNote[CountArray(pCompanionItem->szNote)-1]=0;
		pCompanionItem->szAccounts[CountArray(pCompanionItem->szAccounts)-1]=0;

		//�����ϵ
		if (SearchCompanionItem(pCompanionItem->dwUserID)!=NULL) m_CompanionItemRelease.Add(pCompanionItem);
		else m_CompanionItemActive.Add(pCompanionItem);
	}

	//���ñ���
	m_bNeedUpdate=false;

	return true;
}

//�����ϵ
bool __cdecl CCompanionManager::SaveCompanion()
{
	//�����ϵ
	if (m_bNeedUpdate==true)
	{
		//���ļ�
		CFile CompanionFile;
		if (CompanionFile.Open(TEXT("Companion.DAT"),CFile::modeCreate|CFile::modeWrite)==FALSE) return false;

		//�����ļ�ͷ
		tagCompanionFileHead CompanionFileHead;
		CompanionFileHead.wItemSize=sizeof(tagCompanionItem);
		CompanionFileHead.wVersion=MAKEWORD(VER_COMPANION_SUB,VER_COMPANION_MAIN);
		CompanionFileHead.dwFileSize=(DWORD)sizeof(CompanionFileHead)+sizeof(tagCompanionItem)*GetCompanionItemCount();

		//д���ļ�
		tagCompanionItem * pCompanionItem=NULL;
		CompanionFile.Write(&CompanionFileHead,sizeof(CompanionFileHead));
		for (INT_PTR i=0;i<m_CompanionItemActive.GetCount();i++)
		{
			pCompanionItem=m_CompanionItemActive[i];
			CompanionFile.Write(pCompanionItem,sizeof(tagCompanionItem));
		}

		//�ر��ļ�
		CompanionFile.SetLength(CompanionFileHead.dwFileSize);
		CompanionFile.Flush();
		CompanionFile.Close();

		//���ñ���
		m_bNeedUpdate=false;
	}

	return true;
}

//��ʾ����
bool __cdecl CCompanionManager::ShowCompanionManager()
{
	if (m_DlgCompanionManager.m_hWnd==NULL)
	{
		AfxSetResourceHandle(GetModuleHandle(COMPANION_DLL_NAME));
		m_DlgCompanionManager.Create(IDD_COMPANION_MANAGER,AfxGetMainWnd());
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	m_DlgCompanionManager.ShowWindow(SW_SHOW);

	return true;
}

//�����ϵ
const tagCompanionItem * __cdecl CCompanionManager::InsertCompanionItem(tagCompanionItem & CompanionItem)
{
	//Ѱ�ҹ�ϵ
	bool bChangeCompanion=false;
	tagCompanionItem * pCompanionItem=NULL;
	INT_PTR i=0;
	for (;i<m_CompanionItemActive.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemActive[i];
		ASSERT(pCompanionItem!=NULL);
		if (pCompanionItem->dwUserID==CompanionItem.dwUserID)
		{
			bChangeCompanion=(CompanionItem.Companion!=pCompanionItem->Companion);
			break;
		}
	}
	if (i==m_CompanionItemActive.GetCount()) pCompanionItem=NULL;

	//������ϵ
	bool bInsert=false;
	if (pCompanionItem==NULL)
	{
		bInsert=true;
		INT_PTR nReleaseCount=m_CompanionItemRelease.GetCount();
		if (nReleaseCount>0)
		{
			pCompanionItem=m_CompanionItemRelease[nReleaseCount-1];
			m_CompanionItemRelease.RemoveAt(nReleaseCount-1);
		}
		else
		{
			try { pCompanionItem=new tagCompanionItem; }
			catch (...) { return NULL; }
		}
		m_CompanionItemActive.Add(pCompanionItem);
	}

	//д����Ϣ
	m_bNeedUpdate=true;
	CompanionItem.szNote[CountArray(CompanionItem.szNote)-1]=0;
	CompanionItem.szAccounts[CountArray(CompanionItem.szAccounts)-1]=0;
	CopyMemory(pCompanionItem,&CompanionItem,sizeof(tagCompanionItem));

	//���½���
	if (bInsert==true) m_DlgCompanionManager.InsertCompanionItem(pCompanionItem);
	else m_DlgCompanionManager.UpdateCompanionItem(pCompanionItem,bChangeCompanion);

	return pCompanionItem;
}

//���¹�ϵ
const tagCompanionItem * __cdecl CCompanionManager::UpdateCompanionItem(tagCompanionItem & CompanionItem)
{
	tagCompanionItem * pCompanionItem=NULL;
	for (INT_PTR i=0;i<m_CompanionItemActive.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemActive[i];
		ASSERT(pCompanionItem!=NULL);
		if (pCompanionItem->dwUserID==CompanionItem.dwUserID)
		{
			//��������
			bool bChangeCompanion=(pCompanionItem->Companion!=CompanionItem.Companion);

			//��������
			m_bNeedUpdate=true;
			CompanionItem.szNote[CountArray(CompanionItem.szNote)-1]=0;
			CompanionItem.szAccounts[CountArray(CompanionItem.szAccounts)-1]=0;
			CopyMemory(pCompanionItem,&CompanionItem,sizeof(tagCompanionItem));

			//���½���
			m_DlgCompanionManager.UpdateCompanionItem(pCompanionItem,bChangeCompanion);

			return pCompanionItem;
		}
	}
	return NULL;
}

//���¹�ϵ
const tagCompanionItem * __cdecl CCompanionManager::UpdateCompanionItem(DWORD dwUserID, LPCTSTR pszNote)
{
	tagCompanionItem * pCompanionItem=NULL;
	for (INT_PTR i=0;i<m_CompanionItemActive.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemActive[i];
		ASSERT(pCompanionItem!=NULL);
		if (pCompanionItem->dwUserID==dwUserID)
		{
			//��������
			m_bNeedUpdate=true;
			lstrcpyn(pCompanionItem->szNote,pszNote,CountArray(pCompanionItem->szNote));

			//���½���
			m_DlgCompanionManager.UpdateCompanionItem(pCompanionItem,false);

			return pCompanionItem;
		}
	}
	return NULL;
}

//���¹�ϵ
const tagCompanionItem * __cdecl CCompanionManager::UpdateCompanionItem(DWORD dwUserID, LPCTSTR pszAccounts, WORD wFaceID)
{
	tagCompanionItem * pCompanionItem=NULL;
	for (INT_PTR i=0;i<m_CompanionItemActive.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemActive[i];
		ASSERT(pCompanionItem!=NULL);
		if (pCompanionItem->dwUserID==dwUserID)
		{
			//��������
			m_bNeedUpdate=true;
			pCompanionItem->wFaceID=wFaceID;
			lstrcpyn(pCompanionItem->szAccounts,pszAccounts,CountArray(pCompanionItem->szAccounts));

			//���½���
			m_DlgCompanionManager.UpdateCompanionItem(pCompanionItem,false);

			return pCompanionItem;
		}
	}
	return NULL;
}

//ɾ����ϵ
bool __cdecl CCompanionManager::DeleteCompanionItem(DWORD dwUserID)
{
	tagCompanionItem * pCompanionItem=NULL;
	for (INT_PTR i=0;i<m_CompanionItemActive.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemActive[i];
		ASSERT(pCompanionItem!=NULL);
		if (pCompanionItem->dwUserID==dwUserID)
		{
			//���½���
			m_DlgCompanionManager.DeleteCompanionItem(pCompanionItem);

			//��������
			m_bNeedUpdate=true;
			m_CompanionItemActive.RemoveAt(i);
			m_CompanionItemRelease.Add(pCompanionItem);
			memset(pCompanionItem,0,sizeof(tagCompanionItem));

			return true;
		}
	}
	return false;
}

//ö�ٹ�ϵ
const tagCompanionItem * __cdecl CCompanionManager::EmunCompanionItem(WORD wIndex)
{
	if (wIndex>=m_CompanionItemActive.GetCount()) return NULL;
	return m_CompanionItemActive[wIndex];
}

//Ѱ�ҹ�ϵ
const tagCompanionItem * __cdecl CCompanionManager::SearchCompanionItem(DWORD dwUserID)
{
	tagCompanionItem * pCompanionItem=NULL;
	for (INT_PTR i=0;i<m_CompanionItemActive.GetCount();i++)
	{
		pCompanionItem=m_CompanionItemActive[i];
		ASSERT(pCompanionItem!=NULL);
		if (pCompanionItem->dwUserID==dwUserID) return pCompanionItem;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateCompanionManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CCompanionManager * pCompanionManager=NULL;
	try
	{
		pCompanionManager=new CCompanionManager();
		if (pCompanionManager==NULL) throw TEXT("����ʧ��");
		void * pObject=pCompanionManager->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pCompanionManager);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
