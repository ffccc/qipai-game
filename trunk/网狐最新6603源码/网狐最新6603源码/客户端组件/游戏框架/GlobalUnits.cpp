#include "StdAfx.h"
#include "Mmsystem.h"
#include "GlobalUnits.h"

//////////////////////////////////////////////////////////////////////////////////

//控件颜色
#define OPTION_CHAT_TX				RGB(0,0,0)							//聊天文字

//静态变量
CGlobalUnits * CGlobalUnits::m_pGlobalUnits=NULL;						//全局单元

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalUnits::CGlobalUnits()
{
	//默认参数
	DefaultParameter();

	//设置对象
	ASSERT(m_pGlobalUnits==NULL);
	if (m_pGlobalUnits==NULL) m_pGlobalUnits=this;

	return;
}

//析构函数
CGlobalUnits::~CGlobalUnits()
{
	//设置对象
	ASSERT(m_pGlobalUnits==this);
	if (m_pGlobalUnits==this) m_pGlobalUnits=NULL;

	//删除组件
	m_ClientKernelModule.CloseInstance();
	m_FaceItemControlModule.CloseInstance();
	m_GameLevelParserModule.CloseInstance();

	return;
}

//创建组件
bool CGlobalUnits::InitGlobalUnits()
{
	//加载参数
	LoadParameter();

	//设置资源
	m_ExpressionManager.LoadExpression();
	m_SkinResourceManager.SetSkinResource(m_SkinRenderInfo);

	//创建组件
	if (m_ClientKernelModule.CreateInstance()==false) return false;
	if (m_FaceItemControlModule.CreateInstance()==false) return false;
	if (m_GameLevelParserModule.CreateInstance()==false) return false;
	if (m_UserOrderParserModule.CreateInstance()==false) return false;

	//注册组件
	RegisterGlobalModule(MODULE_CLIENT_KERNEL,m_ClientKernelModule.GetInterface());
	RegisterGlobalModule(MODULE_FACE_ITEM_CONTROL,m_FaceItemControlModule.GetInterface());
	RegisterGlobalModule(MODULE_GAME_LEVEL_PARSER,m_GameLevelParserModule.GetInterface());

	//用户元素
	m_UserItemElement.SetUserOrderParser(m_UserOrderParserModule.GetInterface());
	m_UserItemElement.SetFaceItemControl(m_FaceItemControlModule.GetInterface());

	//配置内核
	if (m_ClientKernelModule->InitClientKernel(AfxGetApp()->m_lpCmdLine)==false) return false;

	return true;
}

//注销组件
bool CGlobalUnits::UnInitGlobalUnits()
{
	//保存参数
	SaveParameter();

	//销毁组件
	if (m_ClientKernelModule.GetInterface()!=NULL) m_ClientKernelModule.CloseInstance();
	if (m_FaceItemControlModule.GetInterface()!=NULL) m_FaceItemControlModule.CloseInstance();
	if (m_GameLevelParserModule.GetInterface()!=NULL) m_GameLevelParserModule.CloseInstance();

	return true;
}

//加载参数
VOID CGlobalUnits::LoadParameter()
{
	//变量定义
	CWHRegKey RegOptionItem;

	//读取配置
	if (RegOptionItem.OpenRegKey(TEXT("GameFrame"),TEXT("Option"),false)==true)
	{
		//颜色设置
		m_crChatTX=RegOptionItem.GetValue(TEXT("crChatTX"),OPTION_CHAT_TX);

		//界面配置
		m_bAllowSound=RegOptionItem.GetValue(TEXT("AllowSound"),TRUE)?true:false;
		m_bFullGameView=RegOptionItem.GetValue(TEXT("FullGameView"),FALSE)?true:false;
		m_bNotifyUserInOut=RegOptionItem.GetValue(TEXT("NotifyUserInOut"),TRUE)?true:false;

		//旁观选项
		m_bAllowLookon=RegOptionItem.GetValue(TEXT("AllowLookon"),TRUE)?true:false;
		m_bAllowFriendLookon=RegOptionItem.GetValue(TEXT("AllowFriendLookon"),TRUE)?true:false;

		//消息模式
		m_cbMessageMode=(BYTE)RegOptionItem.GetValue(TEXT("MessageMode"),m_cbMessageMode);
		switch (m_cbMessageMode)
		{
		case MESSAGE_MODE_ALL:
		case MESSAGE_MODE_FRIEND: { break; }
		default: { m_cbMessageMode=MESSAGE_MODE_DETEST; }
		}

		//界面资源
		m_SkinRenderInfo.wColorH=(WORD)RegOptionItem.GetValue(TEXT("RenderColorH"),m_SkinRenderInfo.wColorH);
		m_SkinRenderInfo.dRectifyS=(DOUBLE)(RegOptionItem.GetValue(TEXT("RenderColorS"),(INT)((m_SkinRenderInfo.dRectifyS+1.0)*100)))/100.0-1.0;
		m_SkinRenderInfo.dRectifyB=(DOUBLE)(RegOptionItem.GetValue(TEXT("RenderColorB"),(INT)((m_SkinRenderInfo.dRectifyB+1.0)*100)))/100.0-1.0;
	}

	return;
}

//保存参数
VOID CGlobalUnits::SaveParameter()
{
	//变量定义
	CWHRegKey RegOptionItem;

	//保存配置
	if (RegOptionItem.OpenRegKey(TEXT("GameFrame"),TEXT("Option"),true)==true)
	{
		//颜色配置
		RegOptionItem.WriteValue(TEXT("crChatTX"),m_crChatTX);

		//模式变量
		RegOptionItem.WriteValue(TEXT("MessageMode"),m_cbMessageMode);

		//界面配置
		RegOptionItem.WriteValue(TEXT("AllowSound"),m_bAllowSound);
		RegOptionItem.WriteValue(TEXT("FullGameView"),m_bFullGameView);
		RegOptionItem.WriteValue(TEXT("NotifyUserInOut"),m_bNotifyUserInOut);

		//旁观配置
		RegOptionItem.WriteValue(TEXT("AllowLookon"),m_bAllowLookon);
		RegOptionItem.WriteValue(TEXT("AllowFriendLookon"),m_bAllowFriendLookon);

		//界面资源
		RegOptionItem.WriteValue(TEXT("RenderColorH"),m_SkinRenderInfo.wColorH);
		RegOptionItem.WriteValue(TEXT("RenderColorS"),(DWORD)(m_SkinRenderInfo.dRectifyS*100+100));
		RegOptionItem.WriteValue(TEXT("RenderColorB"),(DWORD)(m_SkinRenderInfo.dRectifyB*100+100));
	}

	return;
}

//默认参数
VOID CGlobalUnits::DefaultParameter()
{
	//其他选项
	m_bAllowSound=true;
	m_bFullGameView=false;
	m_bNotifyUserInOut=true;

	//旁观选项
	m_bAllowLookon=false;
	m_bAllowFriendLookon=true;

	//颜色设置
	m_crChatTX=OPTION_CHAT_TX;

	//模式变量
	m_cbMessageMode=MESSAGE_MODE_DETEST;

	//界面资源
	m_SkinRenderInfo.wColorH=DEF_COLOR_H;
	m_SkinRenderInfo.dRectifyS=DEF_COLOR_S;
	m_SkinRenderInfo.dRectifyB=DEF_COLOR_B;

	return;
}

//播放声音
bool CGlobalUnits::PlayGameSound(LPCTSTR pszSoundName)
{
	//播放判断
	if (m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//获取对象
	CD3DSound * pD3DSound=CD3DSound::GetInstance();

	//播放声音
	if ((pD3DSound->PlaySound(pszSoundName,false))==false)
	{
		//错误断言
		ASSERT(FALSE);

		//默认播放
		PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);
	}

	return true;
}

//播放声音
bool CGlobalUnits::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//播放判断
	if (m_bAllowSound==false) return false;
	if (AfxGetMainWnd()->IsWindowVisible()==FALSE) return false;

	//获取对象
	CD3DSound * pD3DSound=CD3DSound::GetInstance();

	//播放声音
	if ((pD3DSound->PlaySound(hInstance,pszSoundName,TEXT("WAVE"),false))==false)
	{
		//错误断言
		ASSERT(FALSE);

		//加载资源
		HRSRC hResource=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
		if (hResource==NULL) return false;

		//锁定资源
		HGLOBAL hGlobal=LoadResource(hInstance,hResource);
		if (hGlobal==NULL) return false;

		//播放声音
		LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
		PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

		//清理资源
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}

	return true;
}

//注册组件
bool CGlobalUnits::RegisterGlobalModule(WORD wModuleID, IUnknownEx * pIUnknownEx)
{
	//效验参数
	ASSERT((wModuleID!=0L)&&(pIUnknownEx!=NULL));
	if ((wModuleID==0L)||(pIUnknownEx==NULL)) return false;

	//搜索组件
	IUnknownEx * pIUnknownExMap=NULL;
	if (m_GlobalModuleMap.Lookup(wModuleID,pIUnknownExMap)==TRUE) 
	{
		ASSERT(FALSE);
		return false;
	}

	//注册组件
	m_GlobalModuleMap[wModuleID]=pIUnknownEx;

	return true;
}

//查询组件
VOID * CGlobalUnits::QueryGlobalModule(WORD wModuleID, REFGUID Guid, DWORD dwQueryVer)
{
	//搜索组件
	IUnknownEx * pIUnknownEx=NULL;
	if (m_GlobalModuleMap.Lookup(wModuleID,pIUnknownEx)==FALSE)
	{
		ASSERT(FALSE);
		return NULL;
	}

	//查询接口
	return pIUnknownEx->QueryInterface(Guid,dwQueryVer);
}

//////////////////////////////////////////////////////////////////////////////////

//全局单元
CGlobalUnits						g_GlobalUnits;						//全局单元

//////////////////////////////////////////////////////////////////////////////////
