#ifndef SCORE_CONTROL_HEAD_FILE
#define SCORE_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//积分信息
struct tagScoreInfo
{
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];		//用户名字

	//用户信息
	WORD							wMeChairId;								//
	BYTE							cbWinOrder;								//胡牌排名

	//积分信息
	WORD							wProvideUser;							//供应用户
	LONG							lGameScore;								//游戏积分
	LONG							lGameTax;								//

	DWORD							dwChiHuRight[MAX_RIGHT_COUNT];			//

	LONG							lGangScore;								//详细得分
	BYTE							cbGenCount;								//
	WORD							wLostFanShu[GAME_PLAYER];				//
};

//////////////////////////////////////////////////////////////////////////

//积分控件
class CScoreControl
{
	//变量定义
protected:
	tagScoreInfo					m_ScoreInfo;							//积分信息
	CChiHuRight						m_ChiHuRight;							//
	bool							m_bShow;								//

	//位置变量
protected:
	CPoint							m_ptBenchmark;							//

	//资源变量
protected:
	CPngImage						m_ImageGameWin;							//积分视图
	CPngImage						m_ImageGameLost;						//
	CPngImage						m_PngNum;								//

	//函数定义
public:
	//构造函数
	CScoreControl();
	//析构函数
	virtual ~CScoreControl();

	//功能函数
public:
	//复位数据
	void RestorationData();
	//设置积分
	void SetScoreInfo(const tagScoreInfo & ScoreInfo);
	//
	void DrawScoreControl( CDC *pDC );
	//
	void SetBenchmarkPos( int nXPos, int nYPos );

	//辅助函数
protected:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, bool bDrawCenter);
	//绘画数字
	void DrawNumberString( CDC *pDC, CPngImage &PngNumber, int nXPos, int nYPos, LONG lNumber, bool bDrawCenter);
};

//////////////////////////////////////////////////////////////////////////

#endif
