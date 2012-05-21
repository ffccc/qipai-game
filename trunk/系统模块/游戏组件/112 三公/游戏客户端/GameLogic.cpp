#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
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

//��ȡ����
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч������
	ASSERT(cbCardCount==MAX_COUNT);
	if (cbCardCount!=MAX_COUNT) return 0;

	//��ȡ��ֵ
	BYTE cbValueList[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbValueList[i]=GetCardValue(cbCardData[i]);

	//�����ж�
	if ((cbValueList[0]==cbValueList[1])&&(cbValueList[0]==cbValueList[2]))	return CT_THREE_KING;

	//�����ж�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbValueList[i]<=10) break;
	}
	if (i==cbCardCount) return CT_THREE_JQK;

	return CT_VALUES;
}

//��ȡ����
BYTE CGameLogic::GetCardTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	//��ȡ����
	BYTE cbCardType=GetCardType(cbCardData,cbCardCount);

	//��ȡ����
	switch (cbCardType)
	{
	case CT_VALUES:			//��ֵ����
		{
			//��������
			BYTE cbValueList=0;
			BYTE cbValueCard=0;

			//��ȡ��ֵ
			for (BYTE i=0;i<cbCardCount;i++)
			{
				cbValueCard=GetCardValue(cbCardData[i]);
				if (cbValueCard<10) cbValueList+=cbValueCard;
			}

			return ((cbValueList%10)>=8)?2:1;
		}
	case CT_THREE_JQK:		//��������
		{
			return 3;
		}
	case CT_THREE_KING:		//��������
		{
			return (GetCardValue(cbCardData[0])>10)?9:5;
		}
	}

	return 0;
}

//��ȡ����
BYTE CGameLogic::GetCardValue(BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	BYTE cbValueList=0;

	//�������
	for (BYTE i=0;i<MAX_COUNT;i++)
	{
		BYTE cbCardValue=GetCardValue(cbCardData[i]);
		if (cbCardValue<10) cbValueList=(cbValueList+cbCardValue)%10;
	}

	return cbValueList;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч�����
	ASSERT(cbCardCount==MAX_COUNT);
	if (cbCardCount!=MAX_COUNT) return;

	//ת����ֵ
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);

	//�������
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbLogicValue[i]<cbLogicValue[i+1])||
				((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(m_cbCardListData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(m_cbCardListData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//���Ե���
	if (cbCardValue==10) cbCardValue=0;

	//ת����ֵ
	return (cbCardValue*4)+(cbCardColor>>4);
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
	BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

	//�����ж�
	if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);

	//������
	switch(cbFirstType)
	{
	case CT_THREE_JQK:		//��������
	case CT_THREE_KING:		//��������
		{
			//�Ա�ɢ��
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextData[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstData[0]);

			//�Ա�ɢ��
			return cbFirstLogicValue>cbNextLogicValue;
		}
	case CT_VALUES:			//��ֵ����
		{
			//��ȡ��ֵ
			BYTE cbNextValueList=0;
			BYTE cbFirstValueList=0;
			for (BYTE i=0;i<MAX_COUNT;i++)
			{
				BYTE cbNextCardValue=GetCardValue(cbNextData[i]);
				BYTE cbFirstCardValue=GetCardValue(cbFirstData[i]);
				if (cbNextCardValue<10) cbNextValueList=(cbNextValueList+cbNextCardValue)%10;
				if (cbFirstCardValue<10) cbFirstValueList=(cbFirstValueList+cbFirstCardValue)%10;
			}

			//��ֵ�Ա�
			if (cbFirstValueList!=cbNextValueList) return cbFirstValueList>cbNextValueList;

			//��ȡ����
			BYTE cbNextCountJQK=0;
			BYTE cbFirstCountJQK=0;
			for (BYTE i=0;i<MAX_COUNT;i++)
			{
				if (GetCardValue(cbNextData[i])>10) cbNextCountJQK++;
				if (GetCardValue(cbFirstData[i])>10) cbFirstCountJQK++;
			}

			//�����Ա�
			if (cbFirstCountJQK!=cbNextCountJQK) return cbFirstCountJQK>cbNextCountJQK;

			//�Ա�ɢ��
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextData[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstData[0]);

			//�Ա�ɢ��
			return cbFirstLogicValue>cbNextLogicValue;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
