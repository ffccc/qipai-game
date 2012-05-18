#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//结束信息
struct tagScoreViewInfo
{
	LONG							lDrawScore[2];						//游戏积分
};

//////////////////////////////////////////////////////////////////////////

//积分视图类
class CScoreView : public CDialog
{
	//积分信息
protected:
	LONG							m_lScore[GAME_PLAYER];				//游戏得分
	LONG							m_lDrawScore[2];					//游戏得分
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//用户名字

	//资源变量
protected:
	CSkinImage						m_ImageBack;						//背景图案
	CBitmap                         m_BTbitmap;

	//函数定义
public:
	//构造函数
	CScoreView();
	//析构函数
	virtual ~CScoreView();
	
	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual void OnOK() { return; }
	//取消消息
	virtual void OnCancel() { return; }

	//功能函数
public:
	//设置信息
	void SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo);
	//设置积分
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore);

	//消息函数
protected:
	//重画函数
	afx_msg void OnPaint();
	//绘画背景
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

//////////////////////////////////////////////////////////////////////////

#endif