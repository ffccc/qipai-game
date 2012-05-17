#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//结束信息
struct tagScoreViewInfo
{
	//胜利信息
	BYTE							cbHuCard;							//胡牌扑克
	WORD							wWinUser;							//胜利用户
	WORD							wProvideUser;						//放跑用户

	//成绩变量
	LONG							lGameTax;							//游戏税收
	LONG							lGameScore[GAME_PLAYER];			//游戏积分
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//用户名字

	//胡牌信息
	tagHuCardInfo					HuCardInfo;							//胡牌信息
};

//////////////////////////////////////////////////////////////////////////

//积分视图类
class CScoreView : public CWnd
{
	//变量定义
protected:
	CSkinImage						m_ImageBack;						//背景图案
	tagScoreViewInfo				m_ScoreViewInfo;					//积分信息

	//函数定义
public:
	//构造函数
	CScoreView(void);
	//析构函数
	virtual ~CScoreView(void);
	
	//功能函数
public:
	//重置积分
	void ResetScore();
	//设置信息
	void SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo, bool bShowWnd);

	//消息函数
protected:
	//重画函数
	afx_msg void OnPaint();
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif