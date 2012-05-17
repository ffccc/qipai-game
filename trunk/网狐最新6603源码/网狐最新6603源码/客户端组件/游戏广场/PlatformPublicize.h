#ifndef PLATFORM_PUBLICIZE_HEAD_FILE
#define PLATFORM_PUBLICIZE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//框架广告
class CPlatformPublicize : public CWebPublicize
{
	//变量定义
protected:
	COLORREF						m_crBoradColor;						//边框颜色

	//函数定义
public:
	//构造函数
	CPlatformPublicize();
	//析构函数
	virtual ~CPlatformPublicize();

	//重置函数
protected:
	//位置函数
	virtual VOID GetWebBrowserRect(CRect & rcWebBrowser);
	//绘画界面
	virtual VOID DrawPublicizeView(CDC * pDC, INT nWidth, INT nHeight, BYTE cbStatus);

	//功能函数
public:
	//设置边框
	VOID SetBoradColor(COLORREF crBoradColor);
};

//////////////////////////////////////////////////////////////////////////////////

#endif