#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//命令定义
#define IDM_CONTINUE			(WM_USER+200)							//继续消息

//////////////////////////////////////////////////////////////////////////

//积分视图类
class CScoreView : public CDialog
{
	//状态变量
protected:
	UINT							m_uLeaveTime;						//剩余时间

	//变量定义
protected:
	LONG							m_lRevenue[GAME_PLAYER];			//游戏税收
	LONG							m_lGameScore[GAME_PLAYER];			//游戏得分
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//用户名字

	//资源变量
protected:
	CSkinImage						m_ImageBack;						//背景图案

	//控件变量
public:
	CSkinButton						m_btContinue;						//继续按钮
	CSkinButton						m_btQuitGame;						//退出按钮

	//函数定义
public:
	//构造函数
	CScoreView(void);
	//析构函数
	virtual ~CScoreView(void);
	
	//重载函数
public:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK() { return; }
	//取消消息
	virtual void OnCancel() { return; }

	//功能函数
public:
	//重置积分
	void ResetScore();
	//显示成绩
	void ShowScore();
	//设置积分
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lGameScore, LONG lRevenue);

	//消息函数
protected:
	//重画函数
	afx_msg void OnPaint();
	//继续按钮
	afx_msg void OnContinue();
	//离开按钮
	afx_msg void OnQuitGame();
	//时间消息
	afx_msg void OnTimer(UINT nIDEvent);
	//绘画背景
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
