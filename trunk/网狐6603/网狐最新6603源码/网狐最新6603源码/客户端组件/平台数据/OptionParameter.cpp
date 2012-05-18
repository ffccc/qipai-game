#include "StdAfx.h"
#include "OptionParameter.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CParameterGlobal * CParameterGlobal::m_pParameterGlobal=NULL;			//全局配置

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CParameterGame::CParameterGame()
{
	//默认参数
	DefaultParameter();

	//属性变量
	ZeroMemory(m_szRegKeyName,sizeof(m_szRegKeyName));

	return;
}

//析构函数
CParameterGame::~CParameterGame()
{
}

//加载参数
VOID CParameterGame::LoadParameter()
{
	//变量定义
	CWHRegKey RegOptionItem;

	//配置表项
	if (RegOptionItem.OpenRegKey(m_szRegKeyName,REG_GAME_OPTION,false)==true)
	{
		//胜率限制
		m_wMinWinRate=(WORD)RegOptionItem.GetValue(TEXT("MinWinRate"),m_wMinWinRate);
		m_bLimitWinRate=RegOptionItem.GetValue(TEXT("LimitWinRate"),m_bLimitWinRate)?true:false;

		//逃率限制
		m_wMaxFleeRate=(WORD)RegOptionItem.GetValue(TEXT("MaxFleeRate"),m_wMaxFleeRate);
		m_bLimitFleeRate=RegOptionItem.GetValue(TEXT("LimitFleeRate"),m_bLimitFleeRate)?true:false;
		
		//积分限制
		m_lMaxGameScore=RegOptionItem.GetValue(TEXT("MaxGameScore"),m_lMaxGameScore);
		m_lMinGameScore=RegOptionItem.GetValue(TEXT("MinGameScore"),m_lMinGameScore);
		m_bLimitGameScore=RegOptionItem.GetValue(TEXT("LimitGameScore"),m_bLimitGameScore)?true:false;
	}

	return;
}

//保存参数
VOID CParameterGame::SaveParameter()
{
	//变量定义
	CWHRegKey RegOptionItem;

	//配置表项
	if (RegOptionItem.OpenRegKey(m_szRegKeyName,REG_GAME_OPTION,true)==true)
	{
		//胜率限制
		RegOptionItem.WriteValue(TEXT("MinWinRate"),m_wMinWinRate);
		RegOptionItem.WriteValue(TEXT("LimitWinRate"),m_bLimitWinRate);

		//逃率限制
		RegOptionItem.WriteValue(TEXT("MaxFleeRate"),m_wMaxFleeRate);
		RegOptionItem.WriteValue(TEXT("LimitFleeRate"),m_bLimitFleeRate);

		//积分限制
		RegOptionItem.WriteValue(TEXT("MaxGameScore"),m_lMaxGameScore);
		RegOptionItem.WriteValue(TEXT("MinGameScore"),m_lMinGameScore);
		RegOptionItem.WriteValue(TEXT("LimitGameScore"),m_bLimitGameScore);
	}

	return;
}

//默认参数
VOID CParameterGame::DefaultParameter()
{
	//胜率限制
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//逃率限制
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//积分限制
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	return;
}

//配置参数
bool CParameterGame::InitParameter(LPCTSTR pszProcessName)
{
	//构造键名
	WORD wNameIndex=0;
	while (wNameIndex<(CountArray(m_szRegKeyName)-1))
	{
		//终止判断
		if (pszProcessName[wNameIndex]==0) break;
		if (pszProcessName[wNameIndex]==TEXT('.')) break;

		//设置字符
		WORD wCurrentIndex=wNameIndex++;
		m_szRegKeyName[wCurrentIndex]=pszProcessName[wCurrentIndex];
	}

	//设置变量
	m_szRegKeyName[wNameIndex]=0;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CParameterServer::CParameterServer()
{
	//默认参数
	DefaultParameter();

	return;
}

//析构函数
CParameterServer::~CParameterServer()
{
}

//默认参数
VOID CParameterServer::DefaultParameter()
{
	//配置变量
	m_bTakePassword=false;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CParameterGlobal::CParameterGlobal()
{
	//默认参数
	DefaultParameter();

	//设置对象
	ASSERT(m_pParameterGlobal==NULL);
	if (m_pParameterGlobal==NULL) m_pParameterGlobal=this;

	return;
}

//析构函数
CParameterGlobal::~CParameterGlobal()
{
	//变量定义
	POSITION PositionGame=m_ParameterGameMap.GetStartPosition();
	POSITION PositionServer=m_ParameterGameMap.GetStartPosition();

	//删除配置
	while (PositionGame!=NULL)
	{
		//获取子项
		WORD wKindID=0;
		CParameterGame * pParameterGame=NULL;
		m_ParameterGameMap.GetNextAssoc(PositionGame,wKindID,pParameterGame);

		//删除配置
		SafeDelete(pParameterGame);
	}

	//删除配置
	while (PositionServer!=NULL)
	{
		//获取子项
		WORD wServerID=0;
		CParameterServer * pParameterServer=NULL;
		m_ParameterServerMap.GetNextAssoc(PositionServer,wServerID,pParameterServer);

		//删除配置
		SafeDelete(pParameterServer);
	}

	//删除索引
	m_ParameterGameMap.RemoveAll();
	m_ParameterServerMap.RemoveAll();

	//释放对象
	ASSERT(m_pParameterGlobal==this);
	if (m_pParameterGlobal==this) m_pParameterGlobal=NULL;

	return;
}

//加载参数
VOID CParameterGlobal::LoadParameter()
{
	//变量定义
	CWHRegKey RegOptionItem;

	//界面信息
	if (RegOptionItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		//界面配置
		m_bSalienceTable=RegOptionItem.GetValue(TEXT("SalienceTable"),m_bSalienceTable)?true:false;
		m_bFullScreenBrowse=RegOptionItem.GetValue(TEXT("FullScreenBrowse"),m_bFullScreenBrowse)?true:false;

		//界面资源
		m_SkinRenderInfo.wColorH=(WORD)RegOptionItem.GetValue(TEXT("RenderColorH"),m_SkinRenderInfo.wColorH);
		m_SkinRenderInfo.dRectifyS=(DOUBLE)(RegOptionItem.GetValue(TEXT("RenderColorS"),(INT)((m_SkinRenderInfo.dRectifyS+1.0)*100)))/100.0-1.0;
		m_SkinRenderInfo.dRectifyB=(DOUBLE)(RegOptionItem.GetValue(TEXT("RenderColorB"),(INT)((m_SkinRenderInfo.dRectifyB+1.0)*100)))/100.0-1.0;
	}

	//控制变量
	if (RegOptionItem.OpenRegKey(REG_GOBAL_OPTION,false)==true)
	{
		//自动登录
		m_bLogonAuto=RegOptionItem.GetValue(TEXT("LogonAuto"),m_bLogonAuto)?true:false;
		m_bWriteCookie=RegOptionItem.GetValue(TEXT("WriteCookie"),m_bWriteCookie)?true:false;

		//邀请模式
		m_cbInviteMode=(BYTE)RegOptionItem.GetValue(TEXT("InviteMode"),m_cbInviteMode);
		switch (m_cbInviteMode)
		{
		case INVITE_MODE_NONE:
		case INVITE_MODE_FRIEND: { break; }
		default: { m_cbInviteMode=INVITE_MODE_ALL; }
		}

		//消息模式
		m_cbMessageMode=(BYTE)RegOptionItem.GetValue(TEXT("MessageMode"),m_cbMessageMode);
		switch (m_cbMessageMode)
		{
		case MESSAGE_MODE_ALL:
		case MESSAGE_MODE_FRIEND: { break; }
		default: { m_cbMessageMode=MESSAGE_MODE_DETEST; }
		}

		//自动加入
		m_cbActionHitAutoJoin=(BYTE)RegOptionItem.GetValue(TEXT("ActionHitAutoJoin"),m_cbActionHitAutoJoin);
		switch (m_cbActionHitAutoJoin)
		{
		case ACTION_SHOW_SEARCH_DLG: { break; }
		default: { m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE; }
		}

		//双击列表
		m_cbActionLeftDoubleList=(BYTE)RegOptionItem.GetValue(TEXT("ActionLeftDoubleList"),m_cbActionLeftDoubleList);
		switch (m_cbActionLeftDoubleList)
		{
		case ACTION_ORIENTATION:
		case ACTION_SHOW_USER_INFO: { break; }
		default: { m_cbActionLeftDoubleList=ACTION_SEND_WHISPER; }
		}

		//颜色设置
		m_crChatTX=RegOptionItem.GetValue(TEXT("ChatTX"),m_crChatTX);
		m_crWhisperTX=RegOptionItem.GetValue(TEXT("WhisperTX"),m_crWhisperTX);

		//房间配置
		m_bLimitDetest=RegOptionItem.GetValue(TEXT("LimitDetest"),m_bLimitDetest)?true:false;
		m_bLimitSameIP=RegOptionItem.GetValue(TEXT("LimitSameIP"),m_bLimitSameIP)?true:false;
		m_bNotifyUserInOut=RegOptionItem.GetValue(TEXT("NotifyUserInOut"),m_bNotifyUserInOut)?true:false;
		m_bNotifyFriendCome=RegOptionItem.GetValue(TEXT("NotifyFriendCome"),m_bNotifyFriendCome)?true:false;

		//系统配置
		m_bAllowSound=RegOptionItem.GetValue(TEXT("AllowSound"),m_bAllowSound)?true:false;
		m_bAutoSitDown=RegOptionItem.GetValue(TEXT("AutoSitDown"),m_bAutoSitDown)?true:false;
		m_bAutoShowWhisper=RegOptionItem.GetValue(TEXT("AutoShowWhisper"),m_bAutoShowWhisper)?true:false;
		m_bSaveWhisperChat=RegOptionItem.GetValue(TEXT("SaveWhisperChat"),m_bSaveWhisperChat)?true:false;
		m_bSendWhisperByEnter=RegOptionItem.GetValue(TEXT("SendWhisperByEnter"),m_bSendWhisperByEnter)?true:false;

		//热键设置
		m_wBossHotKey=(WORD)RegOptionItem.GetValue(TEXT("BossHotKey"),m_wBossHotKey);
		m_wWhisperHotKey=(WORD)RegOptionItem.GetValue(TEXT("WhisperHotKey"),m_wWhisperHotKey);
	}

	//代理信息
	if (RegOptionItem.OpenRegKey(REG_PROXY_SERVER_INFO,false)==true)
	{
		//重置参数
		m_ProxyServer.szUserName[0]=0;
		m_ProxyServer.szPassword[0]=0;
		m_ProxyServer.szProxyServer[0]=0;

		//读取参数
		m_cbProxyType=(BYTE)RegOptionItem.GetValue(TEXT("ProxyType"),PROXY_NONE);
		m_ProxyServer.wProxyPort=(WORD)RegOptionItem.GetValue(TEXT("ProxyPort"),0);
		RegOptionItem.GetString(TEXT("UserName"),m_ProxyServer.szUserName,CountArray(m_ProxyServer.szUserName));
		RegOptionItem.GetString(TEXT("Password"),m_ProxyServer.szPassword,CountArray(m_ProxyServer.szPassword));
		RegOptionItem.GetString(TEXT("ProxyServer"),m_ProxyServer.szProxyServer,CountArray(m_ProxyServer.szProxyServer));

		//代理类型
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

//保存参数
VOID CParameterGlobal::SaveParameter()
{
	//变量定义
	CWHRegKey RegOptionItem;
	POSITION PositionGame=m_ParameterGameMap.GetStartPosition();

	//界面信息
	if (RegOptionItem.OpenRegKey(REG_INTERFACE_INFO,true)==true)
	{
		//界面配置
		RegOptionItem.WriteValue(TEXT("SalienceTable"),m_bSalienceTable);
		RegOptionItem.WriteValue(TEXT("FullScreenBrowse"),m_bFullScreenBrowse);

		//界面资源
		RegOptionItem.WriteValue(TEXT("RenderColorH"),m_SkinRenderInfo.wColorH);
		RegOptionItem.WriteValue(TEXT("RenderColorS"),(DWORD)(m_SkinRenderInfo.dRectifyS*100+100));
		RegOptionItem.WriteValue(TEXT("RenderColorB"),(DWORD)(m_SkinRenderInfo.dRectifyB*100+100));
	}

	//控制变量
	if (RegOptionItem.OpenRegKey(REG_GOBAL_OPTION,true)==true)
	{
		//颜色设置
		RegOptionItem.WriteValue(TEXT("ChatTX"),m_crChatTX);
		RegOptionItem.WriteValue(TEXT("WhisperTX"),m_crWhisperTX);

		//登录配置
		RegOptionItem.WriteValue(TEXT("LogonAuto"),m_bLogonAuto);
		RegOptionItem.WriteValue(TEXT("WriteCookie"),m_bWriteCookie);

		//模式配置
		RegOptionItem.WriteValue(TEXT("InviteMode"),m_cbInviteMode);
		RegOptionItem.WriteValue(TEXT("MessageMode"),m_cbMessageMode);
		RegOptionItem.WriteValue(TEXT("ActionHitAutoJoin"),m_cbActionHitAutoJoin);
		RegOptionItem.WriteValue(TEXT("ActionLeftDoubleList"),m_cbActionLeftDoubleList);

		//房间配置
		RegOptionItem.WriteValue(TEXT("LimitDetest"),m_bLimitDetest);
		RegOptionItem.WriteValue(TEXT("LimitSameIP"),m_bLimitSameIP);
		RegOptionItem.WriteValue(TEXT("NotifyUserInOut"),m_bNotifyUserInOut);
		RegOptionItem.WriteValue(TEXT("NotifyFriendCome"),m_bNotifyFriendCome);

		//系统配置
		RegOptionItem.WriteValue(TEXT("AllowSound"),m_bAllowSound);
		RegOptionItem.WriteValue(TEXT("AutoSitDown"),m_bAutoSitDown);
		RegOptionItem.WriteValue(TEXT("AutoShowWhisper"),m_bAutoShowWhisper);
		RegOptionItem.WriteValue(TEXT("SaveWhisperChat"),m_bSaveWhisperChat);
		RegOptionItem.WriteValue(TEXT("SendWhisperByEnter"),m_bSendWhisperByEnter);

		//热键设置
		RegOptionItem.WriteValue(TEXT("BossHotKey"),m_wBossHotKey);
		RegOptionItem.WriteValue(TEXT("WhisperHotKey"),m_wWhisperHotKey);
	}

	//代理信息
	if (RegOptionItem.OpenRegKey(REG_PROXY_SERVER_INFO,true)==true)
	{
		RegOptionItem.WriteValue(TEXT("ProxyType"),m_cbProxyType);
		RegOptionItem.WriteValue(TEXT("ProxyPort"),m_ProxyServer.wProxyPort);
		RegOptionItem.WriteString(TEXT("UserName"),m_ProxyServer.szUserName);
		RegOptionItem.WriteString(TEXT("Password"),m_ProxyServer.szPassword);
		RegOptionItem.WriteString(TEXT("ProxyServer"),m_ProxyServer.szProxyServer);
	}

	//游戏配置
	while (PositionGame!=NULL)
	{
		//获取子项
		WORD wKindID=0;
		CParameterGame * pParameterGame=NULL;
		m_ParameterGameMap.GetNextAssoc(PositionGame,wKindID,pParameterGame);

		//保存配置
		ASSERT(pParameterGame!=NULL);
		if (pParameterGame!=NULL) pParameterGame->SaveParameter();
	}

	return;
}

//默认参数
VOID CParameterGlobal::DefaultParameter()
{
	//登录配置
	m_bLogonAuto=false;
	m_bWriteCookie=true;
	m_bRemberPassword=false;

	//时间配置
	m_wIntermitTime=0L;
	m_wOnLineCountTime=0L;

	//房间配置
	m_cbInviteMode=INVITE_MODE_ALL;
	m_cbMessageMode=MESSAGE_MODE_ALL;
	m_cbActionHitAutoJoin=ACTION_SEARCH_TABLE;
	m_cbActionLeftDoubleList=ACTION_SEND_WHISPER;

	//房间配置
	m_bLimitDetest=true;
	m_bLimitSameIP=true;
	m_bNotifyUserInOut=false;
	m_bNotifyFriendCome=true;

	//界面配置
	m_bSalienceTable=true;
	m_bFullScreenBrowse=false;

	//系统配置
	m_bAllowSound=true;
	m_bAutoSitDown=true;
	m_bSaveWhisperChat=true;
	m_bAutoShowWhisper=false;
	m_bSendWhisperByEnter=true;

	//颜色设置
	m_crChatTX=RGB(0,0,0);
	m_crWhisperTX=RGB(0,0,0);
	m_SkinRenderInfo.wColorH=DEF_COLOR_H;
	m_SkinRenderInfo.dRectifyS=DEF_COLOR_S;
	m_SkinRenderInfo.dRectifyB=DEF_COLOR_B;

	//热键配置
	m_wWhisperHotKey=MAKEWORD(VK_F11,0);
	m_wBossHotKey=MAKEWORD(VK_F12,HOTKEYF_CONTROL);

	//代理配置
	m_cbProxyType=PROXY_NONE;
	ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

	return;
}

//游戏配置
CParameterGame * CParameterGlobal::GetParameterGame(tagGameKind * pGameKind)
{
	//寻找现存
	CParameterGame * pParameterGame=NULL;
	if (m_ParameterGameMap.Lookup(pGameKind->wKindID,pParameterGame)==TRUE) return pParameterGame;

	//创建对象
	try
	{
		//创建对象
		pParameterGame=new CParameterGame;
		if (pParameterGame==NULL) return NULL;

		//配置对象
		pParameterGame->InitParameter(pGameKind->szProcessName);

		//加载参数
		pParameterGame->LoadParameter();

		//设置数组
		m_ParameterGameMap[pGameKind->wKindID]=pParameterGame;

		return pParameterGame;
	}
	catch (...) {}

	return NULL;
}

//房间配置
CParameterServer * CParameterGlobal::GetParameterServer(tagGameServer * pGameServer)
{
	//寻找现存
	CParameterServer * pParameterServer=NULL;
	if (m_ParameterServerMap.Lookup(pGameServer->wServerID,pParameterServer)==TRUE) return pParameterServer;

	//创建对象
	try
	{
		//创建对象
		pParameterServer=new CParameterServer;
		if (pParameterServer==NULL) return NULL;

		//设置数组
		m_ParameterServerMap[pGameServer->wServerID]=pParameterServer;

		return pParameterServer;
	}
	catch (...) {}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
