#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ApplyUserList.h"
#include "GameLogic.h"
//#include "DlgViewChart.h"

//////////////////////////////////////////////////////////////////////////



//筹码定义
#define JETTON_COUNT				7									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息
#define	IDM_BANK					WM_USER+202
//索引定义
#define INDEX_PLAYER				0									//闲家索引
#define INDEX_BANKER				1									//庄家索引

//////////////////////////////////////////////////////////////////////////
//结构定义

//筹码信息
struct tagJettonInfo
{
	int								nXPos;								//筹码位置
	int								nYPos;								//筹码位置
	BYTE							cbJettonIndex;						//筹码索引
};

//马
#define HORSE_COUNT					8									//筹码数目
struct tagHorseInfo
{
	bool							bStatic;							//马的状态
	bool							bBlow;								//是否喘气
	int								nXPos;								//马的位置
	int								nYPos;								//马的位置
	int								nSpeed;								//马的速度
	BYTE							cbHorseIndex;						//马的索引
	BYTE							cbHorseIndex1;						//马的索引

};

struct tagSignInfo
{
	float							fXPos;								//x位置
	float							fYPos;								//y位置
	float							fSpeed;								//速度
};

//筹码数组
typedef CArrayTemplate<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//累声明
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameView
{
	//限制信息
protected:
	LONGLONG							m_lMeMaxScore;						//最大下注
	LONGLONG							m_lAreaLimitScore;					//区域限制

//下注信息
protected:

	LONGLONG							m_lScore2;							//买平总注
	LONGLONG							m_lScore3;							//买庄总注
	LONGLONG							m_lScore1;							//买闲总注
	LONGLONG							m_lScore6;							//平天王注
	LONGLONG							m_lScore5;							//庄天王注
	LONGLONG							m_lScore4;							//闲天王注
	LONGLONG							m_lScore7;							//闲家对子
	LONGLONG							m_lScore8;							//庄家对子

	LONGLONG							m_lMeScore2;						//买平总注
	LONGLONG							m_lMeScore3;						//买庄总注
	LONGLONG							m_lMeScore1;						//买闲总注
	LONGLONG							m_lMeScore6;						//同点平注
	LONGLONG							m_lMeScore5;						//闲天王注
	LONGLONG							m_lMeScore4;						//庄天王注
	LONGLONG							m_lMeScore7;						//对子下注
	LONGLONG							m_lMeScore8;						//对子下注

	//全体下注
	protected:
	LONGLONG							m_lAllScore2;						//买平总注
	LONGLONG							m_lAllScore3;						//买庄总注
	LONGLONG							m_lAllScore1;						//买闲总注
	LONGLONG							m_lAllScore6;						//平天王注
	LONGLONG							m_lAllScore5;						//庄天王注
	LONGLONG							m_lAllScore4;						//闲天王注
	LONGLONG							m_lAllScore7;						//闲家对子
	LONGLONG							m_lAllScore8;						//庄家对子

	//位置信息
protected:
	int								m_nWinFlagsExcursionX;				//偏移位置
	int								m_nWinFlagsExcursionY;				//偏移位置
	int								m_nWinPointsExcursionX;				//偏移位置
	int								m_nWinPointsExcursionY;				//偏移位置
	int								m_nScoreHead;						//成绩位置
	CRect							m_rcFrame1;							//闲家区域
	CRect							m_rcFrame4;							//闲天王区域
	CRect							m_rcFrame7;							//对子区域
	CRect							m_rcFrame2;							//平区域
	CRect							m_rcFrame6;							//同点平区域
	CRect							m_rcFrame3;							//庄家区域
	CRect							m_rcFrame5;							//庄天王区域
	CRect							m_rcFrame8;							//对子区域

	//动画变量
protected:
	bool							m_bBombEffect[9];					//爆炸效果
	BYTE							m_cbBombFrameIndex[9];				//帧数索引
	BYTE							m_cbBlowIndex;						//喘气帧序列

	//历史信息
protected:
	LONGLONG						m_lMeStatisticScore;				//游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_C_SCORE_HISTORY];//游戏记录
	int								m_nRecordFirst;						//开始记录
	int								m_nRecordLast;						//最后记录

	int								m_Num;								//最后记录

	//状态变量
protected:
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbWinnerSide;						//胜利玩家
	BYTE							m_cbAreaFlash;						//胜利玩家
	LONGLONG						m_lCurrentJetton;					//当前筹码
	bool							m_bShowChangeBanker;				//轮换庄家

	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	LONGLONG						m_lBankerScore;						//庄家积分
	LONGLONG						m_lBankerWinScore;					//庄家成绩	
	LONGLONG						m_lTmpBankerWinScore;				//庄家成绩	
	bool							m_bEnableSysBanker;					//系统做庄

	//当局成绩
public:
	LONGLONG						m_lMeCurGameScore;					//我的成绩
	LONGLONG						m_lMeCurGameReturnScore;			//返回成绩
	LONGLONG						m_lBankerCurGameScore;				//庄家成绩
	LONGLONG						m_lGameRevenue;						//游戏税收
	LONGLONG						m_lMeCurJettonScore;				//我的成绩

	//数据变量
protected:
	CPoint							m_PointJetton[8];					//筹码位置
	CPoint							m_PointJettonNumber[8];				//数字位置
	CJettonInfoArray				m_JettonInfoArray[8];				//筹码数组

	//控件变量
public:
	CSkinButton						m_btJetton100;						//筹码按钮
	CSkinButton						m_btJetton1000;						//筹码按钮
	CSkinButton						m_btJetton10000;					//筹码按钮
	CSkinButton						m_btJetton100000;					//筹码按钮
	CSkinButton						m_btJetton1000000;					//筹码按钮
	CSkinButton						m_btJetton5000000;					//筹码按钮
	CSkinButton						m_btJetton10000000;					//筹码按钮
	CSkinButton						m_btApplyBanker;					//申请庄家
	CSkinButton						m_btCancelBanker;					//取消庄家

	CSkinButton						m_btBank;							//银行

	CSkinButton						m_btScoreMoveL;						//移动成绩
	CSkinButton						m_btScoreMoveR;						//移动成绩

	//控件变量
public:
	CApplyUser						m_ApplyUser;						//申请列表
	CGameClientDlg					*m_pGameClientDlg;					//父类指针
	CGameLogic						m_GameLogic;						//游戏逻辑

	//界面变量
protected:
	CSkinImage						m_ImageViewBack;					//背景位图	
	
	CPngImage						m_ImageJettonView;					//筹码视图
	CPngImage						m_ImageMeScoreNumber;				//数字视图

	CPngImage						m_ImageWinFlags;					//标志位图

	CPngImage						m_ImageBombEffect;					//动画图片

	CPngImage						m_ImageSaticHorse[HORSE_COUNT];		//静态图片
	CPngImage						m_ImageDynamicHorse[HORSE_COUNT];	//赛跑图片
	CPngImage						m_ImageGround;						//跑道图片
	CPngImage						m_ImageBgJetton;					//下注背景
	CSkinImage						m_ImageCut;							//照相效果
	CPngImage						m_ImageSign;						//小地图
	CPngImage						m_ImageStartLine;					//起跑线
	CPngImage						m_ImageEndLine;						//结束线
	CPngImage						m_ImageMeScore;						//我的下注
	CPngImage						m_ImageFirst;						//NO.1
	CPngImage						m_ImageBlow;						//游戏结算
	CPngImage						m_ImageResult;						//游戏结算
	CSkinImage						m_ImageWin;							//胜利玩家

	CPngImage						m_ImageFrame;						//游戏结算

	int								m_nPosX;
	int								m_nStartPosX;
	int								m_nEndPosX;
	tagHorseInfo					m_Horse[HORSE_COUNT];
	tagSignInfo						m_Sign[HORSE_COUNT];


	//边框资源
protected:
	//CSkinImage						m_ImageFrame;					//边框图片

	CPngImage						m_ImageMeBanker;					//切换庄家
	CPngImage						m_ImageChangeBanker;				//切换庄家
	CPngImage						m_ImageNoBanker;					//切换庄家
	

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

	//设置函数
public:
	//设置信息
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//我的位置
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID; }
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//庄家信息
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore);
	//庄家成绩
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//当局成绩
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//区域限制
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//设置扑克
	void SetHorseSpeed(BYTE cbSpeedArray[8][8]);
	//设置筹码
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//历史记录
	void SetGameHistory(enOperateResult OperateResult, BYTE cbWinner);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	//马的信息
public:
	BYTE							m_cbHorseSpeed[HORSE_COUNT][HORSE_COUNT];
	BYTE							m_nSprint;
	bool                            m_bFinish;
	bool                            m_bCutPhoto;

	//马的信息初始化
	void InitHorseInfo();
	//结束赛跑
	void FinishRun();
	//设置赛马速度
	void SetEveryHorseSpeed(BYTE cbSpeed);

	void SetFreeTime(); 

	BYTE GetFastOrLater(bool bFast = true);

	bool GetIsFinishRace(){return m_bCutPhoto;}

	void InitSignInfo();

	//绘画时间
	void DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//动画函数
public:	
	////开始发牌
	//void DispatchCard();
	////结束发牌
	//void FinishDispatchCard();
	//设置爆炸
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//绘画爆炸
	void DrawBombEffect(CDC *pDC);

	//绘制赛马
	void DrawHorse(CDC *pDC);
	//绘制我的下注
	void DrawMeScore(CDC * pDC, int nWidth, int nHeight);

	//计算限制
protected:
	//最大下注 闲家
	LONGLONG GetMaxHorse1();
	//最大下注 闲天王
	LONGLONG GetMaxHorse4();
	//最大下注 庄家
	LONGLONG GetMaxHorse3();
	//最大下注 庄天王
	LONGLONG GetMaxHorse5();
	//最大下注 平
	LONGLONG GetMaxHorse2();
	//最大下注 平天王
	LONGLONG GetMaxHorse6();
	//最大下注 闲家对子
	LONGLONG GetMaxHorse7();
	//最大下注 庄家对子
	LONGLONG GetMaxHorse8();


	//绘画函数
protected:
	////下注闪烁边框
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);


	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(BYTE cbWinnerSide);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//绘画数字
	void DrawMeJettonNumber(CDC *pDC);

	//照相截图
	void CutPhoto(int nWidth, int nHeight, CDC *pDC);

	//内联函数
public:
	//当前筹码
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	

	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat=-1);
	//绘画标识
	void DrawWinFlags(CDC * pDC);
	//显示结果
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//透明绘画
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//推断赢家
	//void DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair);

	//按钮消息
protected:
	//筹码按钮
	afx_msg void OnJettonButton100();
	//筹码按钮
	afx_msg void OnJettonButton1000();
	//筹码按钮
	afx_msg void OnJettonButton10000();
	//筹码按钮
	afx_msg void OnJettonButton100000();
	//筹码按钮
	afx_msg void OnJettonButton1000000();
	//筹码按钮
	afx_msg void OnJettonButton5000000();
	//筹码按钮
	afx_msg void OnJettonButton10000000();
	//查看路子
	afx_msg void OnViewChart();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//移动按钮
	afx_msg void OnScoreMoveL();
	//移动按钮
	afx_msg void OnScoreMoveR();
	//存钱按钮
	afx_msg void OnBank();
	////取钱按钮
	//afx_msg void OnBankGet();
	//
	//消息映射
protected:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//鼠标消息
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
