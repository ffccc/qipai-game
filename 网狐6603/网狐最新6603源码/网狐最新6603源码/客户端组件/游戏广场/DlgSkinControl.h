#ifndef DLG_SKIN_CONTROL_HEAD_FILE
#define DLG_SKIN_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//界面控制
class CDlgSkinControl : public CDialog
{
	//盘旋变量
protected:
	WORD							m_wHoverItem;						//盘旋子项
	WORD							m_wHoverColor;						//盘旋颜色
	WORD							m_wMouseDownColor;					//点击颜色

	//大小变量
protected:
	CSize							m_SizeSkinItem;						//选择子项
	CSize							m_SizeColorItem;					//选择颜色

	//选择变量
protected:
	tagSkinRenderInfo				m_RenderInfoItem[8];				//颜色子项
	tagSkinRenderInfo				m_CurrentRenderInfo;				//颜色选择

	//函数定义
public:
	//构造函数
	CDlgSkinControl();
	//析构函数
	virtual ~CDlgSkinControl();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK() { return; }
	//取消消息
	virtual VOID OnCancel() { return; }

	//辅助函数
protected:
	//获取选择
	WORD GetCurrentItem(CPoint MousePoint);
	//获取选择
	WORD GetCurrentColor(CPoint MousePoint);
	//更新颜色
	VOID SetCurrentColor(WORD wColorIndex, INT nXExcursion);

	//消息函数
public:
	//重画消息
	VOID OnPaint();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//焦点消息
	VOID OnKillFocus(CWnd * pNewWnd);
	//鼠标消息
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif