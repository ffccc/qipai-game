#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_TIMEOUT					(WM_USER+101)						//超时消息
#define IDM_DICE_CAST				(WM_USER+102)						//投掷骰子消息
#define IDM_CHESS_CHOOSE			(WM_USER+103)						//选择棋子消息
#define IDM_AUTOMATISM				(WM_USER+104)						//选择棋子消息

//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
	//控件变量
protected:
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

	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	LRESULT OnDiceCast(WPARAM wParam, LPARAM lParam);
	LRESULT OnChessChoose(WPARAM wParam, LPARAM lParam);
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	bool OnSubDiceCast(const void * pBuffer, WORD wDataSize);
	bool OnSubChessChoose(const void * pBuffer, WORD wDataSize);
	bool OnSubUserLeft(const void * pBuffer, WORD wDataSize);
	bool OnSubUserGiveUp(const void * pBuffer, WORD wDataSize);
	bool OnSubUserEnter(const void * pBuffer, WORD wDataSize);
	bool OnSubUserReady(const void * pBuffer, WORD wDataSize);
	bool OnSubUserAutomatism(const void * pBuffer, WORD wDataSize);
	bool OnSubUserTimeOut(const void * pBuffer, WORD wDataSize);
	LRESULT OnUserTimeOut(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserStart(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserAutomatism(WPARAM wParam, LPARAM lParam);
	//播放选择按钮声音
	LRESULT OnPlayMusic(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
