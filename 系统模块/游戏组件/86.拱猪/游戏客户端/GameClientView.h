#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "../../../开发库/Include/SkinButton.h"


//////////////////////////////////////////////////////////////////////////
//消息定义

#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_OUT_CARD				(WM_USER+101)						//出牌消息
#define IDM_WAIT_START			    (WM_USER+102)                       //布牌消息
#define IDM_SHOW_CARD               (WM_USER+103)                       //亮牌消息
#define IDM_NOT_SHOW_CARD           (WM_USER+104)                       //不亮消息
#define IDM_AUTO_OUT_CARD           (WM_USER+105)                       //自动代打
#define IDM_CANCEL_AUTO_OUT_CARD    (WM_USER+106)                       //取消代打
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//游戏视图
class CGameClientView : public CGameFrameView
{
	//游戏变量
public:
	BYTE							m_bScoreCard[GAME_PLAYER][16];      //分值扑克
	BYTE							m_bScoreCardCount[GAME_PLAYER];     //扑克数目
	BYTE							m_bShowCard[GAME_PLAYER][4];        //玩家亮牌 
	BYTE							m_bShowCardCount[GAME_PLAYER];      //亮牌数目

	//界面变量
protected:
	//配置变量
protected:
	bool							m_bDeasilOrder;						//出牌顺序
	//状态变量
public:
	bool							m_bSendWaiMsg;                      //举手计时
	BYTE							m_bCardCount[GAME_PLAYER];			//扑克数目
	bool							m_bShowCardHint;                    //亮牌提示

	//控件变量
	//按钮控件
public:
	CSkinButton					    m_btStart;							//开始按钮
	CSkinButton					    m_btOutCard;						//出牌按钮
	CSkinButton						m_btShowCard;                       //亮牌按钮
	CSkinButton						m_btNotShowCard;                    //不亮按钮
	CSkinButton						m_btAutoOutCard;                    //自动代打
	CSkinButton						m_btCancelAutoOutCard;              //取消代打

	//扑克控件
public:
	CCardControl					m_HandCardControl;					//手上扑克
	CCardControl					m_UserCardControl[GAME_PLAYER];		//扑克视图

	//位图变量
protected:
	CSkinImage						m_ImageBack;						//背景资源
	CSkinImage						m_ImageBackGround;					//背景资源

	CSkinImage						m_ImageScoreLeft;					//积分左边
	CSkinImage						m_ImageScoreMid;					//积分中间
	CSkinImage						m_ImageScoreRight;					//积分右边
	CSkinImage						m_ImageScoreSeparator;				//积分分割
	
	CSkinImage						m_ImageScore[16];                   //分数位图

	//控件组件
public:
	CScoreView						m_ScoreView;						//积分视图

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();
	//配置函数
public:
	//用户顺序
	void SetUserOrder(bool bDeasilOrder);
	//获取顺序
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//功能函数
public:	
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//增加分数
	void AddScoreCard(WORD wChairID , BYTE bCardData[] , BYTE bCardCount) ;
	//设置亮牌
	void SetShowCard(WORD wChairID , BYTE bCardData[] , BYTE bCardCount) ;

	//辅助函数
protected:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

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

	//开始按钮
	afx_msg void OnStart();
	//出牌按钮
	afx_msg void OnOutCard();
	//亮牌按钮
	afx_msg void OnShowCard() ;
	//不亮按钮
	afx_msg void OnNotShowCard() ;
	//自动代打
	afx_msg void OnAutoOutCard() ;
	//取消代打
	afx_msg void OnCancelAutoOutCard() ;
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
