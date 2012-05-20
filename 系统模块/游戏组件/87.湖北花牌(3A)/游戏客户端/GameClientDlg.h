#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "HuCardDlg.h"
#include "GameClientView.h"
#include "mmsystem.h"
//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
public:
	bool				m_IsTuoGuan;						//看当前状态是否是托管状态
	int                m_wPlayNativeSound;                 //播放本地话

	//作用于整个局中的变量
protected:

	WORD			    m_wBankerUser;						    //本局的庄家用户

	BYTE                m_bCardCount[GAME_PLAYER];               //记录三家的扑克牌数
	BYTE                m_SiTongCount;                           //四统的次数
	BYTE                m_SiTongIndex[8];                        //四统的索引

	BYTE                m_WuTongCount;                           //五统的次数
	BYTE                m_WuTongIndex[8];                        //五统的索引

	BYTE                m_cbHuaWeaveCardIndex[5];                //5张花牌索引
	BYTE                m_cbHuaCardIndex[5];                     //5张花牌索引

	//组合扑克
	BYTE				m_cbWeaveCount[GAME_PLAYER];		     //组合数目
	tagWeaveItem		m_WeaveItemArray[GAME_PLAYER][8];	     //组合扑克

	bool                m_bGameStart;                            //标记游戏是否开始

	//扑克变量
	BYTE				m_cbLeftCardCount;					     //剩余数目
	BYTE				m_cbCardIndex[MAX_INDEX];			     //手中扑克

	BYTE				m_cbDiscardCount[GAME_PLAYER];		     //丢弃数目
	BYTE				m_cbDiscardCard[GAME_PLAYER][34];	     //丢弃记录

	//保留的作用是：到最后时，不能胡牌就直接返回
	bool				b_IsHD;								     //是否海底

	//作用于每轮操作中的变量
protected:
	BYTE                m_cbSex;                                 //出牌用户的性别
	WORD				m_wCurrentUser;						     //当前正在操作的用户

	//保存本轮过程中，用户能操作的情况
	BYTE                m_cbActionMask;                            //操作码
	BYTE                m_cbActionCard;                            //操作牌

	//出牌信息
	WORD				m_wOutCardUser;						     //出牌用户
	BYTE				m_cbOutCardData;					     //出牌扑克

	//控件变量
protected:
	CGameLogic			m_GameLogic;						    //游戏逻辑
	CGameClientView		m_GameClientView;					    //游戏视图

	
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
	//定庄
	bool OnSubGameDecideBanker(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户出牌
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//操作提示
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//操作结果
	bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//设置用户手中的牌
	bool SetWoindowData(BYTE cbCardData[] ,BYTE cbDataCount);
	//辅助函数
protected:

	//根据所给的中心牌索引，操作类型，得出最佳组合牌数据，保存到
	bool GetHuCardWeave(BYTE cbCardIndex[],BYTE cbWeaveKind[],BYTE cbYaKou[],tagHuCardWeave &HuCardWeave);
	//根据所给的中心牌索引，操作类型，得出最佳组合牌数据，保存到
	bool GetHuCardWeave(BYTE cbCardIndex[],BYTE cbWeaveKind[],BYTE cbYaKou[],BYTE cbHuaWeaveCount[],BYTE cbHuaHandCount[],BYTE cbWeaveCount,tagHuCardWeave &HuCardWeave);

	//播放声音
	void PlayCardSound(BYTE cbCardData);
	//播放声音
	void PlayActionSound(BYTE cbAction);
	//显示控制
	void ShowTongOperateControl(BYTE cbUserAction, BYTE cbActionCard);
	//允许用户出牌
	void EnableOutCard();
	//禁止用户出牌
	void DisableOutCard();
	//将发到用户手中的牌按一定的规则分到不同的区中
	void StartSendAfterFace();

	//消息映射
protected:
	//取消托管消息
	LRESULT OnDisTuoGuan(WPARAM wParam, LPARAM lParam);
	//托管消息
	LRESULT OnTuoGuan(WPARAM wParam, LPARAM lParam);

	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//出牌操作
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//扑克操作
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void GameStartedFace(void);
};

//////////////////////////////////////////////////////////////////////////
