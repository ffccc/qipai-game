#ifndef GAME_FRAME_VIEW_GDI_HEAD_FILE
#define GAME_FRAME_VIEW_GDI_HEAD_FILE

#pragma once

#include "GameFrameView.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//视图框架
class GAME_FRAME_CLASS CGameFrameViewGDI : public CGameFrameView
{
	//缓冲变量
private:
	CSize							m_SizeBackSurface;					//表面大小
	CRect							m_RectDirtySurface;					//重画区域
	CImage							m_ImageBackSurface;					//缓冲表面

	//函数定义
public:
	//构造函数
	CGameFrameViewGDI();
	//析构函数
	virtual ~CGameFrameViewGDI();

	//重载函数
private:
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight)=NULL;

	//重载函数
public:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//界面更新
	virtual VOID InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//游戏元素
public:
	//绘画准备
	VOID DrawUserReady(CDC * pDC, INT nXPos, INT nYPos);
	//绘画时间
	VOID DrawUserClock(CDC * pDC, INT nXPos, INT nYPos, WORD wUserClock);
	//绘画头像
	VOID DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem);

	//画图函数
public:
	//绘画背景
	VOID DrawViewImage(CDC * pDC, CBitImage & BitImage, BYTE cbDrawMode);
	//绘画字符
	VOID DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect);
	//绘画字符
	VOID DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos);

	//消息映射
protected:
	//绘画函数
	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif