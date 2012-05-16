#include "StdAfx.h"
#include "Direct.h"
#include "DownLoadService.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ʱ����ʶ
#define IDI_UPDATE_TIMER					10							//���¶�ʱ��

//��ȡ���� HTML
const TCHAR szErrorHtml1[]=TEXT("<html>");
const TCHAR szErrorHtml2[]=TEXT("<!DOCTYPE HTML PUBLIC");

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDownLoadMission, CSkinDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RETRY, OnBnClickedReTry)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoadThread::CDownLoadThread()
{
	//���ñ���
	m_bPreparative=false;
	m_szTempFile[0]=0;
	m_szLocalFile[0]=0;
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;
	m_pHttpFile=NULL;
	m_pHttpConnection=NULL;
	m_pDownLoadRequest=NULL;
	m_DownLoadResult=enDownLoadResult_Noknow;
	m_DownLoadStatus=enDownLoadStatus_Unknow;

	return;
}

//��������
CDownLoadThread::~CDownLoadThread()
{
	DownLoadCleanUp();
	return;
}

//��ʼ���߳�
bool CDownLoadThread::InitThread(tagDownLoadRequest * pDownLoadRequest)
{
	ASSERT(pDownLoadRequest!=NULL);
	m_pDownLoadRequest=pDownLoadRequest;
	return true;
}

//����״̬
void CDownLoadThread::GetDownLoadStatus(tagDownLoadStatus & DownLoadStatus)
{
	//������Դ
	CThreadLock ThreadLockHandle(m_CriticalSection);

	//���ñ���
	DownLoadStatus.DownLoadStatus=m_DownLoadStatus;
	switch (m_DownLoadStatus)
	{
	case enDownLoadStatus_Unknow:
	case enDownLoadStatus_Ready:
		{
			DownLoadStatus.wProgress=0;
			lstrcpyn(DownLoadStatus.szStatus,TEXT("���ڻ�ȡ�ļ�..."),CountArray(DownLoadStatus.szStatus));
			break;
		}
	case enDownLoadStatus_DownLoadIng:
		{
			DownLoadStatus.wProgress=(WORD)(m_dwDownLoadSize*100L/m_dwTotalFileSize);
			_snprintf(DownLoadStatus.szStatus,sizeof(DownLoadStatus.szStatus),
				TEXT("�Ѹ��ƣ�%ld KB ���� %ld KB��"),m_dwDownLoadSize/1000L,m_dwTotalFileSize/1000L);
			break;
		}
	case enDownLoadStatus_Finish:
		{
			DownLoadStatus.wProgress=100;
			lstrcpyn(DownLoadStatus.szStatus,TEXT("���سɹ�"),CountArray(DownLoadStatus.szStatus));
			break;
		}
	case enDownLoadStatus_Fails:
		{
			DownLoadStatus.wProgress=0;
			_snprintf(DownLoadStatus.szStatus,sizeof(DownLoadStatus.szStatus),TEXT("����ʧ�ܣ�����ţ�%ld"),m_DownLoadResult);
			break;
		}
	default: 
		{
			ASSERT(FALSE);
			memset(&DownLoadStatus,0,sizeof(DownLoadStatus));
			break;
		}
	}

	return;
}

//Ŀ���ļ�
LPCTSTR CDownLoadThread::GetDownLoadFileName()
{
	ASSERT(m_DownLoadStatus==enDownLoadStatus_Finish);
	return m_szLocalFile;
}

//���к���
bool CDownLoadThread::OnEventThreadRun()
{
	try
	{
		if (m_bPreparative==true)
		{
			//��ȡ�ļ�
			TCHAR szBuffer[4096];
			UINT uReadCount=m_pHttpFile->Read(szBuffer,sizeof(szBuffer));
			if (uReadCount>0)
			{
				//д���ļ�
				m_LocalFile.Write(szBuffer,uReadCount);

				//������Դ
				CThreadLock ThreadLockHandle(m_CriticalSection);

				//���ñ���
				m_dwDownLoadSize+=uReadCount;
				m_DownLoadStatus=enDownLoadStatus_DownLoadIng;

				return true;
			}
			else 
			{
				//�ر��ļ�
				m_LocalFile.Close();

				//�ƶ��ļ�
				_snprintf(m_szLocalFile,sizeof(m_szLocalFile),TEXT("%s\\%s"),m_pDownLoadRequest->szLocalPath,m_pDownLoadRequest->szFileName);
				DeleteFile(m_szLocalFile);
				if (MoveFileWithProgress(m_szTempFile,m_szLocalFile,NULL, NULL, MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
					throw enDownLoadResult_CreateFileFails;

				//������Դ
				CThreadLock ThreadLockHandle(m_CriticalSection);

				//���ñ���
				m_DownLoadStatus=enDownLoadStatus_Finish;

				//��������
				DownLoadCleanUp();

				return false;
			}
		}
		else 
		{
			//������Դ
			CThreadLock ThreadLockHandle(m_CriticalSection);

			//���ñ���
			m_DownLoadStatus=enDownLoadStatus_Ready;

			//����׼��
			DownLoadPreparative();

			return true;
		}
	}
	catch (enDownLoadResult DownLoadResult)
	{
		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=DownLoadResult;
	}
	catch (CInternetException * pInternetException)
	{
		//�����쳣
		pInternetException->Delete();

		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=enDownLoadResult_Exception;
	}
	catch (...) 
	{
		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=enDownLoadResult_Exception;
	}

	//������
	DownLoadCleanUp();

	return false;
}

//�ر��¼�
bool CDownLoadThread::OnEventThreadConclude()
{
	DownLoadCleanUp();
	return true;
}

//����׼��
void CDownLoadThread::DownLoadPreparative()
{
	//Ч��״̬
	ASSERT(m_pHttpFile==NULL);
	ASSERT(m_bPreparative==false);

	//���ñ���
	m_szTempFile[0]=0;
	m_szLocalFile[0]=0;
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;

	//����Ŀ¼
	int nExcursion=0;
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	lstrcpyn(szDirectory,m_pDownLoadRequest->szLocalPath,CountArray(szDirectory));
	do
	{
		if (szDirectory[nExcursion]==0) 
		{
			::CreateDirectory(szDirectory,NULL);
			break;
		}
		if (szDirectory[nExcursion]==TEXT('\\'))
		{
			szDirectory[nExcursion]=0;
			::CreateDirectory(szDirectory,NULL);
			szDirectory[nExcursion]=TEXT('\\');
		}
		nExcursion++;
	} while (true);

	//�ж���������
	int nStepCount=0;
	DWORD dwStatusCode =0;
TCHAR szDownBuffer[4096];
	try
	{
		//��ȡ����
		UINT nReadCount=0;
		
		m_pHttpFile=(CHttpFile *)m_InternetSession.OpenURL(m_pDownLoadRequest->szDownLoadUrl,1,INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_DONT_CACHE);
		nStepCount++;
		m_pHttpFile->QueryInfoStatusCode(dwStatusCode);
		nReadCount=m_pHttpFile->Read(szDownBuffer,sizeof(szDownBuffer));
		nStepCount++;
		if (nReadCount<sizeof(szErrorHtml1)) throw 0;
		nStepCount++;
		if (nReadCount<sizeof(szErrorHtml2)) throw 0;
		nStepCount++;
		if (memcmp(szErrorHtml1,szDownBuffer,lstrlen(szErrorHtml1))==0) throw 0;
		nStepCount++;
		if (memcmp(szErrorHtml2,szDownBuffer,lstrlen(szErrorHtml2))==0)
		{
			
			throw 0;
		}
		nStepCount++;

		//��ȡ��С
		ASSERT(m_pHttpFile!=NULL);
		BOOL bSuccess=m_pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,m_dwTotalFileSize);
		nStepCount++;
		if (bSuccess==FALSE)
		{
			//CString strError;
			//DWORD dwError=GetLastError();
			//strError.Format(TEXT("����ţ�%ld"),dwError);
			//AfxMessageBox(strError);
			throw enDownLoadResult_InternetReadError;
		}

		nStepCount++;

		//�����ļ�
		_snprintf(m_szTempFile,sizeof(m_szTempFile),TEXT("%s\\%s.DTM"),m_pDownLoadRequest->szLocalPath,m_pDownLoadRequest->szFileName);
		if (m_LocalFile.Open(m_szTempFile,CFile::modeReadWrite|CFile::modeCreate)==FALSE) throw enDownLoadResult_CreateFileFails;
		nStepCount++;

		//д���ļ�
		m_LocalFile.Write(szDownBuffer,nReadCount);
		nStepCount++;

		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);
		nStepCount++;

		//���ñ���
		m_dwDownLoadSize+=nReadCount;
		m_DownLoadStatus=enDownLoadStatus_DownLoadIng;

		//���ñ���
		m_bPreparative=true;

		return;
	}
	catch (...)
	{ 
		//CString strError;
		//DWORD dwError=GetLastError();
		//strError.Format(TEXT("ִ�в��裺%ld,״̬�룺%ld"),nStepCount,dwStatusCode);
		//AfxMessageBox(szDownBuffer);
		//SafeDelete(m_pHttpFile);
		throw enDownLoadResult_InternetReadError;
	}

	return;
}

//�������
void CDownLoadThread::DownLoadCleanUp()
{
	//���ñ���
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;
	m_bPreparative=false;

	//������Դ
	if (m_pHttpFile!=NULL) 
	{
		m_pHttpFile->Close();
		SafeDelete(m_pHttpFile);
	}
	if (m_pHttpConnection!=NULL)
	{
		m_pHttpConnection->Close();
		SafeDelete(m_pHttpConnection);
	}
	if (m_LocalFile!=INVALID_HANDLE_VALUE) m_LocalFile.Close();

	//ɾ���ļ�
	if (m_szTempFile[0]!=0) DeleteFile(m_szTempFile);

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoadMission::CDownLoadMission(IDownLoadMissionSink * pIDownLoadMissionSink) : CSkinDialogEx(IDD_DOWN_LOAD)
{
	//���ñ���
	m_dwMissionID=0;
	m_dwDownLoadType=0;
	m_pIDownLoadMissionSink=pIDownLoadMissionSink;
	m_DownLoadThread.InitThread(&m_DownLoadRequest);
	memset(&m_DownLoadRequest,0,sizeof(m_DownLoadRequest));

	return;
}

//��������
CDownLoadMission::~CDownLoadMission()
{
}

//�ؼ���
void CDownLoadMission::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RETRY, m_btReTry);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
	DDX_Control(pDX, IDC_DOWN_LOAD_URL, m_DownLoadUrl);
}

//��ʼ������
BOOL CDownLoadMission::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	SetDlgItemText(IDC_DESCRIPTION,m_DownLoadRequest.szDescribe);

	//���ö�ʱ��
	SetTimer(IDI_UPDATE_TIMER,300,NULL);

	return TRUE;
}

//��Ϣ����
BOOL CDownLoadMission::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//ȡ����Ϣ
void CDownLoadMission::OnCancel()
{
	//�رս���
	DestroyWindow();

	//ֹͣ����
	m_DownLoadThread.ConcludeThread(INFINITE);

	//�¼�֪ͨ
	m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Fails,this);

	return;
}

//���԰�ť
void CDownLoadMission::OnBnClickedReTry()
{
	//���ý���
	m_btReTry.EnableWindow(FALSE);

	//��������
	m_DownLoadThread.StartThread();

	//����ʱ��
	SetTimer(IDI_UPDATE_TIMER,300,NULL);

	return;
}

//��ʱ����Ϣ
void CDownLoadMission::OnTimer(UINT_PTR nIDEvent)
{
	__super::OnTimer(nIDEvent);

	//����״̬
	if (nIDEvent==IDI_UPDATE_TIMER)
	{
		//���ÿؼ�
		tagDownLoadStatus DownLoadStatus;
		m_DownLoadThread.GetDownLoadStatus(DownLoadStatus);
		m_ProgressCtrl.SetPos(DownLoadStatus.wProgress);
		SetDlgItemText(IDC_STATUS,DownLoadStatus.szStatus);

		//״̬�л�
		switch (DownLoadStatus.DownLoadStatus)
		{
		case enDownLoadStatus_Fails:
			{
				//���ý���
				KillTimer(IDI_UPDATE_TIMER);
				m_btReTry.EnableWindow(TRUE);
				
				//�¼�֪ͨ
				if (m_DownLoadRequest.bDisplay==false)
				{
					DestroyWindow();
					m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Fails,this);
				}

				break;
			}
		case enDownLoadStatus_Finish:
			{
				DestroyWindow();
				LPCTSTR pszLocalFile=m_DownLoadThread.GetDownLoadFileName();
				ShellExecute(NULL,TEXT("open"),pszLocalFile,NULL,NULL,SW_SHOWDEFAULT);

				//�¼�֪ͨ
				m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Finish,this);

				break;
			}
		}

		return;
	}

	return;
}

//��ʾ����
bool CDownLoadMission::DisplayDownLoadFace(bool bDisplay)
{
	//���ñ���
	m_DownLoadRequest.bDisplay=bDisplay;

	//��ʾ����
	if (m_hWnd!=NULL) ShowWindow(bDisplay?SW_SHOW:SW_HIDE);

	return true;
}

//�Ա�����
bool CDownLoadMission::CompareRequest(tagDownLoadRequest * pDownLoadRequest)
{
	if (lstrcmp(m_DownLoadRequest.szDownLoadUrl,pDownLoadRequest->szDownLoadUrl)!=0) return false;
	return true;
}

//��ʼ����
bool CDownLoadMission::StartDownLoad(DWORD dwMissionID, DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest)
{
	m_DownLoadUrl.SetHyperLinkUrl(pDownLoadRequest->szDownLoadUrl);

	//���ñ���
	m_dwMissionID=dwMissionID;
	m_dwDownLoadType=dwDownLoadType;
	CopyMemory(&m_DownLoadRequest,pDownLoadRequest,sizeof(m_DownLoadRequest));
	if (m_DownLoadRequest.szLocalPath[0]==0) GetCurrentDirectory(sizeof(m_DownLoadRequest.szLocalPath),m_DownLoadRequest.szLocalPath);

	//��������
	m_DownLoadThread.StartThread();

	//��������
	AfxSetResourceHandle(GetModuleHandle(IDOWN_LOAD_DLL_NAME));
	Create(IDD_DOWN_LOAD,AfxGetMainWnd());
	AfxSetResourceHandle(GetModuleHandle(NULL));
	if (m_DownLoadRequest.bDisplay==true) ShowWindow(SW_SHOW);

	return true;
}

//ֹͣ����
bool CDownLoadMission::StopDownLoad()
{
	//�رս���
	DestroyWindow();

	//ֹͣ�߳�
	m_DownLoadThread.ConcludeThread(INFINITE);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoadService::CDownLoadService()
{
	m_dwMissionID=0;
	m_pIDownLoadServiceSink=NULL;
}

//��������
CDownLoadService::~CDownLoadService()
{
	//��������
	CDownLoadMission * pDownLoadMission=NULL;
	for (INT_PTR i=0;i<m_DownLoadMissionActive.GetCount();i++)
	{
		pDownLoadMission=m_DownLoadMissionActive[i];
		pDownLoadMission->StopDownLoad();
		SafeDelete(pDownLoadMission);
	}
	for (INT_PTR i=0;i<m_DownLoadMissionRelease.GetCount();i++)
	{
		pDownLoadMission=m_DownLoadMissionRelease[i];
		SafeDelete(pDownLoadMission);
	}
	m_DownLoadMissionActive.RemoveAll();
	m_DownLoadMissionRelease.RemoveAll();
	
	return;
}

//�ӿڲ�ѯ
void * __cdecl CDownLoadService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDownLoadService,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDownLoadService,Guid,dwQueryVer);
	return NULL;
}

//��ȡ��Ŀ
INT_PTR __cdecl CDownLoadService::GetDownLoadMissionCount()
{
	return m_DownLoadMissionActive.GetCount();
}

//���ýӿ�
bool __cdecl CDownLoadService::SetDownLoadServiceSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIDownLoadServiceSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadServiceSink);
	ASSERT(m_pIDownLoadServiceSink!=NULL);
	return (m_pIDownLoadServiceSink!=NULL);
}

//��������
DWORD __cdecl CDownLoadService::AddDownLoadRequest(DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest)
{
	//Ч�����
	ASSERT(pDownLoadRequest!=NULL);
	if (pDownLoadRequest==NULL) return 0;

	//Ѱ������
	CDownLoadMission * pDownLoadMission=SearchMission(pDownLoadRequest);
	if (pDownLoadMission!=NULL)
	{
		if (pDownLoadRequest->bDisplay==true) pDownLoadMission->DisplayDownLoadFace(pDownLoadRequest->bDisplay);
		return pDownLoadMission->GetMissionID();
	}

	//��������
	INT_PTR nReleaseCount=m_DownLoadMissionRelease.GetCount();
	if (nReleaseCount>0)
	{
		pDownLoadMission=m_DownLoadMissionRelease[nReleaseCount-1];
		m_DownLoadMissionRelease.RemoveAt(nReleaseCount-1);
	}
	else
	{
		try { pDownLoadMission=new CDownLoadMission(this); }
		catch (...) { return 0; }
	}
	ASSERT(pDownLoadMission!=NULL);
	if (pDownLoadMission==NULL) return 0;
	m_DownLoadMissionActive.Add(pDownLoadMission);

	//��������
	m_dwMissionID++;
	pDownLoadMission->StartDownLoad(m_dwMissionID,dwDownLoadType,pDownLoadRequest);

	return pDownLoadMission->GetMissionID();
}

//����֪ͨ
void CDownLoadService::OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadMission * pDownLoadMission)
{
	//Ѱ������
	INT_PTR i=0;
	for (;i<m_DownLoadMissionActive.GetCount();i++)
	{
		if (pDownLoadMission==m_DownLoadMissionActive[i]) break;
	}
	ASSERT(i!=m_DownLoadMissionActive.GetCount());
	if (i==m_DownLoadMissionActive.GetCount()) return;

	//��������
	m_DownLoadMissionActive.RemoveAt(i);
	m_DownLoadMissionRelease.Add(pDownLoadMission);

	return;
}

//��������
CDownLoadMission * CDownLoadService::SearchMission(tagDownLoadRequest * pDownLoadRequest)
{
	CDownLoadMission * pDownLoadMission=NULL;
	for (INT_PTR i=0;i<m_DownLoadMissionActive.GetCount();i++)
	{
		pDownLoadMission=m_DownLoadMissionActive[i];
		if (pDownLoadMission->CompareRequest(pDownLoadRequest)==true) return pDownLoadMission;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateDownLoadService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDownLoadService * pDownLoadService=NULL;
	try
	{
		pDownLoadService=new CDownLoadService();
		if (pDownLoadService==NULL) throw TEXT("����ʧ��");
		void * pObject=pDownLoadService->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pDownLoadService);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
