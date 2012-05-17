#include "StdAfx.h"
#include "Mmsystem.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CGlobalUnits * CGlobalUnits::m_pGlobalUnits=NULL;						//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalUnits::CGlobalUnits()
{
	//���ñ���
	m_pDlgDownLoad=NULL;

	//�汾��Ϣ
	m_dwPlazaVersion=VERSION_PLAZA;
	m_dwFrameVersion=VERSION_FRAME;

	//���ö���
	ASSERT(m_pGlobalUnits==NULL);
	if (m_pGlobalUnits==NULL) m_pGlobalUnits=this;

	return;
}

//��������
CGlobalUnits::~CGlobalUnits()
{
	//�ͷŶ���
	ASSERT(m_pGlobalUnits==this);
	if (m_pGlobalUnits==this) m_pGlobalUnits=NULL;

	return;
}

//��¼���
bool CGlobalUnits::WriteUserCookie()
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��¼���
	if (pParameterGlobal->m_bWriteCookie==true)
	{
		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//�������
		TCHAR szUserID[32]=TEXT("");
		_sntprintf(szUserID,CountArray(szUserID),TEXT("%ld"),pGlobalUserData->dwUserID);

		//�û�����
		CInternetSession::SetCookie(szCookieUrl,TEXT("UserID"),szUserID);
		CInternetSession::SetCookie(szCookieUrl,TEXT("Accounts"),pGlobalUserData->szAccounts);
		CInternetSession::SetCookie(szCookieUrl,TEXT("Password"),pGlobalUserData->szPassword);
	}

	return true;
}

//ɾ�����
bool CGlobalUnits::DeleteUserCookie()
{
	//ɾ����Ϣ
	CInternetSession::SetCookie(szCookieUrl,TEXT("UserID"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("Accounts"),TEXT(""));
	CInternetSession::SetCookie(szCookieUrl,TEXT("Password"),TEXT(""));

	return true;
}

//��������
bool CGlobalUnits::PlayGameSound(LPCTSTR pszSoundName)
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�����ж�
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//��������
	PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);

	return true;
}

//��������
bool CGlobalUnits::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�����ж�
	if (pParameterGlobal->m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//������Դ
	HRSRC hResource=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
	if (hResource==NULL) return false;

	//������Դ
	HGLOBAL hGlobal=LoadResource(hInstance,hResource);
	if (hGlobal==NULL) return false;

	//��������
	LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
	PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

	//������Դ
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return true;
}

//������Ϸ
bool CGlobalUnits::DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID)
{
	//��������
	if (m_pDlgDownLoad==NULL)
	{
		m_pDlgDownLoad=new CDlgDownLoad;
	}

	//��������
	if (m_pDlgDownLoad->m_hWnd==NULL)
	{
		m_pDlgDownLoad->Create(IDD_DLG_DOWNLOAD,CPlatformFrame::GetInstance());
	}

	//������Ϸ
	m_pDlgDownLoad->DownLoadClient(pszClientName,wKindID,wServerID);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
