#include "StdAfx.h"
#include "GlobalServer.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalServer::CGlobalServer()
{
}

//��������
CGlobalServer::~CGlobalServer()
{
}

//������Ŀ
WORD CGlobalServer::GetItemCount()
{
	//��ȡ��Ŀ
	WORD wItemCount=ReadFileValue(TEXT("ServerInfo"),TEXT("ItemCount"),0);

	return wItemCount;
}

//��������
VOID CGlobalServer::GetServerItemName(WORD wIndex, TCHAR szResult[LEN_SERVER])
{
	//��������
	TCHAR szItemName[LEN_SERVER]=TEXT("");
	_sntprintf(szItemName,CountArray(szItemName),TEXT("ItemDescribe%d"),wIndex+1);

	//��ȡ����
	ReadFileString(TEXT("ServerInfo"),szItemName,szResult,LEN_SERVER);

	return;
}

//�ϴε�¼
VOID CGlobalServer::SetLastServerName(LPCTSTR pszLastServer)
{
	//д������
	WriteFileString(TEXT("GlobalInfo"),TEXT("LastServerName"),pszLastServer);

	return;
}

//�ϴε�¼
VOID CGlobalServer::GetLastServerName(TCHAR szResult[LEN_SERVER])
{
	//��ȡ����
	ReadFileString(TEXT("GlobalInfo"),TEXT("LastServerName"),szResult,LEN_SERVER);

	return;
}

//������Ϣ
bool CGlobalServer::GetServerItemInfo(LPCTSTR pszServerName, tagServerItem & ServerItem)
{
	//Ч�����
	ASSERT(pszServerName!=NULL);
	if (pszServerName==NULL) return false;

	//��������
	TCHAR szServerName[LEN_MD5];
	CWHEncrypt::MD5Encrypt(pszServerName,szServerName);

	//��ȡ��Ϣ
	ServerItem.wServerPort=ReadFileValue(szServerName,TEXT("ServerPort"),PORT_LOGON);
	ServerItem.dwServerAddr=ReadFileValue(szServerName,TEXT("ServerAddr"),INADDR_NONE);
	ReadFileString(szServerName,TEXT("ServerDomain"),ServerItem.szServerHost,CountArray(ServerItem.szServerHost));

	//��ַת��
	if (ServerItem.dwServerAddr==0L) ServerItem.dwServerAddr=INADDR_NONE;

	return ((ServerItem.dwServerAddr!=INADDR_NONE)||(ServerItem.szServerHost[0]!=0));
}

//д����ֵ
VOID CGlobalServer::WriteFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nValue)
{
	//��ȡĿ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\ServerInfo.INI"),szDirectory);

	//��������
	TCHAR szString[16]=TEXT("");
	_sntprintf(szString,CountArray(szString),TEXT("%d"),nValue);

	//д������
	WritePrivateProfileString(pszKeyName,pszItemName,szString,szServerInfoPath);

	return;
}

//д������
VOID CGlobalServer::WriteFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, LPCTSTR pszString)
{
	//��ȡĿ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\ServerInfo.INI"),szDirectory);

	//��������
	TCHAR szResultEncrypt[MAX_ENCRYPT_LEN];
	ZeroMemory(szResultEncrypt,sizeof(szResultEncrypt));

	//д������
	CWHEncrypt::XorEncrypt(pszString,szResultEncrypt,CountArray(szResultEncrypt));
	WritePrivateProfileString(pszKeyName,pszItemName,szResultEncrypt,szServerInfoPath);

	return;
}

//��ȡ��ֵ
UINT CGlobalServer::ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault)
{
	//��ȡĿ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\ServerInfo.INI"),szDirectory);

	//��ȡ����
	UINT nValue=GetPrivateProfileInt(pszKeyName,pszItemName,nDefault,szServerInfoPath);

	return nValue;
}

//��ȡ����
VOID CGlobalServer::ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount)
{
	//��ȡĿ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\ServerInfo.INI"),szDirectory);

	//��ȡ����
	TCHAR szResultEncrypt[MAX_ENCRYPT_LEN];
	GetPrivateProfileString(pszKeyName,pszItemName,TEXT(""),szResultEncrypt,CountArray(szResultEncrypt),szServerInfoPath);

	//��������
	szResult[0]=0;
	CWHEncrypt::XorCrevasse(szResultEncrypt,szResult,wMaxCount);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
