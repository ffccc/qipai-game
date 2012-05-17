#ifndef GLOBAL_UNITS_HEAD_FILE
#define GLOBAL_UNITS_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "GameFrameHead.h"
#include "GameFrameEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//全局组件
#define MODULE_CLIENT_KERNEL		1									//游戏内核
#define MODULE_STRING_MESSAGE		2									//
#define MODULE_FACE_ITEM_CONTROL	3									//头像组件
#define MODULE_GAME_LEVEL_PARSER	4									//等级描述
#define MODULE_GAME_USER_MANAGER	5									//用户管理

//框架组件
#define MODULE_GAME_FRAME_WND		10									//游戏框架
#define MODULE_GAME_FRAME_VIEW		11									//框架视图
#define MODULE_GAME_FRAME_ENGINE	12									//框架引擎
#define MODULE_GAME_FRAME_SERVICE	13									//框架服务
#define MODULE_GAME_FRAME_CONTROL	14									//控制框架
#define MODULE_GAME_FRAME_TOOL_BAR	15									//工具框架

//模块索引
typedef CMap<WORD,WORD,IUnknownEx *,IUnknownEx * &> CGlobalModuleMap;

//////////////////////////////////////////////////////////////////////////////////

//全局单元
class GAME_FRAME_CLASS CGlobalUnits
{
	//界面配置
public:
	bool							m_bAllowSound;						//允许声音
	bool							m_bFullGameView;					//全屏模式
	bool							m_bNotifyUserInOut;					//进出消息

	//旁观配置
public:
	bool							m_bAllowLookon;						//允许旁观
	bool							m_bAllowFriendLookon;				//允许旁观

	//模式变量
public:
	BYTE							m_cbMessageMode;					//消息模式

	//界面配置
public:
	COLORREF						m_crChatTX;							//聊天文字
	tagSkinRenderInfo				m_SkinRenderInfo;					//渲染颜色

	//服务组件
public:
	CClientKernelHelper				m_ClientKernelModule;				//游戏内核
	CFaceItemControlHelper			m_FaceItemControlModule;			//头像组件
	CUserOrderParserHelper			m_UserOrderParserModule;			//用户等级
	CGameLevelParserHelper			m_GameLevelParserModule;			//游戏等级

	//组件变量
public:
	CGlobalModuleMap				m_GlobalModuleMap;					//模块索引
	CUserItemElement				m_UserItemElement;					//用户元素
	CExpressionManager				m_ExpressionManager;				//表情管理
	CGamePropertyManager			m_GamePropertyManager;				//道具管理

	//资源组件
public:
	CSkinRenderManager				m_SkinRenderManager;				//渲染管理
	CSkinResourceManager			m_SkinResourceManager;				//资源管理

	//静态变量
protected:
	static CGlobalUnits *			m_pGlobalUnits;						//全局单元

	//函数定义
public:
	//构造函数
	CGlobalUnits();
	//析构函数
	virtual ~CGlobalUnits();

	//组件配置
public:
	//创建组件
	bool InitGlobalUnits();
	//注销组件
	bool UnInitGlobalUnits();

	//配置函数
public:
	//加载参数
	VOID LoadParameter();
	//保存参数
	VOID SaveParameter();
	//默认参数
	VOID DefaultParameter();

	//声音函数
public:
	//播放声音
	bool PlayGameSound(LPCTSTR pszSoundName);
	//播放声音
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//组件管理
public:
	//注册组件
	bool RegisterGlobalModule(WORD wModuleID, IUnknownEx * pIUnknownEx);
	//查询组件
	VOID * QueryGlobalModule(WORD wModuleID, REFGUID Guid, DWORD dwQueryVer);

	//静态函数
public:
	//获取实例
	static CGlobalUnits * GetInstance() { return m_pGlobalUnits; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif