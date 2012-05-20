#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
const E_SICBO_NUMBER CGameLogic::m_enSicboData[MAX_SICBO_NUMBER]=
{
	enSicboNum_One,  enSicboNum_Two,  enSicboNum_Three,
	enSicboNum_Four, enSicboNum_Five, enSicboNum_Six
};

// ����
const __int64 CGameLogic::m_i64Loss_Percent[COUNT_AZIMUTH]={
2L /*�� ��2*/,  2L/*С��2*/,  2L/*����2*/,  2L/*˫��2*/,

64L/*3�����ӵ�����Ϊ4  ��64*/, 32L/*3�����ӵ�����Ϊ5  ��32*/, 
20L/*3�����ӵ�����Ϊ6  ��20*/, 14L/*3�����ӵ�����Ϊ7  ��14*/, 
10L/*3�����ӵ�����Ϊ8  ��10*/,   8L/*3�����ӵ�����Ϊ9  ��8*/, 
8L/*3�����ӵ�����Ϊ10  ��8*/,    8L/*3�����ӵ�����Ϊ11  ��8*/,
8L/*3�����ӵ�����Ϊ12  ��8*/,   10L/*3�����ӵ�����Ϊ13  ��10*/,
14L/*3�����ӵ�����Ϊ14  ��14*/, 20L/*3�����ӵ�����Ϊ15  ��20*/,
32L/*3�����ӵ�����Ϊ16  ��32*/, 64L/*3�����ӵ�����Ϊ17  ��64*/,

2L /*����1 */, 2L/*����2*/, 2L/*����3*/, 2L/*����4*/, 2L/*����5*/, 2L/*����6*/,

6L/*����1,2*/, 6L/*����1,3*/, 6L/*����1,4*/,6L/*����1,5*/,6L/*����1,6*/,
6L/*����2,3*/, 6L/*����2,4*/, 6L/*����2,5*/,6L/*����2,6*/,6L/*����3,4*/,
6L/*����3,5*/, 6L/*����3,6*/, 6L/*����4,5*/,6L/*����4,6*/,6L/*����5,6*/,

12L/*����2��1*/,	12L/*����2��2*/, 12L/*����2��3*/,
12L/*����2��4*/, 12L/*����2��5*/, 12L/*����2��6*/,

210L/*����3��1*/,210L/*����3��2*/,210L/*����3��3*/,
210L/*����3��4*/,210L/*����3��5*/,210L/*����3��6*/,
32L          // ����3����ͬ����
};

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	// ������5�飬����,����Ҫʹ����ֵ����
	for (BYTE i=0; i<cbBufferCount; ++i)
	{
		cbCardBuffer[i]=(BYTE)(rand()%6 + 1);
	}
	return;
}

// ����
void CGameLogic::SortCards(BYTE byCardsBuffer[], BYTE byCardCount)
{
	// �Ӵ�С����
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
	// ��ȡ��������
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

// ��˫
E_CARD_TYPE CGameLogic::IsSicboSingleDouble(const E_SICBO_NUMBER enSicboNumber[])
{
	// �������ӵ�����
	BYTE bySum = 0;
	for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
	{
		bySum += enSicboNumber[i];
	}
	return ((0 == bySum%2)?enCardType_Double:enCardType_Single);
}

// 3�����ӵ�����
E_CARD_TYPE CGameLogic::IsSicboNumber(const E_SICBO_NUMBER enSicboNumber[])
{
	// �������ӵ�����
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


// ��ȡ����
bool CGameLogic::GetCardType(E_SICBO_NUMBER enCardsBuffer[], E_CARD_TYPE enCardType[], int &iCardTypeCount)
{
	iCardTypeCount = 0;
	// ��Ҫ�ҳ�Χ��
	// ��ȡ��������
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
			return true;  // ������������������

		}
		else if (iCount>0) // ������Χ��
		{
			break;
		}
	}

	// ��ȡ����
	E_CARD_TYPE enCardTypeTemp = IsSicboNumber(enCardsBuffer);	
	if (enCardType_Illegal != enCardTypeTemp)
	{
		enCardType[iCardTypeCount]=enCardTypeTemp;
		++iCardTypeCount;

		// ��ȡ��С
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

	// ��ȡ��˫
	enCardTypeTemp = IsSicboSingleDouble(enCardsBuffer);
	if (enCardType_Illegal != enCardTypeTemp)
	{
		enCardType[iCardTypeCount]=enCardTypeTemp;
		++iCardTypeCount;
	}

	// ��ȡ��������
	for (i=0; i<MAX_SICBO_NUMBER; ++i)
	{
		int iCount = GetSicboCountByNumber(enCardsBuffer, m_enSicboData[i]);
		if (iCount > 0)
		{
			enCardType[iCardTypeCount]=(E_CARD_TYPE)(enCardType_SicboOne+i);
			++iCardTypeCount;

			// ��һ����������
			if (iCount> 1)
			{
				enCardType[iCardTypeCount]=(E_CARD_TYPE)(enCardType_SicboDoubleOne+i);
				++iCardTypeCount;
			}

			// ɢ��
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