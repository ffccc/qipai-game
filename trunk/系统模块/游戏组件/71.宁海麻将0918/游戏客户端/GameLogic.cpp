#include "StdAfx.h"
#include "GameLogic.h"
#include "AndroidAI.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//����
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//����
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//����
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//����
		0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F 							//����
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

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���˿�
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//ɾ���˿�
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//ʧ��Ч��
	ASSERT(FALSE);

	return false;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//ɾ���˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//ɾ���˿�
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//�������
			ASSERT(FALSE);

			//��ԭɾ��
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//ɾ���˿�
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//��������
	ASSERT(cbCardCount<=MAX_COUNT);
	ASSERT(cbRemoveCount<=cbCardCount);

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
	if (cbCardCount>CountArray(cbTempCardData))
		return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
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

	//�ɹ��ж�
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) 
			cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>0)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=15)&&(cbColor==3)));
}

//�˿���Ŀ
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+2;

			return 3;
		}
	case WIK_RIGHT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard-2;

			return 3;
		}
	case WIK_CENTER:	//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard-1;

			return 3;
		}
	case WIK_PENG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_GANG:		//���Ʋ���
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

//�����ȼ�
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//���Ƶȼ�
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//���Ƶȼ�
	if (cbUserAction&WIK_GANG) { return 3; }

	//���Ƶȼ�
	if (cbUserAction&WIK_PENG) { return 2; }

	//���Ƶȼ�
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

	return 0;
}

//���Ƶȼ�
BYTE CGameLogic::GetChiHuActionRank(tagChiHuResult & ChiHuResult)
{
	//��������
	BYTE cbChiHuOrder=0;
	WORD wChiHuRight=ChiHuResult.wChiHuRight;
	WORD wChiHuKind=(ChiHuResult.wChiHuKind&0xFF00)>>4;

	//�������
	for (BYTE i=0;i<8;i++)
	{
		wChiHuKind>>=1;
		if ((wChiHuKind&0x0001)!=0) 
			cbChiHuOrder++;
	}

	//Ȩλ����
	for (BYTE i=0;i<16;i++)
	{
		wChiHuRight>>=1;
		if ((wChiHuRight&0x0001)!=0) 
			cbChiHuOrder++;
	}

	return cbChiHuOrder;
}

//�����ж�
BYTE CGameLogic::EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	//��������
	if (cbCurrentCard>=0x31) 
		return WIK_NULL;

	//��������
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//�����ж�
	BYTE cbEatKind=0,cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//�����ж�
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];
			if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
				continue;

			//��������
			cbEatKind|=cbItemKind[i];
		}
	}

	return cbEatKind;
}

//�����ж�
BYTE CGameLogic::EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//�����ж�
BYTE CGameLogic::EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
}

//���Ʒ���
BYTE CGameLogic::AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//���ñ���
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//���ϸ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]==4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount]=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//��ϸ���
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount]=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}

//�Ժ�����
BYTE CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult,WORD wBankerUser,bool bJugeHuOnly,bool bZimo)
{
	//��������
	WORD wChiHuKind=CHK_NULL;
	BYTE cbHuCount = 0;
	BYTE cbHuTimes=0;
	static CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//�����˿�
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	if (IsQingYiSe(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) 
		wChiHuRight|=CHR_QING_YI_SE;
	if (IsHunYiSe(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true)
		wChiHuRight|=CHR_HUN_YI_SE;

	if(true == bZimo)
	{
		wChiHuRight |=CHR_ZI_MO;
		cbHuCount  += 2;
	}

	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			bool bLianCard=false,bPengCard=false;
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//���ͷ���
			for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
			{
				BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[j];
				bPengCard=((cbWeaveKind&(WIK_GANG|WIK_PENG))!=0)?true:bPengCard;
				bLianCard=((cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianCard;
			}

			//�����ж�
			ASSERT((bLianCard==true)||(bPengCard==true));

			//��������
			if ((bLianCard==false)&&(bPengCard==true)) 
			{
				wChiHuRight|=CHR_PENG_PENG;
			}
			if ((bLianCard==true)||(bPengCard==true))
				wChiHuKind|=CHK_JI_HU;

			//��һɫ
			if (IsZiYiSe(*pAnalyseItem)==true) 
				wChiHuRight|=CHR_ZI_YI_SE;				
		}
	}
	if(wChiHuKind !=0 && bJugeHuOnly==false)
	{
		BYTE cbHuTempCount[2]={0,0};
		BYTE cbHuTempTimes[2]={0,0};
		
		//�ж�Ƕ
		if ((AnalyseItemArray.GetCount() >0)&&(cbCurrentCard !=0)) 
		{
			for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
			{
				tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

				if(pAnalyseItem->cbCardEye>=0x35 && pAnalyseItem->cbCardEye<=0x37)
				{
					cbHuTempCount[1] += 2;
				}

				if((pAnalyseItem->cbCardEye&MASK_COLOR) == 0x30)
				{
					if((pAnalyseItem->cbCardEye&MASK_VALUE)-1 == wBankerUser)
					{
						cbHuTempCount[1] += 2;
					}
				}

				//���ͷ���
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					BYTE cbWeaveKind=WeaveItem[j].cbWeaveKind;
					if(cbWeaveKind==WIK_PENG)
					{
						if((WeaveItem[j].cbCenterCard&MASK_COLOR)==0x30)
						{
							cbHuTempCount[1] +=4;
						}
						else
						{
							if((WeaveItem[j].cbCenterCard&MASK_VALUE)==0x01 ||(WeaveItem[j].cbCenterCard&MASK_VALUE)==0x09)
							{
								cbHuTempCount[1] +=4;
							}
							else
								cbHuTempCount[1] +=2;
						}
					}
					if(cbWeaveKind==WIK_GANG)
					{
						if(WeaveItem[j].cbPublicCard==FALSE)
						{
							if((WeaveItem[j].cbCenterCard&MASK_COLOR)==0x30)
							{
								cbHuTempCount[1] +=32;
							}
							else
							{
								if((WeaveItem[j].cbCenterCard&MASK_VALUE)==0x01 ||(WeaveItem[j].cbCenterCard&MASK_VALUE)==0x09)
								{
									cbHuTempCount[1] +=32;
								}
								else
									cbHuTempCount[1] +=16;
							}
						}
						if(WeaveItem[j].cbPublicCard==TRUE)
						{
							if((WeaveItem[j].cbCenterCard&MASK_COLOR)==0x30)
							{
								cbHuTempCount[1] +=16;
							}
							else
							{
								if((WeaveItem[j].cbCenterCard&MASK_VALUE)==0x01 ||(WeaveItem[j].cbCenterCard&MASK_VALUE)==0x09)
								{
									cbHuTempCount[1] +=16;
								}
								else
									cbHuTempCount[1] +=8;
							}
						}
					}
				}

				for (BYTE j=cbWeaveCount;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
				{
					if(pAnalyseItem->cbWeaveKind[j]==WIK_PENG)
					{
						if((pAnalyseItem->cbCenterCard[j]&MASK_COLOR)==0x30)
						{
							cbHuTempCount[1] += 8;
						}
						else
						{
							if((pAnalyseItem->cbCenterCard[j]&MASK_VALUE)==0x01 ||(pAnalyseItem->cbCenterCard[j]&MASK_VALUE)==0x09)
							{
								cbHuTempCount[1] += 8;
							}
							else
							{
								cbHuTempCount[1] +=4;
							}
						}
					}
	
				}

				for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
				{
					if(pAnalyseItem->cbCenterCard[j]>=0x35 && pAnalyseItem->cbCenterCard[j]<=0x37)
						cbHuTempTimes[1] += 1; 
					if(((pAnalyseItem->cbCenterCard[j]&MASK_VALUE)-1 == wBankerUser) && ((pAnalyseItem->cbCenterCard[j]&MASK_COLOR)==0x30))
						cbHuTempTimes[1] += 1;


				}
				for (BYTE j=cbWeaveCount;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
				{
					if ((pAnalyseItem->cbCardData[j][1] == cbCurrentCard)&&(pAnalyseItem->cbWeaveKind[j]== WIK_LEFT))
					{
						cbHuTempCount[1] +=2;
						break;
					}
				}
				if(bZimo==false)
				{
					for (BYTE j=cbWeaveCount;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
					{
						if(pAnalyseItem->cbCenterCard[j]!=cbCurrentCard) continue;
						if(pAnalyseItem->cbWeaveKind[j]==WIK_PENG)
						{
							if((pAnalyseItem->cbCenterCard[j]&MASK_COLOR)==0x30 && cbCardIndex[SwitchToCardIndex(cbCurrentCard)<3])
							{
								cbHuTempCount[1] -= 4;
							}
							else
							{
								if(((pAnalyseItem->cbCenterCard[j]&MASK_VALUE)==0x01 ||(pAnalyseItem->cbCenterCard[j]&MASK_VALUE)==0x09)&&( cbCardIndex[SwitchToCardIndex(cbCurrentCard)<3]))
								{
									cbHuTempCount[1] -= 4;
								}
								else
								{
									cbHuTempCount[1] -=2;
								}
							}
						}
					}

				}



				if(cbHuTempCount[0]<cbHuTempCount[1])
				{
					cbHuTempCount[0]=cbHuTempCount[1];
				}
				if(cbHuTempTimes[0]<cbHuTempTimes[1])
				{
					cbHuTempTimes[0]=cbHuTempTimes[1];
					cbHuTimes=cbHuTempTimes[0];
				}
	
			}


		}
		cbHuCount +=cbHuTempCount[0];

	}

	//����ж�
	if (wChiHuKind!=0)
	{
		//���ý��
		ChiHuResult.wChiHuKind=wChiHuKind;
		ChiHuResult.wChiHuRight=wChiHuRight;
		if(bJugeHuOnly)
		{
			ChiHuResult.cbHuCount=0;
			ChiHuResult.cbHuTimes=0;
		}
		else
		{
			ChiHuResult.cbHuCount=cbHuCount;
			ChiHuResult.cbHuTimes=cbHuTimes;
		}

		return WIK_CHI_HU;
	}

	return WIK_NULL;
}

//��һɫ��
bool CGameLogic::IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
{
	//�����ж�
	BYTE cbCardColor=0xFF;

	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			//��ɫ�ж�
			if (cbCardColor!=0xFF)
				return false;

			//���û�ɫ
			cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

			//��������
			i=(i/9+1)*9-1;
		}
	}

	//��������
	if(cbCardColor == 0x30) return false;

	//����ж�
	for (BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR)!=cbCardColor)
			return false;
	}

	return true;
}

//��һɫ��
bool CGameLogic::IsHunYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
{
	//��������
	bool bColorFlags[5];
	ZeroMemory(&bColorFlags,sizeof(bColorFlags));

	//�˿��˿�
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0) 
			bColorFlags[i/9]=true;
	}

	//����ж�
	for (BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
		bColorFlags[(cbCenterCard&MASK_COLOR)>>4]=true;
	}


	//��ɫͳ��
	BYTE cbColorCount=0;
	for (BYTE i=0;i<3;i++)
	{
		if (bColorFlags[i]==true)
			cbColorCount++;
	}
	if ((cbColorCount!=1)||(bColorFlags[3]==false)) 
		return false;

	return true;
}

//��һɫ��
bool CGameLogic::IsZiYiSe(tagAnalyseItem & AnalyseItem)
{
	//�˿��ж�
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if ((AnalyseItem.cbCenterCard[i]&MASK_COLOR)!=0x30)
			return false;
	}

	return true;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<MAX_INDEX);
	if(cbCardIndex<34)
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	else
		return (48|((cbCardIndex-34)%8+8));
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	return (cbColor>=0x03)?(cbValue+27-1):(cbColor*9+cbValue-1);
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//ת���˿�
	BYTE cbPosition=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbPosition<MAX_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}
//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT],BYTE bMaxCount)
{
	//ת���˿� ���Ʒŵ���ǰ��
	BYTE bPosition=0;

	for (BYTE i=0;i<MAX_INDEX;i++) 
	{
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(bPosition<bMaxCount);
				cbCardData[bPosition++]=SwitchToCardData(i);
			}
		}
	}

	return bPosition;
}


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

//��̧����
LONG CGameLogic::CalScore(tagChiHuResult & ChiHuResult,LONG &lTimes)
{
	WORD wChihuKind = ChiHuResult.wChiHuKind;

	WORD wChihuRight = ChiHuResult.wChiHuRight;
	if (wChihuRight&CHR_HUN_YI_SE)
	{
		lTimes += RIGHT_HUN_YI_SE;
	}
	if(wChihuRight&CHR_PENG_PENG)
	{
		lTimes +=RIGHT_PENG_PENG;
	}
	if(wChihuRight&CHR_QING_YI_SE)
	{
		lTimes += RIGHT_QING_YI_SE;
	}
	if(wChihuRight&CHR_ZI_YI_SE)
	{
		lTimes += RIGHT_ZI_YI_SE;
	}

	return lTimes;
}

//�����˿�
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//������Ŀ
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbCardIndex[i];

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))
		return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==4);

	//�����ж� 
	if (cbLessKindItem==0)
	{
		//Ч�����
		ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

		//�����ж�
		for (BYTE i=0;i<MAX_INDEX-8;i++)
		{
			if (cbCardIndex[i]==2)
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);

				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//��ַ���
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX-8;i++)
		{
			//ͬ���ж�
			if (cbCardIndex[i]>=3)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount++].cbWeaveKind=WIK_PENG;
			}
			
			//�����ж�
			if ((i<(MAX_INDEX-2-15))&&(cbCardIndex[i]>0)&&((i%9)<7))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount++].cbWeaveKind=WIK_LEFT;
					}
				}
			}
		}
	}

	//��Ϸ���
	if (cbKindItemCount>=cbLessKindItem)
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[4]={0,1,2,3};
		tagKindItem * pKindItem[4];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<cbLessKindItem;i++)
				pKindItem[i]=&KindItem[cbIndex[i]];

			//�����ж�
			bool bEnoughCard=true;
			for (BYTE i=0;i<cbLessKindItem*3;i++)
			{
				//�����ж�
				BYTE cbCardIndex=pKindItem[i/3]->cbCardIndex[i%3]; 
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else 
					cbCardIndexTemp[cbCardIndex]--;
			}

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;
				for (BYTE i=0;i<MAX_INDEX-8;i++)
				{
					if (cbCardIndexTemp[i]==2)
					{
						cbCardEye=SwitchToCardData(i);
						break;
					}
				}

				//�������
				if (cbCardEye!=0)
				{
					//��������
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));


					//�������
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
						GetWeaveCard(WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,AnalyseItem.cbCardData[i]);

					}

					//��������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						BYTE cbCenterCard = SwitchToCardData(pKindItem[i]->cbCenterCard);
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=cbCenterCard;
						GetWeaveCard(pKindItem[i]->cbWeaveKind,cbCenterCard,AnalyseItem.cbCardData[i+cbWeaveCount]);

					}

					//��������
					AnalyseItem.cbCardEye=cbCardEye;

					//������
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//��������
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				for (BYTE i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) 
							cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0)
					break;
			}
			else
				cbIndex[cbLessKindItem-1]++;
			
		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���������


//���Ʒ���
bool CGameLogic::AnalyseTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, WORD wChiHuRight)
{
	//��������
	tagChiHuResult ChiHuResult;
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//���Ʒ���
	BYTE cbHandTai=0,cbHandFeng=0,cbBankerUser=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{

		//�����ж�
		for (BYTE j=0;j<MAX_INDEX;j++)
		{
			//���Ʒ���
			BYTE cbCurrentCard=SwitchToCardData(j);
			BYTE cbHuCardKind=AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbItemCount,cbCurrentCard,wChiHuRight,ChiHuResult,
				cbBankerUser,true,true);

			//����ж�
			if (cbHuCardKind!=CHK_NULL) 
				return true;
		}

	}

	return false;
}

//����������
BYTE CGameLogic::AnalyseSingleCount( BYTE cbCardIndex[MAX_INDEX] )
{
	//������Ŀ
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
	{
		ASSERT( cbCardIndex[i]>=0 && cbCardIndex[i]<=4 );
		cbCardCount+=cbCardIndex[i];
	}

	//Ч����Ŀ
	ASSERT((cbCardCount>=1)&&(cbCardCount<=MAX_COUNT));
	if ((cbCardCount<1)||(cbCardCount>MAX_COUNT)) return 0;

	BYTE bCardIndex[MAX_INDEX];
	CopyMemory(bCardIndex,cbCardIndex,sizeof(bCardIndex));

	//��ַ���
	if (cbCardCount>=1)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{	
			//�����ж�
			if ((i<25)&&(bCardIndex[i]>0)&&(i<25)&&((i%9)<7))
			{
				BYTE bCount = 0;
				while( bCardIndex[i]>bCount && bCardIndex[i+1]>bCount && bCardIndex[i+2]>bCount )
					bCount++;
				if( bCount > 0 )
				{
					bCardIndex[i] -= bCount;
					bCardIndex[i+1] -= bCount;
					bCardIndex[i+2] -= bCount;
					cbCardCount -= 3*bCount;
				}
			}
			//ͬ���ж�
			if (bCardIndex[i]>=2)
			{
				cbCardCount -= bCardIndex[i];
				bCardIndex[i] = 0;
			}
		}
	}
	return cbCardCount;
}

//����������
bool CGameLogic::AnalyseTrushCard( BYTE cbCardIndex[MAX_INDEX],BYTE bTrushCardIndex[MAX_INDEX] )
{
	//����
	CopyMemory(bTrushCardIndex,cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//������Ŀ
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
	{
		ASSERT( cbCardIndex[i]>=0 && cbCardIndex[i]<=4 );
		cbCardCount+=cbCardIndex[i];
	}

	//Ч����Ŀ
	ASSERT((cbCardCount>=1)&&(cbCardCount<=MAX_COUNT));
	if ((cbCardCount<1)||(cbCardCount>MAX_COUNT)) return 0;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT*4];
	ZeroMemory(KindItem,sizeof(KindItem));

	//��ַ���
	if (cbCardCount>=2)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//ͬ���ж�
			BYTE cbLessSameCount = 2;			//��С��ͬ����
			if (cbCardIndex[i]>=cbLessSameCount)
			{
				ASSERT( cbKindItemCount<MAX_COUNT*4 );
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
				KindItem[cbKindItemCount++].cbCenterCard=SwitchToCardData(i);
			}
			
			//�����ж�
			if ((i<25)&&(cbCardIndex[i]>0)&&(i<25))
			{
				if( i%9 < 7 )
				{
					for (BYTE j=1;j<=cbCardIndex[i];j++)
					{
						if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j) ||
							(cbCardIndex[i+1]==j-1&&cbCardIndex[i+2]>=j) ||
							(cbCardIndex[i+1]>=j&&cbCardIndex[i+2]==j-1) 
							)
						{
							ASSERT( cbKindItemCount<MAX_COUNT*4 );
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
							KindItem[cbKindItemCount++].cbCenterCard=SwitchToCardData(i);
						}
					}
				}
				else if( (i%9)==7 && cbCardIndex[i] > 0 && cbCardIndex[i+1] > 0 && cbCardIndex[i-1]==0 )
				{
					KindItem[cbKindItemCount].cbCardIndex[0]=i-1;
					KindItem[cbKindItemCount].cbCardIndex[1]=i;
					KindItem[cbKindItemCount].cbCardIndex[2]=i+1;
					KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
					KindItem[cbKindItemCount++].cbCenterCard=SwitchToCardData(i);
				}
			}
		}
	}
	//��ȥ����ϵ���
	for( BYTE i = 0; i < cbKindItemCount; i++ )
	{
		if( bTrushCardIndex[KindItem[i].cbCardIndex[0]]>0 )
		{
			bTrushCardIndex[KindItem[i].cbCardIndex[0]]--;
			cbCardCount--;
		}
		if( bTrushCardIndex[KindItem[i].cbCardIndex[1]]>0 )
		{
			bTrushCardIndex[KindItem[i].cbCardIndex[1]]--;
			cbCardCount--;
		}
		if( bTrushCardIndex[KindItem[i].cbCardIndex[2]]>0 )
		{
			bTrushCardIndex[KindItem[i].cbCardIndex[2]]--;
			cbCardCount--;
		}
	}
	ASSERT( cbCardCount>=0 && cbCardCount<=14 );
	return cbCardCount>0;
}

//����,������ֵ����
bool CGameLogic::SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount )
{
	//��Ŀ����
	if (cbCardCount==0||cbCardCount>MAX_COUNT) return false;

	//�������
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if (cbCardData[i]>cbCardData[i+1])
			{
				//���ñ�־
				bSorted=false;

				//�˿�����
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return true;
}

//��������
bool CGameLogic::SearchTingCard(WORD wMeChairId,BYTE (&cbCardIndex)[GAME_PLAYER][MAX_INDEX],tagWeaveItem (&cWeaveItem)[GAME_PLAYER][4],BYTE cbItemCount[4],
								BYTE (&cbDiscardCard)[GAME_PLAYER][55],BYTE cbDiscardCount[GAME_PLAYER],tagTingCardResult &TingCardResult)
{
	//��������
	tagChiHuResult ChiHuResult;
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
	ZeroMemory(&TingCardResult,sizeof(TingCardResult));
	BYTE bAbandonCardCount=0;

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex[wMeChairId],sizeof(cbCardIndexTemp));

	//���Ʒ���

	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//���ƹ���
		if (cbCardIndexTemp[i]==0) continue;

		//���ƴ���
		cbCardIndexTemp[i]--;

		//�����ж�
		bool bHuCard = false;
		BYTE cbHandTai=0,cbHandFeng=0,cbBankerUser=0;
		bAbandonCardCount = TingCardResult.bAbandonCount;
		for (BYTE j=0;j<MAX_INDEX;j++)
		{
			//���Ʒ���
			BYTE cbCurrentCard=SwitchToCardData(j);
			BYTE cbHuCardKind=AnalyseChiHuCard(cbCardIndexTemp,cWeaveItem[wMeChairId],cbItemCount[wMeChairId],cbCurrentCard,0,ChiHuResult,
				cbBankerUser,true);

			//����ж�
			if (cbHuCardKind!=CHK_NULL)
			{
				bHuCard = true;
				TingCardResult.bTingCard[bAbandonCardCount][TingCardResult.bTingCardCount[bAbandonCardCount]++] = cbCurrentCard;
			}
		}
		if( bHuCard == true )
		{
			TingCardResult.bAbandonCard[TingCardResult.bAbandonCount++] = SwitchToCardData(i);
		}
		//��ԭ����
		cbCardIndexTemp[i]++;
	}

	//����ʣ����
	for( BYTE n = 0; n < TingCardResult.bAbandonCount; n++ )
	{
		TingCardResult.bRemainCount[n] = TingCardResult.bTingCardCount[n]*4;

		for( BYTE i = 0; i < TingCardResult.bTingCardCount[n]; i++ )
		{
			BYTE bCardData = TingCardResult.bTingCard[n][i];
			//���Լ���
			if( cbCardIndex[wMeChairId][SwitchToCardIndex(bCardData)] > 0 )
			{
				TingCardResult.bRemainCount[n] -= cbCardIndex[wMeChairId][SwitchToCardIndex(bCardData)];
				ASSERT( TingCardResult.bRemainCount[n]>=0 );
			}

			for( BYTE j = 0; j < GAME_PLAYER; j++ )
			{
				//�������
				for( BYTE k = 0; k < cbItemCount[j]; k++ )
				{
					if( cWeaveItem[j][k].cbCenterCard == bCardData )
					{
						TingCardResult.bRemainCount[n] -= cWeaveItem[j][k].cbWeaveKind==WIK_GANG?4:3;
						ASSERT( TingCardResult.bRemainCount[n]>=0 );
					}
				}
				//��������
				for( k = 0; k < cbDiscardCount[j]; k++ )
				{
					if( bCardData == cbDiscardCard[j][k] )
					{
						TingCardResult.bRemainCount[n]--;
						ASSERT( TingCardResult.bRemainCount[n]>=0 );
					}
				}
			}
		}
	}
	return true;
}

//��������
bool CGameLogic::SearchOutCard( WORD wMeChairId,WORD wCurrentUser,BYTE (&cbCardIndex)[GAME_PLAYER][MAX_INDEX],tagWeaveItem (&cWeaveItem)[GAME_PLAYER][4],
							   BYTE cbItemCount[4],BYTE (&cbDiscardCard)[GAME_PLAYER][55],BYTE cbDiscardCount[GAME_PLAYER],BYTE cbActionMask,BYTE cbActionCard,
							   const BYTE cbEnjoinCard[MAX_COUNT],BYTE cbEnjoinCount,tagOutCardResult &OutCardResult )
{
	//��ʼ��
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	ASSERT( wMeChairId != INVALID_CHAIR );
	if( wMeChairId == INVALID_CHAIR ) return false;

	//�жϺ�
	if( (cbActionMask&WIK_CHI_HU) != WIK_NULL )
	{
		OutCardResult.cbOperateCode = WIK_CHI_HU;
		OutCardResult.cbOperateCard = cbActionCard;
		return true;
	}

	//ת��������
	BYTE byCard[MAX_COUNT],byCardCount = 0;
	for( BYTE i = 0; i < MAX_INDEX; i++ )
	{
		for( BYTE j = 0; j < cbCardIndex[wMeChairId][i]; j++ )
		{
			byCard[byCardCount++] = i;
		}
	}
	BYTE byEnjoinCardIndex[MAX_COUNT];
	ZeroMemory(byEnjoinCardIndex,sizeof(byEnjoinCardIndex));
	for( i = 0; i < cbEnjoinCount; i++ )
		byEnjoinCardIndex[i] = SwitchToCardIndex(cbEnjoinCard[i]);

	//�жϸ�
	if( (cbActionMask&WIK_GANG) != WIK_NULL )
	{
		if( cbActionCard == 0 || wMeChairId == wCurrentUser)
		{
			ASSERT( wMeChairId == wCurrentUser );
			tagGangCardResult GangCardResult;
			AnalyseGangCard(cbCardIndex[wMeChairId],cWeaveItem[wMeChairId],cbItemCount[wMeChairId],GangCardResult);
			ASSERT( GangCardResult.cbCardCount > 0 );
			cbActionCard = GangCardResult.cbCardData[0];
		}
		//����ԭʼ��
		int nOrgScore;
		CAndroidAI AndroidAi;
		AndroidAi.SetCardData(byCard,byCardCount);
		AndroidAi.Think();
		nOrgScore = AndroidAi.GetMaxScore();
		//����ܺ�÷�
		int nScore;
		AndroidAi.SetCardData(byCard,byCardCount);
		AndroidAi.SetAction(WIK_GANG,SwitchToCardIndex(cbActionCard));
		AndroidAi.Think();
		nScore = AndroidAi.GetMaxScore();
		//����ܺ�÷�>=ԭʼ��,���
		if( nScore >= nOrgScore ||
			wMeChairId==wCurrentUser&&nScore>=nOrgScore-100)
		{
			OutCardResult.cbOperateCard = cbActionCard;
			OutCardResult.cbOperateCode = WIK_GANG;
			return true;
		}
		if( wCurrentUser == INVALID_CHAIR )
			return false;
	}

	//�ж���
	if( wMeChairId==wCurrentUser )
	{
		if( AnalyseTingCard(cbCardIndex[wMeChairId],cWeaveItem[wMeChairId],cbItemCount[wMeChairId],0) )
		{
			tagTingCardResult TingCardResult;
			//��������
			if( SearchTingCard(wMeChairId,cbCardIndex,cWeaveItem,
				cbItemCount,cbDiscardCard,cbDiscardCount,TingCardResult) )
			{
				ASSERT( TingCardResult.bAbandonCount > 0 );
				BYTE bMostIndex = 0;
				BYTE bMostCount = TingCardResult.bRemainCount[0];
				//ɸѡ����������
				for( BYTE i = 1; i < TingCardResult.bAbandonCount; i++ )
				{
					if( TingCardResult.bRemainCount[i] > bMostCount )
					{
						bMostIndex = i;
						bMostCount = TingCardResult.bRemainCount[i];
					}
				}
				//���ƿ���
				if( bMostCount > 0 )
				{
					OutCardResult.cbOperateCode = WIK_NULL;
					OutCardResult.cbOperateCard = TingCardResult.bAbandonCard[bMostIndex];
					ASSERT( cbCardIndex[wMeChairId][SwitchToCardIndex(OutCardResult.cbOperateCard)] > 0 );
					return true;
				}
				//������
				else
				{
					//����AI
					CAndroidAI AndroidAi;
					AndroidAi.SetCardData(byCard,byCardCount);
					AndroidAi.SetEnjoinOutCard(byEnjoinCardIndex,cbEnjoinCount);
					AndroidAi.Think();
					//������в���
					BYTE byBadlyCard = 0xff;
					if( byCardCount <= 2 )
						byBadlyCard = AndroidAi.GetBadlyCard();
					else
						byBadlyCard = AndroidAi.GetBadlyIn2Card();
					if( 0xff != byBadlyCard )
					{
						OutCardResult.cbOperateCode = WIK_NULL;
						ASSERT( cbCardIndex[wMeChairId][byBadlyCard] > 0 );
						OutCardResult.cbOperateCard = SwitchToCardData(byBadlyCard);
						return true;
					}
					else
					{
						//�������ֻ����в���
						byBadlyCard = AndroidAi.GetBadlyIn3Card();
						if( 0xff != byBadlyCard )
						{
							OutCardResult.cbOperateCode = WIK_NULL;
							ASSERT( cbCardIndex[wMeChairId][byBadlyCard] > 0 );
							OutCardResult.cbOperateCard = SwitchToCardData(byBadlyCard);
							return true;
						}
					}
				}
			}
			ASSERT( FALSE );
			return false;
		}
	}

	//�ж���,��
	if( (cbActionMask&(WIK_PENG|WIK_LEFT|WIK_CENTER|WIK_RIGHT)) != WIK_NULL )
	{
		ASSERT( cbActionCard != 0 );

		int nScore[5] = { 0,0,0,0,0 };
		BYTE cbCode[5] = { WIK_NULL,WIK_PENG,WIK_LEFT,WIK_CENTER,WIK_RIGHT };
		//������߷�
		CAndroidAI ai;
		ai.SetCardData(byCard,byCardCount);
		ai.Think();
		nScore[0] = ai.GetMaxScore();
		if( cbActionMask & WIK_PENG )
		{
			ai.SetCardData(byCard,byCardCount);
			ai.SetAction(WIK_PENG,SwitchToCardIndex(cbActionCard));
			ai.Think();
			nScore[1] = ai.GetMaxScore();
		}
		if( cbActionMask & WIK_LEFT )
		{
			ai.SetCardData(byCard,byCardCount);
			ai.SetAction(WIK_LEFT,SwitchToCardIndex(cbActionCard));
			ai.Think();
			nScore[2] = ai.GetMaxScore();
		}
		if( cbActionMask & WIK_CENTER )
		{
			ai.SetCardData(byCard,byCardCount);
			ai.SetAction(WIK_CENTER,SwitchToCardIndex(cbActionCard));
			ai.Think();
			nScore[3] = ai.GetMaxScore();
		}
		if( cbActionMask & WIK_RIGHT )
		{
			ai.SetCardData(byCard,byCardCount);
			ai.SetAction(WIK_RIGHT,SwitchToCardIndex(cbActionCard));
			ai.Think();
			nScore[4] = ai.GetMaxScore();
		}
		//������߷�
		BYTE byIndex = 0;
		int nMaxScore = nScore[0];
		for( i = 1; i < 5; i++ )
		{
			if( nScore[i] > nMaxScore )
			{
				byIndex = i;
				nMaxScore = nScore[i];
			}
		}
		//��������÷�>ԭʼ��
		if( byIndex > 0 )
		{
			OutCardResult.cbOperateCode = cbCode[byIndex];
			OutCardResult.cbOperateCard = cbActionCard;
			return true;
		}
		return false;
	}

	//Ч�鵱ǰ������Ϊ�Լ�
	ASSERT( wMeChairId == wCurrentUser );
	if( wMeChairId != wCurrentUser )
		return false;
	//Ч����Ŀ
#ifdef _DEBUG
	BYTE bCardCount = 0;
	for( BYTE i = 0; i < MAX_INDEX; i++ )
		bCardCount += cbCardIndex[wMeChairId][i];
	ASSERT( (bCardCount+1)%3 == 0 );
#endif

	//���ñ���
	OutCardResult.cbOperateCode = WIK_NULL;
	//��������
	CAndroidAI AndroidAi;
	AndroidAi.SetCardData(byCard,byCardCount);
	AndroidAi.SetEnjoinOutCard(byEnjoinCardIndex,cbEnjoinCount);
	AndroidAi.Think();
	BYTE byBadlyIndex = AndroidAi.GetBadlyCard();
	if( byBadlyIndex != 0xff )
	{
		OutCardResult.cbOperateCard = SwitchToCardData(byBadlyIndex);
		return true;
	}
	//��ֹ������ֹ������
	else
	{
		byBadlyIndex = AndroidAi.GetBadlyIn2Card();
		if( byBadlyIndex != 0xff )
		{
			OutCardResult.cbOperateCard = SwitchToCardData(byBadlyIndex);
			return true;
		}else
		{
			byBadlyIndex = AndroidAi.GetBadlyIn3Card();
			if( 0xff != byBadlyIndex )
			{
				OutCardResult.cbOperateCard = SwitchToCardData(byBadlyIndex);
				return true;
			}
		}
	}

	ASSERT( FALSE );
	return false;
}



//////////////////////////////////////////////////////////////////////////
