#include "StdAfx.h"
#include "OptionParameter.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CParameterGlobal * CParameterGlobal::m_pParameterGlobal=NULL;			//ȫ������

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CParameterGame::CParameterGame()
{
	//Ĭ�ϲ���
	DefaultParameter();

	//���Ա���
	ZeroMemory(m_szRegKeyName,sizeof(m_szRegKeyName));

	return;
}

//��������
CParameterGame::~CParameterGame()
{
}

//���ز���
VOID CParameterGame::LoadParameter()
{
	//��������
	CWHRegKey RegOptionItem;

	//���ñ���
	if (RegOptionItem.OpenRegKey(m_szRegKeyName,REG_GAME_OPTION,false)==true)
	{
		//ʤ������
		m_wMinWinRate=(WORD)RegOptionItem.GetValue(TEXT("MinWinRate"),m_wMinWinRate);
		m_bLimitWinRate=RegOptionItem.GetValue(TEXT("LimitWinRate"),m_bLimitWinRate)?true:false;

		//��������
		m_wMaxFleeRate=(WORD)RegOptionItem.GetValue(TEXT("MaxFleeRate"),m_wMaxFleeRate);
		m_bLimitFleeRate=RegOptionItem.GetValue(TEXT("LimitFleeRate"),m_bLimitFleeRate)?true:false;
		
		//��������
		m_lMaxGameScore=RegOptionItem.GetValue(TEXT("MaxGameScore"),m_lMaxGameScore);
		m_lMinGameScore=RegOptionItem.GetValue(TEXT("MinGameScore"),m_lMinGameScore);
		m_bLimitGameScore=RegOptionItem.GetValue(TEXT("LimitGameScore"),m_bLimitGameScore)?true:false;
	}

	return;
}

//�������
VOID CParameterGame::SaveParameter()
{
	//��������
	CWHRegKey RegOptionItem;

	//���ñ���
	if (RegOptionItem.OpenRegKey(m_szRegKeyName,REG_GAME_OPTION,true)==true)
	{
		//ʤ������
		RegOptionItem.WriteValue(TEXT("MinWinRate"),m_wMinWinRate);
		RegOptionItem.WriteValue(TEXT("LimitWinRate"),m_bLimitWinRate);

		//��������
		RegOptionItem.WriteValue(TEXT("MaxFleeRate"),m_wMaxFleeRate);
		RegOptionItem.WriteValue(TEXT("LimitFleeRate"),m_bLimitFleeRate);

		//��������
		RegOptionItem.WriteValue(TEXT("MaxGameScore"),m_lMaxGameScore);
		RegOptionItem.WriteValue(TEXT("MinGameScore"),m_lMinGameScore);
		RegOptionItem.WriteValue(TEXT("LimitGameScore"),m_bLimitGameScore);
	}

	return;
}

//Ĭ�ϲ���
VOID CParameterGame::DefaultParameter()
{
	//ʤ������
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//��������
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//��������
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	return;
}

//���ò���
bool CParameterGame::InitParameter(LPCTSTR pszProcessName)
{
	//�������
	WORD wNameIndex=0;
	while (wNameIndex<(CountArray(m_szRegKeyName)-1))
	{
		//��ֹ�ж�
		if (pszProcessName[wNameIndex]==0) break;
		if (pszProcessName[wNameIndex]==TEXT('.')) break;

		//�����ַ�
		WORD wCurrentIndex=wNameIndex++;
		m_szRegKeyName[wCurrentIndex]=pszProcessName[wCurrentIndex];
	}

	//���ñ���
	m_szRegKeyName[wNameIndex]=0;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CParameterServer::CParameterServer()
{
	//Ĭ�ϲ���
	DefaultParameter();

	return;
}

//��������
CParameterServer::~CParameterServer()
{
}

//Ĭ�ϲ���
VOID CParameterServer::DefaultParameter()
{
	//���ñ���
	m_bTakePassword=false;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CParameterGlobal::CParameterGlobal()
{
	//Ĭ�ϲ���
	DefaultParameter();

	//���ö���
	ASSERT(m_pParameterGlobal==NULL);
	if (m_pParameterGlobal==NULL) m_pParameterGlobal=this;

	return;
}

//��������
CParameterGlobal::~CParameterGlobal()
{
	//��������
	POSITION PositionGame=m_ParameterGameMap.GetStartPosition();
	POSITION PositionServer=m_ParameterGameMap.GetStartPosition();

	//ɾ������
	while (PositionGame!=NULL)
	{
		//��ȡ����
		WORD wKindID=0;
		CParameterGame * pParameterGame=NULL;
		m_ParameterGameMap.GetNextAssoc(PositionGame,wKindID,pParameterGame);

		//ɾ������
		SafeDelete(pParameterGame);
	}

	//ɾ������
	while (PositionServer!=NULL)
	{
		//��ȡ����
		WORD wServerID=0;
		CParameterServer * pParameterServer=NULL;
		m_ParameterServerMap.GetNextAssoc(PositionServer,wServerID,pParameterServer);

		//ɾ������
		SafeDelete(pParameterServer);
	}

	//ɾ������
	m_ParameterGameMap.RemoveAll();
	m_ParameterServerMap.RemoveAll();

	//�ͷŶ���
	ASSERT(m_pParameterGlobal==this);
	if (m_pParameterGlobal==this) m_pParameterGlobal=NULL;

	return;
}

//���ز���
VOID CParameterGlobal::LoadParameter()
{
	//��������
	CWHRegKey RegOptionItem;

	//������Ϣ
	if (RegOptionItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		//��������
		m_bSalienceTable=RegOptionItem.GetValue(TEXT("SalienceTable"),m_bSalienceTable)?true:false;
		m_bFullScreenBrowse=RegOptionItem.GetValue(TEXT("FullScreenBrowse"),m_bFullScreenBrowse)?true:false;

		//������Դ
		m_SkinRenderInfo.wColorH=(WORD)RegOptionItem.GetValue(TEXT("RenderColorH"),m_SkinRenderInfo.wColorH);
		m_SkinRenderInfo.dRectifyS=(DOUBLE)(RegOptionItem.GetValue(TEXT("RenderColorS"),(INT)((m_SkinRenderInfo.dRectifyS+1.0)*100)))/100.0-1.0;
		m_SkinRenderInfo.dRectifyB=(DOUBLE)(RegOptionItem.GetValue(TEXT("RenderColorB"),(INT)((m_SkinRenderInfo.dRectifyB+1.0)*100)))/100.0-1.0;
	}

	//���Ʊ���
	if (RegOptionItem.OpenRegKey(REG_GOBAL_OPTION,false)==true)
	{
		//�Զ���¼
		m_bLogonAuto=RegOptionItem.GetValue(TEXT("LogonAuto"),m_bLogonAuto)?true:false;
		m_bWriteCookie=RegOptionItem.GetValue(TEXT("WriteCookie"),m_bWriteCookie)?true:false;

		//����ģʽ
		m_cbInviteMode=(BYTE)RegOptionItem.GetValue(TEXT("InviteMode"),m_cbInviteMode);
		switch (m_cbInviteMode)
		{
		case INVITE_MODE_NONE:
		case INVITE_MODE_FRIEND: { break; }
		default: { m_cbInviteMode=INVITE_MODE_ALL; }
		}

		//��Ϣģʽ
		m_cbMessageMode=(BYTE)RegOptionItem.GetValue(TEXT("MessageMode"),m_cbMessageMode);
		switch (m_cbMessageMode)
		{
		case MESSAGE_MODE_ALL:
		case MESSAGE_MODE_FRIEND: { break; }
		default: { m_cbMessageMode=MESSAGE_MODE_DETEST; }
		}

		//�Զ�����
		m_cbActionHitAutoJoin=(BYTE)RegOptionItem.GetValue(TEXT("ActionHitAutoJoin"),m_cbActionHitAutoJoin);
		switch (m_cbActionHitAutoJoin)
		{
		case ACTION_SHOW_SEARCH_DLG: { break; }
		default: { m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE; }
		}

		//˫���б�
		m_cbActionLeftDoubleList=(BYTE)RegOptionItem.GetValue(TEXT("ActionLeftDoubleList"),m_cbActionLeftDoubleList);
		switch (m_cbActionLeftDoubleList)
		{
		case ACTION_ORIENTATION:
		case ACTION_SHOW_USER_INFO: { break; }
		default: { m_cbActionLeftDoubleList=ACTION_SEND_WHISPER; }
		}

		//��ɫ����
		m_crChatTX=RegOptionItem.GetValue(TEXT("ChatTX"),m_crChatTX);
		m_crWhisperTX=RegOptionItem.GetValue(TEXT("WhisperTX"),m_crWhisperTX);

		//��������
		m_bLimitDetest=RegOptionItem.GetValue(TEXT("LimitDetest"),m_bLimitDetest)?true:false;
		m_bLimitSameIP=RegOptionItem.GetValue(TEXT("LimitSameIP"),m_bLimitSameIP)?true:false;
		m_bNotifyUserInOut=RegOptionItem.GetValue(TEXT("NotifyUserInOut"),m_bNotifyUserInOut)?true:false;
		m_bNotifyFriendCome=RegOptionItem.GetValue(TEXT("NotifyFriendCome"),m_bNotifyFriendCome)?true:false;

		//ϵͳ����
		m_bAllowSound=RegOptionItem.GetValue(TEXT("AllowSound"),m_bAllowSound)?true:false;
		m_bAutoSitDown=RegOptionItem.GetValue(TEXT("AutoSitDown"),m_bAutoSitDown)?true:false;
		m_bAutoShowWhisper=RegOptionItem.GetValue(TEXT("AutoShowWhisper"),m_bAutoShowWhisper)?true:false;
		m_bSaveWhisperChat=RegOptionItem.GetValue(TEXT("SaveWhisperChat"),m_bSaveWhisperChat)?true:false;
		m_bSendWhisperByEnter=RegOptionItem.GetValue(TEXT("SendWhisperByEnter"),m_bSendWhisperByEnter)?true:false;

		//�ȼ�����
		m_wBossHotKey=(WORD)RegOptionItem.GetValue(TEXT("BossHotKey"),m_wBossHotKey);
		m_wWhisperHotKey=(WORD)RegOptionItem.GetValue(TEXT("WhisperHotKey"),m_wWhisperHotKey);
	}

	//������Ϣ
	if (RegOptionItem.OpenRegKey(REG_PROXY_SERVER_INFO,false)==true)
	{
		//���ò���
		m_ProxyServer.szUserName[0]=0;
		m_ProxyServer.szPassword[0]=0;
		m_ProxyServer.szProxyServer[0]=0;

		//��ȡ����
		m_cbProxyType=(BYTE)RegOptionItem.GetValue(TEXT("ProxyType"),PROXY_NONE);
		m_ProxyServer.wProxyPort=(WORD)RegOptionItem.GetValue(TEXT("ProxyPort"),0);
		RegOptionItem.GetString(TEXT("UserName"),m_ProxyServer.szUserName,CountArray(m_ProxyServer.szUserName));
		RegOptionItem.GetString(TEXT("Password"),m_ProxyServer.szPassword,CountArray(m_ProxyServer.szPassword));
		RegOptionItem.GetString(TEXT("ProxyServer"),m_ProxyServer.szProxyServer,CountArray(m_ProxyServer.szProxyServer));

		//��������
		switch (m_cbProxyType)
		{
		case PROXY_NONE:
		case PROXY_HTTP:
		case PROXY_SOCKS4:
		case PROXY_SOCKS5: { break; }
		default: { m_cbProxyType=PROXY_NONE; break; }
		}
	}

	return;
}

//�������
VOID CParameterGlobal::SaveParameter()
{
	//��������
	CWHRegKey RegOptionItem;
	POSITION PositionGame=m_ParameterGameMap.GetStartPosition();

	//������Ϣ
	if (RegOptionItem.OpenRegKey(REG_INTERFACE_INFO,true)==true)
	{
		//��������
		RegOptionItem.WriteValue(TEXT("SalienceTable"),m_bSalienceTable);
		RegOptionItem.WriteValue(TEXT("FullScreenBrowse"),m_bFullScreenBrowse);

		//������Դ
		RegOptionItem.WriteValue(TEXT("RenderColorH"),m_SkinRenderInfo.wColorH);
		RegOptionItem.WriteValue(TEXT("RenderColorS"),(DWORD)(m_SkinRenderInfo.dRectifyS*100+100));
		RegOptionItem.WriteValue(TEXT("RenderColorB"),(DWORD)(m_SkinRenderInfo.dRectifyB*100+100));
	}

	//���Ʊ���
	if (RegOptionItem.OpenRegKey(REG_GOBAL_OPTION,true)==true)
	{
		//��ɫ����
		RegOptionItem.WriteValue(TEXT("ChatTX"),m_crChatTX);
		RegOptionItem.WriteValue(TEXT("WhisperTX"),m_crWhisperTX);

		//��¼����
		RegOptionItem.WriteValue(TEXT("LogonAuto"),m_bLogonAuto);
		RegOptionItem.WriteValue(TEXT("WriteCookie"),m_bWriteCookie);

		//ģʽ����
		RegOptionItem.WriteValue(TEXT("InviteMode"),m_cbInviteMode);
		RegOptionItem.WriteValue(TEXT("MessageMode"),m_cbMessageMode);
		RegOptionItem.WriteValue(TEXT("ActionHitAutoJoin"),m_cbActionHitAutoJoin);
		RegOptionItem.WriteValue(TEXT("ActionLeftDoubleList"),m_cbActionLeftDoubleList);

		//��������
		RegOptionItem.WriteValue(TEXT("LimitDetest"),m_bLimitDetest);
		RegOptionItem.WriteValue(TEXT("LimitSameIP"),m_bLimitSameIP);
		RegOptionItem.WriteValue(TEXT("NotifyUserInOut"),m_bNotifyUserInOut);
		RegOptionItem.WriteValue(TEXT("NotifyFriendCome"),m_bNotifyFriendCome);

		//ϵͳ����
		RegOptionItem.WriteValue(TEXT("AllowSound"),m_bAllowSound);
		RegOptionItem.WriteValue(TEXT("AutoSitDown"),m_bAutoSitDown);
		RegOptionItem.WriteValue(TEXT("AutoShowWhisper"),m_bAutoShowWhisper);
		RegOptionItem.WriteValue(TEXT("SaveWhisperChat"),m_bSaveWhisperChat);
		RegOptionItem.WriteValue(TEXT("SendWhisperByEnter"),m_bSendWhisperByEnter);

		//�ȼ�����
		RegOptionItem.WriteValue(TEXT("BossHotKey"),m_wBossHotKey);
		RegOptionItem.WriteValue(TEXT("WhisperHotKey"),m_wWhisperHotKey);
	}

	//������Ϣ
	if (RegOptionItem.OpenRegKey(REG_PROXY_SERVER_INFO,true)==true)
	{
		RegOptionItem.WriteValue(TEXT("ProxyType"),m_cbProxyType);
		RegOptionItem.WriteValue(TEXT("ProxyPort"),m_ProxyServer.wProxyPort);
		RegOptionItem.WriteString(TEXT("UserName"),m_ProxyServer.szUserName);
		RegOptionItem.WriteString(TEXT("Password"),m_ProxyServer.szPassword);
		RegOptionItem.WriteString(TEXT("ProxyServer"),m_ProxyServer.szProxyServer);
	}

	//��Ϸ����
	while (PositionGame!=NULL)
	{
		//��ȡ����
		WORD wKindID=0;
		CParameterGame * pParameterGame=NULL;
		m_ParameterGameMap.GetNextAssoc(PositionGame,wKindID,pParameterGame);

		//��������
		ASSERT(pParameterGame!=NULL);
		if (pParameterGame!=NULL) pParameterGame->SaveParameter();
	}

	return;
}

//Ĭ�ϲ���
VOID CParameterGlobal::DefaultParameter()
{
	//��¼����
	m_bLogonAuto=false;
	m_bWriteCookie=true;
	m_bRemberPassword=false;

	//ʱ������
	m_wIntermitTime=0L;
	m_wOnLineCountTime=0L;

	//��������
	m_cbInviteMode=INVITE_MODE_ALL;
	m_cbMessageMode=MESSAGE_MODE_ALL;
	m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;
	m_cbActionLeftDoubleList=ACTION_SEND_WHISPER;

	//��������
	m_bLimitDetest=true;
	m_bLimitSameIP=true;
	m_bNotifyUserInOut=false;
	m_bNotifyFriendCome=true;

	//��������
	m_bSalienceTable=true;
	m_bFullScreenBrowse=false;

	//ϵͳ����
	m_bAllowSound=true;
	m_bAutoSitDown=true;
	m_bSaveWhisperChat=true;
	m_bAutoShowWhisper=false;
	m_bSendWhisperByEnter=true;

	//��ɫ����
	m_crChatTX=RGB(0,0,0);
	m_crWhisperTX=RGB(0,0,0);
	m_SkinRenderInfo.wColorH=DEF_COLOR_H;
	m_SkinRenderInfo.dRectifyS=DEF_COLOR_S;
	m_SkinRenderInfo.dRectifyB=DEF_COLOR_B;

	//�ȼ�����
	m_wWhisperHotKey=MAKEWORD(VK_F11,0);
	m_wBossHotKey=MAKEWORD(VK_F12,HOTKEYF_CONTROL);

	//��������
	m_cbProxyType=PROXY_NONE;
	ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

	return;
}

//��Ϸ����
CParameterGame * CParameterGlobal::GetParameterGame(tagGameKind * pGameKind)
{
	//Ѱ���ִ�
	CParameterGame * pParameterGame=NULL;
	if (m_ParameterGameMap.Lookup(pGameKind->wKindID,pParameterGame)==TRUE) return pParameterGame;

	//��������
	try
	{
		//��������
		pParameterGame=new CParameterGame;
		if (pParameterGame==NULL) return NULL;

		//���ö���
		pParameterGame->InitParameter(pGameKind->szProcessName);

		//���ز���
		pParameterGame->LoadParameter();

		//��������
		m_ParameterGameMap[pGameKind->wKindID]=pParameterGame;

		return pParameterGame;
	}
	catch (...) {}

	return NULL;
}

//��������
CParameterServer * CParameterGlobal::GetParameterServer(tagGameServer * pGameServer)
{
	//Ѱ���ִ�
	CParameterServer * pParameterServer=NULL;
	if (m_ParameterServerMap.Lookup(pGameServer->wServerID,pParameterServer)==TRUE) return pParameterServer;

	//��������
	try
	{
		//��������
		pParameterServer=new CParameterServer;
		if (pParameterServer==NULL) return NULL;

		//��������
		m_ParameterServerMap[pGameServer->wServerID]=pParameterServer;

		return pParameterServer;
	}
	catch (...) {}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
