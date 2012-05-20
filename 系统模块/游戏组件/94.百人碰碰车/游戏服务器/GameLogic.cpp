#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
 BYTE CGameLogic::m_cbCardListData[172-4]=
{
	    2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16, 18,20,22,24,26,28,30,32,\
		1,9,17,25,3,11,19,27,3,11,19,27, 5,13,21,29,5,\
		13,21,29,5,13,21,29,7,15,23,31,7,15,23,31,7,15,23,31,7,15,23,31,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,\
		2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32\
	
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
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=(BYTE)(rand()+GetTickCount()*2)%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);


	return;
}
//获取牌型
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE *bcOutCadDataWin )
{
	ASSERT(1==cbCardCount);
	if (!(1==cbCardCount) )return 0;
	memset(bcOutCadDataWin,0,AREA_COUNT);
	BYTE bcData =cbCardData[0];

	/*if(1<=cbCardData[0]<=4)
	{
		bcOutCadDataWin[0]= 40;
	}
	else if(5<=cbCardData[0]<=8)
	{
		bcOutCadDataWin[1]= 30;

	}else if(9<=cbCardData[0]<=12)
	{
		bcOutCadDataWin[2]= 20;

	}else if(13<=cbCardData[0]<=16)
	{
		bcOutCadDataWin[3]= 10;

	}else if(17<=cbCardData[0]<=20)
	{
		bcOutCadDataWin[4]= 5;

	}else if(21<=cbCardData[0]<=24)
	{
		bcOutCadDataWin[5]= 5;

	}else if(25<=cbCardData[0]<=28)
	{
		bcOutCadDataWin[6]= 5;

	}else if(29<=cbCardData[0]<=32)
	{
		bcOutCadDataWin[7]= 5;

	}*/

	if(1==bcData||bcData==2||bcData==1+8||bcData==2+8||bcData==1+2*8||bcData==2+2*8||bcData==1+3*8||bcData==2+3*8)
	{
		if(bcData%2==1)
			bcOutCadDataWin[0]= 40-1;
		else
			bcOutCadDataWin[4]= 5-1;

	}
	else if(3==bcData||bcData==4||bcData==3+8||bcData==4+8||bcData==3+2*8||bcData==4+2*8||bcData==3+3*8||bcData==4+3*8)
	{
		if(bcData%2==1)
			bcOutCadDataWin[1]= 30-1;
		else
			bcOutCadDataWin[5]= 5-1;



	}else if(5==bcData||bcData==6||bcData==5+8||bcData==6+8||bcData==5+2*8||bcData==6+2*8||bcData==5+3*8||bcData==6+3*8)
	{
		if(bcData%2==1)
			bcOutCadDataWin[2]= 20-1;
		else
			bcOutCadDataWin[6]= 5-1;

	}else if(7==bcData||bcData==8||bcData==7+8||bcData==8+8||bcData==7+2*8||bcData==8+2*8||bcData==7+3*8||bcData==8+3*8)
	{
		if(bcData%2==1)
			bcOutCadDataWin[3]= 10-1;
		else
			bcOutCadDataWin[7]= 5-1;

	}

	return true;
	
}

//大小比较
/*
cbNextCardData>cbFirstCardData  返回1
cbNextCardData<cbFirstCardData  返回-1
cbNextCardData==cbFirstCardData 返回0
*/
//Multiple 比较出来的倍数
int CGameLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple)
{
	//合法判断
	ASSERT(1==cbFirstCardCount && 1==cbNextCardCount);
	if (!(1==cbFirstCardCount && 1==cbNextCardCount)) return 0;

	return true;
}

//获取牌点
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//变量定义
	BYTE cbPipCount=0;

	//获取牌点
	BYTE cbCardValue=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		cbCardValue=GetCardValue(cbCardData[i]);
		if(cbCardValue>10)
		{
			cbCardValue = 10;
			
		}
		cbPipCount+=cbCardValue;
	}
	return (cbPipCount%10);
}
BYTE CGameLogic::GetCardNewValue(BYTE cbCardData)
{
	//扑克属性
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//转换数值
	if (cbCardColor==0x04) return cbCardValue+13+2;
	return cbCardValue;

}
//逻辑大小
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	BYTE cbValue=GetCardValue(cbCardData);

	//获取花色
	BYTE cbColor=GetCardColor(cbCardData);

	if(cbValue>10)
	{
		cbValue = 10;

	}
	if(cbColor==0x4)
	{
		return 11;
	}
	return cbValue;
}

//排列扑克
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//数目过虑
	if (cbCardCount==0) return;

	//转换数值
	BYTE cbSortValue[CARD_COUNT];
	if (ST_VALUE==cbSortType)
	{
		for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardValue(cbCardData[i]);	
	}
	else 
	{
		if(cbSortType==ST_NEW)
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardNewValue(cbCardData[i]);	

		}else
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	

		}
		
	}


	//排序操作
	bool bSorted=true;
	BYTE cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//交换位置
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return;
}

//////////////////////////////////////////////////////////////////////////

