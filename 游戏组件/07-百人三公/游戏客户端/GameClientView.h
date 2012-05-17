#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "GameLogic.h"

//操作结果
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Pleace,
	enOperateResult_Win,
	enOperateResult_Lost
	
};

//记录信息
struct tagClientGameRecord
{
//	enOperateResult					enOperateFlags;						//操作标识
	//BYTE							cbPlayerCount;						//闲家点数
	//BYTE							cbBankerCount;						//庄家点数
	//BYTE							cbKingWinner;						//天王赢家
	//bool							bPlayerTwoPair;						//对子标识
	//bool							bBankerTwoPair;						//对子标识
	BYTE							cbWinner;						
	BYTE							cbMePlaceRegion;
};

//历史记录
#define MAX_SCORE_HISTORY			65									//历
//////////////////////////////////////////////////////////////////////////

//筹码定义
#define JETTON_COUNT				9									//筹码数目
//#define JETTON_COUNT				6									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息

//索引定义
#define INDEX_BANKER				0									//庄家索引
#define INDEX_PLAYER1				1									//闲家索引
#define INDEX_PLAYER2				2									//闲家索引
#define INDEX_PLAYER3				3									//闲家索引
#define INDEX_PLAYER4				4									//闲家索引


//////////////////////////////////////////////////////////////////////////
//结构定义

//筹码信息
struct tagJettonInfo
{
	int								nXPos;								//筹码位置
	int								nYPos;								//筹码位置
	BYTE							cbJettonIndex;						//筹码索引
};

//筹码数组
typedef CArrayTemplate<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//累声明
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

//游戏视图
class CGameClientView : public CGameFrameView
{
public:
	bool							m_bCanGo;
	bool							m_bSet;
	bool							m_bShowResult;

	bool							m_bAutoModel;
	BYTE							m_cbWaitPre;
protected:
	CSkinImage						m_ImageNiu;
	//CSkinButton						m_btnShouDong;
	//CSkinButton						m_btnZiDong;
	BYTE							m_cbNiuType[5];
	CPoint							m_ptDrawNiu[5];
	
	//限制信息
protected:
	LONG							m_lMeMaxScore;						//最大下注
    LONG							m_lAreaLimitScore;					//区域限制


	//下注信息
protected:



	LONG							m_lMeTianScore;						//天
	LONG							m_lMeDiScore;						//地
	LONG							m_lMeXuanScore;						//玄
	LONG							m_lMeHuangScore;					//黄

	//全体下注
protected:


	LONG							m_lAllTianScore;					//天
	LONG							m_lAllDiScore;						//地
	LONG							m_lAllXuanScore;					//玄
	LONG							m_lAllHuangScore;					//黄

	//位置信息
protected:
	int								m_nWinFlagsExcursionX;				//偏移位置
	int								m_nWinFlagsExcursionY;				//偏移位置
	int								m_nWinPointsExcursionX;				//偏移位置
	int								m_nWinPointsExcursionY;				//偏移位置
	int								m_nScoreHead;						//成绩位置
	CRect							m_rcTian;							//闲家区域
	CRect							m_rcDi;						//闲天王区域
	CRect							m_rcXuan;					//对子区域
	CRect							m_rcHuang;							//平区域
	//CRect							m_rcTieKing;						//同点平区域
	//CRect							m_rcBanker;							//庄家区域
	//CRect							m_rcBankerKing;						//庄天王区域
	//CRect							m_rcBankerTwoPair;					//对子区域

	//扑克信息
protected:	
    BYTE							m_cbTableCardArray[6][3];			//桌面扑克
	//动画变量
protected:
	POINT							m_ptDispatchCard;					//发牌位置
	BYTE							m_cbSendCount[5];					//扑克数目
	CCardControl					m_DispatchCard;						//发牌扑克
	bool							m_bBombEffect[9];					//爆炸效果
	BYTE							m_cbBombFrameIndex[9];				//帧数索引

	//历史信息
protected:
	LONG							m_lMeStatisticScore;				//游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//游戏记录
	int								m_nRecordFirst;						//开始记录
	int								m_nRecordLast;						//最后记录

	//状态变量
protected:
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbWinnerSide;						//胜利玩家
	BYTE							m_cbAreaFlash;						//胜利玩家
	LONG							m_lCurrentJetton;					//当前筹码
	CString							m_strDispatchCardTips;				//发牌提示
	bool							m_bShowChangeBanker;				//轮换庄家

	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	LONG							m_lBankerScore;						//庄家积分
	LONG							m_lBankerWinScore;					//庄家成绩	
	LONG							m_lTmpBankerWinScore;				//庄家成绩	
	bool							m_bEnableSysBanker;					//系统做庄

	//当局成绩
public:
	LONG							m_lMeCurGameScore;					//我的成绩
	LONG							m_lMeCurGameReturnScore;			//我的成绩
	LONG							m_lBankerCurGameScore;				//庄家成绩
	LONG							m_lGameRevenue;						//游戏税收

	//数据变量
protected:
	CPoint							m_PointJetton[4];					//筹码位置
	CPoint							m_PointJettonNumber[4];				//数字位置
	CJettonInfoArray				m_JettonInfoArray[4];				//筹码数组

	//控件变量
public:
	CSkinButton						m_btJetton100;						//筹码按钮
	CSkinButton						m_btJetton1000;						//筹码按钮
	CSkinButton						m_btJetton10000;					//筹码按钮
	CSkinButton						m_btJetton50000;					//筹码按钮
	CSkinButton						m_btJetton100000;					//筹码按钮
	CSkinButton						m_btJetton500000;					//筹码按钮
	CSkinButton						m_btJetton1000000;					//筹码按钮
	CSkinButton						m_btJetton5000000;					//筹码按钮
//	CSkinButton						m_btViewChart;						//查看路子
	
	CSkinButton						m_btApplyBanker;					//申请庄家
	CSkinButton						m_btCancelBanker;					//取消庄家

	CSkinButton						m_btScoreMoveL;						//移动成绩
	CSkinButton						m_btScoreMoveR;						//移动成绩

	//控件变量
public:
	CApplyUser						m_ApplyUser;						//申请列表
	CGameRecord						m_GameRecord;						//记录列表
	CCardControl					m_CardControl[6];					//扑克控件	
	CGameClientDlg					*m_pGameClientDlg;					//父类指针
	CGameLogic						m_GameLogic;						//游戏逻辑

	//界面变量
protected:
	CSkinImage						m_ImageViewFill;					//背景位图
	CSkinImage						m_ImageViewBack;					//背景位图
	CSkinImage						m_ImageWinFlags;					//标志位图
	CSkinImage						m_ImageJettonView;					//筹码视图
	CSkinImage						m_ImageScoreNumber;					//数字视图
	CSkinImage						m_ImageMeScoreNumber;				//数字视图
	CSkinImage						m_ImageTimeFlag;					//时间标识
	CPngImage						m_ImageBombEffect;					//动画图片

	//边框资源
protected:
	CSkinImage						m_ImageFrameXianJia;				//边框图片
	CSkinImage						m_ImageFrameZhuangJia;				//边框图片
	//CSkinImage						m_ImageFrameXianTianWang;			//边框图片
	//CSkinImage						m_ImageFrameZhuangTianWang;			//边框图片
	//CSkinImage						m_ImageFramePingJia;				//边框图片
	//CSkinImage						m_ImageFrameTongDianPing;			//边框图片
	//CSkinImage						m_ImageFramePlayerTwoPair;			//边框图片
	//CSkinImage						m_ImageFrameBankerTwoPair;			//边框图片

	CSkinImage						m_ImageMeBanker;					//切换庄家
	CSkinImage						m_ImageChangeBanker;				//切换庄家
	CSkinImage						m_ImageNoBanker;					//切换庄家


	//结束资源
protected:
	CSkinImage						m_ImageGameEnd;						//成绩图片
	CSkinImage						m_ImageGameEndFrame;				//边框图片
	CSkinImage						m_ImageGamePoint;					//点数图片

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
	void SetAreaFlash(BYTE cbAreaFlash){m_cbAreaFlash=cbAreaFlash;}
	//设置信息
	void SetMeMaxScore(LONG lMeMaxScore);
	//我的位置
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID; }
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount);
	//庄家信息
	void SetBankerInfo(WORD wBankerUser, LONG lBankerScore);
	//庄家成绩
	void SetBankerScore(WORD wBankerTime, LONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//当局成绩
	void SetCurGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore, LONG lGameRevenue);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount);
	//区域限制
	void SetAreaLimitScore(LONG lAreaLimitScore);	
	//设置扑克
	void SetCardInfo(bool bHaveCard,BYTE cbTableCardArray[6][3]);
	//设置筹码
	void SetCurrentJetton(LONG lCurrentJetton);
	//历史记录
//	void SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair);
	void SetGameHistory(enOperateResult OperateResult,BYTE cbWinner);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//成绩设置
	void SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	//动画函数
public:	
	//开始发牌
	void DispatchCard();
	//结束发牌
	void FinishDispatchCard();
	//设置爆炸
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//绘画爆炸
	void DrawBombEffect(CDC *pDC);

	//获取结束时牌型标志图标
	BYTE GetIconNum(BYTE cbBufferData[], BYTE cbBufferCount);

	//计算限制
protected:
	//最大下注
	LONG GetMaxTianScore();
	//最大下注
	LONG GetMaxDiScore();
	//最大下注
	LONG GetMaxXuanScore();
	//最大下注
	LONG GetMaxHuangScore();


	//绘画函数
protected:
	//闪烁边框
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
	//发牌提示
	void SetDispatchCardTips();

	//内联函数
public:
	//当前筹码
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }	

	//内部函数
private:
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//绘画数字
	void DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, CRect rcRect, INT nFormat=-1);
	//绘画标识
	void DrawWinFlags(CDC * pDC);
	//显示结果
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//透明绘画
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//推断赢家
	void DeduceWinner(BYTE &cbWinner);

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
	afx_msg void OnJettonButton50000();
	afx_msg void OnJettonButton500000();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//移动按钮
	afx_msg void OnScoreMoveL();
	//移动按钮
	afx_msg void OnScoreMoveR();

	afx_msg void OnShouDongModel();
	afx_msg void OnZiDongModel();

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
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////

#endif
