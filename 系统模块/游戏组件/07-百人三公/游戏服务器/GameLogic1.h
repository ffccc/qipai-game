#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
#define MAX_COUNT					5									//最大数目
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码
//扑克类型
#define OX_VALUE0					0									//混合牌型  无牛

#define OX_SMALL_WANG				11									//小王牛
#define OX_BIG_WANG					12									//大王牛
#define OX_DOUBLECOW				13									//牛牛

//#define OX_FOUR_SAME				13									//四条牌型
//#define OX_FOURKING					14									//天王牌型
//#define OX_FIVEKING					15									//天王牌型

//#define OX_THREE_SAME				12									//三条牌型
//////////////////////////////////////////////////////////////////////////
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[54];				//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }


	BYTE GetMaxCardPos(BYTE cbCardData[],BYTE cbCardCount);

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardPip(BYTE cbCardData);
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	
public:
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取倍数
	BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
	//获取牛牛
	bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
	//获取整数
	bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);

	void SortByWang(BYTE cbCardData[],BYTE cbCardCount);

	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);

};

//////////////////////////////////////////////////////////////////////////

#endif
