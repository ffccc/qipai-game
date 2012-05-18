#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图

	//配置变量
protected:
	DWORD							m_dwCardHSpace;						//扑克象素
	static int						m_bCompareTime;						//比较时间
	BYTE							m_bShowCardTime;					//摊牌时间

	//游戏变量
protected:
	BYTE							m_bHandCardData[HAND_CARD_COUNT];	//手上扑克
	BYTE							m_bTmpHandCardData[HAND_CARD_COUNT];//备份扑克
	BYTE							m_bHandCardCount ;					//扑克数目
	WORD							m_wBanker;							//庄家玩家
	LONG							m_nChip[GAME_PLAYER];				//压注大小
	BYTE							m_bSegmentCard[GAME_PLAYER][3][5];	//分段扑克
	UINT_PTR						m_nTimer ;							//定时器ID
	LONG							m_wMaxChip;							//最大下注
	bool							m_bFinishSegment;					//完成分段
	bool							m_bReady;							//是否准备
	bool							m_bCanSetChip;						//最大下注
	LONG							m_lGameScore[GAME_PLAYER];			//玩家积分
	
	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//消息处理
protected:
	//发送扑克
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize) ;
	//设置下注
	bool OnSubSetChip(const void * pBuffer , WORD wDataSize) ;
	//设置分段
    bool OnSubSetSegment(const void * pBuffer , WORD wDataSize) ;
    //玩家摊牌
	bool OnSubShowCard(const void * pBuffer , WORD wDataSize) ;
	//比较扑克
	bool OnSubCompareCard(const void * pBuffer , WORD wDataSize) ;
	//摊牌时间
	bool OnSubShowCardTime(const void * pBuffer , WORD wDataSize) ;



    //辅助函数
protected:
	//自动代打
	void AutomaticOutCard() ;
	//获取下注
	void GetChips( LONG lChipArray[SET_CHIP_COUNT] ) ;

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

	//消息函数
protected:
	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//压注消息
	LRESULT OnSetChip(WPARAM wParam, LPARAM lParam);
	//左键扑克
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam) ;
	//设置前墩
	LRESULT OnFrontCard(WPARAM wParam, LPARAM lParam) ;
	//设置中墩
	LRESULT OnMidCard(WPARAM wParam, LPARAM lParam) ;
	//设置后墩
	LRESULT OnBackCard(WPARAM wParam, LPARAM lParam) ;
	//玩家摊牌
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam) ;	
	//降序排列
	LRESULT	OnDescendSort(WPARAM wParam, LPARAM lParam) ;	
	//升序排列
	LRESULT	OnAscendSort(WPARAM wParam, LPARAM lParam) ;	
	//花色排列
	LRESULT	OnColorSort(WPARAM wParam, LPARAM lParam) ;	
	//自动代打
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//恢复扑克
	LRESULT OnRestoreCard(WPARAM wParam, LPARAM lParam) ;
	//叫庄消息
	LRESULT OnCallBanker(WPARAM wParam, LPARAM lParam) ;
	//不叫消息
	LRESULT OnNoCallBanker(WPARAM wParam, LPARAM lParam) ;

	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
