#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"


//显示胡牌对话框
class CHuCardDlg :public CWnd
{
protected:
	CSkinImage		    m_ImageBack;				//背景资源
	tagHuCardWeave      m_HuCardWeave;              //最佳胡牌组合 
	BYTE                m_bHuScore;                 //胡牌分数
	BYTE                m_bGoldCard;                //主金的牌
	CPoint	     		m_PointBenchmark;			//基准位置

	CSkinButton			m_btChiHu;					//吃胡按钮
	CSkinButton			m_btGiveUp;					//放弃按钮

	CSkinImage			m_ImageControlLeft;			//资源图片
	CSkinImage			m_ImageControlMid;			//资源图片
	CSkinImage			m_ImageControlRight;		//资源图片

	//函数定义
public:
	//构造函数
	CHuCardDlg(void);

	//析构函数
	virtual ~CHuCardDlg(void);


	//内部函数
protected:
	//调整控件
	void RectifyControl();

	//功能函数
public:
	//设置胡牌信息
	void SetHuCardWeave(tagHuCardWeave HuCardWeave,BYTE HuScore,BYTE GoldCard);
	//基准位置
	void SetBenchmarkPos(int nXPos, int nYPos);

	//重画函数
	afx_msg void OnPaint();

	//吃胡按钮
	afx_msg void OnChiHu();
	//放弃按钮
	afx_msg void OnGiveUp();

	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
