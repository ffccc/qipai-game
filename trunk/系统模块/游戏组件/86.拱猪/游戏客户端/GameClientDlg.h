#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
	//游戏变量
protected:
	BYTE							m_bCardCount[GAME_PLAYER];	    	//扑克数目
	BYTE							m_bHandCardCount;					//当前数目
	BYTE							m_bHandCardData[HAND_CARD_COUNT];	//手上扑克
	bool							m_bFirstGame ;                      //首局游戏
	bool							m_bAutoOutCard ;                    //自动代打

	//出牌变量
protected:
	BYTE							m_bTurnCardData[GAME_PLAYER];	    //桌面出牌
	WORD							m_wLastOutUser;                     //最后出牌
	WORD							m_wFirstOutUser ;                   //首先出牌
	BYTE							m_bTurnCardCount;                   //桌面出牌  
	bool							m_bFirstOutSpade;                   //第一张牌
	bool							m_bFirstHearts;                     //第一张牌
	bool							m_bFirstClubs;                      //第一张牌
	bool							m_bFirstDiamonds;                   //第一张牌
	bool							m_bFirstOutCard;                    //第一张牌

	//配置变量
protected:
	bool							m_bDeasilOrder;						//出牌顺序
	DWORD							m_dwCardHSpace;						//扑克象素
	BYTE							m_bTimerElapse ;                    //定时器长


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
	//辅助函数
protected:
	//出牌判断
	bool VerdictOutCard(bool bAutoOutCard=false);
	//亮牌判断
	bool VerdictShowCard() ;
	//第一轮花色
	bool VerdictFirstGameColor(BYTE bFirstOutColor , BYTE bCardDatabool , bool bAutoOutCard=false) ;
	//自动出牌
	bool AutomatismOutCard() ;

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
	//消息映射
	
	//消息处理
protected:
	//发送扑克
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户出牌
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize) ;
	//用户亮牌
	bool OnSubShowCard(const void * pBuffer, WORD wDataSize) ;
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize) ;


protected:
	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//自动代打
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//取消代打
	LRESULT OnCancelAutoOutCard(WPARAM wParam, LPARAM lParam);
	//出牌消息
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//玩家亮牌
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam);
	//玩家不亮
	LRESULT OnNotShowCard(WPARAM wParam, LPARAM lParam);
	//右键扑克
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//左键扑克
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//等待开始计时
	LRESULT OnWaitStart(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
