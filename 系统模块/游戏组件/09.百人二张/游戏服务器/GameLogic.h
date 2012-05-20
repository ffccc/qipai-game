#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//牌型
#define	CT_ERROR					0									//错误类型
#define	CT_POINT					1									//点数类型
#define	CT_SPECIAL_11				2									//特殊类型
#define	CT_SPECIAL_10				3									//特殊类型
#define	CT_SPECIAL_9				4									//特殊类型
#define	CT_SPECIAL_8				5									//特殊类型
#define	CT_SPECIAL_7				6									//特殊类型
#define	CT_SPECIAL_6				7									//特殊类型
#define	CT_SPECIAL_5				8									//特殊类型
#define	CT_SPECIAL_4				9									//特殊类型
#define	CT_SPECIAL_3				10									//特殊类型
#define	CT_SPECIAL_2				11									//特殊类型
#define	CT_SPECIAL_1				12									//特殊类型

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//排序类型
#define	ST_VALUE					1									//数值排序
#define	ST_LOGIC					2									//逻辑排序

//扑克数目
#define CARD_COUNT					32									//扑克数目
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
		if(cbCardData == 0x42)
		{
			BYTE bcTemp = 0x31;
			return  bcTemp&LOGIC_MASK_VALUE; 
		}
		if(cbCardData==0x23)
		{
			BYTE bcTemp = 0x33;
			return  bcTemp&LOGIC_MASK_VALUE; 

		}
		if(cbCardData == 0x0B)
		{
			BYTE bcTemp = 0x1B;
			return  bcTemp&LOGIC_MASK_VALUE; 
		}
		if(cbCardData == 0x2B)
		{
			BYTE bcTemp = 0x3B;
			return  bcTemp&LOGIC_MASK_VALUE; 
		}
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//获取花色
	BYTE GetCardColor(BYTE cbCardData)
	{
		if(cbCardData == 0x42)
		{
			BYTE bcTemp = 0x31;
			return  (bcTemp&LOGIC_MASK_COLOR)>>4; 
		}
		if(cbCardData==0x23)
		{
			BYTE bcTemp = 0x33;
			return  (bcTemp&LOGIC_MASK_COLOR)>>4; 

		}
		if(cbCardData == 0x0B)
		{
			BYTE bcTemp = 0x1B;
			return  (bcTemp&LOGIC_MASK_COLOR)>>4; 
		}
		if(cbCardData == 0x2B)
		{
			BYTE bcTemp = 0x3B;
			 return  (bcTemp&LOGIC_MASK_COLOR)>>4; 
		}

		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//控制函数
public:
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//逻辑函数
public:
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//获取牌型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//大小比较
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount);
	//逻辑大小
	BYTE GetCardLogicValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
