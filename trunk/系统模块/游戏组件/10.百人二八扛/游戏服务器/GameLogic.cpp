#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
const BYTE CGameLogic::m_cbCardListData[CARD_COUNT] =
{
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,	// 1 - 9 Ͳ
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,	// 1 - 9 Ͳ
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,	// 1 - 9 Ͳ
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,	// 1 - 9 Ͳ
//	0x0A, 0x0A, 0x0A, 0x0A									// �װ�4��
};

//////////////////////////////////////////////////////////////////////////

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
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData, m_cbCardListData, sizeof(m_cbCardListData));

	//�����˿�
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

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//�Ϸ��ж�
	ASSERT(CARD_COUNT_OF_ONE == cbCardCount);
	if (CARD_COUNT_OF_ONE != cbCardCount) return CT_ERROR;

	if (cbCardData[0] == 0x0A && cbCardData[1] == 0x0A) return CT_SPECIAL_NIU3;	//�װ�����
	else if (cbCardData[0] == cbCardData[1]) return CT_SPECIAL_NIU1;			//��������
	else return CT_POINT;	//��������
}

//��С�Ƚ�
/*
-1 : �䣬ׯ�Ҵ�
 1 : Ӯ���мҴ�
 0 : ƽ��һ���󣬼�Ϊׯ�Ҵ�
*/

//Multiple �Ƚϳ����ı���
int CGameLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount, const BYTE cbNextCardData[], BYTE cbNextCardCount, BYTE &Multiple)
{
	Multiple = 1;

	//�Ϸ��ж�
	ASSERT(CARD_COUNT_OF_ONE == cbFirstCardCount && CARD_COUNT_OF_ONE == cbNextCardCount);
	if (CARD_COUNT_OF_ONE != cbFirstCardCount || CARD_COUNT_OF_ONE != cbNextCardCount) return 0;

	//��ȡ����
	BYTE cbFirstCardType = GetCardType(cbFirstCardData, cbFirstCardCount);
	BYTE cbNextCardType = GetCardType(cbNextCardData, cbNextCardCount);

	//���ͱȽ�
	if (cbFirstCardType != cbNextCardType)
	{
		return (cbFirstCardType - cbNextCardType >= 0) ? -1 : 1;
	}
	else
	{
		switch (cbFirstCardType)
		{
			case CT_POINT:			//��������
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
			case CT_SPECIAL_NIU3:	//�װ����		--���
			{
				return 0;
			}
			case CT_SPECIAL_NIU1:	//��������
			{
				return (cbFirstCardData[0] - cbNextCardData[0] >= 0) ? -1 : 1;
			}
		}
	}

	return 0;
}

//��ȡ�Ƶ�
BYTE CGameLogic::GetCardPip(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor = GetCardColor(cbCardData);
	BYTE cbCardValue = GetCardValue(cbCardData);

	//ת����ֵ
	return cbCardValue;
}

//��ȡ�Ƶ�
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	BYTE cbPipCount = 0;
	//��ȡ�Ƶ�
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbPipCount += GetCardValue(cbCardData[i]);
	}
	return (cbPipCount % 10);
}

//////////////////////////////////////////////////////////////////////////

