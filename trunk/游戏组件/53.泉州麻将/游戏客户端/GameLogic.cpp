#include "StdAfx.h"
#include "GameLogic.h"
#include ".\gamelogic.h"

//////////////////////////////////////////////////////////////////////////

//�齫����
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
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
	0x31,0x31,0x31,0x31,												//����
	0x32,0x32,0x32,0x32,												//����
	0x33,0x33,0x33,0x33,												//�Ϸ�
	0x34,0x34,0x34,0x34,												//����
	0x35,0x35,0x35,0x35,												//��
	0x36,0x36,0x36,0x36,												//��
	0x37,0x37,0x37,0x37,												//��
	0x41,0x42,0x43,0x44,												//�����ﶬ
	0x45,0x46,0x47,0x48													//÷������
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

//�����齫
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{
	//����׼��
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));	

	//�����齫
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//ɾ���齫
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���齫
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//ɾ���齫
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

//ɾ���齫
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//ɾ���齫
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//Ч���齫
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//ɾ���齫
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
			//ɾ���齫
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//ɾ���齫
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//��������
	ASSERT(cbCardCount<=14);
	ASSERT(cbRemoveCount<=cbCardCount);

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[14];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����齫
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

	//�����齫
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3))||((cbValue>=1)&&(cbValue<=8)&&(cbColor==4)));
}

//�齫��Ŀ
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//����齫
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard+1;
			cbCardBuffer[1]=cbCenterCard+2;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_RIGHT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard-2;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:	//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard-1;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

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
		if ((wChiHuKind&0x0001)!=0) cbChiHuOrder++;
	}

	//Ȩλ����
	for (BYTE i=0;i<16;i++)
	{
		wChiHuRight>>=1;
		if ((wChiHuRight&0x0001)!=0) cbChiHuOrder++;
	}

	return cbChiHuOrder;
}

//�����ж�
BYTE CGameLogic::EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (cbCurrentCard>=0x31) return WIK_NULL;

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
			if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0)) continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0)) continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0)) continue;

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
BYTE CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult,BYTE cbGoldCard[],BYTE cbGoldCount,bool &bGoldCardEye)
{
	//��������
	WORD wChiHuKind=CHK_NULL;
	static CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
	bGoldCardEye = false;

	//�����齫
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//�����齫
	if (cbCurrentCard!=0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;	

	//������Ŀ
	BYTE bGetCount=0;
	bGetCount = GetGoldCardCount(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbGoldCard,cbGoldCount);


	//��������
	//��������
	if(bGetCount==3)wChiHuKind|=CHK_SKY_SWIM;
	//��������
	if(wChiHuRight==CHR_SINGLE_SWIM)wChiHuKind|=CHK_SINGLE_SWIM;
	//˫������
	else if(wChiHuRight==CHR_DOUBLE_SWIM)wChiHuKind|=CHK_DOUBLE_SWIM;
	//��������
	else if(wChiHuRight==CHR_THREE_SWIM)wChiHuKind|=CHK_THREE_SWIM;

	
	/*if(bGetCount==1)
		wChiHuRight|=CHR_SINGLE_SWIM;//�ν�Ȩλ
	else if(bGetCount==2)
		wChiHuRight|=CHR_DOUBLE_SWIM;//˫��Ȩλ
	else if(bGetCount>2)
		wChiHuRight|=CHR_THREE_SWIM;//����Ȩλ*/

	//�����齫
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//��������
		bool bNeedSymbol=((wChiHuRight&0xFF00)==0);

		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			bool bLianCard=false,bPengCard=false;
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//��������
			BYTE cbEyeValue=pAnalyseItem->cbCardEye&MASK_VALUE;

			//���ͷ���
			for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
			{
				BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[j];				
				bPengCard=((cbWeaveKind&(WIK_GANG|WIK_PENG))!=0)?true:bPengCard;
				bLianCard=((cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianCard;
				
			}


			//���۷���
			for(BYTE i=0;i<cbGoldCount;i++)
			{
				if(pAnalyseItem->cbCardEye==cbGoldCard[i])
				{
					bGoldCardEye=true;
					break;
				}
			}

			//�����ж�
			ASSERT((bLianCard==true)||(bPengCard==true));
			
			if (bLianCard==true) wChiHuKind|=CHK_PING_HU;
			else if ((bLianCard==false)&&(bPengCard==true)) wChiHuKind|=CHK_PING_HU;	
			
			//��������
			//if ((bLianCard==false)&&(bPengCard==true)) wChiHuKind|=CHK_PENG_PENG;		

			
		
		}
		
	}	

	//����ж�
	if (wChiHuKind!=CHK_NULL)
	{
		//���ý��
		ChiHuResult.wChiHuKind=wChiHuKind;
		ChiHuResult.wChiHuRight=wChiHuRight;

		return WIK_CHI_HU;
	}


	return WIK_NULL;
}

//�齫ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<42);	
	if(cbCardIndex<34)
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	else
		return (0x41+cbCardIndex-34);

}

//�齫ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));	
	if(cbCardData<0x41)
		return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
	else 
		return (34+(cbCardData&MASK_VALUE)-1);

}

//�齫ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//ת���齫
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

//�齫ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//ת���齫
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//�Ժ�����
BYTE CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult)
{
	//��������
	WORD wChiHuKind=CHK_NULL;
	static CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));


	//�����齫
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//�����齫
	if (cbCurrentCard!=0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;		

	//�����齫
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//��������
		bool bNeedSymbol=((wChiHuRight&0xFF00)==0);

		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			bool bLianCard=false,bPengCard=false;
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//��������
			BYTE cbEyeValue=pAnalyseItem->cbCardEye&MASK_VALUE;

			//���ͷ���
			for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
			{
				BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[j];				
				bPengCard=((cbWeaveKind&(WIK_GANG|WIK_PENG))!=0)?true:bPengCard;
				bLianCard=((cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianCard;
				
			}
			

			//�����ж�
			ASSERT((bLianCard==true)||(bPengCard==true));
			
			if (bLianCard==true) wChiHuKind|=CHK_PING_HU;
			else if ((bLianCard==false)&&(bPengCard==true)) wChiHuKind|=CHK_PING_HU;				
		
		}
		
	}	

	//����ж�
	if (wChiHuKind!=CHK_NULL)
	{
		//���ý��
		ChiHuResult.wChiHuKind=wChiHuKind;
		ChiHuResult.wChiHuRight=wChiHuRight;

		return WIK_CHI_HU;
	}


	return WIK_NULL;
}

//�����齫
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	BYTE i, j;

	//������Ŀ
	BYTE cbCardCount=0;
	for ( i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0)) return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==5);

	//�����ж�
	if (cbLessKindItem==0)
	{
		//Ч�����
		ASSERT((cbCardCount==2)&&(cbWeaveCount==5));

		//�����ж�
		for ( i=0;i<MAX_INDEX;i++)
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
		for (BYTE i=0;i<MAX_INDEX;i++)
		{

			if(cbCardIndex[i]==0)continue;		
			

			//ͬ���ж�
			if (cbCardIndex[i]>=3)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				if(cbCardIndex[i]==3)
					KindItem[cbKindItemCount++].cbWeaveKind=WIK_PENG;
				else
					//KindItem[cbKindItemCount++].cbWeaveKind=WIK_FILL;
					KindItem[cbKindItemCount++].cbWeaveKind=WIK_GANG;
			}
			
			
			
			
			//�����ж�:Ͳ�����������			
			if ((i<(MAX_INDEX-17))&&(cbCardIndex[i]>0)&&((i%9)<7))
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
		BYTE cbIndex[5]={0,1,2,3,4};
		tagKindItem * pKindItem[5];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<cbLessKindItem;i++) pKindItem[i]=&KindItem[cbIndex[i]];

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
				else cbCardIndexTemp[cbCardIndex]--;
			}

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;
				for (BYTE i=0;i<MAX_INDEX;i++)
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
					}

					//��������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
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
						for (BYTE j=(i-1);j<cbLessKindItem;j++) cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0) break;
			}
			else cbIndex[cbLessKindItem-1]++;
			
		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}

//�Ժ�����
BYTE CGameLogic::AnalyseChiHuLevity(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult &ChiHuResult,BYTE cbGoldCard[],BYTE cbGoldCount,BYTE cbReCardIndex[MAX_INDEX])
{
	//��������
	WORD wChiHuKind=CHK_NULL;
	static CAnalyseItemArray AnalyseItemArray;
	ZeroMemory(cbReCardIndex,sizeof(BYTE)*MAX_INDEX);

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));


	//�����齫
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	CopyMemory(cbReCardIndex,cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//�����齫
	if (cbCurrentCard!=0)
	{
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
		cbReCardIndex[SwitchToCardIndex(cbCurrentCard)]++;
	}

	//������Ŀ
	BYTE bGoldCount=0;
	bGoldCount = GetGoldCardCount(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbGoldCard,cbGoldCount);

	//��������
	//��������
	if(bGoldCount==3)wChiHuKind|=CHK_SKY_SWIM;
	//��������
	if(wChiHuRight==CHR_SINGLE_SWIM)wChiHuKind|=CHK_SINGLE_SWIM;
	//˫������
	else if(wChiHuRight==CHR_DOUBLE_SWIM)wChiHuKind|=CHK_DOUBLE_SWIM;
	//��������
	else if(wChiHuRight==CHR_THREE_SWIM)wChiHuKind|=CHK_THREE_SWIM;	

	//ͳ�Ƴ�����������Ľ���
	bGoldCount=GetGoldCardCount(cbCardIndexTemp,NULL,0,cbGoldCard,cbGoldCount);

	
	AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,0,0,ChiHuResult);

	wChiHuKind|=ChiHuResult.wChiHuKind;

	//һ�Ž���
	if((wChiHuKind==CHK_NULL)&&(bGoldCount==1))
	{
		//�����˿�
		BYTE cbTempCard[MAX_INDEX];
		CopyMemory(cbTempCard,cbCardIndexTemp,sizeof(cbTempCard));

		//�޳�����
		for(BYTE i=0;i<cbGoldCount;i++)
		{
			BYTE cbIndex=SwitchToCardIndex(cbGoldCard[i]);
			if(cbTempCard[cbIndex]>0)
			{
				cbTempCard[cbIndex]=0;				
			}
		}

		//�жϺ���
		BYTE  cbCardDataTemp[34]=
		{
			0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
			0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
			0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
			0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//��
		};

		tagWeaveItem *pWeaveItem=WeaveItem;

		//�м����
		BYTE bHuKind=WIK_NULL;
		tagChiHuResult  ChiHuResult;

		//�ܷ����
		for (BYTE i=0;i<34;i++)
		{
			ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
			bHuKind=WIK_NULL;
			bHuKind=AnalyseChiHuCard(cbTempCard,pWeaveItem,cbWeaveCount,cbCardDataTemp[i],0, ChiHuResult);
			if (bHuKind!=WIK_NULL)
			{
				//�����˿�
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]++;

				//ȡ���˿�
				CopyMemory(cbReCardIndex,cbTempCard,sizeof(cbTempCard));

				break;
			}
				
		}

		wChiHuKind|=ChiHuResult.wChiHuKind;	

	}
	else if((wChiHuKind==CHK_NULL)&&(bGoldCount==2))//���Ž���
	{
		//�����˿�
		BYTE cbTempCard[MAX_INDEX];
		CopyMemory(cbTempCard,cbCardIndexTemp,sizeof(cbTempCard));

		//�޳�����
		for(BYTE i=0;i<cbGoldCount;i++)
		{
			BYTE cbIndex=SwitchToCardIndex(cbGoldCard[i]);
			if(cbTempCard[cbIndex]>0)
			{
				cbTempCard[cbIndex]=0;				
			}
		}

		//�жϺ���
		BYTE  cbCardDataTemp[34]=
		{
			0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
			0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
			0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
			0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//��
		};

		tagWeaveItem *pWeaveItem=WeaveItem;

		//�м����
		BYTE bHuKind=WIK_NULL;
		tagChiHuResult  ChiHuResult;

		
	

		//��ȡ�˿�
		for (BYTE i=0;i<34;i++)
		{
			//�����˿�
			cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]++;
			ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

			pWeaveItem=WeaveItem;

			//�ܷ����
			bHuKind=WIK_NULL;
			for(BYTE j=0;j<34;j++)
			{	
				ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
				bHuKind=WIK_NULL;
				bHuKind=AnalyseChiHuCard(cbTempCard,pWeaveItem,cbWeaveCount,cbCardDataTemp[j],0, ChiHuResult);
				if (bHuKind!=WIK_NULL) break;
					
			}

			BYTE j;

			if(bHuKind!=WIK_NULL)
			{
				//�����˿�
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]++;
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[j])]++;

				//ȡ���˿�
				CopyMemory(cbReCardIndex,cbTempCard,sizeof(cbTempCard));

				break;
			}

			//�޳��˿�
			cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]--;
		}

		wChiHuKind|=ChiHuResult.wChiHuKind;

	}
	

	//����ж�
	if (wChiHuKind!=CHK_NULL)
	{
		//���ý��
		ChiHuResult.wChiHuKind=wChiHuKind;
		ChiHuResult.wChiHuRight=wChiHuRight;

		return WIK_CHI_HU;
	}


	return WIK_NULL;

}
/*
// �ν�״̬
bool CGameLogic::IsGoldSwimSingleStatus(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount,BYTE bGoldCard[],BYTE bGoldCount)
{
	//�����˿�
	BYTE cbTempCard[MAX_INDEX];
	CopyMemory(cbTempCard,cbCardIndex,sizeof(cbTempCard));

	BYTE  cbCardDataTemp[34]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//��
	};

	tagWeaveItem *pWeaveItem=WeaveItem;

	BYTE cbTmpGoldCard[4]={0,0,0,0};

	//��ȡ�˿�
	for (BYTE i=0;i<34;i++)
	{	
		tagChiHuResult  ChiHuResult;
		BYTE bHuKind=WIK_NULL;
		bHuKind=AnalyseChiHuCard(cbTempCard,pWeaveItem,cbWeaveCount,cbCardDataTemp[i],0, ChiHuResult,cbTmpGoldCard,4);
		if (bHuKind!=WIK_NULL) return true;
			
	}

	return false;
}*/

// ���޻���
bool CGameLogic::IsHaveFlowerCard(BYTE cbCardIndex[MAX_INDEX],BYTE bFlowerCard[],BYTE bFlowerCount)
{
	//�齫�ж�
	for(BYTE i=0;i<bFlowerCount;i++)
	{
		if(bFlowerCard[i]==0)continue;

		ASSERT(IsValidCard(bFlowerCard[i]));

		if(cbCardIndex[SwitchToCardIndex(bFlowerCard[i])]>0)return true;
	}
	return false;
}

//�Ƿ��ǻ���
bool CGameLogic::IsFlowerCard(BYTE cbCardData,BYTE bFlowerCard[],BYTE bFlowerCount)
{
	for(BYTE i=0;i<bFlowerCount;i++)
	{
		if(bFlowerCard[i]==0)continue;
		ASSERT(IsValidCard(bFlowerCard[i]));
		if(cbCardData==bFlowerCard[i])return true;
	}

	return false;
}

//��ȡ����
BYTE CGameLogic::GetFlowerCard(BYTE cbCardIndex[MAX_INDEX],BYTE bFlowerCard[],BYTE bFlowerCount)
{
	BYTE cbCardData=0;
	for(BYTE i=0;i<bFlowerCount;i++)
	{
		if(bFlowerCard[i]==0)continue;

		ASSERT(IsValidCard(bFlowerCard[i]));
		if(cbCardIndex[SwitchToCardIndex(bFlowerCard[i])]>0)
		{
			cbCardData = bFlowerCard[i];
			break;
		}
	}

	return cbCardData;

}

//�����ж�
bool CGameLogic::IsGoldCard(BYTE cbCardData,BYTE bGoldCard[],BYTE bGoldCount)
{
	for(BYTE i=0;i<bGoldCount;i++)
	{
		if(bGoldCard[i]==0)continue;

		if(cbCardData==bGoldCard[i])return true;

	}
	return false;
}

//������Ŀ
BYTE CGameLogic::GetGoldCardCount(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,BYTE bGoldCard[],BYTE bGoldCount)
{
	BYTE bReturnGoldCount=0;

	for(BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCardData=WeaveItem[i].cbCenterCard;

		

		if(WeaveItem[i].cbWeaveKind==WIK_PENG)
		{

			if(IsGoldCard(cbCardData,bGoldCard,bGoldCount))	//�����Ѿ�����	
			{
				bReturnGoldCount=3;
				return bReturnGoldCount;
			}
		}
		else if(WeaveItem[i].cbWeaveKind!=WIK_GANG)
		{
			BYTE bCardData[3]={0,0,0};//��������
			bool bEateType=false;	  //�����ж�
			switch(WeaveItem[i].cbWeaveKind)
			{
			case WIK_LEFT://���
				{
					bCardData[0]=cbCardData;
					bCardData[1]=cbCardData+0x01;
					bCardData[2]=cbCardData+0x02;
					bEateType=true;				
					break;
				}
			case WIK_CENTER://�г�
				{
					bCardData[0]=cbCardData-0x01;
					bCardData[1]=cbCardData;
					bCardData[2]=cbCardData+0x01;
					bEateType=true;
				break;
				}
			case WIK_RIGHT://�ҳ�
				{
					bCardData[0]=cbCardData-0x02;
					bCardData[1]=cbCardData-0x01;
					bCardData[2]=cbCardData;
					bEateType=true;
					break;
				}
			}

			if(bEateType)
			{
				for(BYTE j=0;j<3;j++)
				{
					//�������н���
					if(IsGoldCard(bCardData[j],bGoldCard,bGoldCount))bReturnGoldCount++;					
				}
			}

		}

		if(bReturnGoldCount>2)return bReturnGoldCount;
	}

	for(BYTE i=0;i<MAX_INDEX;i++)
	{
		if(cbCardIndex[i]>0)
		{
			BYTE cbCard=SwitchToCardData(i);

			//�������н���
			if(IsGoldCard(cbCard,bGoldCard,bGoldCount))bReturnGoldCount+=cbCardIndex[i];

			if(bReturnGoldCount>2)return bReturnGoldCount;
		}
	}

	return bReturnGoldCount;
}

//�����ϱ�
bool CGameLogic::IsExistAllSidesCard(BYTE cbCardIndex[MAX_INDEX])
{
	if(cbCardIndex[SwitchToCardIndex(0x31)]&&cbCardIndex[SwitchToCardIndex(0x32)]&&
		cbCardIndex[SwitchToCardIndex(0x33)]&&cbCardIndex[SwitchToCardIndex(0x34)])return true;

	return false;

}
	//÷�����
bool CGameLogic::IsExistAllPlantCard(BYTE cbCardIndex[MAX_INDEX])
{
	if(cbCardIndex[SwitchToCardIndex(0x41)]&&cbCardIndex[SwitchToCardIndex(0x42)]&&
		cbCardIndex[SwitchToCardIndex(0x43)]&&cbCardIndex[SwitchToCardIndex(0x44)])return true;

	return false;

}
//�����ﶬ
bool CGameLogic::IsExistAllSeasonCard(BYTE cbCardIndex[MAX_INDEX])
{
	if(cbCardIndex[SwitchToCardIndex(0x45)]&&cbCardIndex[SwitchToCardIndex(0x46)]&&
		cbCardIndex[SwitchToCardIndex(0x47)]&&cbCardIndex[SwitchToCardIndex(0x48)])return true;

	return false;

}

//////////////////////////////////////////////////////////////////////////