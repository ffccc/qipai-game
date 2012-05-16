#include "StdAfx.h"
#include "AtlBase.h"
#include "InitParamter.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CInitParamter::CInitParamter() 
{ 
	InitParamter();
}

//��������
CInitParamter::~CInitParamter()
{
}

//��ʼ��
void CInitParamter::InitParamter()
{
	//���ñ���
	m_szMainPage[0]=0;
	m_wMaxConnect=512;
	m_szCenterServerAddr[0]=0;
	m_wListenPort=PORT_LOGON_SERVER;

	//��¼���ݿ�
	m_wUserDataBasePort=1433;
	m_szUserDataBaseAddr[0]=0;
	m_szUserDataBaseUser[0]=0;
	m_szUserDataBasePass[0]=0;
	lstrcpyn(m_szUserDataBaseName,szGameUserDB,CountArray(m_szUserDataBaseName));

	return;
}

//��������
void CInitParamter::LoadInitParamter()
{
	//��ʼ������
	InitParamter();

	//��ע���
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_snprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\LogonServer"),szSystemRegKey);
	if (RegParamter.Open(HKEY_CURRENT_USER,szRegName,KEY_READ)!=ERROR_SUCCESS) return;

	//��������
	TCHAR szReadData[1024]=TEXT("");
	DWORD dwDataType,dwReadData,dwDataSize;

	//��Ϸ��վ
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	LONG lErrorCode=RegParamter.QueryValue(TEXT("MainPage"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szMainPage,sizeof(m_szMainPage));

	//���ķ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("CenterServerAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szCenterServerAddr,sizeof(m_szCenterServerAddr));

	//�������
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("MaxConnect"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wMaxConnect=(WORD)dwReadData;

	//�����˿�
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ListenPort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wListenPort=(WORD)dwReadData;

	//���ݿ�˿�
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("UserDataBasePort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wUserDataBasePort=(WORD)dwReadData;

	//���ݿ��ַ
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("UserDataBaseAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szUserDataBaseAddr,sizeof(m_szUserDataBaseAddr));

	//���ݿ��û���
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("UserDataBaseUser"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szUserDataBaseUser,sizeof(m_szUserDataBaseUser));

	//���ݿ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("UserDataBasePass"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szUserDataBasePass,sizeof(m_szUserDataBasePass));

	//���ݿ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("UserDataBaseName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szUserDataBaseName,sizeof(m_szUserDataBaseName));

	return;
}

//��������
void CInitParamter::SaveInitParamter(bool bSaveAll)
{
	//��ע���
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_snprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\LogonServer"),szSystemRegKey);
	if (RegParamter.Create(HKEY_CURRENT_USER,szRegName)!=ERROR_SUCCESS) return;

	//��������
	TCHAR szWriteBuffer[1024]=TEXT("");

	//��¼���ݿ�
	RegParamter.SetDWORDValue(TEXT("UserDataBasePort"),m_wUserDataBasePort);
	CXOREncrypt::EncryptData(m_szUserDataBaseAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("UserDataBaseAddr"));
	CXOREncrypt::EncryptData(m_szUserDataBaseUser,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("UserDataBaseUser"));
	CXOREncrypt::EncryptData(m_szUserDataBasePass,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("UserDataBasePass"));

	//���ķ�����
	CXOREncrypt::EncryptData(m_szCenterServerAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("CenterServerAddr"));

	//��Ϸ��վ
	CXOREncrypt::EncryptData(m_szMainPage,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("MainPage"));

	if (bSaveAll==true)
	{
		//��������
		RegParamter.SetDWORDValue(TEXT("MaxConnect"),m_wMaxConnect);
		RegParamter.SetDWORDValue(TEXT("ListenPort"),m_wListenPort);

		//��¼���ݿ�
		CXOREncrypt::EncryptData(m_szUserDataBaseName,szWriteBuffer,sizeof(szWriteBuffer));
		RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("UserDataBaseName"));
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
