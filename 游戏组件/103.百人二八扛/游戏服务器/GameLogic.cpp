#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
const BYTE CGameLogic::m_cbCardListData[CARD_COUNT] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,	// 1 - 9 筒
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,	// 1 - 9 筒
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,	// 1 - 9 筒
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,	// 1 - 9 筒
//	0x0A, 0x0A, 0x0A, 0x0A									// 白板4张
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData, m_cbCardListData, sizeof(m_cbCardListData));

	//混乱扑克
	BYTE cbRandCount = 0, cbPosition = 0;
	do
	{
		cbPosition = rand() % (CountArray(cbCardData) - cbRandCount);
		cbCardBuffer[cbRandCount++] = cbCardData[cbPosition];
		cbCardData[cbPosition] = cbCardData[CountArray(cbCardData)-cbRandCount];
	}
	while (cbRandCount < cbBufferCount);

	return;
}

//获取牌型
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//合法判断
	ASSERT(CARD_COUNT_OF_ONE == cbCardCount);
	if (CARD_COUNT_OF_ONE != cbCardCount) return CT_ERROR;

	if (cbCardData[0] == 0x0A && cbCardData[1] == 0x0A) return CT_SPECIAL_NIU3;	//白板类型
	else if (cbCardData[0] == cbCardData[1]) return CT_SPECIAL_NIU1;			//对子类型
	else return CT_POINT;	//点数类型
}

//大小比较
/*
-1 : 输，庄家大
 1 : 赢，闲家大
 0 : 平，一样大，即为庄家大
*/

//Multiple 比较出来的倍数
int CGameLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount, const BYTE cbNextCardData[], BYTE cbNextCardCount, BYTE &Multiple)
{
	Multiple = 1;

	//合法判断
	ASSERT(CARD_COUNT_OF_ONE == cbFirstCardCount && CARD_COUNT_OF_ONE == cbNextCardCount);
	if (CARD_COUNT_OF_ONE != cbFirstCardCount || CARD_COUNT_OF_ONE != cbNextCardCount) return 0;

	//获取牌型
	BYTE cbFirstCardType = GetCardType(cbFirstCardData, cbFirstCardCount);
	BYTE cbNextCardType = GetCardType(cbNextCardData, cbNextCardCount);

	//牌型比较
	if (cbFirstCardType != cbNextCardType)
	{
		return (cbFirstCardType - cbNextCardType >= 0) ? -1 : 1;
	}
	else
	{
		switch (cbFirstCardType)
		{
			case CT_POINT:			//点数类型
			{
// 				if (0 == (GetCardListPip(cbFirstCardData, cbFirstCardCount) - GetCardListPip(cbNextCardData, cbNextCardCount)))
// 				{
//  					if ((cbFirstCardData[0] == cbNextCardData[0] && cbFirstCardData[1] == cbNextCardData[1]) ||
//  						(cbFirstCardData[0] == cbNextCardData[1] && cbFirstCardData[1] == cbNextCardData[0]))
//  						return -1;
//  					else
//  					{
//  						if ((cbFirstCardData[0] > cbNextCardData[0] && cbFirstCardData[0] > cbNextCardData[1]) ||
//  							(cbFirstCardData[1] > cbNextCardData[0] && cbFirstCardData[1] > cbNextCardData[1])) return -1;
//  						else
//  							return 1;
//  					}
// 					return -1;
// 				}
// 				else
// 				{
				return (GetCardListPip(cbFirstCardData, cbFirstCardCount) - GetCardListPip(cbNextCardData, cbNextCardCount) >= 0) ? -1 : 1;
//				}
			}
			case CT_SPECIAL_NIU3:	//白板对子		--最大
			{
				return 0;
			}
			case CT_SPECIAL_NIU1:	//对子类型
			{
				return (cbFirstCardData[0] - cbNextCardData[0] >= 0) ? -1 : 1;
			}
		}
	}

	return 0;
}

//获取牌点
BYTE CGameLogic::GetCardPip(BYTE cbCardData)
{
	//扑克属性
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//转换数值
	return cbCardValue;
}

//获取牌点
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//变量定义
	BYTE cbPipCount = 0;
	//获取牌点
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbPipCount += GetCardValue(cbCardData[i]);
	}
	return (cbPipCount % 10);
}

//////////////////////////////////////////////////////////////////////////

