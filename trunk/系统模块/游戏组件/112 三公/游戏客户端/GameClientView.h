#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//消息定义

#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_SCORE					(WM_USER+101)						//加注消息
#define IDM_CLEAN					(WM_USER+102)						//加注消息
#define IDM_BANKER					(WM_USER+103)						//抢庄消息
#define IDM_CHIP_IN					(WM_USER+104)						//加注消息
#define IDM_SEND_CARD_FINISH		(WM_USER+105)						//发牌完成
#define IDM_RECLAIM_CARD_FINISH		(WM_USER+106)						//收牌完成

//////////////////////////////////////////////////////////////////////////
//结构定义

//发牌子项
struct tagSendCardItem
{
	WORD							wChairID;							//发牌用户
	BYTE							cbCardData;							//发牌数据
};

//数组说明
typedef CArrayTemplate<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameView
{
	//等待变量
protected:
	BYTE							m_bDrawTimes;						//绘画次数
	WORD							m_wUserPost[GAME_PLAYER];			//物理位置

	//界面变量
public:
	WORD							m_wMeChairID;						//我的位置
	WORD							m_wBankerUser;						//庄家位置
	WORD							m_wChairIndex[GAME_PLAYER];			//位置索引

	//历史记录
protected:
	WORD							m_wDrawCount;						//游戏局数
	LONG							m_lGameScore;						//游戏成绩

	//状态变量
protected:
	bool							m_bShowType;						//显示牌型
	bool							m_bWaitBanker;						//等待庄家
	bool							m_bWaitPlayer;						//等待闲家
	TCHAR							m_szCardType[GAME_PLAYER][16];		//牌型信息

	//下注信息
protected:
	LONG							m_lMaxScore;						//最高下注
	LONG							m_lLessScore;						//最少下注

	//发牌变量
protected:
	WORD							m_wSendCount;						//发牌次数
	WORD							m_wSendIndex;						//发牌索引
	CPoint							m_SendCardPos;						//发牌位置
	CSendCardItemArray				m_SendCardItemArray;				//发牌数组

	//收牌变量
protected:
	bool							m_bReclaimIng;						//回收标志
	WORD							m_wReclaimCount;					//收牌次数
	WORD							m_wReclaimIndex;					//收牌索引
	CPoint							m_ReclaimCardPos;					//当前位置
	CPoint							m_ReclaimStartPos;					//收牌位置
	CPoint							m_ReclaimEndingPos;					//收牌位置

	//位置变量
protected:
	CPoint							m_SendStartPos;						//开始位置
	CPoint							m_SendEndingPos[GAME_PLAYER];		//结束位置

	//位置变量
public:
	bool							m_bLongBanker;						//长庄标志
	bool							m_bTakeScore;						//回收标志
	WORD							m_wScoreIndex;
	bool							m_bBankerScore[GAME_PLAYER];		//庄家标志
	CPoint							m_GoldTarget[GAME_PLAYER];			//回收位置
	CPoint							m_GoldStation[GAME_PLAYER];			//筹码位置

	//筹码按钮
public:
	CSkinButton						m_btScore1;							//筹码按钮
	CSkinButton						m_btScore2;							//筹码按钮
	CSkinButton						m_btScore3;							//筹码按钮
	CSkinButton						m_btScore4;							//筹码按钮
	CSkinButton						m_btScore5;							//筹码按钮

	//功能按钮
public:
	CSkinButton						m_btStart;							//开始按钮
	CSkinButton						m_btClean;							//清除按钮
	CSkinButton						m_btChipIn;							//下注按钮
	CSkinButton						m_btBanker;							//抢庄按钮
	CSkinButton						m_btNoBanker;						//放弃按钮
	CSkinButton						m_btControlMax;

	//用户控件
public:
	CGoldView						m_GoldView[GAME_PLAYER];			//筹码控件
	CCardControl					m_CardControl[GAME_PLAYER];			//扑克控件

	//控件变量
public:
	CScoreView						m_ScoreView;						//成绩视图

	//资源变量
protected:
	CSkinImage						m_ImageCard;						//扑克资源
	CSkinImage						m_ImageCards;						//扑克资源
	CSkinImage						m_ImageBanker;						//庄家标志
	CSkinImage						m_ImageLongBanker;					//长庄标志
	CSkinImage						m_ImageDrawInfo;					//游戏信息
	CSkinImage						m_ImageViewFill;					//背景位图
	CSkinImage						m_ImageViewBanker;					//背景位图
	CSkinImage						m_ImageViewPlayer;					//背景位图
	CSkinImage						m_ImageWaitBanker;					//等待庄家
	CSkinImage						m_ImageWaitPlayer;					//等待玩家
	CSkinImage						m_ImageWaitEnter;					//等待进入
	CSkinImage						m_ImageWaitContinue;				//等待继续
	CSkinImage						m_ImageWait;						//等待资源
	CSkinImage						m_ImageUserFlag;					//座号资源

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

	//信息函数
public:
	//获取筹码
	LONG GetUserClipInInfo(WORD wChairID);

	//功能函数
public:
	//显示牌型
	void SetCardType(bool bShowType);
	//等待庄家
	void SetWaitBanker(bool bWaitBanker);
	//等待闲家
	void SetWaitPlayer(bool bWaitPlayer);
	//下注信息
	void SetChipInInfo(LONG lMaxScore, LONG lLessScore);
	//设置信息
	void SetUserDrawInfo(WORD wDrawCount, LONG lGameScore);
	//设置牌型
	void SetCardTypeInfo(WORD wChairID, LPCTSTR pszCardType);
	//设置位置
	void SetUserChairInfo(WORD wBankerUser, WORD wMeChairID);
	//设置筹码
	void SetUserClipInInfo(WORD wChairID, LONG lChipInCount);
	//设置长庄
	void SetLongBanker(bool bLongBanker);
	//物理位置
	void SetUserPost(WORD wPhPost,WORD wViewPost);
	//获取位置
	WORD GetUserPost(WORD wViewPost){return m_wUserPost[wViewPost];}
	//获取位置
	WORD GetBankerViewID(){return m_wChairIndex[m_wBankerUser];}
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//发牌函数
public:
	//动画判断
	bool IsDispatchCard();
	//收起扑克
	void ReclaimUserCard();
	//收起筹码
	void ReclaimUserScore();
	//完成发牌
	void FinishDispatchCard();
	//发送扑克
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);

	//消息映射
protected:
	//开始按钮
	afx_msg void OnStart();
	//筹码按钮
	afx_msg void OnScore1();
	//筹码按钮
	afx_msg void OnScore2();
	//筹码按钮
	afx_msg void OnScore3();
	//筹码按钮
	afx_msg void OnScore4();
	//筹码按钮
	afx_msg void OnScore5();
	//清除按钮
	afx_msg void OnClean();
	//下注按钮
	afx_msg void OnChipin();
	//抢庄按钮
	afx_msg void OnBanker();
	//放弃按钮
	afx_msg void OnNoBanker();
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void ControlMax();

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
