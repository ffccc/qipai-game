#ifndef SKIN_RESOURCE_MANAGER_HEAD_FILE
#define SKIN_RESOURCE_MANAGER_HEAD_FILE

#pragma once

#include "SkinControlHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//资源管理
class SKIN_CONTROL_CLASS CSkinResourceManager
{
	//窗口资源
protected:

	//变量定义
protected:
	CFont							m_DefaultFont;						//默认字体

	//静态变量
protected:
	static CSkinResourceManager *	m_pSkinResourceManager;				//对象指针

	//函数定义
public:
	//构造函数
	CSkinResourceManager();
	//析构函数
	virtual ~CSkinResourceManager();

	//资源函数
public:
	//默认字体
	CFont & GetDefaultFont() { return m_DefaultFont; }

	//设置资源
public:
	//设置资源
	VOID SetSkinResource(const tagSkinRenderInfo & SkinRenderInfo);

	//静态函数
public:
	//枚举函数
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	//获取实例
	static CSkinResourceManager * GetInstance() { return m_pSkinResourceManager; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif
