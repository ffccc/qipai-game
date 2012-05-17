#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
const BYTE CGameLogic::m_cbCardListData[54]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x4E,0x4F

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
		cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//获取牌点
BYTE CGameLogic::GetCardPip(BYTE cbCardData)
{
	//计算牌点
	BYTE cbCardValue=GetCardValue(cbCardData);
	BYTE cbPipCount=(cbCardValue>=10)?0:cbCardValue;

	return cbPipCount;
}

//获取牌点
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//变量定义
	BYTE cbPipCount=0;

	//获取牌点
	for (BYTE i=0;i<cbCardCount;i++)
	{
		cbPipCount=(GetCardPip(cbCardData[i])+cbPipCount)%10;
	}

	return cbPipCount;
}

//////////////////////////////////////////////////////////////////////////



//获取类型
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount==MAX_COUNT);

	BYTE bKingCount=0,bTenCount=0;
	bool bSmallWang(false),bBigWang(false);
	for(BYTE i=0;i<cbCardCount;i++)
	{
		if(cbCardData[i]==0x4E)bSmallWang=true;
		if(cbCardData[i]==0x4F)bBigWang=true;
		//if(bSmallWang || bBigWang)continue;
		//if(GetCardValue(cbCardData[i])>10)
		//{
		//	bKingCount++;
		//}
		//else if(GetCardValue(cbCardData[i])==10)
		//{
		//	bTenCount++;
		//}
	}
	//if(bKingCount==MAX_COUNT) return OX_FIVEKING;
	//else if(bKingCount==MAX_COUNT-1 && bTenCount==1) return OX_FOURKING;

	if(bSmallWang || bBigWang)
	{
		if(bSmallWang && bBigWang)
		{
			return OX_BIG_WANG;
		}
		BYTE cbMaxTwoCardValue(0),cbTemp(0);
		BYTE cbTempValue[2];
		for(BYTE i=0;i<cbCardCount-1;i++)
		{
			if(cbCardData[i]==0x4E || cbCardData[i]==0x4F)
				continue;
			for(BYTE j=i+1;j<cbCardCount;j++)
			{
				if(cbCardData[j]==0x4E || cbCardData[j]==0x4F)
					continue;
				cbTempValue[0]=GetCardLogicValue(cbCardData[i]);
				cbTempValue[1]=GetCardLogicValue(cbCardData[j]);
				cbTemp=(cbTempValue[0]+cbTempValue[1]>10)?(cbTempValue[0]+cbTempValue[1]-10):(cbTempValue[0]+cbTempValue[1]);
				if(cbMaxTwoCardValue<cbTemp)cbMaxTwoCardValue=cbTemp;
			}
		}
		if(cbMaxTwoCardValue==10)
			return bSmallWang?OX_SMALL_WANG:OX_BIG_WANG;
		else
			return cbMaxTwoCardValue;
	}

	BYTE bTemp[MAX_COUNT];
	BYTE bSum=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		bTemp[i]=GetCardLogicValue(cbCardData[i]);
		bSum+=bTemp[i];
	}

	BYTE cbTempValue(0);
	for (BYTE i=0;i<cbCardCount-1;i++)
	{
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			if((bSum-bTemp[i]-bTemp[j])%10==0)
			{
				cbTempValue=((bTemp[i]+bTemp[j])>10)?(bTemp[i]+bTemp[j]-10):(bTemp[i]+bTemp[j]);
				if(cbTempValue==10)
					return OX_DOUBLECOW;
				else
					return cbTempValue;
			}
		}
	}
	return OX_VALUE0;
}

//获取倍数
BYTE CGameLogic::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	if(cbCardCount!=MAX_COUNT)return 0;

	BYTE bTimes=GetCardType(cbCardData,MAX_COUNT);
	if(bTimes<2)return 1;
	if(bTimes>=2 && bTimes<=9)return bTimes;
	else if(bTimes==OX_SMALL_WANG)return 10;
	else if(bTimes==OX_BIG_WANG)return 10;
	else if(bTimes==OX_DOUBLECOW)return 10;

	return 0;
}
BYTE CGameLogic::GetMaxCardPos(BYTE cbCardData[],BYTE cbCardCount)
{
	//BYTE cbPos=0;
	//for(BYTE i=1;i<cbCardCount;i++)
	//{
	//	cbCardData[cbPos]
	//}
	return 1;
}
//获取牛牛
bool CGameLogic::GetOxCard(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount==MAX_COUNT);

	//设置变量
	BYTE bTemp[MAX_COUNT],bTempData[MAX_COUNT];
	CopyMemory(bTempData,cbCardData,sizeof(bTempData));


	bool bSmallWang(false),bBigWang(false);
	for(BYTE i=0;i<cbCardCount;i++)
	{
		if(cbCardData[i]==0x4E)bSmallWang=true;
		if(cbCardData[i]==0x4F)bBigWang=true;
	}
	if(bSmallWang || bBigWang)
	{
		//if(bSmallWang && bBigWang)
		//{
			BYTE cbMaxValue=0;
			for(BYTE i=0;i<cbCardCount-1;i++)
			{
				for(BYTE j=i+1;j<cbCardCount;j++)
				{
					BYTE cbCount=0;
					bool bHaveWang(false);
					BYTE cbTempCardData[5];
					ZeroMemory(cbTempCardData,sizeof(cbTempCardData));
					for(BYTE k=0;k<cbCardCount;k++)
					{
						if(k!=i && k!=j)
						{
							if(bTempData[k]==0x4E || bTempData[k]==0x4F)
							{
								bHaveWang=true;
								break;
							}
							else
							{
								cbTempCardData[cbCount++]=bTempData[k];
							}
						}
					}
					if(!bHaveWang && cbCount==3)
					{
						ASSERT(cbCount==3);
						if(GetCardListPip(cbTempCardData,cbCount)%10==0)
						{
							bHaveWang=true;
						}
					}
					if(bHaveWang)
					{
						BYTE cbTempValue(0);
						if(GetCardColor(bTempData[i])==4 || GetCardColor(bTempData[j])==4)
						{
							cbTempValue=100;
						}
						else
						{
							BYTE cbCard1=GetCardLogicValue(bTempData[i]);
							BYTE cbCard2=GetCardLogicValue(bTempData[j]);
							cbTempValue=(cbCard1+cbCard2)>10?cbCard1+cbCard2-10:cbCard1+cbCard2;
						}
						if(cbTempValue>cbMaxValue)
						{
							cbMaxValue=cbTempValue;
							BYTE bCount=0;
							for (BYTE v=0;v<cbCardCount;v++)
							{
								if(v!=i && v!=j)
								{
									cbCardData[bCount++] = bTempData[v];
								}
							}ASSERT(bCount==3);
							cbCardData[bCount++] = bTempData[i];
							cbCardData[bCount++] = bTempData[j];
						}
					}
					
				}
			//}
		}

		//for (BYTE i=0;i<cbCardCount-1;i++)
		//{
		//	if(cbCardData[i]==0x4E || cbCardData[i]==0x4F)
		//		continue;
		//	bTemp[i]=GetCardLogicValue(cbCardData[i]);
		//	for (BYTE j=i+1;j<cbCardCount;j++)
		//	{
		//		if(cbCardData[j]==0x4E || cbCardData[j]==0x4F)
		//			continue;
		//		bTemp[j]=GetCardLogicValue(cbCardData[j]);

		//		if((bTemp[i]+bTemp[j])%10==0)
		//		{
		//			BYTE bCount=0;
		//			for (BYTE k=0;k<cbCardCount;k++)
		//			{
		//				if(k!=i && k!=j)
		//				{
		//					cbCardData[bCount++] = bTempData[k];
		//				}
		//			}ASSERT(bCount==3);

		//			cbCardData[bCount++] = bTempData[i];
		//			cbCardData[bCount++] = bTempData[j];
		//			return true;
		//		}
		//	}
		//}
		//
		return true;
	}




	BYTE bSum=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		bTemp[i]=GetCardLogicValue(cbCardData[i]);
		bSum+=bTemp[i];
	}

	//查找牛牛
	for (BYTE i=0;i<cbCardCount-1;i++)
	{
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			if((bSum-bTemp[i]-bTemp[j])%10==0)
			{
				BYTE bCount=0;
				for (BYTE k=0;k<cbCardCount;k++)
				{
					if(k!=i && k!=j)
					{
						cbCardData[bCount++] = bTempData[k];
					}
				}ASSERT(bCount==3);

				cbCardData[bCount++] = bTempData[i];
				cbCardData[bCount++] = bTempData[j];

				return true;
			}
		}
	}

	return false;
}

//获取整数
bool CGameLogic::IsIntValue(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE sum=0;
	for(BYTE i=0;i<cbCardCount;i++)
	{
		sum+=GetCardLogicValue(cbCardData[i]);
	}
	ASSERT(sum>0);
	return (sum%10==0);
}

//排列扑克
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//转换数值
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardValue(cbCardData[i]);	

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



//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//扑克属性
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);

	//转换数值
	return (bCardValue>10)?(10):bCardValue;
}

//对比扑克
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount==MAX_COUNT);
	//获取点数
	BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
	BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

	//点数判断
	if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);

	//排序大小
	BYTE bFirstTemp[MAX_COUNT],bNextTemp[MAX_COUNT];
	CopyMemory(bFirstTemp,cbFirstData,cbCardCount);
	CopyMemory(bNextTemp,cbNextData,cbCardCount);
	SortCardList(bFirstTemp,cbCardCount);
	SortCardList(bNextTemp,cbCardCount);

	//比较数值
	BYTE cbNextMaxValue=GetCardValue(bNextTemp[0]);
	BYTE cbFirstMaxValue=GetCardValue(bFirstTemp[0]);
	BYTE cbPosFirst(0),cbPosNext(0);

	while(bFirstTemp[cbPosFirst]==0x4E || bFirstTemp[cbPosFirst]==0x4F)
	{
		++cbPosFirst;
		if(cbPosFirst>cbCardCount-1)break;
		cbFirstMaxValue=GetCardValue(bFirstTemp[cbPosFirst]);
	}
	while(bNextTemp[cbPosNext]==0x4E || bNextTemp[cbPosNext]==0x4F)
	{
		++cbPosNext;
		if(cbPosNext>cbCardCount-1)break;
		cbNextMaxValue=GetCardValue(bNextTemp[cbPosNext]);
	}


	if(cbNextMaxValue!=cbFirstMaxValue)return cbFirstMaxValue>cbNextMaxValue;

	//CString str;
	//str.Format("\n%x  %x",bFirstTemp[cbPosFirst],bNextTemp[cbPosNext]);
	//OutputDebugString(str);
	//比较颜色
	return GetCardColor(bFirstTemp[cbPosFirst]) > GetCardColor(bNextTemp[cbPosNext]);

	return false;
}

