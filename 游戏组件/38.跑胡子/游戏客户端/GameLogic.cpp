#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE const CGameLogic::m_cbCardDataArray[80]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
};

/*BYTE const CGameLogic::m_cbCardDataArray[80]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x07,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x08,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x09,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x0A,0x0A,0x0A,0x0A,					//小写
	0x11,0x12,0x13,0x14,0x15,0x13,0x17,0x18,0x19,0x17,					//大写
	0x11,0x12,0x13,0x14,0x15,0x14,0x17,0x18,0x19,0x18,					//大写
	0x11,0x12,0x13,0x14,0x15,0x15,0x17,0x18,0x19,0x19,					//大写
	0x11,0x12,0x16,0x16,0x16,0x16,0x1A,0x1A,0x1A,0x1A,					//大写
};*/

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
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE bMaxCount)
{
	//混乱准备
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bMaxCount-bRandCount);
		cbCardData[bRandCount++]=cbCardDataTemp[bPosition];
		cbCardDataTemp[bPosition]=cbCardDataTemp[bMaxCount-bRandCount];
	} while (bRandCount<bMaxCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//效验扑克
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//删除扑克
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//失败效验
	ASSERT(FALSE);

	return false;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE bRemoveCount)
{
	//删除扑克
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//删除扑克
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//错误断言
			ASSERT(FALSE);

			//还原删除
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE bCardCount, BYTE cbRemoveCard[], BYTE bRemoveCount)
{
	//检验数据
	ASSERT(bCardCount<=MAX_COUNT);
	ASSERT(bRemoveCount<=bCardCount);

	//定义变量
	BYTE bDeleteCount=0,cbTempCardData[MAX_COUNT];
	if (bCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,bCardCount*sizeof(cbCardData[0]));

	//置零扑克
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				bDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}

	//成功判断
	if (bDeleteCount!=bRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//清理扑克
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[bCardPos++]=cbTempCardData[i];
	}

	return true;
}

//提牌判断
BYTE CGameLogic::GetAcitonTiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbTiCardIndex[5])
{
	//提牌搜索
	BYTE cbTiCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]==4) cbTiCardIndex[cbTiCardCount++]=i;
	}

	return cbTiCardCount;
}

//畏牌判断
BYTE CGameLogic::GetActionWeiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbWeiCardIndex[7])
{
	//畏牌搜索
	BYTE cbWeiCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]==3) cbWeiCardIndex[cbWeiCardCount++]=i;
	}

	return cbWeiCardCount;
}

//吃牌判断
BYTE CGameLogic::GetActionChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, tagChiCardInfo ChiCardInfo[6])
{
	//效验扑克
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return 0;

	//变量定义
	BYTE cbChiCardCount=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//三牌判断
	if (cbCardIndex[cbCurrentIndex]>=3) return cbChiCardCount;

	//大小搭吃
	BYTE cbReverseIndex=(cbCurrentIndex+10)%MAX_INDEX;
	if ((cbCardIndex[cbCurrentIndex]>=1)&&(cbCardIndex[cbReverseIndex]>=1)&&(cbCardIndex[cbReverseIndex]!=3))
	{
		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//删除扑克
		cbCardIndexTemp[cbCurrentIndex]--;
		cbCardIndexTemp[cbReverseIndex]--;

		//提取判断
		BYTE cbResultCount=1;
		while (cbCardIndexTemp[cbCurrentIndex]>0)
		{
			PBYTE pcbResult=ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
			if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,pcbResult)!=CK_NULL) cbResultCount++;
			else break;
		}

		//设置结果
		if (cbCardIndexTemp[cbCurrentIndex]==0)
		{
			ChiCardInfo[cbChiCardCount].cbCenterCard=cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbResultCount=cbResultCount;
			ChiCardInfo[cbChiCardCount].cbCardData[0][0]=cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][1]=cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][2]=SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount++].cbChiKind=((cbCurrentCard&MASK_COLOR)==0x00)?CK_XXD:CK_XDD;
		}
	}

	//大小搭吃
	if (cbCardIndex[cbReverseIndex]==2)
	{
		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//删除扑克
		cbCardIndexTemp[cbReverseIndex]-=2;

		//提取判断
		BYTE cbResultCount=1;
		while (cbCardIndexTemp[cbCurrentIndex]>0)
		{
			PBYTE pcbResult=ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
			if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,pcbResult)!=CK_NULL) cbResultCount++;
			else break;
		}

		//设置结果
		if (cbCardIndexTemp[cbCurrentIndex]==0)
		{
			ChiCardInfo[cbChiCardCount].cbCenterCard=cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbResultCount=cbResultCount;
			ChiCardInfo[cbChiCardCount].cbCardData[0][0]=cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][1]=SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount].cbCardData[0][2]=SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount++].cbChiKind=((cbCurrentCard&MASK_COLOR)==0x00)?CK_XDD:CK_XXD;
		}
	}

	//二七十吃
	BYTE bCardValue=cbCurrentCard&MASK_VALUE;
	if ((bCardValue==0x02)||(bCardValue==0x07)||(bCardValue==0x0A))
	{
		//变量定义
		BYTE cbExcursion[]={1,6,9};
		BYTE cbInceptIndex=((cbCurrentCard&MASK_COLOR)==0x00)?0:10;

		//类型判断
		for (BYTE i=0;i<CountArray(cbExcursion);i++)
		{
			BYTE cbIndex=cbInceptIndex+cbExcursion[i];
			if ((cbIndex!=cbCurrentIndex)&&((cbCardIndex[cbIndex]==0)||(cbCardIndex[cbIndex]==3))) break;
		}

		//提取判断
		if (i==CountArray(cbExcursion))
		{
			//构造扑克
			BYTE cbCardIndexTemp[MAX_INDEX];
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

			//删除扑克
			for (BYTE j=0;j<CountArray(cbExcursion);j++)
			{
				BYTE cbIndex=cbInceptIndex+cbExcursion[j];
				if (cbIndex!=cbCurrentIndex) cbCardIndexTemp[cbIndex]--;
			}

			//提取判断
			BYTE cbResultCount=1;
			while (cbCardIndexTemp[cbCurrentIndex]>0)
			{
				PBYTE pcbResult=ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
				if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,pcbResult)!=CK_NULL) cbResultCount++;
				else break;
			}

			//设置结果
			if (cbCardIndexTemp[cbCurrentIndex]==0)
			{
				ChiCardInfo[cbChiCardCount].cbChiKind=CK_EQS;
				ChiCardInfo[cbChiCardCount].cbCenterCard=cbCurrentCard;
				ChiCardInfo[cbChiCardCount].cbResultCount=cbResultCount;
				ChiCardInfo[cbChiCardCount].cbCardData[0][0]=SwitchToCardData(cbInceptIndex+cbExcursion[0]);
				ChiCardInfo[cbChiCardCount].cbCardData[0][1]=SwitchToCardData(cbInceptIndex+cbExcursion[1]);
				ChiCardInfo[cbChiCardCount++].cbCardData[0][2]=SwitchToCardData(cbInceptIndex+cbExcursion[2]);
			}
		}
	}

	//顺子类型
	BYTE cbExcursion[3]={0,1,2};
	for (BYTE i=0;i<CountArray(cbExcursion);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%10;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=7))
		{
			//索引定义
			BYTE cbFirstIndex=cbCurrentIndex-cbExcursion[i];

			//吃牌判断
			for (BYTE j=0;j<3;j++)
			{
				BYTE cbIndex=cbFirstIndex+j;
				if ((cbIndex!=cbCurrentIndex)&&((cbCardIndex[cbIndex]==0)||(cbCardIndex[cbIndex]==3))) break;
			}

			//提取判断
			if (j==CountArray(cbExcursion))
			{
				//构造扑克
				BYTE cbCardIndexTemp[MAX_INDEX];
				CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

				//删除扑克
				for (BYTE j=0;j<3;j++)
				{
					BYTE cbIndex=cbFirstIndex+j;
					if (cbIndex!=cbCurrentIndex) cbCardIndexTemp[cbIndex]--;
				}

				//提取判断
				BYTE cbResultCount=1;
				while (cbCardIndexTemp[cbCurrentIndex]>0)
				{
					PBYTE pcbResult=ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
					if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,pcbResult)!=CK_NULL) cbResultCount++;
					else break;
				}

				//设置结果
				if (cbCardIndexTemp[cbCurrentIndex]==0)
				{
					BYTE cbChiKind[3]={CK_LEFT,CK_CENTER,CK_RIGHT};
					ChiCardInfo[cbChiCardCount].cbChiKind=cbChiKind[i];
					ChiCardInfo[cbChiCardCount].cbCenterCard=cbCurrentCard;
					ChiCardInfo[cbChiCardCount].cbResultCount=cbResultCount;
					ChiCardInfo[cbChiCardCount].cbCardData[0][0]=SwitchToCardData(cbFirstIndex);
					ChiCardInfo[cbChiCardCount].cbCardData[0][1]=SwitchToCardData(cbFirstIndex+1);
					ChiCardInfo[cbChiCardCount++].cbCardData[0][2]=SwitchToCardData(cbFirstIndex+2);
				}
			}
		}
	}

	return cbChiCardCount;
}

//是否吃牌
bool CGameLogic::IsChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//效验扑克
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return false;

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//插入扑克
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//提取判断
	while (cbCardIndexTemp[cbCurrentIndex]>0)
	{
		BYTE cbResult[3];
		if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,cbResult)==CK_NULL) break;
	}

	return (cbCardIndexTemp[cbCurrentIndex]==0);
}

//是否提跑
bool CGameLogic::IsTiPaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//效验扑克
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return false;

	//转换索引
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//碰牌判断
	return (cbCardIndex[cbCurrentIndex]==3)?true:false;
}

//是否偎碰
bool CGameLogic::IsWeiPengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//效验扑克
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return false;

	//转换索引
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//跑偎判断
	return (cbCardIndex[cbCurrentIndex]==2)?true:false;
}

//胡牌结果
bool CGameLogic::GetHuCardInfo(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbHuXiWeave, tagHuCardInfo & HuCardInfo)
{
	//变量定义
	static CAnalyseItemArray AnalyseItemArray;	

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//设置结果
	ZeroMemory(&HuCardInfo,sizeof(HuCardInfo));	

	//提取碰牌
	if ((cbCurrentCard!=0)&&(IsWeiPengCard(cbCardIndexTemp,cbCurrentCard)==true))
	{	
		//判断胡牌	
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
		AnalyseItemArray.RemoveAll();
		if (AnalyseCard(cbCardIndexTemp,AnalyseItemArray)==true)
		{			
			//寻找最优
			BYTE cbHuXiCard=0;
			INT_PTR nBestItem=-1;
			for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

				//胡息分析
				if (pAnalyseItem->cbHuXiCount>=cbHuXiCard)
				{
					nBestItem=i;
					cbHuXiCard=pAnalyseItem->cbHuXiCount;
				}
			}
			HuCardInfo.cbHuXiCount+=cbHuXiCard;

			//设置结果
			if (nBestItem>=0)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[nBestItem];

				//设置变量
				HuCardInfo.cbCardEye=pAnalyseItem->cbCardEye;

				//设置组合
				for (BYTE i=0;i<pAnalyseItem->cbWeaveCount;i++)
				{
					BYTE cbIndex=HuCardInfo.cbWeaveCount++;
					HuCardInfo.WeaveItemArray[cbIndex]=pAnalyseItem->WeaveItemArray[i];
				}
			}

			if((HuCardInfo.cbHuXiCount+cbHuXiWeave)>=15)
				return true;				
		}

		//构造组合
		BYTE cbIndex=HuCardInfo.cbWeaveCount++;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardCount=3;
		HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind=ACK_PENG;
		HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard=cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0]=cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1]=cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2]=cbCurrentCard;	

		//删除扑克
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]=0;

		//设置胡息
		HuCardInfo.cbHuXiCount+=GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
	}
	else if (cbCurrentCard!=0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//提取三牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndexTemp[i]==3)
		{
			//设置扑克
			cbCardIndexTemp[i]=0;

			//设置组合
			BYTE cbCardData=SwitchToCardData(i);
			BYTE cbIndex=HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardCount=3;
			HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind=ACK_WEI;
			HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard=cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0]=cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1]=cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2]=cbCardData;

			//设置胡息
			HuCardInfo.cbHuXiCount+=GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		}
	}

	//分析扑克
	AnalyseItemArray.RemoveAll();
	if (AnalyseCard(cbCardIndexTemp,AnalyseItemArray)==false) return false;

	//寻找最优
	BYTE cbHuXiCard=0;
	INT_PTR nBestItem=-1;
	for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
	{
		//获取子项
		tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

		//胡息分析
		if (pAnalyseItem->cbHuXiCount>=cbHuXiCard)
		{
			nBestItem=i;
			cbHuXiCard=pAnalyseItem->cbHuXiCount;
		}
	}
	HuCardInfo.cbHuXiCount+=cbHuXiCard;

	//设置结果
	if (nBestItem>=0)
	{
		//获取子项
		tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[nBestItem];

		//设置变量
		HuCardInfo.cbCardEye=pAnalyseItem->cbCardEye;

		//设置组合
		for (BYTE i=0;i<pAnalyseItem->cbWeaveCount;i++)
		{
			BYTE cbIndex=HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex]=pAnalyseItem->WeaveItemArray[i];
		}
	}

	return ((HuCardInfo.cbHuXiCount+cbHuXiWeave)>=15);
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return ((cbValue>=1)&&(cbValue<=10)&&(cbColor<=1));
}

//扑克数目
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//数目统计
	BYTE cbCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCount+=cbCardIndex[i];

	return cbCount;
}

//获取胡息
BYTE CGameLogic::GetWeaveHuXi(const tagWeaveItem & WeaveItem)
{
	//计算胡息
	switch (WeaveItem.cbWeaveKind)
	{
	case ACK_TI:	//提
		{
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?12:9;
		}
	case ACK_PAO:	//跑
		{
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?9:6;
		}
	case ACK_WEI:	//偎
		{
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?6:3;
		}
	case ACK_PENG:	//碰
		{
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?3:1;
		}
	case ACK_CHI:	//吃
		{
			//获取数值
			BYTE cbValue1=WeaveItem.cbCardList[0]&MASK_VALUE;
			BYTE cbValue2=WeaveItem.cbCardList[1]&MASK_VALUE;
			BYTE cbValue3=WeaveItem.cbCardList[2]&MASK_VALUE;

			//一二三吃
			if ((cbValue1==1)&&(cbValue2==2)&&(cbValue3==3)) return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?6:3;

			//二七十吃
			if ((cbValue1==2)&&(cbValue2==7)&&(cbValue3==10)) return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?6:3;

			return 0;
		}
	}

	return 0;
}

//分析扑克
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], CAnalyseItemArray & AnalyseItemArray)
{
	//变量定义
	BYTE cbWeaveItemCount=0;
	tagWeaveItem WeaveItem[76];
	ZeroMemory(WeaveItem,sizeof(WeaveItem));

	//数目统计
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	//数目判断
	if (cbCardCount==0) return true;
	if ((cbCardCount%3!=0)&&((cbCardCount+1)%3!=0)) return false;

	//需求计算
	BYTE bLessWeavItem=cbCardCount/3;
	bool bNeedCardEye=((cbCardCount+1)%3==0);

	//单吊判断
	if ((bLessWeavItem==0)&&(bNeedCardEye==true))	
	{
		//牌眼判断
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if (cbCardIndex[i]==2)
			{
				//变量定义
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//设置结果
				AnalyseItem.cbHuXiCount=0;
				AnalyseItem.cbWeaveCount=0;
				AnalyseItem.cbCardEye=SwitchToCardData(i);

				//插入结果
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
			
		}
		

		return false;
	}

	//拆分处理
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//分析过虑
		if (cbCardIndex[i]==0) continue;

		//变量定义
		BYTE cbCardData=SwitchToCardData(i);

		//大小搭吃
		if ((cbCardIndex[i]==2)&&(cbCardIndex[(i+10)%MAX_INDEX]>=1))
		{
			WeaveItem[cbWeaveItemCount].cbCardCount=3;
			WeaveItem[cbWeaveItemCount].cbWeaveKind=ACK_CHI;
			WeaveItem[cbWeaveItemCount].cbCenterCard=cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[0]=cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[1]=cbCardData;
			WeaveItem[cbWeaveItemCount++].cbCardList[2]=(cbCardData+16)%32;
		}

		//大小搭吃
		if ((cbCardIndex[i]>=1)&&(cbCardIndex[(i+10)%MAX_INDEX]==2))
		{
			WeaveItem[cbWeaveItemCount].cbCardCount=3;
			WeaveItem[cbWeaveItemCount].cbWeaveKind=ACK_CHI;
			WeaveItem[cbWeaveItemCount].cbCenterCard=cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[0]=cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[1]=(cbCardData+16)%32;
			WeaveItem[cbWeaveItemCount++].cbCardList[2]=(cbCardData+16)%32;
		}

		//二七十吃
		if ((cbCardData&MASK_VALUE)==0x02)
		{
			for (BYTE j=1;j<=cbCardIndex[i];j++)
			{
				if ((cbCardIndex[i+5]>=j)&&(cbCardIndex[i+8]>=j))
				{
					WeaveItem[cbWeaveItemCount].cbCardCount=3;
					WeaveItem[cbWeaveItemCount].cbWeaveKind=ACK_CHI;
					WeaveItem[cbWeaveItemCount].cbCenterCard=cbCardData;
					WeaveItem[cbWeaveItemCount].cbCardList[0]=cbCardData;
					WeaveItem[cbWeaveItemCount].cbCardList[1]=cbCardData+5;
					WeaveItem[cbWeaveItemCount++].cbCardList[2]=cbCardData+8;
				}
			}
		}

		//顺子判断
		if ((i<(MAX_INDEX-2))&&(cbCardIndex[i]>0)&&((i%10)<=7))
		{
			for (BYTE j=1;j<=cbCardIndex[i];j++)
			{
				if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
				{
					WeaveItem[cbWeaveItemCount].cbCardCount=3;
					WeaveItem[cbWeaveItemCount].cbWeaveKind=ACK_CHI;
					WeaveItem[cbWeaveItemCount].cbCenterCard=cbCardData;
					WeaveItem[cbWeaveItemCount].cbCardList[0]=cbCardData;
					WeaveItem[cbWeaveItemCount].cbCardList[1]=cbCardData+1;
					WeaveItem[cbWeaveItemCount++].cbCardList[2]=cbCardData+2;
				}
			}
		}
	}

	//组合分析
	if (cbWeaveItemCount>=bLessWeavItem)	
	{
		//变量定义
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//变量定义
		BYTE cbIndex[7]={0,1,2,3,4,5,6};
		tagWeaveItem * pWeaveItem[CountArray(cbIndex)];

		//开始组合
		do
		{
			//设置变量
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<bLessWeavItem;i++) pWeaveItem[i]=&WeaveItem[cbIndex[i]];

			//数量判断
			bool bEnoughCard=true;
			for (BYTE i=0;i<bLessWeavItem*3;i++)
			{
				//存在判断
				BYTE cbIndex=SwitchToCardIndex(pWeaveItem[i/3]->cbCardList[i%3]);
				if (cbCardIndexTemp[cbIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else cbCardIndexTemp[cbIndex]--;
			}

			//胡牌判断
			if (bEnoughCard==true)
			{
				//牌眼判断
				BYTE cbCardEye=0;
				if (bNeedCardEye==true)
				{
					for (BYTE i=0;i<MAX_INDEX;i++)
					{
						if (cbCardIndexTemp[i]==2)
						{
							cbCardEye=SwitchToCardData(i);
							break;
						}
					}
				}

				//组合类型
				if ((bNeedCardEye==false)||(cbCardEye!=0))
				{
					//变量定义
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//设置结果
					AnalyseItem.cbHuXiCount=0;
					AnalyseItem.cbCardEye=cbCardEye;
					AnalyseItem.cbWeaveCount=bLessWeavItem;

					//设置组合
					for (BYTE i=0;i<bLessWeavItem;i++)
					{
						AnalyseItem.WeaveItemArray[i]=*pWeaveItem[i];
						AnalyseItem.cbHuXiCount+=GetWeaveHuXi(AnalyseItem.WeaveItemArray[i]);
					}

					//插入结果
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//设置索引
			if (cbIndex[bLessWeavItem-1]==(cbWeaveItemCount-1))
			{
				for (BYTE i=bLessWeavItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<bLessWeavItem;j++) cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0) break;
			}
			else cbIndex[bLessWeavItem-1]++;
			
		} while (true);

		return (AnalyseItemArray.GetCount()>0);
	}

	return false;
}

//提取吃牌
BYTE CGameLogic::TakeOutChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbResultCard[3])
{
	//效验扑克
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return 0;

	//变量定义
	BYTE cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//大小搭吃
	BYTE cbReverseIndex=(cbCurrentIndex+10)%MAX_INDEX;
	if ((cbCardIndex[cbCurrentIndex]>=2)&&(cbCardIndex[cbReverseIndex]>=1)&&(cbCardIndex[cbReverseIndex]!=3))
	{
		//删除扑克
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbReverseIndex]--;

		//设置结果
		cbResultCard[0]=cbCurrentCard;
		cbResultCard[1]=cbCurrentCard;
		cbResultCard[2]=SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR)==0x00)?CK_XXD:CK_XDD;
	}

	//大小搭吃
	if (cbCardIndex[cbReverseIndex]==2)
	{
		//删除扑克
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbReverseIndex]-=2;

		//设置结果
		cbResultCard[0]=cbCurrentCard;
		cbResultCard[1]=SwitchToCardData(cbReverseIndex);
		cbResultCard[2]=SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR)==0x00)?CK_XDD:CK_XXD;
	}

	//二七十吃
	BYTE bCardValue=(cbCurrentCard&MASK_VALUE);
	if ((bCardValue==0x02)||(bCardValue==0x07)||(bCardValue==0x0A))
	{
		//变量定义
		BYTE cbExcursion[]={1,6,9};
		BYTE cbInceptIndex=((cbCurrentCard&MASK_COLOR)==0x00)?0:10;

		//类型判断
		for (BYTE i=0;i<CountArray(cbExcursion);i++)
		{
			BYTE cbIndex=cbInceptIndex+cbExcursion[i];
			if ((cbCardIndex[cbIndex]==0)||(cbCardIndex[cbIndex]==3)) break;
		}

		//成功判断
		if (i==CountArray(cbExcursion))
		{
			//删除扑克
			cbCardIndex[cbInceptIndex+cbExcursion[0]]--;
			cbCardIndex[cbInceptIndex+cbExcursion[1]]--;
			cbCardIndex[cbInceptIndex+cbExcursion[2]]--;

			//设置结果
			cbResultCard[0]=SwitchToCardData(cbInceptIndex+cbExcursion[0]);
			cbResultCard[1]=SwitchToCardData(cbInceptIndex+cbExcursion[1]);
			cbResultCard[2]=SwitchToCardData(cbInceptIndex+cbExcursion[2]);

			return CK_EQS;
		}
	}

	//顺子判断
	BYTE cbExcursion[3]={0,1,2};
	for (BYTE i=0;i<CountArray(cbExcursion);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%10;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=7))
		{
			//吃牌判断
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];
			if ((cbCardIndex[cbFirstIndex]==0)||(cbCardIndex[cbFirstIndex]==3)) continue;
			if ((cbCardIndex[cbFirstIndex+1]==0)||(cbCardIndex[cbFirstIndex+1]==3)) continue;
			if ((cbCardIndex[cbFirstIndex+2]==0)||(cbCardIndex[cbFirstIndex+2]==3)) continue;

			//删除扑克
			cbCardIndex[cbFirstIndex]--;
			cbCardIndex[cbFirstIndex+1]--;
			cbCardIndex[cbFirstIndex+2]--;

			//设置结果
			cbResultCard[0]=SwitchToCardData(cbFirstIndex);
			cbResultCard[1]=SwitchToCardData(cbFirstIndex+1);
			cbResultCard[2]=SwitchToCardData(cbFirstIndex+2);

			BYTE cbChiKind[3]={CK_LEFT,CK_CENTER,CK_RIGHT};
			return cbChiKind[i];
		}
	}

	return CK_NULL;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<MAX_INDEX);
	return ((cbCardIndex/10)<<4)|(cbCardIndex%10+1);
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*10+(cbCardData&MASK_VALUE)-1;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[], BYTE bMaxCount)
{
	//转换扑克
	BYTE bPosition=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbIndex=(i%2)*10+i/2;
		if (cbCardIndex[cbIndex]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[cbIndex];j++)
			{
				ASSERT(bPosition<bMaxCount);
				cbCardData[bPosition++]=SwitchToCardData(cbIndex);
			}
		}
	}

	return bPosition;
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE bCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//设置变量
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//转换扑克
	for (BYTE i=0;i<bCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return bCardCount;
}

//////////////////////////////////////////////////////////////////////////
