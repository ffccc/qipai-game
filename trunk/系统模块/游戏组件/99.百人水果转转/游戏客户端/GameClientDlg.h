#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "DirectSound.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
	//友元声明
	friend class CGameClientView;

	//限制信息
protected:
	__int64							m_iMeMaxScore;						//最大下注
	__int64							m_iApplyBankerCondition;			//庄家申请条件

	//个人下注
protected:
	__int64							m_iMeAreaScoreArray[JETTON_AREA_COUNT];			

	//庄家信息
protected:
	__int64							m_iBankerScore;						//庄家积分
	WORD							m_wCurrentBanker;					//当前庄家
	bool							m_bEnableSysBanker;					//系统做庄

	//状态变量
protected:
	bool							m_bMeApplyBanker;					//本是否申请了上庄

	//控件变量
protected:
	CGameClientView					m_GameClientView;					//游戏视图

	//声音资源
protected:
	CDirectSound					m_DTSDBackground;					//背景音乐
	CDirectSound					m_DTSDCheer[3];						//背景音乐
	CDirectSound					m_DTStopJetton;						

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

	//赋值函数
protected:
	//设置庄家
	void SetBankerInfo(WORD wBanker,__int64 iScore);
	//个人下注
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount);

	//功能函数
protected:
	//更新控制
	void UpdateButtonContron();	

	//消息映射
protected:
	//加注消息
	void OnPlaceJetton(BYTE iJettonArea, __int64 iJettonNum);
	//申请消息
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);

	void OpenBank();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
