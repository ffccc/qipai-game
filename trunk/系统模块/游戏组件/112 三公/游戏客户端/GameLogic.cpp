#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE CGameLogic::m_cbCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
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

//获取类型
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	//效验数据
	ASSERT(cbCardCount==MAX_COUNT);
	if (cbCardCount!=MAX_COUNT) return 0;

	//获取数值
	BYTE cbValueList[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbValueList[i]=GetCardValue(cbCardData[i]);

	//三条判断
	if ((cbValueList[0]==cbValueList[1])&&(cbValueList[0]==cbValueList[2]))	return CT_THREE_KING;

	//三公判断
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbValueList[i]<=10) break;
	}
	if (i==cbCardCount) return CT_THREE_JQK;

	return CT_VALUES;
}

//获取倍数
BYTE CGameLogic::GetCardTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	//获取类型
	BYTE cbCardType=GetCardType(cbCardData,cbCardCount);

	//获取倍数
	switch (cbCardType)
	{
	case CT_VALUES:			//点值类型
		{
			//变量定义
			BYTE cbValueList=0;
			BYTE cbValueCard=0;

			//获取数值
			for (BYTE i=0;i<cbCardCount;i++)
			{
				cbValueCard=GetCardValue(cbCardData[i]);
				if (cbValueCard<10) cbValueList+=cbValueCard;
			}

			return ((cbValueList%10)>=8)?2:1;
		}
	case CT_THREE_JQK:		//三公类型
		{
			return 3;
		}
	case CT_THREE_KING:		//三条类型
		{
			return (GetCardValue(cbCardData[0])>10)?9:5;
		}
	}

	return 0;
}

//获取点数
BYTE CGameLogic::GetCardValue(BYTE cbCardData[], BYTE cbCardCount)
{
	//变量定义
	BYTE cbValueList=0;

	//计算点数
	for (BYTE i=0;i<MAX_COUNT;i++)
	{
		BYTE cbCardValue=GetCardValue(cbCardData[i]);
		if (cbCardValue<10) cbValueList=(cbValueList+cbCardValue)%10;
	}

	return cbValueList;
}

//排列扑克
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//效验参数
	ASSERT(cbCardCount==MAX_COUNT);
	if (cbCardCount!=MAX_COUNT) return;

	//转换数值
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);

	//排序操作
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
				//交换位置
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

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(m_cbCardListData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(m_cbCardListData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//扑克属性
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//属性调整
	if (cbCardValue==10) cbCardValue=0;

	//转换数值
	return (cbCardValue*4)+(cbCardColor>>4);
}

//对比扑克
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	//获取类型
	BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
	BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

	//类型判断
	if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);

	//简单类型
	switch(cbFirstType)
	{
	case CT_THREE_JQK:		//三公类型
	case CT_THREE_KING:		//三条类型
		{
			//对比散牌
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextData[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstData[0]);

			//对比散牌
			return cbFirstLogicValue>cbNextLogicValue;
		}
	case CT_VALUES:			//点值类型
		{
			//获取数值
			BYTE cbNextValueList=0;
			BYTE cbFirstValueList=0;
			for (BYTE i=0;i<MAX_COUNT;i++)
			{
				BYTE cbNextCardValue=GetCardValue(cbNextData[i]);
				BYTE cbFirstCardValue=GetCardValue(cbFirstData[i]);
				if (cbNextCardValue<10) cbNextValueList=(cbNextValueList+cbNextCardValue)%10;
				if (cbFirstCardValue<10) cbFirstValueList=(cbFirstValueList+cbFirstCardValue)%10;
			}

			//点值对比
			if (cbFirstValueList!=cbNextValueList) return cbFirstValueList>cbNextValueList;

			//获取公数
			BYTE cbNextCountJQK=0;
			BYTE cbFirstCountJQK=0;
			for (BYTE i=0;i<MAX_COUNT;i++)
			{
				if (GetCardValue(cbNextData[i])>10) cbNextCountJQK++;
				if (GetCardValue(cbFirstData[i])>10) cbFirstCountJQK++;
			}

			//公数对比
			if (cbFirstCountJQK!=cbNextCountJQK) return cbFirstCountJQK>cbNextCountJQK;

			//对比散牌
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextData[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstData[0]);

			//对比散牌
			return cbFirstLogicValue>cbNextLogicValue;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
