#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "ChooseWnd.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////

//消息定义
#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_OUT_CARD				(WM_USER+101)						//出牌消息

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameView
{
	//友员定义
	friend class CGameClientDlg;

	//游戏变量
protected:
	LONG							m_lCellScore;						//基础积分
	WORD							m_wBankerUser;						//庄家用户
	BYTE							m_bLeftCardCount;					//剩余扑克

	//玩家状态
protected:
	bool							m_bOutCard;							//出牌标志
	bool							m_bWaitOther;						//等待标志
	bool							m_bHuangZhuang;						//荒庄标志

	//用户状态
protected:
	BYTE							m_cbCardData;						//出牌扑克
	WORD							m_wOutCardUser;						//出牌用户
	BYTE							m_bUserAction[3];					//用户动作
	BYTE							m_bUserHuXiCount[3];				//胡息数目

	//辅助变量
protected:
	bool							m_bMoveCard;						//移动标志
	bool							m_bSwitchCard;						//交换标志
	WORD							m_wMouseDownItem;					//点击索引

	//位置信息
protected:
	CSize							m_BankerSize;						//庄家大小
	CPoint							m_BankerPoint[GAME_PLAYER];			//庄家位置

	//扑克控件
public:
	CUserCard						m_UserCard[2];						//用户扑克
	CWeaveCard						m_WeaveCard[3][7];					//组合扑克
	CDiscardCard					m_DiscardCard[3];					//丢弃扑克
	CCardControl					m_HandCardControl;					//手上扑克

	//控件变量
public:
	CSkinButton						m_btStart;							//开始按钮
	CScoreView						m_ScoreView;						//积分视图
	CChooseWnd						m_ChooseWnd;						//控制窗口
	CControlWnd						m_ControlWnd;						//控制窗口

	//界面变量
protected:
	CSkinImage						m_ImageWait;						//等待提示
	CSkinImage						m_ImageTitle;						//标题位图
	CSkinImage						m_ImageBanker;						//庄家位图
	CSkinImage						m_ImageOutCard;						//出牌提示
	CSkinImage						m_ImageViewBack;					//背景位图
	CSkinImage						m_ImageCardFrame;					//背景位图
	CSkinImage						m_ImageUserAction;					//用户动作
	CSkinImage						m_ImageActionBack;					//动作背景
	CSkinImage						m_ImageHuangZhuang;					//荒庄标志

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

	//功能函数
public:
	//基础积分
	void SetCellScore(LONG lCellScore);
	//庄家用户
	void SetBankerUser(WORD wBankerUser);
	//设置荒庄
	void SetHuangZhuang(bool bHuangZhuang);
	//剩余扑克
	void SetLeftCardCount(BYTE bLeftCardCount);
	//状态标志
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//胡息数目
	void SetUserHuXiCount(WORD wChairID, BYTE bHuXiCount);
	//动作信息
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//出牌信息
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);

	//消息映射
protected:
	//开始按钮
	afx_msg void OnStart();
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
