#include "StdAfx.h"
#include "InitParameter.h"

//////////////////////////////////////////////////////////////////////////////////

#define REG_ITEM_NAME				TEXT("Information")					//ע�����

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
	//ϵͳ����
	m_wMaxConnect=MAX_CONTENT;
	m_wServicePort=PORT_CENTER;

	//������Ϣ
	ZeroMemory(m_szServerName,sizeof(m_szServerName));

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
	m_wMaxConnect=IniData.ReadInt(TEXT("Correspond"),TEXT("ConnectMax"),m_wMaxConnect);
	m_wServicePort=IniData.ReadInt(TEXT("Correspond"),TEXT("ServicePort"),m_wServicePort);
	IniData.ReadEncryptString(TEXT("ServerInfo"),TEXT("ServiceName"),NULL,m_szServerName,CountArray(m_szServerName));

	return;
}

//////////////////////////////////////////////////////////////////////////////////
