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
	0x4E,0x4F,
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x4E,0x4F,
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
	//逻辑变量
	m_cbMainValue=2;

	return;
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//随机扑克
BYTE CGameLogic::GetRandomCard(void)
{
	size_t cbIndex = rand()%(sizeof(m_cbCardData)) ;
	return m_cbCardData[cbIndex] ;
}

//获取积分
WORD CGameLogic::GetCardScore(const BYTE cbCardData[], BYTE cbCardCount)
{
	//变量定义
	WORD wCardScore=0;

	//扑克累计
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//获取数值
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//累计积分
		if (cbCardValue==5) 
		{
			wCardScore+=5;
			continue;
		}

		//累计积分
		if ((cbCardValue==10)||(cbCardValue==13)) 
		{
			wCardScore+=10;
			continue;
		}
	}

	return wCardScore;
}

//获取类型
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//简单牌型
	switch (cbCardCount)
	{
	case 0:	//空牌
		{
			return CT_ERROR;
		}
	case 1: //单牌
		{
			return CT_SINGLE;
		}
	case 2:	//两张
		{
			if (GetCardValue(cbCardData[0])==GetCardValue(cbCardData[1])) 
			{
				if(GetCardValue(cbCardData[0])!=0x0E && GetCardValue(cbCardData[0])!=0x0F)
					return CT_DOUBLE;		//一般对
				else if(GetCardValue(cbCardData[0])==0x0E)
					return CT_KING_ST;		//纯小王
				else if(GetCardValue(cbCardData[0])==0x0F)
					return CT_KING_SK;		//纯大王
			}else if((GetCardValue(cbCardData[0])==0x0E && GetCardValue(cbCardData[1])==0x0F)
				|| (GetCardValue(cbCardData[0])==0x0F && GetCardValue(cbCardData[1])==0x0E))
				return CT_KING_DC;			//大小王
			else
				return CT_ERROR;
		}
	case 3:	//三牌
		{
			if (GetCardValue(cbCardData[0])==GetCardValue(cbCardData[2]))  return CT_BOMB_3;
			if (GetCardValue(cbCardData[0])==13&&GetCardValue(cbCardData[1])==10&&GetCardValue(cbCardData[2])==5)
			{
				if(GetCardColor(cbCardData[0])==GetCardColor(cbCardData[1])&&GetCardColor(cbCardData[0])==GetCardColor(cbCardData[2]))
					return CT_510K_SC;	//同色510K
				return CT_510K_DC;		//普通510K
			}
			if ((GetCardValue(cbCardData[0])==0x0E || GetCardValue(cbCardData[0])==0x0F) &&
				(GetCardValue(cbCardData[1])==0x0E || GetCardValue(cbCardData[1])==0x0F) &&
				(GetCardValue(cbCardData[2])==0x0E || GetCardValue(cbCardData[2])==0x0F)
				)
				return CT_KING_THREE;	//3王
		}
	}

	//分析扑克
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//炸弹类型
	if ((cbCardCount==4)&&(cbCardData[0]==0x4F)&&(cbCardData[3]==0x4E)) return CT_KING_FOUR;//4王
	if ((cbCardCount==4)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_4;
	if ((cbCardCount==5)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_5;
	if ((cbCardCount==6)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_6;
	if ((cbCardCount==7)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_7;
	if ((cbCardCount==8)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_8;

	//顺子类型
	if ((cbCardCount>=3)&&(AnalyseResult.cbBlockCount[0]==cbCardCount))
	{
		//扑克属性
		BYTE cbSignedCount=AnalyseResult.cbBlockCount[0];
		BYTE cbCardColor=GetCardColor(AnalyseResult.cbCardData[0],cbSignedCount);
		if ( IsStructureLink(AnalyseResult.cbCardData[0],cbSignedCount,1) )
			return CT_SINGLE_LINK;		
	}

	//对连类型
	if ((cbCardCount>=6)&&((AnalyseResult.cbBlockCount[1]*2)==cbCardCount))
	{
		BYTE cbDoubleCount=AnalyseResult.cbBlockCount[1]*2;
		if (IsStructureLink(AnalyseResult.cbCardData[1],cbDoubleCount,2)) 
			return CT_DOUBLE_LINK;
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
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//排序过虑
	if (cbCardCount==0) return;
	if (cbSortType==ST_CUSTOM) return;

	//转换数值
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++)
	{
		switch (cbSortType)
		{
		case ST_COUNT:	//数目排序
		case ST_ORDER:	//等级排序
			{
				cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
				break;
			}
		case ST_VALUE:	//数值排序
			{
				cbSortValue[i]=GetCardValue(cbCardData[i]);
				break;
			}
		case ST_COLOR:	//花色排序
			{
				cbSortValue[i]=GetCardColor(cbCardData[i])+GetCardLogicValue(cbCardData[i]);
				break;
			}
		}
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

	//数目排序
	if (cbSortType==ST_COUNT)
	{
		//变量定义
		BYTE cbCardIndex=0;

		//分析扑克
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

		//提取扑克
		for (BYTE i=0;i<CountArray(AnalyseResult.cbBlockCount);i++)
		{
			//拷贝扑克
			BYTE cbIndex=CountArray(AnalyseResult.cbBlockCount)-i-1;
			CopyMemory(&cbCardData[cbCardIndex],AnalyseResult.cbCardData[cbIndex],AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE));

			//设置索引
			cbCardIndex+=AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE);
		}
	}

	return;
}

//混乱扑克
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
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

//删除扑克
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//检验数据
	ASSERT(cbRemoveCount<=cbCardCount);

	//定义变量
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
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

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//扑克属性
	BYTE cbCardValue=GetCardValue(cbCardData);

	//主牌扑克
	//if (cbCardValue==m_cbMainValue) return 15;

	//逻辑数值
	switch (cbCardValue)
	{
	case 0x01: { return 14; }
	case 0x02: { return 15;	}
	case 0x0E: { return 16; }
	case 0x0F: { return 17; }
	}

	//转换数值
	return cbCardValue;
}
//对比扑克
bool CGameLogic::CompareCard( const BYTE cbFirstCard,const BYTE cbNextCard )
{
	return GetCardLogicValue(cbNextCard)>GetCardLogicValue(cbFirstCard);
}
//按变幻牌分析扑克
VOID CGameLogic::AnalyseMagicData( const BYTE cbHandCardData[],BYTE cbHandCardCount,tagMagicResult &MagicResult )
{
	//清0
	ZeroMemory( &MagicResult,sizeof(MagicResult) );
	//验证
	if( cbHandCardCount <= 0 ) return ;

	for( BYTE i = 0; i < cbHandCardCount; i++ )
	{
		BYTE cbCardValue = GetCardValue(cbHandCardData[i]);
		BYTE cbCardColor = GetCardColor(cbHandCardData[i]);
		if( cbCardValue==m_cbMainValue && cbCardColor==0x20 )
			MagicResult.cbMagicCard[MagicResult.cbMagicCardCount++] = cbHandCardData[i];
		else
			MagicResult.cbNormalCard[MagicResult.cbNormalCardCount++] = cbHandCardData[i];
	}
}

//对比扑克
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//类型判断
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

	//炸弹以上一定大于单牌、对子和单龙
	if(cbNextType>=CT_BOMB_3 && cbFirstType<=CT_SINGLE_LINK) return true;

	//第二类型牌
	if(cbNextType>=CT_BOMB_3 && cbFirstType!=CT_DOUBLE_LINK)
	{
		if(cbNextType!=cbFirstType) //不同类型直接比较
			return cbNextType>cbFirstType;
		else if(cbNextType==CT_510K_SC) //类型相同且是纯510K时，需要比较花色
			return GetCardColor(cbNextCard[0])>GetCardColor(cbFirstCard[0]);
		else
			return GetCardLogicValue(cbNextCard[cbNextCount-1])>GetCardLogicValue(cbFirstCard[cbFirstCount-1]);
	}

	//第一类牌型的不同类型比较
	if (cbFirstType!=cbNextType)
	{
		if (cbFirstType==CT_DOUBLE_LINK)
		{
			if (cbNextType>=CT_BOMB_4)
				return true;
			else
				return false;
		}
		//特殊处理 数目不同不比较

		if(cbNextCount!=cbFirstCount) return false;
		else return cbNextType>cbFirstType;
	}

	//相同类型
	switch (cbFirstType)
	{
	case CT_SINGLE:				//单牌类型
	case CT_DOUBLE:				//对子类型
	{
			//变量定义
			BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
			BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);

			return cbConsultNext>cbConsultFirst;
		}
	case CT_SINGLE_LINK:		//单连类型
	case CT_DOUBLE_LINK:		//对连类型
		{
			if(cbNextCount!=cbFirstCount) return false;
			return CompareCardByValue(cbFirstCard,cbNextCard,cbFirstCount,cbNextCount);
		}
	}

	//错误断言
	ASSERT(FALSE);
	
	return false;
}

//变幻扑克
bool CGameLogic::MagicCardData(BYTE cbCardData[], BYTE cbCardCount, BYTE cbResultCard[MAX_COUNT], BYTE cbCardType)
{
	//不变换
	//设置扑克
	CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));

	return false;

	//变量定义
	BYTE cbNormalCard[MAX_COUNT];
	BYTE cbMagicCardCount=0,cbNormalCardCount=0;

	//变幻准备
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//扑克属性
		BYTE cbCardValue=GetCardValue(cbCardData[i]);
		BYTE cbCardColor=GetCardColor(cbCardData[i]);

		//变幻统计
		if ((cbCardValue==m_cbMainValue)&&(cbCardColor==0x20)) cbMagicCardCount++;
		else cbNormalCard[cbNormalCardCount++]=cbCardData[i];
	}

	//失败判断
	if ((cbMagicCardCount==0)||(cbNormalCardCount==0))
	{
		CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));
		return false;
	}

	//获取数值
	BYTE cbLogicValue=GetCardLogicValue(cbNormalCard[0]);

	//同牌变幻
	if (cbLogicValue<=15)
	{
		for (BYTE i=1;i<cbNormalCardCount;i++)
		{
			if (GetCardLogicValue(cbNormalCard[i])!=cbLogicValue) break;
		}
		if (i==cbNormalCardCount)
		{
			//设置结果
			for (BYTE j=0;j<cbMagicCardCount;j++) cbResultCard[j]=cbNormalCard[0];
			for (BYTE j=0;j<cbNormalCardCount;j++) cbResultCard[j+cbMagicCardCount]=cbNormalCard[j];

			return true;
		}
	}

	//扑克分析
	BYTE cbMaxSameCount=1,cbSameCount=1;
	for (BYTE i=1;i<cbNormalCardCount;i++)
	{
		//获取扑克
		BYTE cbCardValueTemp=GetCardLogicValue(cbNormalCard[i]);
		if (cbCardValueTemp==cbLogicValue) cbSameCount++;

		//结果处理
		if ((cbCardValueTemp!=cbLogicValue)||(i==(cbNormalCardCount-1)))
		{
			cbMaxSameCount=__max(cbSameCount,cbMaxSameCount);
		}

		//恢复变量
		if ((cbCardValueTemp!=cbLogicValue)&&(i!=(cbNormalCardCount-1)))
		{
			cbSameCount=1;
			cbLogicValue=cbCardValueTemp;
		}
	}

	//三带二型
	if ((cbCardCount==5)&&(cbMaxSameCount<=3))
	{
		//变量定义
		BYTE cbHeadCount=1;
		BYTE cbTailCount=1;

		//变量定义
		BYTE cbHeadValue=GetCardValue(cbNormalCard[0]);
		BYTE cbTailValue=GetCardValue(cbNormalCard[cbNormalCardCount-1]);

		//变幻判断
		for (BYTE i=1;i<cbNormalCardCount-1;i++)
		{
			//获取数值
			BYTE cbCardValue=GetCardValue(cbNormalCard[i]);

			//扑克判断
			if (cbCardValue==cbHeadValue) 
			{
				cbHeadCount++;
				continue;
			}

			//扑克判断
			if (cbCardValue==cbTailValue) 
			{
				cbTailCount++;
				continue;
			}

			break;
		}

		//成功判断
		if (i==(cbNormalCardCount-1))
		{
			//变化数值
			BYTE cbMagicValue=cbHeadValue;
			if (cbHeadCount==3) cbMagicValue=cbTailValue;

			//构造扑克
			CopyMemory(cbResultCard,cbNormalCard,cbHeadCount*sizeof(BYTE));
			CopyMemory(&cbResultCard[cbCardCount-cbTailCount],&cbNormalCard[cbHeadCount],cbTailCount*sizeof(BYTE));

			//变幻扑克
			for (BYTE j=0;j<cbMagicCardCount;j++)
			{
				cbResultCard[cbHeadCount+j]=MakeCardData(cbMagicValue-1,0);
			}

			return true;
		}
	}

	//连牌判断
	bool bMagicLink=false;
	if ((cbMaxSameCount==1)&&(cbCardCount==5)) bMagicLink=true;
	if (((cbMaxSameCount==2)||(cbMaxSameCount==3))&&(cbCardCount==6)) bMagicLink=true;

	//连牌变幻
	if (bMagicLink==true)
	{
		//分析分布
		tagDistributing Distributing;
		AnalysebDistributing(cbNormalCard,cbNormalCardCount,Distributing);

		//填充分析
		for (BYTE cbTimes=0;cbTimes<2;cbTimes++)
		{
			//变量定义
			BYTE cbFillCount=0;
			BYTE cbLeaveCount=cbNormalCardCount;
			BYTE cbUseableCount=cbMagicCardCount;

			//填充状态
			BYTE cbFillStatus[13];
			ZeroMemory(cbFillStatus,sizeof(cbFillStatus));

			//填充扑克
			for (BYTE i=0;i<14;i++)
			{
				//变量定义
				BYTE cbIndex=((cbTimes==1)?(13-i)%13:i)%13;

				//填充判断
				if ((cbLeaveCount!=cbNormalCardCount)||(Distributing.cbDistributing[cbIndex][cbIndexCount]!=0))
				{
					//失败判断
					if ((Distributing.cbDistributing[cbIndex][cbIndexCount]+cbUseableCount)<cbMaxSameCount) break;

					//变量定义
					BYTE cbTurnFillCount=0;

					//常规扑克
					for (BYTE j=0;j<4;j++)
					{
						for (BYTE k=0;k<Distributing.cbDistributing[cbIndex][j];k++)
						{
							cbLeaveCount--;
							cbTurnFillCount++;
							cbResultCard[cbFillCount++]=MakeCardData(cbIndex,j);
						}
					}

					//填充变幻
					for (BYTE i=cbTurnFillCount;i<cbMaxSameCount;i++)
					{
						cbUseableCount--;
						cbResultCard[cbFillCount++]=MakeCardData(cbIndex,GetCardColor(cbNormalCard[0])>>4);
					}

					//设置变量
					cbFillStatus[cbIndex]=cbMaxSameCount;
				}
			}

			//成功判断
			if ((cbUseableCount==0)&&(cbLeaveCount==0)) 
			{
				SortCardList(cbResultCard,cbCardCount,ST_ORDER);
				return true;
			}
		}
	}

	//设置扑克
	CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));

	return false;
}

//分析扑克
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//设置结果
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//扑克分析
	//for (BYTE i=0;i<cbCardCount;i++)
	for (BYTE i=0;i<cbCardCount;)
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
		BYTE cbIndex=AnalyseResult.cbBlockCount[cbSameCount-1]++;
		for (BYTE j=0;j<cbSameCount;j++) 
			AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];

		//设置索引
		i+=cbSameCount;
		//i+=cbSameCount-1;
	}

	return;
}

//分析分布
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//设置变量
	ZeroMemory(&Distributing,sizeof(Distributing));

	//设置变量
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbCardData[i]==0) continue;

		//获取属性
		BYTE cbCardColor=GetCardColor(cbCardData[i]);
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//分布信息
		Distributing.cbCardCount++;//手中牌的总个数
		Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;		//某一数值牌的总个数
		Distributing.cbDistributing[cbCardValue-1][cbCardColor>>4]++;	//某一数值牌，某个花色的总个数
	}

	return;
}
//搜索洞牌
bool CGameLogic::SearchDong( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult )
{
	BYTE tempCardData[MAX_COUNT];ZeroMemory(tempCardData,sizeof(tempCardData));
	CopyMemory(tempCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	SortCardList(tempCardData,cbHandCardCount,ST_ORDER);

	for(BYTE i=cbHandCardCount;i>=2;i--)
	{
		if(tempCardData[i]==tempCardData[i-2])
		{
			if(GetCardLogicValue(tempCardData[i])>GetCardLogicValue(cbReferCard))
			{
				OutCardResult.cbCardCount=3;
				OutCardResult.cbResultCard[0]=tempCardData[i-2];
				OutCardResult.cbResultCard[1]=tempCardData[i-1];
				OutCardResult.cbResultCard[2]=tempCardData[i-0];
				return true;
			}
		}
	}
	return false;
}


bool CGameLogic::SearchPromptCard(const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
								  tagOutCardResult& OutCardResult,BYTE & cbPromptType,const tagAnalyseResult & AnalyseResult)
{
	BYTE cbType = cbPromptType,cbSameCount = 0;
	int i;
	switch(cbType)
	{
		//单牌类型
		case CT_SINGLE:
			{
				BYTE cbTurnCardLogicValue = GetCardLogicValue(cbTurnCardData[0]);
				for(i=cbHandCardCount-1;i>=0;i--)
				{
					if(GetCardLogicValue(cbHandCardData[i])>cbTurnCardLogicValue)
					{
						OutCardResult.cbCardCount = 1;
						OutCardResult.cbResultCard[0] = cbHandCardData[i];
						return true;
					}
				}
				cbPromptType = CT_BOMB_3;	//从3炸弹开始搜索
			}
			break;
		case CT_DOUBLE:
			{
				if(AnalyseResult.cbBlockCount[1]>0)
				{
					for(BYTE j=0;j<AnalyseResult.cbBlockCount[1];j++)
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[1][2*j],2,2))
						{
							OutCardResult.cbCardCount = 2;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[1][2*j],sizeof(BYTE)*2);
							return true;
						}
				}
				cbPromptType = CT_BOMB_3;
			}
			break;
		case CT_SINGLE_LINK:
			{
				BYTE cbRefCard = cbTurnCardData[0];
				if( SearchLinkCard(cbHandCardData,cbHandCardCount,cbRefCard,CT_SINGLE_LINK,OutCardResult,cbTurnCardCount))
				{
					return true;
				}
				cbPromptType = CT_BOMB_3;
			}
			break;
		case CT_DOUBLE_LINK:
			{
				BYTE cbRefCard = cbTurnCardData[0];
				if( SearchLinkCard(cbHandCardData,cbHandCardCount,cbRefCard,CT_DOUBLE_LINK,OutCardResult,cbTurnCardCount))
				{
					return true;
				}
				cbPromptType = CT_BOMB_4;
			}
			break;
		case CT_510K_DC:
		case CT_510K_SC:
			{
				if( Search510K(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount,cbType,OutCardResult) )
				{
				//		ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
						return true;
				}
				cbPromptType = cbType+1;
			}
			break;
		//大小王类型
		case CT_KING_DC:	//大小王
		case CT_KING_ST:	//纯小王
		case CT_KING_SK:	//纯大王
		case CT_KING_THREE:	//3王
		case CT_KING_FOUR:	//4王
			if(SearchKings(cbHandCardData,cbHandCardCount,OutCardResult,cbType))
				return true;
			cbPromptType = cbType+1;
			break;
		//3至8炸弹类型
		case CT_BOMB_3:
			cbSameCount = 3;
			goto LABLE_BOMB3_8;
		case CT_BOMB_4:
			cbSameCount = 4;
			goto LABLE_BOMB3_8;
		case CT_BOMB_5:
			cbSameCount = 5;
			goto LABLE_BOMB3_8;
		case CT_BOMB_6:
			cbSameCount = 6;
			goto LABLE_BOMB3_8;
		case CT_BOMB_7:
			cbSameCount = 7;
			goto LABLE_BOMB3_8;
		case CT_BOMB_8:
			cbSameCount = 8;
			{
LABLE_BOMB3_8:
				BYTE cbIndex=cbSameCount-1;
				if( AnalyseResult.cbBlockCount[cbIndex] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[cbIndex]-1; j >= 0; j-- )
					{
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[cbIndex][j*cbSameCount],cbTurnCardCount,cbSameCount) )
						{
							OutCardResult.cbCardCount = cbSameCount;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[cbIndex][j*cbSameCount],sizeof(BYTE)*cbSameCount);
							ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
							return true;
						}
					}
				}
				cbPromptType = cbType+1;//在更高级的牌型中查找
			}
			break;
	}
	if(cbPromptType>CT_BOMB_8)
		cbPromptType = CT_ERROR;
	return false;
}

//出牌搜索
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, BYTE& cbPromptType,tagOutCardResult & OutCardResult)
{
	//设置结果
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));
	bool bOK=false;
	//构造扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//排列扑克
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//获取类型
	BYTE cbTurnOutType = GetCardType(cbTurnCardData,cbTurnCardCount);
	cbPromptType  = cbTurnOutType;
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	do{

		//bOK=查找出牌提示(const BYTE cbHandCardData[],BYTE cbHandCardCout,const BYTE cbTurnCardData[]
		//BYTE cbTurnCardCout,tagOutCardResult& OutCardResult BYTE & cbPromptType);
		bOK=SearchPromptCard(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult,cbPromptType,AnalyseResult);
	}while(!bOK && cbPromptType !=CT_ERROR);

	return bOK;
	//出牌分析
	/*switch (cbTurnOutType)
	{
	case CT_ERROR:					//错误类型
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
				return true;
			}

			//单牌处理
			OutCardResult.cbCardCount=1;
			OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-1];

			return true;
		}
	case CT_SINGLE:					//单牌类型
		{
			//BYTE byTurnCardLogicValue = GetCardLogicValue(cbTurnCardData[0]);
			//for( BYTE i = 0; i < cbCardCount; i++ )
			//{
			//	if( GetCardLogicValue(cbCardData[cbCardCount-i-1]) > byTurnCardLogicValue )
			//	{
			//		OutCardResult.cbCardCount = 1;
			//		OutCardResult.cbResultCard[0] = cbCardData[cbCardCount-i-1];
			//		return true;
			//	}
			//}
			//从单牌搜索起
			//for( BYTE i = 0; i < CountArray(AnalyseResult.cbBlockCount); i++ )
			//{
			//	if( AnalyseResult.cbBlockCount[i] > 0 )
			//	{
			//		for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0 ; j-- )
			//		{
			//			if( CompareCard(cbTurnCardData[0],AnalyseResult.cbCardData[i][(i+1)*j]) )
			//			{
			//				OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = AnalyseResult.cbCardData[i][(i+1)*j];
			//				return true;
			//			}
			//		}
			//	}
			//}
			//return false;
		}
	case CT_DOUBLE:					//对牌
		{
			for( BYTE i = cbTurnCardCount-1; i < 3; i++ )
			{
				if( AnalyseResult.cbBlockCount[i] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0; j-- )
					{
						//比较扑克
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[i][(i+1)*j],cbTurnCardCount,cbTurnCardCount) )
						{
							//构造数据
							OutCardResult.cbCardCount = cbTurnCardCount;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][(i+1)*j],sizeof(BYTE)*(cbTurnCardCount));
							return true;
						}
					}
				}
			}
		}
	case CT_SINGLE_LINK:			//单顺
		{
			//只有出牌牌型对应,才搜索
			if(cbTurnOutType == CT_SINGLE_LINK)
			{
				//寻找对比牌
				BYTE byReferCard = cbTurnCardData[0];
				if( GetCardValue(cbTurnCardData[0])==m_cbMainValue )
				{
					BYTE nCellCount = 1;
					if( GetCardValue(cbTurnCardData[nCellCount])<6 && GetCardValue(cbTurnCardData[cbTurnCardCount])<6 )
					{
						byReferCard = IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount)?
							MakeCardData(5,0):MakeCardData(4,0);
					}
					else if( IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount) )
					{
						byReferCard = GetCardValue(cbTurnCardData[nCellCount])==1?
							cbTurnCardData[nCellCount]:MakeCardData(GetCardValue(cbTurnCardData[nCellCount])%13,0);
					}
					else byReferCard = cbTurnCardData[nCellCount];
				}
				if( SearchLinkCard(cbCardData,cbCardCount,byReferCard,cbTurnOutType,OutCardResult,cbTurnCardCount) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}
			}
		}
	case CT_THREE:					//三张牌
		{
			for( BYTE i = cbTurnCardCount-1; i < 3; i++ )
			{
				if( AnalyseResult.cbBlockCount[i] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0; j-- )
					{
						//比较扑克
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[i][(i+1)*j],cbTurnCardCount,cbTurnCardCount) )
						{
							//构造数据
							OutCardResult.cbCardCount = cbTurnCardCount;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][(i+1)*j],sizeof(BYTE)*(cbTurnCardCount));
							return true;
						}
					}
				}
			}
		}
	
	case CT_DOUBLE_LINK:			//双连
		{
			//只有出牌牌型对应,才搜索
			if( cbTurnOutType <= CT_DOUBLE_LINK )
			{
				//寻找对比牌
				BYTE byReferCard = cbTurnCardData[0];
				if(cbTurnOutType<CT_DOUBLE_LINK ) byReferCard=0;
				if( GetCardValue(cbTurnCardData[0])==m_cbMainValue )
				{
					BYTE nCellCount = 2;
					if( GetCardValue(cbTurnCardData[nCellCount])<6 && GetCardValue(cbTurnCardData[cbTurnCardCount])<6 )
					{
						byReferCard = IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount)?
							MakeCardData(5,0):MakeCardData(4,0);
					}
					else if( IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount) )
					{
						byReferCard = GetCardValue(cbTurnCardData[nCellCount])==1?
							cbTurnCardData[nCellCount]:MakeCardData(GetCardValue(cbTurnCardData[nCellCount])%13,0);
					}
					else byReferCard = cbTurnCardData[nCellCount];
				}
				if( SearchLinkCard(cbCardData,cbCardCount,byReferCard,cbTurnOutType,OutCardResult,cbTurnCardCount) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}
			}
		}
	
	case CT_BOMB_4:					//炸弹
		{
			//确定牌数
			BYTE bSameCount = 0;
			bSameCount = 4;

			for( BYTE i = bSameCount-1; i < CountArray(AnalyseResult.cbBlockCount); i++ )
			{
				if( AnalyseResult.cbBlockCount[i] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0; j-- )
					{
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[i][j*(i+1)],cbTurnCardCount,i+1) )
						{
							OutCardResult.cbCardCount = i+1;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][j*(i+1)],sizeof(BYTE)*(i+1));
							ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
							return true;
						}
					}
				}
			}
		}
	case CT_510K_DC:
	case CT_510K_SC:
		{
			if( Search510K(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult) )
			{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
			}
			
		}
	case CT_BOMB_5:
		{
			//确定牌数
			BYTE bSameCount = 0;
			bSameCount = cbTurnOutType>=CT_BOMB_4?cbTurnCardCount:4;

			for( BYTE i = bSameCount-1; i < CountArray(AnalyseResult.cbBlockCount); i++ )
			{
				if( AnalyseResult.cbBlockCount[i] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0; j-- )
					{
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[i][j*(i+1)],cbTurnCardCount,i+1) )
						{
							OutCardResult.cbCardCount = i+1;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][j*(i+1)],sizeof(BYTE)*(i+1));
							ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
							return true;
						}
					}
				}
				//如果5张搜索不到,先搜索同花顺,再搜索6张
				if( i == 4 )
				{
					if( SearchTongHuaShun(cbCardData,cbCardCount,0,	OutCardResult,cbTurnCardData) )
					{
						ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
				}
			}
		}
	case CT_BOMB_6:
	case CT_BOMB_7:
	case CT_BOMB_8:
		{
			if(cbTurnOutType<=CT_BOMB_6)
			{
				for(BYTE i=5;i<12;i++)
				{
					if(AnalyseResult.cbCardData[i][0]!=0)
					{
						if(CompareCard(cbTurnCardData,AnalyseResult.cbCardData[i],cbTurnCardCount,i)==true)
						{
							OutCardResult.cbCardCount=i;
							CopyMemory(OutCardResult.cbResultCard,AnalyseResult.cbCardData[i],sizeof(BYTE)*i);
							return true;
						}
					}
				}
			}
		}

	}*/
	//return false;
}

//构造扑克
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	return (cbColorIndex<<4)|(cbValueIndex+1);
}

//是否连牌
bool CGameLogic::IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCellCount)
{
	//数目判断
	ASSERT((cbCardCount%cbCellCount)==0);
	if ((cbCardCount%cbCellCount)!=0) return false;
	
	//构造扑克
	BYTE cbCardDataTemp[MAX_COUNT];
	CopyMemory(cbCardDataTemp,cbCardData,cbCardCount*sizeof(BYTE));

	SortCardList(cbCardDataTemp,cbCardCount,ST_ORDER);
	if(GetCardValue(cbCardDataTemp[0])==2) return false;

	//扑克排序
	SortCardList(cbCardDataTemp,cbCardCount,ST_VALUE);


	//变量定义
	BYTE cbBlockCount=cbCardCount/cbCellCount;
	BYTE cbFirstValue=GetCardValue(cbCardDataTemp[0]);

	//无效过虑
	if (cbFirstValue>=14) return false;

	//扑克搜索
	for (BYTE i=1;i<cbBlockCount;i++)
	{
		//扑克数值
		BYTE cbCardValue=GetCardValue(cbCardDataTemp[i*cbCellCount]);

		//特殊过虑
		if ((cbCardValue==1)&&(cbFirstValue==13)) continue;

		//连牌判断
		if (cbFirstValue!=(cbCardValue+i)) return false;
	}

	//结果判断
	return true;
}

//对比扑克
bool CGameLogic::CompareCardByValue(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//变量定义
	bool bHaveTwoNext=false;
	BYTE cbConsultNext[2]={0x00,0x00};

	//参照扑克
	for (BYTE i=0;i<cbNextCount;i++)
	{
		//获取数值
		BYTE cbConsultValue=GetCardValue(cbNextCard[i]);

		//设置变量
		if ((bHaveTwoNext==false)&&(cbConsultValue==0x02)) bHaveTwoNext=true;

		//设置扑克
		if (cbConsultValue==0x01)
		{
			if (14>cbConsultNext[0]) cbConsultNext[0]=14;
			if (cbConsultValue>cbConsultNext[1]) cbConsultNext[1]=cbConsultValue;
		}
		else
		{
			if (cbConsultValue>cbConsultNext[0]) cbConsultNext[0]=cbConsultValue;
			if (cbConsultValue>cbConsultNext[1]) cbConsultNext[1]=cbConsultValue;
		}
	}

	//变量定义
	bool bHaveTwoFirst=false;
	BYTE cbConsultFirst[2]={0x00,0x00};

	//参照扑克
	for (BYTE i=0;i<cbFirstCount;i++)
	{
		//获取数值
		BYTE cbConsultValue=GetCardValue(cbFirstCard[i]);

		//设置变量
		if ((bHaveTwoFirst==false)&&(cbConsultValue==0x02)) bHaveTwoFirst=true;

		//设置扑克
		if (cbConsultValue==0x01)
		{
			if (14>cbConsultFirst[0]) cbConsultFirst[0]=14;
			if (cbConsultValue>cbConsultFirst[1]) cbConsultFirst[1]=cbConsultValue;
		}
		else
		{
			if (cbConsultValue>cbConsultFirst[0]) cbConsultFirst[0]=cbConsultValue;
			if (cbConsultValue>cbConsultFirst[1]) cbConsultFirst[1]=cbConsultValue;
		}
	}

	//对比扑克
	BYTE cbResultNext=(bHaveTwoNext==false)?cbConsultNext[0]:cbConsultNext[1];
	BYTE cbResultFirst=(bHaveTwoFirst==false)?cbConsultFirst[0]:cbConsultFirst[1];

	return cbResultNext>cbResultFirst;
}

//搜索连牌
bool CGameLogic::SearchLinkCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbCardType,tagOutCardResult &OutCardResult  ,BYTE TurnCardCount)
{
	//A封顶,直接返回或者手中牌数不够
	if( GetCardValue(cbReferCard) == 1 || cbHandCardCount<TurnCardCount)
	{
		//if(cbCardType==CT_THREE_LINK)	return false;
		//else
		//{
		//	if(cbCardType==CT_SINGLE_LINK&&TurnCardCount%2==0) cbCardType=CT_DOUBLE_LINK;
		//	else if(cbCardType==CT_DOUBLE_LINK&&TurnCardCount%3==0) cbCardType=CT_THREE_LINK;
		//	else return false;
		//}
		return false;
	}
research:

	//确定单元数据,连牌数目
	BYTE byCellCount,byLinkCount;
	switch( cbCardType )
	{
	case CT_SINGLE_LINK:
		{
			byCellCount = 1;
			byLinkCount = TurnCardCount/byCellCount;
			break;
		}
	case CT_DOUBLE_LINK:
		{
			byCellCount = 2;
			byLinkCount = TurnCardCount/byCellCount;
			break;
		}
	default:
		ASSERT( FALSE );
		return false;
	}
	//if( cbHandCardCount < byCellCount*byLinkCount ) return false;

	//确定搜索开始位置
	BYTE byReferIndex = 0;
	if( cbReferCard != 0 ) 
		byReferIndex = GetCardValue(cbReferCard)-byLinkCount+1;

	//分析扑克
	tagDistributing Distribute;
	AnalysebDistributing( cbHandCardData,cbHandCardCount,Distribute );

	//搜索常规顺子
	BYTE byTempLinkCount = 0;
	for( BYTE i = byReferIndex; i < 13; i++ )
	{
		if( Distribute.cbDistributing[i][cbIndexCount] < byCellCount )
		{
			byTempLinkCount = 0;
			continue;
		}
		//搜索到
		if( ++byTempLinkCount == byLinkCount )
		{
			//构造数据
			OutCardResult.cbCardCount = 0;
			for( int j = i; j >= i-byLinkCount+1; j-- )
			{
				BYTE byCount = 0;
				for( BYTE k = 3; ; k = (k+1)%4 )
				{
					if( Distribute.cbDistributing[j][k] > 0 )
					{
						for( BYTE n = 0; n < Distribute.cbDistributing[j][k]; n++ )
						{
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(j,k);
							if( ++byCount == byCellCount ) break;
						}
						if( byCount == byCellCount ) break;
					}
				}
			}
			return true;
		}
	}
	//搜索到A的顺子
	if( byTempLinkCount == byLinkCount-1 && Distribute.cbDistributing[0][cbIndexCount] >= byCellCount )
	{
		//构造数据
		OutCardResult.cbCardCount = 0;
		BYTE byCount = 0;
		//放入A
		for( BYTE k = 3; ; k = (k+1)%4 )
		{
			if( Distribute.cbDistributing[0][k] > 0 )
			{
				for( BYTE n = 0; n < Distribute.cbDistributing[0][k]; n++ )
				{
					OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(0,k);
					if( ++byCount == byCellCount ) break;
				}
				if( byCount == byCellCount ) break;
			}
		}
		//放入其他牌
		for( i = 12; i >= 14-byLinkCount; i-- )
		{
			byCount = 0;
			for( k = 3; ; k = (k+1)%4 )
			{
				if( Distribute.cbDistributing[i][k] > 0 )
				{
					for( BYTE n = 0; n < Distribute.cbDistributing[i][k]; n++ )
					{
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(i,k);
						if( ++byCount == byCellCount ) break;
					}
					if( byCount == byCellCount ) break;
				}
			}
		}
		return true;
	}

	/*if(OutCardResult.cbCardCount==0)
	{
		//if(cbCardType==CT_SINGLE_LINK&&TurnCardCount%2==0) cbCardType=CT_DOUBLE_LINK;
		//else if(cbCardType==CT_DOUBLE_LINK&&TurnCardCount%3==0) cbCardType=CT_THREE_LINK;
		//else return false;
		return false;

		goto research;
	}*/
	return false;
}


//搜索510K
bool CGameLogic::Search510K( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,BYTE cbType,tagOutCardResult &OutCardResult )
{
	//先过滤当前牌型小于510K
	/*if(GetCardType(cbTurnCardData,cbTurnCardCount)<CT_510K_DC)
	{
		BYTE Card5=0;
		BYTE Card10=0;
		BYTE CardK=0;
		for(BYTE i=0;i<cbHandCardCount;i++)
		{
			if(GetCardValue(cbHandCardData[i])==5) Card5=cbHandCardData[i];
			if(GetCardValue(cbHandCardData[i])==10) Card10=cbHandCardData[i];
			if(GetCardValue(cbHandCardData[i])==13) CardK=cbHandCardData[i];
		}

		if(Card5!=0&&Card10!=0&&CardK!=0)
		{
			OutCardResult.cbCardCount=3;
			OutCardResult.cbResultCard[0]=CardK;
			OutCardResult.cbResultCard[1]=Card10;
			OutCardResult.cbResultCard[2]=Card5;
			return true;
		}
	}

	//搜索纯色510K
	BYTE bTurnColor=GetCardColor(cbTurnCardData[0]);

	//黑桃510K最大
	if(bTurnColor==0x30) return false;

	//记录5
	BYTE Card5[8];
	BYTE Card5Count=0;
	ZeroMemory(Card5,sizeof(Card5));

	//记录10
	BYTE Card10[8];
	BYTE Card10Count=0;
	ZeroMemory(Card10,sizeof(Card10));

	//记录K
	BYTE CardK[8];
	BYTE CardKCount=0;
	ZeroMemory(CardK,sizeof(CardK));

	//搜索510K
	for(BYTE i=0;i<cbHandCardCount;i++)
	{
		if(GetCardValue(cbHandCardData[i])==5) Card5[Card5Count++]=cbHandCardData[i];
		if(GetCardValue(cbHandCardData[i])==10) Card10[Card10Count++]=cbHandCardData[i];
		if(GetCardValue(cbHandCardData[i])==13) CardK[CardKCount++]=cbHandCardData[i];
	}

	BYTE temp510K[3];	ZeroMemory(temp510K,sizeof(temp510K));
	for(BYTE a=0;a<Card5Count;a++)
	for(BYTE b=0;b<Card10Count;b++)
	for(BYTE c=0;c<CardKCount;c++)
	{
		temp510K[0]=CardK[c];	
		temp510K[1]=Card10[b];  
		temp510K[2]=Card5[a];	

		if(CompareCard(cbTurnCardData,temp510K,cbTurnCardCount,3)==true)
		{
			OutCardResult.cbCardCount=3;
			OutCardResult.cbResultCard[0]=CardK[c];
			OutCardResult.cbResultCard[1]=Card10[b];
			OutCardResult.cbResultCard[2]=Card5[a];
			return true;
		}
	}*/
	//花色掩码
	BYTE COLOR_MASK=0;
	//5，10，k的花色
	BYTE COLOR_5=0,COLOR_10=0,COLOR_K=0;
	//纯510k的花色
	BYTE COLOR_510K=0;
	//5,10,k存在标识
	BYTE FLAG_510K=0;
	//检查要求查询的牌型是否正确
	if(cbType!=CT_510K_DC && cbType!=CT_510K_SC) return false;
	//如果已经出的牌是黑桃510K，就不用查了
	if((GetCardType(cbTurnCardData,cbTurnCardCount)==CT_510K_SC)
		&& (GetCardColor(cbTurnCardData[0])==0x30))
		return false;
	for(BYTE i=0;i<cbHandCardCount;i++)
	{
		FLAG_510K=0;
		if(GetCardValue(cbHandCardData[i])==0x05)
		{
			FLAG_510K=1;
		}
		else if(GetCardValue(cbHandCardData[i])==0x0A)
		{
			FLAG_510K=2;
		}
		else if(GetCardValue(cbHandCardData[i])==0x0D)
		{
			FLAG_510K=4;
		}
		if(FLAG_510K)
		{
			switch(GetCardColor(cbHandCardData[i]))
			{
			case 0:	//方块
				COLOR_MASK=0x01;
				break;
			case 1:	//梅花
				COLOR_MASK=0x02;
				break;
			case 2:	//红桃
				COLOR_MASK=0x04;
				break;
			case 3:	//黑桃
				COLOR_MASK=0x08;
				break;
			}
			switch(FLAG_510K)
			{
			case 1:	//5
				COLOR_5|=COLOR_MASK;
				break;
			case 2:	//10
				COLOR_10|=COLOR_MASK;
				break;
			case 4:	//k
				COLOR_K|=COLOR_MASK;
				break;
			}
		}
	}
	//5，10，k都存在
	if(COLOR_5>0 && COLOR_10>0 && COLOR_K>0)
	{
		COLOR_510K=COLOR_5 & COLOR_10 & COLOR_K;	//得到纯510k的花色
		//查询纯510k
		if(cbType==CT_510K_SC)
		{
			switch(GetCardColor(cbTurnCardData[0]))
			{
			case 0:	//方块
				COLOR_MASK=0x01;
				break;
			case 1:	//梅花
				COLOR_MASK=0x02;
				break;
			case 2:	//红桃
				COLOR_MASK=0x04;
				break;
			case 3:	//黑桃
				COLOR_MASK=0x08;
				break;
			}
			//没有或有也不大于
			if(COLOR_510K==0 || COLOR_510K<=COLOR_MASK) return false;
			for(BYTE i=COLOR_MASK<<1;i<0x10;i<<=1)
				if((COLOR_510K & i)>0)
				{
					COLOR_MASK = i;
					break;
				}
			switch(COLOR_MASK)
			{
			case 1:
				FLAG_510K =0;
				break;
			case 2:
				FLAG_510K =0x10;
				break;
			case 4:
				FLAG_510K =0x20;
				break;
			case 8:
				FLAG_510K =0x30;
				break;
			default:
				return false;
			}
			OutCardResult.cbCardCount = 3;
			//构造纯510k
			OutCardResult.cbResultCard[0] = FLAG_510K+0x0D;
			OutCardResult.cbResultCard[1] = FLAG_510K+0x0A;
			OutCardResult.cbResultCard[2] = FLAG_510K+0x05;
			return true;
		}
		//查询混510K
		else if(cbType == CT_510K_DC)
		{
			//清除纯510k的花色
			COLOR_5 ^=COLOR_510K;
			COLOR_10 ^=COLOR_510K;
			COLOR_K ^=COLOR_510K;
			BYTE Cards[3],Data510K[]={0x0D,0x0A,0x05};
			Cards[0]=COLOR_K;
			Cards[1]=COLOR_10;
			Cards[2]=COLOR_5;
			OutCardResult.cbCardCount=3;
			for(BYTE i=0;i<3;i++)
			{
				for(BYTE j=0x01;j<0x10;j<<=1)
				{
					if(Cards[i]&j)
					{
						switch(Cards[i]&j)
						{
						case 1:
							FLAG_510K =0;
							break;
						case 2:
							FLAG_510K =0x10;
							break;
						case 4:
							FLAG_510K =0x20;
							break;
						case 8:
							FLAG_510K =0x30;
							break;
						default:
							return false;
						}
						OutCardResult.cbResultCard[i]=FLAG_510K+Data510K[i];
						break;
					}
				}
			}
			return true;
		}
	}
	return false;
}


//按给定王牌类型搜索
bool CGameLogic::SearchKings(const BYTE cbHandCardData[],BYTE cbHandCardCount,tagOutCardResult &OutCardResult,BYTE cbType)
{
	if(cbHandCardCount<2) return false;
	if(cbHandCardCount<3 && cbType ==CT_KING_THREE) return false;
	if(cbHandCardCount<4 && cbType ==CT_KING_FOUR) return false;
	BYTE cbKingLData[2]={0},cbKingLCount=0;
	BYTE cbKingBData[2]={0},cbKingBCount=0;
	for(BYTE i=0;i<cbHandCardCount;i++)
	{
		if(GetCardValue(cbHandCardData[i])==0x0E)
			cbKingLData[cbKingLCount++]=cbHandCardData[i];
		if(GetCardValue(cbHandCardData[i])==0x0F)
			cbKingBData[cbKingBCount++]=cbHandCardData[i];
	}

	if(cbType == CT_KING_DC && cbKingLCount>0 && cbKingBCount>0) //大小王
	{
		OutCardResult.cbCardCount=2;
		OutCardResult.cbResultCard[0]=cbKingBData[0];
		OutCardResult.cbResultCard[1]=cbKingLData[0];
		return true;
	}
	if(cbType == CT_KING_ST && cbKingLCount==2) //纯小王
	{
		OutCardResult.cbCardCount=2;
		OutCardResult.cbResultCard[0]=OutCardResult.cbResultCard[1]=cbKingLData[0];
		return true;
	}
	
	if(cbType == CT_KING_SK && cbKingBCount==2) //纯大王
	{
		OutCardResult.cbCardCount=2;
		OutCardResult.cbResultCard[0]=OutCardResult.cbResultCard[1]=cbKingBData[0];
		return true;
	}

	if(cbType == CT_KING_THREE && (cbKingLCount+cbKingBCount)>=3) //3王
	{
		OutCardResult.cbCardCount=3;
		CopyMemory(OutCardResult.cbResultCard,cbKingBData,sizeof(BYTE)*cbKingBCount);
		CopyMemory(OutCardResult.cbResultCard+sizeof(BYTE)*cbKingBCount,cbKingLData,sizeof(BYTE)*cbKingLCount);
		return true;
	}

	if(cbType == CT_KING_FOUR && (cbKingLCount+cbKingBCount)==4) //4王
	{
		OutCardResult.cbCardCount=4;
		OutCardResult.cbResultCard[0]=OutCardResult.cbResultCard[1]=cbKingBData[0];
		OutCardResult.cbResultCard[2]=OutCardResult.cbResultCard[3]=cbKingLData[0];
		return true;
	}

	return false;
}
//搜索天王
bool CGameLogic::SearchTianWang( const BYTE cbHandCardData[],BYTE cbHandCardCount,tagOutCardResult &OutCardResult )
{
	//验证
	if( cbHandCardCount < 4 ) return false;

	//搜索
	if( cbHandCardData[0] == 0x4F && cbHandCardData[3] == 0x4E )
	{
		OutCardResult.cbCardCount = 4;
		CopyMemory(OutCardResult.cbResultCard,cbHandCardData,sizeof(BYTE)*4);
		return true;
	}
	return false;
}

//变幻搜索出牌
bool CGameLogic::SearchMagicOutCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,tagOutCardResult &OutCardResult )
{
	//获取牌型
	BYTE cbTurnCardType = GetCardType(cbTurnCardData,cbTurnCardCount);
	ASSERT( cbTurnCardType > CT_ERROR );
	//if( cbTurnCardType == CT_BOMB_TW ) return false;

	//根据牌型搜索出牌
	switch( cbTurnCardType )
	{
	case CT_SINGLE:				//单牌,跳到炸弹开始搜索
	case CT_DOUBLE:				//对牌
	//case CT_THREE:				//三同张
		{
			BYTE cbSameCount = cbTurnCardType;
			//搜索同牌
			if(  cbTurnCardType!=CT_SINGLE && SearchMagicSameCard(cbHandCardData,cbHandCardCount,cbTurnCardData[0],cbSameCount,OutCardResult) )
				return true;
		}
	case CT_SINGLE_LINK:		//单连
	case CT_DOUBLE_LINK:		//双连
		{
			//只有出牌牌型相对应,才搜索
			if( cbTurnCardType >= CT_SINGLE_LINK && cbTurnCardType <= CT_DOUBLE_LINK )
			{
				//寻找对比牌
				BYTE byReferCard = cbTurnCardData[0];
				if( GetCardValue(cbTurnCardData[0])==m_cbMainValue )
				{				
					BYTE nCellCount = cbTurnCardType-3;
					if( GetCardValue(cbTurnCardData[nCellCount])<6 && GetCardValue(cbTurnCardData[cbTurnCardCount])<6 )
					{
						byReferCard = IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount)?
							MakeCardData(5,0):MakeCardData(4,0);
					}
					else if( IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount) )
					{
						byReferCard = GetCardValue(cbTurnCardData[nCellCount])==1?
							cbTurnCardData[nCellCount]:MakeCardData(GetCardValue(cbTurnCardData[nCellCount])%13,0);
					}
					else byReferCard = cbTurnCardData[nCellCount];
				}
				if( SearchMagicLink(cbHandCardData,cbHandCardCount,byReferCard,cbTurnCardType,OutCardResult) )
					return true;
			}
		}
	case CT_BOMB_4:				//炸弹
		{
			//确定炸弹数目
			BYTE cbSameCount = 0,cbReferCard = 0;
			cbSameCount = cbTurnCardType>=CT_BOMB_4?cbTurnCardCount:4;
			cbReferCard = cbTurnCardType>=CT_BOMB_4?cbTurnCardData[0]:0;

			//如果炸弹数小于等于5
			if( cbSameCount <= 5 )
			{
				if( SearchMagicSameCard(cbHandCardData,cbHandCardCount,cbReferCard,cbSameCount,OutCardResult) )
					return true;
				//if( SearchMagicTongHuaShun(cbHandCardData,cbHandCardCount,0,OutCardResult) )
				//	return true;
			}
			//搜索同牌
			else if( SearchMagicSameCard(cbHandCardData,cbHandCardCount,cbReferCard,cbSameCount,OutCardResult) )
				return true;
			break;
		}
	}

	return false;
}



//变幻搜索连牌
bool CGameLogic::SearchMagicLink( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbCardType,tagOutCardResult &OutCardReslut )
{
	//A封顶,直接返回
	if( GetCardValue(cbReferCard) == 1 ) return false;

	//确定单元数,连牌数
	BYTE byCellCount,byLinkCount;
	switch(cbCardType)
	{
	case CT_SINGLE_LINK:
		{
			byCellCount = 1;
			byLinkCount = 5;
			break;
		}
	case CT_DOUBLE_LINK:
		{
			byCellCount = 2;
			byLinkCount = 3;
			break;
		}
	default:
		ASSERT( FALSE );
		return false;
	}
	//手上牌少于必需数,直接返回
	if( byCellCount*byLinkCount > cbHandCardCount ) return false;

	//分析扑克
	tagMagicResult MagicResult;
	AnalyseMagicData(cbHandCardData,cbHandCardCount,MagicResult);
	if( MagicResult.cbMagicCardCount == 0 ) return false;

	//分析扑克
	tagDistributing Distribute;
	AnalysebDistributing( MagicResult.cbNormalCard,MagicResult.cbNormalCardCount,Distribute );

	//确定开始搜索位置
	BYTE byReferIndex = 0;
	if( cbReferCard != 0 ) 
		byReferIndex = GetCardValue(cbReferCard)-byLinkCount+1;

	//搜索
	for( BYTE byMagicCount = 1; byMagicCount <= MagicResult.cbMagicCardCount; byMagicCount++ )
	{
		//变量定义
		int nMagicCount = byMagicCount;
		BYTE byTmpLinkCount = 0;
		//搜索顺子
		BYTE i = byReferIndex;
		do
		{
			if( Distribute.cbDistributing[i][cbIndexCount] < byCellCount )
			{
				nMagicCount -= (byCellCount-Distribute.cbDistributing[i][cbIndexCount]);
				//判断是否足够
				if( nMagicCount < 0 )
				{
					byTmpLinkCount = 0;
					//可以填补时,向后追朔
					if( byMagicCount >= (byCellCount-Distribute.cbDistributing[i][cbIndexCount]) )
					{
						int nTmpMagicCount = byMagicCount;
						nTmpMagicCount -= (byCellCount-Distribute.cbDistributing[i][cbIndexCount]);
						byTmpLinkCount++;
						if( nTmpMagicCount > 0 )
						{
							BYTE byIndex = i-1;
							do
							{
								if( Distribute.cbDistributing[byIndex][cbIndexCount] < byCellCount )
								{
									nTmpMagicCount -= (byCellCount-Distribute.cbDistributing[i][cbIndexCount]);
									if( nTmpMagicCount < 0 ) break;
									byTmpLinkCount++;
								}
								else byTmpLinkCount++;
								byIndex--;					//注:byIndex始终大于byReferIndex
							}while(true);
						}
						nMagicCount = 0;
					}
					//不可填补时,向前继续搜索
					else nMagicCount = byMagicCount;

					i = (i+1)%13;
					if( i == 1 ) break;
					continue;
				}
				else byTmpLinkCount++;
			}
			else byTmpLinkCount++;
			//可变换
			if( byTmpLinkCount == byLinkCount )
			{
				//构造数据
				BYTE byIndex = (i==0?14-byLinkCount:i-byLinkCount+1);
				BYTE byCount = 0;
				nMagicCount = 0;
				do
				{
					BYTE byTmpCount = 0;
					if( Distribute.cbDistributing[byIndex][cbIndexCount] > 0 )
					{
						for( BYTE j = 0; j < 4; j++ )
						{
							if( Distribute.cbDistributing[byIndex][j] > 0 )
							{
								for( BYTE n = 0; n < Distribute.cbDistributing[byIndex][j]; n++ )
								{
									OutCardReslut.cbResultCard[OutCardReslut.cbCardCount++] = MakeCardData(byIndex,j);
									if( ++byTmpCount == byCellCount ) break;
								}
								if( byTmpCount == byCellCount ) break;
							}
						}
					}
					//填补
					if( byTmpCount < byCellCount )
					{
						for( BYTE k = 0; k < byCellCount-byTmpCount; k++ )
						{
							OutCardReslut.cbResultCard[OutCardReslut.cbCardCount++] = MagicResult.cbMagicCard[nMagicCount++];
							ASSERT( nMagicCount <= byMagicCount );
						}
					}
					byIndex = (byIndex+1)%13;
				}while( ++byCount < byLinkCount );
				//排序
				SortCardList(OutCardReslut.cbResultCard,OutCardReslut.cbCardCount,ST_ORDER);
				return true;
			}
			i = (i+1)%13;
		}while( i != 1 );		//搜索到A为止
	}
	return false;
}

//变幻搜索同张
bool CGameLogic::SearchMagicSameCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbLessSameCount,tagOutCardResult &OutCardResult )
{
	//验证
	if( cbHandCardCount < cbLessSameCount ) return false;

	//变量定义
	tagMagicResult MagicResult;
	tagAnalyseResult AnalyseResult;
	
	//分析扑克
	AnalyseMagicData(cbHandCardData,cbHandCardCount,MagicResult);
	if( MagicResult.cbMagicCardCount == 0 ) return false;

	//分析扑克
	AnalysebCardData(MagicResult.cbNormalCard,MagicResult.cbNormalCardCount,AnalyseResult);

	//搜索
	for( BYTE byMagicCount = 1; byMagicCount <= MagicResult.cbMagicCardCount; byMagicCount++ )
	{
		//确定搜索牌数位置
		int nIndex = cbLessSameCount-byMagicCount-1;
		if( nIndex < 0 ) break;
		if( AnalyseResult.cbBlockCount[nIndex] > 0 )
		{
			for( int i = AnalyseResult.cbBlockCount[nIndex]-1; i >= 0; i-- )
			{ 
				//大小王不能变幻
				if( GetCardLogicValue(AnalyseResult.cbCardData[nIndex][(nIndex+1)*i]) > 14 ) continue;
				//比较大小
				if( cbReferCard==0 || CompareCard(cbReferCard,AnalyseResult.cbCardData[nIndex][(nIndex+1)*i]) )
				{
					CopyMemory(OutCardResult.cbResultCard,MagicResult.cbMagicCard,sizeof(BYTE)*byMagicCount);
					OutCardResult.cbCardCount = byMagicCount;
					CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount],
						&AnalyseResult.cbCardData[nIndex][(nIndex+1)*i],sizeof(BYTE)*(nIndex+1));
					OutCardResult.cbCardCount += nIndex+1;
					SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
					return true;
				}
			}
		}
		//当有相同牌数,但小于cbReferCard时
		if( byMagicCount == 1 && nIndex != 0 /*确保不是在搜索两张牌*/  
			&& AnalyseResult.cbBlockCount[nIndex+1] > 0 )
		{
			ASSERT( CompareCard(AnalyseResult.cbCardData[nIndex+1][0],cbReferCard) );
			OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MagicResult.cbMagicCard[0];
			CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount],
				&AnalyseResult.cbCardData[nIndex+1][(nIndex+2)*(AnalyseResult.cbBlockCount[nIndex+1]-1)],
				sizeof(BYTE)*(nIndex+2));
			OutCardResult.cbCardCount += nIndex+2;
			SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
			return true;
		}
		//当存在小于出牌者一张的相同牌,且有两个变幻牌时
		if( byMagicCount == 2 && AnalyseResult.cbBlockCount[nIndex+1] > 0 )
		{
			if( GetCardLogicValue(AnalyseResult.cbCardData[nIndex+1][(nIndex+2)*(AnalyseResult.cbBlockCount[nIndex+1]-1)]) < 15 )
			{
				CopyMemory(OutCardResult.cbResultCard,MagicResult.cbMagicCard,sizeof(BYTE)*byMagicCount);
				OutCardResult.cbCardCount = 2;
				CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount],
					&AnalyseResult.cbCardData[nIndex+1][(nIndex+2)*(AnalyseResult.cbBlockCount[nIndex+1]-1)],
					sizeof(BYTE)*(nIndex+2));
				SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
				return true;
			}
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
