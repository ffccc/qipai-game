#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "GameLogic.h"
#include "AdminControlDlg.h"
#include "DlgViewChart.h"

#ifdef __SPECIAL___
#include "DlgBank.h"
#endif
//////////////////////////////////////////////////////////////////////////

//筹码定义
#define JETTON_COUNT				6									//筹码数目
#define JETTON_RADII				68									//筹码半径

//消息定义
#define IDM_PLACE_JETTON			WM_USER+200							//加住信息
#define IDM_APPLY_BANKER			WM_USER+201							//申请信息

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
	LONGLONG						m_lMeMaxScore;						//最大下注
    LONGLONG						m_lAreaLimitScore;					//区域限制

	//下注信息
protected:
	LONGLONG						m_lMeTieScore;						//买平总注
	LONGLONG						m_lMeBankerScore;					//买庄总注
	LONGLONG						m_lMePlayerScore;					//买闲总注
	LONGLONG						m_lMeTieSamePointScore;				//同点平注
	LONGLONG						m_lMePlayerKingScore;				//闲天王注
	LONGLONG						m_lMeBankerKingScore;				//庄天王注
	LONGLONG						m_lMePlayerTwoPair;					//对子下注
	LONGLONG						m_lMeBankerTwoPair;					//对子下注

	//全体下注
protected:
	LONGLONG						m_lAllTieScore;						//买平总注
	LONGLONG						m_lAllBankerScore;					//买庄总注
	LONGLONG						m_lAllPlayerScore;					//买闲总注
	LONGLONG						m_lAllTieSamePointScore;			//平天王注
	LONGLONG						m_lAllBankerKingScore;				//庄天王注
	LONGLONG						m_lAllPlayerKingScore;				//闲天王注
	LONGLONG						m_lAllPlayerTwoPair;				//闲家对子
	LONGLONG						m_lAllBankerTwoPair;				//庄家对子

	//位置信息
protected:
	int								m_nWinFlagsExcursionX;				//偏移位置
	int								m_nWinFlagsExcursionY;				//偏移位置
	int								m_nWinPointsExcursionX;				//偏移位置
	int								m_nWinPointsExcursionY;				//偏移位置
	int								m_nScoreHead;						//成绩位置
	CRect							m_rcPlayer;							//闲家区域
	CRect							m_rcPlayerKing;						//闲天王区域
	CRect							m_rcPlayerTwoPair;					//对子区域
	CRect							m_rcTie;							//平区域
	CRect							m_rcTieKing;						//同点平区域
	CRect							m_rcBanker;							//庄家区域
	CRect							m_rcBankerKing;						//庄天王区域
	CRect							m_rcBankerTwoPair;					//对子区域
	CRect							m_rcTrigger;						//触发区域
	//扑克信息
protected:	
	BYTE							m_cbCardCount[2];					//扑克数目
    BYTE							m_cbTableCardArray[2][3];			//桌面扑克

	//动画变量
protected:
	POINT							m_ptDispatchCard;					//发牌位置
	BYTE							m_cbSendCount[2];					//扑克数目
	CCardControl					m_DispatchCard;						//发牌扑克
	bool							m_bBombEffect[9];					//爆炸效果
	BYTE							m_cbBombFrameIndex[9];				//帧数索引

	//历史信息
protected:
	LONGLONG						m_lMeStatisticScore;				//游戏成绩
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//游戏记录
	int								m_nRecordFirst;						//开始记录
	int								m_nRecordLast;						//最后记录

	//状态变量
protected:
	WORD							m_wMeChairID;						//我的位置
	BYTE							m_cbWinnerSide;						//胜利玩家
	BYTE							m_cbAreaFlash;						//胜利玩家
	LONGLONG						m_lCurrentJetton;					//当前筹码
	CString							m_strDispatchCardTips;				//发牌提示
	bool							m_bShowChangeBanker;				//轮换庄家
	bool							m_bNeedSetGameRecord;				//完成设置

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
	LONGLONG						m_lMeCurGameReturnScore;			//我的成绩
	LONGLONG						m_lBankerCurGameScore;				//庄家成绩
	LONGLONG						m_lGameRevenue;						//游戏税收

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
	CSkinButton						m_btViewChart;						//查看路子
	
	CSkinButton						m_btApplyBanker;					//申请庄家
	CSkinButton						m_btCancelBanker;					//取消庄家

	CSkinButton						m_btScoreMoveL;						//移动成绩
	CSkinButton						m_btScoreMoveR;						//移动成绩

	CSkinButton						m_btUp;								//按钮
	CSkinButton						m_btDown;							//按钮

	CButton							m_btOpenAdmin;
	//控件变量
public:
	CApplyUser						m_ApplyUser;						//申请列表
	CGameRecord						m_GameRecord;						//记录列表
	CCardControl					m_CardControl[2];					//扑克控件	
	CGameClientDlg					*m_pGameClientDlg;					//父类指针
	CGameLogic						m_GameLogic;						//游戏逻辑
	CDlgViewChart					m_DlgViewChart;						//查看路子

	//
#ifdef __SPECIAL___
	CDlgBank						m_DlgBank;							//银行控件	
	CSkinButton						m_btBankerStorage;					//存款按钮
	CSkinButton						m_btBankerDraw;						//取款按钮
#endif

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
	CSkinImage						m_ImageFrameXianTianWang;			//边框图片
	CSkinImage						m_ImageFrameZhuangTianWang;			//边框图片
	CSkinImage						m_ImageFramePingJia;				//边框图片
	CSkinImage						m_ImageFrameTongDianPing;			//边框图片
	CSkinImage						m_ImageFramePlayerTwoPair;			//边框图片
	CSkinImage						m_ImageFrameBankerTwoPair;			//边框图片

	CSkinImage						m_ImageMeBanker;					//切换庄家
	CSkinImage						m_ImageChangeBanker;				//切换庄家
	CSkinImage						m_ImageNoBanker;					//切换庄家

	//结束资源
protected:
	CSkinImage						m_ImageGameEnd;						//成绩图片
	CSkinImage						m_ImageGameEndFrame;				//边框图片
	CSkinImage						m_ImageGamePoint;					//点数图片
	bool                            m_bFlashResult;
public:
    CAdminControlDlg				m_AdminDlg;							//管理员控制
	//银行
public:
	bool							m_blCanStore;                       //是否能保存

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
	void SetCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3]);
	//设置筹码
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//历史记录
	void SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair);
	//轮换庄家
	void ShowChangeBanker( bool bChangeBanker );
	//成绩设置
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//取消闪动
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }

	//动画函数
public:	
	//开始发牌
	void DispatchCard();
	//结束发牌
	void FinishDispatchCard(bool bScene = false);
	//设置爆炸
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//绘画爆炸
	void DrawBombEffect(CDC *pDC);

	//计算限制
public:
	//最大下注
	LONGLONG GetMaxPlayerScore();
	//最大下注
	LONGLONG GetMaxPlayerKingScore();
	//最大下注
	LONGLONG GetMaxBankerScore();
	//最大下注
	LONGLONG GetMaxBankerKingScore();
	//最大下注
	LONGLONG GetMaxTieScore();
	//最大下注
	LONGLONG GetMaxTieKingScore();
	//最大下注
	LONGLONG GetMaxPlayerTwoPair();
	//最大下注
	LONGLONG GetMaxBankerTwoPair();

	//公共函数
public:
	//获取所有下注
	LONGLONG GetTotalChip();


	//绘画函数
protected:
	//闪烁边框
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);


	//界面函数
public:
	//清理筹码
	void CleanUserJetton();
	//设置胜方
	void SetWinnerSide(BYTE cbWinnerSide,bool blSet = false);
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//绘画数字
	void DrawMeJettonNumber(CDC *pDC);
	//发牌提示
	void SetDispatchCardTips();

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
	void DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair);

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
	//银行存款
	afx_msg void OnBankStorage();
	//银行取款
	afx_msg void OnBankDraw();
	//管理员控制
	afx_msg void OpenAdminWnd();
	//上翻页
	afx_msg void OnUp();
	//下翻页
	afx_msg void OnDown();

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
