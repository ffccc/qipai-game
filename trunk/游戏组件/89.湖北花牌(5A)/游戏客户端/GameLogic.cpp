#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x01,0x01,0x11,0x11,               //一--0
	0x02,0x02,0x02,0x02,0x02,               //二--1
	0x03,0x03,0x03,0x13,0x13,               //三--2
	0x04,0x04,0x04,0x04,0x04,               //四--3
	0x05,0x05,0x05,0x15,0x15,               //五--4
	0x06,0x06,0x06,0x06,0x06,               //六--5
	0x07,0x07,0x07,0x17,0x17,               //七--6
	0x08,0x08,0x08,0x08,0x08,               //八--7
	0x09,0x09,0x09,0x19,0x19,				//九--8
	0x0A,0x0A,0x0A,0x0A,0x0A,               //十--9
	
	0x21,0x21,0x21,0x21,0x21,               //孔--10
	0x22,0x22,0x22,0x22,0x22,               //己--11

	0x23,0x23,0x23,0x23,0x23,               //上--12
	0x24,0x24,0x24,0x24,0x24,               //大--13
	0x25,0x25,0x25,0x25,0x25,               //人--14

	0x26,0x26,0x26,0x26,0x26,               //土--15
	0x27,0x27,0x27,0x27,0x27,               //化--16
	0x28,0x28,0x28,0x28,0x28,               //千--17

	0x29,0x29,0x29,0x29,0x29,               //可--18
	0x2A,0x2A,0x2A,0x2A,0x2A,               //知--19
	0x2B,0x2B,0x2B,0x2B,0x2B,               //礼--20

	0x2C,0x2C,0x2C,0x2C,0x2C,               //子--21
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

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//效验扑克
	ASSERT(IsValidCard(cbRemoveCard));

	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	ASSERT(cbCardIndex[cbRemoveIndex]>0);

	//删除扑克
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
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//删除扑克
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		//删除扑克
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


//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{

	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//如果颜色是0||1，牌值必须是0--A(10)
	//如果颜色是2，牌值必须是0--C(12)
	return ( ((cbColor == 0 || cbColor == 1) && (cbValue>0 && cbValue<=10) ) || ( (cbColor == 2) && (cbValue>=1 && cbValue<=12)));
}

BYTE CGameLogic::GetHuCardWeave(BYTE cbWeaveKind, BYTE cbCenterCardIndex, BYTE cbCardBuffer[],BYTE cbHuaCount)
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case WIK_DUI:		//对牌操作
		{
			//根据花牌的个数，设置变量
			switch(cbHuaCount)
			{
			case 0:        //0张花牌,全部存白皮的
				{
					cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);

					break;
				}
			case 1:  //1张花牌
				{
					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);

					break;
				}
			case 2:    //2张花牌
				{
					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);

					break;
				}
			}
			return 3;
		}
	case WIK_WU_TONG:        //五藏
	case WIK_SI_ZHAO:		//四招操作
		{
			switch(cbHuaCount)
			{
			case 0:  //0张花牌///////////////////2.26/////////////////
				{
					cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[3]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[4]=SwitchToCardData(cbCenterCardIndex);

					break;
				}
			case 1:
			case 2:
				{

					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[3]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[4]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					break;
				}				
			}
			return 5;
		}
	case WIK_SI_TONG:       //四藏操作
	case WIK_SAN_ZHAO:		//三招操作
		{

			switch(cbHuaCount)
			{
			case 0:       //0张花牌	
				{
					cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);
					cbCardBuffer[3]=SwitchToCardData(cbCenterCardIndex);

					break;
				}
			case 1:       //1张花牌
				{
					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[3]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					
					break;
				}
			case 2:    //2张花牌
				{
					cbCardBuffer[0]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[1]=0x10|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[2]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);
					cbCardBuffer[3]=0x00|(SwitchToCardData(cbCenterCardIndex)&MASK_VALUE);

					break;
				}
			}
			return 4;
		}
	case WIK_QI_TA:             //三连的组合,暂时就将所有的牌都作为白皮的求出来
		{
			//0--9之间的数字牌组合
			if (cbCenterCardIndex<(10-2))
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex+1);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+2);
			}
			//孔乙已
			if (cbCenterCardIndex==10)
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(0);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+1);
			}
			//上大人
			if( cbCenterCardIndex ==12 )
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex+1);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+2);
			}
			//七十土
			if (cbCenterCardIndex==15)
			{
				cbCardBuffer[0]=SwitchToCardData(6);
				cbCardBuffer[1]=SwitchToCardData(9);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);
			}
			//化三千
			if (cbCenterCardIndex==16)
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(2);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+1);
			}
			//可知礼
			if (cbCenterCardIndex==18)
			{
				cbCardBuffer[0]=SwitchToCardData(cbCenterCardIndex);
				cbCardBuffer[1]=SwitchToCardData(cbCenterCardIndex+1);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex+2);
			}
			//八九子
			if (cbCenterCardIndex==21)
			{
				cbCardBuffer[0]=SwitchToCardData(7);
				cbCardBuffer[1]=SwitchToCardData(8);
				cbCardBuffer[2]=SwitchToCardData(cbCenterCardIndex);
			}
			return 3;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//获取组合
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[],BYTE cbHuaCount)
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case WIK_DUI:		//对牌操作
		{
			//根据花牌的个数，设置变量
			switch(cbHuaCount)
			{
			case 0:        //0张花牌,全部存白皮的
				{
					cbCardBuffer[0]=cbCenterCard;
					cbCardBuffer[1]=cbCenterCard;
					cbCardBuffer[2]=cbCenterCard;

					break;
				}
			case 1:  //1张花牌
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);

					break;
				}
			case 2:    //2张花牌
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);

					break;
				}
			}
			return 3;
		}
	case WIK_SI_ZHAO:		//四招操作
		{
			//设置变量
			switch(cbHuaCount)
			{
			case 0:        //0张花牌
				{
					cbCardBuffer[0]=cbCenterCard;
					cbCardBuffer[1]=cbCenterCard;
					cbCardBuffer[2]=cbCenterCard;
					cbCardBuffer[3]=cbCenterCard;
					cbCardBuffer[4]=cbCenterCard;
				
					break;
				}	
			case 2:    //2张花牌
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[3]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[4]=0x00|(cbCenterCard&MASK_VALUE);

					break;
				}
			}
			return 5;
		}
	case WIK_SAN_ZHAO:		//三招操作
		{
			switch(cbHuaCount)
			{
			case 0:        //0张花牌
				{
					//设置变量
					cbCardBuffer[0]=cbCenterCard;
					cbCardBuffer[1]=cbCenterCard;
					cbCardBuffer[2]=cbCenterCard;
					cbCardBuffer[3]=cbCenterCard;
					
					break;
				}
			case 1:       //1张花牌
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[3]=0x00|(cbCenterCard&MASK_VALUE);
					
					break;
				}
			case 2:    //2张花牌
				{
					cbCardBuffer[0]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[1]=0x10|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[2]=0x00|(cbCenterCard&MASK_VALUE);
					cbCardBuffer[3]=0x00|(cbCenterCard&MASK_VALUE);

					break;
				}
			}
			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

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

////////////////////babala//////////
//分析玩家手中牌数组中是否可以藏牌
BYTE CGameLogic::AnalyseSiTong(BYTE cbCardIndex[MAX_INDEX],  tagSiTongResult & SiTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&SiTongResult,sizeof(SiTongResult));
	//bool bSiTong = false;

	//分析索引数组中是否有四张相同的,如果有得到四统的操作
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i] == 4 || cbCardIndex[i] == 5) 
		{
			SiTongResult.cbCardData[SiTongResult.cbCardCount++] = SwitchToCardData(i);
			cbActionMask |=WIK_SI_TONG;
			//bSiTong = true;
		}
	}
	return cbActionMask;
}

///////////////babala  11_15////////////////////////////////////
void CGameLogic::SaveHuaCard(BYTE cbCardData[],BYTE bCardCount,BYTE cbHuaCardIndex[])
{
	//找花牌
	for (BYTE i=0; i<bCardCount; i++)
	{
		//如果是花牌，保存到花牌索引数组中
		if ((cbCardData[i]&MASK_COLOR)==0x10)
		{
			BYTE index = SwitchToCardIndex(cbCardData[i]);
			cbHuaCardIndex[index/2]++;
		}
	}
}

//分析玩家手中牌数组中是否可以藏牌，五统
BYTE CGameLogic::AnalyseWuTong(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,tagWuTongResult & WuTongResult)
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

//扑克转换成牌数据
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT((cbCardIndex >=0) && (cbCardIndex<22));

	if((cbCardIndex >= 0)&&(cbCardIndex<10))
	{
		return 0x00|(cbCardIndex+1);
	}
	else
	{
		return 0x20|(cbCardIndex-9);
	}
}

//扑克转换成牌点数
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));

	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//如果是数字，直接得到最后一位值
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}

//扑克转换,
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT], BYTE cbHuaCardIndex[])
{
	//BYTE cbTempIndex[5];
	///CopyMemory(cbTempIndex,cbHuaCardIndex,sizeof(cbHuaCardIndex));
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
				for (k=0; k<(cbCardIndex[i]-cbHuaCardIndex[i/2]); k++)
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
//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[], BYTE cbCardCount, BYTE cbCardData[], BYTE cbHuaCardIndex[])
{
	for(BYTE cbCount=0;cbCount<cbCardCount;cbCount++)
	{
		BYTE cbIndex=cbCardIndex[cbCount];
		//如果是经牌，特殊处理
		if (cbIndex==0 || cbIndex==2 || cbIndex==4 || cbIndex==6 || cbIndex==8 )
		{
			//如果此精牌的花牌个数不为0，则先转换成花牌
			if(cbHuaCardIndex[cbIndex/2]>0)
			{
				cbCardData[cbCount]=0x10|(cbIndex+1);
				cbHuaCardIndex[cbIndex/2]--;
			}
			else
			{
				cbCardData[cbCount]=0x00|(cbIndex+1);
			}
		}
		else
		{
			cbCardData[cbCount]=SwitchToCardData(cbIndex);
		}
	}
	return cbCardCount;
}
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
//判断加入了待分析的牌后是否构成丫口
bool CGameLogic:: IsYaKou(BYTE cbCardIndex[MAX_INDEX],BYTE &FirstIndex,BYTE &SecondeIndex)
{
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//统计数组中的个数要为2
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndexTemp[i];

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
	//判断是不是丫口，
	//1.如果两张牌都是数字的情况下，相减绝对值必须<=2  单独考虑七，十，
	if (FirstIndex < 10 && SecondeIndex <10)
	{
		//特例，七，十
		if ((FirstIndex == 6 && SecondeIndex == 9) || (FirstIndex == 9 && SecondeIndex == 6 ) ) return true;

		//其他情况按数字组合算,相减绝对值必须<=2 
		if( abs(FirstIndex-SecondeIndex) <= 2 ) return true;
	}
	//2.有数字和字的情况下
	//孔,乙，己
	if ((FirstIndex == 10) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;
	if ((FirstIndex == 0) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;
	if ((FirstIndex == 11) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;

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

	return false;


}
