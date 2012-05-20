#ifndef CHOOSE_WND_HEAD_FILE
#define CHOOSE_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//消息定义
#define IDM_CHOOSE_CARD				(WM_USER+500)						//选择扑克

//////////////////////////////////////////////////////////////////////////

//选择窗口
class CChooseWnd : public CWnd
{
	//数据变量
protected:
	BYTE							m_bWeaveCount;						//组合数目
	tagChiCardInfo					m_ChiCardInfo[6];					//吃牌信息

	//状态变量
protected:
	BYTE							m_cbCurrentIndex;					//当前索引

	//位置变量
protected:
	CSize							m_SizeCell;							//单元大小
	CSize							m_SizeLeft;							//左边大小
	CSize							m_SizeRight;						//右边大小
	CPoint							m_ControlPoint;						//控件位置

	//资源变量
protected:
	CSkinImage						m_ImageBackL;						//资源图片
	CSkinImage						m_ImageBackM;						//资源图片
	CSkinImage						m_ImageBackR;						//资源图片
	CSkinImage						m_ImageBackH;						//资源图片

	//函数定义
public:
	//构造函数
	CChooseWnd();
	//析构函数
	virtual ~CChooseWnd();

	//功能函数
public:
	//设置基准
	void SetBenchmarkPos(int nXPos, int nYPos);
	//设置数据
	bool SetChooseWeave(tagChiCardInfo ChiCardInfo[6], BYTE cbWeaveCount);

	//消息映射
protected:
	//重画函数
	afx_msg void OnPaint();
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif