#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//牌型

enum emCardType
{
	CT_ERROR			=		0,									//错误类型
	CT_POINT			=		1,									//点数类型
	CT_SPECIAL_NIU1		=		2,									//对子类型
	CT_SPECIAL_NIU3		=		3,									//白板对子		--最大
};

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//排序类型
#define	ST_VALUE					1									//数值排序
#define	ST_NEW					    3									//数值排序
#define	ST_LOGIC					2									//逻辑排序

//扑克数目
#define CARD_COUNT					36									//扑克数目
//////////////////////////////////////////////////////////////////////////

//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[CARD_COUNT];		//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData)
	{
		return cbCardData & LOGIC_MASK_VALUE;
	}

	//获取花色
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR) >> 4;
	}

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//逻辑函数
public:
	//单一牌点
	BYTE GetCardPip(BYTE cbCardData);
	//数组牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//获取牌型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//大小比较
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount, const BYTE cbNextCardData[], BYTE cbNextCardCount, BYTE &Multiple);
};

//////////////////////////////////////////////////////////////////////////

#endif
