#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
const E_SICBO_NUMBER CGameLogic::m_enSicboData[MAX_SICBO_NUMBER]=
{
	enSicboNum_One,  enSicboNum_Two,  enSicboNum_Three,
	enSicboNum_Four, enSicboNum_Five, enSicboNum_Six
};

// 赔率
const __int64 CGameLogic::m_i64Loss_Percent[COUNT_AZIMUTH]={
2L /*大 ×2*/,  2L/*小×2*/,  2L/*单×2*/,  2L/*双×2*/,

64L/*3个骰子点数和为4  ×64*/, 32L/*3个骰子点数和为5  ×32*/, 
20L/*3个骰子点数和为6  ×20*/, 14L/*3个骰子点数和为7  ×14*/, 
10L/*3个骰子点数和为8  ×10*/,   8L/*3个骰子点数和为9  ×8*/, 
8L/*3个骰子点数和为10  ×8*/,    8L/*3个骰子点数和为11  ×8*/,
8L/*3个骰子点数和为12  ×8*/,   10L/*3个骰子点数和为13  ×10*/,
14L/*3个骰子点数和为14  ×14*/, 20L/*3个骰子点数和为15  ×20*/,
32L/*3个骰子点数和为16  ×32*/, 64L/*3个骰子点数和为17  ×64*/,

2L /*骰点1 */, 2L/*骰点2*/, 2L/*骰点3*/, 2L/*骰点4*/, 2L/*骰点5*/, 2L/*骰点6*/,

6L/*骰点1,2*/, 6L/*骰点1,3*/, 6L/*骰点1,4*/,6L/*骰点1,5*/,6L/*骰点1,6*/,
6L/*骰点2,3*/, 6L/*骰点2,4*/, 6L/*骰点2,5*/,6L/*骰点2,6*/,6L/*骰点3,4*/,
6L/*骰点3,5*/, 6L/*骰点3,6*/, 6L/*骰点4,5*/,6L/*骰点4,6*/,6L/*骰点5,6*/,

12L/*骰点2个1*/,	12L/*骰点2个2*/, 12L/*骰点2个3*/,
12L/*骰点2个4*/, 12L/*骰点2个5*/, 12L/*骰点2个6*/,

210L/*骰点3个1*/,210L/*骰点3个2*/,210L/*骰点3个3*/,
210L/*骰点3个4*/,210L/*骰点3个5*/,210L/*骰点3个6*/,
32L          // 骰点3个相同点数
};

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
	// 骰子有5组，六个,不需要使用牌值定义
	for (BYTE i=0; i<cbBufferCount; ++i)
	{
		cbCardBuffer[i]=(BYTE)(rand()%6 + 1);
	}
	return;
}

// 排序
void CGameLogic::SortCards(BYTE byCardsBuffer[], BYTE byCardCount)
{
	// 从大到小排列
	for (BYTE i=1; i<byCardCount; ++i)
	{
		for (BYTE j=0; j<i; ++j)
		{
			if (byCardsBuffer[i]>byCardsBuffer[j])
			{
				BYTE byCardTemp = byCardsBuffer[i];
				byCardsBuffer[i] = byCardsBuffer[j];
				byCardsBuffer[j] = byCardTemp;
			}
		}
	}
}


int CGameLogic::GetSicboCountByNumber(const E_SICBO_NUMBER enSicboBuffer[], E_SICBO_NUMBER enSicboNumber)
{
	// 获取点数个数
	int i=0;
	int iCount = 0;
	for (i=0; i<MAX_COUNT_SICBO; ++i)
	{
		if (enSicboNumber == enSicboBuffer[i])
		{
			++iCount;
		}
	}
	return iCount;
}

// 单双
E_CARD_TYPE CGameLogic::IsSicboSingleDouble(const E_SICBO_NUMBER enSicboNumber[])
{
	// 计算骰子点数和
	BYTE bySum = 0;
	for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
	{
		bySum += enSicboNumber[i];
	}
	return ((0 == bySum%2)?enCardType_Double:enCardType_Single);
}

// 3个骰子点数和
E_CARD_TYPE CGameLogic::IsSicboNumber(const E_SICBO_NUMBER enSicboNumber[])
{
	// 计算骰子点数和
	BYTE bySum = 0;
	for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
	{
		bySum += enSicboNumber[i];
	}
	if ((bySum<enCardType_NumberFour) || (bySum>enCardType_NumberSeventeen))
	{
		return enCardType_Illegal;
	}
	return ((E_CARD_TYPE)bySum);
}


// 获取牌型
bool CGameLogic::GetCardType(E_SICBO_NUMBER enCardsBuffer[], E_CARD_TYPE enCardType[], int &iCardTypeCount)
{
	iCardTypeCount = 0;
	// 先要找出围骰
	// 获取点数个数
	int i=0;
	for (i=0; i<MAX_SICBO_NUMBER; ++i)
	{
		int iCount = GetSicboCountByNumber(enCardsBuffer, m_enSicboData[i]);
		if (MAX_COUNT_SICBO == iCount)
		{
			enCardType[iCardTypeCount]=enCardType_SicboThreeSame;
			++iCardTypeCount;

			enCardType[iCardTypeCount]=(E_CARD_TYPE)(enCardType_SicboThreeOne+i);
			++iCardTypeCount;

			enCardType[iCardTypeCount]=(E_CARD_TYPE)(enCardType_SicboOne+i);
			++iCardTypeCount;
			return true;  // 不会有其他的牌型了

		}
		else if (iCount>0) // 不存在围骰
		{
			break;
		}
	}

	// 获取点数
	E_CARD_TYPE enCardTypeTemp = IsSicboNumber(enCardsBuffer);	
	if (enCardType_Illegal != enCardTypeTemp)
	{
		enCardType[iCardTypeCount]=enCardTypeTemp;
		++iCardTypeCount;

		// 获取大小
		if (enCardTypeTemp>enCardType_NumberTen)
		{
			enCardType[iCardTypeCount]=enCardType_Big;			
		}
		else
		{
			enCardType[iCardTypeCount]=enCardType_Small;
		}
		++iCardTypeCount;
	}

	// 获取单双
	enCardTypeTemp = IsSicboSingleDouble(enCardsBuffer);
	if (enCardType_Illegal != enCardTypeTemp)
	{
		enCardType[iCardTypeCount]=enCardTypeTemp;
		++iCardTypeCount;
	}

	// 获取点数个数
	for (i=0; i<MAX_SICBO_NUMBER; ++i)
	{
		int iCount = GetSicboCountByNumber(enCardsBuffer, m_enSicboData[i]);
		if (iCount > 0)
		{
			enCardType[iCardTypeCount]=(E_CARD_TYPE)(enCardType_SicboOne+i);
			++iCardTypeCount;

			// 有一个才有两个
			if (iCount> 1)
			{
				enCardType[iCardTypeCount]=(E_CARD_TYPE)(enCardType_SicboDoubleOne+i);
				++iCardTypeCount;
			}

			// 散牌
			for (int j=(i+1); j<MAX_SICBO_NUMBER; ++j)
			{
				int iCount2 = GetSicboCountByNumber(enCardsBuffer, m_enSicboData[j]);
				if (iCount2 > 0)
				{
					enCardType[iCardTypeCount]=(E_CARD_TYPE)(enCardType_SicboOneAndTwo+(i*MAX_SICBO_NUMBER+j)-(((i+1)*(i+2))/2));
					++iCardTypeCount;
				}
			}
		}
	}
	return true;
}