

#pragma once

#include "Stdafx.h"

//游戏记录
class CGameLZ : public CGameWnd
{
	//历史数据
public:
	WORD							m_wPageNO;							//当前显示页
	CArrayTemplate<tagHistory>		m_HistoryArray;						//开奖结果历史记录
	WORD							m_wHistoryWinCount[3];				//历史记录庄闲和得局数

protected:

	SURFACE*					    m_pImageHistoryWin;					//赢的数字位图

	SURFACE*					    m_pImageRed;							//红圈

	SURFACE*					    m_pImageBlue;							//红圈

	SURFACE*					    	m_pImageGreen;

	SURFACE*					    m_pImageZ;							//庄

	SURFACE*					    m_pImageX;							//闲

	SURFACE*					    m_pImageP;							//平

	SURFACE*					    m_pImageD;							//点

	SURFACE*					    m_pImageBG;							//背景图

	CGameButton						m_pImageClose;						//关闭按键

public:
//	筹码按钮点击事件
	LRESULT OnButtonClick(CGameWnd* pGameWnd, WPARAM wParam, LPARAM lParam);

	//函数定义
public:
	//构造函数
	CGameLZ();
	//析构函数
	virtual ~CGameLZ();

	//功能函数
public:
	virtual void OnDraw(SURFACE* psurfTarget);
	virtual void OnSize();
	virtual bool OnCreate();
	LRESULT OnWndMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	//绘画数字
	void DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource, LONG lNumber, INT nXPos, INT nYPos, BYTE cbNumCount);
	//历史开奖结果
	void SetHistoryResult(tagHistory[], WORD Count);
	void ReHistoryResult();
	//历史开奖结果
	void AddHistoryResult(BYTE cbPlayerValue, BYTE cbBankerValue, int bRefresh,bool Banker,bool Player, bool big);

};

//////////////////////////////////////////////////////////////////////////

