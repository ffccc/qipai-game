#pragma once

#include "Stdafx.h"
#include "GameScoreWnd.h"
#include "ScoreView.h"
#include "ApplyUserList.h"
#include "SkinButtonEx.h"
//////////////////////////////////////////////////////////////////////////
//消息定义
#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_SCORE_L					(WM_USER+101)						//滚动消息
#define IDM_SCORE_R					(WM_USER+102)						//滚动消息
#define IDM_BANKER_T				(WM_USER+103)						//滚动消息
#define IDM_BANKER_B				(WM_USER+104)						//滚动消息
#define IDM_PLACE_JETTON			(WM_USER+105)						//加住信息
#define IDM_APPLY_BANKER			(WM_USER+106)						//申请信息
#define	IDM_BANK					(WM_USER+202)						//银行信息
//////////////////////////////////////////////////////////////////////////
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
//游戏视图
class CGameClientView : public CGameFrameView
{
	//友元定义
	friend class CGameClientDlg;
	//限制信息
protected:
	LONG							m_lMeMaxScore;						//最大下注
	LONG							m_lAreaLimitScore;					//区域限制
	bool							m_bIsMove;							//开局动画
	BYTE							m_cbMoveIndex;						//移动索引
	BYTE							m_cbGameState;						//游戏状态
	BYTE							m_cbGameTimer;						//游戏时间
	LONG							m_lMeCanUseScore;					//可用金币
	BYTE							m_cbSlectPos;						//选中方向
	//信息变量
protected:
	WORD							m_wMeChairID;						//自己椅子位置	
	LONG							m_lCurrentJetton;					//当前筹码
	BYTE							m_cbJettonIndex;					//筹码区域索引
	BYTE							m_cbSelectIndex;					//选中索引
	//个人下注信息
protected:
	LONG							m_lMeBigSharkScore;					//自己买大白鲨总数
	LONG							m_lMeFeiQinScore  ;					//飞禽
	LONG							m_lMeZouShouScore ;					//走兽
	LONG							m_lMeYanZiScore;					//自己买燕子总数
	LONG							m_lMeTuZiScore;						//兔子
	LONG							m_lMeGeZiScore;						//鸽子
	LONG							m_lMeXiongMaoScore;					//熊猫
	LONG							m_lMeKongQueScore;					//孔雀
	LONG							m_lMeHouZiScore;					//猴子
	LONG							m_lMeLaoYingScore;					//老鹰
	LONG							m_lMeShiZiScore;					//狮子

	//全体下注
protected:
	LONG							m_lAllBigSharkScore;
	LONG							m_lAllFeiQinScore;
	LONG							m_lAllZouShouScore;
	LONG							m_lALLYanZiScore;					//燕子总数
	LONG							m_lALLTuZiScore;					//兔子
	LONG							m_lALLGeZiScore;					//鸽子
	LONG							m_lALLXiongMaoScore;				//熊猫
	LONG							m_lALLKongQueScore;					//孔雀
	LONG							m_lALLHouZiScore;					//猴子
	LONG							m_lALLLaoYingScore;					//老鹰
	LONG							m_lALLShiZiScore;					//狮子
	//庄家信息
protected:	
	WORD							m_wBankerUser;						//当前庄家
	WORD							m_wBankerTime;						//做庄次数
	LONG							m_lBankerScore;						//庄家积分
	LONG							m_lTmpBankerWinScore;				//庄家成绩	
	bool							m_bEnableSysBanker;					//系统做庄
	BOOL							m_bIsShowJetton;					//是否显示筹码
	//历史信息
protected:
	LONG							m_lMeStatisticScore;				//游戏成绩
	CArrayTemplate<BYTE>			m_cbScoreHistroy;					//历史成绩
	BYTE							m_cbScoreHistroyIndex;				//历史成绩索引
	//数据变量
protected:
	CPoint							m_PointJetton[11];					//筹码位置
	CPoint							m_PointJettonNumber[11];				//数字位置
	CJettonInfoArray				m_JettonInfoArray[11];				//筹码数组
	//界面变量
protected:
	CSkinImage						m_ImageViewBack;					//背景位图
	CPngImage						m_ImageJettonView;					//筹码视图
	CPngImage						m_ImageScoreNumber;					//数字视图
	CPngImage						m_ImageMeScoreNumber;				//数字视图
	CPngImage						m_ImageSelect;						//选中状态
	CPngImage						m_ImageGameState;					//游戏状态
	CPngImage						m_ImageGameTimer;					//游戏时间
	CPngImage						m_ImageScoreHistory;				//历史成绩
	CPngImage						m_ImageStar;						//流星效果
	CPngImage						m_ImageJettonBK;					//筹码背景

	//动物图片
	CPngImage						m_ImageYanZi;						//燕子
	CPngImage						m_ImageYanZi1;
	CPngImage						m_ImageTuZi;						//兔子
	CPngImage						m_ImageTuZi1;
	CPngImage						m_ImageGeZi;						//鸽子
	CPngImage						m_ImageGeZi1;
	CPngImage						m_ImageXiongMao;					//熊猫
	CPngImage						m_ImageXiongMao1;
	CPngImage						m_ImageKongQue;						//孔雀
	CPngImage						m_ImageKongQue1;
	CPngImage						m_ImageHouZi;						//猴子
	CPngImage						m_ImageHouZi1;
	CPngImage						m_ImageLaoYing;						//老鹰
	CPngImage						m_ImageLaoYing1;
	CPngImage						m_ImageShiZi;						//狮子
	CPngImage						m_ImageShiZi1;
	CPngImage						m_ImageDaBaiSha;					//大白鲨
	CPngImage						m_ImageDaBaiSha1;
	CPngImage						m_ImageFeiQin;						//飞禽
	CPngImage						m_ImageZouShou;						//走兽

	//位置变量
	CPoint							m_ptUserInfo;						//用户信息位置
	CRect							m_rcBigShark;						//大白鲨
	CRect							m_rcFeiQin;							//飞禽
	CRect							m_rcZouShou;						//走兽
	CRect							m_rcYanZi;							//燕子
	CRect							m_rcTuZi;							//兔子
	CRect							m_rcGeZi;							//鸽子
	CRect							m_rcXiongMao;						//熊猫
	CRect							m_rcKongQue;						//孔雀
	CRect							m_rcHouZi;							//猴子
	CRect							m_rcLaoYing;						//老鹰
	CRect							m_rcShiZi;							//狮子
	CPoint							m_ptBaseTop;						//上大奔点
	CPoint							m_ptBaseBottom;						//下大奔点
	CPoint							m_ptBaseLeft;						//左大奔点
	CPoint							m_ptBaseRight;						//右大本点
	CPoint							m_ptGameState;						//游戏状态
	CPoint							m_ptGameTimer;						//时间
	CPoint							m_ptScoreHistory;					//历史成绩
	//按钮
	//CSkinButton						m_btBanker_T;						//申请上庄按钮上
	//CSkinButton						m_btBanker_B;						//申请上庄按钮下
	CSkinButton						m_btScore_L;						//申请上庄按钮左
	CSkinButton						m_btScore_R;						//申请上庄按钮右
	CSkinButtonEx					m_btJetton100;						//筹码按钮
	CSkinButtonEx					m_btJetton1000;						//筹码按钮
	CSkinButtonEx					m_btJetton10000;					//筹码按钮
	CSkinButtonEx					m_btJetton100000;					//筹码按钮
	CSkinButtonEx					m_btJetton1000000;					//筹码按钮
	CSkinButtonEx					m_btJetton10000000;					//筹码按钮
	CSkinButtonEx					m_btJetton;							//激活筹码


	//控件组件
public:
	CScoreView						m_ScoreView;						//积分视图
	//CApplyUser						m_ApplyUser;						//申请列表
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
	//设置自己位置
	void SetMeChairID(WORD wMeChair);
	//设置信息
	void SetMeMaxScore(LONG lMeMaxScore);
	//庄家信息
	void SetBankerInfo(WORD wBankerUser, LONG lBankerScore);
	//庄家成绩
	void SetBankerScore(WORD wBankerTime, LONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//允许系统做庄
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//区域限制
	void SetAreaLimitScore(LONG lAreaLimitScore);	
	//获取区域
	BYTE GetJettonArea(CPoint MousePoint);
	//设置筹码
	void PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount);
	//清理筹码
	void CleanUserJetton();
	//绘画数字
	void DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false, bool bTimer=false);
	//绘画数字
	void DrawMeJettonNumber(CDC *pDC);
	//绘画数字
	void DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount);
	//设置选中索引
	void SetSelectIndex(BYTE cbSelectIndex);
	//获取选中索引的起始点
	void GetSelectPt(CPoint &ptSelect, BYTE cbMoveIndex=0xff);
	//开局动画
	void SetCanMove(bool bMove);
	//设置动画索引
	void SetMoveIndex(BYTE cbMoveIndex);
	//设置筹码
	void SetCurrentJetton(LONG lCurrentJetton);
	//设置游戏状态
	void SetGameState(BYTE cbState);
	//绘画时间
	void SetGameTimerNum(BYTE cbTimerNum);
	//设置可用金币
	void SetMeCanUseSocre(LONG lCanUseScore);
	//设置成绩
	void SetMeScore(LONG lMeScore);
	//设置选中区域所有
	void SetSelectJettonIndex(BYTE cbSelectIndex);
	//设置个人成绩
	void SetMeHisScore(LONG lHisScore);
	//设置历史信息
	void SetScoreHistory(BYTE cbScoreHistory[MAX_SCORE_HISTORY]);
	//最大下注
	LONG GetMaxScore(BYTE cbArea);
	//获取角度
	double GetAngle(BYTE cbMoveIndex);
	//坐标矫正
	POINT GetErrorPoint(BYTE cbMoveIndex);

	//内联函数
public:
	//当前筹码
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }	
	//按钮消息
protected:
	//滚动按钮
	afx_msg void OnScoreLeft(); 
	//滚动按钮
	afx_msg void OnScoreRight();
	//滚动按钮
	afx_msg void OnBankerTop();
	//滚动按钮
	afx_msg void OnBankerBottom();
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
	afx_msg void OnJettonButton10000000();
	//筹码按钮
	afx_msg void OnJettonButton();
	//上庄按钮
	afx_msg void OnApplyBanker();
	//下庄按钮
	afx_msg void OnCancelBanker();
	//存取按钮
	afx_msg void OnSaveBank();
	//取钱按钮
	afx_msg void OnGetBank();

	//消息映射
protected:
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
