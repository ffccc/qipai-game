#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
	//友元定义
	friend class CGameClientView;

	//游戏变量
protected:
	WORD							m_wBankerUser;						//庄家位置
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//游戏状态

	//历史记录
protected:
	WORD							m_wDrawCount;						//游戏局数
	LONG							m_lGameScore;						//游戏成绩

	//下注信息
protected:
	LONG							m_lMaxScore;						//最高下注
	LONG							m_lLessScore;						//最少下注

	//辅助变量
protected:
	bool							m_bGameScene;						//场景标志
	LONG							m_lChipInCount[5];					//筹码数目
	TCHAR							m_szAccounts[GAME_PLAYER][NAME_LEN];//玩家名字

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
	
	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//常规继承
private:
	//初始函数
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

	//重载函数
protected:
	//取消消息
	virtual void OnCancel();

	//消息处理
protected:
	//时间信息
	bool OnSubTimeInfo(const void * pBuffer, WORD wDataSize);
	//下注信息
	bool OnSubScoreInfo(const void * pBuffer, WORD wDataSize);
	//庄家信息
	bool OnSubBankerUser(const void * pBuffer, WORD wDataSize);
	//抢庄信息
	bool OnSubChangeInfo(const void * pBuffer, WORD wDataSize);
	//下注信息
	bool OnSubUserChipin(const void * pBuffer, WORD wDataSize);
	//发送扑克
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//功能函数
public:
	//更新筹码
	void UpdateClipinText(LONG lMaxScore);
	//更新筹码
	void UpdateClipinStatus(LONG lLeaveScore);

	//消息映射
protected:
	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//加注消息
	LRESULT OnScore(WPARAM wParam, LPARAM lParam);
	//清除消息
	LRESULT OnClean(WPARAM wParam, LPARAM lParam);
	//下注消息
	LRESULT OnClipin(WPARAM wParam, LPARAM lParam);
	//抢庄消息
	LRESULT OnBanker(WPARAM wParam, LPARAM lParam);
	//继续消息
	LRESULT OnContinue(WPARAM wParam, LPARAM lParam);
	//发牌完成
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
	//收牌完成
	LRESULT OnReclaimCardFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
