#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "..\消息定义\GameLogic.h"
//#include "DxSound.h"
//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameDlg, public CGameApp
{
	//下注信息
public:
	INT64							m_lMyMaxStake;						//最大下注

	//扑克信息
protected:
	BYTE							m_cbCardCount;					//扑克数目
    //BYTE							m_cbTableCardArray[2][3];			//桌面扑克

	//历史信息
protected:
	WORD							m_wMyWinCount;						//赢的局数
	WORD							m_wMyLossCount;						//输的局数
	INT64							m_lMyWinAmountSum;					//游戏成绩

	//坐庄相关
public:
	LONG							m_lApplyBankerCondition;			//申请条件
	WORD							m_wCurrentBanker;					//当前庄家
	WORD							m_wApplyCountMax;					//申请坐庄的最大人数
	BYTE							m_cbRakeOffPlayer;					//庄家税收百分比
	BYTE							m_cbRakeOffBanker;					//闲家税收百分比


	//资源相关
protected:
	CGameSound						m_GameSound;						//播放游戏声音
	string							m_strSoundPath;						//声音文件路径
	//控件变量
public:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图

	BOOL                            m_sound;                            //声音控制

	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//常规继承
private:
	//初始函数
	virtual  bool OnInit();

	virtual bool InitGameFrame();
	//重置框架
	virtual void ResetGameFrame();
	//游戏设置
	virtual void OnGameOptionSet();
	//时间消息
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//旁观状态
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//网络消息
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//游戏场景
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);

	//消息处理
protected:
	//时间状态
	bool OnSubTimerType(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubApplyBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//更新申请庄家列表分数
	bool OnSubChangeApplyScore(const void * pBuffer, WORD wDataSize);
    bool OnSupperuser( const void * pBuffer, WORD wDataSize );

	bool OnGAME( const void * pBuffer, WORD wDataSize );
    bool OnPLU( const void * pBuffer, WORD wDataSize );
	//功能函数
protected:
	//更新筹码按钮
	void UpdateJettonButtons();
	//更新申请上下庄按钮
	void UpdateBankerButtons();
	//派发扑克
	bool DispatchUserCard(BYTE cbPlayerCount);
	//void PlayGameSound(string strSoundName);
	//void StopGameSound(string strSoundName);
	void ResetGameData();

	//消息映射
protected:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);
	//加注消息
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);

	//申请坐庄
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);

	//申请坐庄
	LRESULT OnFang(WPARAM wParam, LPARAM lParam);

	LRESULT OnInsertSystemString(WPARAM wParam, LPARAM lParam);

	LRESULT OnDealAnimationFinish(WPARAM wParam, LPARAM lParam);

	LRESULT OnPlaySound(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
