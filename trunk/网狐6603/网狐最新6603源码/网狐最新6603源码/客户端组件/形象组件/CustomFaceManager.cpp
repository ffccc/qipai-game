#include "StdAfx.h"
#include "CustomFaceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//�ļ��ṹ
struct tagCustomFaceFile
{
	DWORD							dwCustomID;							//�Զ���ʶ
	tagCustomFaceInfo				CustomFaceInfo;						//ͷ����Ϣ
};

//��̬����
CCustomFaceManager * CCustomFaceManager::m_pCustomFaceManager=NULL;		//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCustomFaceManager::CCustomFaceManager()
{
	//���ñ���
	m_pICustomFaceEvent=NULL;
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));

	//���ö���
	ASSERT(m_pCustomFaceManager==NULL);
	if (m_pCustomFaceManager==NULL) m_pCustomFaceManager=this;

	return;
}

//��������
CCustomFaceManager::~CCustomFaceManager()
{
	//�ͷŶ���
	ASSERT(m_pCustomFaceManager==this);
	if (m_pCustomFaceManager==this) m_pCustomFaceManager=NULL;

	return;
}

//�ӿڲ�ѯ
VOID * CCustomFaceManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadSink,Guid,dwQueryVer);
	QUERYINTERFACE(ICustomFaceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceManager,Guid,dwQueryVer);
	return NULL;
}

//���ýӿ�
bool CCustomFaceManager::SetCustomFaceEvent(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent)!=NULL);
		m_pICustomFaceEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent);

		//�ɹ��ж�
		if (m_pICustomFaceEvent==NULL) return false;
	}
	else m_pICustomFaceEvent=NULL;

	return true;
}

//����ͷ��
bool CCustomFaceManager::LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID)
{
	//��������
	for (INT_PTR i=0;i<m_CustomFaceIndexArray.GetCount();i++)
	{
		//��ȡ����
		tagCustomFaceIndex * pCustomFaceIndex=&m_CustomFaceIndexArray[i];

		//�����ж�
		if (pCustomFaceIndex->dwUserID==dwUserID)
		{
			//�汾һ��
			if (pCustomFaceIndex->dwCustomID==dwCustomID)
			{
				return true;
			}

			//�汾����
			if (pCustomFaceIndex->dwCustomID!=dwCustomID)
			{
				if (i==0)
				{
					//��������
					tagCustomFaceIndex CustomFaceIndex;
					ZeroMemory(&CustomFaceIndex,sizeof(CustomFaceIndex));

					//���ñ���
					CustomFaceIndex.dwUserID=dwUserID;
					CustomFaceIndex.dwCustomID=dwCustomID;

					//��������
					m_CustomFaceIndexArray.Add(CustomFaceIndex);
				}
				else
				{
					//��д����
					pCustomFaceIndex->dwCustomID=dwCustomID;
				}

				return true;
			}
		}
	}

	//��������
	tagCustomFaceIndex CustomFaceIndex;
	ZeroMemory(&CustomFaceIndex,sizeof(CustomFaceIndex));

	//���ñ���
	CustomFaceIndex.dwUserID=dwUserID;
	CustomFaceIndex.dwCustomID=dwCustomID;

	//��������
	m_CustomFaceIndexArray.Add(CustomFaceIndex);

	//��������
	if (m_CustomFaceIndexArray.GetCount()==1L)
	{
		PerformDownLoad(dwUserID,dwCustomID);
	}

	return true;
}

//����ͷ��
bool CCustomFaceManager::LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//Ŀ��Ŀ¼
	TCHAR szCustomFacePath[MAX_PATH]=TEXT("");
	_sntprintf(szCustomFacePath,CountArray(szCustomFacePath),TEXT("%s\\CustomFace\\%ld.DAT"),szWorkDirectory,dwUserID);

	//���ļ�
	CFile DataFile;
	if (DataFile.Open(szCustomFacePath,CFile::modeRead)==FALSE) return false;

	//Ч�鳤��
	ASSERT(DataFile.GetLength()==sizeof(tagCustomFaceFile));
	if (DataFile.GetLength()!=sizeof(tagCustomFaceFile)) return false;

	//��ȡ����
	tagCustomFaceFile CustomFaceFile;
	UINT uReadCount=DataFile.Read(&CustomFaceFile,sizeof(CustomFaceFile));

	//��ȡЧ��
	if (uReadCount!=sizeof(CustomFaceFile)) return false;
	if (CustomFaceFile.dwCustomID!=dwCustomID) return false;
	if (CustomFaceFile.CustomFaceInfo.dwDataSize!=sizeof(CustomFaceFile.CustomFaceInfo)) return false;

	//��������
	CopyMemory(&CustomFaceInfo,&CustomFaceFile.CustomFaceInfo,sizeof(CustomFaceInfo));

	return true;
}

//��������
bool CCustomFaceManager::SaveUserCustomFace(DWORD dwUserID, DWORD dwCustomID, DWORD dwCustomFace[FACE_CX*FACE_CY])
{
	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//Ŀ��Ŀ¼
	TCHAR szCustomDirectory[MAX_PATH]=TEXT("");
	_sntprintf(szCustomDirectory,CountArray(szCustomDirectory),TEXT("%s\\CustomFace"),szWorkDirectory);

	//Ŀ���ļ�
	TCHAR szCustomFacePath[MAX_PATH]=TEXT("");
	_sntprintf(szCustomFacePath,CountArray(szCustomFacePath),TEXT("%s\\CustomFace\\%ld.DAT"),szWorkDirectory,dwUserID);

	//����Ŀ¼
	CreateDirectory(szCustomDirectory,NULL);

	//���ļ�
	CFile DataFile;
	if (DataFile.Open(szCustomFacePath,CFile::modeWrite|CFile::modeCreate)==TRUE)
	{
		//��������
		tagCustomFaceFile CustomFaceFile;
		ZeroMemory(&CustomFaceFile,sizeof(CustomFaceFile));

		//��д����
		CustomFaceFile.dwCustomID=dwCustomID;
		CustomFaceFile.CustomFaceInfo.dwDataSize=sizeof(tagCustomFaceInfo);
		CopyMemory(CustomFaceFile.CustomFaceInfo.dwCustomFace,dwCustomFace,sizeof(CustomFaceFile.CustomFaceInfo.dwCustomFace));

		//д���ļ�
		DataFile.Write(&CustomFaceFile,sizeof(CustomFaceFile));
		DataFile.SetLength(sizeof(CustomFaceFile));

		//�ر��ļ�
		DataFile.Close();
	}

	return true;
}

//�����쳣
bool CCustomFaceManager::OnDownLoadError(enDownLoadError DownLoadError)
{
	//״̬Ч��
	ASSERT(m_CustomFaceIndexArray.GetCount()>0L);
	if (m_CustomFaceIndexArray.GetCount()==0L) return false;

	//ɾ������
	m_CustomFaceIndexArray.RemoveAt(0);

	//��������
	if (m_CustomFaceIndexArray.GetCount()>=1L)
	{
		tagCustomFaceIndex * pCustomFaceIndex=&m_CustomFaceIndexArray[0];
		PerformDownLoad(pCustomFaceIndex->dwUserID,pCustomFaceIndex->dwCustomID);
	}

	return true;
}

//����״̬
bool CCustomFaceManager::OnDownLoadStatus(enDownLoadStatus DownLoadStatus)
{
	//�������
	if (DownLoadStatus==DownLoadStatus_Conclude)
	{
		//״̬Ч��
		ASSERT(m_CustomFaceIndexArray.GetCount()>0L);
		if (m_CustomFaceIndexArray.GetCount()==0L) return false;

		//�����ļ�
		tagCustomFaceIndex CustomFaceIndex=m_CustomFaceIndexArray[0];
		SaveUserCustomFace(CustomFaceIndex.dwUserID,CustomFaceIndex.dwCustomID,m_CustomFaceInfo.dwCustomFace);

		//ɾ������
		m_CustomFaceIndexArray.RemoveAt(0);

		//�¼�֪ͨ
		ASSERT(m_pICustomFaceEvent!=NULL);
		m_pICustomFaceEvent->OnEventCustomFace(CustomFaceIndex.dwUserID,CustomFaceIndex.dwCustomID,m_CustomFaceInfo);

		//��������
		if (m_CustomFaceIndexArray.GetCount()>=1L)
		{
			tagCustomFaceIndex * pCustomFaceIndex=&m_CustomFaceIndexArray[0];
			PerformDownLoad(pCustomFaceIndex->dwUserID,pCustomFaceIndex->dwCustomID);
		}
	}

	return true;
}

//��������
bool CCustomFaceManager::OnDataStream(const VOID * pcbMailData, WORD wStreamSize)
{
	//Ч������
	ASSERT((wStreamSize+m_CustomFaceInfo.dwDataSize)<=sizeof(m_CustomFaceInfo.dwCustomFace));
	if ((wStreamSize+m_CustomFaceInfo.dwDataSize)>sizeof(m_CustomFaceInfo.dwCustomFace)) return false;

	//��������
	DWORD dwSourceIndex=m_CustomFaceInfo.dwDataSize;
	m_CustomFaceInfo.dwDataSize=m_CustomFaceInfo.dwDataSize+wStreamSize;
	CopyMemory((BYTE *)(m_CustomFaceInfo.dwCustomFace)+dwSourceIndex,pcbMailData,wStreamSize);

	return true;
}

//������Ϣ
bool CCustomFaceManager::OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)
{
	return true;
}

//ִ������
bool CCustomFaceManager::PerformDownLoad(DWORD dwUserID, DWORD dwCustomID)
{
	//���ñ���
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));

	//�����ַ
	TCHAR szCustomFace[128]=TEXT("");
	_sntprintf(szCustomFace,CountArray(szCustomFace),TEXT("%s/CustomFace.asp?UserID=%ld&CustomID=%ld"),szPlatformLink,dwUserID,dwCustomID);

	//����ͷ��
	ASSERT(m_DownLoad.GetDownLoadStatus()==DownLoadStatus_Conclude);
	m_DownLoad.PerformDownLoad(szCustomFace,QUERY_ME_INTERFACE(IUnknownEx));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(CustomFaceManager);

//////////////////////////////////////////////////////////////////////////////////
