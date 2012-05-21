#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

#define MAX_COUNT					3									//最大数目

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define CT_VALUES					1									//点值类型
#define CT_THREE_JQK				2									//三公类型
#define CT_THREE_KING				3									//三条类型

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
private:
	static BYTE						m_cbCardListData[52];				//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//属性函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取倍数
	BYTE GetCardTimes(BYTE cbCardData[], BYTE cbCardCount);
	//获取点数
	BYTE GetCardValue(BYTE cbCardData[], BYTE cbCardCount);

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//功能函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
