#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//消息定义

#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_OUT_CARD				(WM_USER+101)						//出牌消息
#define IDM_PASS_CARD				(WM_USER+102)						//放弃出牌
#define IDM_OUT_PROMPT				(WM_USER+103)						//提示出牌
#define IDM_LAST_TURN_CARD			(WM_USER+104)						//上轮按钮
#define IDM_TRUSTEE_CONTROL			(WM_USER+105)						//托管控制
#define IDM_SORT_CARD				(WM_USER+106)						//排列扑克

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameView
{
	//配置变量
protected:
	bool							m_bDeasilOrder;						//出牌顺序
	bool							m_bLastTurnCard;					//上轮标志

	//状态变量
protected:
	LONG							m_lGameScore[3];					//游戏得分
	bool							m_bPass[GAME_PLAYER];				//放弃数组
	BYTE							m_cbWinTaxis[GAME_PLAYER];			//游戏排名
	BYTE							m_bCardCount[GAME_PLAYER];			//扑克数目

	//位置信息
protected:
	CPoint							m_PointWin[GAME_PLAYER];			//名次位置
	CPoint							m_PointUserPass[GAME_PLAYER];		//放弃位置

	//位图变量
protected:
	CSkinImage						m_ImageBack;						//背景资源
	CSkinImage						m_ImageCenter;						//背景资源
	CSkinImage						m_ImageBanker;						//庄家资源
	CSkinImage						m_ImageUserPass;					//放弃标志
	CSkinImage						m_ImageScoreInfo;					//得分信息
	CSkinImage						m_ImageWinTaxis;					//排名信息
	CSkinImage						m_ImageLastTurnTip;					//上轮提示
	CSkinImage						m_ImageReady;						//准备图标
	
	//按钮控件
public:
	CSkinButton						m_btStart;							//开始按钮
	CSkinButton						m_btOutCard;						//出牌按钮
	CSkinButton						m_btPassCard;						//放弃按钮
	CSkinButton						m_btOutPrompt;						//提示按钮
	CSkinButton						m_btSortCard;						//排列扑克
	CSkinButton						m_btLastTurn;						//上轮扑克
	CSkinButton						m_btStusteeControl;					//拖管控制

	//扑克控件
public:
	CCardControl					m_HandCardControl[2];				//手上扑克
	CCardControl					m_UserCardControl[4];				//扑克视图
	CCardControl					m_OtherCardControl[2];				//手上扑克

	//控件组件
public:
	CScoreView						m_ScoreView;						//积分视图
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//继承函数
private:
	//重置界面
	virtual void ResetGameView();
	//调整控件
	virtual void RectifyGameView(int nWidth, int nHeight);
	//绘画界面
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//配置函数
public:
	//用户顺序
	void SetUserOrder(bool bDeasilOrder);
	//获取顺序
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//功能函数
public:
	//设置上轮
	void SetLastTurnCard(bool bLastTurnCard);
	//设置放弃
	void SetPassFlag(WORD wChairID, bool bPass);
	//设置得分
	void SetUserScore(WORD wIndex, LONG lScore);
	//设置名次
	void SetUserTaxis(WORD wChairID, BYTE cbTaxis);
	//扑克数目
	void SetCardCount(WORD wChairID, BYTE bCardCount);

	//消息映射
protected:
	//开始按钮
	afx_msg void OnStart();
	//出牌按钮
	afx_msg void OnOutCard();
	//放弃按钮
	afx_msg void OnPassCard();
	//提示按钮
	afx_msg void OnOutPrompt();
	//排列按钮
	afx_msg void OnSortCard();
	//拖管控制
	afx_msg void OnStusteeControl();
	//上轮按钮
	afx_msg void OnBnClickedLastTurn();
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//鼠标双击
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
