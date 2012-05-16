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
	m_szAccreditID[0]=0;
	m_szCenterServerAddr[0]=0;

	m_wGameUserDBPort=1433;
	m_szGameUserDBAddr[0]=0;
	m_szGameUserDBUser[0]=0;
	m_szGameUserDBPass[0]=0;
	lstrcpyn(m_szGameUserDBName,szGameUserDB,CountArray(m_szGameUserDBName));

	m_wServerInfoDBPort=1433;
	m_szServerInfoDBAddr[0]=0;
	m_szServerInfoDBUser[0]=0;
	m_szServerInfoDBPass[0]=0;
	lstrcpyn(m_szServerInfoDBName,szServerInfoDB,CountArray(m_szServerInfoDBName));

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
	_snprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\ServiceLoader"),szSystemRegKey);
	if (RegParamter.Open(HKEY_CURRENT_USER,szRegName,KEY_READ)!=ERROR_SUCCESS) return;

	//��������
	TCHAR szReadData[1024]=TEXT("");
	DWORD dwDataType,dwReadData,dwDataSize;

	//���ķ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	LONG lErrorCode=RegParamter.QueryValue(TEXT("CenterServerAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szCenterServerAddr,sizeof(m_szCenterServerAddr));

	//��Ȩ����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("AccreditID"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szAccreditID,sizeof(m_szAccreditID));

	//���ݿ�˿�
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBPort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wGameUserDBPort=(WORD)dwReadData;

	//���ݿ��ַ
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameUserDBAddr,sizeof(m_szGameUserDBAddr));

	//���ݿ��û���
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBUser"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameUserDBUser,sizeof(m_szGameUserDBUser));

	//���ݿ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBPass"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameUserDBPass,sizeof(m_szGameUserDBPass));

	//���ݿ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("GameUserDBName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szGameUserDBName,sizeof(m_szGameUserDBName));

	//���ݿ�˿�
	dwDataSize=sizeof(dwReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBPort"),&dwDataType,&dwReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) m_wServerInfoDBPort=(WORD)dwReadData;

	//���ݿ��ַ
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBAddr"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szServerInfoDBAddr,sizeof(m_szServerInfoDBAddr));

	//���ݿ��û���
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBUser"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szServerInfoDBUser,sizeof(m_szServerInfoDBUser));

	//���ݿ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBPass"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szServerInfoDBPass,sizeof(m_szServerInfoDBPass));

	//���ݿ�����
	szReadData[0]=0;
	dwDataSize=sizeof(szReadData);
	lErrorCode=RegParamter.QueryValue(TEXT("ServerInfoDBName"),&dwDataType,szReadData,&dwDataSize);
	if (lErrorCode==ERROR_SUCCESS) CXOREncrypt::CrevasseData(szReadData,m_szServerInfoDBName,sizeof(m_szServerInfoDBName));

	return;
}

//��������
void CInitParamter::SaveInitParamter(bool bSaveAll)
{
	//��ע���
	CRegKey RegParamter;
	TCHAR szRegName[MAX_PATH];
	_snprintf(szRegName,sizeof(szRegName),TEXT("Software\\%s\\ServiceLoader"),szSystemRegKey);
	if (RegParamter.Create(HKEY_CURRENT_USER,szRegName)!=ERROR_SUCCESS) return;

	//��������
	TCHAR szWriteBuffer[1024]=TEXT("");

	//���ķ�����
	CXOREncrypt::EncryptData(m_szCenterServerAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("CenterServerAddr"));

	//��Ȩ����
	CXOREncrypt::EncryptData(m_szAccreditID,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("AccreditID"));

	//��¼���ݿ�
	RegParamter.SetDWORDValue(TEXT("GameUserDBPort"),m_wGameUserDBPort);
	CXOREncrypt::EncryptData(m_szGameUserDBAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("GameUserDBAddr"));
	CXOREncrypt::EncryptData(m_szGameUserDBUser,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("GameUserDBUser"));
	CXOREncrypt::EncryptData(m_szGameUserDBPass,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("GameUserDBPass"));

	//�������ݿ�
	RegParamter.SetDWORDValue(TEXT("ServerInfoDBPort"),m_wServerInfoDBPort);
	CXOREncrypt::EncryptData(m_szServerInfoDBAddr,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerInfoDBAddr"));
	CXOREncrypt::EncryptData(m_szServerInfoDBUser,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerInfoDBUser"));
	CXOREncrypt::EncryptData(m_szServerInfoDBPass,szWriteBuffer,sizeof(szWriteBuffer));
	RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerInfoDBPass"));

	if (bSaveAll==true)
	{
		//��¼���ݿ�
		CXOREncrypt::EncryptData(m_szServerInfoDBName,szWriteBuffer,sizeof(szWriteBuffer));
		RegParamter.SetKeyValue(TEXT(""),szWriteBuffer,TEXT("ServerInfoDBName"));
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
