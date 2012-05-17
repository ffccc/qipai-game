#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "DirectSound.h"
#include "DlgControl.h"
#include "GameBank.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
	//友元声明
	friend class CGameClientView;

	//限制信息
protected:
	LONGLONG						m_lMeMaxScore;						//最大下注
	LONGLONG						m_lAreaLimitScore;					//区域限制
	LONGLONG						m_lApplyBankerCondition;			//申请条件

	//个人下注
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//个人总注

	//庄家信息
protected:
	LONGLONG						m_lBankerScore;						//庄家积分
	LONGLONG						m_wCurrentBanker;					//当前庄家
	BYTE							m_cbLeftCardCount;					//扑克数目
	bool							m_bEnableSysBanker;					//系统做庄

	//状态变量
protected:
	bool							m_bMeApplyBanker;					//申请标识

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
	CDlgControl						m_DlgControl;

	//声音资源
protected:
	CDirectSound					m_DTSDBackground;					//背景音乐
	CDirectSound					m_DTSDCheer[3];						//背景音乐


	struct SortTemp
	{
		LONG a;
		LONG b;
		LONG c;
		LONG d;
		SortTemp()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	SortTemp m_TempData;
	CGameBank						m_GameBank;							//银行

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

	virtual bool CGameClientDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//游戏空闲
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//游戏记录
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//下注失败
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//扑克牌
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);

	//赋值函数
protected:
	//设置庄家
	void SetBankerInfo(WORD wBanker, LONGLONG lScore);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);

	//功能函数
protected:
	//更新控制
	void UpdateButtonContron();

	//消息映射
protected:
	//加注消息
	LRESULT OnPlaceJetton(WPARAM wParam, LPARAM lParam);
	//申请消息
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);
	//继续发牌
	LRESULT OnContinueCard(WPARAM wParam, LPARAM lParam);

	//手工搓牌
	LRESULT  OnOpenCard(WPARAM wParam, LPARAM lParam);
	//自动搓牌
	LRESULT  OnAutoOpenCard(WPARAM wParam, LPARAM lParam);

	LRESULT OnManageControl(WPARAM wParam, LPARAM lParam);

	LRESULT OnBank(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
