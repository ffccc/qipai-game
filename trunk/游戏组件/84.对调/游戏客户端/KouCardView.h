#ifndef KOU_CARD_VIEW_HEAD_FILE
#define KOU_CARD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//积分视图类
class CKouCardView : public CWnd
{
	//结束信息
protected:
	BYTE							m_cbConcealCount;					//底牌数目
	BYTE							m_cbConcealCard[8];					//底牌扑克

	//资源变量
protected:
	CSize							m_CardSize;							//扑克大小
	CSkinImage						m_ImageBack;						//背景图案
	CSkinImage						m_ImageCard;						//扑克位图

	//扑克控件
public:
	CCardControl					m_ConcealCard;						//底牌扑克
	CCardControl					m_HandCardControl;					//手上扑克
	
	//函数定义
public:
	//构造函数
	CKouCardView();
	//析构函数
	virtual ~CKouCardView();
	
	//功能函数
public:
	//设置信息
	void SetConcealCard(BYTE cbConcealCard[], BYTE cbConcealCount);

	//消息函数
protected:
	//重画函数
	afx_msg void OnPaint();
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif