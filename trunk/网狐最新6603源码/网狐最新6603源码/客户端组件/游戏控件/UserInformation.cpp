#include "StdAfx.h"
#include "UserInformation.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CUserInformation *	CUserInformation::m_pUserInformation=NULL;			//��ϵ�ӿ�

//////////////////////////////////////////////////////////////////////////////////

//�ļ�����
#define STREAM_COMPANION			L"CompanionInfo"					//��ϵ����
#define INFORMATION_FILE			L"UserInformation.DAT"				//�ļ�����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUserInformation::CUserInformation()
{
	//���ñ���
	m_pIUserCompanionSink=NULL;

	//���ñ���
	ASSERT(m_pUserInformation==NULL);
	if (m_pUserInformation==NULL) m_pUserInformation=this;

	return;
}

//��������
CUserInformation::~CUserInformation()
{
	//���ñ���
	ASSERT(m_pUserInformation==this);
	if (m_pUserInformation==this) m_pUserInformation=NULL;

	//ɾ����ϵ
	for (INT_PTR i=0;i<m_CompanionInfoActive.GetCount();i++)
	{
		tagCompanionInfo * pCompanionInfo=m_CompanionInfoActive[i];
		SafeDelete(pCompanionInfo);
	}
	for (INT_PTR i=0;i<m_CompanionInfoBuffer.GetCount();i++)
	{
		tagCompanionInfo * pCompanionInfo=m_CompanionInfoBuffer[i];
		SafeDelete(pCompanionInfo);
	}

	//ɾ������
	m_CompanionInfoActive.RemoveAll();
	m_CompanionInfoBuffer.RemoveAll();

	return;
}

//�ӿڲ�ѯ
VOID * CUserInformation::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserInformation,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IUserInformation,Guid,dwQueryVer);
	return NULL;
}

//������Ϣ
bool CUserInformation::LoadInformation()
{
	//��������
	IStream * pIStreamSub=NULL;
	IStorage * pIStorageSub=NULL;
	IStorage * pIStorageRoot=NULL;
	IEnumSTATSTG * pIEnumSTATSTG=NULL;

	//��ȡ����
	try
	{
		//��������
		STATSTG Statstg;
		tagCompanionInfo CompanionInfo;
		ZeroMemory(&Statstg,sizeof(Statstg));
		ZeroMemory(&CompanionInfo,sizeof(CompanionInfo));

		//����Ŀ¼
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//���ļ�
		WCHAR szInfomationFile[MAX_PATH]=L"";
		_snwprintf(szInfomationFile,CountArray(szInfomationFile),L"%s\\%s",(LPCWSTR)(CT2CW(szDirectory)),INFORMATION_FILE);

		//���ļ�
		StgOpenStorage(szInfomationFile,NULL,STGM_READ|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageRoot);
		if (pIStorageRoot==NULL) return false;

		//����ö��
		pIStorageRoot->EnumElements(0,NULL,0,&pIEnumSTATSTG);
		if (pIEnumSTATSTG==NULL) throw TEXT("EnumElements Create pIEnumSTATSTG Error");

		//ö�ٶ���
		while (pIEnumSTATSTG->Next(1,&Statstg,NULL)==NOERROR)
		{
			//Ŀ¼����
			if (Statstg.type==STGTY_STORAGE)
			{
				//�򿪴洢
				pIStorageRoot->OpenStorage(Statstg.pwcsName,NULL,STGM_READ|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageSub);
				if (pIStorageSub==NULL) throw TEXT("CreateStorage Create pIStorageSub Error");

				//������
				pIStorageSub->OpenStream(STREAM_COMPANION,NULL,STGM_READ|STGM_SHARE_EXCLUSIVE,0,&pIStreamSub);

				//��ȡ����
				if (pIStreamSub!=NULL)
				{
					//��ȡ��ϵ
					ULONG cbReadCount=0L;
					pIStreamSub->Read(&CompanionInfo,sizeof(CompanionInfo),&cbReadCount);

					//��ȡЧ��
					ASSERT(cbReadCount==sizeof(CompanionInfo));
					if (cbReadCount!=sizeof(CompanionInfo)) throw TEXT("Read Data CompanionInfo Error");

					//��������
					tagCompanionInfo * pCompanionInfo=CreateCompanionItem();
					if (pCompanionInfo==NULL) throw TEXT("CreateCompanionItem Error");

					//������Ϣ
					CopyMemory(pCompanionInfo,&CompanionInfo,sizeof(CompanionInfo));
				}

				//�ͷŽӿ�
				SafeRelease(pIStreamSub);
				SafeRelease(pIStorageSub);
			}

			//�ͷ��ڴ�
			CoTaskMemFree(Statstg.pwcsName);
		}

		//�ͷŽӿ�
		SafeRelease(pIEnumSTATSTG);
		SafeRelease(pIStreamSub);
		SafeRelease(pIStorageSub);
		SafeRelease(pIStorageRoot);

		return true;
	}
	catch (...)
	{
		//�������
		ASSERT(FALSE);

		//�ͷŽӿ�
		SafeRelease(pIEnumSTATSTG);
		SafeRelease(pIStreamSub);
		SafeRelease(pIStorageSub);
		SafeRelease(pIStorageRoot);
	}

	return false;
}

//���ýӿ�
bool CUserInformation::SetUserCompanionSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserCompanionSink)!=NULL);
		m_pIUserCompanionSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserCompanionSink);

		//�ɹ��ж�
		if (m_pIUserCompanionSink==NULL) return false;
	}
	else m_pIUserCompanionSink=NULL;

	return true;
}

//ö�ٹ�ϵ
tagCompanionInfo * CUserInformation::EmunCompanionInfo(WORD wIndex)
{
	//ö������
	if (wIndex<m_CompanionInfoActive.GetCount())
	{
		tagCompanionInfo * pCompanionInfo=m_CompanionInfoActive[wIndex];
		return pCompanionInfo;
	}
	
	return NULL;
}

//Ѱ�ҹ�ϵ
tagCompanionInfo * CUserInformation::SearchCompanionInfo(DWORD dwUserID)
{
	//������ϵ
	for (INT_PTR i=0;i<m_CompanionInfoActive.GetCount();i++)
	{
		tagCompanionInfo * pCompanionInfo=m_CompanionInfoActive[i];
		if (pCompanionInfo->dwUserID==dwUserID) return pCompanionInfo;
	}

	return NULL;
}

//���ù�ϵ
tagCompanionInfo * CUserInformation::InsertCompanionInfo(IClientUserItem * pIClientUserItem, BYTE cbCompanion)
{
	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return NULL;

	//�û�����
	DWORD dwUserID=pIClientUserItem->GetUserID();
	tagCompanionInfo * pCompanionInfo=SearchCompanionInfo(dwUserID);

	//������Ϣ
	if (pCompanionInfo==NULL)
	{
		//��������
		pCompanionInfo=CreateCompanionItem();
		if (pCompanionInfo==NULL) return NULL;

		//������Ϣ
		pCompanionInfo->cbCompanion=cbCompanion;
		pCompanionInfo->dwGameID=pIClientUserItem->GetGameID();
		pCompanionInfo->dwUserID=pIClientUserItem->GetUserID();
		lstrcpyn(pCompanionInfo->szNickName,pIClientUserItem->GetNickName(),CountArray(pCompanionInfo->szNickName));
		lstrcpyn(pCompanionInfo->szUserNote,pIClientUserItem->GetUserNoteInfo(),CountArray(pCompanionInfo->szUserNote));

		//����֪ͨ
		if (m_pIUserCompanionSink!=NULL) m_pIUserCompanionSink->OnCompanionInsert(pCompanionInfo);
	}
	else
	{
		//�޸��ж�
		bool bModify=false;
		if (pCompanionInfo->cbCompanion!=cbCompanion) bModify=true;
		if ((bModify==false)&&(lstrcmp(pCompanionInfo->szNickName,pIClientUserItem->GetNickName())!=0)) bModify=true;
		if ((bModify==false)&&(lstrcmp(pCompanionInfo->szUserNote,pIClientUserItem->GetUserNoteInfo())!=0)) bModify=true;

		//�޸��ж�
		if (bModify=false) return pCompanionInfo;

		//������Ϣ
		pCompanionInfo->cbCompanion=cbCompanion;
		pCompanionInfo->dwGameID=pIClientUserItem->GetGameID();
		pCompanionInfo->dwUserID=pIClientUserItem->GetUserID();
		lstrcpyn(pCompanionInfo->szNickName,pIClientUserItem->GetNickName(),CountArray(pCompanionInfo->szNickName));
		lstrcpyn(pCompanionInfo->szUserNote,pIClientUserItem->GetUserNoteInfo(),CountArray(pCompanionInfo->szUserNote));

		//����֪ͨ
		if (m_pIUserCompanionSink!=NULL) m_pIUserCompanionSink->OnCompanionUpdate(pCompanionInfo);
	}

	//��������
	IStream * pIStreamSub=NULL;
	IStorage * pIStorageSub=NULL;
	IStorage * pIStorageRoot=NULL;

	//д������
	try
	{
		//��������
		WCHAR szStorageName[16]=L"";
		_snwprintf(szStorageName,CountArray(szStorageName),L"%ld",dwUserID);

		//����Ŀ¼
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

		//���ļ�
		WCHAR szInfomationFile[MAX_PATH]=L"";
		_snwprintf(szInfomationFile,CountArray(szInfomationFile),L"%s\\%s",(LPCWSTR)(CT2CW(szDirectory)),INFORMATION_FILE);

		//���ļ�
		StgOpenStorage(szInfomationFile,NULL,STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageRoot);
		if (pIStorageRoot==NULL) StgCreateDocfile(szInfomationFile,STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pIStorageRoot);

		//�����ж�
		if (pIStorageRoot==NULL) throw TEXT("StgCreateDocfile Create pIStorageRoot Error");

		//�򿪴洢
		pIStorageRoot->CreateStorage(szStorageName,STGM_WRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageSub);
		if (pIStorageSub==NULL) pIStorageRoot->CreateStorage(szStorageName,STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStorageSub);

		//������
		if (pIStorageSub==NULL) throw TEXT("CreateStorage Create pIStorageSub Error");

		//��������
		pIStorageSub->CreateStream(STREAM_COMPANION,STGM_WRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStreamSub);
		if (pIStreamSub==NULL) pIStorageSub->CreateStream(STREAM_COMPANION,STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE,0,0,&pIStreamSub);

		//������
		if (pIStreamSub==NULL) throw TEXT("CreateStream Create pIStreamSub Error");

		//д������
		ULONG cbWriteCount=0L;
		pIStreamSub->Write(pCompanionInfo,sizeof(tagCompanionInfo),&cbWriteCount);

		//�ͷŽӿ�
		SafeRelease(pIStreamSub);
		SafeRelease(pIStorageSub);
		SafeRelease(pIStorageRoot);

		return pCompanionInfo;
	}
	catch (...)
	{
		//�������
		ASSERT(FALSE);

		//�ͷŽӿ�
		SafeRelease(pIStreamSub);
		SafeRelease(pIStorageSub);
		SafeRelease(pIStorageRoot);
	}

	return NULL;
}

//������ϵ
tagCompanionInfo * CUserInformation::CreateCompanionItem()
{
	//��������
	tagCompanionInfo * pCompanionInfo=NULL;

	//��ȡ����
	if (m_CompanionInfoBuffer.GetCount()>0)
	{
		INT_PTR nItemCount=m_CompanionInfoBuffer.GetCount();
		pCompanionInfo=m_CompanionInfoBuffer[nItemCount-1];
		m_CompanionInfoBuffer.RemoveAt(nItemCount-1);
	}

	//��������
	if (pCompanionInfo==NULL)
	{
		pCompanionInfo=new tagCompanionInfo;
		if (pCompanionInfo==NULL) return NULL;
	}

	//��ԭ����
	ZeroMemory(pCompanionInfo,sizeof(tagCompanionInfo));

	//��������
	m_CompanionInfoActive.Add(pCompanionInfo);

	return pCompanionInfo;
}

//�ͷŹ�ϵ
bool CUserInformation::DeleteCompanionItem(tagCompanionInfo * pCompanionInfo)
{
	//��������
	for (INT_PTR i=0;i<m_CompanionInfoActive.GetCount();i++)
	{
		if (m_CompanionInfoActive[i]==pCompanionInfo)
		{
			//ɾ������
			m_CompanionInfoActive.RemoveAt(i);
			m_CompanionInfoBuffer.Add(pCompanionInfo);

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(UserInformation);

//////////////////////////////////////////////////////////////////////////////////
