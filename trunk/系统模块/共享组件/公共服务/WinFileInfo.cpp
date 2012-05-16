#include "Stdafx.h"
#include "WinFileInfo.h"

//�����˵��
#pragma comment(lib,"Version")

//////////////////////////////////////////////////////////////////////////

//���캯��
CWinFileInfo::CWinFileInfo() 
{
	m_dwDataSize=0;
	m_dwLangCharset=0;
	m_pFixedFileInfo=NULL;
	memset(m_cbInfoBuffer,0,sizeof(m_cbInfoBuffer));
}

//��������
CWinFileInfo::~CWinFileInfo() 
{ 
} 

//���ļ�
bool CWinFileInfo::OpenWinFile(LPCTSTR lpszFileName)
{
	//���ñ���
	m_dwDataSize=0;
	m_dwLangCharset=0;
	m_pFixedFileInfo=NULL;
	memset(m_cbInfoBuffer,0,sizeof(m_cbInfoBuffer));

	//���ļ�
    DWORD dwFileHandle=NULL;
	DWORD dwBufferSize=::GetFileVersionInfoSize((LPTSTR)lpszFileName,&dwFileHandle);
    if ((dwBufferSize==0)||(dwBufferSize>sizeof(m_cbInfoBuffer))) return false;

	//�ļ���Ϣ
	m_dwDataSize=dwBufferSize;
    if (::GetFileVersionInfo(lpszFileName,dwFileHandle,sizeof(m_cbInfoBuffer),m_cbInfoBuffer)==FALSE) return false;

	//������Ϣ
	UINT uQuerySize=0;
    DWORD * pTransTable=NULL;
	BOOL bSuccess=::VerQueryValue(m_cbInfoBuffer,TEXT("\\VarFileInfo\\Translation"),(LPVOID *)&pTransTable,&uQuerySize);
    if (bSuccess) m_dwLangCharset=MAKELONG(HIWORD(pTransTable[0]),LOWORD(pTransTable[0]));

	//�汾��Ϣ
    if (::VerQueryValue(m_cbInfoBuffer,TEXT("\\"),(void * *)&m_pFixedFileInfo,&uQuerySize)==FALSE) return false;
	if ((uQuerySize!=sizeof(VS_FIXEDFILEINFO))||(m_pFixedFileInfo==NULL)) return false;

    return true;
}

//��ѯ��Ϣ
LPCTSTR CWinFileInfo::QueryValue(LPCTSTR lpszValueName)
{
    UINT nQuerySize=0;
    LPTSTR pszValue=NULL;
	TCHAR szBlockName[128]=TEXT("");
	_snprintf(szBlockName,sizeof(szBlockName),TEXT("\\StringFileInfo\\%08lx\\%s"),m_dwLangCharset,lpszValueName);
    ::VerQueryValue(m_cbInfoBuffer,szBlockName,(LPVOID *)&pszValue,&nQuerySize);
	return pszValue;
}

//��ȡ�汾
bool CWinFileInfo::GetFileVersion(DWORD & dwFileVersionMS, DWORD & dwFileVersionLS)
{
	//Ч�����
	ASSERT(m_pFixedFileInfo!=NULL);
	if (m_pFixedFileInfo==NULL) 
	{
		dwFileVersionMS=0L;
		dwFileVersionLS=0L;
		return false;
	}

	//���ñ���
	dwFileVersionMS=m_pFixedFileInfo->dwFileVersionMS;
	dwFileVersionLS=m_pFixedFileInfo->dwFileVersionLS;

	return true;
}

//��ȡ�汾
bool CWinFileInfo::GetProductVersion(DWORD & dwProductVersionMS, DWORD & dwProductVersionLS)
{
	//Ч�����
	ASSERT(m_pFixedFileInfo!=NULL);
	if (m_pFixedFileInfo==NULL)
	{
		dwProductVersionMS=0L;
		dwProductVersionLS=0L;
		return false;
	}

	//���ñ���
	dwProductVersionMS=m_pFixedFileInfo->dwProductVersionMS;
	dwProductVersionLS=m_pFixedFileInfo->dwProductVersionLS;

    return true;
}

//////////////////////////////////////////////////////////////////////////
