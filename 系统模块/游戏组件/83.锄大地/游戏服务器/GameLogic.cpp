#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//静态变量

//索引变量
const BYTE cbIndexCount=5;

//扑克数据
const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
};

const BYTE CGameLogic::m_cbBigCardData[BIG_COUNT]=
{
	0x01,0x02,0x0A,0x0B,0x0C,0x0D,
	0x11,0x12,0x1A,0x1B,0x1C,0x1D,
	0x21,0x22,0x2A,0x2B,0x2C,0x2D,
	0x31,0x32,0x3A,0x3B,0x3C,0x3D
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
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//无效过虑
	if ((cbCardCount==0)||(cbCardCount>5)) return CT_ERROR;

	//简单牌型
	if (cbCardCount<=4)
	{
		//获取属性
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[0]);
		BYTE cbCardType[]={CT_SINGLE,CT_DOUBLE,CT_THREE_TIAO,CT_FOUR_TIAO};

		//同牌对比
		for (BYTE i=1;i<cbCardCount;i++)
		{
			if (GetCardLogicValue(cbCardData[i])!=cbLogicValue) return CT_ERROR;
		}

		return cbCardType[cbCardCount-1];
	}

	//分析扑克
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//顺子类型
	if (AnalyseResult.cbCardCount[0]==5)
	{
		//牌列属性
		BYTE cbCardColor=GetCardColor(cbCardData,cbCardCount);
		bool bStructureLink=IsStructureLink(cbCardData,cbCardCount);

		//类型判断
		if ((cbCardColor==0xF0)&&(bStructureLink==true)) return CT_SHUN_ZI;
		if ((cbCardColor!=0xF0)&&(bStructureLink==false)) return CT_TONG_HUA;
		if ((cbCardColor!=0xF0)&&(bStructureLink==true)) return CT_TONG_HUA_SHUN;
	}
	else
	{
		//类型判断
		if ((AnalyseResult.cbCardCount[2]==1)&&(AnalyseResult.cbCardCount[1]==1)) return CT_HU_LU;
		if ((AnalyseResult.cbCardCount[3]==1)&&(AnalyseResult.cbCardCount[0]==1)) return CT_TIE_ZHI;
	}

	return CT_ERROR;
}

//获取花色
BYTE CGameLogic::GetCardColor(const BYTE cbCardData[], BYTE cbCardCount)
{
	//效验参数
	ASSERT(cbCardCount>0);
	if (cbCardCount==0) return 0xF0;

	//首牌花色
	BYTE cbCardColor=GetCardColor(cbCardData[0]);

	//花色判断
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (GetCardColor(cbCardData[i])!=cbCardColor) return 0xF0;
	}

	return cbCardColor;
}

//排列扑克
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//数目过虑
	if (cbCardCount==0) return;

	//转换数值
	BYTE cbSortValue[FULL_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) 
	{
		if (cbSortType==ST_ORDER) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
		else cbSortValue[i]=GetCardColor(cbCardData[i])+GetCardLogicValue(cbCardData[i]);
	}

	//排序操作
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//设置标志
				bSorted=false;

				//扑克数据
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;

				//排序权位
				cbSwitchData=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return;
}

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardData[CountArray(m_cbCardData)];
	CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//混乱扑克
void CGameLogic::RandCard(const BYTE bCardData[],BYTE bCardCount,BYTE bCardBuffer[],BYTE bBufferCount)
{
	//混乱准备
	BYTE cbCardData[FULL_COUNT];
	CopyMemory(cbCardData,bCardData,sizeof(BYTE)*bCardCount);

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(bBufferCount-cbRandCount);
		bCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[bBufferCount-cbRandCount];
	} while (cbRandCount<bBufferCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//检验数据
	ASSERT(cbRemoveCount<=cbCardCount);
	if( cbRemoveCount > cbCardCount )
		return false;

	//定义变量
	BYTE cbDeleteCount=0,cbTempCardData[FULL_COUNT];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//置零扑克
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}
	if (cbDeleteCount!=cbRemoveCount) return false;

	//清理扑克
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//获取属性
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//有效判断
	if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) return true;

	return false;
}

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//扑克属性
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//转换数值
	return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//对比扑克
bool CGameLogic::CompareCard(BYTE cbFirstCard, BYTE cbNextCard)
{
	//获取数值
	BYTE cbNextValue=GetCardLogicValue(cbNextCard);
	BYTE cbFirstValue=GetCardLogicValue(cbFirstCard);

	//数值判断
	if (cbFirstValue==cbNextValue)
	{
		//花色判断
		BYTE cbNextColor=GetCardColor(cbNextCard);
		BYTE cbFirstColor=GetCardColor(cbFirstCard);

		return (cbNextColor>cbFirstColor);
	}
		
	return (cbNextValue>cbFirstValue);
}

//对比扑克
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//数目判断
	if (cbFirstCount!=cbNextCount) return false;

	//类型判断
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);
	if (cbFirstType!=cbNextType) return (cbNextType>cbFirstType);

	//开始对比
	switch (cbFirstType)
	{
	case CT_SINGLE:			//单牌类型
	case CT_DOUBLE:			//对子类型
	case CT_THREE_TIAO:		//三条类型
	case CT_FOUR_TIAO:		//四条类型
	case CT_SHUN_ZI:		//顺子类型
	case CT_TONG_HUA:		//同花类型
	case CT_TONG_HUA_SHUN:	//同花顺型
		{
			return CompareCard(cbFirstCard[0],cbNextCard[0]);
		}
	case CT_HU_LU:			//葫芦类型
		{
			return CompareCard(cbFirstCard[2],cbNextCard[2]);
		}
	case CT_TIE_ZHI:		//铁支类型
		{
			return CompareCard(cbFirstCard[1],cbNextCard[1]);
		}
	}

	//错误断言
	ASSERT(FALSE);
	
	return false;
}

//出牌搜索
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
	//设置结果
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//构造扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//出牌判断
	if (cbTurnCardCount!=0)
	{
		//获取类型
		BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

		//简单类型
		switch (cbTurnOutType)
		{
		case CT_SINGLE:					//单牌类型,找最大
			{
				//分析扑克
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

				//获取数值
				BYTE cbTurnCard=cbTurnCardData[0];

				//寻找扑克
				for (BYTE i=cbTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
				{
					if( AnalyseResult.cbCardCount[i] == 0 ) continue;
					if (CompareCard(cbTurnCard,AnalyseResult.cbCardData[i][0])==false) continue;
					else cbTurnCard = AnalyseResult.cbCardData[i][0];	
				}
				if( cbTurnCard != cbTurnCardData[0] )
				{
					//设置结果
					OutCardResult.cbCardCount=cbTurnCardCount;
					OutCardResult.cbResultCard[0] = cbTurnCard;
					return true;
				}
				return false;
			}
		case CT_DOUBLE:					//对牌类型
		case CT_THREE_TIAO:				//三条类型
		case CT_FOUR_TIAO:				//四条类型
			{										//找最小
				//分析扑克
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

				//获取数值
				BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

				//寻找扑克
				for (BYTE i=cbTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
				{
					for (BYTE j=0;j<AnalyseResult.cbCardCount[i];j++)
					{
						//大小对比
						BYTE cbIndex=(AnalyseResult.cbCardCount[i]-j-1)*(i+1);
						if (CompareCard(cbTurnCardData[0],AnalyseResult.cbCardData[i][cbIndex])==false) continue;

						//设置结果
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][cbIndex],sizeof(BYTE)*cbTurnCardCount);
						SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );

						return true;
					}
				}

				return false;
			}
		}

		//组合类型
		if (cbCardCount>=5)
		{
			//分析分布
			tagDistributing Distributing;
			AnalysebDistributing(cbCardData,cbCardCount,Distributing);

			//搜索顺子
			if (cbTurnOutType<=CT_SHUN_ZI)
			{
				//搜索顺子
				BYTE cbReferCard=(cbTurnOutType==CT_SHUN_ZI)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchShunZi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//搜索同花
			if (cbTurnOutType<=CT_TONG_HUA)
			{
				//搜索同花
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHua(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//搜索葫芦
			if (cbTurnOutType<=CT_HU_LU)
			{
				//搜索葫芦
				BYTE cbReferCard=(cbTurnOutType==CT_HU_LU)?cbTurnCardData[2]:0x00;
				OutCardResult.cbCardCount=SearchHuLu(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//搜索铁支
			if (cbTurnOutType<=CT_TIE_ZHI)
			{
				//搜索铁支
				BYTE cbReferCard=(cbTurnOutType==CT_TIE_ZHI)?cbTurnCardData[1]:0x00;
				OutCardResult.cbCardCount=SearchTieZhi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//搜索同花顺
			if (cbTurnOutType<=CT_TONG_HUA_SHUN)
			{
				//搜索同花顺
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA_SHUN)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHuaShun(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}
		}

		return false;
	}
	else
	{
		//获取数值
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[cbCardCount-1]);

		//多牌判断
		BYTE cbSameCount=1;
		for (BYTE i=1;i<cbCardCount;i++)
		{
			if (GetCardLogicValue(cbCardData[cbCardCount-i-1])==cbLogicValue) cbSameCount++;
			else break;
		}

		//完成处理
		if (cbSameCount>1)
		{
			OutCardResult.cbCardCount=cbSameCount;
			for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=cbCardData[cbCardCount-1-j];
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//单牌处理
		OutCardResult.cbCardCount=1;
		OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-1];

		return true;
	}

	return false;
}

//搜索葫芦
BYTE CGameLogic::SearchHuLu(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//变量定义
	BYTE cbThreeIndex=0xFF;
	BYTE cbDoubleIndex=0xFF;

	//搜索葫芦
	for (BYTE cbValueIndex=0;cbValueIndex<12;cbValueIndex++)
	{
		//搜索三条
		if ((cbThreeIndex==0xFF)&&(Distributing.cbDistributing[cbValueIndex][5]>=3))
		{
			//大小判断
			BYTE cbHeadCard=MakeCardData(cbValueIndex,0);
			if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
			{
				cbThreeIndex=cbValueIndex;
				cbValueIndex = 0;
				continue;
			}
		}

		//搜索对牌
		if ((cbDoubleIndex==0xFF)&&cbValueIndex!=cbThreeIndex&&(Distributing.cbDistributing[cbValueIndex][5]>=2))
		{
			cbDoubleIndex=cbValueIndex;
			continue;
		}

		//完成判断
		if ((cbThreeIndex!=0xFF)&&(cbDoubleIndex!=0xFF)) break;
	}

	//构造葫芦
	if ((cbThreeIndex!=0xFF)&&(cbDoubleIndex!=0xFF))
	{
		//变量定义
		BYTE cbThreeCount=0;
		BYTE cbDoubleCount=0;

		//构造三条
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//扑克判断
			if (Distributing.cbDistributing[cbThreeIndex][cbColorIndex]==0) continue;
			
			//构造扑克
			BYTE cbIndex= 0;
			cbResultCard[cbIndex+cbThreeCount++]=MakeCardData(cbThreeIndex,cbColorIndex);

			//完成判断
			if (cbThreeCount==3) break;
		}

		//构造对子
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//扑克判断
			if (Distributing.cbDistributing[cbDoubleIndex][cbColorIndex]==0) continue;
			
			//构造扑克
			BYTE cbIndex= 3;
			cbResultCard[cbIndex+cbDoubleCount++]=MakeCardData(cbDoubleIndex,cbColorIndex);

			//完成判断
			if (cbDoubleCount==2) break;
		}

		return cbThreeCount+cbDoubleCount;
	}

	return 0;
}

//搜索铁支
BYTE CGameLogic::SearchTieZhi(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//变量定义
	BYTE cbFourIndex=0xFF;
	BYTE cbSignedIndex=0xFF;

	//搜索葫芦
	for (BYTE cbValueIndex=0;cbValueIndex<12;cbValueIndex++)
	{
		//搜索四条
		if ((cbFourIndex==0xFF)&&(Distributing.cbDistributing[cbValueIndex][5]>=4))
		{
			//大小判断
			BYTE cbHeadCard=MakeCardData(cbValueIndex,0);
			if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
			{
				cbFourIndex=cbValueIndex;
				continue;
			}
		}

		//搜索单牌
		if ((cbSignedIndex==0xFF)&&(Distributing.cbDistributing[cbValueIndex][5]>=1))
		{
			cbSignedIndex=cbValueIndex;
			continue;
		}

		//完成判断
		if ((cbFourIndex!=0xFF)&&(cbSignedIndex!=0xFF)) break;
	}

	//构造葫芦
	if ((cbFourIndex!=0xFF)&&(cbSignedIndex!=0xFF))
	{
		//变量定义
		BYTE cbFourCount=0;
		BYTE cbSignedCount=0;

		//构造四条
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//扑克判断
			if (Distributing.cbDistributing[cbFourIndex][3-cbColorIndex]==0) continue;
			
			//构造扑克
			BYTE cbIndex= 0;
			cbResultCard[cbIndex+cbFourCount++]=MakeCardData(cbFourIndex,3-cbColorIndex);

			//完成判断
			if (cbFourCount==4) break;
		}

		//构造单牌
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//扑克判断
			if (Distributing.cbDistributing[cbSignedIndex][cbColorIndex]==0) continue;
			
			//构造扑克
			BYTE cbIndex= 4;
			cbResultCard[cbIndex+cbSignedCount++]=MakeCardData(cbSignedIndex,cbColorIndex);

			//完成判断
			if (cbSignedCount==1) break;
		}

		return cbFourCount+cbSignedCount;
	}

	return 0;
}

//搜索顺子
BYTE CGameLogic::SearchShunZi(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//变量定义
	BYTE cbLinkCount=0;
	BYTE cbResultCount=0;

	//普通顺子
	for (BYTE cbValueIndex=0;cbValueIndex<12;cbValueIndex++)
	{
		//继续判断
		if (Distributing.cbDistributing[cbValueIndex][5]==0)
		{
			cbLinkCount=0;
			continue;
		}

		//结束判断
		if ((++cbLinkCount)>=5)
		{
			//获取花色
			BYTE cbHeadColor=0xFF;
			for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
			{
				if (Distributing.cbDistributing[cbValueIndex][3-cbColorIndex]>0)
				{
					cbHeadColor=3-cbColorIndex;
					break;
				}
			}

			//构造顺子
			BYTE cbHeadCard=MakeCardData(cbValueIndex,cbHeadColor);
			if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
			{
				//构造顺子
				for (BYTE k=0;k<5;k++)
				{
					for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
					{
						BYTE bTempColorIndex = (k == 0 ? 3-cbColorIndex:cbColorIndex);
						if (Distributing.cbDistributing[cbValueIndex-k][bTempColorIndex]>0)
						{
							cbResultCard[cbResultCount++]=MakeCardData(cbValueIndex-k,bTempColorIndex);
							break;
						}
					}
				}

				return cbResultCount;
			}
		}
	}

	//特殊顺子
	if (Distributing.cbDistributing[12][5]>0)
	{
		//获取花色
		BYTE cbHeadColor=0xFF;
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			if (Distributing.cbDistributing[12][3-cbColorIndex]>0)
			{
				cbHeadColor=3-cbColorIndex;
				break;
			}
		}

		//构造顺子
		BYTE cbHeadCard=MakeCardData(12,cbHeadColor);
		if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
		{
			//变量定义
			BYTE cbBackCount=0;
			BYTE cbForwardCount=0;

			//前向搜索
			for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
			{
				if (Distributing.cbDistributing[cbColorIndex][5]>0)
				{
					cbForwardCount++;
				}
				else break;
			}

			//反向搜索
			if ((cbForwardCount<4)&&(Distributing.cbDistributing[11][5]>0)) cbBackCount=1;

			//顺子判断
			if ((cbForwardCount+cbBackCount)>=4)
			{
				//索引定义
				BYTE cbValueCount=1;
				BYTE cbValueIndex[5]={12};

				//构造索引
				if (cbBackCount>=1) cbValueIndex[cbValueCount++]=11;
				for (BYTE i=0;i<cbForwardCount;i++) cbValueIndex[cbValueCount++]=(cbForwardCount-i-1);

				//构造顺子
				for (BYTE i=0;i<5;i++)
				{
					for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
					{
						BYTE bTempColorIndex = (i == 0 ? 3-cbColorIndex:cbColorIndex);
						if (Distributing.cbDistributing[cbValueIndex[i]][bTempColorIndex]>0)
						{
							cbResultCard[cbResultCount++]=MakeCardData(cbValueIndex[i],bTempColorIndex);
							break;
						}
					}
				}

				return cbResultCount;
			}
		}
	}

	return cbResultCount;
}

//搜索同花
BYTE CGameLogic::SearchTongHua(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//花色变量
	BYTE cbColorCount[4];
	ZeroMemory(cbColorCount,sizeof(cbColorCount));

	//花色搜索
	for (BYTE cbValueIndex=0;cbValueIndex<13;cbValueIndex++)
	{
		//数目判断
		if (Distributing.cbDistributing[cbValueIndex][5]==0) continue;

		//花色判断
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//扑克判断
			if (Distributing.cbDistributing[cbValueIndex][cbColorIndex]==0) continue;

			//构造判断
			if ((++cbColorCount[cbColorIndex])>=5)
			{
				//大小判断
				BYTE cbHeadCard=MakeCardData(cbValueIndex,cbColorIndex);
				if ((cbReferCard!=0x00)&&(CompareCard(cbReferCard,cbHeadCard)==false)) continue;

				//构造结果
				BYTE cbResultCount=0;
				cbResultCard[cbResultCount++]=cbHeadCard;

				//剩余扑克
				for (BYTE k=0;k<cbValueIndex;k++)
				{
					if (Distributing.cbDistributing[k][cbColorIndex]>0)
					{
						//设置扑克
						cbResultCard[5-cbResultCount++]=MakeCardData(k,cbColorIndex);

						//完成判断
						if (cbResultCount==5) break;
					}
				}

				return cbResultCount;
			}
		}
	}

	return 0;
}

//搜索同花顺
BYTE CGameLogic::SearchTongHuaShun(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//变量定义
	BYTE cbLinkCount[4];
	ZeroMemory(cbLinkCount,sizeof(cbLinkCount));

	//普通顺子
	for (BYTE cbValueIndex=0;cbValueIndex<12;cbValueIndex++)
	{
		//继续判断
		if (Distributing.cbDistributing[cbValueIndex][5]==0)
		{
			ZeroMemory(cbLinkCount,sizeof(cbLinkCount));
			continue;
		}

		//同花搜索
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//扑克判断
			if (Distributing.cbDistributing[cbValueIndex][cbColorIndex]==0) 
			{
				cbLinkCount[cbColorIndex]=0;
				continue;
			}
		
			//结束判断
			if ((++cbLinkCount[cbColorIndex])>=5)
			{
				//构造顺子
				BYTE cbHeadCard=MakeCardData(cbValueIndex,cbColorIndex);
				if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
				{
					//设置结果
					BYTE cbResultCount=0;
					for (BYTE k=0;k<5;k++) cbResultCard[cbResultCount++]=MakeCardData(cbValueIndex-k,cbColorIndex);

					return cbResultCount;
				}
			}
		}
	}

	//特殊顺子
	if (Distributing.cbDistributing[12][5]>0)
	{
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			if( Distributing.cbDistributing[12][cbColorIndex]==0 ) continue;

			//大小判断
			BYTE cbHeadCard=MakeCardData(12,cbColorIndex);
			if ((cbReferCard!=0x00)&&(CompareCard(cbReferCard,cbHeadCard)==false)) continue;

			//变量定义
			BYTE cbBackCount=0;
			BYTE cbForwardCount=0;

			//前向搜索
			for (BYTE i=0;i<4;i++)
			{
				if (Distributing.cbDistributing[i][cbColorIndex]>0)
				{
					cbForwardCount++;
				}
				else break;
			}

			//反向搜索
			if ((cbForwardCount<4)&&(Distributing.cbDistributing[11][cbColorIndex]>0)) cbBackCount=1;

			//顺子判断
			if ((cbForwardCount+cbBackCount)>=4)
			{
				//索引定义
				BYTE cbValueCount=1;
				BYTE cbValueIndex[5]={12};

				//构造索引
				if (cbBackCount>=1) cbValueIndex[cbValueCount++]=11;
				for (BYTE i=0;i<cbForwardCount;i++) cbValueIndex[cbValueCount++]=(cbForwardCount-i-1);

				//构造顺子
				BYTE cbResultCount=0;
				for (BYTE i=0;i<5;i++) cbResultCard[cbResultCount++]=MakeCardData(cbValueIndex[i],cbColorIndex);

				return cbResultCount;
			}
		}
	}

	return 0;
}

//构造扑克
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	//构造扑克
	switch (cbValueIndex)
	{
	case 11:	//A 扑克
		{
			return (cbColorIndex<<4)|0x01;
		}
	case 12:	//2 扑克
		{
			return (cbColorIndex<<4)|0x02;
		}
	case 13:	//小鬼扑克
		{
			return (cbColorIndex<<4)|0x01;
		}
	case 14:	//大鬼扑克
		{
			return (cbColorIndex<<4)|0x02;
		}
	default:	//其他扑克
		{
			return (cbColorIndex<<4)|(cbValueIndex+3);
		}
	}

	return 0x00;
}

//是否连牌
bool CGameLogic::IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount)
{
	//数目判断
	if (cbCardCount!=5) return false;
	
	//变量定义
	BYTE cbFirstValue=GetCardLogicValue(cbCardData[0]);

	//特殊处理
	if (cbFirstValue==15)
	{
		for (BYTE i=1;i<cbCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);
			if ((cbLogicValue>=7)&&(cbLogicValue<=13)) return false;
			if ((cbFirstValue!=(cbLogicValue+i))&&(cbFirstValue!=(cbLogicValue+i+8))) return false;
		}

		return true;
	}

	//分析处理
	for (BYTE i=1;i<cbCardCount;i++)
	{
		if (cbFirstValue!=(GetCardLogicValue(cbCardData[i])+i)) return false;
	}

	return true;
}

//分析扑克
void CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//设置结果
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//扑克分析
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//变量定义
		BYTE cbSameCount=1,cbCardValueTemp=0;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//搜索同牌
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//获取扑克
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//设置变量
			cbSameCount++;
		}

		//设置结果
		switch (cbSameCount)
		{
		case 1:		//单张
			{
				BYTE cbIndex=AnalyseResult.cbCardCount[0]++;
				AnalyseResult.cbCardData[0][cbIndex*cbSameCount]=cbCardData[i];
				break;
			}
		case 2:		//两张
			{
				BYTE cbIndex=AnalyseResult.cbCardCount[1]++;
				AnalyseResult.cbCardData[1][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[1][cbIndex*cbSameCount+1]=cbCardData[i+1];
				break;
			}
		case 3:		//三张
			{
				BYTE cbIndex=AnalyseResult.cbCardCount[2]++;
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount+2]=cbCardData[i+2];
				break;
			}
		case 4:		//四张
			{
				BYTE cbIndex=AnalyseResult.cbCardCount[3]++;
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+3]=cbCardData[i+3];
				break;
			}
		}

		//设置索引
		i+=(cbSameCount-1);
	}

	return;
}

//分析分布
void CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//设置变量
	ZeroMemory(&Distributing,sizeof(Distributing));

	//设置变量
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbCardData[i]==0) continue;

		//获取属性
		BYTE cbCardColor=GetCardColor(cbCardData[i]);
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//分布信息
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbLogicValue-3][cbIndexCount]++;
		Distributing.cbDistributing[cbLogicValue-3][cbCardColor>>4]++;
	}

	return;
}

//分析扑克
void CGameLogic::AnalyseCardData( const BYTE cbCardData[],BYTE cbCardCount,tagAnalysedCard &AnalysedCard )
{
	//初始化
	ZeroMemory( &AnalysedCard,sizeof(tagAnalysedCard) );

	BYTE bCardData[MAX_COUNT];
	BYTE bCardCount = cbCardCount;
	CopyMemory( bCardData,cbCardData,sizeof(BYTE)*cbCardCount );

	//变量定义
	BYTE bDoubleCardCount = 0;
	BYTE bDoubleCardKCount = 0;
	tagAnalyseResult AnalyseResult;

	SortCardList( bCardData,bCardCount );
	AnalysebCardData( bCardData,bCardCount,AnalyseResult );

	BYTE bTempData[MAX_COUNT];
	BYTE bTempCount = 0;

	tagDistributing Distributing;
	AnalysebDistributing( bCardData,bCardCount,Distributing );
	//搜索同花顺
	while( SearchTongHuaShun( Distributing,0x00,bTempData ) != 0 )
	{
		//构造
		bTempCount = AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1][bTempCount*5],bTempData,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData,5,bCardData,bCardCount ) );
		bCardCount -= 5;
		//重新分析分布
		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}

	//搜索四条、三条
	AnalysebCardData( bCardData,bCardCount,AnalyseResult );
	bTempCount = AnalyseResult.cbCardCount[3]*4;
	CopyMemory( bTempData,AnalyseResult.cbCardData[3],sizeof(BYTE)*bTempCount );
	CopyMemory( &bTempData[bTempCount],AnalyseResult.cbCardData[2],sizeof(BYTE)*AnalyseResult.cbCardCount[2]*3 );
	bTempCount += AnalyseResult.cbCardCount[2]*3;
	//移除四条、三条后搜索同花、顺子
	VERIFY( RemoveCard( bTempData,bTempCount,bCardData,bCardCount ) );
	bCardCount -= bTempCount;


	//搜索同花、顺子
	AnalysebDistributing( bCardData,bCardCount,Distributing );
	BYTE bTempData1[5];
	BYTE bTempCount1 = 0;
	while( SearchTongHua( Distributing,0x00,bTempData1 ) == 5 )
	{
		//构造
		bTempCount1 = AnalysedCard.bCardCount[CT_TONG_HUA-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_TONG_HUA-1][bTempCount1*5],bTempData1,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData1,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData1,5,bCardData,bCardCount ) );
		bCardCount -= 5;
		//重新分析分布
		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}
	while( SearchShunZi( Distributing,0x00,bTempData1 ) == 5 )
	{
		//构造
		bTempCount1 = AnalysedCard.bCardCount[CT_SHUN_ZI-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_SHUN_ZI-1][bTempCount1*5],bTempData1,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData1,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData1,5,bCardData,bCardCount ) );
		bCardCount -= 5;

		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}

	//构造铁子
	int nCount4 = 0;	//四条数据
	tagAnalyseResult AnalyseResult1;
	AnalysebCardData( bCardData,bCardCount,AnalyseResult1 );
	if( AnalyseResult.cbCardCount[3] > 0 && 
		(AnalyseResult1.cbCardCount[0]+AnalyseResult1.cbCardCount[1]*2) < AnalyseResult.cbCardCount[3] )		
	{
		//释放顺子
		CopyMemory( &bCardData[bCardCount],AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*AnalysedCard.bCardCount[CT_SHUN_ZI-1]*5 );
		ZeroMemory( AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*MAX_COUNT );
		bCardCount += AnalysedCard.bCardCount[CT_SHUN_ZI-1]*5;
		AnalysedCard.bCardCount[CT_SHUN_ZI-1] = 0;

		SortCardList( bCardData,bCardCount );
		tagAnalyseResult AnalyseResult2;
		AnalysebCardData( bCardData,bCardCount,AnalyseResult2 );
		
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = 0; i < 2; i++ )
			for( int j = AnalyseResult2.cbCardCount[i]*(i+1)-1; j >= 0 ; j-- )
				bTempCard[bTempCardCount++] = AnalyseResult2.cbCardData[i][j];

		nCount4 = AnalyseResult.cbCardCount[3];
		BYTE bTempIndex = 0;
		while( bTempCardCount-- && nCount4 )
		{
			if( GetCardLogicValue( bTempCard[bTempIndex] ) != 15 )		//排除2
			{
				CopyMemory( &AnalysedCard.bCardData[CT_TIE_ZHI-1][5*AnalysedCard.bCardCount[CT_TIE_ZHI-1]++],
					&AnalyseResult.cbCardData[3][(nCount4-1)*4],sizeof(BYTE)*4 );
				AnalysedCard.bCardData[CT_TIE_ZHI-1][5*AnalysedCard.bCardCount[CT_TIE_ZHI-1]-1] = bTempCard[bTempIndex];
				VERIFY( RemoveCard( &bTempCard[bTempIndex++],1,bCardData,bCardCount ) );
				bCardCount--;
				nCount4--;
			}
			else bTempIndex++;
		}

	}
	else if( AnalyseResult.cbCardCount[3] > 0 )
	{
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = 0; i < 3; i++ )
			for( int j = AnalyseResult1.cbCardCount[i]*(i+1)-1; j >= 0; j-- )
				bTempCard[bTempCardCount++] = AnalyseResult1.cbCardData[i][j];

		nCount4 = AnalyseResult.cbCardCount[3];
		BYTE bTempIndex = 0;
		while( bTempCardCount-- && nCount4 )
		{
			if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )		//排除2
			{
				CopyMemory( &AnalysedCard.bCardData[CT_TIE_ZHI-1][5*AnalysedCard.bCardCount[CT_TIE_ZHI-1]++],
					&AnalyseResult.cbCardData[3][(nCount4-1)*4],sizeof(BYTE)*4 );
				AnalysedCard.bCardData[CT_TIE_ZHI-1][5*AnalysedCard.bCardCount[CT_TIE_ZHI-1]-1] = bTempCard[bTempIndex];
				VERIFY( RemoveCard( &bTempCard[bTempIndex++],1,bCardData,bCardCount ) );
				bCardCount--;
				nCount4--;
			}
			else bTempIndex++;
		}
	}
	//重新收回剩余四条
	if( nCount4 > 0 )
	{
		while( nCount4-- )
		{
			CopyMemory( &bCardData[bCardCount],&AnalyseResult.cbCardData[3][nCount4*4],sizeof(BYTE)*4 );
			bCardCount += 4;
		}
		//重新排序
		SortCardList( bCardData,bCardCount );
	}


	//构造葫芦
	int nCount3 = 0;	//三条数据
	tagAnalyseResult AnalyseResult3;
	AnalysebCardData( bCardData,bCardCount,AnalyseResult3 );
	BYTE bHuLuCard[MAX_COUNT];
	BYTE bHuLuCardCount = 0;
	bool bBreakCard = true;			
	if( AnalyseResult.cbCardCount[2]>0 && GetCardLogicValue(AnalyseResult.cbCardData[2][0])>=11 &&
		AnalyseResult3.cbCardCount[1]<AnalyseResult.cbCardCount[2] )
	{
		if( AnalyseResult3.cbCardCount[1] > 0 )
		{
			BYTE bIndex = 0;
			while( bIndex<AnalyseResult3.cbCardCount[1] && GetCardLogicValue(AnalyseResult3.cbCardData[1][bIndex*2]) == 15 ) bIndex++;
			if( bIndex<AnalyseResult3.cbCardCount[1] && GetCardLogicValue(AnalyseResult3.cbCardData[1][bIndex*2]) != 15 )
			{
				CopyMemory( &bHuLuCard[5*bHuLuCardCount++],
					AnalyseResult.cbCardData[2],sizeof(BYTE)*3 );
				CopyMemory( &bHuLuCard[5*bHuLuCardCount-2],&AnalyseResult3.cbCardData[1][bIndex*2],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( &AnalyseResult3.cbCardData[1][bIndex*2],2,bCardData,bCardCount ) );
				VERIFY( RemoveCard( AnalyseResult.cbCardData[2],3,AnalyseResult.cbCardData[2],AnalyseResult.cbCardCount[2]*3 ) );
				AnalyseResult.cbCardCount[2]--;
				bCardCount -= 2;
				bBreakCard = false;
			}
		}
		if( bBreakCard == true )
		{
			BYTE bCount = AnalyseResult.cbCardCount[2];
			if( bCount > 1 )
			{
				BYTE bFirstIndex,bNextIndex;
				if( GetCardLogicValue(AnalyseResult.cbCardData[2][(bCount-1)*3])>=11 ) 
				{
					bFirstIndex = bCount-1;
					bNextIndex = bCount-2;
				}else
				{
					bNextIndex = bCount-1;
					bFirstIndex = bCount-2;
					while( GetCardLogicValue(AnalyseResult.cbCardData[2][3*bFirstIndex])<11 ) bFirstIndex--;
					ASSERT( bFirstIndex>=0 && bFirstIndex<(BYTE)-1 );
				}
				CopyMemory( &bHuLuCard[5*bHuLuCardCount++],
					&AnalyseResult.cbCardData[2][bFirstIndex*3],sizeof(BYTE)*3 );
				CopyMemory( &bHuLuCard[5*bHuLuCardCount-2],
					&AnalyseResult.cbCardData[2][bNextIndex*3+1],sizeof(BYTE)*2 );
				bCardData[bCardCount++] = AnalyseResult.cbCardData[2][bNextIndex*3];
				VERIFY( RemoveCard( &AnalyseResult.cbCardData[2][bFirstIndex*3],3,AnalyseResult.cbCardData[2],
					AnalyseResult.cbCardCount[2]*3 ) );
				AnalyseResult.cbCardCount[2]--;
				VERIFY( RemoveCard( &AnalyseResult.cbCardData[2][bNextIndex*3],3,AnalyseResult.cbCardData[2],
					AnalyseResult.cbCardCount[2]*3 ) );
				AnalyseResult.cbCardCount[2]--;
				bBreakCard = false;
			}
			//２双支
			else if( AnalyseResult3.cbCardCount[1] > 0 )
			{
				CopyMemory( &bHuLuCard[5*bHuLuCardCount++],
					AnalyseResult.cbCardData[2],sizeof(BYTE)*3 );
				CopyMemory( &bHuLuCard[5*bHuLuCardCount-2],AnalyseResult3.cbCardData[1],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( AnalyseResult3.cbCardData[1],2,bCardData,bCardCount ) );
				bCardCount -= 2;
				VERIFY( RemoveCard( AnalyseResult.cbCardData[2],3,AnalyseResult.cbCardData[2],AnalyseResult.cbCardCount[2]*3 ) );
				AnalyseResult.cbCardCount[2]--;
				bBreakCard = false;
			}
		}

		//重新分析分布
		if( bBreakCard == false )
		{
			SortCardList(bCardData,bCardCount);
			AnalysebCardData( bCardData,bCardCount,AnalyseResult3 );
		}	
	}

	if( AnalyseResult.cbCardCount[2] > 0 && 
		(AnalyseResult3.cbCardCount[1]) < AnalyseResult.cbCardCount[2] /*&& bBreakCard*/ )
	{
		//释放顺子
		CopyMemory( &bCardData[bCardCount],AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*AnalysedCard.bCardCount[CT_SHUN_ZI-1]*5 );
		ZeroMemory( AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*MAX_COUNT );
		bCardCount += AnalysedCard.bCardCount[CT_SHUN_ZI-1]*5;
		AnalysedCard.bCardCount[CT_SHUN_ZI-1] = 0;

		SortCardList( bCardData,bCardCount );
		tagAnalyseResult AnalyseResult4;
		AnalysebCardData( bCardData,bCardCount,AnalyseResult4 );
		
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = AnalyseResult4.cbCardCount[1]*2-2; i >= 0 ; i-=2 )
		{
			bTempCard[bTempCardCount++] = AnalyseResult4.cbCardData[1][i];
			bTempCard[bTempCardCount++] = AnalyseResult4.cbCardData[1][i+1];
		}

		nCount3 = AnalyseResult.cbCardCount[2];
		BYTE bTempIndex = 0;
		while( (bTempIndex < bTempCardCount) && nCount3 )
		{
			if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )
			{
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
					&AnalyseResult.cbCardData[2][(nCount3-1)*3],sizeof(BYTE)*3 );
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]-2],&bTempCard[bTempIndex],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( &bTempCard[bTempIndex],2,bCardData,bCardCount ) );
				bTempIndex += 2;
				bCardCount -= 2;
				nCount3--;
			}
			else bTempIndex += 2;
		}

		if( nCount3 > 0 )		//释放同花
		{
			CopyMemory( &bCardData[bCardCount],AnalysedCard.bCardData[CT_TONG_HUA-1],sizeof(BYTE)*AnalysedCard.bCardCount[CT_TONG_HUA-1]*5 );
			ZeroMemory( AnalysedCard.bCardData[CT_TONG_HUA-1],sizeof(BYTE)*MAX_COUNT );
			bCardCount += AnalysedCard.bCardCount[CT_TONG_HUA-1]*5;
			AnalysedCard.bCardCount[CT_TONG_HUA-1] = 0;

			SortCardList( bCardData,bCardCount );
			AnalysebCardData( bCardData,bCardCount,AnalyseResult4 );
			
			bTempCardCount = 0;
			for( int i = AnalyseResult4.cbCardCount[1]*2-2; i >= 0 ; i-=2 )
			{
				bTempCard[bTempCardCount++] = AnalyseResult4.cbCardData[1][i];
				bTempCard[bTempCardCount++] = AnalyseResult4.cbCardData[1][i+1];
			}

			bTempIndex = 0;
			while( (bTempIndex < bTempCardCount) && nCount3 )
			{
				if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )
				{
					CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
						&AnalyseResult.cbCardData[2][(nCount3-1)*3],sizeof(BYTE)*3 );
					CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]-2],&bTempCard[bTempIndex],sizeof(BYTE)*2 );
					VERIFY( RemoveCard( &bTempCard[bTempIndex],2,bCardData,bCardCount ) );
					bTempIndex += 2;
					bCardCount -= 2;
					nCount3--;
				}
				else bTempIndex += 2;
			}
		}

	}
	else if( AnalyseResult.cbCardCount[2] > 0 )
	{
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = AnalyseResult3.cbCardCount[1]*2-2; i >= 0 ; i-=2 )
		{
			bTempCard[bTempCardCount++] = AnalyseResult3.cbCardData[1][i];
			bTempCard[bTempCardCount++] = AnalyseResult3.cbCardData[1][i+1];
		}

		nCount3 = AnalyseResult.cbCardCount[2];
		BYTE bTempIndex = 0;
		while( (bTempIndex < bTempCardCount) && nCount3 )
		{
			if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )
			{
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
					&AnalyseResult.cbCardData[2][(nCount3-1)*3],sizeof(BYTE)*3 );
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]-2],&bTempCard[bTempIndex],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( &bTempCard[bTempIndex],2,bCardData,bCardCount ) );
				bTempIndex += 2;
				bCardCount -= 2;
				nCount3--;
			}
			else bTempIndex += 2;
		}
	}
	//重新收回剩余三条
	if( nCount3 > 0 )
	{
		while( nCount3-- )
		{
			CopyMemory( &bCardData[bCardCount],&AnalyseResult.cbCardData[2][nCount3*3],sizeof(BYTE)*3 );
			bCardCount += 3;
		}
		//重新排序
		SortCardList( bCardData,bCardCount );
	}
	//
	if( bHuLuCardCount > 0 )
	{
		CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
			bHuLuCard,sizeof(BYTE)*5 );
	}


	AnalysebDistributing( bCardData,bCardCount,Distributing );
	//构造同花
	while( SearchTongHua( Distributing,0x00,bTempData ) == 5 )
	{
		//构造
		bTempCount = AnalysedCard.bCardCount[CT_TONG_HUA-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_TONG_HUA-1][bTempCount*5],bTempData,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData,5,bCardData,bCardCount ) );
		bCardCount -= 5;
		//重新分析分布
		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}
	//构造顺子
	while( SearchShunZi( Distributing,0x00,bTempData ) == 5 )
	{
		//构造
		bTempCount = AnalysedCard.bCardCount[CT_SHUN_ZI-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_SHUN_ZI-1][bTempCount*5],bTempData,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData,5,bCardData,bCardCount ) );
		bCardCount -= 5;
		//重新分析分布
		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}
	
	AnalysebCardData( bCardData,bCardCount,AnalyseResult );
	//葫芦
	nCount3=0;
	if( AnalyseResult.cbCardCount[2] > 0 && AnalyseResult.cbCardCount[1]>0 )
	{
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = AnalyseResult.cbCardCount[1]*2-2; i >= 0 ; i-=2 )
		{
			bTempCard[bTempCardCount++] = AnalyseResult.cbCardData[1][i];
			bTempCard[bTempCardCount++] = AnalyseResult.cbCardData[1][i+1];
		}

		nCount3 = AnalyseResult.cbCardCount[2];
		BYTE bTempIndex = 0;
		while( (bTempIndex < bTempCardCount) && nCount3 )
		{
			if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )
			{
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
					&AnalyseResult.cbCardData[2][(nCount3-1)*3],sizeof(BYTE)*3 );
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]-2],&bTempCard[bTempIndex],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( &bTempCard[bTempIndex],2,bCardData,bCardCount ) );
				bCardCount -= 2;
				VERIFY( RemoveCard( &AnalyseResult.cbCardData[2][(nCount3-1)*3],3,bCardData,bCardCount ) );
				bCardCount -= 3;
				bTempIndex += 2;
				nCount3--;
			}
			else bTempIndex += 2;
		}
	}
	SortCardList(bCardData,bCardCount);
	AnalysebCardData( bCardData,bCardCount,AnalyseResult );
	//构造简单类型
	//CT_FOUR_TIAO
	for( int i = AnalyseResult.cbCardCount[3]-1; i >= 0; i-- )
	{
		bTempCount = AnalysedCard.bCardCount[CT_FOUR_TIAO-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_FOUR_TIAO-1][bTempCount*4],&AnalyseResult.cbCardData[3][i*4],4*sizeof(BYTE) );
	}
	//CT_THREE_TIAO
	for( int i = AnalyseResult.cbCardCount[2]-1; i >= 0; i-- )
	{
		bTempCount = AnalysedCard.bCardCount[CT_THREE_TIAO-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_THREE_TIAO-1][bTempCount*3],&AnalyseResult.cbCardData[2][i*3],3*sizeof(BYTE) );
	}
	//CT_DOUBLE
	for( int i = AnalyseResult.cbCardCount[1]-1; i >= 0; i-- )
	{
		bTempCount = AnalysedCard.bCardCount[CT_DOUBLE-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_DOUBLE-1][bTempCount*2],&AnalyseResult.cbCardData[1][i*2],2*sizeof(BYTE) );
	}
	//CT_SINGLE
	for( int i = AnalyseResult.cbCardCount[0]-1; i >= 0; i-- )
	{
		bTempCount = AnalysedCard.bCardCount[CT_SINGLE-1]++;
		AnalysedCard.bCardData[CT_SINGLE-1][bTempCount] = AnalyseResult.cbCardData[0][i];
	}

	//如果两条以下简单类型>=3,则多条2当做简单类型
	int nSimpleCount = 0;
	for( int i = CT_DOUBLE-1; i >= 0; i-- )
		nSimpleCount += AnalysedCard.bCardCount[i];
	if( nSimpleCount >= 3 )
	{
		for( int i = CT_HU_LU-1; i < CT_TIE_ZHI; i++ )
		{
			if( AnalysedCard.bCardCount[i]==0 ) continue;
			if( GetCardLogicValue(AnalysedCard.bCardData[i][(AnalysedCard.bCardCount[i]-1)*5]) == 15 )
			{
				//释放
				CopyMemory( bCardData,&AnalysedCard.bCardData[i][(AnalysedCard.bCardCount[i]-1)*5],sizeof(BYTE)*5 );
				AnalysedCard.bCardCount[i]--;

				BYTE bTempType = 7 - i;
				CopyMemory( &AnalysedCard.bCardData[bTempType][AnalysedCard.bCardCount[bTempType]*(bTempType+1)],
					&bCardData[4-bTempType],(bTempType+1)*sizeof(BYTE) );
				ASSERT( GetCardType(&bCardData[4-bTempType],(bTempType+1))>CT_ERROR );
				AnalysedCard.bCardCount[bTempType]++;

				BYTE bCount = i==CT_HU_LU-1?1:2;
				//单
				CopyMemory( &AnalysedCard.bCardData[0][AnalysedCard.bCardCount[0]],
					bCardData,(bCount)*sizeof(BYTE) );
				AnalysedCard.bCardCount[0] += bCount;
				//双
				CopyMemory( &AnalysedCard.bCardData[1][AnalysedCard.bCardCount[1]*2],
					&bCardData[bCount],2*sizeof(BYTE) );
				ASSERT( GetCardType(&bCardData[bCount],2)>CT_ERROR );
				AnalysedCard.bCardCount[1]++;
			}

		}
		for( int i = CT_THREE_TIAO-1; i < CT_FOUR_TIAO; i++ )
		{
			if( AnalysedCard.bCardCount[i] == 0 ) continue;
			if( GetCardLogicValue(AnalysedCard.bCardData[i][(AnalysedCard.bCardCount[i]-1)*(i+1)]) == 15 )
			{
				//释放
				CopyMemory( bCardData,&AnalysedCard.bCardData[i][(AnalysedCard.bCardCount[i]-1)*(i+1)],sizeof(BYTE)*(i+1) );
				AnalysedCard.bCardCount[i]--;
				//双
				BYTE bTempType = 1;
				CopyMemory( &AnalysedCard.bCardData[bTempType][AnalysedCard.bCardCount[bTempType]*(bTempType+1)],
					&bCardData[i-1],(bTempType+1)*sizeof(BYTE) );
				ASSERT( GetCardType(&bCardData[i-1],(bTempType+1))>CT_ERROR );
				AnalysedCard.bCardCount[bTempType]++;
				//单
				BYTE bCount = i==CT_THREE_TIAO-1?1:2;
				CopyMemory( &AnalysedCard.bCardData[0][AnalysedCard.bCardCount[0]],
					bCardData,(bCount)*sizeof(BYTE) );
				AnalysedCard.bCardCount[0] += bCount;
			}
		}
	}
	if( AnalysedCard.bCardCount[0] >= 5 )
	{
		if( AnalysedCard.bCardCount[1]>0 && GetCardLogicValue(AnalysedCard.bCardData[1][(AnalysedCard.bCardCount[1]-1)*2]) == 15 )
		{
			//释放
			CopyMemory( bCardData,&AnalysedCard.bCardData[1][(AnalysedCard.bCardCount[1]-1)*2],sizeof(BYTE)*2 );
			AnalysedCard.bCardCount[1]--;
			ASSERT( GetCardType(bCardData,2)>CT_ERROR );
			//单
			BYTE bCount = 2;
			CopyMemory( &AnalysedCard.bCardData[0][AnalysedCard.bCardCount[0]],
				bCardData,(bCount)*sizeof(BYTE) );
			AnalysedCard.bCardCount[0] += bCount;
		}
	}

	return;

}

//搜索出牌
bool CGameLogic::SearchFromAnalysedCard( const tagAnalysedCard& AnalysedCard,WORD wChairId,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE bOutCardRecord[],BYTE bOutCardCount,
										const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardResult &OutCardResult,bool &bSplit )
{
	//
	ZeroMemory( &OutCardResult,sizeof(tagOutCardResult) );

	/**
	*出牌策略：
	*1.有顺出顺
	*2.同花,同花顺
	*3.三条，及Q以下葫芦;四条及Q以下铁支
	*3.简单类型,如果只剩2张，则先出桌面最大牌，如果简单类型牌数>3
	*4.Q以上葫芦，Q以上铁支
	*5.简单类型
	*
	*
	*压牌策略：
	*1.能压则压,但不拆牌
	**/

	BYTE bMostCount=0,bLessCount=0;
	bool bTurn = bTurnCardCount==0?true:false;
	bool bFinish = IsOutCardFinish(wChairId,AnalysedCard,bOutCardRecord,bOutCardCount,bTurn,cbCardData,cbCardCount,bMostCount,bLessCount);

	WORD wPreChairId = wChairId==0?GAME_PLAYER-1:wChairId-1;
	WORD wLastPreChairId = wPreChairId==0?GAME_PLAYER-1:wPreChairId-1;
	WORD wNextChairId = (wChairId+1)%GAME_PLAYER;

	if( bTurnCardCount == 0 )					//出牌
	{
		BYTE bMostCard = GetMostCardValue( AnalysedCard,bOutCardRecord,bOutCardCount );

		//可以出完的情况
		if( bFinish )
		{
			if( bLessCount<=1 )	//先出大
			{
				for( int i = CountArray(AnalysedCard.bCardCount)-1; i>=0 ; i-- )
				{
					BYTE bCount = i>=CT_SHUN_ZI-1?5:i+1;
					for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
					{
						if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,
							cbCardData,cbCardCount) )
						{
							OutCardResult.cbCardCount = bCount;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[i][j*bCount],
								sizeof(BYTE)*OutCardResult.cbCardCount );
							SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
							ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
							return true;
						}
					}
				}
			}
			else				//先出小牌最大
			{
				int bIndex = -1,bType=0;
				BYTE bCount;
				//小牌中最大且能收回牌权
				for( int i = 0; i < CountArray(AnalysedCard.bCardCount); i++ )
				{
					bCount = i>=CT_SHUN_ZI-1?5:i+1;
					if( AnalysedCard.bCardCount[i] == 0 ) continue;
					if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*(AnalysedCard.bCardCount[i]-1)],bCount,
						cbCardData,cbCardCount) )
						continue;
					if( bIndex == -1 )
						for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
						{
							if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*j],bCount,
								cbCardData,cbCardCount) )
							{
								bType = i+1;
								bIndex = j;
								break;
							}
						}
					for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
					{
						if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*j],bCount,
								cbCardData,cbCardCount) && 
							CompareCard(AnalysedCard.bCardData[i][bIndex*bCount],AnalysedCard.bCardData[i][j*bCount]) )
						{
							bType = i+1;
							bIndex = j;
						}
					}
					if( bIndex != -1 )
						break;
				}
				//大于其它玩家小于最大牌的牌型
				if( bIndex == -1 )
				{
					for( int i = 1; i < CountArray(AnalysedCard.bCardCount); i++ )
					{
						BYTE bCount = i>=CT_SHUN_ZI-1?5:i+1;
						if( AnalysedCard.bCardCount[i]>0 && 
							(CompareCard(GetCardLogicValue(AnalysedCard.bCardData[i][0]),bMostCard) &&
							GetCardLogicValue(AnalysedCard.bCardData[i][0])<13 ||
							i>=CT_SHUN_ZI-1) &&
							IsTurnCardMost(wChairId,AnalysedCard.bCardData[i],bCount,cbCardData,cbCardCount)
							)
						{
							bType = i + 1;
							bIndex = 0;
							break;
						}
					}
				}
				//小牌中最大者
				if( bIndex == -1 )
				{
					for( int i = 0; i < CountArray(AnalysedCard.bCardCount); i++ )
					{
						bCount = i>=CT_SHUN_ZI-1?5:i+1;
						if( bIndex == -1 )
							for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
							{
								if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*j],bCount,
									cbCardData,cbCardCount) )
								{
									bType = i+1;
									bIndex = j;
									break;
								}
							}
						for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
						{
							if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*j],bCount,
									cbCardData,cbCardCount) && 
								CompareCard(AnalysedCard.bCardData[i][bIndex*bCount],AnalysedCard.bCardData[i][j*bCount]) )
							{
								bType = i+1;
								bIndex = j;
							}
						}
						if( bIndex != -1 )
							break;
					}
				}
				if( bIndex != -1 )
				{
					bCount = bType>=CT_SHUN_ZI?5:bType;
					OutCardResult.cbCardCount = bCount;
					CopyMemory(OutCardResult.cbResultCard,&AnalysedCard.bCardData[bType-1][bIndex*bCount],sizeof(BYTE)*bCount);
					SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
					ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
					return true;
				}
			}
		}

		//顺子
		if( AnalysedCard.bCardCount[CT_SHUN_ZI-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//同花
		if( AnalysedCard.bCardCount[CT_TONG_HUA-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TONG_HUA-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//Q以下三条
		if( AnalysedCard.bCardCount[CT_THREE_TIAO-1] > 0 && GetCardLogicValue(AnalysedCard.bCardData[CT_THREE_TIAO-1][0]) < 12 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_THREE_TIAO-1],sizeof(BYTE)*3 );
			OutCardResult.cbCardCount = 3;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q以下葫芦
		if( AnalysedCard.bCardCount[CT_HU_LU-1] > 0 && GetCardLogicValue(AnalysedCard.bCardData[CT_HU_LU-1][0]) < 12 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_HU_LU-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q以下四条
		if( AnalysedCard.bCardCount[CT_FOUR_TIAO-1] > 0 && GetCardLogicValue(AnalysedCard.bCardData[CT_FOUR_TIAO-1][0]) < 12 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_FOUR_TIAO-1],sizeof(BYTE)*4 );
			OutCardResult.cbCardCount = 4;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		if( cbCardCount[wChairId]<=10 && (cbCardCount[wPreChairId]<8 ||
			cbCardCount[wLastPreChairId]<8 || cbCardCount[wNextChairId]<8)
			)
		{
			//Q以下铁支
			if( AnalysedCard.bCardCount[CT_TIE_ZHI-1] > 0 && GetCardLogicValue(AnalysedCard.bCardData[CT_TIE_ZHI-1][0]) < 12 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_TIE_ZHI-1],sizeof(BYTE)*5 );
				OutCardResult.cbCardCount = 5;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}
			//同花顺
			if( AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1] > 0 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1],sizeof(BYTE)*5 );
				OutCardResult.cbCardCount = 5;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}
		}

		//如果简单类型（两条和单支)>3，则出简单类型
		int nSimpleCount = 0;
		for( int i = 0; i < CT_DOUBLE; i++ )
			nSimpleCount += AnalysedCard.bCardCount[i];
		if( nSimpleCount >= 3 )
		{
			if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 2 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
				OutCardResult.cbCardCount = 2;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}else if( AnalysedCard.bCardCount[0] > AnalysedCard.bCardCount[CT_DOUBLE-1]*2 &&
				( GetCardLogicValue(AnalysedCard.bCardData[0][0]) < GetCardLogicValue(bMostCard) ||
				CompareCard(AnalysedCard.bCardData[0][0],bMostCard) )  )
			{
				OutCardResult.cbResultCard[0] = AnalysedCard.bCardData[0][0];
				OutCardResult.cbCardCount = 1;
				return true;
			}

			if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 0 && 
				GetCardLogicValue(AnalysedCard.bCardData[CT_DOUBLE-1][0])<14 &&
				!IsTurnCardMost(wChairId,AnalysedCard.bCardData[CT_DOUBLE-1],2,
				cbCardData,cbCardCount)
				)
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
				OutCardResult.cbCardCount = 2;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}

			
			if( AnalysedCard.bCardCount[0] > 0 )
			{
				OutCardResult.cbResultCard[0] = AnalysedCard.bCardData[0][0];
				OutCardResult.cbCardCount = 1;
				return true;
			}

			if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 0 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
				OutCardResult.cbCardCount = 2;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}

		}

		//Q及以上三条
		if( AnalysedCard.bCardCount[CT_THREE_TIAO-1] > 0 && 
			( GetCardLogicValue(AnalysedCard.bCardData[CT_THREE_TIAO-1][0])<GetCardLogicValue(bMostCard) ||
			CompareCard(AnalysedCard.bCardData[CT_THREE_TIAO-1][0],bMostCard) ) &&
			GetCardLogicValue(AnalysedCard.bCardData[CT_THREE_TIAO-1][0])<14 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_THREE_TIAO-1],sizeof(BYTE)*3 );
			OutCardResult.cbCardCount = 3;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q及以上葫芦
		if( AnalysedCard.bCardCount[CT_HU_LU-1] > 0 )
		{
			if( GetCardLogicValue(AnalysedCard.bCardData[CT_HU_LU-1][0])<GetCardLogicValue(bMostCard) ||
				CompareCard(AnalysedCard.bCardData[CT_HU_LU-1][0],bMostCard) &&
				GetCardLogicValue(AnalysedCard.bCardData[CT_HU_LU-1][0])<14 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_HU_LU-1],sizeof(BYTE)*5 );
				OutCardResult.cbCardCount = 5;
			}else
			{
				CopyMemory( OutCardResult.cbResultCard,
					&AnalysedCard.bCardData[CT_HU_LU-1][3],sizeof(BYTE)*2 );
				OutCardResult.cbCardCount = 2;
				bSplit = true;
			}
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q及以上四条
		if( AnalysedCard.bCardCount[CT_FOUR_TIAO-1] > 0 &&
			( GetCardLogicValue(AnalysedCard.bCardData[CT_FOUR_TIAO-1][0])<GetCardLogicValue(bMostCard)||
			  CompareCard(AnalysedCard.bCardData[CT_FOUR_TIAO-1][0],bMostCard) ) &&
			  GetCardLogicValue(AnalysedCard.bCardData[CT_FOUR_TIAO-1][0])<14 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_FOUR_TIAO-1],sizeof(BYTE)*4 );
			OutCardResult.cbCardCount = 4;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q及以上铁支
		if( AnalysedCard.bCardCount[CT_TIE_ZHI-1] > 0 )
		{
			if( GetCardLogicValue(AnalysedCard.bCardData[CT_TIE_ZHI-1][0])<GetCardLogicValue(bMostCard) ||
				CompareCard(AnalysedCard.bCardData[CT_TIE_ZHI-1][0],bMostCard) &&
				GetCardLogicValue(AnalysedCard.bCardData[CT_TIE_ZHI-1][0])<14 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_TIE_ZHI-1],sizeof(BYTE)*5 );
				OutCardResult.cbCardCount = 5;
			}else
			{
				OutCardResult.cbResultCard[0] = AnalysedCard.bCardData[CT_TIE_ZHI-1][4];
				OutCardResult.cbCardCount = 1;
				bSplit = true;
			}
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//同花顺
		if( AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//双支
		if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
			OutCardResult.cbCardCount = 2;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//Q及以上三条
		if( AnalysedCard.bCardCount[CT_THREE_TIAO-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_THREE_TIAO-1],sizeof(BYTE)*3 );
			OutCardResult.cbCardCount = 3;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q及以上葫芦
		if( AnalysedCard.bCardCount[CT_HU_LU-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_HU_LU-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q及以上四条
		if( AnalysedCard.bCardCount[CT_FOUR_TIAO-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_FOUR_TIAO-1],sizeof(BYTE)*4 );
			OutCardResult.cbCardCount = 4;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q及以上铁支
		if( AnalysedCard.bCardCount[CT_TIE_ZHI-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TIE_ZHI-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//单支
		if( AnalysedCard.bCardCount[0] > 0 )
		{
			OutCardResult.cbResultCard[0] = AnalysedCard.bCardData[0][0];
			OutCardResult.cbCardCount = 1;
			return true;
		}

		ASSERT( FALSE );
	}
	else										//压牌
	{
		BYTE bTurnCardType = GetCardType( bTurnCardData,bTurnCardCount );
		ASSERT( bTurnCardType > CT_ERROR );
		if( bTurnCardType == CT_ERROR ) return false;
		//判断出完
		if( bFinish )
		{
			if( SearchOutCard(cbCardData[wChairId],cbCardCount[wChairId],bTurnCardData,bTurnCardCount,
				OutCardResult) )
			{
				ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
				if( IsTurnCardMost(wChairId,OutCardResult.cbResultCard,OutCardResult.cbCardCount,
					cbCardData,cbCardCount) )
				{
					tagAnalysedCard tmpAnalysedCard;
					BYTE bHandCard[MAX_COUNT],bHandCardCount;
					BYTE bTmpOutCardRecord[FULL_COUNT],bTmpOutCardRecordCount;
					BYTE bTmpMostCount=0,bTmpLessCount=0;

					bHandCardCount = cbCardCount[wChairId];
					CopyMemory(bHandCard,cbCardData[wChairId],sizeof(BYTE)*bHandCardCount);
					VERIFY( RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bHandCardCount) );
					bHandCardCount -= OutCardResult.cbCardCount;

					AnalyseCardData(bHandCard,bHandCardCount,tmpAnalysedCard);

					CopyMemory(bTmpOutCardRecord,bOutCardRecord,sizeof(BYTE)*bOutCardCount);
					bTmpOutCardRecordCount = bOutCardCount;
					CopyMemory(&bTmpOutCardRecord[bTmpOutCardRecordCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
					bTmpOutCardRecordCount += OutCardResult.cbCardCount;

					if( IsOutCardFinish(wChairId,tmpAnalysedCard,bTmpOutCardRecord,bTmpOutCardRecordCount,
						true,cbCardData,cbCardCount,bTmpMostCount,bTmpLessCount) )
					{
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
							return false;
						bSplit = true;
						return true;
					}
				}
			}
			if( SearchTurnCard(cbCardData[wChairId],cbCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult) )
			{
				ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
				tagAnalysedCard tmpAnalysedCard;
				BYTE bHandCard[MAX_COUNT],bHandCardCount;
				BYTE bTmpOutCardRecord[FULL_COUNT],bTmpOutCardRecordCount;
				BYTE bTmpMostCount=0,bTmpLessCount=0;

				bHandCardCount = cbCardCount[wChairId];
				CopyMemory(bHandCard,cbCardData[wChairId],sizeof(BYTE)*bHandCardCount);
				VERIFY( RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bHandCardCount) );
				bHandCardCount -= OutCardResult.cbCardCount;

				AnalyseCardData(bHandCard,bHandCardCount,tmpAnalysedCard);

				CopyMemory(bTmpOutCardRecord,bOutCardRecord,sizeof(BYTE)*bOutCardCount);
				bTmpOutCardRecordCount = bOutCardCount;
				CopyMemory(&bTmpOutCardRecord[bTmpOutCardRecordCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
				bTmpOutCardRecordCount += OutCardResult.cbCardCount;

				if( IsOutCardFinish(wChairId,tmpAnalysedCard,bTmpOutCardRecord,bTmpOutCardRecordCount,
					false,cbCardData,cbCardCount,bTmpMostCount,bTmpLessCount) )
				{
					ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
					if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
							return false;
					bSplit = true;
					return true;
				}
			}
		}
		switch( bTurnCardType )
		{
		case CT_SINGLE:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);
					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_DOUBLE:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					if( IsTurnCardMost(wChairId,OutCardType.bCardData,OutCardType.bCardCount,
						cbCardData,cbCardCount) ||
						GetCardLogicValue(OutCardType.bCardData[0])==15 )
					{
						tagOutCardType SingleType;
						AnalyseOutCardType(AnalysedCard,wChairId,CT_SINGLE,NULL,0,cbCardData,cbCardCount,SingleType);
						if( SingleType.bRemainCount > 2 ) return false;
					}
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);
					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_THREE_TIAO:
		case CT_FOUR_TIAO:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					if( GetCardLogicValue(OutCardType.bCardData[0])>=14 )
					{
						tagOutCardType SingleType,DoubleType;
						AnalyseOutCardType(AnalysedCard,wChairId,CT_SINGLE,NULL,0,cbCardData,cbCardCount,SingleType);
						AnalyseOutCardType(AnalysedCard,wChairId,CT_DOUBLE,NULL,0,cbCardData,cbCardCount,DoubleType);
						if( SingleType.bRemainCount+DoubleType.bRemainCount > 3 ) return false;
					}
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_SHUN_ZI:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_TONG_HUA:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_HU_LU:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					if( GetCardLogicValue(OutCardType.bCardData[0])>=14 )
					{
						tagOutCardType SingleType,DoubleType;
						AnalyseOutCardType(AnalysedCard,wChairId,CT_SINGLE,NULL,0,cbCardData,cbCardCount,SingleType);
						AnalyseOutCardType(AnalysedCard,wChairId,CT_DOUBLE,NULL,0,cbCardData,cbCardCount,DoubleType);
						if( SingleType.bRemainCount+DoubleType.bRemainCount > 3 ) return false;
					}

					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_TIE_ZHI:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					if( GetCardLogicValue(OutCardType.bCardData[0])>=14 )
					{
						tagOutCardType SingleType,DoubleType;
						AnalyseOutCardType(AnalysedCard,wChairId,CT_SINGLE,NULL,0,cbCardData,cbCardCount,SingleType);
						AnalyseOutCardType(AnalysedCard,wChairId,CT_DOUBLE,NULL,0,cbCardData,cbCardCount,DoubleType);
						if( SingleType.bRemainCount+DoubleType.bRemainCount > 3 ) return false;
					}

					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_TONG_HUA_SHUN:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		}
	}


	return false;
}


//首先出牌搜索
bool CGameLogic::SearchFirstOutCard( const tagAnalysedCard &AnalysedCard,tagOutCardResult &OutCardResult,bool &bSplit )
{
	//
	ZeroMemory( &OutCardResult,sizeof(tagOutCardResult) );

	/**
	*首先出牌策略
	*1.按方块3所在类型出牌,若为葫芦及以上且大于Q，则拆牌
	**/
	BYTE bOutCardData[MAX_COUNT];
	BYTE bOutCardCount = 0;
	for( int i = 0; i < CT_TONG_HUA_SHUN; i++ )
	{
		int nCount = (i >= CT_SHUN_ZI-1)?5:i+1;
		for( int j = 0; j < AnalysedCard.bCardCount[i]*nCount; j++ )
		{
			if( AnalysedCard.bCardData[i][j] == 0x03 )
			{
				BYTE bIndex = j / nCount;
				bOutCardCount = nCount;
				CopyMemory( bOutCardData,&AnalysedCard.bCardData[i][bIndex*nCount],nCount*sizeof(BYTE) );
				SortCardList( bOutCardData,bOutCardCount );
				break;
			}
		}
	}
	
	if( bOutCardCount > 0 )
	{
		BYTE bCardType = GetCardType( bOutCardData,bOutCardCount );
		if( bCardType == CT_HU_LU && GetCardLogicValue(bOutCardData[0]) >= 12 )
		{
			OutCardResult.cbCardCount = 2;
			CopyMemory( OutCardResult.cbResultCard,&bOutCardData[5-OutCardResult.cbCardCount],sizeof(BYTE)*OutCardResult.cbCardCount );
			bSplit = true;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		if( bCardType == CT_TIE_ZHI && GetCardLogicValue(bOutCardData[0]) != 3 )
		{
			OutCardResult.cbCardCount = 1;
			OutCardResult.cbResultCard[0] = 0x03;
			bSplit = true;
			return true;
		}
		OutCardResult.cbCardCount = bOutCardCount;
		CopyMemory( OutCardResult.cbResultCard,bOutCardData,bOutCardCount*sizeof(BYTE) );
		return true;
	}
	ASSERT(FALSE);
	return false;
}

//强制出最大牌搜索
bool CGameLogic::SearchForcedOutCard(const tagAnalysedCard& AnalysedCard,WORD wChairId,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE bOutCardRecord[],BYTE bOutCardCount,
		const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardResult& OutCardResult,bool &bReAnalyse)
{
	//
	ZeroMemory( &OutCardResult,sizeof(tagOutCardResult) );
	bReAnalyse = false;

	/**
	*强制出最大牌策略
	*1.若首先出牌:
	*	a.出单张除外的牌型
	*	b.出单张最大牌
	*2.若非首次出牌：
	*	a.若上家出牌数>1,则按压牌策略
	*	b.若上家出单张，则压最大单支
	**/

	BYTE bMostCount=0,bLessCount=0;
	bool bTurn = bTurnCardCount==0?true:false;
	bool bFinish = IsOutCardFinish(wChairId,AnalysedCard,bOutCardRecord,bOutCardCount,bTurn,cbCardData,cbCardCount,bMostCount,bLessCount);


	if( bTurnCardCount == 0 )
	{
		//判断出完
		if( bFinish && bLessCount<=1 )
		{
			for( int i = CountArray(AnalysedCard.bCardCount)-1; i > 0 ; i-- )
			{
				BYTE bCount = i>=CT_SHUN_ZI-1?5:i+1;
				for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
				{
					if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,
						cbCardData,cbCardCount) )
					{
						OutCardResult.cbCardCount = bCount;
						CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[i][j*bCount],
							sizeof(BYTE)*OutCardResult.cbCardCount );
						SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
						ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
						return true;
					}
				}
			}
		}
		//顺子
		if( AnalysedCard.bCardCount[CT_SHUN_ZI-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//同花
		if( AnalysedCard.bCardCount[CT_TONG_HUA-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TONG_HUA-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//葫芦
		if( AnalysedCard.bCardCount[CT_HU_LU-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_HU_LU-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//铁支
		if( AnalysedCard.bCardCount[CT_TIE_ZHI-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TIE_ZHI-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//四条
		if( AnalysedCard.bCardCount[CT_FOUR_TIAO-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_FOUR_TIAO-1],sizeof(BYTE)*4 );
			OutCardResult.cbCardCount = 4;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//同花顺
		if( AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//三条
		if( AnalysedCard.bCardCount[CT_THREE_TIAO-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_THREE_TIAO-1],sizeof(BYTE)*3 );
			OutCardResult.cbCardCount = 3;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//两条
		if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
			OutCardResult.cbCardCount = 2;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//出最大单张
		BYTE bHandCardData[MAX_COUNT],bHandCardCount;
		CopyMemory(bHandCardData,cbCardData[wChairId],sizeof(BYTE)*cbCardCount[wChairId]);
		bHandCardCount = cbCardCount[wChairId];
		SortCardList( bHandCardData,bHandCardCount );
		OutCardResult.cbResultCard[0] = bHandCardData[0];
		OutCardResult.cbCardCount = 1;
		bReAnalyse = true;
		return true;
	}
	else
	{
		if( bTurnCardCount == 1 )
		{
			//出最大单张
			BYTE bHandCardData[MAX_COUNT],bHandCardCount;
			CopyMemory(bHandCardData,cbCardData[wChairId],sizeof(BYTE)*cbCardCount[wChairId]);
			bHandCardCount = cbCardCount[wChairId];
			SortCardList( bHandCardData,bHandCardCount );
			if( CompareCard( bTurnCardData[0],bHandCardData[0] ) )
			{
				OutCardResult.cbResultCard[0] = bHandCardData[0];
				OutCardResult.cbCardCount = 1;
				bReAnalyse = true;
				return true;
			}
			return false;
		}
		else						//按压牌策略
		{
			BYTE bTurnCardType = GetCardType( bTurnCardData,bTurnCardCount );
			ASSERT( bTurnCardType > CT_SINGLE );
			if( bTurnCardType <= CT_SINGLE ) return false;

			if( bFinish )
			{
				if( SearchOutCard(cbCardData[wChairId],cbCardCount[wChairId],bTurnCardData,bTurnCardCount,
					OutCardResult) )
				{
					ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
					if( IsTurnCardMost(wChairId,OutCardResult.cbResultCard,OutCardResult.cbCardCount,
						cbCardData,cbCardCount) )
					{
						tagAnalysedCard tmpAnalysedCard;
						BYTE bHandCard[MAX_COUNT],bHandCardCount;
						BYTE bTmpOutCardRecord[FULL_COUNT],bTmpOutCardRecordCount;
						BYTE bTmpMostCount=0,bTmpLessCount=0;

						bHandCardCount = cbCardCount[wChairId];
						CopyMemory(bHandCard,cbCardData[wChairId],sizeof(BYTE)*bHandCardCount);
						VERIFY( RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bHandCardCount) );
						bHandCardCount -= OutCardResult.cbCardCount;

						AnalyseCardData(bHandCard,bHandCardCount,tmpAnalysedCard);

						CopyMemory(bTmpOutCardRecord,bOutCardRecord,sizeof(BYTE)*bOutCardCount);
						bTmpOutCardRecordCount = bOutCardCount;
						CopyMemory(&bTmpOutCardRecord[bTmpOutCardRecordCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
						bTmpOutCardRecordCount += OutCardResult.cbCardCount;

						if( IsOutCardFinish(wChairId,tmpAnalysedCard,bTmpOutCardRecord,bTmpOutCardRecordCount,
							true,cbCardData,cbCardCount,bTmpMostCount,bTmpLessCount) )
						{
							ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
							if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
								return false;
							bReAnalyse = true;
							return true;
						}
					}
				}
				if( SearchTurnCard(cbCardData[wChairId],cbCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult) )
				{
					ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
					tagAnalysedCard tmpAnalysedCard;
					BYTE bHandCard[MAX_COUNT],bHandCardCount;
					BYTE bTmpOutCardRecord[FULL_COUNT],bTmpOutCardRecordCount;
					BYTE bTmpMostCount=0,bTmpLessCount=0;

					bHandCardCount = cbCardCount[wChairId];
					CopyMemory(bHandCard,cbCardData[wChairId],sizeof(BYTE)*bHandCardCount);
					VERIFY( RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bHandCardCount) );
					bHandCardCount -= OutCardResult.cbCardCount;

					AnalyseCardData(bHandCard,bHandCardCount,tmpAnalysedCard);

					CopyMemory(bTmpOutCardRecord,bOutCardRecord,sizeof(BYTE)*bOutCardCount);
					bTmpOutCardRecordCount = bOutCardCount;
					CopyMemory(&bTmpOutCardRecord[bTmpOutCardRecordCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
					bTmpOutCardRecordCount += OutCardResult.cbCardCount;

					if( IsOutCardFinish(wChairId,tmpAnalysedCard,bTmpOutCardRecord,bTmpOutCardRecordCount,
						true,cbCardData,cbCardCount,bTmpMostCount,bTmpLessCount) )
					{
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
							return false;
						bReAnalyse = true;
						return true;
					}
				}
			}
			switch( bTurnCardType )
			{
			case CT_DOUBLE:
			case CT_THREE_TIAO:
			case CT_FOUR_TIAO:
				{
					int nCount = AnalysedCard.bCardCount[bTurnCardType-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[bTurnCardType-1][i*bTurnCardType],
							bTurnCardType,bTurnCardType ) )
						{
							OutCardResult.cbCardCount = bTurnCardType;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[bTurnCardType-1][i*bTurnCardType],
								sizeof(BYTE)*bTurnCardType );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
					return false;
				}
			case CT_SHUN_ZI:
				{
					int nCount = AnalysedCard.bCardCount[CT_SHUN_ZI-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_SHUN_ZI-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_SHUN_ZI-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
				}
			case CT_TONG_HUA:
				{
					int nCount = AnalysedCard.bCardCount[CT_TONG_HUA-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_TONG_HUA-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_TONG_HUA-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
				}
			case CT_HU_LU:
				{
					int nCount = AnalysedCard.bCardCount[CT_HU_LU-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_HU_LU-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_HU_LU-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
				}
			case CT_TIE_ZHI:
				{
					int nCount = AnalysedCard.bCardCount[CT_TIE_ZHI-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_TIE_ZHI-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_TIE_ZHI-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
				}
			case CT_TONG_HUA_SHUN:
				{
					int nCount = AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
					return false;
				}
			}
		}
	}
	return false;
}


//抓人
bool CGameLogic::HuntUser( const tagAnalysedCard &AnalysedCard,BYTE bHandCardData[],BYTE bHandCardCount,BYTE bNextUserCardCount,tagOutCardResult &OutCardResult,bool &bSplit )
{
	if( bNextUserCardCount == 1 )				//强制出最大
	{
		SortCardList( bHandCardData,bHandCardCount );
		OutCardResult.cbCardCount = 1;
		OutCardResult.cbResultCard[0] = bHandCardData[0];
		bSplit = true;
		return true;
	}

	if( bNextUserCardCount < 5 && AnalysedCard.bCardCount[bNextUserCardCount-1] > 0 )
	{
		OutCardResult.cbCardCount = bNextUserCardCount;
		CopyMemory( OutCardResult.cbResultCard,AnalysedCard.bCardData[bNextUserCardCount-1],sizeof(BYTE)*bNextUserCardCount );
		SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
		return true;
	}
	else if( bNextUserCardCount == 5 )
	{
		BYTE bCardType = CT_SHUN_ZI;
		for( int i = bCardType; i <= CT_TONG_HUA_SHUN; i++ )
		{
			if( AnalysedCard.bCardCount[i-1] > 0 )
			{
				OutCardResult.cbCardCount = bNextUserCardCount;
				CopyMemory( OutCardResult.cbResultCard,AnalysedCard.bCardData[i-1],sizeof(BYTE)*bNextUserCardCount );
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}
		}
	}
	return false;
}

//自动玩牌
bool CGameLogic::AutomatismPlay( tagAnalysedCard &AnalysedCard,WORD wChairId,WORD wOutCardUser,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE (&bCardData)[GAME_PLAYER][13],
								const BYTE bCardCount[],const BYTE bOutCardRecord[],BYTE bOutCardCount,tagOutCardResult &OutCardResult,bool bOutCard3 )
{
	if( wChairId == INVALID_CHAIR ) return false;

	//初始化
	ZeroMemory( &OutCardResult,sizeof(tagOutCardResult) );

	WORD wPreChairId = wChairId == 0 ? 3:wChairId-1;			//上家
	WORD wLastPreChair = wPreChairId == 0 ? 3:wPreChairId-1;	//上上玩家
	WORD wNextChairId = (wChairId+1) % GAME_PLAYER;				//下家

	//
	BYTE bHandCard[MAX_COUNT];
	CopyMemory( bHandCard,bCardData[wChairId],sizeof(BYTE)*bCardCount[wChairId] );
	SortCardList(bHandCard,bCardCount[wChairId]);

	//是否可以出完牌
	BYTE bMostCount=0,bLessCount=0;
	bool bTurn = bTurnCardCount==0?true:false;
	bool bFinish = IsOutCardFinish(wChairId,AnalysedCard,bOutCardRecord,bOutCardCount,bTurn,bCardData,bCardCount,bMostCount,bLessCount);

	//判断抓人
	if( bCardCount[wChairId]<8 && bCardCount[wChairId]>2 && !bFinish &&
		(bCardCount[wPreChairId]>7||bCardCount[wLastPreChair]>7||bCardCount[wNextChairId]>7) &&
		(bCardCount[wPreChairId]<=2||bCardCount[wLastPreChair]<=2||bCardCount[wNextChairId]<=2)
		)
	{
		if( bCardCount[wOutCardUser]==2 && (wNextChairId==wOutCardUser ||
			wLastPreChair==wOutCardUser) && bCardCount[wNextChairId]<8 &&(bCardCount[wNextChairId]>1||bTurnCardCount>1) )
			return false;
		if( bCardCount[wOutCardUser]==1 && wNextChairId==wOutCardUser && bTurnCardCount > 1 )
			return false;
		if( bCardCount[wOutCardUser]==1 && wLastPreChair==wOutCardUser && 
			(bCardCount[wNextChairId]>1||bTurnCardCount>1) && bCardCount[wNextChairId]<8 )
			return false;
		if( bTurnCardCount == 0 )
		{
			BYTE bIndex = 0;
			if( bCardCount[wNextChairId]==1 )
				bIndex = 0;
			else if( bCardCount[wNextChairId]==2 || bCardCount[wLastPreChair]<=2&&bCardCount[wPreChairId]>7&&bCardCount[wNextChairId]<8 )
			{
				bIndex = bCardCount[wChairId]-1;
				WORD wId;
				if( bCardCount[wNextChairId]==2 ) wId = wNextChairId;
				else wId = wLastPreChair;
				BYTE bCardType = GetCardType(bCardData[wId],bCardCount[wId]);
				if( bCardType > CT_ERROR )
				{
					ASSERT( bCardType <= CT_DOUBLE );
					if( AnalysedCard.bCardCount[bCardType-1] > 0 )
					{
						if( CompareCard(AnalysedCard.bCardData[bCardType-1][0],bCardData[wId][0]) )
						{
							OutCardResult.cbCardCount = bCardType;
							CopyMemory(OutCardResult.cbResultCard,AnalysedCard.bCardData[bCardType-1],sizeof(BYTE)*bCardType);
							VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
							//重新分析分布
							AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
							return true;
						}
					}
				}
			}
			OutCardResult.cbCardCount = 1;
			OutCardResult.cbResultCard[0] = bHandCard[bIndex];
			VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
			//重新分析分布
			AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
			return true;
		}else
		{
			BYTE bTurnCardType=GetCardType(bTurnCardData,bTurnCardCount);
			ASSERT( bTurnCardType>CT_ERROR );
			if( (bCardCount[wNextChairId]==2||
				(bCardCount[wLastPreChair]==2&&bCardCount[wPreChairId]>7&&bCardCount[wNextChairId]<8&&
				 (bCardCount[wNextChairId]>1||bTurnCardType==CT_DOUBLE))) &&
				bTurnCardType<=CT_DOUBLE )
				return false;
			if( bCardCount[wLastPreChair]==1&&bCardCount[wNextChairId]>1&&bCardCount[wNextChairId]<8 &&
				bTurnCardType==CT_SINGLE )
				return false;
			//搜索最大
			BYTE bReferCard=0;
			switch(bTurnCardType)
			{
			case CT_SINGLE:					//单牌类型,找最大
				{
					//分析扑克
					tagAnalyseResult AnalyseResult;
					AnalysebCardData(bHandCard,bCardCount[wChairId],AnalyseResult);
					//获取数值
					BYTE cbTurnCard=bTurnCardData[0];
					//寻找扑克
					for (BYTE i=0;i<CountArray(AnalyseResult.cbCardCount);i++)
					{
						//大小对比
						if( AnalyseResult.cbCardCount[i] == 0 ) continue;
						if (CompareCard(cbTurnCard,AnalyseResult.cbCardData[i][0])==false) continue;
						else cbTurnCard = AnalyseResult.cbCardData[i][0];
					}
					if( cbTurnCard != bTurnCardData[0] )
					{
						OutCardResult.cbCardCount=1;
						OutCardResult.cbResultCard[0]=cbTurnCard;
						VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
						//重新分析分布
						AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
						return true;
					}
					return false;
				}
			case CT_DOUBLE:					//对牌类型
			case CT_THREE_TIAO:				//三条类型
			case CT_FOUR_TIAO:				//四条类型
				{										//找最大
					//分析扑克
					tagAnalyseResult AnalyseResult;
					AnalysebCardData(bHandCard,bCardCount[wChairId],AnalyseResult);
					//获取数值
					BYTE cbTurnCard=bTurnCardData[0];
					BYTE bIndex = 0;
					//寻找扑克
					for (BYTE i=bTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
					{
						if( AnalyseResult.cbCardCount[i] == 0 ) continue;
						if( i==CT_SHUN_ZI-1 || i==CT_TONG_HUA-1 || i==CT_TONG_HUA_SHUN-1 || (bTurnCardType==CT_FOUR_TIAO&&i==CT_HU_LU-1) )
							continue;
						if (CompareCard(bTurnCardData[0],AnalyseResult.cbCardData[i][0])==false) continue;
						else 
						{
							cbTurnCard = AnalyseResult.cbCardData[i][0];
							bIndex = i;
						}
					}
					if( cbTurnCard != bTurnCardData[0] )
					{
						//设置结果
						OutCardResult.cbCardCount=bTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,AnalyseResult.cbCardData[bIndex],sizeof(BYTE)*bTurnCardCount);
						SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
						VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
						//重新分析分布
						AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
						return true;
					}
					return false;
				}
			case CT_SHUN_ZI:
			case CT_TONG_HUA:
			case CT_HU_LU:
			case CT_TIE_ZHI:
			case CT_TONG_HUA_SHUN:
				{					//搜索最大
					BYTE bOutCard[5],bTempData[5];
					bool bFound = false;
					tagDistributing Distributing;
					AnalysebDistributing( bHandCard,bCardCount[wChairId],Distributing );
					bReferCard = bTurnCardType < CT_TONG_HUA_SHUN ? 0x00:bTurnCardData[0];
					while( SearchTongHuaShun( Distributing,bReferCard,bTempData ) != 0 )
					{
						bFound = true;
						bReferCard = bTempData[0];
						CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
					}
					if( !bFound && bTurnCardType < CT_TONG_HUA_SHUN )
					{
						bReferCard = bTurnCardType < CT_TIE_ZHI ? 0x00:bTurnCardData[1];
						while( SearchTieZhi(Distributing,bReferCard,bTempData) != 0 )
						{
							bFound = true;
							bReferCard = bTempData[0];
							CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
						}
					}
					if( !bFound && bTurnCardType < CT_TIE_ZHI )
					{
						bReferCard = bTurnCardType < CT_HU_LU ? 0x00:bTurnCardData[2];
						while( SearchHuLu(Distributing,bReferCard,bTempData) != 0 )
						{
							bFound = true;
							bReferCard = bTempData[0];
							CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
						}
					}
					if( !bFound && bTurnCardType < CT_HU_LU )
					{
						bReferCard = bTurnCardType < CT_TONG_HUA ? 0x00:bTurnCardData[0];
						while( SearchTongHua(Distributing,bReferCard,bTempData) != 0 )
						{
							bFound = true;
							bReferCard = bTempData[0];
							CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
						}
					}
					if( !bFound && bTurnCardType < CT_TONG_HUA )
					{
						bReferCard = bTurnCardData[0];
						while( SearchShunZi(Distributing,bReferCard,bTempData) != 0 )
						{
							bFound = true;
							bReferCard = bTempData[0];
							CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
						}
					}
					if( bFound )
					{
						OutCardResult.cbCardCount=5;
						CopyMemory(OutCardResult.cbResultCard,bTempData,sizeof(BYTE)*5);
						SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
						VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
						//重新分析分布
						AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
						return true;
					}
					return false;
				}
			}
			
			ASSERT(FALSE);
			return false;
		}
	}

	if ( bOutCard3 )								//首次出牌
	{
		bool bReAnalyse = false;
		if( SearchFirstOutCard( AnalysedCard,OutCardResult,bReAnalyse ) )
		{
			if( bReAnalyse == false )
			{
				BYTE bType = GetCardType( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				ASSERT( bType > CT_ERROR );
				int nCount = (bType >= 5)?5:bType;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,
					AnalysedCard.bCardData[bType-1],AnalysedCard.bCardCount[bType-1]*nCount ) );
				AnalysedCard.bCardCount[bType-1]--;
			}else
			{
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
			}
			return true;
		}

		ASSERT(FALSE);

		return false;
	}
	else if( bCardCount[wNextChairId]==1 )			//强制出最大
	{
		bool bReAnalyse = false;
		if( SearchForcedOutCard( AnalysedCard,wChairId,bTurnCardData,bTurnCardCount,bOutCardRecord,bOutCardCount,
			bCardData,bCardCount,OutCardResult,bReAnalyse ) )
		{
			ASSERT( bTurnCardCount==0 || CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
			if( bTurnCardCount>1 && CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
				return false;
			if( bReAnalyse == false )
			{
				BYTE bType = GetCardType( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				ASSERT( bType > CT_ERROR );
				int nCount = (bType >= 5)?5:bType;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,
					AnalysedCard.bCardData[bType-1],AnalysedCard.bCardCount[bType-1]*nCount ) );
				AnalysedCard.bCardCount[bType-1]--;
			}else
			{
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
			}
			return true;
		}
		else if( bTurnCardCount > 1 && bCardCount[wOutCardUser] <= 5 )
		{
			//拆分搜索
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		else return false;

	}
	else							//常规出牌
	{
		//只剩下2张时
		if( bCardCount[wChairId] == 2 )
		{
			if( bTurnCardCount == 0 )
			{
				if( GetCardLogicValue(bHandCard[0]) == GetCardLogicValue(bHandCard[1]) )
				{
					OutCardResult.cbCardCount = 2;
					CopyMemory( OutCardResult.cbResultCard,bHandCard,sizeof(BYTE)*OutCardResult.cbCardCount );
					return true;
				}
				BYTE bMostCard = GetMostCardValue(AnalysedCard,bOutCardRecord,bOutCardCount);
				BYTE bIndex = CompareCard(bMostCard,bHandCard[0])||bMostCard==bHandCard[0] ? 0:1;
				OutCardResult.cbCardCount = 1;
				OutCardResult.cbResultCard[0] = bHandCard[bIndex];
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else if( bTurnCardCount == 1 )
			{
				BYTE bMostCard = GetMostCardValue(AnalysedCard,bOutCardRecord,bOutCardCount);
				BYTE bIndex = CompareCard(bMostCard,bHandCard[0])||bMostCard==bHandCard[0] ? 0:1;
				if( CompareCard(bTurnCardData[0],bHandCard[bIndex]))
				{
					OutCardResult.cbCardCount = 1;
					OutCardResult.cbResultCard[0] = bHandCard[bIndex];
					VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
					//重新分析分布
					AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
					return true;
				}
				else return false;
			}
			else if( GetCardLogicValue(bHandCard[1])==GetCardLogicValue(bHandCard[0]) && CompareCard(bTurnCardData,bHandCard,bTurnCardCount,2) )
			{
				OutCardResult.cbCardCount = 2;
				OutCardResult.cbResultCard[0] = bHandCard[0];
				OutCardResult.cbResultCard[1] = bHandCard[1];
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		//出牌者为３，自己牌小于８，有其它玩家可加倍
		if( bCardCount[wOutCardUser]==3 && bCardCount[wChairId]<8 && wOutCardUser!=wChairId && bCardCount[wChairId]>1 &&
			(bCardCount[wPreChairId]>7||bCardCount[wLastPreChair]>7||bCardCount[wNextChairId]>7) &&
			!bFinish && GetCardLogicValue(bTurnCardData[0])>=11 && bTurnCardCount<=CT_DOUBLE)
			return false;
		bool bReAnalyse = false;
		if( SearchFromAnalysedCard( AnalysedCard,wChairId,bTurnCardData,bTurnCardCount,bOutCardRecord,bOutCardCount,
			bCardData,bCardCount,OutCardResult,bReAnalyse ) )
		{
			ASSERT( bTurnCardCount==0 || CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==true );
			if( bTurnCardCount>0 && CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
				return false;
			if( bReAnalyse == false )
			{
				BYTE bType = GetCardType( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				ASSERT( bType > CT_ERROR );
				int nCount = (bType >= 5)?5:bType;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,
					AnalysedCard.bCardData[bType-1],AnalysedCard.bCardCount[bType-1]*nCount ) );
				AnalysedCard.bCardCount[bType-1]--;
			}else
			{
				ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
			}

			return true;

		}
		//自己大于７，出牌者小于８
		else if( bCardCount[wChairId]>7 && bCardCount[wOutCardUser]<8 )
		{
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		//自己大于７，有两个小于８
		else if( bCardCount[wChairId]>7 )
		{
			BYTE bLessCount = 0;
			for( int i=0; i < 4; i++ )
			{
				if( i == wChairId ) continue;
				if( bCardCount[i] < 8 ) bLessCount++;
			}
			if( bLessCount > 1 )
			{
				if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
				{
					ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
					if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
						return false;
					VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
					//重新分析分布
					AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
					return true;
				}
				else return false;
			}
		}
		//自己小于８，且最大
		else if( bCardCount[wChairId]<8 && bCardCount[wChairId]>bCardCount[wNextChairId] &&
			bCardCount[wChairId]>bCardCount[wLastPreChair] && bCardCount[wChairId]>bCardCount[wPreChairId] )
		{
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		//自己小于8，且有两家小于自己
		else if( bCardCount[wChairId]<8 &&
			( bCardCount[wChairId]>bCardCount[wNextChairId]&&bCardCount[wChairId]>bCardCount[wLastPreChair] ||
			bCardCount[wChairId]>bCardCount[wNextChairId]&&bCardCount[wChairId]>bCardCount[wPreChairId] ||
			bCardCount[wChairId]>bCardCount[wPreChairId]&&bCardCount[wChairId]>bCardCount[wLastPreChair] )
			)
		{
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		//其它全部小于5
		else if( /*bCardCount[wChairId]<8 &&*/ bCardCount[wPreChairId]<5 && bCardCount[wLastPreChair]<5 &&
			bCardCount[wNextChairId]<5 )
		{
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//重新分析分布
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}

	}
	return false;
}


//获取最大牌
BYTE CGameLogic::GetMostCardValue( const tagAnalysedCard &AnalysedCard,const BYTE bOutCardRecord[],BYTE bOutCardCount )
{
	BYTE bCardRecord[FULL_COUNT],bCardCount,bMostCard;
	ZeroMemory( bCardRecord,FULL_COUNT*sizeof(BYTE) );

	int i = 1;
	bCardCount = 0;
	for( i = 0; i < CountArray(AnalysedCard.bCardCount); i++ )
		for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
			bCardRecord[bCardCount++] = AnalysedCard.bCardData[i][j];

	CopyMemory( &bCardRecord[bCardCount],bOutCardRecord,sizeof(BYTE)*bOutCardCount );
	bCardCount += bOutCardCount;
	ASSERT( bCardCount <= FULL_COUNT );
	if( bCardCount > FULL_COUNT ) 
		return false;
	SortCardList( bCardRecord,bCardCount,ST_ORDER );

	BYTE bColorIndex[4] = { 0,0,0,0 };
	BYTE bMostIndex = 0;
	BYTE bSameCount = 1;
	BYTE bMostValue = 15;	// 2
	for( i = 1; i < bCardCount; i++ )
	{
		if( GetCardLogicValue(bCardRecord[bMostIndex]) < bMostValue ) 
		{
			bSameCount = 0;
			break;
		}
		if( GetCardLogicValue(bCardRecord[bMostIndex]) == GetCardLogicValue(bCardRecord[i]) ) 
			bSameCount++;
		else break;
		if( bSameCount == 4 )
		{
			bMostValue--;
			bMostIndex = i+1;
			bSameCount = 1;
			i++;
		}
	}
	for( i = bMostIndex; i < bMostIndex+bSameCount; i++ )
	{
		BYTE bIndex = (bCardRecord[i]>>4);
		bColorIndex[bIndex]++;
	}
	for( i = 3; i >= 0; i-- )
		if( bColorIndex[i]==0 ) break;

	ASSERT( i>=0 );
	bMostCard = MakeCardData(bMostValue-3,i );

	return bMostCard;
}

//判断是否可以出完
bool CGameLogic::IsOutCardFinish(WORD wChairId,const tagAnalysedCard &AnalysedCard,const BYTE bOutCardRecord[],BYTE bOutCardCount,bool bTurn,
								 const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],BYTE &bMostCount,BYTE &bLessCount )
{
	ASSERT( wChairId != INVALID_CHAIR );
	if( wChairId == INVALID_CHAIR ) return false;

	BYTE bMostCard = GetMostCardValue(AnalysedCard,bOutCardRecord,bOutCardCount);
	bLessCount = 0;
	bMostCount = 0;
	bLessCount = 0;
	for( int i = 0; i < CountArray(AnalysedCard.bCardCount); i++ )
	{
		BYTE bCount = i>=CT_SHUN_ZI-1?5:i+1;
		for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
		{
			if( i==CT_SHUN_ZI-1 || i==CT_TONG_HUA-1 || i==CT_TONG_HUA_SHUN-1 )
			{
				if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,
					cbCardData,cbCardCount) )
					bMostCount++;
				else
					bLessCount++;
				continue;
			}
			if( (i==CT_HU_LU-1||i==CT_TIE_ZHI-1) && CompareCard(AnalysedCard.bCardData[i][j*bCount],bMostCard) )
			{
				if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,
					cbCardData,cbCardCount) )
					bMostCount++;
				else
					bLessCount++;
				continue;
			}
			if( CompareCard(bMostCard,AnalysedCard.bCardData[i][j*bCount]) || bMostCard==AnalysedCard.bCardData[i][j*bCount] )
			{
				if( i==CT_HU_LU-1 || i==CT_TIE_ZHI-1 )
				{
					bMostCount += (i-3);
					bLessCount++;
				}else
					bMostCount += bCount;
			}
			else 
			{
				if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,cbCardData,cbCardCount) )
					bMostCount++;
				else
					bLessCount++;
			}
		}
	}
	
	//先出牌
	if( bTurn ) 
		return bMostCount+1>=bLessCount;
	else return bMostCount>=bLessCount;
}

//分析牌型
bool CGameLogic::AnalyseOutCardType( const tagAnalysedCard &cAnalysedCard,WORD wMeChairId,BYTE bCardType,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
									const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardType &OutCardType )
{
	//初始化
	ZeroMemory( &OutCardType,sizeof(OutCardType) );

	tagAnalysedCard AnalysedCard;
	CopyMemory( &AnalysedCard,&cAnalysedCard,sizeof(cAnalysedCard) );

	int bRemainCount=0;
	int bMostCount = 0;

	BYTE bCardData[GAME_PLAYER][13];
	BYTE bCardCount[GAME_PLAYER];
	CopyMemory( bCardData,cbCardData,sizeof(bCardData) );
	CopyMemory( bCardCount,cbCardCount,sizeof(bCardCount) );

	tagOutCardResult OutCardResult;
	BYTE bTempData[MAX_COUNT],bTempCount=0;
	BYTE bLessTypeIndex=0,bLessIndex = 0;

	//顺子以上牌型
	bLessTypeIndex = bCardType-1;
	if( bCardType >= CT_SHUN_ZI && bCardType <= CT_TONG_HUA_SHUN )
	{
		for( int k = CT_TONG_HUA_SHUN-1; k >= bCardType-1; k-- )
		{
			for( int i=AnalysedCard.bCardCount[k]-1; i>=0; i-- )
			{
				bTempCount = 5;
				CopyMemory( bTempData,&AnalysedCard.bCardData[k][i*bTempCount],sizeof(BYTE)*bTempCount );
				ASSERT( GetCardType(bTempData,bTempCount)>CT_ERROR );
				for( WORD j = (wMeChairId+1)%GAME_PLAYER; j != wMeChairId; j = (j+1)%GAME_PLAYER )
				{
					if( bCardCount[j]==0 ) continue;
					if( SearchTurnCard(bCardData[j],bCardCount[j],bTempData,bTempCount,OutCardResult) )
					{
						VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bCardData[j],bCardCount[j] ) );
						bCardCount[j] -= OutCardResult.cbCardCount;
						break;
					}
				}
				if( j == wMeChairId ) bMostCount++;
				else	bRemainCount++;
			}
		}
	}
	else						//顺子以下牌型
	{
		bTempCount = bCardType>=5?5:bCardType;
		for( int i=AnalysedCard.bCardCount[bCardType-1]-1; i>=0; i-- )
		{
			CopyMemory( bTempData,&AnalysedCard.bCardData[bCardType-1][i*bTempCount],sizeof(BYTE)*bTempCount );
			ASSERT( GetCardType(bTempData,bTempCount)>CT_ERROR );
			for( WORD j = (wMeChairId+1)%GAME_PLAYER; j != wMeChairId; j = (j+1)%GAME_PLAYER )
			{
				if( bCardCount[j]==0 ) continue;
				if( SearchTurnCard(bCardData[j],bCardCount[j],bTempData,bTempCount,OutCardResult) )
				{
					VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bCardData[j],bCardCount[j] ) );
					bCardCount[j] -= OutCardResult.cbCardCount;
					break;
				}
			}
			if( j == wMeChairId ) bMostCount++;
			else bRemainCount++;
		}
	}

	ASSERT( bCardType > CT_ERROR && bCardType <=CT_TONG_HUA_SHUN );
	bool bFound = false;
	//压牌时
	if( cbTurnCardCount > 0 )
	{
		BYTE bTurnCardType = GetCardType(cbTurnCardData,cbTurnCardCount);
		int i=bTurnCardType-1,j=0;
		switch(bTurnCardType)
		{
		case CT_SINGLE:
		case CT_DOUBLE:
		case CT_THREE_TIAO:
		case CT_FOUR_TIAO:
			{
				bTempCount = bTurnCardType;
				for( j = 0; j < AnalysedCard.bCardCount[bTurnCardType-1]; j++ )
				{
					if( CompareCard(cbTurnCardData,&AnalysedCard.bCardData[bTurnCardType-1][j*bTempCount],bTempCount,bTempCount) )
					{
						CopyMemory( bTempData,&AnalysedCard.bCardData[bTurnCardType-1][j*bTempCount],
							sizeof(BYTE)*bTempCount );
						ASSERT( GetCardType(bTempData,bTempCount)>CT_ERROR );
						bFound = true;
						break;
					}
				}
				if( !bFound ) return false;
				break;
			}
		case CT_SHUN_ZI:
		case CT_TONG_HUA:
		case CT_HU_LU:
		case CT_TIE_ZHI:
		case CT_TONG_HUA_SHUN:
			{
				bTempCount = 5;
				for( i = bTurnCardType-1; i < CT_TONG_HUA_SHUN; i++ )
				{
					for( j = 0; j < AnalysedCard.bCardCount[i]; j++ )
					{
						if( CompareCard(cbTurnCardData,&AnalysedCard.bCardData[i][j*5],5,5) )
						{
							CopyMemory( bTempData,&AnalysedCard.bCardData[i][j*5],
								sizeof(BYTE)*5 );
							ASSERT( GetCardType(bTempData,bTempCount)>CT_ERROR );
							bFound = true;
							break;
						}
					}
					if( bFound ) break;
				}
				if( !bFound ) return false;
				break;
			}
		}
		if( bFound )
		{
			bLessTypeIndex = i;
			bLessIndex = j;
			for( WORD n = (wMeChairId+1)%GAME_PLAYER; n != wMeChairId; n = (n+1)%GAME_PLAYER )
			{
				if( SearchTurnCard(cbCardData[n],cbCardCount[n],bTempData,bTempCount,OutCardResult) )
					break;
			}
			if( n == wMeChairId ) 
				bMostCount--;
			else bRemainCount--;
		}
	}
	
	//计算牌型剩余数
	if( bMostCount >= bRemainCount )
	{
		OutCardType.bPower = true;
		OutCardType.bOutAll = true;
		OutCardType.bRemainCount = 0;
		OutCardType.bCardCount = bLessTypeIndex>=4?5:bLessTypeIndex+1;
		CopyMemory( OutCardType.bCardData,&AnalysedCard.bCardData[bLessTypeIndex][bLessIndex*OutCardType.bCardCount],sizeof(BYTE)*OutCardType.bCardCount );
		SortCardList( OutCardType.bCardData,OutCardType.bCardCount );
	}
	else if( bMostCount >= bRemainCount-1 )
	{
		OutCardType.bPower = false;
		OutCardType.bOutAll = true;
		OutCardType.bRemainCount = 0;
		OutCardType.bCardCount = bLessTypeIndex>=4?5:bLessTypeIndex+1;
		CopyMemory( OutCardType.bCardData,&AnalysedCard.bCardData[bLessTypeIndex][bLessIndex*OutCardType.bCardCount],sizeof(BYTE)*OutCardType.bCardCount );
		SortCardList( OutCardType.bCardData,OutCardType.bCardCount );
	}
	else
	{
		OutCardType.bPower = false;
		OutCardType.bOutAll = false;
		OutCardType.bRemainCount = bRemainCount - bMostCount;
		OutCardType.bCardCount = bLessTypeIndex>=4?5:bLessTypeIndex+1;
		CopyMemory( OutCardType.bCardData,&AnalysedCard.bCardData[bLessTypeIndex][bLessIndex*OutCardType.bCardCount],sizeof(BYTE)*OutCardType.bCardCount );
		SortCardList( OutCardType.bCardData,OutCardType.bCardCount );
	}

	return bFound;
}


//搜索出牌
bool CGameLogic::SearchTurnCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,tagOutCardResult &OutCardResult)
{
	//初始化
	ZeroMemory( &OutCardResult,sizeof(OutCardResult) );

	//构造扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//出牌判断
	if (cbTurnCardCount>0)
	{
		//获取类型
		BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

		//简单类型
		switch (cbTurnOutType)
		{
		case CT_SINGLE:					//单牌类型
		case CT_DOUBLE:					//对牌类型
		case CT_THREE_TIAO:				//三条类型
		case CT_FOUR_TIAO:				//四条类型
			{										//找最小
				//分析扑克
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

				//获取数值
				BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

				//寻找扑克
				for (BYTE i=cbTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
				{
					for (BYTE j=0;j<AnalyseResult.cbCardCount[i];j++)
					{
						//大小对比
						BYTE cbIndex=(AnalyseResult.cbCardCount[i]-j-1)*(i+1);
						if (CompareCard(cbTurnCardData[0],AnalyseResult.cbCardData[i][cbIndex])==false) continue;
						//设置结果
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][cbIndex],sizeof(BYTE)*cbTurnCardCount);
						SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
						return true;
					}
				}

				return false;
			}
		}

		//组合类型
		if (cbCardCount>=5)
		{
			//分析分布
			tagDistributing Distributing;
			AnalysebDistributing(cbCardData,cbCardCount,Distributing);

			//搜索顺子
			if (cbTurnOutType<=CT_SHUN_ZI)
			{
				//搜索顺子
				BYTE cbReferCard=(cbTurnOutType==CT_SHUN_ZI)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchShunZi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					//设置结果
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//搜索同花
			if (cbTurnOutType<=CT_TONG_HUA)
			{
				//搜索同花
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHua(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//搜索葫芦
			if (cbTurnOutType<=CT_HU_LU)
			{
				//搜索葫芦
				BYTE cbReferCard=(cbTurnOutType==CT_HU_LU)?cbTurnCardData[2]:0x00;
				OutCardResult.cbCardCount=SearchHuLu(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//搜索铁支
			if (cbTurnOutType<=CT_TIE_ZHI)
			{
				//搜索铁支
				BYTE cbReferCard=(cbTurnOutType==CT_TIE_ZHI)?cbTurnCardData[1]:0x00;
				OutCardResult.cbCardCount=SearchTieZhi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//搜索同花顺
			if (cbTurnOutType<=CT_TONG_HUA_SHUN)
			{
				//搜索同花顺
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA_SHUN)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHuaShun(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}
		}

		return false;
	}
	return false;
}

//判断是否最大
bool CGameLogic::IsTurnCardMost(WORD wChairId,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,const BYTE (&cbCardData)[GAME_PLAYER][13],
								const BYTE cbCardCount[])
{
	BYTE bTurnCardData[MAX_COUNT];
	CopyMemory( bTurnCardData,cbTurnCardData,sizeof(BYTE)*cbTurnCardCount );
	SortCardList( bTurnCardData,cbTurnCardCount );
	ASSERT( GetCardType(bTurnCardData,cbTurnCardCount)>CT_ERROR );
	if( GetCardType(bTurnCardData,cbTurnCardCount) == CT_ERROR )
		return false;
	ASSERT( wChairId != INVALID_CHAIR );
	if( wChairId == INVALID_CHAIR ) 
		return false;

	//判断其他玩家是否可以压牌
	tagOutCardResult OutCardResult;
	for( WORD i = (wChairId+1)%GAME_PLAYER; i != wChairId; i = (i+1)%GAME_PLAYER )
		if( SearchTurnCard(cbCardData[i],cbCardCount[i],bTurnCardData,cbTurnCardCount,
			OutCardResult) )
			break;
	if( i == wChairId ) return true;
	return false;
}

//搜索出牌
bool CGameLogic::StrusteeOutCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
							   bool bOutCard3,bool bOutMost,tagOutCardResult &OutCardResult )
{
	//设置结果
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//构造扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//出牌判断
	if (cbTurnCardCount!=0)
	{
		//获取类型
		BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

		//简单类型
		switch (cbTurnOutType)
		{
		case CT_SINGLE:					//单牌类型
			{
				//拦最大
				if( bOutMost )
				{
					if( CompareCard(cbTurnCardData[0],cbCardData[0]) == false ) return false;
					else
					{
						OutCardResult.cbCardCount = 1;
						OutCardResult.cbResultCard[0] = cbCardData[0];
						return true;
					}
				}
				else
				{
					//从最小找起
					for( BYTE i = 0; i < cbCardCount; i++ )
					{
						BYTE cbCard = cbCardData[cbCardCount-i-1];
						if( CompareCard(cbTurnCardData[0],cbCard) )
						{
							OutCardResult.cbCardCount = 1;
							OutCardResult.cbResultCard[0] = cbCard;
							return true;
						}
					}
				}
				return false;
			}
		case CT_DOUBLE:					//对牌类型
		case CT_THREE_TIAO:				//三条类型
		case CT_FOUR_TIAO:				//四条类型
			{										//找最小
				//分析扑克
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

				//获取数值
				BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

				//寻找扑克
				for (BYTE i=cbTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
				{
					for (BYTE j=0;j<AnalyseResult.cbCardCount[i];j++)
					{
						//大小对比
						BYTE cbIndex=(AnalyseResult.cbCardCount[i]-j-1)*(i+1);
						if (CompareCard(cbTurnCardData[0],AnalyseResult.cbCardData[i][cbIndex])==false) continue;

						//设置结果
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][cbIndex],sizeof(BYTE)*cbTurnCardCount);

						return true;
					}
				}

				return false;
			}
		}

		//组合类型
		if (cbCardCount>=5)
		{
			//分析分布
			tagDistributing Distributing;
			AnalysebDistributing(cbCardData,cbCardCount,Distributing);

			//搜索顺子
			if (cbTurnOutType<=CT_SHUN_ZI)
			{
				//搜索顺子
				BYTE cbReferCard=(cbTurnOutType==CT_SHUN_ZI)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchShunZi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}

			//搜索同花
			if (cbTurnOutType<=CT_TONG_HUA)
			{
				//搜索同花
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHua(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}

			//搜索葫芦
			if (cbTurnOutType<=CT_HU_LU)
			{
				//搜索葫芦
				BYTE cbReferCard=(cbTurnOutType==CT_HU_LU)?cbTurnCardData[2]:0x00;
				OutCardResult.cbCardCount=SearchHuLu(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}

			//搜索铁支
			if (cbTurnOutType<=CT_TIE_ZHI)
			{
				//搜索铁支
				BYTE cbReferCard=(cbTurnOutType==CT_TIE_ZHI)?cbTurnCardData[1]:0x00;
				OutCardResult.cbCardCount=SearchTieZhi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}

			//搜索同花顺
			if (cbTurnOutType<=CT_TONG_HUA_SHUN)
			{
				//搜索同花顺
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA_SHUN)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHuaShun(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//结果判断
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}
		}

		return false;
	}
	else
	{
		//必须出方块3
		if( bOutCard3 )
		{
			for( BYTE i = 0; i < cbCardCount; i++ )
			{
				if( cbCardData[cbCardCount-i-1] == 0x03 )
				{
					OutCardResult.cbCardCount = 1;
					OutCardResult.cbResultCard[0] = 0x03;
					//搜索同牌
					while( ++i < cbCardCount )
					{
						if( GetCardLogicValue(cbCardData[cbCardCount-i-1]) == 0x03 )
						{
							BYTE cbIndex = OutCardResult.cbCardCount++;
							OutCardResult.cbResultCard[cbIndex] = cbCardData[cbCardCount-i-1];
						}
						else break;
					}
					return true;
				}
			}
		}

		//获取数值
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[cbCardCount-1]);

		//多牌判断
		BYTE cbSameCount=1;
		for (BYTE i=1;i<cbCardCount;i++)
		{
			if (GetCardLogicValue(cbCardData[cbCardCount-i-1])==cbLogicValue) cbSameCount++;
			else break;
		}

		//完成处理
		if (cbSameCount>1)
		{
			OutCardResult.cbCardCount=cbSameCount;
			for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=cbCardData[cbCardCount-1-j];
			return true;
		}

		//单牌处理
		OutCardResult.cbCardCount=1;
		OutCardResult.cbResultCard[0]=bOutMost?cbCardData[0]:cbCardData[cbCardCount-1];

		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
