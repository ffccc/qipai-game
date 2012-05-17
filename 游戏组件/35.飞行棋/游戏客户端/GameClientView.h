#pragma once

#include "Stdafx.h"
#include "D3DWnd.h"
#include "GameScoreView.h"

//////////////////////////////////////////////////////////////////////////
//消息定义

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameView
{
	friend class CGameClientDlg;
	//界面变量
protected:
	CD3DWnd m_wndD3D;
	CGameScoreView m_GameScoreView;
	//控件变量
public:

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//继承函数
private:
	//重置界面
	virtual void ResetGameView();
	//调整控件
	virtual void RectifyGameView(int nWidth, int nHeight);
	//绘画界面
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//消息映射
protected:
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
