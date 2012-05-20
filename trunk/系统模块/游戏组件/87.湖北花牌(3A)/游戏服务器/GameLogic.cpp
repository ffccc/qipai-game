#include "StdAfx.h"
#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x01,0x01,0x11,0x11,               //һ
	0x02,0x02,0x02,0x02,0x02,               //��
	0x03,0x03,0x03,0x13,0x13,               //��
	0x04,0x04,0x04,0x04,0x04,               //��
	0x05,0x05,0x05,0x15,0x15,               //��
	0x06,0x06,0x06,0x06,0x06,               //��
	0x07,0x07,0x07,0x17,0x17,               //��
	0x08,0x08,0x08,0x08,0x08,               //��
	0x09,0x09,0x09,0x19,0x19,				//��
	0x0A,0x0A,0x0A,0x0A,0x0A,               //ʮ
	
	0x21,0x21,0x21,0x21,0x21,               //��
	0x22,0x22,0x22,0x22,0x22,               //��

	0x23,0x23,0x23,0x23,0x23,               //��
	0x24,0x24,0x24,0x24,0x24,               //��
	0x25,0x25,0x25,0x25,0x25,               //��

	0x26,0x26,0x26,0x26,0x26,               //��
	0x27,0x27,0x27,0x27,0x27,               //��
	0x28,0x28,0x28,0x28,0x28,               //ǧ

	0x29,0x29,0x29,0x29,0x29,               //��
	0x2A,0x2A,0x2A,0x2A,0x2A,               //֪
	0x2B,0x2B,0x2B,0x2B,0x2B,               //��

	0x2C,0x2C,0x2C,0x2C,0x2C,               //��
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

//�����˿�
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{

	//����׼��
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	srand((unsigned)time(NULL));
	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{

	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//�����ɫ��0||1����ֵ������0--A(10)
	//�����ɫ��2����ֵ������0--C(12)
	return ( ((cbColor == 0 || cbColor == 1) && (cbValue>0 && cbValue<=10) ) || ( (cbColor == 2) && (cbValue>=1 && cbValue<=12)));
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[])
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_DUI:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_SI_ZHAO:		//���в���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;
			cbCardBuffer[4]=cbCenterCard;

			return 5;
		}
	case WIK_SAN_ZHAO:		//���в���
		{
			//���ñ���
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
//����һ���˿ˣ������˿˵�ֵ�õ��˿��Ƶȼ�
BYTE CGameLogic::GetCardRank(BYTE cbCardData)
{
	//���Դ����������ǺϷ���
	ASSERT(IsValidCard(cbCardData));

	if (IsValidCard(cbCardData) == false) return -1;
	//1.�ϣ��ɣ��ѣ�ǧ������3
	if (cbCardData == 0x23 || cbCardData == 0x29 || cbCardData == 0x22 || cbCardData == 0x28) return 3;
	//2.�ף��ˣ��񣬷���5
	if (cbCardData == 0x21 || cbCardData == 0x25 || cbCardData == 0x2B) return 5;
	//3.������֪���ӷ��� 7
	if (cbCardData == 0x24 || cbCardData == 0x26 || cbCardData == 0x2A || cbCardData == 0x2C) return 7;
	//4.����������ƣ�ֱ�ӵõ���ֵ
	else 
		return cbCardData&0x0F;

}
//�����ȼ�
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//���Ƶȼ�
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//�����Ƶȼ�
	if (cbUserAction&(WIK_SAN_ZHAO |WIK_SI_ZHAO)) { return 3; }

	//���Ƶȼ�
	if (cbUserAction&WIK_DUI) { return 2; }
	return 0;
}
///////////////babala  11_15////////////////////////////////////
void CGameLogic::SaveHuaCard(BYTE cbCardData[],BYTE bCardCount,BYTE cbHuaCardIndex[])
{
	//�һ���
	for (BYTE i=0; i<bCardCount; i++)
	{
		//����ǻ��ƣ����浽��������������
		if ((cbCardData[i]&MASK_COLOR) == 0x10)
		{
			BYTE index = SwitchToCardIndex(cbCardData[i]);
			cbHuaCardIndex[index/2]++;
			ASSERT(cbHuaCardIndex[index/2]<=2);
		}
	}
}

/////////////////////babala///////////////////
//�ж��Ƿ���Զ���,�������������������һ����,���ضԲ�����
BYTE CGameLogic::EstimateDuiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbSiTong[],BYTE cbSiTongCount,BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	BYTE cbIndex = SwitchToCardIndex(cbCurrentCard);

	//������������Ѿ��ڵ�ǰ��ҵ��Ĳ�������,���������з���
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

	//�����ж�
	return (cbCardIndex[cbIndex]>=2)?WIK_DUI:WIK_NULL;
}

//�ж�����
BYTE CGameLogic::EstimateZhaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbSiTong[],BYTE cbSiTongCount, BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));
	BYTE cbActionMask=WIK_NULL;
	
	BYTE cbIndex = SwitchToCardIndex(cbCurrentCard);
	bool bSanZhao = true;
	//������������Ѿ��ڵ�ǰ��ҵ��Ĳ�������,���������з���
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
//�����˿������ݵõ����Ʊ��浽������,���ػ��Ƶĸ���
BYTE CGameLogic:: GetHuaCard(BYTE cbCardData[],BYTE bCardCount, BYTE cbHuaCardIndex[])
{
	BYTE bHuaCardCount =0;
	//ѭ���������������еõ�����
	for (BYTE i=0; i<bCardCount; i++)
	{
		//����Ч��
		ASSERT(IsValidCard(cbCardData[i]));

		//����ǻ��ƣ�������ת����������ŵ���������������
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
//��������������������Ƿ���Բ��ƣ���ͳ
BYTE CGameLogic::AnalyseSiTong(BYTE cbCardIndex[MAX_INDEX],  tagTongResult &SiTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&SiTongResult,sizeof(SiTongResult));

	//���������������Ƿ���������ͬ��,����еõ���ͳ�Ĳ���
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

//��������������������Ƿ���Բ��ƣ���ͳ
BYTE CGameLogic::AnalyseWuTong(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,tagTongResult &WuTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&WuTongResult,sizeof(WuTongResult));

	//���������������Ƿ���������ͬ��,����еõ���ͳ�Ĳ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i] == 5) 
		{
			WuTongResult.cbCardData[WuTongResult.cbCardCount++] = SwitchToCardData(i);
			cbActionMask |=WIK_WU_TONG;
		}
	}

	//������в��Ʒ���
	for (BYTE j=0; j<cbItemCount; j++)
	{
		BYTE cbWeaveKind = WeaveItem[j].cbWeaveKind;
		BYTE cbCenterCardIndex = SwitchToCardIndex( WeaveItem[j].cbCenterCard );
		//Ҫ��������ж�Ӧ���Ʋ���Ϊ���У����������ƶ�Ӧ����λ�õ�����Ϊ1
		if ((cbCardIndex[cbCenterCardIndex] == 1) && (cbWeaveKind == WIK_SAN_ZHAO))
		{
			WuTongResult.cbCardData[WuTongResult.cbCardCount++] = SwitchToCardData(cbCenterCardIndex);
			cbActionMask |= WIK_WU_TONG;
		}
	}
	return cbActionMask;

}
/////////babala 11_25
//����������еĲ���,���������������������Ѿ������ţ�������ʾ����
BYTE CGameLogic::AnalyseWeaveTong(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,tagTongResult &WeaveTongResult)
{
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&WeaveTongResult,sizeof(WeaveTongResult));

	for (BYTE i=0; i<MAX_INDEX; i++)
	{
		//�����Ӧ�����������е�����Ϊ1���ж�������и����Ƿ�����,����ǣ������Ӧλ�õ�����
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
//����һ���������ͺͲ����������������õ������Ƶ�����,���ز��������ĸ���
BYTE CGameLogic::GetWeaveIndex(BYTE cbWeaveKind,BYTE cbCenterCardIndex,BYTE cbCardIndex[])
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_DUI:		//���Ʋ���
		{
			//���ñ���
			cbCardIndex[0]=cbCenterCardIndex;
			cbCardIndex[1]=cbCenterCardIndex;
			cbCardIndex[2]=cbCenterCardIndex;

			return 3;
		}
	case WIK_QI_TA:   //�������
		{
			//0--9֮������������
			if (cbCenterCardIndex<(10-2))
			{
				cbCardIndex[0]=cbCenterCardIndex;
				cbCardIndex[1]=cbCenterCardIndex+1;
				cbCardIndex[2]=cbCenterCardIndex+2;
			}
			//������:��
			if (cbCenterCardIndex==10)
			{
				cbCardIndex[0]=cbCenterCardIndex;
				cbCardIndex[1]=0;
				cbCardIndex[2]=cbCenterCardIndex+1;
			}
			//�ϴ���:��
			if( cbCenterCardIndex ==12 )
			{
				cbCardIndex[0]=cbCenterCardIndex;
				cbCardIndex[1]=cbCenterCardIndex+1;
				cbCardIndex[2]=cbCenterCardIndex+2;
			}
			//��ʮ��:��
			if (cbCenterCardIndex==15)
			{
				cbCardIndex[0]=6;
				cbCardIndex[1]=9;
				cbCardIndex[2]=cbCenterCardIndex;
			}
			//����ǧ:��
			if (cbCenterCardIndex==16)
			{
				cbCardIndex[0]=cbCenterCardIndex;
				cbCardIndex[1]=2;
				cbCardIndex[2]=cbCenterCardIndex+1;
			}
			//��֪��
			if (cbCenterCardIndex==18)
			{
				cbCardIndex[0]=(cbCenterCardIndex);
				cbCardIndex[1]=(cbCenterCardIndex+1);
				cbCardIndex[2]=(cbCenterCardIndex+2);
			}
			//�˾���
			if (cbCenterCardIndex==21)
			{
				cbCardIndex[0]=(7);
				cbCardIndex[1]=(8);
				cbCardIndex[2]=(cbCenterCardIndex);
			}

			return 3;
		}
	case WIK_SI_ZHAO:		//���в���
		{
			//���ñ���
			cbCardIndex[0]=cbCenterCardIndex;
			cbCardIndex[1]=cbCenterCardIndex;
			cbCardIndex[2]=cbCenterCardIndex;
			cbCardIndex[3]=cbCenterCardIndex;
			cbCardIndex[4]=cbCenterCardIndex;

			return 5;
		}
	case WIK_SAN_ZHAO:		//���в���
		{
			//���ñ���
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
//У������,����һ�������ݣ���һ��������
bool CGameLogic::CheckHuPoint(BYTE cbCardData,BYTE cbCardIndex)
{
	//ת���˿�����
	if (cbCardData !=0)
	{
		if (SwitchToCardIndex(cbCardData) == cbCardIndex) return true;
		return false;
	}
	return false;
}

//��������
BYTE CGameLogic::AnalyseHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount,BYTE cbCurrentCard,BYTE HuaCardInex[], BYTE WeavHuaIndex[], tagHuCardResult & ChiHuResult)
{
	CAnalyseItemArray AnalyseItemArray;
	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//������ʱ���鱣������������
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//��������������л��Ƶĸ���
	BYTE cbTempHuaCardIndex[5];
	BYTE cbTempWeaveHuaIndex[5];
	CopyMemory(cbTempHuaCardIndex,HuaCardInex,sizeof(cbTempHuaCardIndex));
	CopyMemory(cbTempWeaveHuaIndex,WeavHuaIndex,sizeof(cbTempWeaveHuaIndex));

	//����ǰ�Ƽ�������������������
	if (cbCurrentCard != 0) 
	{
		BYTE CardIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardIndexTemp[CardIndex]++;

		//�����ǰ���ǻ��ƣ����浽���л�������������
		if ((cbCurrentCard & 0xF0) == 0x10)	cbTempHuaCardIndex[CardIndex/2]++;
	}

	//�õ����и����Ƶĸ���
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

	//�Ժ�����Ͻ��з������ó���ѵĺ������
	if (AnalyseItemArray.GetCount()>0)
	{
		//�ö�̬����
		tagAnalyseItem BestAnalyseItem;
		ZeroMemory(&BestAnalyseItem,sizeof(BestAnalyseItem));

		//��һ����ÿһ�Ϸ����Ƶ����
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��ĳһ����Ͻ��з���
			tagAnalyseItem *pAnalyseItem=&AnalyseItemArray[i];

			BYTE cbMaxPoint = 0;
			BYTE cbMaxPointGoldCard = 255;
			//��ÿһ����Ϸֱ�������1,3,5,7,9��Ϊ����ó������������
			for (BYTE j=0; j<3; j++)
			{
				//�õ�����������ֵ 
				BYTE RealGold = (j*2)+2;   //��ǰ�ļ�������
				WORD WeavePoint=0;     //�û�����Ƶ��ܵ���
				WORD HandPoint=0;      //�û����е��ܵ���
				
				//ÿ�����±������о��ƺͻ��Ƶĸ�����
				BYTE cbGoldCard[3];      //�û������Ƶĸ����Ƹ���
				BYTE cbHuaCardIndex[5];  //�û������Ƶĸ����Ƹ���
				CopyMemory(cbGoldCard,cbHandGoldCard,sizeof(cbGoldCard));
				CopyMemory(cbHuaCardIndex,cbTempHuaCardIndex,sizeof(cbHuaCardIndex));

				//1 �����û�������еķ���
				//////////////////////�����õ��Ļ�������Ӧ����������еĻ���///////////////////
				BYTE cbItemPoint;   //����ʱ�ã����ڼ�¼ÿһ��ɵĵ���
				ZeroMemory(pAnalyseItem->cbKindPoint,sizeof(pAnalyseItem->cbKindPoint));
				for(BYTE m=0;m<cbItemCount;m++)
				{
					//�õ�����Ƶ�����������
					cbItemPoint=0;
					BYTE index = pAnalyseItem->cbCenterCard[m];
					//�������:1.�ǽ���,2.��ͨ��
					if (pAnalyseItem->cbWeaveKind[m]== WIK_SI_ZHAO)
					{
						//1.�Ǿ���1,3,5,7,9,��Ϊ:1.1����,1.2������
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							if(index == 0 || index == 8)
							{
									cbItemPoint = 6;
									WeavePoint += 6;
							}
							else
							{
								//1.��������+56
								if (index == RealGold) 
								{
									cbItemPoint = 56;
									WeavePoint += 56;
								}
								//��������+28
								else  
								{
									cbItemPoint = 28;
									WeavePoint += 28;
								}
							}
						}
						//2.���Ǿ��Ƶ���ͨ�Ʒ�:2.1����,2.2����
						else
						{
							//2.1����,ֻ����,��,��,��,֪,���������+8
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								WeavePoint += 8;
								cbItemPoint = 8;
							}
							//2.2����+4
							else
							{
								WeavePoint += 4;
								cbItemPoint = 4;
							}
						}
					}
					//�������:1.�ǽ���,2.��ͨ��
					else if (pAnalyseItem->cbWeaveKind[m] == WIK_SAN_ZHAO)
					{
						//1,3,5,7,9�����
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							if(index == 0 || index == 8)
							{
								switch(cbTempWeaveHuaIndex[index/2])
								{
								case 1:   //һ�Ż��Ƶ����
									{
										cbItemPoint = 3;
										WeavePoint += 3;
										break;
									}
								case 2:		//���Ż��Ƶ����
									{
										cbItemPoint = 4;
										WeavePoint += 4;
										break;
									}
								}
							}
							else
							{
								//1.�������Ƹ��ݻ��Ƹ����������
								if (index == RealGold)
								{
									//���ݻ��Ƹ������м���
									switch(cbTempWeaveHuaIndex[index/2])
									{
									case 1:    //һ�Ż��Ƶ����
										{
											cbItemPoint = 24;
											WeavePoint += 24;
											break;
										}
									case 2:      //���Ż������
										{
											cbItemPoint = 28;
											WeavePoint += 28;
											break;
										}
									}
								}					
					
								//���������,���ݻ��Ƶĸ����������
								else
								{
									//���ݻ��Ƹ������м���
									switch(cbTempWeaveHuaIndex[index/2])
									{
									case 1:    //һ�Ż��Ƶ����
										{
											cbItemPoint = 12;
											WeavePoint += 12;
											break;
										}
									case 2:      //���Ż������
										{
											cbItemPoint = 14;
											WeavePoint += 14;
											break;
										}
									}
								}
							}
						}
						//��ͨ��2.1������4��,2.2������2��
						else
						{
							//2.1����,ֻ����,��,��,��,֪,���������+4
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								WeavePoint += 4;
								cbItemPoint = 4;
							}
							//2.2����+2
							else 
							{
								WeavePoint += 2;
								cbItemPoint = 2;
							}
						}
					}
					//�������:1.�ǽ���,2.��ͨ��
					else if (pAnalyseItem->cbWeaveKind[m] == WIK_DUI)
					{
						//1,3,5,7,9�����
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							if(index == 0 || index == 8)
							{
								//���ݻ��Ƹ������м���
								switch(cbTempWeaveHuaIndex[index/2])
								{
								case 0:    //0�Ż��Ƶ����
									{
										cbItemPoint = 0;
										WeavePoint += 0;
										
										break;
									}
								case 1:      //1�Ż������
									{
										cbItemPoint = 1;
										WeavePoint += 1;
										break;
									}
								case 2:      //2�Ż������
									{
										cbItemPoint = 2;
										WeavePoint += 2;
										break;
									}
								}

							}
							else
							{
								//1.�������Ƹ��ݻ��Ƹ����������
								if (index == RealGold)
								{
									//���ݻ��Ƹ������м���
									switch(cbTempWeaveHuaIndex[index/2])
									{
									case 0:    //0�Ż��Ƶ����
										{
											cbItemPoint = 6;
											WeavePoint += 6;
											break;
										}
									case 1:      //1�Ż������
										{
											cbItemPoint = 8;
											WeavePoint += 8;
											break;
										}
									case 2:      //2�Ż������
										{
											cbItemPoint = 10;
											WeavePoint += 10;
											break;
										}
									}
								}
								//////////////���������������///////////////////
								//2.���������
								else
								{
									//���ݻ��Ƹ������м���
									switch(cbTempWeaveHuaIndex[index/2])
									{
									case 0:    //0�Ż��Ƶ����
										{
											cbItemPoint = 3;
											WeavePoint += 3;
											
											break;
										}
									case 1:      //1�Ż������
										{
											cbItemPoint = 4;
											WeavePoint += 4;
											break;
										}
									case 2:      //2�Ż������
										{
											cbItemPoint = 5;
											WeavePoint += 5;
											break;
										}
									}
								}
							}
						}
						//��ͨ��2.1������1��,2.2���Ʋ����
						else
						{
							//2.1����,ֻ����,��,��,��,֪,���������+1
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								WeavePoint += 1;
								cbItemPoint = 1;
							}
							//2.2����+0
							else 
							{
								WeavePoint += 0;
								cbItemPoint = 0;
							}
						}
					}
					
					pAnalyseItem->cbKindPoint[m] = cbItemPoint;
				}

				//2 �����û����п��Ƶ����е���
				for(BYTE k=cbItemCount; k<8;k++)
				{
					cbItemPoint=0;
					//ȡÿ����������ÿС�������������
					BYTE index = pAnalyseItem->cbCenterCard[k];
			
					//������:1.1�ǽ���,1.2.��ͨ��
					if (pAnalyseItem->cbWeaveKind[k] == WIK_WU_TONG)
					{
						//1.�ǽ���1,3,5,7,9,��Ϊ:1.1����,1.2������
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							cbHuaCardIndex[index/2] = 0;  //���д˻��Ƹ���Ϊ0
							
							if(index == 0 || index == 8)
							{
								HandPoint += 6;
								cbItemPoint = 6;
							}
							else
							{
								cbGoldCard[(index-2)/2] -= 5;     //���д˾��Ƹ���Ϊ0
							
								//1.1����
								if (index == RealGold)
								{
									//��Ӧλ�õĻ�������Ϊ0
									HandPoint += 56;
									cbItemPoint = 56;
								}
								//1.2������Ľ���
								else
								{
									//��Ӧλ�õĻ�������Ϊ0
									HandPoint += 28;
									cbItemPoint = 28;
								}
							}
						}
						//2.��ͨ��(������):������8����������4��
						else
						{
							//2.1����,ֻ����,��,��,��,֪,���������+8
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								HandPoint += 8;
								cbItemPoint = 8;
							}
							//2.2����+4
							else 
							{
								HandPoint += 4;
								cbItemPoint = 4;							
							}
						}
					}
					//�Ĳ����:1.�ǽ���,2.��ͨ��
					else if (pAnalyseItem->cbWeaveKind[k] == WIK_SI_TONG)
					{
						//1.�ǽ���1,3,5,7,9�����
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
						
							if(index == 0 || index == 8)
							{
									//���ݻ��Ƹ������м���
								switch(cbHuaCardIndex[index/2])
								{
								case 1:    //һ�Ż��Ƶ����
									{
										cbItemPoint = 3;
										HandPoint += 3;
										cbHuaCardIndex[index/2] -= 1;

										break;
									}
								case 2:      //���Ż������
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
								//1.��������,���ݻ��Ƹ����������
								if (index == RealGold)
								{
									//���ݻ��Ƹ������м���
									switch(cbHuaCardIndex[index/2])
									{
									case 1:    //һ�Ż��Ƶ����
										{
											cbItemPoint = 24;
											HandPoint += 24;
											cbHuaCardIndex[index/2] -= 1;

											break;
										}
									case 2:      //���Ż������
										{
											cbItemPoint = 28;
											HandPoint += 28;
											cbHuaCardIndex[index/2] -= 2;
											break;
										}
									}
								}
								//���������
								else
								{
									//���ݻ��Ƹ������м���
									switch(cbHuaCardIndex[index/2])
									{
									case 1:    //һ�Ż��Ƶ����
										{
											cbItemPoint = 12;
											HandPoint += 12;
											cbHuaCardIndex[index/2] -= 1;

											break;
										}
									case 2:      //���Ż������
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
						//��ͨ��2.1������4��,2.2������2��
						else
						{
							//2.1����,ֻ����,��,��,��,֪,���������+4
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 ) 
							{
								cbItemPoint = 4;
								HandPoint += 4;
							}
							//2.2����+2
							else
							{
								cbItemPoint = 2;
								HandPoint += 2;
							}
						}
					}
					//������ͬ�ģ�1.�ǽ���,2.��ͨ��
					else if (pAnalyseItem->cbWeaveKind[k] == WIK_DUI)
					{
						//�ǽ���:1,3,5,7,9�����
						if( index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
						{
							if(index == 0 || index == 8)
							{
								//���ݻ��Ƹ������м���
								switch(cbHuaCardIndex[index/2])
								{
								case 0:    //0�Ż��Ƶ����
									{
										cbItemPoint = 1;
										HandPoint += 1;
										cbHuaCardIndex[index/2] -= 0;

										break;
									}
								case 1:      //1�Ż������
									{
										cbItemPoint = 2;
										HandPoint += 2;
										cbHuaCardIndex[index/2] -= 1;
										break;
									}
								case 2:      //2�Ż������
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
								//1.��������,���ݻ��Ƹ����������
								if (index == RealGold)
								{
									//���ݻ��Ƹ������м���
									switch(cbHuaCardIndex[index/2])
									{
									case 0:    //0�Ż��Ƶ����
										{
											cbItemPoint = 10;
											HandPoint += 10;
											cbHuaCardIndex[index/2] -= 0;

											break;
										}
									case 1:      //1�Ż������
										{
											cbItemPoint = 12;
											HandPoint += 12;
											cbHuaCardIndex[index/2] -= 1;
											break;
										}
									case 2:      //2�Ż������
										{
											cbItemPoint = 14;
											HandPoint += 14;
											cbHuaCardIndex[index/2] -= 2;
											break;
										}
									}
								}
								//1.2������Ļ���
								else
								{
									//���ݻ��Ƹ������м���
									switch(cbHuaCardIndex[index/2])
									{
									case 0:    //0�Ż��Ƶ����
										{
											cbItemPoint = 5;
											HandPoint += 5;
											cbHuaCardIndex[index/2] -= 0;

											break;
										}
									case 1:      //1�Ż������
										{
											cbItemPoint = 6;
											HandPoint += 6;
											cbHuaCardIndex[index/2] -= 1;
											break;
										}
									case 2:      //2�Ż������
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
						//2.��ͨ��
						else
						{
							//2.1����,ֻ����,��,��,��,֪,���������+2
							if (index == 12 || index == 13 || index == 14 ||index == 18 || index == 19 || index == 20 )
							{
								HandPoint += 2;
								cbItemPoint = 2;
							}
							//2.2����+1
							else 
							{
								HandPoint += 1;
								cbItemPoint = 1;
							}
						}
					}

					//�����ж�,ֻͳ���ϴ��ˣ���֪��ĸ�����ʣ�µľ��Ƹ���
					else if (pAnalyseItem->cbWeaveKind[k] == WIK_QI_TA)
					{
						//�ϴ���/��֪����� +1
						if (index == 12 || index == 18) 
						{
							HandPoint += 1;
							cbItemPoint = 1;
						}
					}
					pAnalyseItem->cbKindPoint[k] = cbItemPoint;
				}
				
				//3 ��ʣ�µĽ��Ƶĸ���
				for (BYTE t=0; t<3; t++)
				{
					//���㻨�Ƶĵ���
					if (cbHuaCardIndex[t+1] > 0)
					{
						//��ȥ�����л��Ƶĸ���
						cbGoldCard[t] -= cbHuaCardIndex[t+1];
						//�������
						if (((t*2)+2) == RealGold)
						{
							HandPoint += cbHuaCardIndex[t+1]*4;
						}
						else
						{
							HandPoint += cbHuaCardIndex[t+1]*2;
						}
					}
					//�����Ƥ���Ƶĵ���
					if (cbGoldCard[t] > 0)
					{
						//���������Ƥ��*2
						if (((t*2)+2) == RealGold)
						{
							HandPoint += cbGoldCard[t]*2;
						}
						//������,��Ƥ��*1
						else
						{
							HandPoint += cbGoldCard[t]*1;
						}
					}
				}

				//4 ��ʣ�µĻ��Ƶĸ���
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

				//У�������Ƶĺ���:�����������У����˴����һ�������Լ��������γ�����һ���ģ�ֻ�ܽ��䵱�����µ��������
				//��������ÿһ����Ͻ���ת�����ж��������Ƿ�������
				if (cbCurrentCard != 0) 
				{
					//�õ���������
					BYTE cbCurrentCardIndex = SwitchToCardIndex(cbCurrentCard);
					//������Ѿ���ж�,�������������Ѿ���У�������У��
					if ((cbCurrentCardIndex != pAnalyseItem->cbCardEye[0]) && (cbCurrentCardIndex != pAnalyseItem->cbCardEye[1]))
					{
						bool Qi_Ta = false;
						//�������˳������Ƿ��ھ�����
						for (BYTE l=cbItemCount; l<8; l++)
						{
							//�õ�WIK_QI_TA������͵������������ж��Ƿ�����������
							if (pAnalyseItem->cbWeaveKind[l] == WIK_QI_TA)
							{
								BYTE cbWeaveIndex[5]={-1,-1,-1,-1,-1};
								BYTE cbWeaveCount = GetWeaveIndex(pAnalyseItem->cbWeaveKind[l],pAnalyseItem->cbCenterCard[l],cbWeaveIndex);
								//�ж����������Ƿ���WIK_QI_TA�����У�����ڣ�ֱ����������У��
								for (BYTE n=0; n<cbWeaveCount; n++)
								{
									if (cbCurrentCardIndex == cbWeaveIndex[n]) 
									{
										Qi_Ta = true;
										break;
									}
								}

								//����ھ������ҵ��˱��������������ѭ��
								if (Qi_Ta == true) break;
							}
							//�õ�WIK_SI_TONG��������е������������ж����������Ƿ��������У�����ǣ����ܺ���
							else if (pAnalyseItem->cbWeaveKind[l] == WIK_SI_TONG || pAnalyseItem->cbWeaveKind[l] == WIK_WU_TONG)
							{
								if (cbCurrentCardIndex ==  pAnalyseItem->cbCenterCard[i]) 
								{
									//AfxMessageBox("�Բ�����Ѻ�����Ϸ����ó������˴�������γ����ŵĲ��ܺ���!");
									return WIK_NULL;	
								}	
							}

						}

						//����ھ�����û���ҵ����˴���ƣ��ٵ���������
						if (Qi_Ta == false)
						{
							for (BYTE i=cbItemCount; i<8; i++)
							{
								//WIK_DUI
								if (pAnalyseItem->cbWeaveKind[i] == WIK_DUI)
								{
									//������ж�������������ͬ�����к���У��
									if (cbCurrentCardIndex ==  pAnalyseItem->cbCenterCard[i]) 
									{
										//����������ǽ��Ʒ֣�1.����2.��ͨ����
										if (cbCurrentCardIndex == 2 || cbCurrentCardIndex == 4 || cbCurrentCardIndex == 6)
										{
											//1.�����4��
											if (cbCurrentCardIndex == RealGold) 
											{
												HandPoint -= 4;  //HuPoint-=4;
												pAnalyseItem->cbKindPoint[i] -=4;
											}
											//2.��ͨ���Ƽ�2��
											else
											{
												HandPoint -= 2;  //HuPoint -=2;
												pAnalyseItem->cbKindPoint[i] -=2;
											}
										}
										//��ͨ�Ʒ�,�ܺ�����1
										else
										{
											HandPoint -= 1;
											pAnalyseItem->cbKindPoint[i] -=1;
											//HuPoint -=1;
										}
										//������ɺ󣬽���ѭ��
										break;
									}
								}

							}
						}
					}
				}
				
				//�Ըþ�����Ϊ�����Ƿ����ǰ��ĵ���
				//���汾��������ĵ��������Ӧ��������
				if((WeavePoint+HandPoint)>cbMaxPoint)
				{
					cbMaxPoint=WeavePoint+HandPoint;
					cbMaxPointGoldCard=RealGold;
				}
				//�����ñ�ѭ���м���ĵ���������
				pAnalyseItem->cbPoint = WeavePoint+HandPoint;
				pAnalyseItem->cbGoldCard = RealGold;
#ifdef _DEBUG
		        m_Debug.PrintValidKindItemPointAfterMessage(pAnalyseItem,(BYTE)WeavePoint,(BYTE)HandPoint);
#endif
			}
			//���������ñ�������������ĵ��������Ӧ��������
			pAnalyseItem->cbPoint = cbMaxPoint;
			pAnalyseItem->cbGoldCard = cbMaxPointGoldCard;
		}

		//�ڴ˼���һ������ĺ����ж�--7�����
//���������Ƿ���7���������
//1�������û��������е�����,û��357�Ľ�,û�л��Һͻ���;
//2�������û������ϵ�������Ҫ�������Ժ�ɫ�ֵ��ƣ������Ժ�ɫ�ֵ�����һ����
//3���û����������ż����ϵ������,�������������������ż����ϵ�����,Ҫ��һ����ɫ���Ƶ�һ��,����������
//4������������������պ���7����.

		for (INT_PTR t=0;t<AnalyseItemArray.GetCount();t++)
		{
			tagAnalyseItem *pAnalyseItem=&AnalyseItemArray[t];

//if(pAnalyseItem->cbPoint<10)
//{
//	CString str;
//	str.Format("���ڵĵ���Ϊ:%d",pAnalyseItem->cbPoint);
//	AfxMessageBox(str);
//}

			if(pAnalyseItem->cbPoint+1 == 7)
			{
				bool bHaveHuGold=true;    //�û��������У��Ƿ�û��357�Ľ�,û�л��Һͻ���,Ĭ������
				BYTE cbBlackDuiCount=0;   //�û�������������,�Ƿ����������ϵĺڶ��ƣ�Ĭ��Ϊ0
				BYTE cbReadDuiCount=0;     //�û�����������ͬ�ĺ��ƻ��������еĺ���

				//1����������û��357�Ľ�û�л��Һͻ���
				BYTE cbUserHuaCardIndex[5];          //�����û����л����ĸ���
				BYTE cbUserGoldCardIndex[5];         //�����û����о��Ƶĸ���

				ZeroMemory(cbUserHuaCardIndex,sizeof(cbUserHuaCardIndex));
				ZeroMemory(cbUserGoldCardIndex,sizeof(cbUserGoldCardIndex));

				//���û��������е�����ͳ�Ƶ�������
				for(BYTE i=0;i<5;i++)
				{
					cbUserGoldCardIndex[i]=cbCardIndexTemp[i*2];
					cbUserHuaCardIndex[i]=cbTempHuaCardIndex[i];
				}

				//���û�������е�����ͳ�Ƶ�������
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

				//�û�û��357��
				if(cbUserGoldCardIndex[1]==0 && cbUserGoldCardIndex[2]==0 && cbUserGoldCardIndex[3]==0)
				{
					//�û�û�л�1�ͻ�9
					if(cbUserHuaCardIndex[0]==0 && cbUserHuaCardIndex[4]==0)
					{
						bHaveHuGold=false;   //�����һ��:û��357�Ľ�,û�л��Һͻ���
					}
				}

				//ͳ���û�������к�ɫ�Ķ�����--�û������ϵĶ�����,�ڶ��Ƶ���Ŀ
				for(BYTE i=0;i<cbItemCount;i++)
				{
					if(pAnalyseItem->cbWeaveKind[i] == WIK_DUI)
					{
						BYTE index = pAnalyseItem->cbCenterCard[i];
						//��ɫ��ֻ���ϴ���[12,13,14]����֪��[18,19,20]��������[2,4,6]
						if(index!=12 && index!=13 && index!=14 && index!=18 && index!=19 && index!=20 && index!=2 && index!=4 && index!=6)
						{
							cbBlackDuiCount++;  //����ڶ���û�����Ķ�����,�ڶ��Ƶ���Ŀ
						}
					}
				}

				//ͳ���û����еĶԡ������еĺ������Ŀ,�������Ϻ�����ϵ���Ŀ
				//�û������еĺ����л��������Ŀ
				for(BYTE i=0;i<cbItemCount;i++)
				{
					BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[i];
					if(cbWeaveKind==WIK_SAN_ZHAO || cbWeaveKind==WIK_SI_ZHAO)
					{
						BYTE index = pAnalyseItem->cbCenterCard[i];
						//��ɫ��ֻ���ϴ���[12,13,14]����֪��[18,19,20]��������[2,4,6]
						if(index==12 || index==13 || index==14 || index==18 || index==19 || index==20 || index==2 || index==4 || index==6)
						{
							cbReadDuiCount++;
						}
					}
				}
				//�û����к�ԡ����Ĳء�����ص���Ŀ
				for(BYTE i=cbItemCount;i<9;i++)
				{
					BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[i];
					if(cbWeaveKind==WIK_DUI || cbWeaveKind==WIK_SI_TONG || cbWeaveKind==WIK_WU_TONG)
					{
						BYTE index = pAnalyseItem->cbCenterCard[i];
						//��ɫ��ֻ���ϴ���[12,13,14]����֪��[18,19,20]��������[2,4,6]
						if(index==12 || index==13 || index==14 || index==18 || index==19 || index==20 || index==2 || index==4 || index==6)
						{
							cbReadDuiCount++;
						}
					}
				}

				//��������������ʱ��Ӧ�ý���6�����޸�Ϊ32��
				if(bHaveHuGold==false && cbBlackDuiCount>=3 && cbReadDuiCount>=1)
				{
					pAnalyseItem->cbPoint=32;
				}
			}
		}
			
		//�����еĺ���������ҳ������������
		INT_PTR BestIndex = 0;
		//��ÿһ��������Ͻ��бȽϣ��ҳ���Ѻ������
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			if (AnalyseItemArray[i].cbPoint > AnalyseItemArray[BestIndex].cbPoint)
			{
				BestIndex = i;
			}
		}

		BestAnalyseItem.cbPoint = AnalyseItemArray[BestIndex].cbPoint;
		BestAnalyseItem.cbGoldCard = AnalyseItemArray[BestIndex].cbGoldCard;
		//��ñ����������µĺ������,�Ա�������
		CopyMemory(&BestAnalyseItem ,&AnalyseItemArray[BestIndex],sizeof(tagAnalyseItem));

		if (BestAnalyseItem.cbPoint < 17)
		{
			return WIK_NULL;
		}

		ChiHuResult.IsHu=true;
		ChiHuResult.HuScore= 2 + ((BestAnalyseItem.cbPoint-17)/5)*2;
		ChiHuResult.bHuPoint = BestAnalyseItem.cbPoint;  //���Ƶ���
		ChiHuResult.bRealGold = BestAnalyseItem.cbGoldCard;
		CopyMemory(&ChiHuResult.AnalyseItem, &BestAnalyseItem,sizeof(BestAnalyseItem));

#ifdef _DEBUG
		m_Debug.PrintHuCardMessage(&BestAnalyseItem);
#endif
		return WIK_CHI_HU;
	}
	return WIK_NULL;
}

//�˿�ת����������
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
//�˿�����ת�����˿˵���ת���Ľ����
//  ��  ���� Ͳ
// 0-8   9-15  16-23
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{

	ASSERT(IsValidCard(cbCardData));

	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//��������֣�ֱ�ӵõ����һλֵ
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}

/////////////////////////////////////////babala 11_12

////�˿�ת��,
//�˿�ת��,
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT], BYTE cbHuaCardIndex[])
{
	//ת���˿�
	BYTE cbPosition=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			ASSERT(cbPosition<MAX_COUNT);
			//����Ǿ��ƣ����⴦��
			if (i==0 || i==2 || i==4 || i==6 || i==8 )
			{
				//�����������л��Ƹ������浽��������
				for (BYTE k=0; k<cbHuaCardIndex[i/2]; k++)
				{
					cbCardData[cbPosition++]=0x10|(i+1);
				}
				//��ʣ�µĻ���Ƥ��
				for (BYTE j=0; j<(cbCardIndex[i]-cbHuaCardIndex[i/2]); j++)
				{
					cbCardData[cbPosition++]=0x00|(i+1);
				}
			}
			//��ͨ�ƴ���
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
//	//ת���˿�
//	BYTE cbPosition=0;
//	for (BYTE i=0;i<MAX_INDEX;i++)
//	{
//		if (cbCardIndex[i]!=0)
//		{
//			for (BYTE j=0;j<cbCardIndex[i];j++)
//			{
//				ASSERT(cbPosition<MAX_COUNT);
//				//����Ǿ��ƣ����⴦��
//				if (i==0 || i==2 || i==4 || i==6 || i==8 )
//				{
//					//�����������л��Ƹ������浽��������
//					for (BYTE k=0; k<cbHuaCardIndex[i/2]; k++)
//					{
//						cbCardData[cbPosition++]=0x10|(i+1);
//					}
//					//��ʣ�µĻ���Ƥ��
//					for (k=0; k<(cbCardIndex[i]-cbHuaCardIndex[i/2]); k++)
//					{
//						cbCardData[cbPosition++]=0x00|(i+1);
//					}
//					break;
//				}
//				//��ͨ�ƴ���
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
//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//ת���˿�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));

		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

///////babala 11_16wna
//babala 11-30
//�����ֵ
BYTE  CGameLogic::Abs(BYTE cbFirst,BYTE cbSecond)
{
	return (cbFirst > cbSecond)?(cbFirst-cbSecond):(cbSecond-cbFirst);

}
//�жϼ����˴��������ƺ��Ƿ񹹳�Ѿ��
bool CGameLogic:: IsYaKou(BYTE cbCardIndex[],BYTE &FirstIndex,BYTE &SecondeIndex)
{
	BYTE cbCardIndexTemp[MAX_INDEX];
	ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

	for (BYTE i=0; i<MAX_INDEX;i++)
	{
		cbCardIndexTemp[i]= cbCardIndex[i];
	}
	//CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));


	//ͳ�������еĸ���ҪΪ2
	WORD cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndexTemp[i];

	CString sz;
	sz.Format("ʣ�µ��Ƹ���Ϊ:%d",cbCardCount);
	//AfxMessageBox(sz);

	if (cbCardCount != 2)	return false;
	

	//ASSERT(cbCardCount == 2);
	//if (cbCardCount != 2) return false;

//	BYTE FirstIndex = -1,SecondeIndex = -1;

	//Ѿ��Ϊ��仰
	for (BYTE i=0;i<MAX_INDEX;i++)
	{			
		if (cbCardIndexTemp[i] != 0)
		{
			FirstIndex = i;
			cbCardIndexTemp[i]--;
			break;
		}
	}
	//����һ����
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
	sz.Format("��һ������Ϊ��%d,�ڶ�������Ϊ:%d",FirstIndex,SecondeIndex);
	//AfxMessageBox(sz);

	//�ж��ǲ���Ѿ��
	//1.��������ƶ������ֵ�����£��������ֵ����<=2  ���������ߣ�ʮ��
	if (FirstIndex < 10 && SecondeIndex <10)
	{
		//����:�ߣ�ʮ
		if ((FirstIndex == 6 && SecondeIndex == 9) || (FirstIndex == 9 && SecondeIndex == 6 ) ) return true;

		//������������������,�������ֵ����<=2          
		if( Abs(FirstIndex,SecondeIndex) <= 2 ) return true;
	}
	else
	{
		//2.�����ֺ��ֵ������
		//��,�ң���
		if ((FirstIndex == 10) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;
		if ((FirstIndex == 0) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;
		if ((FirstIndex == 11) && (SecondeIndex == 10 || SecondeIndex == 0 || SecondeIndex == 11 )) return true;

		//�ϴ���
		if ((FirstIndex == 12) && (SecondeIndex == 12 || SecondeIndex == 13 || SecondeIndex == 14 )) return true;
		if ((FirstIndex == 13) && (SecondeIndex == 12 || SecondeIndex == 13 || SecondeIndex == 14 )) return true;
		if ((FirstIndex == 14) && (SecondeIndex == 12 || SecondeIndex == 13 || SecondeIndex == 14 )) return true;


		//��,��,ǧ
		if ((FirstIndex == 16) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;
		if ((FirstIndex == 2) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;
		if ((FirstIndex == 17) && (SecondeIndex == 16 || SecondeIndex == 2 || SecondeIndex == 17 )) return true;

		//��ʮ��
		if ((FirstIndex == 6) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;
		if ((FirstIndex == 9) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;
		if ((FirstIndex == 15) && (SecondeIndex == 6 || SecondeIndex == 9 || SecondeIndex == 15 )) return true;

		//�˾���
		if ((FirstIndex == 7) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;
		if ((FirstIndex == 8) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;
		if ((FirstIndex == 21) && (SecondeIndex == 7 || SecondeIndex == 8 || SecondeIndex == 21 )) return true;

		//��֪��
		if ((FirstIndex == 18) && (SecondeIndex == 18 || SecondeIndex == 19 || SecondeIndex == 20 )) return true;
		if ((FirstIndex == 19) && (SecondeIndex == 18 || SecondeIndex == 19 || SecondeIndex == 20 )) return true;
		if ((FirstIndex == 20) && (SecondeIndex == 18 || SecondeIndex == 19 || SecondeIndex == 20 )) return true;
	}

	return false;
}

//�����û����е��ƣ��õ����ܺ��Ƶ������Ϣ//�û������Ƶ��������� //����Ƶ�����                 //��ϸ���             //�����������
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//ͳ�����������е���������
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	//Ч����Ŀ�������������Ƶ������������������£�������һ��Ѿ��
	ASSERT(cbCardCount>=2);
	if (cbCardCount<2) return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�--�û����к��Ƶ���ȷ�����
	BYTE cbLessKindItem=8-cbWeaveCount;

	//�����жϣ�cbCardCount=2�������ԭ��������ֻ��һ���ƣ�����Ҫ�������ƺ����ù������ţ��������ƶ����������
	if (cbCardCount == 2)
	{
		BYTE FirstIndex = -1,SecondeIndex = -1;
		//���ʣ�µ�������Ѿ�ڣ��������������
		if (IsYaKou(cbCardIndex,FirstIndex,SecondeIndex) == true)
		{
			tagAnalyseItem AnalyseItem;
			ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

			//����ÿһ������ƣ��õ�����Ƶ�������ͺ��м�������,���浽����������
			for (BYTE j=0;j<cbWeaveCount;j++)
			{
				AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
				AnalyseItem.cbCenterCard[j]=SwitchToCardIndex(WeaveItem[j].cbCenterCard);
			}
			//����Ѿ��
			AnalyseItem.cbCardEye[0]=FirstIndex;
			AnalyseItem.cbCardEye[1]=SecondeIndex;
			//������������뵽����������
			AnalyseItemArray.Add(AnalyseItem);

			return true;
		}
		return false;
	}

	//������������ƺ�������>=3�������������������������з���
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//����һ������
			if(cbCardIndex[i] >= 3)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount].cbCardCount = 3;

				KindItem[cbKindItemCount++].cbWeaveKind=WIK_DUI;
			}
			//�ĸ�һ��
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
			//���һ��
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
					
			//2.�����ж�
			//0--9֮�������
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
			//������
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
			//�ϴ���
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
			//��ʮ��
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
			//����ǧ
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
			//��֪��
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
			//�˾���
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

	//��ӡ�û������Ƶ����������Ϣ
#ifdef _DEBUG
	m_Debug.PrintKindItemsMessage(KindItem,cbKindItemCount);
#endif 

	//�������е���ϣ��Ӷ��õ����ܺ��Ƶ����
	if (cbKindItemCount>=cbLessKindItem)
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[8]={0,1,2,3,4,5,6,7};
		tagKindItem * pKindItem[8];
		ZeroMemory(&pKindItem,sizeof(pKindItem));
		do
		{
			//ÿ��ѭ���������������������鿽������ʱ�����У����з���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

			//ÿ�δ���������ó��ķ���������ȡcbLessKindItem������������з�����
			//ע�⣺��������cbIndex[]��ÿ��ѭ������ʱ������������
			for (BYTE i=0;i<cbLessKindItem;i++) 
			{
				pKindItem[i]=&KindItem[cbIndex[i]];
			}
	
			//�����ж�
			bool bEnoughCard=true;
			//�޸���ʱ�����ֵ������ʱ�����й���cbLessKindItem�������������ÿһ���ƣ�������1��
			//�����Ƕ�������һ���
			for(BYTE k=0;k<cbLessKindItem;k++)
			{
				//��������ÿһ�����������з���
				for(BYTE i=0; i<pKindItem[k]->cbCardCount; i++)
				{
					//�����ж�
					BYTE cbCardIndex=pKindItem[k]->cbCardIndex[i];

					if (cbCardIndexTemp[cbCardIndex]==0)
					{
						bEnoughCard=false;
						break;
					}
					else cbCardIndexTemp[cbCardIndex]--;
				}
			}
			//�����жϣ�ע������ʹ�õ���cbCardIndexTemp[]�����Ǿ�ǰ���޸Ĺ����
			if (bEnoughCard==true)
			{
				//ͳ�������еĸ���ҪΪ2
				WORD cbCardCount=0;
				for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+= cbCardIndexTemp[i];

				//Ѿ��ֵ
				BYTE cbFirstIndex=-1,cbSecondIndex=-1;
				//��ʣ�µ��ƽ��з������ж��Ƿ���Ѿ�ڣ�
				if (IsYaKou(cbCardIndexTemp,cbFirstIndex,cbSecondIndex) == true)
				{
					ASSERT((cbFirstIndex != -1)&&(cbSecondIndex != -1));

					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//�õ�������е����ͣ����浽����������
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=SwitchToCardIndex(WeaveItem[i].cbCenterCard);
					}

					//�õ������Ƶ����ͣ����浽����������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
					}
					//��������
					AnalyseItem.cbCardEye[0]=cbFirstIndex;
					AnalyseItem.cbCardEye[1]=cbSecondIndex;

					//������������뵽����������
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//�������������������д�ŵ��Ƿ�������������±꣬ÿ��ȡ����������з���ʱ�����ǰ�����������
			//�����ŵ��±�ֵ���д�ȡ����cbIndex[cbLessKindItem-1]�����һλ��ŵ�ֵ��ó��ķ��������±���ͬ��
			//���µ����������飬��һ��ȡֵ�ͻ�ȡ�µ����
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
				//��������whileѭ��
				if (i==0) break;
			}
			else cbIndex[cbLessKindItem-1]++;
			
		} while (true);
	}
	
	//��ӡ�û������ƺϷ��������Ϣ
#ifdef _DEBUG
	m_Debug.PrintValidKindItemsMessage(AnalyseItemArray);
#endif 

	return (AnalyseItemArray.GetCount()>0);
}

//-----------------�����Ǵ�ӡ������Ϣ�Ĵ���----------------------//

CString CDebugMessage::m_strCardsMessage[22]={"��","��","��","��","��",
										      "��","��","��","��","ʮ",
										      "��","��","��","��","��",
										      "��","��","ǧ","��","֪",
										      "��","��"};
CDebugMessage::CDebugMessage(void)
{
	m_pITableFrame=NULL;
}

CDebugMessage::~CDebugMessage(void)
{
	m_pITableFrame=NULL;
}

//��ʼ����ӡ�����е��������
void CDebugMessage::InitDebug(ITableFrame  *pITableFrame)
{
	ASSERT(pITableFrame!=NULL);
	m_pITableFrame=pITableFrame;
}

//��ʼ��ӡ������Ϣ--ÿ�δ�ӡʱ��������ַ����е�����
void CDebugMessage::BeginPrint()
{
	m_strMessage.Empty();
}

//������ӡ������Ϣ--���ַ����е�����һ�δ�ӡ��ָ���Ŀͻ���
void CDebugMessage::EndPrint(WORD wChairID)
{
	if(m_pITableFrame==NULL) return;
    
	if(m_strMessage.IsEmpty() || m_strMessage.GetLength()==0) return;

	//�����û���λ�Ŷ��õ��û���Ϣ
	IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if(pIServerUserItem==NULL) return;

	m_pITableFrame->SendGameMessage(pIServerUserItem,m_strMessage,SMT_INFO);
}

//��ӡ�û����е�����Ϣ--�ڷ������ˣ��û����е��ƶ����������ķ�ʽ�洢
void CDebugMessage::PrintCardMessage(BYTE cbCardIndex[])
{
	if(m_pITableFrame==NULL) return;
	
	m_strMessage.Append("\n--��ʼ��ӡ�û����е���\n");
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
	m_strMessage.Append("\n--������ӡ�û����е���\n");
}

//��ӡ�û���ϵ�����Ϣ--�û�����Ƶ������У�����һ������һ������һ
void CDebugMessage::PrintWeaveItemsMessage(tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	if(m_pITableFrame==NULL) return;

	if(cbWeaveCount>0)
	{
		m_strMessage.Append("\n--��ʼ��ӡ�û���ϵ���\n");
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
		m_strMessage.Append("\n--������ӡ�û���ϵ���\n");
	}
	else
	{
		m_strMessage.Append("\n--Ŀǰ�û��������\n");
	}
}

//��ӡ�û������Ƶ����п��ܵ������Ϣ
void CDebugMessage::PrintKindItemsMessage(tagKindItem KindItem[],BYTE cbKindItemCount)
{
	if(m_pITableFrame==NULL) return;

	if(cbKindItemCount>0)
	{
		m_strMessage.Append("\n--��ʼ��ӡ�����ƿ��ܵ����\n");
		for (BYTE i=0;i<cbKindItemCount;i++)
		{
			for(BYTE j=0;j<KindItem[i].cbCardCount;j++)
			{
				BYTE cbCardIndex=KindItem[i].cbCardIndex[j];
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
			}
			m_strMessage.Append(TEXT(" "));
		}
		m_strMessage.Append("\n--������ӡ�����ƿ��ܵ����\n");
	}
	else
	{
		m_strMessage.Append("\n--Ŀǰ�û������������\n");
	}
}

//��ӡ�û����ܺ��Ƶ������Ϣ
void CDebugMessage::PrintValidKindItemsMessage(CAnalyseItemArray &AnalyseItemArray)
{
	if(m_pITableFrame==NULL) return;

	if(AnalyseItemArray.GetCount()>0)
	{
		m_strMessage.Append("\n--��ʼ��ӡ���ܺ��Ƶ����\n");
		for (BYTE i=0;i<AnalyseItemArray.GetCount();i++)
		{
			tagAnalyseItem *pAnalyseItem=&AnalyseItemArray[i];
			//�ȴ�ӡ8������Ƶ���Ϣ
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
					if(cbCardIndex>=0 && cbCardIndex<=7)//��ӡ���ֵ��Ӿ�
					{
						m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
						m_strMessage.Append(m_strCardsMessage[cbCardIndex+1]);
						m_strMessage.Append(m_strCardsMessage[cbCardIndex+2]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==10)//��ӡ�����ѵ��Ӿ�
					{
						m_strMessage.Append(m_strCardsMessage[10]);
						m_strMessage.Append(m_strCardsMessage[0]);
						m_strMessage.Append(m_strCardsMessage[11]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==12)//��ӡ�ϴ��˵��Ӿ�
					{
						m_strMessage.Append(m_strCardsMessage[12]);
						m_strMessage.Append(m_strCardsMessage[13]);
						m_strMessage.Append(m_strCardsMessage[14]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==15)//��ӡ��ʮ�����Ӿ�
					{
						m_strMessage.Append(m_strCardsMessage[6]);
						m_strMessage.Append(m_strCardsMessage[9]);
						m_strMessage.Append(m_strCardsMessage[15]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==16)//��ӡ����ǧ���Ӿ�
					{
						m_strMessage.Append(m_strCardsMessage[16]);
						m_strMessage.Append(m_strCardsMessage[2]);
						m_strMessage.Append(m_strCardsMessage[17]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==18)//��ӡ��֪����Ӿ�
					{
						m_strMessage.Append(m_strCardsMessage[18]);
						m_strMessage.Append(m_strCardsMessage[19]);
						m_strMessage.Append(m_strCardsMessage[20]);
						m_strMessage.Append(TEXT(" "));
					}
					else if(cbCardIndex==21)//��ӡ�˾��ӵ��Ӿ�
					{
						m_strMessage.Append(m_strCardsMessage[7]);
						m_strMessage.Append(m_strCardsMessage[8]);
						m_strMessage.Append(m_strCardsMessage[21]);
						m_strMessage.Append(TEXT(" "));
					}
					break;
				}
			}
			
			//���ӡѾ����Ϣ
			m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[0]]);
			m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[1]]);
		}
		m_strMessage.Append("\n--������ӡ���ܺ��Ƶ����\n");
	}
	else
	{
		m_strMessage.Append("\n--Ŀǰ�û����ܺ���\n");
	}
}

//��ӡ����֮ǰ�������ĵ����������Ƶ��ܵ������������ܵ���
void CDebugMessage::PrintValidKindItemPointBeforeMessage(tagAnalyseItem *pAnalyseItem,BYTE cbWeavePoint,BYTE cbHandPoint)
{
	if(m_pITableFrame==NULL) return;

	m_strMessage.Append("\n--��ʼ��ӡ����ǰ�ĵ���\n");
	//�ȴ�ӡ8������Ƶ���Ϣ
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
			if(cbCardIndex>=0 && cbCardIndex<=7)//��ӡ���ֵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+1]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+2]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==10)//��ӡ�����ѵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[10]);
				m_strMessage.Append(m_strCardsMessage[0]);
				m_strMessage.Append(m_strCardsMessage[11]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==12)//��ӡ�ϴ��˵��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[12]);
				m_strMessage.Append(m_strCardsMessage[13]);
				m_strMessage.Append(m_strCardsMessage[14]);
				strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
				m_strMessage.Append(strTemp);
			}
			else if(cbCardIndex==15)//��ӡ��ʮ�����Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[6]);
				m_strMessage.Append(m_strCardsMessage[9]);
				m_strMessage.Append(m_strCardsMessage[15]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==16)//��ӡ����ǧ���Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[16]);
				m_strMessage.Append(m_strCardsMessage[2]);
				m_strMessage.Append(m_strCardsMessage[17]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==18)//��ӡ��֪����Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[18]);
				m_strMessage.Append(m_strCardsMessage[19]);
				m_strMessage.Append(m_strCardsMessage[20]);
				strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
				m_strMessage.Append(strTemp);
			}
			else if(cbCardIndex==21)//��ӡ�˾��ӵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[7]);
				m_strMessage.Append(m_strCardsMessage[8]);
				m_strMessage.Append(m_strCardsMessage[21]);
				m_strMessage.Append(TEXT(" "));
			}
			break;
		}
	}
	//���ӡѾ����Ϣ
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[0]]);
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[1]]);
	
	//��ӡ�������ܵ���
    m_strMessage.Append(TEXT("|"));
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbGoldCard]);
	strTemp.Format("%d",cbWeavePoint+cbHandPoint);
	m_strMessage.Append(strTemp);

	strTemp.Format("\n--����Ƶ���:%d,�����Ƶ���:%d\n",cbWeavePoint,cbHandPoint);
	m_strMessage.Append(strTemp);

	m_strMessage.Append("\n--������ӡ����ǰ�ĵ���\n");
}

//��ӡ����֮��������ĵ����������Ƶ��ܵ������������ܵ���
void CDebugMessage::PrintValidKindItemPointAfterMessage(tagAnalyseItem *pAnalyseItem,BYTE cbWeavePoint,BYTE cbHandPoint)
{
	if(m_pITableFrame==NULL) return;

	m_strMessage.Append("\n--��ʼ��ӡ������ĵ���\n");
	//�ȴ�ӡ8������Ƶ���Ϣ
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
			if(cbCardIndex>=0 && cbCardIndex<=7)//��ӡ���ֵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+1]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+2]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==10)//��ӡ�����ѵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[10]);
				m_strMessage.Append(m_strCardsMessage[0]);
				m_strMessage.Append(m_strCardsMessage[11]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==12)//��ӡ�ϴ��˵��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[12]);
				m_strMessage.Append(m_strCardsMessage[13]);
				m_strMessage.Append(m_strCardsMessage[14]);
				strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
				m_strMessage.Append(strTemp);
			}
			else if(cbCardIndex==15)//��ӡ��ʮ�����Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[6]);
				m_strMessage.Append(m_strCardsMessage[9]);
				m_strMessage.Append(m_strCardsMessage[15]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==16)//��ӡ����ǧ���Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[16]);
				m_strMessage.Append(m_strCardsMessage[2]);
				m_strMessage.Append(m_strCardsMessage[17]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==18)//��ӡ��֪����Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[18]);
				m_strMessage.Append(m_strCardsMessage[19]);
				m_strMessage.Append(m_strCardsMessage[20]);
				strTemp.Format("%d ",pAnalyseItem->cbKindPoint[j]);
				m_strMessage.Append(strTemp);
			}
			else if(cbCardIndex==21)//��ӡ�˾��ӵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[7]);
				m_strMessage.Append(m_strCardsMessage[8]);
				m_strMessage.Append(m_strCardsMessage[21]);
				m_strMessage.Append(TEXT(" "));
			}
			break;
		}
	}
	//���ӡѾ����Ϣ
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[0]]);
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[1]]);
	
	//��ӡ�������ܵ���
    m_strMessage.Append(TEXT("|"));
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbGoldCard]);
	strTemp.Format("%d",cbWeavePoint+cbHandPoint);
	m_strMessage.Append(strTemp);

	strTemp.Format("\n--����Ƶ���:%d,�����Ƶ���:%d\n",cbWeavePoint,cbHandPoint);
	m_strMessage.Append(strTemp);

	m_strMessage.Append("\n--������ӡ������ĵ���\n");
}


//��ӡ��������Ϣ
void CDebugMessage::PrintHuCardMessage(tagAnalyseItem *pAnalyseItem)
{
if(m_pITableFrame==NULL) return;

	m_strMessage.Append("\n--��ʼ��ӡ��������Ϣ\n");
	//�ȴ�ӡ8������Ƶ���Ϣ
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
			if(cbCardIndex>=0 && cbCardIndex<=7)//��ӡ���ֵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[cbCardIndex]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+1]);
				m_strMessage.Append(m_strCardsMessage[cbCardIndex+2]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==10)//��ӡ�����ѵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[10]);
				m_strMessage.Append(m_strCardsMessage[0]);
				m_strMessage.Append(m_strCardsMessage[11]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==12)//��ӡ�ϴ��˵��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[12]);
				m_strMessage.Append(m_strCardsMessage[13]);
				m_strMessage.Append(m_strCardsMessage[14]);
				
			}
			else if(cbCardIndex==15)//��ӡ��ʮ�����Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[6]);
				m_strMessage.Append(m_strCardsMessage[9]);
				m_strMessage.Append(m_strCardsMessage[15]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==16)//��ӡ����ǧ���Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[16]);
				m_strMessage.Append(m_strCardsMessage[2]);
				m_strMessage.Append(m_strCardsMessage[17]);
				m_strMessage.Append(TEXT(" "));
			}
			else if(cbCardIndex==18)//��ӡ��֪����Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[18]);
				m_strMessage.Append(m_strCardsMessage[19]);
				m_strMessage.Append(m_strCardsMessage[20]);
				
			}
			else if(cbCardIndex==21)//��ӡ�˾��ӵ��Ӿ�
			{
				m_strMessage.Append(m_strCardsMessage[7]);
				m_strMessage.Append(m_strCardsMessage[8]);
				m_strMessage.Append(m_strCardsMessage[21]);
				m_strMessage.Append(TEXT(" "));
			}
			break;
		}
	}
	//���ӡѾ����Ϣ
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[0]]);
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbCardEye[1]]);
	
	//��ӡ�������ܵ���
    m_strMessage.Append(TEXT("|"));
	m_strMessage.Append(m_strCardsMessage[pAnalyseItem->cbGoldCard]);
	strTemp.Format("%d",pAnalyseItem->cbPoint);
	m_strMessage.Append(strTemp);

	m_strMessage.Append("\n--������ӡ��������Ϣ\n");
}
BYTE CDebugMessage::SwitchToCardIndex(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//��������֣�ֱ�ӵõ����һλֵ
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}