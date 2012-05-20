#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//消息定义
#define IDM_CARD_OPERATE			(WM_USER+300)						//扑克操作

//////////////////////////////////////////////////////////////////////////

//操作提示窗口--提示用户可进行的操作：对牌、三招、四招、四藏、五藏和放弃
class CControlWnd : public CWnd
{
	//配置变量
protected:
	BYTE				m_cbActionMask;						//类型掩码
	BYTE				m_cbCenterCard;						//中心扑克
	CPoint				m_PointBenchmark;					//基准位置

	//状态变量
protected:
	BYTE				m_cbItemCount;						//子项数目
	BYTE				m_cbCurrentItem;					//当前子项

	//控件变量
protected:
	CSkinButton			m_btChiHu;							//吃胡按钮
	CSkinButton			m_btGiveUp;							//放弃按钮

	//资源变量
protected:
	CSkinImage			m_ImageControlTop;					//资源图片
	CSkinImage			m_ImageControlMid;					//资源图片
	CSkinImage			m_ImageControlButtom;				//资源图片
	CSkinImage			m_ImageActionExplain;				//动作解释

	BYTE                m_cbSiTongCard[9];                 //四藏的中心牌
	BYTE                m_cbsiTongHuaNm[9];                //四藏牌中花牌的个数

	BYTE                m_cbWuTongCard[9];                 //五藏的中心牌 
	BYTE                m_cbWuTongHuaNm[9];                //五藏中花牌的个数
	
	BYTE                m_cbDuiHuaNm;                      //对牌中花牌的个数
	BYTE                m_cbSanZhaoNm;                     //三招中花牌的个数
	BYTE                m_cbSiZhaoNm;                      //四招中花牌的个数

//////////////////////////////////////////////////////////////////////////////////////
	//函数定义
public:
	//构造函数
	CControlWnd();
	//析构函数
	virtual ~CControlWnd();

	//控件控制
public:
	//基准位置
	void SetBenchmarkPos(int nXPos, int nYPos);

	//设置对招操作的牌数据
	void SetDuiZhaoOperateCardData(BYTE cbCenterCard, BYTE cbActionMask,BYTE HuaCardIndex[]);

	//设置藏牌操作的牌数据
	void SetTongOperateCardData(BYTE cbCenterCard, BYTE cbActionMask, tagSiTongResult &SiTongCardResult,tagWuTongResult &WuTongCardResult,BYTE HuaCardIndex[]);

	//内部函数
protected:
	//调整控件
	void RectifyControl();
	//将牌数据转换成牌索引
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//消息映射
protected:
	//重画函数
	afx_msg void OnPaint();
	//吃胡按钮
	afx_msg void OnChiHu();
	//放弃按钮
	afx_msg void OnGiveUp();
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif