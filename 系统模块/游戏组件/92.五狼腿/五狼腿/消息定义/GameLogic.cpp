#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//静态变量

//扑克数据
const BYTE	CGameLogic::m_bCardListData[108]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x41,0x42,															//大鬼小鬼
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x41,0x42,															//大鬼小鬼
};

//////////////////////////////////////////////////////////////////////////



//获取类型
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount, bool bDoubleBomo)
{
	//简单牌形
	switch (bCardCount)
	{
	case 1:		//单牌
		{
			return CT_SINGLE;
		}
	case 2:		//对牌
		{
			//双王判断
			if ((bCardData[0]==0x42)&&(bCardData[1]==0x42)) return CT_KING_BIG;
			if ((bCardData[0]==0x41)&&(bCardData[1]==0x41)) return CT_KING_LITTET;

			//对牌判断
			BYTE bLastLogicValue=GetCardLogicValue(bCardData[1]);
			BYTE bFirstLogicValue=GetCardLogicValue(bCardData[0]);
			return (bFirstLogicValue==bLastLogicValue)?CT_DOUBLE:CT_ERROR;
		}
	default:	//炸弹腿牌
		{
			//腿牌判断
			if (bCardCount==3)
			{
				//数值对比
				bool bFixValue=true;
				if ((bCardData[2]&LOGIC_MASK_VALUE)!=5) bFixValue=false;
				if ((bCardData[1]&LOGIC_MASK_VALUE)!=10) bFixValue=false;
				if ((bCardData[0]&LOGIC_MASK_VALUE)!=13) bFixValue=false;

				//花色对比
				if (bFixValue==true)
				{
					//获取花色
					BYTE cbCardColor[3];
					cbCardColor[0]=bCardData[0]&LOGIC_MASK_COLOR;
					cbCardColor[1]=bCardData[1]&LOGIC_MASK_COLOR;
					cbCardColor[2]=bCardData[2]&LOGIC_MASK_COLOR;

					//花色判断
					if (cbCardColor[0]!=cbCardColor[1]) return CT_CRURA_FAKE;
					if (cbCardColor[0]!=cbCardColor[2]) return CT_CRURA_FAKE;

					return CT_CRURA_PURE;
				}
			}

			//炸弹判断
			BYTE bFirstLogicValue=GetCardLogicValue(bCardData[0]);
			BYTE bLastLogicValue=GetCardLogicValue(bCardData[bCardCount-1]);
			if (bFirstLogicValue==bLastLogicValue) return CT_BOMB;

			break;
		}
	}

	//双炸类型
	if ((bDoubleBomo==true)&&(bCardCount>=6))
	{
		//变量定义
		BYTE cbCount[2]={1,1};
		BYTE cbValue[2]={GetCardValue(bCardData[0]),GetCardValue(bCardData[bCardCount-1])};

		//类型判断
		for (int i=1;i<bCardCount-1;i++)
		{
			BYTE cbTemp=GetCardValue(bCardData[i]);
			if (cbTemp==cbValue[0]) cbCount[0]++;
			else if (cbTemp==cbValue[1]) cbCount[1]++;
			else break;
		}

		//结果判断
		if (((cbCount[0]+cbCount[1])==bCardCount)&&(cbCount[0]>=3)&&(cbCount[1]>=3)) return CT_DOUBLE_BOMB;
	}

	//分析扑克
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(bCardData,bCardCount,AnalyseResult);

	//两连判断
	if ((AnalyseResult.bDoubleCount>=3)&&(AnalyseResult.bDoubleCount*2==bCardCount))
	{
		//逻辑调整
		BYTE bLogicValue=AnalyseResult.bDoubleLogicVolue[0];

		//连牌判断
		if (bLogicValue<=14)
		{
			//连牌判断
			bool bSeriesCard=false;
			for (BYTE i=1;i<AnalyseResult.bDoubleCount;i++)
			{
				if (AnalyseResult.bDoubleLogicVolue[i]!=(bLogicValue-i)) break;
			}

			//两连判断
			if (i==AnalyseResult.bDoubleCount) return CT_DOUBLE_LINE;
		}
	}

	//单连判断
	if ((AnalyseResult.bSignedCount>=3)&&(AnalyseResult.bSignedCount==bCardCount))
	{
		//逻辑调整
		BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

		//连牌判断
		if (bLogicValue<=14)
		{
			//连牌判断
			bool bSeriesCard=false;
			for (BYTE i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-i)) break;
			}

			//单连判断
			if (i==AnalyseResult.bSignedCount) return CT_SINGLE_LINE;
		}
	}

	return CT_ERROR;
}

//排列扑克
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount, BYTE cbSortType)
{
	if (bCardCount==0) return;

	switch (cbSortType)
	{
	case ST_ORDER:
		{
			//转换数值
			BYTE bLogicValue[MAX_COUNT];
			for (BYTE i=0;i<bCardCount;i++) bLogicValue[i]=GetCardLogicValue(bCardData[i]);

			//排序操作
			bool bSorted=true;
			BYTE bTempData,bLast=bCardCount-1;
			do
			{
				bSorted=true;
				for (BYTE i=0;i<bLast;i++)
				{
					if ((bLogicValue[i]<bLogicValue[i+1])||
						((bLogicValue[i]==bLogicValue[i+1])&&(bCardData[i]<bCardData[i+1])))
					{
						//交换位置
						bTempData=bCardData[i];
						bCardData[i]=bCardData[i+1];
						bCardData[i+1]=bTempData;
						bTempData=bLogicValue[i];
						bLogicValue[i]=bLogicValue[i+1];
						bLogicValue[i+1]=bTempData;
						bSorted=false;
					}	
				}
				bLast--;
			} while(bSorted==false);

			return;
		}
	case ST_VIEW:
	case ST_BOMB:
		{
			//提取扑克
			BYTE cbResultCount=0;
			BYTE cbTempCard[MAX_COUNT];
			CopyMemory(cbTempCard,bCardData,sizeof(BYTE)*bCardCount);

			//提取纯腿
			BYTE cbColorMask[]={0x20,0x20,0x00,0x00,0x30,0x30,0x10,0x10};
			for (BYTE j=0;j<CountArray(cbColorMask);j++)
			{
				BYTE cbIndex[3]={0xFF,0xFF,0xFF};
				for (BYTE i=0;i<bCardCount;i++)
				{
					//提取判断
					if (cbTempCard[i]==0) continue;

					//属性判断
					BYTE cbCardColor=GetCardColor(cbTempCard[i]);
					BYTE cbCardValue=GetCardValue(cbTempCard[i]);
					if (cbCardColor==cbColorMask[j])
					{
						if (cbCardValue==5) cbIndex[0]=i;
						else if (cbCardValue==10) cbIndex[1]=i;
						else if (cbCardValue==13) cbIndex[2]=i;
					}

					//结果处理
					if ((cbIndex[0]!=0xFF)&&(cbIndex[1]!=0xFF)&&(cbIndex[2]!=0xFF))
					{
						bCardData[cbResultCount++]=cbTempCard[cbIndex[0]];
						bCardData[cbResultCount++]=cbTempCard[cbIndex[1]];
						bCardData[cbResultCount++]=cbTempCard[cbIndex[2]];
						cbTempCard[cbIndex[0]]=0;
						cbTempCard[cbIndex[1]]=0;
						cbTempCard[cbIndex[2]]=0;
						break;
					}
				}
			}

			//提取假腿
			bool bSuccess=false;
			do
			{
				bSuccess=false;
				BYTE cbIndex[3]={0xFF,0xFF,0xFF};
				for (BYTE i=0;i<bCardCount;i++)
				{
					//提取判断
					if (cbTempCard[i]==0) continue;

					//属性判断
					BYTE cbCardValue=GetCardValue(cbTempCard[i]);
					if (cbCardValue==5) cbIndex[0]=i;
					else if (cbCardValue==10) cbIndex[1]=i;
					else if (cbCardValue==13) cbIndex[2]=i;

					//结果处理
					if ((cbIndex[0]!=0xFF)&&(cbIndex[1]!=0xFF)&&(cbIndex[2]!=0xFF))
					{
						bSuccess=true;
						bCardData[cbResultCount++]=cbTempCard[cbIndex[0]];
						bCardData[cbResultCount++]=cbTempCard[cbIndex[1]];
						bCardData[cbResultCount++]=cbTempCard[cbIndex[2]];
						cbTempCard[cbIndex[0]]=0;
						cbTempCard[cbIndex[1]]=0;
						cbTempCard[cbIndex[2]]=0;
						break;
					}
				}
			} while (bSuccess==true);

			//提取扑克
			if (cbSortType==ST_BOMB)
			{
				//获取分布
				tagDistributing Distributing;
				GetDistributingInfo(cbTempCard,bCardCount,Distributing);

				//提取扑克
				BYTE cbLeftCount=0;
				for (int i=8;i>0;i--)
				{
					for (int j=14;j>=0;j--)
					{
						if (Distributing.cbDistributing[j][5]==i)
						{
							for (int k=0;k<5;k++)
							{
								for (int l=0;l<Distributing.cbDistributing[j][k];l++)
								{
									BYTE cbCardTemp=MakeCardData(j,k);
									bCardData[cbResultCount+cbLeftCount++]=cbCardTemp;
								}
							}
						}
					}
				}
			}
			else
			{
				//提取扑克
				BYTE cbLeftCount=0;
				for (BYTE i=0;i<bCardCount;i++)
				{
					if (cbTempCard[i]!=0)
					{
						bCardData[cbResultCount+cbLeftCount++]=cbTempCard[i];
						cbTempCard[i]=0;
					}
				}

				//普通排列
				SortCardList(&bCardData[cbResultCount],cbLeftCount,ST_ORDER);
			}

			return;
		}
	}
}

//混乱扑克
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//混乱准备
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//检验数据
	ASSERT(bRemoveCount<=bCardCount);

	//定义变量
	BYTE bDeleteCount=0,bTempCardData[MAX_COUNT];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//置零扑克
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//清理扑克
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//变量定义
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//有效判断
	if ((cbColor<=0x30)&&(cbValue>=0x01)&&(cbValue<=0x0D)) return true;
	if ((cbColor==0x40)&&(cbValue>=0x01)&&(cbValue<=0x02)) return true;

	return false;
}

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//扑克属性
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//转换数值
	if (bCardColor==0x40) return bCardValue+15;
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//构造扑克
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	//构造数值
	if (cbColorIndex==4)
	{
		return (0x40|(cbValueIndex-12));
	}

	BYTE cbCardValue=(cbValueIndex+3);
	if ((cbValueIndex>=11)&&(cbValueIndex<=12)) cbCardValue=cbValueIndex-10;

	return ((cbColorIndex<<4)|cbCardValue);
}

//获取分布
void CGameLogic::GetDistributingInfo(BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
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
		Distributing.cbDistributing[cbLogicValue-3][5]++;
		Distributing.cbDistributing[cbLogicValue-3][cbCardColor>>4]++;
	}

	return;
}

//对比扑克
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//获取类型
	BYTE bNextType=GetCardType(bNextList,bNextCount,true);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount,((bNextType==CT_DOUBLE_BOMB)||(bNextType==CT_DOUBLE_LINE)));

	//类型判断
	if (bFirstType==CT_ERROR) return false;

	//简单类型
	switch (bFirstType)
	{
	case CT_SINGLE:
	case CT_DOUBLE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
		{
			//类型判断
			if (bNextType!=bFirstType) return false;
			if (bFirstCount!=bNextCount) return false;
			
			//大小对比
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			return (bFirstLogicValue>bNextLogicValue);
		}
	}

	//特殊判断
	if (bFirstCount>=6)
	{
		//多个炸弹
		if ((bFirstType==CT_BOMB)&&(bNextType==CT_DOUBLE_BOMB))
		{
			//数目对比
			if (bFirstCount<bNextCount) return false;

			//数值对比
			BYTE cbNextValue=GetCardLogicValue(bNextList[0]);
			BYTE cbFirstValue=GetCardLogicValue(bFirstList[0]);
			return cbFirstValue>cbNextValue;
		}

		//炸弹双顺
		if ((bFirstType==CT_BOMB)&&(bNextType==CT_DOUBLE_LINE)) return true;
	}

	//同型对比
	if (bFirstType==bNextType)
	{
		switch (bFirstType)
		{
		case CT_BOMB:			//炸弹类型
			{
				//数目对比
				if (bFirstCount>bNextCount) return true;

				//数值对比
				if (bFirstCount==bNextCount)
				{
					BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
					BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
					return (bFirstLogicValue>bNextLogicValue);
				}

				return false;
			}
		case CT_CRURA_PURE:		//纯腿类型
			{
				//获取花色
				BYTE cbNextColor=GetCardColor(bNextList[0]);
				BYTE cbFirstColor=GetCardColor(bFirstList[0]);

				//花色对比
				BYTE cbFirstOrder=0x00;
				if (cbFirstColor==0x00) cbFirstOrder=0x03;
				else if (cbFirstColor==0x10) cbFirstOrder=0x01;
				else if (cbFirstColor==0x20) cbFirstOrder=0x04;
				else if (cbFirstColor==0x30) cbFirstOrder=0x02;

				//花色对比
				BYTE cbNextOrder=0x00;
				if (cbNextColor==0x00) cbNextOrder=0x03;
				else if (cbNextColor==0x10) cbNextOrder=0x01;
				else if (cbNextColor==0x20) cbNextOrder=0x04;
				else if (cbNextColor==0x30) cbNextOrder=0x02;

				return cbFirstOrder>cbNextOrder;
			}
		case CT_DOUBLE_BOMB:	//双炸类型
			{
				//变量定义
				BYTE cbNextCount[2]={1,1};
				BYTE cbFirstCount[2]={1,1};
				BYTE cbNextValue[2]={GetCardValue(bNextList[0]),GetCardValue(bNextList[bNextCount-1])};
				BYTE cbFirstValue[2]={GetCardValue(bFirstList[0]),GetCardValue(bFirstList[bFirstCount-1])};

				//数据收集
				for (int i=1;i<bNextCount-1;i++)
				{
					BYTE cbTemp=GetCardValue(bNextList[i]);
					if (cbTemp==cbNextValue[0]) cbNextCount[0]++;
					else if (cbTemp==cbNextValue[1]) cbNextCount[1]++;
					else return true;
				}

				//数据收集
				for (int i=1;i<bFirstCount-1;i++)
				{
					BYTE cbTemp=GetCardValue(bFirstList[i]);
					if (cbTemp==cbFirstValue[0]) cbFirstCount[0]++;
					else if (cbTemp==cbFirstValue[1]) cbFirstCount[1]++;
					else return false;
				}

				//位置处理
				if ((cbNextCount[0]>=cbNextCount[1])&&(cbFirstCount[0]>=cbFirstCount[1]))
				{
					//数目对比
					if (cbNextCount[0]>cbFirstCount[0]) return false;
					if (cbNextCount[1]>cbFirstCount[1]) return false;

					//数值对比
					if (cbNextCount[0]==cbFirstCount[0])
					{
						BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
						BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
						if (bFirstLogicValue<=bNextLogicValue) return false;
					}

					//数值对比
					if (cbNextCount[1]==cbFirstCount[1])
					{
						BYTE bNextLogicValue=GetCardLogicValue(bNextList[bNextCount-1]);
						BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[bFirstCount-1]);
						if (bFirstLogicValue<=bNextLogicValue) return false;
					}
				}
				else
				{
					//数目对比
					if (cbNextCount[0]>cbFirstCount[1]) return false;
					if (cbNextCount[1]>cbFirstCount[0]) return false;

					//数值对比
					if (cbNextCount[0]==cbFirstCount[1])
					{
						BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
						BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[bFirstCount-1]);
						if (bFirstLogicValue<=bNextLogicValue) return false;
					}

					//数值对比
					if (cbNextCount[1]==cbFirstCount[0])
					{
						BYTE bNextLogicValue=GetCardLogicValue(bNextList[bNextCount-1]);
						BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
						if (bFirstLogicValue<=bNextLogicValue) return false;
					}
				}

				return true;
			}
		}

		return false;
	}

	//炸弹双顺
	if ((bNextType==CT_DOUBLE_LINE)&&(bFirstType==CT_BOMB)) return false;

	return bFirstType>bNextType;
}

//分析扑克
void CGameLogic::AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult)
{
	//变量定义
	BYTE bSameCount=1,bCardValueTemp=0;
	BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

	//设置结果
	memset(&AnalyseResult,0,sizeof(AnalyseResult));

	//扑克分析
	for (BYTE i=1;i<bCardCount;i++)
	{
		//获取扑克
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//保存结果
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 2:		//两张
				{
					AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount++]=bLogicValue;
					break;
				}
			case 3:		//三张
				{
					AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount++]=bLogicValue;
					break;
				}
			case 4:		//四张
				{
					AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount++]=bLogicValue;
					break;
				}
			}
		}

		//设置变量
		if ((bCardValueTemp!=bLogicValue)&&(i!=(bCardCount-1)))
		{
			bSameCount=1;
			bLogicValue=bCardValueTemp;
		}
	}

	//单牌数目
	BYTE bOtherCount=AnalyseResult.bDoubleCount*2+AnalyseResult.bThreeCount*3+AnalyseResult.bFourCount*4;
	AnalyseResult.bSignedCount=bCardCount-bOtherCount;

	return;
}
#include <algorithm>
bool _cmp1(BYTE _bL, BYTE _bR)
{
	BYTE l = CGameLogic::GetCardColor(_bL);
	BYTE r = CGameLogic::GetCardColor(_bR);
	if(l == r)
		return CGameLogic::GetCardValue(_bL) < CGameLogic::GetCardValue(_bR);
	else
		return l<r; 
}
bool _cmp2(BYTE _bL, BYTE _bR)
{
	BYTE l = CGameLogic::GetCardValue(_bL);
	BYTE r = CGameLogic::GetCardValue(_bR);
	if(l == r)
		return CGameLogic::GetCardColor(_bL) < CGameLogic::GetCardColor(_bR);
	else
		return l<r;
}
void	CGameLogic::_DeleteCards(BYTE bCards[], BYTE& bCardCount, BYTE bDelete[], BYTE bDeleteCount)
{
	BYTE i,j,k;
	for(i=0; i<bDeleteCount; ++i){
		for(k = 0; k<bCardCount; ++k)
			if(bCards[k] == bDelete[i]){
				for(j=k; j<bCardCount; ++j)
					bCards[j] = bCards[j+1];
				--bCardCount;
				break;
			}
	}

}


void CGameLogic::_GetPrivateCard(BYTE bCards[], BYTE& bCardCount, BYTE bPrivateCards[], BYTE& bPrivateCount)
{
	bPrivateCount = 0;
	//是否已经提出
	bool bisOut[MAX_COUNT];
	memset(bisOut, 0, sizeof(bisOut));
	BYTE  bBufCard1[MAX_COUNT];
	BYTE bBufCard2[MAX_COUNT];
	BYTE bBufCardCount1 = 0, bBufCardCount2 = 0;
	
	//首先查找腿
	BYTE i,j,k;
	for(i=0; i<bCardCount; ++i){
		k = GetCardValue(bCards[i]);
		if( k == 5 || k == 10 || k==13)
		{
			//bisOut[i] = true;
			bBufCard1[bBufCardCount1] = bCards[i];
			++bBufCardCount1;
		}
	}
	//对于取出得牌按照花色排序
	if(bBufCardCount1>2){
		bPrivateCards[bPrivateCount++] = 0xff;
		BYTE bb[3] = {0};
		std::sort(bBufCard1, bBufCard1+bBufCardCount1, _cmp1);
		j = GetCardColor(bBufCard1[0]);
		k = GetCardValue(bBufCard1[0]);
		if(k == 5) ++bb[0];
		else if(k == 10) ++bb[1];
		else  ++bb[2];
		
		bool bEqual = false;
		//先整理出真腿
		for(i=1; i<bBufCardCount1; ++i){
			if(GetCardColor(bBufCard1[i]) == j){
				bEqual = true;
				k = GetCardValue(bBufCard1[i]);
				if(k == 5) ++bb[0];
				else if(k == 10) ++bb[1];
				else  ++bb[2];
			}else{
				bEqual = false;
				BYTE bMin = 255;
				for( k=0; k<3; ++k)
					if(bMin > bb[k]) bMin = bb[k];
				if(bMin){
					for(k=0; k<bMin; ++k){
						bPrivateCards[bPrivateCount++] = j|5;
						bPrivateCards[bPrivateCount++] = j|10;
						bPrivateCards[bPrivateCount++] = j|13;
					}
				}
				memset(bb, 0, sizeof(bb));
				j = GetCardColor(bBufCard1[i]);
				k = GetCardValue(bBufCard1[i]);
				if(k == 5) ++bb[0];
				else if(k == 10) ++bb[1];
				else  ++bb[2];
			}
		}
		if(bEqual){
			BYTE bMin = 255;
			for( k=0; k<3; ++k)
				if(bMin > bb[k]) bMin = bb[k];
			if(bMin){
				for(k=0; k<bMin; ++k){
					bPrivateCards[bPrivateCount++] = j|5;
					bPrivateCards[bPrivateCount++] = j|10;
					bPrivateCards[bPrivateCount++] = j|13;
				}
			}
		}
		_DeleteCards(bBufCard1, bBufCardCount1, bPrivateCards, bPrivateCount);
		if(bBufCardCount1>2){
			//以值排序
			std::sort(bBufCard1, bBufCard1+bBufCardCount1, _cmp2);
			//假腿
			memset(bb, 0, sizeof(bb));
			for(i=1; i<bBufCardCount1; ++i){
				k = GetCardValue(bBufCard1[i]);
				if(k == 5) ++bb[0];
				else if(k == 10) ++bb[1];
				else  ++bb[2];
			}

			BYTE bMin = 255;
			for( k=0; k<3; ++k)
				if(bMin > bb[k]) bMin = bb[k];
			if(bMin){
				k = bBufCard1[0];
				for(i=1; i<bBufCardCount1; ++i){
					//if(i>0) k = GetCardValue(bBufCard1[i-1]);
					if(GetCardValue(bBufCard1[i]) != GetCardValue(bBufCard1[i-1])){
						j = i;
						for(BYTE ii=0; ii<bMin; ++ii)
							bPrivateCards[bPrivateCount++] = bBufCard1[i+ii-1];
					}
				}
				for(BYTE ii=0; ii<bMin; ++ii)
					bPrivateCards[bPrivateCount++] = bBufCard1[j+ii];

				_DeleteCards(bBufCard1, bBufCardCount1, bPrivateCards+bPrivateCount-bMin*3, bMin*3);

			}

		}
	}
	bPrivateCards[bPrivateCount++] = 0xfe;
	//清理出炸弹..
	BYTE bSerialCount = 0;
	k = -1;
	for(i=1; i<bCardCount+1; ++i){
		if(GetCardLogicValue(bCards[i-1]) != GetCardLogicValue(bCards[i])){
			if((bSerialCount>=2) || (bSerialCount == 1 && GetCardColor(bCards[i-1])== 0x40)){
				++bSerialCount;
				memcpy(bPrivateCards+bPrivateCount, bCards+i-bSerialCount, bSerialCount);
				bPrivateCount += bSerialCount;
			}
			bSerialCount = 0;

		}else
			++bSerialCount;
		
	}
	if((bSerialCount>=2) || (bSerialCount == 1 && GetCardColor(bCards[i-1])== 0x40)){
		++bSerialCount;
		memcpy(bPrivateCards+bPrivateCount, bCards+i-bSerialCount, bSerialCount);
		bPrivateCount += bSerialCount;
	}
	
	_DeleteCards(bCards, bCardCount, bPrivateCards, bPrivateCount);

}
void	CGameLogic::_AddCards(BYTE bCards[], BYTE& bCardCount, BYTE bAdd[], BYTE bAddCount)
{
	BYTE i,j,k,pos=0xff;
	for(i=0; i<bAddCount; ++i)
		if(bAdd[i] == 0xfe)
		{pos = i; break;}
	//首先清理炸弹..
	BYTE bKeep[20] = {0};
	BYTE bKeepCount = 0;
	if(pos != 0xff){
		k = GetCardLogicValue(bAdd[0]);
		bKeep[bKeepCount++] = k;
		for(i=pos+1; i<bAddCount; ++i){
			BYTE bValue = GetCardLogicValue(bAdd[i]);
			if(k != bValue){
				bKeep[bKeepCount++] = bValue;
			}
			bCards[bCardCount++] = bAdd[i];
		}
	}else
		 pos=bAddCount;
	
	for(i=1; i<pos; ++i){
		BYTE bValue = GetCardLogicValue(bAdd[i]);
		bool bIn = false;
		for(j=0; j<bKeepCount; ++j)
			if(bValue == bKeep[j]) 
			{
				bIn = true; break;
			}
		if(!bIn)
			bCards[bCardCount++] = bAdd[i];

	}
}
//出牌判断
bool CGameLogic::SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, tagOutCardResult & OutCardResult)
{
	//设置结果
	OutCardResult.cbCardCount=0;

	static BYTE _bBufCard[MAX_COUNT];
	static BYTE _bPrivateBufCard[MAX_COUNT*2];
	BYTE _bCardCount,_bPriCardCount;
	_bCardCount = bCardCount;
	memcpy(_bBufCard, bCardData, bCardCount*sizeof(BYTE));
	memset(_bPrivateBufCard, 0, sizeof(_bPrivateBufCard));
	_GetPrivateCard(_bBufCard, _bCardCount, _bPrivateBufCard, _bPriCardCount);

	BYTE bPreCount = bCardCount;
	if(_bCardCount != bCardCount && _bCardCount && bTurnOutType != CT_BOMB){
		//去除炸弹
		bCardData = _bBufCard;
		bCardCount = _bCardCount;
	}

	//类型搜索
	switch (bTurnOutType)
	{
	case CT_ERROR:					//错误类型
		{
			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bCardData[bCardCount-1]);

			//多牌判断
			BYTE cbSameCount=1;
			for (BYTE i=1;i<bCardCount;i++)
			{
				if (GetCardLogicValue(bCardData[bCardCount-i-1])==bLogicValue) cbSameCount++;
				else break;
			}

			//完成处理
			if (cbSameCount>1)
			{
				OutCardResult.cbCardCount=cbSameCount;
				for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=bCardData[bCardCount-1-j];
				return true;
			}

			//单牌处理
			OutCardResult.cbCardCount=1;
			OutCardResult.cbResultCard[0]=bCardData[bCardCount-1];

			return true;
		}
	case CT_SINGLE:					//单牌类型
		{
			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//对比大小
			for (BYTE i=0;i<bCardCount;i++)
			{
				if (GetCardLogicValue(bCardData[bCardCount-i-1])>bLogicValue)
				{
					OutCardResult.cbCardCount=1;
					OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
					return true;
				}
			}

			break;
		}
	case CT_DOUBLE:					//对牌类型
		{
			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//对比大小
			for (BYTE i=1;i<bCardCount;i++)
			{
				//获取扑克
				BYTE cbCardValue1=GetCardLogicValue(bCardData[bCardCount-i]);
				BYTE cbCardValue2=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//构造判断
				if ((cbCardValue1==cbCardValue2)&&(cbCardValue1>bLogicValue))
				{
					OutCardResult.cbCardCount=2;
					OutCardResult.cbResultCard[0]=bCardData[bCardCount-i];
					OutCardResult.cbResultCard[1]=bCardData[bCardCount-i-1];
					return true;
				}
			}

			break;
		}
	case CT_SINGLE_LINE:			//单连类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//搜索连牌
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue>=15) break;
				if (bHandLogicValue<=bLogicValue) continue;

				//搜索连牌
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<bCardCount;j++)
				{
					if ((GetCardLogicValue(bCardData[j])+bLineCount)==bHandLogicValue) 
					{
						//增加连数
						OutCardResult.cbResultCard[bLineCount++]=bCardData[j];

						//完成判断
						if (bLineCount==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_DOUBLE_LINE:	//对连类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//搜索连牌
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue<=bLogicValue) continue;
				if ((bTurnCardCount>2)&&(bHandLogicValue>=15)) break;

				//搜索连牌
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<(bCardCount-1);j++)
				{
					if (((GetCardLogicValue(bCardData[j])+bLineCount)==bHandLogicValue)
						&&((GetCardLogicValue(bCardData[j+1])+bLineCount)==bHandLogicValue))
					{
						//增加连数
						OutCardResult.cbResultCard[bLineCount*2]=bCardData[j];
						OutCardResult.cbResultCard[(bLineCount++)*2+1]=bCardData[j+1];

						//完成判断
						if (bLineCount*2==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_BOMB:		//炸弹类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//搜索炸弹
			for (BYTE i=bTurnCardCount-1;i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//炸弹判断
				BYTE cbCount=1;
				for (BYTE j=bCardCount-i-2;j>=0;j--)
				{
					if (GetCardLogicValue(bCardData[j])==bHandLogicValue) cbCount++;
					else break;
					if (j==0) break;
				}

				//构造判断
				if ((cbCount>bTurnCardCount)||((cbCount==bTurnCardCount)&&(bHandLogicValue>bLogicValue)))
				{
					//完成处理
					OutCardResult.cbCardCount=cbCount;
					for (BYTE j=0;j<cbCount;j++)
					{
						OutCardResult.cbResultCard[j]=bCardData[bCardCount-i-cbCount+j];
					}

					return true;
				}
			}

			break;
		}
	}
	//把之前删除得炸弹重新加入..
	if(bCardCount != bPreCount){
		_AddCards(bCardData, bCardCount, _bPrivateBufCard, _bPriCardCount);
		SortCardList(bCardData, bCardCount, ST_ORDER);
	}

	//双顺类型
	if ((bTurnOutType==CT_DOUBLE_LINE)||(bTurnOutType==CT_DOUBLE_BOMB))
	{
		//获取分布
		tagDistributing Distributing;
		GetDistributingInfo(bCardData,bCardCount,Distributing);

		//双炸类型
		if (bTurnOutType==CT_DOUBLE_BOMB)
		{
			//变量定义
			BYTE cbBombCount=0;
			BYTE cbBombIndex[2]={0xFF,0xFF};

			//变量定义
			BYTE cbTurnBombCount[2]={1,1};
			BYTE cbTurnBombValue[2]={GetCardValue(bTurnCardData[0]),GetCardValue(bTurnCardData[bTurnCardCount-1])};

			//数据收集
			for (int i=1;i<bTurnCardCount-1;i++)
			{
				BYTE cbTempValue=GetCardValue(bTurnCardData[i]);
				if (cbTempValue==cbTurnBombValue[0]) cbTurnBombCount[0]++;
				if (cbTempValue==cbTurnBombValue[1]) cbTurnBombCount[1]++;
			}

			//搜索双炸
			for (BYTE i=0;i<CountArray(Distributing.cbDistributing);i++)
			{
				//这里未完成
			}
		}

		//双顺类型
		if (bTurnOutType==CT_DOUBLE_LINE)
		{
			//变量定义
			BYTE cbBombCount=0;
			BYTE cbBombIndex[2]={0xFF,0xFF};

			//搜索双炸
			for (BYTE i=0;i<CountArray(Distributing.cbDistributing);i++)
			{
				//双顺类型
				if (Distributing.cbDistributing[i][5]>=3)
				{
					cbBombIndex[cbBombCount++]=i;
					if (cbBombCount>=2) break;
					continue;
				}
			}

			//结果判断
			if (cbBombCount==2)
			{
				for (BYTE cbColorIndex=0;cbColorIndex<5;cbColorIndex++)
				{
					for (BYTE i=0;i<Distributing.cbDistributing[cbBombIndex[0]][cbColorIndex];i++)
					{
						BYTE cbCardTemp=MakeCardData(cbBombIndex[0],cbColorIndex);
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardTemp;
					}
				}

				for (BYTE cbColorIndex=0;cbColorIndex<5;cbColorIndex++)
				{
					for (BYTE i=0;i<Distributing.cbDistributing[cbBombIndex[1]][cbColorIndex];i++)
					{
						BYTE cbCardTemp=MakeCardData(cbBombIndex[1],cbColorIndex);
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardTemp;
					}
				}

				return true;
			}
		}
	}

	//搜索炸弹
	if ((bTurnOutType<=CT_SINGLE_LINE)&&(bCardCount>=3))
	{
		//设置变量
		ZeroMemory(&OutCardResult,sizeof(OutCardResult));

		//炸弹搜索
		for (BYTE i=2;i<bCardCount;i++)
		{
			//获取数值
			BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

			//炸弹判断
			BYTE bTempLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);
			for (BYTE j=1;j<3;j++)
			{
				if (GetCardLogicValue(bCardData[bCardCount+j-i-1])!=bTempLogicValue) break;
			}
			if (j!=3) continue;

			//完成处理
			OutCardResult.cbCardCount=3;
			OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
			OutCardResult.cbResultCard[1]=bCardData[bCardCount-i];
			OutCardResult.cbResultCard[2]=bCardData[bCardCount-i+1];

			return true;
		}
	}

	

	//搜索小王
	if (bTurnOutType<CT_KING_LITTET)
	{
		//设置变量
		ZeroMemory(&OutCardResult,sizeof(OutCardResult));

		for (BYTE i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x41) 
			{
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=0x41;
			}
		}
		if (OutCardResult.cbCardCount==2) return true;
	}

	//搜索大王
	if (bTurnOutType<CT_KING_BIG)
	{
		//设置变量
		ZeroMemory(&OutCardResult,sizeof(OutCardResult));

		for (BYTE i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x42) 
			{
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=0x42;
			}
		}
		if (OutCardResult.cbCardCount==2) return true;
	}

	//搜索假腿..
	//设置变量
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	for (BYTE i=0;i<bCardCount;i++)
	{
		if (GetCardValue(bCardData[i])==5) OutCardResult.cbResultCard[2]=bCardData[i];
		if (GetCardValue(bCardData[i])==10) OutCardResult.cbResultCard[1]=bCardData[i];
		if (GetCardValue(bCardData[i])==13) OutCardResult.cbResultCard[0]=bCardData[i];
	}
	if ((OutCardResult.cbResultCard[0]!=0)&&(OutCardResult.cbResultCard[1]!=0)&&(OutCardResult.cbResultCard[2]!=0))
	{
		OutCardResult.cbCardCount=3;
		return true;
	}
	//花色变量
	BYTE cbColorOrder=0x00;
	if (bTurnOutType==CT_CRURA_PURE)
	{
		BYTE cbColor=GetCardColor(bTurnCardData[0]);
		if (cbColor==0x00) cbColorOrder=0x03;
		else if (cbColor==0x10) cbColorOrder=0x01;
		else if (cbColor==0x20) cbColorOrder=0x04;
		else if (cbColor==0x30) cbColorOrder=0x02;
	}

	//搜索纯腿
	for (BYTE j=0;j<4;j++)
	{
		//设置变量
		ZeroMemory(&OutCardResult,sizeof(OutCardResult));

		//花色判断
		BYTE cbSearchColor=j<<4;
		BYTE cbSearchOrder=0xFF;
		if (cbColorOrder!=0x00)
		{
			if (cbSearchColor==0x00) cbSearchOrder=0x03;
			else if (cbSearchColor==0x10) cbSearchOrder=0x01;
			else if (cbSearchColor==0x20) cbSearchOrder=0x04;
			else if (cbSearchColor==0x30) cbSearchOrder=0x02;
		}
		if (cbSearchOrder<=cbColorOrder) continue;

		for (BYTE i=0;i<bCardCount;i++)
		{
			if (GetCardColor(bCardData[i])!=cbSearchColor) continue;
			if (GetCardValue(bCardData[i])==5) OutCardResult.cbResultCard[2]=bCardData[i];
			if (GetCardValue(bCardData[i])==10) OutCardResult.cbResultCard[1]=bCardData[i];
			if (GetCardValue(bCardData[i])==13) OutCardResult.cbResultCard[0]=bCardData[i];
		}
		if ((OutCardResult.cbResultCard[0]!=0)&&(OutCardResult.cbResultCard[1]!=0)&&(OutCardResult.cbResultCard[2]!=0))
		{
			OutCardResult.cbCardCount=3;
			return true;
		}
	}

	//设置变量
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	return false;
}

//////////////////////////////////////////////////////////////////////////
