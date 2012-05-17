#pragma once

#ifndef SKIN_BUTTON_EX_HEAD_FILE
#define SKIN_BUTTON_EX_HEAD_FILE

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

//按钮类
class CSkinButtonEx : public CButton
{
	//变量定义
protected:
	BYTE								m_byStatusCount;				//状态数目
	bool								m_bHovering;					//盘旋标志
	CSkinImage							m_ImageBack;					//按钮位图
	CPngImage							m_PngBack;						//按钮位图

	//函数定义
public:
	//构造函数
	CSkinButtonEx();
	//析构函数
	virtual ~CSkinButtonEx();

	//重载函数
protected:
	//控件子类化
	virtual void PreSubclassWindow();
	//界面绘画函数
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//功能函数
public:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//设置函数
public:
	//调整位置
	bool FixButtonSize();
	//加载位图
	bool SetButtonImage(LPCTSTR pszFileName, BYTE byStatusCount, bool bExpandImage);
	//加载位图
	bool SetButtonImage(UINT uBitmapID, HINSTANCE hInstance, BYTE byStatusCount, bool bExpandImage);
	//默认按钮
	void SetDefaultButton(bool bResetIcon=true, bool bResetColor=true, bool bResetImage=true);

	//消息函数
protected:
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//鼠标离开
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//背景消息
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinButtonEx)
};

//////////////////////////////////////////////////////////////////////////

#endif