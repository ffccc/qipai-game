#ifndef SKIN_SCROLL_BAR_HEAD_FILE
#define SKIN_SCROLL_BAR_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//滚动资源
class SKIN_CONTROL_CLASS CSkinScrollBarAttribute
{
	//资源变量
public:
	CBitImage						m_ImageScroll;						//滚动资源

	//函数定义
public:
	//构造函数
	CSkinScrollBarAttribute();
	//析构函数
	virtual ~CSkinScrollBarAttribute();

	//管理函数
public:
	//配置资源
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//滚动条类
class SKIN_CONTROL_CLASS CSkinScrollBar
{
	//资源变量
public:
	static CSkinScrollBarAttribute	m_SkinAttribute;					//资源变量

	//函数定义
public:
	//构造函数
	CSkinScrollBar();
	//析构函数
	virtual ~CSkinScrollBar();

	//功能函数
public:
	//配置滚动
	VOID InitScroolBar(CWnd * pWnd);
};

//////////////////////////////////////////////////////////////////////////////////

#endif