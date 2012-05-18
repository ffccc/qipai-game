#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "ScoreView.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//消息定义
#define IDM_START					(WM_APP+100)						//开始消息
#define IDM_SETCHIP					(WM_APP+101)						//压注消息
#define IDM_SHOWCARD				(WM_APP+102)						//摊牌消息
#define IDM_FRONTCARD				(WM_APP+103)						//设置前墩  
#define IDM_MIDCARD					(WM_APP+104)						//设置中墩
#define IDM_BACKCARD				(WM_APP+105)						//设置后墩
#define IDM_DESCEND_SORT			(WM_APP+106)						//降序消息
#define IDM_ASCEND_SORT				(WM_APP+107)						//升序消息
#define IDM_COLOR_SORT				(WM_APP+108)						//花色消息
#define IDM_AUTO_OUT_CARD           (WM_APP+109)                        //自动代打
#define IDM_BAO_PAI					(WM_APP+110)                        //包牌消息
#define IDM_RESTORE_CARD			(WM_APP+111)                        //恢复扑克
#define IDM_CALL_BANKER				(WM_APP+112)                        //叫庄消息
#define IDM_NO_CALL_BANKER			(WM_APP+113)                        //不叫消息

////////////////////////////////////////////////////////////////////////////////////////////////

#define SET_CHIP_COUNT				4									//下注档次

//扑克类型
enum enCardType
{
	enHandCard,						//手上扑克
	enSegmentCard,					//分段扑克
	enAllSegmentCard,				//所有分段
	enCompareCard,					//比较扑克
	enBankCard,						//庄家扑克
	enErrorType,					//错误类型
};

//游戏视图
class CGameClientView : public CGameFrameView
{
	//界面变量
protected:
	WORD							m_wBanker;							//庄家玩家	
	LONG							m_nChip[GAME_PLAYER];				//压注大小
	CString							m_strShowCompare;					//显示比较
	bool							m_bShowCompare;						//显示比较
	bool							m_bShowBankerName;					//显示庄家
	enSegmentType					m_enSegmentType ;					//比较类型
	LONG							m_wMaxChip;							//最高下注
	bool							m_bNoCallBanker[GAME_PLAYER] ;		//不叫标识
	LONG							m_lChipsArray[SET_CHIP_COUNT];		//所有下注
	

	//位置变量
private:
	POINT							m_ptCallBankerInfo[GAME_PLAYER] ;	//叫庄信息

	//控件变量
	//按钮控件
public:
	CSkinButton						m_btStart;							//开始按钮
	CSkinButton						m_btShowCard;						//摊牌按钮
	CSkinButton						m_btFrontCard;						//前墩按钮
	CSkinButton						m_btMidCard;						//中墩按钮
	CSkinButton						m_btBackCard;						//后墩按钮
	CSkinButton						m_btSetChip[SET_CHIP_COUNT];		//压注按钮
	CSkinButton						m_btDescend;						//降序按钮
	CSkinButton						m_btAscend;							//升序按钮
	CSkinButton						m_btColor;							//花色按钮
	CSkinButton						m_btAutoOutCard;                    //自动代打
	CSkinButton						m_btBaoPai;							//包牌按钮
	CSkinButton						m_btRestoreCard;					//恢复扑克
	CSkinButton						m_btCallBanker ;					//叫庄按钮
	CSkinButton						m_btNoCallBanker ;					//不叫按钮

	//扑克控件
public:
	CCardControl					m_UserCardControl[GAME_PLAYER];		//手上扑克
	CCardControl					m_AllSegCardControl[GAME_PLAYER][3];//三个分段
	CCardControl					m_SegCardControl[GAME_PLAYER];		//分段扑克
	CCardControl					m_HandSegCardControl[3];			//手上分段
	CCardControl					m_CompareCarControl[GAME_PLAYER];	//对比扑克
	CCardControl					m_BankerCardControl[GAME_PLAYER];	//庄家扑克

	//位图变量
protected:
	CSkinImage						m_ImageBack;						//背景资源
	CSkinImage						m_ImageCenter;						//背景资源
	CSkinImage						m_ImageBanker;						//庄家图像
	CSkinImage						m_ImageShowUserChip;				//显示压注
	CSkinImage						m_ImageCallBankerInfo ;				//叫庄信息
	CSkinImage						m_ImageChipBack ;					//下注背景

	//控件组件
public:
	CScoreView						m_ScoreView;						//积分视图

#ifdef VIDEO_GAME
	//控件变量
protected:
	CVideoServiceControl			m_DlgVideoService[GAME_PLAYER];		//视频窗口
#endif

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	//辅助函数
protected:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//功能函数
public:
	//设置扑克
	void SetCardData(const WORD wChairID , const BYTE bCardData[] , const BYTE bCardCount , const enCardType bCardType , const BYTE bSeg=255) ;
	//设置庄家
	void SetBanker(const WORD wBanker) ;
	//设置压注
	void SetChip(const WORD wChiarID , const int bChip) ;
	//显示比较
	void SetShowCompare(bool bShow , const enSegmentType SegmentType=enErr) ;
	//显示庄家
	void ShowBankerName(bool bShow) ;
	//最高下注
	void SetMaxChip(LONG wMaxChip) ;
	//叫庄信息
	void SetCallBankerInfo( WORD wChairID, bool bNoCallBanker ) ;
	//设置下注
	void SetChips( LONG lChipsArray[SET_CHIP_COUNT] ) ;

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

	//摊牌函数
	afx_msg void OnShowCard();
	//设置前墩
	afx_msg void OnFrontCard();
	//设置中墩
	afx_msg void OnMidCard();
	//设置后墩
	afx_msg void OnBackCard();
	//设置压注
	afx_msg void OnSetChip0() ;
	//设置压注
	afx_msg void OnSetChip1() ;
	//设置压注
	afx_msg void OnSetChip2() ;
	//设置压注
	afx_msg void OnSetChip3() ;
	//开始按钮
	afx_msg void OnStart() ;
	//降序排列
	afx_msg void 	OnDescendSort() ;
	//升序排列
	afx_msg void 	OnAscendSort() ;
	//花色排列
	afx_msg void 	OnColorSort() ;
	//自动代打
	afx_msg void OnAutoOutCard() ;
	//恢复扑克
	afx_msg void OnRestoreCard() ;
	//叫庄消息
	afx_msg void OnCallBanker() ;
	//不叫消息
	afx_msg void OnNoCallBanker() ;

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
