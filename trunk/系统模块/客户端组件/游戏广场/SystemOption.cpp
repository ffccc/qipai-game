#include "Stdafx.h"
#include "SystemOption.h"
#include "ServerItemView.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define REG_GLOBAL_OPTIONS			TEXT("GlobalOption")				//ȫ������

//////////////////////////////////////////////////////////////////////////

CGlobalOption						g_GlobalOption;						//ȫ������

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerOption::CServerOption(WORD wServerID, CListServer * pListServer)
	: m_wServerID(wServerID),m_pListServer(pListServer)
{
	//Я������
	m_bPassword=false;
	m_szPassword[0]=0;
	m_szEnterPassword[0]=0;

	return;
}

//��������
CServerOption::~CServerOption()
{
}

//��������
void CServerOption::LoadOptionParameter()
{
	return;
}

//��������
void CServerOption::SaveOptionParameter()
{
	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameOption::CGameOption(WORD wKindID, CListKind * pListKind) 
	: m_wKindID(wKindID),m_pListKind(pListKind)
{
	//��������
	m_bLimitScore=false;
	m_lMaxScore=2147483647;
	m_lLessScore=-2147483646;

	//��������
	m_wFleeRate=5000;
	m_bLimitFlee=false;

	//ʤ������
	m_wWinRate=0;
	m_bLimitWin=false;

	return;
}

//��������
CGameOption::~CGameOption()
{
}

//��������
void CGameOption::LoadOptionParameter()
{
	//��������
	CWinApp * pApp=AfxGetApp();
	tagGameKind * pGameKind=m_pListKind->GetItemInfo();

	//��������
	m_lMaxScore=pApp->GetProfileInt(pGameKind->szKindName,TEXT("MaxScore"),m_lMaxScore);
	m_lLessScore=pApp->GetProfileInt(pGameKind->szKindName,TEXT("LessScore"),m_lLessScore);
	m_bLimitScore=pApp->GetProfileInt(pGameKind->szKindName,TEXT("LimitScore"),m_bLimitScore)?true:false;

	//��������
	m_wFleeRate=pApp->GetProfileInt(pGameKind->szKindName,TEXT("FleeRate"),m_wFleeRate);
	m_bLimitFlee=pApp->GetProfileInt(pGameKind->szKindName,TEXT("LimitFlee"),m_bLimitFlee)?true:false;

	//ʤ������
	m_wWinRate=pApp->GetProfileInt(pGameKind->szKindName,TEXT("WinRate"),m_wWinRate);
	m_bLimitWin=pApp->GetProfileInt(pGameKind->szKindName,TEXT("LimitWin"),m_bLimitWin)?true:false;

	return;
}

//��������
void CGameOption::SaveOptionParameter()
{
	//��������
	CWinApp * pApp=AfxGetApp();
	tagGameKind * pGameKind=m_pListKind->GetItemInfo();

	//��������
	pApp->WriteProfileInt(pGameKind->szKindName,TEXT("LimitScore"),m_bLimitScore);
	pApp->WriteProfileInt(pGameKind->szKindName,TEXT("MaxScore"),m_lMaxScore);
	pApp->WriteProfileInt(pGameKind->szKindName,TEXT("LessScore"),m_lLessScore);

	//��������
	pApp->WriteProfileInt(pGameKind->szKindName,TEXT("FleeRate"),m_wFleeRate);
	pApp->WriteProfileInt(pGameKind->szKindName,TEXT("LimitFlee"),m_bLimitFlee);

	//ʤ������
	pApp->WriteProfileInt(pGameKind->szKindName,TEXT("WinRate"),m_wWinRate);
	pApp->WriteProfileInt(pGameKind->szKindName,TEXT("LimitWin"),m_bLimitWin);

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalOption::CGlobalOption()
{
	//ȫ������
	m_bExpandedList=true;
	m_enAcountsRule=enAcountsRule_Accounts;
	m_wBossHotKey=MAKEWORD(VK_F12,HOTKEYF_CONTROL);

	//��������
	m_bLimitDetest=true;
	m_bCheckSameIP=false;

	//��Ϣ����
	m_bSaveWisper=true;
	m_bShowInOutMessage=false;
	m_bOnlyFriendMessage=false;
	m_bHideDetestMessage=true;

	//��������
	m_bAutoSit=true;
	m_InviteMode=enInviteMode_All;

	//��ɫ����
	m_crChatTX=RGB(0,0,0);
	m_crMsgName=RGB(0,0,200);
	m_crMsgNormal=RGB(0,0,0);
	m_crMsgSysHead=RGB(200,0,0);
	m_crMsgSysString=RGB(0,0,200);
	m_crServerListTX=RGB(0,0,0);
	m_crServerListBK=RGB(200,200,200);

	//��������
	m_ProxyServerType=ProxyType_None;
	ZeroMemory(&m_ProxyServerInfo,sizeof(m_ProxyServerInfo));

	return;
}

//��������
CGlobalOption::~CGlobalOption()
{
	//ɾ������
	CGameOption * pGameOption=NULL;
	for (INT_PTR i=0;i<m_GameOptionArray.GetCount();i++)
	{
		pGameOption=m_GameOptionArray[i];
		ASSERT(pGameOption!=NULL);
		SafeDelete(pGameOption);
	}
	m_GameOptionArray.RemoveAll();

	//ɾ������
	CServerOption * pServerOption=NULL;
	for (INT_PTR i=0;i<m_ServerOptionArray.GetCount();i++)
	{
		pServerOption=m_ServerOptionArray[i];
		ASSERT(pServerOption!=NULL);
		SafeDelete(pServerOption);
	}
	m_ServerOptionArray.RemoveAll();

	return;
}

//��������
void CGlobalOption::LoadOptionParameter()
{
	//��������
	CWinApp * pApp=AfxGetApp();

	//ȫ������
	m_bExpandedList=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ExpandedList"),m_bExpandedList)?true:false;
	m_enAcountsRule=(enAcountsRule)pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("AcountsRule"),m_enAcountsRule);

	//��������
	m_bLimitDetest=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("LimitDetest"),m_bLimitDetest)?true:false;
	m_bCheckSameIP=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("AutoHideServerList"),m_bCheckSameIP)?true:false;

	//��Ϣ����
	m_wBossHotKey=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("BossHotKey"),m_wBossHotKey);
	m_bSaveWisper=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("SaveWisper"),m_bSaveWisper)?true:false;
	m_bShowInOutMessage=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ShowInOutMessage"),m_bShowInOutMessage)?true:false;
	m_bOnlyFriendMessage=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("OnlyFriendMessage"),m_bOnlyFriendMessage)?true:false;
	m_bHideDetestMessage=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("HideDetestMessage"),m_bHideDetestMessage)?true:false;

	//��������
	m_bAutoSit=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("AutoSit"),m_bAutoSit)?true:false;
	m_InviteMode=(enInviteMode)pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("InviteMode"),m_InviteMode);

	//��ɫ����
	m_crChatTX=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ChatTX"),m_crChatTX);
	m_crMsgName=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgName"),m_crMsgName);
	m_crMsgNormal=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgNormal"),m_crMsgNormal);
	m_crMsgSysHead=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgSysHead"),m_crMsgSysHead);
	m_crMsgSysString=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgSysString"),m_crMsgSysString);
	m_crServerListTX=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ServerListTX"),m_crServerListTX);
	m_crServerListBK=pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ServerListBK"),m_crServerListBK);

	//���ò���
	m_ProxyServerInfo.szUserName[0]=0;
	m_ProxyServerInfo.szPassword[0]=0;
	m_ProxyServerInfo.szHttpDomain[0]=0;
	m_ProxyServerInfo.szProxyServer[0]=0;

	//������Ϣ
	m_ProxyServerInfo.wProxyPort=(WORD)pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ProxyPort"),0);
	m_ProxyServerType=(enProxyServerType)pApp->GetProfileInt(REG_GLOBAL_OPTIONS,TEXT("ProxyType"),ProxyType_None);
	CopyMemory(m_ProxyServerInfo.szUserName,pApp->GetProfileString(REG_GLOBAL_OPTIONS,TEXT("UserName"),TEXT("")),CountArray(m_ProxyServerInfo.szUserName));
	CopyMemory(m_ProxyServerInfo.szPassword,pApp->GetProfileString(REG_GLOBAL_OPTIONS,TEXT("Password"),TEXT("")),CountArray(m_ProxyServerInfo.szPassword));
	CopyMemory(m_ProxyServerInfo.szHttpDomain,pApp->GetProfileString(REG_GLOBAL_OPTIONS,TEXT("HttpDomain"),TEXT("")),CountArray(m_ProxyServerInfo.szHttpDomain));
	CopyMemory(m_ProxyServerInfo.szProxyServer,pApp->GetProfileString(REG_GLOBAL_OPTIONS,TEXT("ProxyServer"),TEXT("")),CountArray(m_ProxyServerInfo.szProxyServer));
	switch (m_ProxyServerType)
	{
	case ProxyType_None:
	case ProxyType_Http:
	case ProxyType_Socks4:
	case ProxyType_Socks5: { break; }
	default: { m_ProxyServerType=ProxyType_None; break; }
	}

	//��Ϸ����
	CGameOption * pGameOption=NULL;
	for (INT_PTR i=0;i<m_GameOptionArray.GetCount();i++)
	{
		pGameOption=m_GameOptionArray[i];
		ASSERT(pGameOption!=NULL);
		pGameOption->LoadOptionParameter();
	}

	//��������
	CServerOption * pServerOption=NULL;
	for (INT_PTR i=0;i<m_ServerOptionArray.GetCount();i++)
	{
		pServerOption=m_ServerOptionArray[i];
		ASSERT(pServerOption!=NULL);
		pServerOption->LoadOptionParameter();
	}

	return;
}

//��������
void CGlobalOption::SaveOptionParameter()
{
	//��������
	CWinApp * pApp=AfxGetApp();

	//ȫ������
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ExpandedList"),m_bExpandedList);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("AcountsRule"),m_enAcountsRule);

	//��������
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("BossHotKey"),m_wBossHotKey);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("LimitDetest"),m_bLimitDetest);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("AutoHideServerList"),m_bCheckSameIP);

	//��Ϣ����
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("SaveWisper"),m_bSaveWisper);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ShowInOutMessage"),m_bShowInOutMessage);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("OnlyFriendMessage"),m_bOnlyFriendMessage);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("HideDetestMessage"),m_bHideDetestMessage);

	//��������
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("AutoSit"),m_bAutoSit);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("InviteMode"),m_InviteMode);

	//��ɫ����
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ChatTX"),m_crChatTX);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgName"),m_crMsgName);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgNormal"),m_crMsgNormal);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgSysHead"),m_crMsgSysHead);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("MsgSysString"),m_crMsgSysString);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ServerListTX"),m_crServerListTX);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ServerListBK"),m_crServerListBK);

	//������Ϣ
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ProxyType"),m_ProxyServerType);
	pApp->WriteProfileInt(REG_GLOBAL_OPTIONS,TEXT("ProxyPort"),m_ProxyServerInfo.wProxyPort);
	pApp->WriteProfileString(REG_GLOBAL_OPTIONS,TEXT("UserName"),m_ProxyServerInfo.szUserName);
	pApp->WriteProfileString(REG_GLOBAL_OPTIONS,TEXT("Password"),m_ProxyServerInfo.szPassword);
	pApp->WriteProfileString(REG_GLOBAL_OPTIONS,TEXT("HttpDomain"),m_ProxyServerInfo.szHttpDomain);
	pApp->WriteProfileString(REG_GLOBAL_OPTIONS,TEXT("ProxyServer"),m_ProxyServerInfo.szProxyServer);

	//��Ϸ����
	CGameOption * pGameOption=NULL;
	for (INT_PTR i=0;i<m_GameOptionArray.GetCount();i++)
	{
		pGameOption=m_GameOptionArray[i];
		ASSERT(pGameOption!=NULL);
		pGameOption->SaveOptionParameter();
	}

	//��������
	CServerOption * pServerOption=NULL;
	for (INT_PTR i=0;i<m_ServerOptionArray.GetCount();i++)
	{
		pServerOption=m_ServerOptionArray[i];
		ASSERT(pServerOption!=NULL);
		pServerOption->SaveOptionParameter();
	}

	return;
}

//��Ϸ����
CGameOption * CGlobalOption::GetGameOption(CListKind * pListKind)
{
	//��ȡ����
	ASSERT(pListKind!=NULL);
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	
	//Ѱ���ִ�
	CGameOption * pGameOption=NULL;
	for (INT_PTR i=0;i<m_GameOptionArray.GetCount();i++)
	{
		pGameOption=m_GameOptionArray[i];
		ASSERT(pGameOption!=NULL);
		if (pGameOption->m_wKindID==pGameKind->wKindID) return pGameOption;
	}

	//��������
	try
	{
		pGameOption=new CGameOption(pGameKind->wKindID,pListKind);
		if (pGameOption==NULL) return NULL;
		pGameOption->LoadOptionParameter();
		m_GameOptionArray.Add(pGameOption);
		return pGameOption;
	}
	catch (...) {}

	return NULL;
}

//��������
CServerOption * CGlobalOption::GetServerOption(CListServer * pListServer)
{
	//��ȡ����
	ASSERT(pListServer!=NULL);
	tagGameServer * pGameServer=pListServer->GetItemInfo();
	
	//Ѱ���ִ�
	CServerOption * pServerOption=NULL;
	for (INT_PTR i=0;i<m_ServerOptionArray.GetCount();i++)
	{
		pServerOption=m_ServerOptionArray[i];
		ASSERT(pServerOption!=NULL);
		if (pServerOption->m_wServerID==pGameServer->wServerID) return pServerOption;
	}

	//��������
	try
	{
		pServerOption=new CServerOption(pGameServer->wServerID,pListServer);
		if (pServerOption==NULL) return NULL;
		pServerOption->LoadOptionParameter();
		m_ServerOptionArray.Add(pServerOption);
		return pServerOption;
	}
	catch (...) {}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
