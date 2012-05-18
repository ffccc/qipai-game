#include "StdAfx.h"
#include "InitParameter.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ�䶨��
#define TIME_CONNECT				30									//����ʱ��
#define TIME_COLLECT				30									//ͳ��ʱ��

//�ͻ�ʱ��
#define TIME_INTERMIT				0									//�ж�ʱ��
#define TIME_ONLINE_COUNT			600									//����ʱ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CInitParameter::CInitParameter()
{ 
	InitParameter();
}

//��������
CInitParameter::~CInitParameter()
{
}

//��ʼ��
VOID CInitParameter::InitParameter()
{
	//ʱ�䶨��
	m_wConnectTime=TIME_CONNECT;
	m_wCollectTime=TIME_COLLECT;

	//Э����Ϣ
	m_wCorrespondPort=PORT_CENTER;
	ZeroMemory(&m_CorrespondAddress,sizeof(m_CorrespondAddress));

	//������Ϣ
	ZeroMemory(m_szServerName,sizeof(m_szServerName));
	ZeroMemory(&m_ServiceAddress,sizeof(m_ServiceAddress));
	ZeroMemory(&m_TreasureDBParameter,sizeof(m_TreasureDBParameter));
	ZeroMemory(&m_PlatformDBParameter,sizeof(m_PlatformDBParameter));

	return;
}

//��������
VOID CInitParameter::LoadInitParameter()
{
	//���ò���
	InitParameter();

	//��ȡ·��
	TCHAR szWorkDir[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));

	//����·��
	TCHAR szIniFile[MAX_PATH]=TEXT("");
	_sntprintf(szIniFile,CountArray(szIniFile),TEXT("%s\\ServerParameter.ini"),szWorkDir);

	//��ȡ����
	CWHIniData IniData;
	IniData.SetIniFilePath(szIniFile);

	//��ȡ����
	IniData.ReadEncryptString(TEXT("ServerInfo"),TEXT("ServiceName"),NULL,m_szServerName,CountArray(m_szServerName));
	IniData.ReadEncryptString(TEXT("ServerInfo"),TEXT("ServiceAddr"),NULL,m_ServiceAddress.szAddress,CountArray(m_ServiceAddress.szAddress));

	//Э����Ϣ
	m_wCorrespondPort=IniData.ReadInt(TEXT("Correspond"),TEXT("ServicePort"),m_wCorrespondPort);
	IniData.ReadEncryptString(TEXT("ServerInfo"),TEXT("CorrespondAddr"),NULL,m_CorrespondAddress.szAddress,CountArray(m_CorrespondAddress.szAddress));

	//������Ϣ
	m_TreasureDBParameter.wDataBasePort=(WORD)IniData.ReadInt(TEXT("TreasureDB"),TEXT("DBPort"),1433);
	IniData.ReadEncryptString(TEXT("TreasureDB"),TEXT("DBAddr"),NULL,m_TreasureDBParameter.szDataBaseAddr,CountArray(m_TreasureDBParameter.szDataBaseAddr));
	IniData.ReadEncryptString(TEXT("TreasureDB"),TEXT("DBUser"),NULL,m_TreasureDBParameter.szDataBaseUser,CountArray(m_TreasureDBParameter.szDataBaseUser));
	IniData.ReadEncryptString(TEXT("TreasureDB"),TEXT("DBPass"),NULL,m_TreasureDBParameter.szDataBasePass,CountArray(m_TreasureDBParameter.szDataBasePass));
	IniData.ReadEncryptString(TEXT("TreasureDB"),TEXT("DBName"),szTreasureDB,m_TreasureDBParameter.szDataBaseName,CountArray(m_TreasureDBParameter.szDataBaseName));

	//������Ϣ
	m_PlatformDBParameter.wDataBasePort=(WORD)IniData.ReadInt(TEXT("PlatformDB"),TEXT("DBPort"),1433);
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBAddr"),NULL,m_PlatformDBParameter.szDataBaseAddr,CountArray(m_PlatformDBParameter.szDataBaseAddr));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBUser"),NULL,m_PlatformDBParameter.szDataBaseUser,CountArray(m_PlatformDBParameter.szDataBaseUser));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBPass"),NULL,m_PlatformDBParameter.szDataBasePass,CountArray(m_PlatformDBParameter.szDataBasePass));
	IniData.ReadEncryptString(TEXT("PlatformDB"),TEXT("DBName"),szPlatformDB,m_PlatformDBParameter.szDataBaseName,CountArray(m_PlatformDBParameter.szDataBaseName));

	return;
}

//////////////////////////////////////////////////////////////////////////////////
