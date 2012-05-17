#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

typedef CArrayTemplate<tagApplyUser,tagApplyUser&> CTagApplyUser;

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
	//友元声明
	friend class CGameClientView;
	//限制信息
protected:
	LONG							m_lMeMaxScore;						//最大下注
	LONG							m_lAreaLimitScore;					//区域限制
	LONG							m_lApplyBankerCondition;			//申请条件
	BYTE							m_cbMoveIndex;						//移动索引
	BYTE							m_cbMoveEndIndex;					//移动减速索引
	BYTE							m_cbResultIndex;					//结果索引
	BYTE							m_cbEndIndex;						//停止索引
	LONG							m_lMeCanUseScore;					//可用金币
	bool							m_bFlashFlags;						//闪动开关
	//庄家信息
protected:
	LONG							m_lBankerScore;						//庄家积分
	WORD							m_wCurrentBanker;					//当前庄家
	bool							m_bEnableSysBanker;					//系统做庄
	WORD							m_wBankerTime;						//做庄次数
	LONG							m_lTmpBankerWinScore;				//庄家成绩	
	//个人信息
protected:
	LONG							m_lMeScore;							//个人积分
	//个人下注信息
protected:
	LONG							m_lMeBigSharkScore;					//大白鲨
	LONG							m_lMeFeiQinScore;					//飞禽
	LONG							m_lMeZouShouScore;					//走兽
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
	LONG							m_lALLBigSharkScore;				//自己买燕子总数
	LONG							m_lALLFeiQinScore;					//自己买燕子总数
	LONG							m_lALLZouShouScore;					//自己买燕子总数
	LONG							m_lALLYanZiScore;					//自己买燕子总数
	LONG							m_lALLTuZiScore;					//兔子
	LONG							m_lALLGeZiScore;					//鸽子
	LONG							m_lALLXiongMaoScore;				//熊猫
	LONG							m_lALLKongQueScore;					//孔雀
	LONG							m_lALLHouZiScore;					//猴子
	LONG							m_lALLLaoYingScore;					//老鹰
	LONG							m_lALLShiZiScore;					//狮子
	//历史信息
protected:
	LONG							m_lMeStatisticScore;				//游戏成绩
	BYTE							m_cbScoreHistroy[MAX_SCORE_HISTORY];//历史成绩
	LONG							m_lBankerEndScore;					//庄家成绩
	LONG							m_lMeEndScore;						//本方成绩
	LONG							m_lRevenue;							//税收

	//状态变量
protected:
	bool							m_bMeApplyBanker;					//申请标识
	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
	//申请上庄信息
	CTagApplyUser					m_ApplyUser;						//申请上庄信息
	BYTE							m_cbApplyCount;						//申请人数
	BYTE							m_cbApplyIndex;						//查看索引
	CDirectSound					m_DirectSound[8];					//游戏声音
	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//常规继承
private:
	virtual bool SetDlgTitle(){return true;}
	//初始函数
	virtual bool InitGameFrame();
	//初始函数
	virtual BOOL OnInitDialog();
	//重置框架
	virtual void ResetGameFrame();
	//游戏设置
	virtual void OnGameOptionSet();
	//时间消息
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//旁观状态
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize){return;}
	//网络消息
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//游戏场景
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);
	//功能函数
public:
	//设置庄家
	void SetBankerInfo(WORD wBanker,LONG lScore);
	void UpdateButtonContron();	
	//开始旋转
	void StartGameMove();
	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//下注失败
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//游戏空闲
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount);
	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeSysBanker(const void * pBuffer, WORD wDataSize);
	//历史记录
	bool OnSubSocreHistory(const void * pBuffer, WORD wDataSize);
	//输出成绩
	CString DrawScoreString(LONG lNumber);

	//消息映射
protected:
	//滚动消息
	LRESULT OnScoreLeft(WPARAM wParam, LPARAM lParam);
	//滚动消息
	LRESULT OnScoreRight(WPARAM wParam, LPARAM lParam);
	////滚动消息
	//LRESULT OnBankerTop(WPARAM wParam, LPARAM lParam);
	////滚动消息
	//LRESULT OnBankerBottom(WPARAM wParam, LPARAM lParam);
protected:
	//加注消息
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//申请消息
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//银行消息
	LRESULT OnBank(WPARAM wParam, LPARAM lParam);

	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
};

//////////////////////////////////////////////////////////////////////////
