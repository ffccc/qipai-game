#include "StdAfx.h"
#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x01,0x01,0x11,0x11,               //一
	0x02,0x02,0x02,0x02,0x02,               //二
	0x03,0x03,0x03,0x13,0x13,               //三
	0x04,0x04,0x04,0x04,0x04,               //四
	0x05,0x05,0x05,0x15,0x15,               //五
	0x06,0x06,0x06,0x06,0x06,               //六
	0x07,0x07,0x07,0x17,0x17,               //七
	0x08,0x08,0x08,0x08,0x08,               //八
	0x09,0x09,0x09,0x19,0x19,				//九
	0x0A,0x0A,0x0A,0x0A,0x0A,               //十
	
	0x21,0x21,0x21,0x21,0x21,               //孔
	0x22,0x22,0x22,0x22,0x22,               //己

	0x23,0x23,0x23,0x23,0x23,               //上
	0x24,0x24,0x24,0x24,0x24,               //大
	0x25,0x25,0x25,0x25,0x25,               //人

	0x26,0x26,0x26,0x26,0x26,               //土
	0x27,0x27,0x27,0x27,0x27,               //化
	0x28,0x28,0x28,0x28,0x28,               //千

	0x29,0x29,0x29,0x29,0x29,               //可
	0x2A,0x2A,0x2A,0x2A,0x2A,               //知
	0x2B,0x2B,0x2B,0x2B,0x2B,               //礼

	0x2C,0x2C,0x2C,0x2C,0x2C,               //子
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
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{

	//混乱准备
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	srand((unsigned)time(NULL));
	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{

	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//如果颜色是0||1，牌值必须是0--A(10)
	//如果颜色是2，牌值必须是0--C(12)
	return ( ((cbColor == 0 || cbColor == 1) && (cbValue>0 && cbValue<=10) ) || ( (cbColor == 2) && (cbValue>=1 && cbValue<=12)));
}

//获取组合
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[])
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case WIK_DUI:		//对牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_SI_ZHAO:		//四招操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;
			cbCardBuffer[4]=cbCenterCard;

			return 5;
		}
	case WIK_SAN_ZHAO:		//三招操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//////////babala////////////////////////////////////////
//给定一个扑克，根据扑克的值得到扑克牌等级
BYTE CGameLogic::GetCardRank(BYTE cbCardData)
{
	//断言传进来的牌是合法的
	ASSERT(IsValidCard(cbCardData));

	if (IsValidCard(cbCardData) == false) return -1;
	//1.上，可，已，千，返回3
	if (cbCardData == 0x23 || cbCardData == 0x29 || cbCardData == 0x22 || cbCardData == 0x28) return 3;
	//2.孔，人，礼，返回5
	if (cbCardData == 0x21 || cbCardData == 0x25 || cbCardData == 0x2B) return 5;
	//3.大，土，知，子返回 7
	if (cbCardData == 0x24 || cbCardData == 0x26 || cbCardData == 0x2A || cbCardData == 0x2C) return 7;
	//4.如果是数字牌，直接得到牌值
	else 
		return cbCardData&0x0F;

}
//动作等级
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//胡牌等级
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//招牌牌等级
	if (cbUserAction&(WIK_SAN_ZHAO |WIK_SI_ZHAO)) { return 3; }

	//对牌等级
	if (cbUserAction&WIK_DUI) { return 2; }
	return 0;
}
///////////////babala  11_15////////////////////////////////////
void CGameLogic::SaveHuaCard(BYTE cbCardData[],BYTE bCardCount,BYTE cbHuaCardIndex[])
{
	//找花牌
	for (BYTE i=0; i<bCardCount; i++)
	{
		//如果是花牌，保存到花牌索引数组中
		if ((cbCardData[i]&MASK_COLOR) == 0x10)
		{
			BYTE index = SwitchToCardIndex(cbCardData[i]);
			cbHuaCardIndex[index/2]++;
			ASSERT(cbHuaCardIndex[index/2]<=2);
		}
	}
}

/////////////////////babala///////////////////
//判断是否可以对牌,如果索引数组中有两张一样的,返回对操作码
BYTE CGameLogic::EstimateDuiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbSiTong[],BYTE cbSiTongCount,BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	BYTE cbIndex = SwitchToCardIndex(cbCurrentCard);

	//如果给定的牌已经在当前玩家的四藏牌里面,不进行三招分析
	if(cbCardIndex[cbIndex]>=4)
	{
		for (BYTE i=0; i<cbSiTongCount; i++)
		{
			if (cbIndex == cbSiTong[i])
			{
				return WIK_NULL;
			}
		}
	}

	//对牌判断
	return (cbCardIndex[cbIndex]>=2)?WIK_DUI:WIK_NULL;
}

//判断招牌
BYTE CGameLogic::EstimateZhaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbSiTong[],BYTE cbSiTongCount, BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));
	BYTE cbActionMask=WIK_NULL;
	
	BYTE cbIndex = SwitchToCardIndex(cbCurrentCard);
	bool bSanZhao = true;
	//如果给定的牌已经在当前玩家的四藏牌里面,不进行三招分析
	for (BYTE i=0; i<cbSiTongCount; i++)
	{
		if (cbIndex == cbSiTong[i])
		{
			bSanZhao = false;
			break;
		}
	}
	if (bSanZhao ==  false)
	{
		if (cbCardIndex[cbIndex] == 4) cbActionMask|= WIK_SI_ZHAO; 
	}
	else
	{
		if (cbCardIndex[cbIndex] == 3) cbActionMask|= WIK_SAN_ZHAO; 
		if (cbCardIndex[cbIndex] == 4) cbActionMask|= WIK_SI_ZHAO|WIK_SAN_ZHAO; 

	}

	return cbActionMask;
}

//11_17
//根据扑克牌数据得到花牌保存到索引中,返回花牌的个数
BYTE CGameLogic:: GetHuaCard(BYTE cbCardData[],BYTE bCardCount, BYTE cbHuaCardIndex[])
{
	BYTE bHuaCardCount =0;
	//循环从牌数据数组中得到花牌
	for (BYTE i=0; i<bCardCount; i++)
	{
		//参数效验
		ASSERT(IsValidCard(cbCardData[i]));

		//如果是花牌，将花牌转换成索引存放到花牌索引数组中
		if((cbCardData[i]&MASK_COLOR) == 0x10)
		{
			cbHuaCardIndex[SwitchToCardIndex(cbCardData[i])/2]++;

			ASSERT(cbHuaCardIndex[SwitchToCardIndex(cbCardData[i])/2] <= 2);
			bHuaCardCount++;
		}
	}
	return bHuaCardCount;

}
/////////babala 11_16
//分析玩家手中牌数组中是否可以藏牌，四统
BYTE CGameLogic::AnalyseSiTong(BYTE cbCardIndex[MAX_INDEX],  tagTongResult &SiTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&SiTongResult,sizeof(SiTongResult));

	//分析索引数组中是否有四张相同的,如果有得到四统的操作
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i] >= 4) 
		{
			SiTongResult.cbCardData[SiTongResult.cbCardCount++] = SwitchToCardData(i);
			cbActionMask |=WIK_SI_TONG;
		}
	}
	return cbActionMask;
}

//分析玩家手中牌数组中是否可以藏牌，五统
BYTE CGameLogic::AnalyseWuTong(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,tagTongResult &WuTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&WuTongResult,sizeof(WuTongResult));

	//分析索引数组中是否有四张相同的,如果有得到五统的操作
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i] == 5) 
		{
			WuTongResult.cbCardData[WuTongResult.cbCardCount++] = SwitchToCardData(i);
			cbActionMask |=WIK_WU_TONG;
		}
	}

	//组合牌中藏牌分析
	for (BYTE j=0; j<cbItemCount; j++)
	{
		BYTE cbWeaveKind = WeaveItem[j].cbWeaveKind;
		BYTE cbCenterCardIndex = SwitchToCardIndex( WeaveItem[j].cbCenterCard );
		//要求：组合牌中对应的牌操作为三招，并且手中牌对应索引位置的牌数为1
		if ((cbCardIndex[cbCenterCardIndex] == 1) && (cbWeaveKind == WIK_SAN_ZHAO))
		{
			WuTongResult.cbCardData[WuTongResult.cbCardCount++] = SwitchToCardData(cbCenterCardIndex);
			cbActionMask |= WIK_WU_TONG;
		}
	}
	return cbActionMask;

}
/////////babala 11_25
//分析组合牌中的藏牌,如果给定的牌在组合牌中已经有四张，可以提示藏牌
BYTE CGameLogic::AnalyseWeaveTong(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,tagTongResult &WeaveTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&WeaveTongResult,sizeof(WeaveTongResult));

	for (BYTE i=0; i<MAX_INDEX; i++)
	{
		//如果对应的索引数组中的牌数为1，判断组合牌中该牌是否四张,如果是，保存对应位置的索引
		if (cbCardIndex[i] == 1)
		{
			for (BYTE j=0; j<cbItemCount; j++)
			{
				BYTE cbWeaveKind = WeaveItem[j].cbWeaveKind;
				BYTE cbCenterCardIndex = SwitchToCardIndex( WeaveItem[j].cbCenterCard );
				if ((cbCenterCardIndex == i) && (cbWeaveKind == WIK_SAN_ZHAO))
				{
					WeaveTongResult.cbCardData[WeaveTongResult.cbCardCount++] = WeaveItem[j].cbCenterCard;
					cbActionMask |= WIK_WU_TONG;
				}
			}
		}
	}

	return cbActionMask;
}
//给定一个操作类型和操作中心牌索引，得到三张牌的索引,返回操作索引的个数
BYTE CGameLogic::GetWeaveIndex(BYTE cbWeaveKind,BYTE cbCenterCardIndex,BYTE cbCardIndex[])
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case WIK_DUI:		//对牌操作
		{
			//设置变量
			cbCardIndex[0]=cbCenterCardIndex;
			cbCardIndex[1]=cbCenterCardIndex;
			cbCardIndex[2]=cbCenterCardIndex;

			return 3;
		}
	case WIK_QI_TA:   //三连情况
		{
			//0--9之间的数字牌组合
			if (cbCenterCardIndex<(10-2))
			{
				cbCardIndex[0]=cbCenterCardIndex;
				cbCardIndex[1]=cbCenterCardIndex+1;
				cbCardIndex[2]=cbCenterCardIndex+2;
			}
			//孔乙已:孔
			if (cbCenterCardIndex==10)
			{
				cbCardIndex[0]=cbCenterCardIndex;
				cbCardIndex[1]=0;
				cbCardIndex[2]=cbCenterCardIndex+1;
			}
			//上大人:上
			if( cbCenterCardIndex ==12 )
			{
				cbCardIndex[0]=cbCenterCardIndex;
				cbCardIndex[1]=cbCenterCardIndex+1;
				cbCardIndex[2]=cbCenterCardIndex+2;
			}
			//七十土:土
			if (cbCenterCardIndex==15)
			{
				cbCardIndex[0]=6;
				cbCardIndex[1]=9;
				cbCardIndex[2]=cbCenterCardIndex;
			}
			//化三千:化
			if (cbCenterCardIndex==16)
			{
				cbCardIndex[0]=cbCenterCardIndex;
				cbCardIndex[1]=2;
				cbCardIndex[2]=cbCenterCardIndex+1;
			}
			//可知礼
			if (cbCenterCardIndex==18)
			{
				cbCardIndex[0]=(cbCenterCardIndex);
				cbCardIndex[1]=(cbCenterCardIndex+1);
				cbCardIndex[2]=(cbCenterCardIndex+2);
			}
			//八九子
			if (cbCenterCardIndex==21)
			{
				cbCardIndex[0]=(7);
				cbCardIndex[1]=(8);
				cbCardIndex[2]=(cbCenterCardIndex);
			}

			return 3;
		}
	case WIK_SI_ZHAO:		//四招操作
		{
			//设置变量
			cbCardIndex[0]=cbCenterCardIndex;
			cbCardIndex[1]=cbCenterCardIndex;
			cbCardIndex[2]=cbCenterCardIndex;
			cbCardIndex[3]=cbCenterCardIndex;
			cbCardIndex[4]=cbCenterCardIndex;

			return 5;
		}
	case WIK_SAN_ZHAO:		//三招操作
		{
			//设置变量
			cbCardIndex[0]=cbCenterCardIndex;
			cbCardIndex[1]=cbCenterCardIndex;
			cbCardIndex[2]=cbCenterCardIndex;
			cbCardIndex[3]=cbCenterCardIndex;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}
//校正胡点,传进一个牌数据，和一个牌索引
bool CGameLogic::CheckHuPoint(BYTE cbCardData,BYTE cbCardIndex)
{
	//转换扑克数据
	if (cbCardData !=0)
	{
		if (SwitchToCardIndex(cbCardData) == cbCardIndex) return true;
		return false;
	}
	return false;
}

//分析胡牌
BYTE CGameLogic::AnalyseHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,BYTE cbCurrentCard,BYTE HuaCardInex[], BYTE WeavHuaIndex[], tagHuCardResult & ChiHuResult)
{
	CAnalyseItemArray AnalyseItemArray;
	//设置变量
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//定义临时数组保存手中牌索引
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//定义变量保存手中花牌的个数
	BYTE cbTempHuaCardIndex[5];
	BYTE cbTempWeaveHuaIndex[5];
	CopyMemory(cbTempHuaCardIndex,HuaCardInex,sizeof(cbTempHuaCardIndex));
	CopyMemory(cbTempWeaveHuaIndex,WeavHuaIndex,sizeof(cbTempWeaveHuaIndex));

	//将当前牌加入手中牌索引数组中
	if (cbCurrentCard != 0) 
	{
		BYTE CardIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardIndexTemp[CardIndex]++;

		//如果当前牌是花牌，保存到手中花牌索引数组中
		if ((cbCurrentCard & 0xF0) == 0x10)	cbTempHuaCardIndex[CardIndex/2]++;
	}

	//得到手中各经牌的个数
	BYTE cbHandGoldCard[3];
	for (BYTE i=0; i<3; i++)
	{
		cbHandGoldCard[i] = cbCardIndexTemp[(i*2)+2];
	}

//#ifdef _DEBUG
//	m_Debug.PrintCardMessage(cbCardIndex);
//	m_Debug.PrintWeaveItemsMessage(WeaveItem,cbItemCount);
//#endif

	AnalyseCard(cbCardIndexTemp, WeaveItem, cbItemCount, AnalyseItemArray);

	//对胡牌组合进行分析，得出最佳的胡牌组合
	if (AnalyseItemArray.GetCount()>0)
	{
		//用动态数组
		tagAnalyseItem BestAnalyseItem;
		ZeroMemory(&BestAnalyseItem,sizeof(BestAnalyseItem));

		//逐一分析每一合法胡牌的组合
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//对某一种组合进行分析
			tagAnalyseItem *pAnalyseItem=&AnalyseItemArray[i];

			BYTE cbMaxPoint = 0;
			BYTE cbMaxPointGoldCard = 255;
			//对每一种组合分别讨论以1,3,5,7,9作为主金得出胡数最大的情况
			for (BYTE j=0; j<3; j++)
			{
				//得到主经的索引值 
				BYTE RealGold = (j*2)+2;   //当前的假设主精
				WORD WeavePoint=0;     //用户组合牌的总点数
				WORD HandPoint=0;      //用户手中的总点数
				
				//每次重新保存手中经牌和花牌的个数，
				BYTE cbGoldCard[3];      //用户手中牌的各精牌个数
				BYTE cbHuaCardIndex[5];  //用户手中牌的各花牌个数
				CopyMemory(cbGoldCard,cbHandGoldCard,sizeof(cbGoldCard));
				CopyMemory(cbHuaCardIndex,cbTempHuaCardIndex,sizeof(cbHuaCardIndex));

				//1 分析用户组合牌中的分数
				//////////////////////下面用到的花牌索引应该是组合牌中的花牌///////////////////
				BYTE cbItemPoint;   //测试时用，用于记录每一组成的点数
				ZeroMemory(pAnalyseItem->cbKindPoint,sizeof(pAnalyseItem->cbKindPoint));
				for(BYTE m=0;m<cbItemCount;m++)
				{
					//得到组合牌的中心牌索引
					cbItemPoint=0;
					BYTE index = pAnalyseItem->cbCenterCard[m];
					//四招情况:1.是金牌,2.普通牌
					if (pAnalyseItem->cbWeaveKind[m]== WIK_SI_ZHAO)
					{
						//1.是经牌1,3,5,7,9,分为:1.1主经,1.2非主经
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							if(index == 0 || index == 8)
							{
									cbItemPoint = 6;
									WeavePoint += 6;
							}
							else
							{
								//1.是主金牌+56
								if (index == RealGold) 
								{
									cbItemPoint = 56;
									WeavePoint += 56;
								}
								//不是主金+28
								else  
								{
									cbItemPoint = 28;
									WeavePoint += 28;
								}
							}
						}
						//2.不是经牌的普通牌分:2.1红牌,2.2黑牌
						else
						{
							//2.1红牌,只有上,大,人,可,知,礼六种情况+8
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								WeavePoint += 8;
								cbItemPoint = 8;
							}
							//2.2黑牌+4
							else
							{
								WeavePoint += 4;
								cbItemPoint = 4;
							}
						}
					}
					//三招情况:1.是金牌,2.普通牌
					else if (pAnalyseItem->cbWeaveKind[m] == WIK_SAN_ZHAO)
					{
						//1,3,5,7,9的情况
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							if(index == 0 || index == 8)
							{
								switch(cbTempWeaveHuaIndex[index/2])
								{
								case 1:   //一张花牌的情况
									{
										cbItemPoint = 3;
										WeavePoint += 3;
										break;
									}
								case 2:		//二张花牌的情况
									{
										cbItemPoint = 4;
										WeavePoint += 4;
										break;
									}
								}
							}
							else
							{
								//1.是主金牌根据花牌个数进行算分
								if (index == RealGold)
								{
									//根据花牌个数进行计算
									switch(cbTempWeaveHuaIndex[index/2])
									{
									case 1:    //一张花牌的情况
										{
											cbItemPoint = 24;
											WeavePoint += 24;
											break;
										}
									case 2:      //两张花牌情况
										{
											cbItemPoint = 28;
											WeavePoint += 28;
											break;
										}
									}
								}					
					
								//非主金的牌,根据花牌的个数进行算分
								else
								{
									//根据花牌个数进行计算
									switch(cbTempWeaveHuaIndex[index/2])
									{
									case 1:    //一张花牌的情况
										{
											cbItemPoint = 12;
											WeavePoint += 12;
											break;
										}
									case 2:      //两张花牌情况
										{
											cbItemPoint = 14;
											WeavePoint += 14;
											break;
										}
									}
								}
							}
						}
						//普通牌2.1红牌算4胡,2.2黑牌算2胡
						else
						{
							//2.1红牌,只有上,大,人,可,知,礼六种情况+4
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								WeavePoint += 4;
								cbItemPoint = 4;
							}
							//2.2黑牌+2
							else 
							{
								WeavePoint += 2;
								cbItemPoint = 2;
							}
						}
					}
					//对牌情况:1.是金牌,2.普通牌
					else if (pAnalyseItem->cbWeaveKind[m] == WIK_DUI)
					{
						//1,3,5,7,9的情况
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							if(index == 0 || index == 8)
							{
								//根据花牌个数进行计算
								switch(cbTempWeaveHuaIndex[index/2])
								{
								case 0:    //0张花牌的情况
									{
										cbItemPoint = 0;
										WeavePoint += 0;
										
										break;
									}
								case 1:      //1张花牌情况
									{
										cbItemPoint = 1;
										WeavePoint += 1;
										break;
									}
								case 2:      //2张花牌情况
									{
										cbItemPoint = 2;
										WeavePoint += 2;
										break;
									}
								}

							}
							else
							{
								//1.是主金牌根据花牌个数进行算分
								if (index == RealGold)
								{
									//根据花牌个数进行计算
									switch(cbTempWeaveHuaIndex[index/2])
									{
									case 0:    //0张花牌的情况
										{
											cbItemPoint = 6;
											WeavePoint += 6;
											break;
										}
									case 1:      //1张花牌情况
										{
											cbItemPoint = 8;
											WeavePoint += 8;
											break;
										}
									case 2:      //2张花牌情况
										{
											cbItemPoint = 10;
											WeavePoint += 10;
											break;
										}
									}
								}
								//////////////非主金的情况下算胡///////////////////
								//2.非主金的牌
								else
								{
									//根据花牌个数进行计算
									switch(cbTempWeaveHuaIndex[index/2])
									{
									case 0:    //0张花牌的情况
										{
											cbItemPoint = 3;
											WeavePoint += 3;
											
											break;
										}
									case 1:      //1张花牌情况
										{
											cbItemPoint = 4;
											WeavePoint += 4;
											break;
										}
									case 2:      //2张花牌情况
										{
											cbItemPoint = 5;
											WeavePoint += 5;
											break;
										}
									}
								}
							}
						}
						//普通牌2.1红牌算1胡,2.2黑牌不算胡
						else
						{
							//2.1红牌,只有上,大,人,可,知,礼六种情况+1
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								WeavePoint += 1;
								cbItemPoint = 1;
							}
							//2.2黑牌+0
							else 
							{
								WeavePoint += 0;
								cbItemPoint = 0;
							}
						}
					}
					
					pAnalyseItem->cbKindPoint[m] = cbItemPoint;
				}

				//2 计算用户手中坎牌的所有点数
				for(BYTE k=cbItemCount; k<8;k++)
				{
					cbItemPoint=0;
					//取每种组合里面的每小组的中心牌索引
					BYTE index = pAnalyseItem->cbCenterCard[k];
			
					//五藏情况:1.1是金牌,1.2.普通牌
					if (pAnalyseItem->cbWeaveKind[k] == WIK_WU_TONG)
					{
						//1.是金牌1,3,5,7,9,分为:1.1主金,1.2非主金
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							cbHuaCardIndex[index/2] = 0;  //手中此花牌个数为0
							
							if(index == 0 || index == 8)
							{
								HandPoint += 6;
								cbItemPoint = 6;
							}
							else
							{
								cbGoldCard[(index-2)/2] -= 5;     //手中此精牌个数为0
							
								//1.1主金
								if (index == RealGold)
								{
									//对应位置的花牌数减为0
									HandPoint += 56;
									cbItemPoint = 56;
								}
								//1.2非主金的金牌
								else
								{
									//对应位置的花牌数减为0
									HandPoint += 28;
									cbItemPoint = 28;
								}
							}
						}
						//2.普通牌(手中牌):红牌算8胡，黑牌算4胡
						else
						{
							//2.1红牌,只有上,大,人,可,知,礼六种情况+8
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								HandPoint += 8;
								cbItemPoint = 8;
							}
							//2.2黑牌+4
							else 
							{
								HandPoint += 4;
								cbItemPoint = 4;							
							}
						}
					}
					//四藏情况:1.是金牌,2.普通牌
					else if (pAnalyseItem->cbWeaveKind[k] == WIK_SI_TONG)
					{
						//1.是金牌1,3,5,7,9的情况
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
						
							if(index == 0 || index == 8)
							{
									//根据花牌个数进行计算
								switch(cbHuaCardIndex[index/2])
								{
								case 1:    //一张花牌的情况
									{
										cbItemPoint = 3;
										HandPoint += 3;
										cbHuaCardIndex[index/2] -= 1;

										break;
									}
								case 2:      //两张花牌情况
									{
										cbItemPoint = 4;
										HandPoint += 4;
										cbHuaCardIndex[index/2] -= 2;
										break;
									}
								}
							}					
							else
							{
								cbGoldCard[(index-2)/2] -= 4;
								//1.是主金牌,根据花牌个数进行算分
								if (index == RealGold)
								{
									//根据花牌个数进行计算
									switch(cbHuaCardIndex[index/2])
									{
									case 1:    //一张花牌的情况
										{
											cbItemPoint = 24;
											HandPoint += 24;
											cbHuaCardIndex[index/2] -= 1;

											break;
										}
									case 2:      //两张花牌情况
										{
											cbItemPoint = 28;
											HandPoint += 28;
											cbHuaCardIndex[index/2] -= 2;
											break;
										}
									}
								}
								//非主金的牌
								else
								{
									//根据花牌个数进行计算
									switch(cbHuaCardIndex[index/2])
									{
									case 1:    //一张花牌的情况
										{
											cbItemPoint = 12;
											HandPoint += 12;
											cbHuaCardIndex[index/2] -= 1;

											break;
										}
									case 2:      //两张花牌情况
										{
											cbItemPoint = 14;
											HandPoint += 14;
											cbHuaCardIndex[index/2] -= 2;
											break;
										}
									}

								}
							}
						}
						//普通牌2.1红牌算4胡,2.2黑牌算2胡
						else
						{
							//2.1红牌,只有上,大,人,可,知,礼六种情况+4
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								cbItemPoint = 4;
								HandPoint += 4;
							}
							//2.2黑牌+2
							else
							{
								cbItemPoint = 2;
								HandPoint += 2;
							}
						}
					}
					//三张相同的：1.是金牌,2.普通牌
					else if (pAnalyseItem->cbWeaveKind[k] == WIK_DUI)
					{
						//是金牌:1,3,5,7,9的情况
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							if(index == 0 || index == 8)
							{
								//根据花牌个数进行计算
								switch(cbHuaCardIndex[index/2])
								{
								case 0:    //0张花牌的情况
									{
										cbItemPoint = 1;
										HandPoint += 1;
										cbHuaCardIndex[index/2] -= 0;

										break;
									}
								case 1:      //1张花牌情况
									{
										cbItemPoint = 2;
										HandPoint += 2;
										cbHuaCardIndex[index/2] -= 1;
										break;
									}
								case 2:      //2张花牌情况
									{
										cbItemPoint = 3;
										HandPoint += 3;
										cbHuaCardIndex[index/2] -= 2;
										break;
									}
								}
							}
							else
							{
								cbGoldCard[(index-2)/2] -= 3;
								//1.是主金牌,根据花牌个数进行算分
								if (index == RealGold)
								{
									//根据花牌个数进行计算
									switch(cbHuaCardIndex[index/2])
									{
									case 0:    //0张花牌的情况
										{
											cbItemPoint = 10;
											HandPoint += 10;
											cbHuaCardIndex[index/2] -= 0;

											break;
										}
									case 1:      //1张花牌情况
										{
											cbItemPoint = 12;
											HandPoint += 12;
											cbHuaCardIndex[index/2] -= 1;
											break;
										}
									case 2:      //2张花牌情况
										{
											cbItemPoint = 14;
											HandPoint += 14;
											cbHuaCardIndex[index/2] -= 2;
											break;
										}
									}
								}
								//1.2非主金的花牌
								else
								{
									//根据花牌个数进行计算
									switch(cbHuaCardIndex[index/2])
									{
									case 0:    //0张花牌的情况
										{
											cbItemPoint = 5;
											HandPoint += 5;
											cbHuaCardIndex[index/2] -= 0;

											break;
										}
									case 1:      //1张花牌情况
										{
											cbItemPoint = 6;
											HandPoint += 6;
											cbHuaCardIndex[index/2] -= 1;
											break;
										}
									case 2:      //2张花牌情况
										{
											cbItemPoint = 7;
											HandPoint += 7;
											cbHuaCardIndex[index/2] -= 2;
											break;
										}
									}
								}
							}
						}
						//2.普通牌
						else
						{
							//2.1红牌,只有上,大,人,可,知,礼六种情况+2
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 )
							{
								HandPoint += 2;
								cbItemPoint = 2;
							}
							//2.2黑牌+1
							else 
							{
								HandPoint += 1;
								cbItemPoint = 1;
							}
						}
					}

					//句子判断,只统计上大人，可知礼的个数和剩下的经牌个数
					else if (pAnalyseItem->cbWeaveKind[k] == WIK_QI_TA)
					{
						//上大人/可知礼组合 +1
						if (index == 12 || index == 18) 
						{
							HandPoint += 1;
							cbItemPoint = 1;
						}
					}
					pAnalyseItem->cbKindPoint[k] = cbItemPoint;
				}
				
				//3 数剩下的金牌的个数
				for (BYTE t=0; t<3; t++)
				{
					//计算花牌的点数
					if (cbHuaCardIndex[t+1] > 0)
					{
						//除去经牌中花牌的个数
						cbGoldCard[t] -= cbHuaCardIndex[t+1];
						//主金情况
						if (((t*2)+2) == RealGold)
						{
							HandPoint += cbHuaCardIndex[t+1]*4;
						}
						else
						{
							HandPoint += cbHuaCardIndex[t+1]*2;
						}
					}
					//计算白皮经牌的点数
					if (cbGoldCard[t] > 0)
					{
						//主金情况白皮的*2
						if (((t*2)+2) == RealGold)
						{
							HandPoint += cbGoldCard[t]*2;
						}
						//非主金,白皮的*1
						else
						{
							HandPoint += cbGoldCard[t]*1;
						}
					}
				}

				//4 数剩下的花牌的个数
				for (BYTE t=0; t<2; t++)
				{
					if(cbHuaCardIndex[t*4] > 0)
					{
						HandPoint += cbHuaCardIndex[t*4]*1;
					}
				}
				
//#ifdef _DEBUG
//				m_Debug.PrintValidKindItemPointBeforeMessage(pAnalyseItem,WeavePoint,HandPoint);
//#endif

				//校正手中牌的胡点:如果胡牌组合中，别人打出的一张牌与自己手中牌形成三张一样的，只能将其当作倒下的牌算点数
				//将手中牌每一种组合进行转换，判断所供牌是否在其中
				if (cbCurrentCard != 0) 
				{
					//得到所供的牌
					BYTE cbCurrentCardIndex = SwitchToCardIndex(cbCurrentCard);
					//首先是丫口判断,如果所供的牌在丫口中，不进行校正
					if ((cbCurrentCardIndex != pAnalyseItem->cbCardEye[0]) && (cbCurrentCardIndex != pAnalyseItem->cbCardEye[1]))
					{
						bool Qi_Ta = false;
						//分析别人出的牌是否在句牌中
						for (BYTE l=cbItemCount; l<8; l++)
						{
							//得到WIK_QI_TA组合类型的所有索引，判断是否有所供的牌
							if (pAnalyseItem->cbWeaveKind[l] == WIK_QI_TA)
							{
								BYTE cbWeaveIndex[5]={-1,-1,-1,-1,-1};
								BYTE cbWeaveCount = GetWeaveIndex(pAnalyseItem->cbWeaveKind[l],pAnalyseItem->cbCenterCard[l],cbWeaveIndex);
								//判断所供的牌是否在WIK_QI_TA类型中，如果在，直接跳出不用校正
								for (BYTE n=0; n<cbWeaveCount; n++)
								{
									if (cbCurrentCardIndex == cbWeaveIndex[n]) 
									{
										Qi_Ta = true;
										break;
									}
								}

								//如果在句牌中找到了别人所打的牌跳出循环
								if (Qi_Ta == true) break;
							}
							//得到WIK_SI_TONG组合类型中的所有索引，判断所供的牌是否是在其中，如果是，不能胡牌
							else if (pAnalyseItem->cbWeaveKind[l] == WIK_SI_TONG || pAnalyseItem->cbWeaveKind[l] == WIK_WU_TONG)
							{
								if (cbCurrentCardIndex ==  pAnalyseItem->cbCenterCard[i]) 
								{
									//AfxMessageBox("对不起！最佳胡牌组合分析得出，别人打出的牌形成四张的不能胡牌!");
									return WIK_NULL;	
								}	
							}

						}

						//如果在句牌中没有找到别人打的牌，再到对牌中找
						if (Qi_Ta == false)
						{
							for (BYTE i=cbItemCount; i<8; i++)
							{
								//WIK_DUI
								if (pAnalyseItem->cbWeaveKind[i] == WIK_DUI)
								{
									//如果手中对牌与所供牌相同，进行胡点校正
									if (cbCurrentCardIndex ==  pAnalyseItem->cbCenterCard[i]) 
									{
										//如果所供牌是金牌分：1.主金，2.普通金牌
										if (cbCurrentCardIndex == 2 || cbCurrentCardIndex == 4 || cbCurrentCardIndex == 6)
										{
											//1.主金减4胡
											if (cbCurrentCardIndex == RealGold) 
											{
												HandPoint -= 4;  //HuPoint-=4;
												pAnalyseItem->cbKindPoint[i] -=4;
											}
											//2.普通金牌减2胡
											else
											{
												HandPoint -= 2;  //HuPoint -=2;
												pAnalyseItem->cbKindPoint[i] -=2;
											}
										}
										//普通牌分,总胡数减1
										else
										{
											HandPoint -= 1;
											pAnalyseItem->cbKindPoint[i] -=1;
											//HuPoint -=1;
										}
										//处理完成后，结束循环
										break;
									}
								}

							}
						}
					}
				}
				
				//以该经牌做为主经是否大于前面的点数
				//保存本组合中最大的点数及其对应的主精牌
				if((WeavePoint+HandPoint)>cbMaxPoint)
				{
					cbMaxPoint=WeavePoint+HandPoint;
					cbMaxPointGoldCard=RealGold;
				}
				//先设置本循环中计算的点数及主精
				pAnalyseItem->cbPoint = WeavePoint+HandPoint;
				pAnalyseItem->cbGoldCard = RealGold;
#ifdef _DEBUG
		        m_Debug.PrintValidKindItemPointAfterMessage(pAnalyseItem,(BYTE)WeavePoint,(BYTE)HandPoint);
#endif
			}
			//修正：设置本组合中真正最大的点数及其对应的主精牌
			pAnalyseItem->cbPoint = cbMaxPoint;
			pAnalyseItem->cbGoldCard = cbMaxPointGoldCard;
		}

		//在此加上一项特殊的胡牌判断--7点胡牌
//分析胡牌是否是7点特殊胡牌
//1、就是用户手中所有的牌里,没有357的金,没有花乙和花九;
//2、就是用户桌面上的组合牌里，要对上三对黑色字的牌，这三对黑色字的牌算一个胡
//3、用户手中有三张及以上的组合牌,或桌面的组合牌中有四张及以上的招牌,要有一个红色字牌的一坎,比如三个上
//4、并且最后胡牌是手里刚好是7个胡.

		for (INT_PTR t=0;t<AnalyseItemArray.GetCount();t++)
		{
			tagAnalyseItem *pAnalyseItem=&AnalyseItemArray[t];

//if(pAnalyseItem->cbPoint<10)
//{
//	CString str;
//	str.Format("现在的点数为:%d",pAnalyseItem->cbPoint);
//	AfxMessageBox(str);
//}

			if(pAnalyseItem->cbPoint+1 == 7)
			{
				bool bHaveHuGold=true;    //用户整个牌中，是否没有357的金,没有花乙和花九,默认是有
				BYTE cbBlackDuiCount=0;   //用户桌面的组合牌中,是否有三对以上的黑对牌，默认为0
				BYTE cbReadDuiCount=0;     //用户手中三张相同的红牌或桌面上招的红牌

				//1、就是手里没有357的金，没有花乙和花九
				BYTE cbUserHuaCardIndex[5];          //保存用户牌中花精的个数
				BYTE cbUserGoldCardIndex[5];         //保存用户牌中精牌的个数

				ZeroMemory(cbUserHuaCardIndex,sizeof(cbUserHuaCardIndex));
				ZeroMemory(cbUserGoldCardIndex,sizeof(cbUserGoldCardIndex));

				//将用户手中牌中的数据统计到数组中
				for(BYTE i=0;i<5;i++)
				{
					cbUserGoldCardIndex[i]=cbCardIndexTemp[i*2];
					cbUserHuaCardIndex[i]=cbTempHuaCardIndex[i];
				}

				//将用户组合牌中的数据统计到数组中
				for(BYTE i=0;i<cbItemCount;i++)
				{
					BYTE index = pAnalyseItem->cbCenterCard[i];
					if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
					{
						if (pAnalyseItem->cbWeaveKind[i]== WIK_SI_ZHAO)
						{
							cbUserGoldCardIndex[i]+=4;
							cbUserHuaCardIndex[i]+=cbTempWeaveHuaIndex[i];
						}
						else if (pAnalyseItem->cbWeaveKind[i] == WIK_SAN_ZHAO)
						{
							cbUserGoldCardIndex[i]+=5;
							cbUserHuaCardIndex[i]+=cbTempWeaveHuaIndex[i];
						}
						else if(pAnalyseItem->cbWeaveKind[i] == WIK_DUI)
						{
							cbUserGoldCardIndex[i]+=3;
							cbUserHuaCardIndex[i]+=cbTempWeaveHuaIndex[i];
						}
					}
				}

				//用户没有357牌
				if(cbUserGoldCardIndex[1]==0 && cbUserGoldCardIndex[2]==0 && cbUserGoldCardIndex[3]==0)
				{
					//用户没有花1和花9
					if(cbUserHuaCardIndex[0]==0 && cbUserHuaCardIndex[4]==0)
					{
						bHaveHuGold=false;   //满足第一项:没有357的金,没有花乙和花九
					}
				}

				//统计用户组合牌中黑色的对牌数--用户桌面上的对牌中,黑对牌的数目
				for(BYTE i=0;i<cbItemCount;i++)
				{
					if(pAnalyseItem->cbWeaveKind[i] == WIK_DUI)
					{
						BYTE index = pAnalyseItem->cbCenterCard[i];
						//红色的只有上大人[12,13,14]，可知礼[18,19,20]，三五七[2,4,6]
						if(index!=12 && index!=13 && index!=14 && index!=18 && index!=19 && index!=20 && index!=2 && index!=4 && index!=6)
						{
							cbBlackDuiCount++;  //满足第二项：用户桌面的对牌中,黑队牌的数目
						}
					}
				}

				//统计用户手中的对、藏型中的红组合数目,及桌面上红招组合的数目
				//用户桌面中的红三招或红四招数目
				for(BYTE i=0;i<cbItemCount;i++)
				{
					BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[i];
					if(cbWeaveKind==WIK_SAN_ZHAO || cbWeaveKind==WIK_SI_ZHAO)
					{
						BYTE index = pAnalyseItem->cbCenterCard[i];
						//红色的只有上大人[12,13,14]，可知礼[18,19,20]，三五七[2,4,6]
						if(index==12 || index==13 || index==14 || index==18 || index==19 || index==20 || index==2 || index==4 || index==6)
						{
							cbReadDuiCount++;
						}
					}
				}
				//用户手中红对、红四藏、红五藏的数目
				for(BYTE i=cbItemCount;i<9;i++)
				{
					BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[i];
					if(cbWeaveKind==WIK_DUI || cbWeaveKind==WIK_SI_TONG || cbWeaveKind==WIK_WU_TONG)
					{
						BYTE index = pAnalyseItem->cbCenterCard[i];
						//红色的只有上大人[12,13,14]，可知礼[18,19,20]，三五七[2,4,6]
						if(index==12 || index==13 || index==14 || index==18 || index==19 || index==20 || index==2 || index==4 || index==6)
						{
							cbReadDuiCount++;
						}
					}
				}

				//当满足四条规则时，应该将其6点数修改为32点
				if(bHaveHuGold==false && cbBlackDuiCount>=3 && cbReadDuiCount>=1)
				{
					pAnalyseItem->cbPoint=32;
				}
			}
		}
			
		//从所有的胡牌组合中找出胡点最大的组合
		INT_PTR BestIndex = 0;
		//对每一个胡牌组合进行比较，找出最佳胡牌组合
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			if (AnalyseItemArray[i].cbPoint > AnalyseItemArray[BestIndex].cbPoint)
			{
				BestIndex = i;
			}
		}

		BestAnalyseItem.cbPoint = AnalyseItemArray[BestIndex].cbPoint;
		BestAnalyseItem.cbGoldCard = AnalyseItemArray[BestIndex].cbGoldCard;
		//最好保存该种情况下的胡牌组合,以便别人算分
		CopyMemory(&BestAnalyseItem ,&AnalyseItemArray[BestIndex],sizeof(tagAnalyseItem));

		if (BestAnalyseItem.cbPoint < 17)
		{
			return WIK_NULL;
		}

		ChiHuResult.IsHu=true;
		ChiHuResult.HuScore= 2 + ((BestAnalyseItem.cbPoint-17)/5)*2;
		ChiHuResult.bHuPoint = BestAnalyseItem.cbPoint;  //胡牌点数
		ChiHuResult.bRealGold = BestAnalyseItem.cbGoldCard;
		CopyMemory(&ChiHuResult.AnalyseItem, &BestAnalyseItem,sizeof(BestAnalyseItem));

#ifdef _DEBUG
		m_Debug.PrintHuCardMessage(&BestAnalyseItem);
#endif
		return WIK_CHI_HU;
	}
	return WIK_NULL;
}

//扑克转换成牌数据
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	//ASSERT(cbCardIndex<34);
	//return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	ASSERT((cbCardIndex >=0) && (cbCardIndex<22));

	//BYTE CardData;
	if((cbCardIndex >= 0)&&(cbCardIndex<10))
	{
		//CardData=((cbCardIndex/10)<<4)|((cbCardIndex%10)+1);
		return 0x00|(cbCardIndex+1);
	}
	else
	{
		return 0x20|(cbCardIndex-9);

		//CardData=(2<<4)|((cbCardIndex%12)+1);
	}
	//return CardData;
}


/////////////////////////////babala 11_12
//扑克数据转换成扑克点数转换的结果：
//  万，  条， 筒
// 0-8   9-15  16-23
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{

	ASSERT(IsValidCard(cbCardData));

	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//如果是数字，直接得到最后一位值
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}

/////////////////////////////////////////babala 11_12

////扑克转换,
//扑克转换,
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT], BYTE cbHuaCardIndex[])
{
	//转换扑克
	BYTE cbPosition=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			ASSERT(cbPosition<MAX_COUNT);
			//如果是经牌，特殊处理
			if (i==0 || i==2 || i==4 || i==6 || i==8 )
			{
				//将花牌数组中花牌个数保存到牌数据中
				for (BYTE k=0; k<cbHuaCardIndex[i/2]; k++)
				{
					cbCardData[cbPosition++]=0x10|(i+1);
				}
				//将剩下的画白皮的
				for (BYTE j=0; j<(cbCardIndex[i]-cbHuaCardIndex[i/2]); j++)
				{
					cbCardData[cbPosition++]=0x00|(i+1);
				}
			}
			//普通牌处理
			else
			{
				for (BYTE j=0;j<cbCardIndex[i];j++)
				{
					cbCardData[cbPosition++]=SwitchToCardData(i);
				}
			}
		}
	}

	return cbPosition;
}


//BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT], BYTE cbHuaCardIndex[])
//{
//	//BYTE cbTempIndex[5];
//	///CopyMemory(cbTempIndex,cbHuaCardIndex,sizeof(cbHuaCardIndex));
//	//转换扑克
//	BYTE cbPosition=0;
//	for (BYTE i=0;i<MAX_INDEX;i++)
//	{
//		if (cbCardIndex[i]!=0)
//		{
//			for (BYTE j=0;j<cbCardIndex[i];j++)
//			{
//				ASSERT(cbPosition<MAX_COUNT);
//				//如果是经牌，特殊处理
//				if (i==0 || i==2 || i==4 || i==6 || i==8 )
//				{
//					//将花牌数组中花牌个数保存到牌数据中
//					for (BYTE k=0; k<cbHuaCardIndex[i/2]; k++)
//					{
//						cbCardData[cbPosition++]=0x10|(i+1);
//					}
//					//将剩下的画白皮的
//					for (k=0; k<(cbCardIndex[i]-cbHuaCardIndex[i/2]); k++)
//					{
//						cbCardData[cbPosition++]=0x00|(i+1);
//					}
//					break;
//				}
//				//普通牌处理
//				else
//				{
//					cbCardData[cbPosition++]=SwitchToCardData(i);
//				}
//			}
//		}
//	}
//
//	return cbPosition;
//}
//
//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//设置变量
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//转换扑克
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));

		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

///////babala 11_16wna
//babala 11-30
//求绝对值
BYTE  CGameLogic::Abs(BYTE cbFirst,BYTE cbSecond)
{
	return (cbFirst > cbSecond)?(cbFirst-cbSecond):(cbSecond-cbFirst);

}
//判断加入了待分析的牌后是否构成丫口
bool CGameLogic:: IsYaKou(BYTE cbCardIndex[],BYTE &FirstIndex,BYTE &SecondeIndex)
{
	BYTE cbCardIndexTemp[MAX_INDEX];
	ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

	for (BYTE i=0; i<MAX_INDEX;i++)
	{
		cbCardIndexTemp[i]= cbCardIndex[i];
	}
	//CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));


	//统计数组中的个数要为2
	WORD cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndexTemp[i];

	CString sz;
	sz.Format("剩下的牌个数为:%d",cbCardCount);
	//AfxMessageBox(sz);

	if (cbCardCount != 2)	return false;
	

	//ASSERT(cbCardCount == 2);
	//if (cbCardCount != 2) return false;

//	BYTE FirstIndex = -1,SecondeIndex = -1;

	//丫口为半句话
	for (BYTE i=0;i<MAX_INDEX;i++)
	{			
		if (cbCardIndexTemp[i] != 0)
		{
			FirstIndex = i;
			cbCardIndexTemp[i]--;
			break;
		}
	}
	//找下一张牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{			
		if (cbCardIndexTemp[i] != 0)
		{
			SecondeIndex = i;
			cbCardIndexTemp[i]--;
			break;
		}
	}

	//CString sz;
	sz.Format("第一个索引为：%d,第二个索引为:%d",FirstIndex,SecondeIndex);
	//AfxMessageBox(sz);

	//判断是不是丫口
	//1.如果两张牌都是数字的情况下，相减绝对值必须<=2  单独考虑七，十，
	if (FirstIndex < 10 && SecondeIndex <10)
	{
		//特例:七，十
		if ((FirstIndex == 6 && SecondeIndex == 9) || (FirstIndex == 9 && SecondeIndex == 6 ) ) return true;

		//其他情况按数字组合算,相减绝对值必须<=2          
		if( Abs(FirstIndex,SecondeIndex) <= 2 ) return true;
	}
	else
	{
		//2.有数字和字的情况下
		//孔,乙，己
		if ((FirstIndex == 10) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;
		if ((FirstIndex == 0) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;
		if ((FirstIndex == 11) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;

		//上大人
		if ((FirstIndex == 12) && (SecondeIndex == 12 || SecondeIndex == 13 || SecondeIndex == 14 )) return true;
		if ((FirstIndex == 13) && (SecondeIndex == 12 || SecondeIndex == 13 || SecondeIndex == 14 )) return true;
		if ((FirstIndex == 14) && (SecondeIndex == 12 || SecondeIndex == 13 || SecondeIndex == 14 )) return true;


		//化,三,千
		if ((FirstIndex == 16) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;
		if ((FirstIndex == 2) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;
		if ((FirstIndex == 17) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;

		//七十土
		if ((FirstIndex == 6) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;
		if ((FirstIndex == 9) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;
		if ((FirstIndex == 15) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;

		//八九子
		if ((FirstIndex == 7) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;
		if ((FirstIndex == 8) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;
		if ((FirstIndex == 21) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;

		//可知礼
		if ((FirstIndex == 18) && (SecondeIndex == 18 || SecondeIndex == 19 || SecondeIndex == 20 )) return true;
		if ((FirstIndex == 19) && (SecondeIndex == 18 || SecondeIndex == 19 || SecondeIndex == 20 )) return true;
		if ((FirstIndex == 20) && (SecondeIndex == 18 || SecondeIndex == 19 || SecondeIndex == 20 )) return true;
	}

	return false;
}

//分析用户手中的牌，得到可能胡牌的组合信息//用户手中牌的索引数组 //组合牌的数组                 //组合个数             //胡牌组合数组
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//统计索引数组中的所有牌数
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	//效验数目，索引数组中牌的总数在最特殊的情况下，都会有一对丫口
	ASSERT(cbCardCount>=2);
	if (cbCardCount<2) return false;

	//变量定义
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//需求判断--用户手中胡牌的正确组合数
	BYTE cbLessKindItem=8-cbWeaveCount;

	//单吊判断，cbCardCount=2的情况：原来手中牌只有一张牌，加入要分析的牌后正好构成两张，其他的牌都在组合牌中
	if (cbCardCount == 2)
	{
		BYTE FirstIndex = -1,SecondeIndex = -1;
		//如果剩下的两张是丫口，保存所有组合牌
		if (IsYaKou(cbCardIndex,FirstIndex,SecondeIndex) == true)
		{
			tagAnalyseItem AnalyseItem;
			ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

			//分析每一组组合牌，得到组合牌的组合牌型和中间牌索引,保存到分析子项中
			for (BYTE j=0;j<cbWeaveCount;j++)
			{
				AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
				AnalyseItem.cbCenterCard[j]=SwitchToCardIndex(WeaveItem[j].cbCenterCard);
			}
			//保存丫口
			AnalyseItem.cbCardEye[0]=FirstIndex;
			AnalyseItem.cbCardEye[1]=SecondeIndex;
			//将分析结果插入到分析数组中
			AnalyseItemArray.Add(AnalyseItem);

			return true;
		}
		return false;
	}

	//加入待分析的牌后，手中牌>=3的情况，对手中牌索引数组进行分析
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//三个一样的牌
			if(cbCardIndex[i] >= 3)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount].cbCardCount = 3;

				KindItem[cbKindItemCount++].cbWeaveKind=WIK_DUI;
			}
			//四个一样
			if(cbCardIndex[i] >= 4)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount].cbCardIndex[3]=i;
				KindItem[cbKindItemCount].cbCardCount = 4;

				KindItem[cbKindItemCount++].cbWeaveKind=WIK_SI_TONG;
			}
			//五个一样
			if(cbCardIndex[i]==5)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount].cbCardIndex[3]=i;
				KindItem[cbKindItemCount].cbCardIndex[4]=i;
				KindItem[cbKindItemCount].cbCardCount = 5;

				KindItem[cbKindItemCount++].cbWeaveKind=WIK_WU_TONG;
			}
					
			//2.连牌判断
			//0--9之间的排序
			if ((i<(10-2))&&(cbCardIndex[i]>0))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount].cbCardCount = 3;

						KindItem[cbKindItemCount++].cbWeaveKind=WIK_QI_TA;
					}
				}
			}
			//11-21
			//孔乙已
			else if((i==10)&&(cbCardIndex[i]>0))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if(cbCardIndex[0]>=j && cbCardIndex[11]>=j)
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=0;
						KindItem[cbKindItemCount].cbCardIndex[2]=11;
						KindItem[cbKindItemCount].cbCardCount = 3;

						KindItem[cbKindItemCount++].cbWeaveKind=WIK_QI_TA;
					}
				}
			}
			//上大人
			else if((i==12)&&(cbCardIndex[i]>0))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if(cbCardIndex[13]>=j && cbCardIndex[14]>=j)
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount].cbCardCount = 3;

						KindItem[cbKindItemCount++].cbWeaveKind=WIK_QI_TA;
					}
				}
			}
			//七十土
			else if((i==15)&&(cbCardIndex[i]>0))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if(cbCardIndex[6]>=j && cbCardIndex[9]>=j)
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=6;
						KindItem[cbKindItemCount].cbCardIndex[1]=9;
						KindItem[cbKindItemCount].cbCardIndex[2]=i;
						KindItem[cbKindItemCount].cbCardCount = 3;

						KindItem[cbKindItemCount++].cbWeaveKind=WIK_QI_TA;
					}
				}
			}
			//化三千
			else if((i==16)&&(cbCardIndex[i]>0))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if(cbCardIndex[2]>=j && cbCardIndex[17]>=j)
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=2;
						KindItem[cbKindItemCount].cbCardIndex[2]=17;
						KindItem[cbKindItemCount].cbCardCount = 3;

						KindItem[cbKindItemCount++].cbWeaveKind=WIK_QI_TA;
					}
				}

			}
			//可知礼
			else if((i==18)&&(cbCardIndex[i]>0))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if(cbCardIndex[19]>=j && cbCardIndex[20]>=j)
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount].cbCardCount = 3;

						KindItem[cbKindItemCount++].cbWeaveKind=WIK_QI_TA;
					}
				}
			}
			//八九子
			else if((i==21)&&(cbCardIndex[i]>0))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if(cbCardIndex[7]>=j && cbCardIndex[8]>=j)
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=7;
						KindItem[cbKindItemCount].cbCardIndex[1]=8;
						KindItem[cbKindItemCount].cbCardIndex[2]=i;
						KindItem[cbKindItemCount].cbCardCount = 3;

						KindItem[cbKindItemCount++].cbWeaveKind=WIK_QI_TA;
					}
				}

			}
		}
	}

	//打印用户手中牌的所有组合信息
#ifdef _DEBUG
	m_Debug.PrintKindItemsMessage(KindItem,cbKindItemCount);
#endif 

	//分析所有的组合，从而得到可能胡牌的情况
	if (cbKindItemCount>=cbLessKindItem)
	{
		//变量定义
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//变量定义
		BYTE cbIndex[8]={0,1,2,3,4,5,6,7};
		tagKindItem * pKindItem[8];
		ZeroMemory(&pKindItem,sizeof(pKindItem));
		do
		{
			//每次循环将传进来的牌索引数组拷贝到临时数组中，进行分析
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

			//每次从上面分析得出的分析子项中取cbLessKindItem个分析子项进行分析，
			//注意：索引数组cbIndex[]在每次循环结束时都重新设置了
			for (BYTE i=0;i<cbLessKindItem;i++) 
			{
				pKindItem[i]=&KindItem[cbIndex[i]];
			}
	
			//数量判断
			bool bEnoughCard=true;
			//修改临时数组的值，把临时数组中构成cbLessKindItem个分析子项里的每一张牌，牌数减1，
			//以下是都是三个一组的
			for(BYTE k=0;k<cbLessKindItem;k++)
			{
				//对组合里的每一个牌索引进行分析
				for(BYTE i=0; i<pKindItem[k]->cbCardCount; i++)
				{
					//存在判断
					BYTE cbCardIndex=pKindItem[k]->cbCardIndex[i];

					if (cbCardIndexTemp[cbCardIndex]==0)
					{
						bEnoughCard=false;
						break;
					}
					else cbCardIndexTemp[cbCardIndex]--;
				}
			}
			//胡牌判断，注意下面使用到的cbCardIndexTemp[]数组是经前面修改过后的
			if (bEnoughCard==true)
			{
				//统计数组中的个数要为2
				WORD cbCardCount=0;
				for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+= cbCardIndexTemp[i];

				//丫口值
				BYTE cbFirstIndex=-1,cbSecondIndex=-1;
				//对剩下的牌进行分析，判断是否是丫口，
				if (IsYaKou(cbCardIndexTemp,cbFirstIndex,cbSecondIndex) == true)
				{
					ASSERT((cbFirstIndex != -1)&&(cbSecondIndex != -1));

					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//得到组合牌中的牌型，保存到分析子项中
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=SwitchToCardIndex(WeaveItem[i].cbCenterCard);
					}

					//得到手中牌的牌型，保存到分析子项中
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
					}
					//设置牌眼
					AnalyseItem.cbCardEye[0]=cbFirstIndex;
					AnalyseItem.cbCardEye[1]=cbSecondIndex;

					//将分析子项插入到分析数组中
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//设置索引，索引数组中存放的是分析子项数组的下标，每次取分析子项进行分析时，都是按照索引数组
			//里面存放的下标值进行存取，当cbIndex[cbLessKindItem-1]的最后一位存放的值与得出的分析子项下标相同，
			//重新调整索引数组，下一次取值就会取新的组合
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				for (BYTE i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) 
						{
							cbIndex[j]=cbNewIndex+j-i+2;
						}
						break;
					}
				}
				//跳出整个while循环
				if (i==0) break;
			}
			else cbIndex[cbLessKindItem-1]++;
			
		} while (true);
	}
	
	//打印用户手中牌合法的组合信息
#ifdef _DEBUG
	m_Debug.PrintValidKindItemsMessage(AnalyseItemArray);
#endif 

	return (AnalyseItemArray.GetCount()>0);
}

//-----------------以下是打印调试信息的代码----------------------//

CString CDebugMessage::m_strCardsMessage[22]={"乙","二","三","四","五",
										      "六","七","八","九","十",
										      "孔","已","上","大","人",
										      "土","化","千","可","知",
										      "礼","子"};
CDebugMessage::CDebugMessage(void)
{
	m_pITableFrame=NULL;
}

CDebugMessage::~CDebugMessage(void)
{
	m_pITableFrame=NULL;
}

//初始化打印对象中的输出对象
void CDebugMessage::InitDebug(ITableFrame  *pITableFrame)
{
	ASSERT(pITableFrame!=NULL);
	m_pITableFrame=pITableFrame;
}

//开始打印调试信息--每次打印时，先清空字符串中的内容
void CDebugMessage::BeginPrint()
{
	m_strMessage.Empty();
}

//结束打印调度信息--将字符串中的内容一次打印到指定的客户端
void CDebugMessage::EndPrint(WORD wChairID)
{
	if(m_pITableFrame==NULL) return;
    
	if(m_strMessage.IsEmpty() || m_strMessage.GetLength()==0) return;

	//依据用户坐位号而得到用户信息
	IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if(pIServerUserItem==NULL) return;

	m_pITableFrame->SendGameMessage(pIServerUserItem,m_strMessage,SMT_INFO);
}

//打印用户手中的牌信息--在服务器端，用户手中的牌都是以索引的方式存储
void CDebugMessage::PrintCardMessage(BYTE cbCardIndex[])
{
	if(m_pITableFrame==NULL) return;
	
	m_strMessage.Append("\n--开始打印用户手中的牌\n");
	for(BYTE i=0;i<MAX_INDEX;i++)
	{
		if(cbCardIndex[i]>0)
		{
			for(int j=0;j<cbCardIndex[i];j++)
			{
				m_strMessage.Append(m_strCardsMessage[i]);
			}
		}
	}
	m_strMessage.Append("\n--结束打印用户手中的牌\n");
}

//打印用户组合的牌信息--用户组合牌的类型有：二对一、三招一、四招一
void CDebugMessage::PrintWeaveItemsMessage(tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	if(m_pITableFrame==NULL) return;

	if(cbWeaveCount>0)
	{
		m_strMessage.Append("\n--开始打印用户组合的牌\n");
		for (BYTE i=0;i<cbWeaveCount;i++)
		{
			BYTE cbCardIndex=SwitchToCardIndex(WeaveItem[i].cbCenterCard);
			switch(WeaveItem[i].cbWeaveKind)
			{
			case WIK_DUI:
				for(BYTE j=0;j<3;j++)
				{
					m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				}
				m_strMessage.Append(TEXT(" "));
				break;
			case WIK_SAN_ZHAO:
				for(BYTE j=0;j<4;j++)
				{
					m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				}
				m_strMessage.Append(TEXT(" "));
				break;
			case WIK_SI_ZHAO:
				for(BYTE j=0;j<5;j++)
				{
					m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				}
				m_strMessage.Append(TEXT(" "));
				break;
			}
		}
		m_strMessage.Append("\n--结束打印用户组合的牌\n");
	}
	else
	{
		m_strMessage.Append("\n--目前用户无组合牌\n");
	}
}

//打印用户手中牌的所有可能的组合信息
void CDebugMessage::PrintKindItemsMessage(tagKindItem KindItem[],BYTE cbKindItemCount)
{
	if(m_pITableFrame==NULL) return;

	if(cbKindItemCount>0)
	{
		m_strMessage.Append("\n--开始打印手中牌可能的组合\n");
		for (BYTE i=0;i<cbKindItemCount;i++)
		{
			for(BYTE j=0;j<KindItem[i].cbCardCount;j++)
			{
				BYTE cbCardIndex=KindItem[i].cbCardIndex[j];
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			m_strMessage.Append(TEXT(" "));
		}
		m_strMessage.Append("\n--结束打印手中牌可能的组合\n");
	}
	else
	{
		m_strMessage.Append("\n--目前用户手中无组合牌\n");
	}
}

//打印用户可能胡牌的组合信息
void CDebugMessage::PrintValidKindItemsMessage(CAnalyseItemArray &AnalyseItemArray)
{
	if(m_pITableFrame==NULL) return;

	if(AnalyseItemArray.GetCount()>0)
	{
		m_strMessage.Append("\n--开始打印可能胡牌的组合\n");
		for (BYTE i=0;i<AnalyseItemArray.GetCount();i++)
		{
			tagAnalyseItem *pAnalyseItem=&AnalyseItemArray[i];
			//先打印8个组合牌的信息
			for(BYTE j=0;j<8;j++)
			{
				BYTE cbCardIndex=pAnalyseItem->cbCenterCard[j];
				
				switch(pAnalyseItem->cbWeaveKind[j])
				{
				case WIK_DUI:
					for(int t=0;t<3;t++)
					{
						m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
					}
					m_strMessage.Append(TEXT(" "));
					break;
				case WIK_SAN_ZHAO:
					for(int t=0;t<4;t++)
					{
						m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
					}
					m_strMessage.Append(TEXT(" "));
					break;
				case WIK_SI_ZHAO:
					for(int t=0;t<5;t++)
					{
						m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
					}
					m_strMessage.Append(TEXT(" "));
					break;
				case WIK_SI_TONG:
					for(int t=0;t<4;t++)
					{
						m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
					}
					m_strMessage.Append(TEXT(" "));
					break;
				case WIK_WU_TONG:
					for(int t=0;t<6;t++)
					{
						m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
					}
					m_strMessage.Append(TEXT(" "));
					break;
				case WIK_QI_TA:
					if(cbCardIndex>=0 && cbCardIndex<=7)//打印数字的子句
					{
						m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
						m_strMessage.Append(m_strCardsMessage[cbCardIndex+1]);
						m_strMessage.Append(m_strCardsMessage[cbCardIndex+2]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==10)//打印孔乙已的子句
					{
						m_strMessage.Append(m_strCardsMessage[10]);
						m_strMessage.Append(m_strCardsMessage[0]);
						m_strMessage.Append(m_strCardsMessage[11]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==12)//打印上大人的子句
					{
						m_strMessage.Append(m_strCardsMessage[12]);
						m_strMessage.Append(m_strCardsMessage[13]);
						m_strMessage.Append(m_strCardsMessage[14]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==15)//打印七十土的子句
					{
						m_strMessage.Append(m_strCardsMessage[6]);
						m_strMessage.Append(m_strCardsMessage[9]);
						m_strMessage.Append(m_strCardsMessage[15]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==16)//打印化三千的子句
					{
						m_strMessage.Append(m_strCardsMessage[16]);
						m_strMessage.Append(m_strCardsMessage[2]);
						m_strMessage.Append(m_strCardsMessage[17]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==18)//打印可知礼的子句
					{
						m_strMessage.Append(m_strCardsMessage[18]);
						m_strMessage.Append(m_strCardsMessage[19]);
						m_strMessage.Append(m_strCardsMessage[20]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==21)//打印八九子的子句
					{
						m_strMessage.Append(m_strCardsMessage[7]);
						m_strMessage.Append(m_strCardsMessage[8]);
						m_strMessage.Append(m_strCardsMessage[21]);
						m_strMessage.Append(TEXT(" "));
					}
					break;
				}
			}
			
			//后打印丫口信息
			m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[0]]);
			m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[1]]);
		}
		m_strMessage.Append("\n--结束打印可能胡牌的组合\n");
	}
	else
	{
		m_strMessage.Append("\n--目前用户不能胡牌\n");
	}
}

//打印调整之前的组合项的点数、手中牌的总点数、组合项的总点数
void CDebugMessage::PrintValidKindItemPointBeforeMessage(tagAnalyseItem *pAnalyseItem,BYTE cbWeavePoint,BYTE cbHandPoint)
{
	if(m_pITableFrame==NULL) return;

	m_strMessage.Append("\n--开始打印调整前的点数\n");
	//先打印8个组合牌的信息
	CString strTemp;
	for(BYTE j=0;j<8;j++)
	{
		BYTE cbCardIndex=pAnalyseItem->cbCenterCard[j];
		switch(pAnalyseItem->cbWeaveKind[j])
		{
		case WIK_DUI:
			for(int t=0;t<3;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
   			break;
		case WIK_SAN_ZHAO:
			for(int t=0;t<4;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
			break;
		case WIK_SI_ZHAO:
			for(int t=0;t<5;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
			break;
		case WIK_SI_TONG:
			for(int t=0;t<4;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
			break;
		case WIK_WU_TONG:
			for(int t=0;t<6;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
			break;
		case WIK_QI_TA:
			if(cbCardIndex>=0 && cbCardIndex<=7)//打印数字的子句
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+1]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+2]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==10)//打印孔乙已的子句
			{
				m_strMessage.Append(m_strCardsMessage[10]);
				m_strMessage.Append(m_strCardsMessage[0]);
				m_strMessage.Append(m_strCardsMessage[11]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==12)//打印上大人的子句
			{
				m_strMessage.Append(m_strCardsMessage[12]);
				m_strMessage.Append(m_strCardsMessage[13]);
				m_strMessage.Append(m_strCardsMessage[14]);
				strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
				m_strMessage.Append(strTemp);
			}
			else if(cbCardIndex==15)//打印七十土的子句
			{
				m_strMessage.Append(m_strCardsMessage[6]);
				m_strMessage.Append(m_strCardsMessage[9]);
				m_strMessage.Append(m_strCardsMessage[15]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==16)//打印化三千的子句
			{
				m_strMessage.Append(m_strCardsMessage[16]);
				m_strMessage.Append(m_strCardsMessage[2]);
				m_strMessage.Append(m_strCardsMessage[17]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==18)//打印可知礼的子句
			{
				m_strMessage.Append(m_strCardsMessage[18]);
				m_strMessage.Append(m_strCardsMessage[19]);
				m_strMessage.Append(m_strCardsMessage[20]);
				strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
				m_strMessage.Append(strTemp);
			}
			else if(cbCardIndex==21)//打印八九子的子句
			{
				m_strMessage.Append(m_strCardsMessage[7]);
				m_strMessage.Append(m_strCardsMessage[8]);
				m_strMessage.Append(m_strCardsMessage[21]);
				m_strMessage.Append(TEXT(" "));
			}
			break;
		}
	}
	//后打印丫口信息
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[0]]);
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[1]]);
	
	//打印主精及总点数
    m_strMessage.Append(TEXT("|"));
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbGoldCard]);
	strTemp.Format("%d",cbWeavePoint+cbHandPoint);
	m_strMessage.Append(strTemp);

	strTemp.Format("\n--组合牌点数:%d,手中牌点数:%d\n",cbWeavePoint,cbHandPoint);
	m_strMessage.Append(strTemp);

	m_strMessage.Append("\n--结束打印调整前的点数\n");
}

//打印调整之后的组合项的点数、手中牌的总点数、组合项的总点数
void CDebugMessage::PrintValidKindItemPointAfterMessage(tagAnalyseItem *pAnalyseItem,BYTE cbWeavePoint,BYTE cbHandPoint)
{
	if(m_pITableFrame==NULL) return;

	m_strMessage.Append("\n--开始打印调整后的点数\n");
	//先打印8个组合牌的信息
	CString strTemp;
	for(BYTE j=0;j<8;j++)
	{
		BYTE cbCardIndex=pAnalyseItem->cbCenterCard[j];
		switch(pAnalyseItem->cbWeaveKind[j])
		{
		case WIK_DUI:
			for(int t=0;t<3;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
   			break;
		case WIK_SAN_ZHAO:
			for(int t=0;t<4;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
			break;
		case WIK_SI_ZHAO:
			for(int t=0;t<5;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
			break;
		case WIK_SI_TONG:
			for(int t=0;t<4;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
			break;
		case WIK_WU_TONG:
			for(int t=0;t<6;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
			m_strMessage.Append(strTemp);
			break;
		case WIK_QI_TA:
			if(cbCardIndex>=0 && cbCardIndex<=7)//打印数字的子句
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+1]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+2]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==10)//打印孔乙已的子句
			{
				m_strMessage.Append(m_strCardsMessage[10]);
				m_strMessage.Append(m_strCardsMessage[0]);
				m_strMessage.Append(m_strCardsMessage[11]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==12)//打印上大人的子句
			{
				m_strMessage.Append(m_strCardsMessage[12]);
				m_strMessage.Append(m_strCardsMessage[13]);
				m_strMessage.Append(m_strCardsMessage[14]);
				strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
				m_strMessage.Append(strTemp);
			}
			else if(cbCardIndex==15)//打印七十土的子句
			{
				m_strMessage.Append(m_strCardsMessage[6]);
				m_strMessage.Append(m_strCardsMessage[9]);
				m_strMessage.Append(m_strCardsMessage[15]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==16)//打印化三千的子句
			{
				m_strMessage.Append(m_strCardsMessage[16]);
				m_strMessage.Append(m_strCardsMessage[2]);
				m_strMessage.Append(m_strCardsMessage[17]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==18)//打印可知礼的子句
			{
				m_strMessage.Append(m_strCardsMessage[18]);
				m_strMessage.Append(m_strCardsMessage[19]);
				m_strMessage.Append(m_strCardsMessage[20]);
				strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
				m_strMessage.Append(strTemp);
			}
			else if(cbCardIndex==21)//打印八九子的子句
			{
				m_strMessage.Append(m_strCardsMessage[7]);
				m_strMessage.Append(m_strCardsMessage[8]);
				m_strMessage.Append(m_strCardsMessage[21]);
				m_strMessage.Append(TEXT(" "));
			}
			break;
		}
	}
	//后打印丫口信息
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[0]]);
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[1]]);
	
	//打印主精及总点数
    m_strMessage.Append(TEXT("|"));
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbGoldCard]);
	strTemp.Format("%d",cbWeavePoint+cbHandPoint);
	m_strMessage.Append(strTemp);

	strTemp.Format("\n--组合牌点数:%d,手中牌点数:%d\n",cbWeavePoint,cbHandPoint);
	m_strMessage.Append(strTemp);

	m_strMessage.Append("\n--结束打印调整后的点数\n");
}


//打印最佳组合信息
void CDebugMessage::PrintHuCardMessage(tagAnalyseItem *pAnalyseItem)
{
if(m_pITableFrame==NULL) return;

	m_strMessage.Append("\n--开始打印最佳组合信息\n");
	//先打印8个组合牌的信息
	CString strTemp;
	for(BYTE j=0;j<8;j++)
	{
		BYTE cbCardIndex=pAnalyseItem->cbCenterCard[j];
		switch(pAnalyseItem->cbWeaveKind[j])
		{
		case WIK_DUI:
			for(int t=0;t<3;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			
   			break;
		case WIK_SAN_ZHAO:
			for(int t=0;t<4;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			
			break;
		case WIK_SI_ZHAO:
			for(int t=0;t<5;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			
			break;
		case WIK_SI_TONG:
			for(int t=0;t<4;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			
			break;
		case WIK_WU_TONG:
			for(int t=0;t<6;t++)
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			
			break;
		case WIK_QI_TA:
			if(cbCardIndex>=0 && cbCardIndex<=7)//打印数字的子句
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+1]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+2]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==10)//打印孔乙已的子句
			{
				m_strMessage.Append(m_strCardsMessage[10]);
				m_strMessage.Append(m_strCardsMessage[0]);
				m_strMessage.Append(m_strCardsMessage[11]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==12)//打印上大人的子句
			{
				m_strMessage.Append(m_strCardsMessage[12]);
				m_strMessage.Append(m_strCardsMessage[13]);
				m_strMessage.Append(m_strCardsMessage[14]);
				
			}
			else if(cbCardIndex==15)//打印七十土的子句
			{
				m_strMessage.Append(m_strCardsMessage[6]);
				m_strMessage.Append(m_strCardsMessage[9]);
				m_strMessage.Append(m_strCardsMessage[15]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==16)//打印化三千的子句
			{
				m_strMessage.Append(m_strCardsMessage[16]);
				m_strMessage.Append(m_strCardsMessage[2]);
				m_strMessage.Append(m_strCardsMessage[17]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==18)//打印可知礼的子句
			{
				m_strMessage.Append(m_strCardsMessage[18]);
				m_strMessage.Append(m_strCardsMessage[19]);
				m_strMessage.Append(m_strCardsMessage[20]);
				
			}
			else if(cbCardIndex==21)//打印八九子的子句
			{
				m_strMessage.Append(m_strCardsMessage[7]);
				m_strMessage.Append(m_strCardsMessage[8]);
				m_strMessage.Append(m_strCardsMessage[21]);
				m_strMessage.Append(TEXT(" "));
			}
			break;
		}
	}
	//后打印丫口信息
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[0]]);
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[1]]);
	
	//打印主精及总点数
    m_strMessage.Append(TEXT("|"));
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbGoldCard]);
	strTemp.Format("%d",pAnalyseItem->cbPoint);
	m_strMessage.Append(strTemp);

	m_strMessage.Append("\n--结束打印最佳组合信息\n");
}
BYTE CDebugMessage::SwitchToCardIndex(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//如果是数字，直接得到最后一位值
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}