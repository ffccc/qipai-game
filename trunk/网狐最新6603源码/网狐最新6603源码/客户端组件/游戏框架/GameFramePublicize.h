#ifndef GAME_FRAME_PUBLICIZE_HEAD_FILE
#define GAME_FRAME_PUBLICIZE_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//框架广告
class GAME_FRAME_CLASS CGameFramePublicize : public CWebPublicize
{
	//变量定义
protected:
	COLORREF						m_crBoradColor;						//边框颜色

	//函数定义
public:
	//构造函数
	CGameFramePublicize();
	//析构函数
	virtual ~CGameFramePublicize();

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