#ifndef RECORD_GAME_LIST_HEAD_FILE
#define RECORD_GAME_LIST_HEAD_FILE

#define HISTORY_RECORD_SHOW			10									//每页显示的历史记录数
#pragma once

#include "Stdafx.h"

//游戏记录
class CGameRecord /*: public CGameWnd*/
{
	//历史数据
public:
	WORD							m_wPageNO;							//当前显示页
	CArrayTemplate<tagHistory>		m_HistoryArray;						//开奖结果历史记录
	WORD							m_wHistoryWinCount[3];				//历史记录庄闲和得局数

	BYTE                            m_Cardsot[12];



public:

	//函数定义
public:
	//构造函数
	CGameRecord();
	//析构函数
	virtual ~CGameRecord();

	//功能函数
public:
	void OnDraw(SURFACE* psurfTarget,SURFACE* psurfSource,int nx,int ny);

public:

   void  DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource1,SURFACE* psurfSource2, LONG lNumber, INT nXPos, INT nYPos, bool isleft,BYTE cbNumCount);

   //绘画数字
   void DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource, SURFACE* psurfSource2,LONG lNumber, INT nXPos, INT nYPos, BYTE cbNumCount);
   //绘画数字
   void DrawNumberString(SURFACE* psurfTarget, SURFACE* psurfSource, LONG lNumber, INT nXPos, INT nYPos);

	//历史开奖结果
	void SetHistoryResult(tagHistory[], WORD Count);

	//历史开奖结果
	void AddHistoryResult(BYTE cbPlayerValue);
};

//////////////////////////////////////////////////////////////////////////

#endif