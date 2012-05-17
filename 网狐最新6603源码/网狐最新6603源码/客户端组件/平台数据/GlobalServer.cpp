#include "StdAfx.h"
#include "GlobalServer.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalServer::CGlobalServer()
{
}

//析构函数
CGlobalServer::~CGlobalServer()
{
}

//子项数目
WORD CGlobalServer::GetItemCount()
{
	//读取数目
	WORD wItemCount=ReadFileValue(TEXT("ServerInfo"),TEXT("ItemCount"),0);

	return wItemCount;
}

//子项名字
VOID CGlobalServer::GetServerItemName(WORD wIndex, TCHAR szResult[LEN_SERVER])
{
	//构造名字
	TCHAR szItemName[LEN_SERVER]=TEXT("");
	_sntprintf(szItemName,CountArray(szItemName),TEXT("ItemDescribe%d"),wIndex+1);

	//读取名字
	ReadFileString(TEXT("ServerInfo"),szItemName,szResult,LEN_SERVER);

	return;
}

//上次登录
VOID CGlobalServer::SetLastServerName(LPCTSTR pszLastServer)
{
	//写入名字
	WriteFileString(TEXT("GlobalInfo"),TEXT("LastServerName"),pszLastServer);

	return;
}

//上次登录
VOID CGlobalServer::GetLastServerName(TCHAR szResult[LEN_SERVER])
{
	//读取名字
	ReadFileString(TEXT("GlobalInfo"),TEXT("LastServerName"),szResult,LEN_SERVER);

	return;
}

//子项信息
bool CGlobalServer::GetServerItemInfo(LPCTSTR pszServerName, tagServerItem & ServerItem)
{
	//效验参数
	ASSERT(pszServerName!=NULL);
	if (pszServerName==NULL) return false;

	//加密数据
	TCHAR szServerName[LEN_MD5];
	CWHEncrypt::MD5Encrypt(pszServerName,szServerName);

	//获取信息
	ServerItem.wServerPort=ReadFileValue(szServerName,TEXT("ServerPort"),PORT_LOGON);
	ServerItem.dwServerAddr=ReadFileValue(szServerName,TEXT("ServerAddr"),INADDR_NONE);
	ReadFileString(szServerName,TEXT("ServerDomain"),ServerItem.szServerHost,CountArray(ServerItem.szServerHost));

	//地址转换
	if (ServerItem.dwServerAddr==0L) ServerItem.dwServerAddr=INADDR_NONE;

	return ((ServerItem.dwServerAddr!=INADDR_NONE)||(ServerItem.szServerHost[0]!=0));
}

//写入数值
VOID CGlobalServer::WriteFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nValue)
{
	//获取目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\ServerInfo.INI"),szDirectory);

	//构造数据
	TCHAR szString[16]=TEXT("");
	_sntprintf(szString,CountArray(szString),TEXT("%d"),nValue);

	//写入数据
	WritePrivateProfileString(pszKeyName,pszItemName,szString,szServerInfoPath);

	return;
}

//写入数据
VOID CGlobalServer::WriteFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, LPCTSTR pszString)
{
	//获取目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\ServerInfo.INI"),szDirectory);

	//加密数据
	TCHAR szResultEncrypt[MAX_ENCRYPT_LEN];
	ZeroMemory(szResultEncrypt,sizeof(szResultEncrypt));

	//写入数据
	CWHEncrypt::XorEncrypt(pszString,szResultEncrypt,CountArray(szResultEncrypt));
	WritePrivateProfileString(pszKeyName,pszItemName,szResultEncrypt,szServerInfoPath);

	return;
}

//读取数值
UINT CGlobalServer::ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault)
{
	//获取目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\ServerInfo.INI"),szDirectory);

	//读取数据
	UINT nValue=GetPrivateProfileInt(pszKeyName,pszItemName,nDefault,szServerInfoPath);

	return nValue;
}

//读取数据
VOID CGlobalServer::ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount)
{
	//获取目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\ServerInfo.INI"),szDirectory);

	//读取数据
	TCHAR szResultEncrypt[MAX_ENCRYPT_LEN];
	GetPrivateProfileString(pszKeyName,pszItemName,TEXT(""),szResultEncrypt,CountArray(szResultEncrypt),szServerInfoPath);

	//解密数据
	szResult[0]=0;
	CWHEncrypt::XorCrevasse(szResultEncrypt,szResult,wMaxCount);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
