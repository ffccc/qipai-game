#pragma once

//系统文件
#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "PlatformEvent.h"
#include "ServerListData.h"

//其他文件
#include "Other\IDispimp.h"
#include "Other\Custsite.h"

//////////////////////////////////////////////////////////////////////////////////

//应用程序
class CGamePlazaApp : public CWinApp
{
	//变量定义
public:
	CImpIDispatch *					m_pIDispatch;						//自动接口
	CCustomOccManager *				m_pCustomOccManager;				//管理对象

	//平台数据
protected:
	CGlobalUnits					m_GlobalUnits;						//全局单元
	CGlobalUserInfo					m_GlobalUserInfo;					//用户信息

	//全局组件
protected:
	CPlatformEvent					m_PlatformEvent;					//平台事件
	CParameterGlobal				m_ParameterGlobal;					//全局配置
	CUserItemElement				m_UserItemElement;					//用户元素
	CUserInformation				m_UserInformation;					//用户信息
	CExpressionManager				m_ExpressionManager;				//聊天表情
	CCustomFaceManager				m_CustomFaceManager;				//头像管理
	CGamePropertyManager			m_GamePropertyManager;				//道具管理

	//资源组件
public:
	CSkinRenderManager				m_SkinRenderManager;				//渲染管理
	CSkinResourceManager			m_SkinResourceManager;				//资源管理

	//组件变量
public:
	CFaceItemControlHelper			m_FaceItemControlModule;			//头像组件
	CUserOrderParserHelper			m_UserOrderParserModule;			//等级组件

	//函数定义
public:
	//构造函数
	CGamePlazaApp();

	//重载函数
public:
	//退出函数
	virtual INT ExitInstance();
	//进入函数
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//程序对象
extern CGamePlazaApp theApp;

//////////////////////////////////////////////////////////////////////////////////
