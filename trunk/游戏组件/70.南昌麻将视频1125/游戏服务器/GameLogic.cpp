#include "StdAfx.h"
#include "GameLogic.h"

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
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	FillMemory(m_cbKingCardIndex,sizeof(m_cbKingCardIndex),0xFF);
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
	if (cbCardCount>CountArray(cbTempCardData))	return false;

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
//���þ���
void CGameLogic::SetKingCardIndex(BYTE cbKingCardIndex[2])
{
	//ѭ������
	for (BYTE i=0;i<2;i++)
	{
		m_cbKingCardIndex[i]=cbKingCardIndex[i];
	}
}
//��ȡ����
BYTE* CGameLogic::GetKingCardIndex(BYTE cbKingCardIndex[2])
{
	//��ʼ��ֵ
	memset(cbKingCardIndex,0xFF,sizeof(BYTE)*2);

	//ѭ����ȡ
	for (BYTE i=0;i<2;i++)
	{
		cbKingCardIndex[i]=m_cbKingCardIndex[i];
	}
	return cbKingCardIndex;
}
//�ж�����
bool CGameLogic::IsKingCardData(BYTE cbCardData)
{
	return m_cbKingCardIndex[0]==SwitchToCardIndex(cbCardData);
}
//�ж�����
bool CGameLogic::IsKingCardIndex(BYTE cbCardIndex)
{
	return m_cbKingCardIndex[0]==cbCardIndex;
}
//�жϸ���
bool CGameLogic::IsViceKingCardData(BYTE cbCardData)
{
	return m_cbKingCardIndex[1]==SwitchToCardIndex(cbCardData);
}
//�жϸ���
bool CGameLogic::IsViceKingCardIndex(BYTE cbCardIndex)
{
	return m_cbKingCardIndex[1]==cbCardIndex;
}
//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
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
BYTE CGameLogic::GetWeaveCard(WORD wWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//����˿�
	switch (wWeaveKind)
	{
	case WIK_LEFT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+2;
			cbCardBuffer[0]=cbCenterCard;

			return 3;
		}
	case WIK_RIGHT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard-2;
			cbCardBuffer[2]=cbCenterCard-1;
			cbCardBuffer[0]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:	//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[0]=cbCenterCard;
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
	case WIK_DNBL:		//���ϱ���
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+3;
			cbCardBuffer[0]=cbCenterCard;

			return 3;
		}
	case WIK_DNBC:		//���ϱ���
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+2;

			return 3;
		}
	case WIK_DNBR:		//���ϱ���
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard-3;
			cbCardBuffer[2]=cbCenterCard-2;
			cbCardBuffer[0]=cbCenterCard;

			return 3;
		}
	case WIK_DXBL:		//��������
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard+2;
			cbCardBuffer[2]=cbCenterCard+3;
			cbCardBuffer[0]=cbCenterCard;

			return 3;
		}
	case WIK_DXBC:		//��������
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard-2;
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_DXBR:		//��������
		{
			//���ñ���
			cbCardBuffer[1]=cbCenterCard-3;
			cbCardBuffer[2]=cbCenterCard-1;
			cbCardBuffer[0]=cbCenterCard;

			return 3;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//�����ȼ�
BYTE CGameLogic::GetUserActionRank(WORD wUserAction)
{
	//���Ƶȼ�
	if (wUserAction&WIK_CHI_HU) { return 4; }

	//���Ƶȼ�
	if (wUserAction&WIK_GANG) { return 3; }

	//���Ƶȼ�
	if (wUserAction&WIK_PENG) { return 2; }

	//���Ƶȼ�
	if (wUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT|WIK_DNBL|WIK_DNBC|WIK_DNBR|WIK_DXBL|WIK_DXBC|WIK_DXBR)) { return 1; }

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
WORD CGameLogic::EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{

	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));


	//��������
	BYTE cbExcursion[3]={0,1,2};
	WORD wItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//�����ж�
	WORD wEatKind=0;
	BYTE cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
	//�з���
	if(cbCurrentIndex>=31)
	{
		for (BYTE i=0;i<CountArray(wItemKind);i++)
		{
			BYTE cbValueIndex=cbCurrentIndex%9;
			if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=4))
			{
				//�����ж�
				cbFirstIndex=cbCurrentIndex-cbExcursion[i];
				//����
				if(cbFirstIndex<31) continue;
				if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
					continue;

				//��������
				wEatKind|=wItemKind[i];
			}
		}

	}
	else if(cbCurrentIndex>=27)
	{
		for (BYTE i=0;i<CountArray(wItemKind);i++)
		{
			BYTE cbValueIndex=cbCurrentIndex%9;
			if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=1))
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
				wEatKind|=wItemKind[i];
			}
		}

	}
	else
	{
		for (BYTE i=0;i<CountArray(wItemKind);i++)
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
				wEatKind|=wItemKind[i];
			}
		}
	}
	//���������з��׵����
	if(cbCurrentIndex>=27)
	{
		if(cbCurrentIndex==27)
		{
			if(cbCardIndex[28]>0&&cbCardIndex[30]>0)
				wEatKind |=WIK_DNBL;
			if(cbCardIndex[29]>0&&cbCardIndex[30]>0)
				wEatKind |=WIK_DXBL;
		}
		if(cbCurrentIndex==28)
		{
			if(cbCardIndex[27]>0&&cbCardIndex[30]>0)
				wEatKind |=WIK_DNBC;
		}
		if(cbCurrentIndex==29)
		{
			if(cbCardIndex[27]>0&&cbCardIndex[30]>0)
				wEatKind |=WIK_DXBC;
		}
		if(cbCurrentIndex==30)
		{
			if(cbCardIndex[27]>0&&cbCardIndex[28]>0)
				wEatKind |=WIK_DNBR;
			if(cbCardIndex[27]>0&&cbCardIndex[29]>0)
				wEatKind |=WIK_DXBR;
		}
	}

	return wEatKind;
}

//�����ж�
WORD CGameLogic::EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{

	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//�����ж�
WORD CGameLogic::EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{

	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
}

//���Ʒ���
WORD CGameLogic::AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//���ñ���
	WORD wActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//���ϸ��� 
	for (BYTE i=0;i<MAX_INDEX-1;i++)
	{
		if (cbCardIndex[i]==4)
		{
			wActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//��ϸ���
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].wWeaveKind==WIK_PENG)
		{
			if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				wActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return wActionMask;
}
//�����˿�
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray,BYTE cbCurrentCard,bool bZimo)
{
	//������Ŀ
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbCardIndex[i];

	//��������
	BYTE cbTempCardIndex[MAX_INDEX];
	CopyMemory(cbTempCardIndex,cbCardIndex,sizeof(cbTempCardIndex));

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))	return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_INDEX-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==4);

	//�����ж�
	if (cbLessKindItem==0)
	{
		//Ч�����
		ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

		//����
		if(cbCardIndex[m_cbKingCardIndex[0]] == 2 )
		{
			//��������
			tagAnalyseItem AnalyseItem;
			ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

			//�����滻
			AnalyseItem.cbEyeKingReplace=0;

			//���ý��
			for (BYTE j=0;j<cbWeaveCount;j++)
			{
				AnalyseItem.wWeaveKind[j]=WeaveItem[j].wWeaveKind;
				AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
			}
			//�����滻 
			AnalyseItem.cbWeaveKingReplace=0;

			AnalyseItem.cbCardEye=SwitchToCardData(m_cbKingCardIndex[0]);

			//������
			AnalyseItemArray.Add(AnalyseItem);
			return true;

		}

		//����
		if( cbCardIndex[m_cbKingCardIndex[1]]==2)
		{
			//��������
			tagAnalyseItem AnalyseItem;
			ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

			//�����滻
			AnalyseItem.cbEyeKingReplace=0;

			//���ý��
			for (BYTE j=0;j<cbWeaveCount;j++)
			{
				AnalyseItem.wWeaveKind[j]=WeaveItem[j].wWeaveKind;
				AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
			}
			//�����滻 
			AnalyseItem.cbWeaveKingReplace=0;

			AnalyseItem.cbCardEye=SwitchToCardData(m_cbKingCardIndex[1]);

			//������
			AnalyseItemArray.Add(AnalyseItem);
			return true;

		}
		//����+����
		if (cbCardIndex[m_cbKingCardIndex[0]]==1&& cbCardIndex[m_cbKingCardIndex[1]]==1) 
		{
			//��������
			tagAnalyseItem AnalyseItem;
			ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

			//�����滻
			AnalyseItem.cbEyeKingReplace=1;

			//���ý��
			for (BYTE j=0;j<cbWeaveCount;j++)
			{
				AnalyseItem.wWeaveKind[j]=WeaveItem[j].wWeaveKind;
				AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
			}
			//�����滻
			AnalyseItem.cbWeaveKingReplace=0;
			
			AnalyseItem.cbCardEye=SwitchToCardData(m_cbKingCardIndex[0]);

			//������
			AnalyseItemArray.Add(AnalyseItem);
			return true;
		}

		
		//��������
		//����+����
		if ((cbCardIndex[m_cbKingCardIndex[0]]  + cbCardIndex[m_cbKingCardIndex[1]])==1 && bZimo==true)
		{
			//��������
			tagAnalyseItem AnalyseItem;
			ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

			//�����滻
			AnalyseItem.cbEyeKingReplace=1;

			//���ý��
			for (BYTE j=0;j<cbWeaveCount;j++)
			{
				AnalyseItem.wWeaveKind[j]=WeaveItem[j].wWeaveKind;
				AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
			}
			//�����滻
			AnalyseItem.cbWeaveKingReplace=0;

			for (BYTE i=0;i<MAX_INDEX;i++)
			{
				// ��������
				if(IsKingCardIndex(i)) continue;

				//��������
				if(IsViceKingCardIndex(i)) continue;

				//�ҵ�����
				if (cbCardIndex[i]==1)
				{
					AnalyseItem.cbCardEye=SwitchToCardData(i);
				}
			}
			//������
			AnalyseItemArray.Add(AnalyseItem);
			return true;
		}

		//�������� ���ں�
		if ((cbCardIndex[m_cbKingCardIndex[0]])==1 && bZimo==false &&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[0])
		{
			//��������
			tagAnalyseItem AnalyseItem;
			ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

			//�����滻
			AnalyseItem.cbEyeKingReplace=1;

			//���ý��
			for (BYTE j=0;j<cbWeaveCount;j++)
			{
				AnalyseItem.wWeaveKind[j]=WeaveItem[j].wWeaveKind;
				AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
			}
			//�����滻
			AnalyseItem.cbWeaveKingReplace=false;

			for (BYTE i=0;i<MAX_INDEX;i++)
			{
				// ��������
				if(IsKingCardIndex(i)) continue;


				//�ҵ�����
				if (cbCardIndex[i]==1)
				{
					AnalyseItem.cbCardEye=SwitchToCardData(i);
				}
			}
			//������
			AnalyseItemArray.Add(AnalyseItem);
			return true;
		}

		
		//�������� ���ں�
		if ((cbCardIndex[m_cbKingCardIndex[1]])==1 && bZimo==false &&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[1])
		{
			//��������
			tagAnalyseItem AnalyseItem;
			ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

			//�����滻
			AnalyseItem.cbEyeKingReplace=1;

			//���ý��
			for (BYTE j=0;j<cbWeaveCount;j++)
			{
				AnalyseItem.wWeaveKind[j]=WeaveItem[j].wWeaveKind;
				AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
			}
			//�����滻
			AnalyseItem.cbWeaveKingReplace=false;

			for (BYTE i=0;i<MAX_INDEX;i++)
			{
				// ��������
				if(IsViceKingCardIndex(i)) continue;


				//�ҵ�����
				if (cbCardIndex[i]==1)
				{
					AnalyseItem.cbCardEye=SwitchToCardData(i);
				}
			}
			//������
			AnalyseItemArray.Add(AnalyseItem);
			return true;
		}


		//�����ж�
		for (BYTE i=0;i<MAX_INDEX;i++)
		{

			//����������˶�����ν��
			// ��������
			if(IsKingCardIndex(i)) continue;

			//��������
			if(IsViceKingCardIndex(i)) continue;

			if (cbCardIndex[i]==2)
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.wWeaveKind[j]=WeaveItem[j].wWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
				}
				AnalyseItem.cbWeaveKingReplace=0;
				AnalyseItem.cbEyeKingReplace=0;

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
			//ͬ���ж�
			if (cbCardIndex[i]>=3)
			{
				ASSERT( cbKindItemCount < CountArray(KindItem) );
				KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
			}

			//�����ж�
			if ((i<(MAX_INDEX-2-7))&&(cbCardIndex[i]>0)&&((i%9)<7))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
					{
						ASSERT( cbKindItemCount < CountArray(KindItem) );
						KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
					}
				}
			}


		}
		//��������
		for (BYTE i=MAX_INDEX-7;i<MAX_INDEX-5;i++)
		{
			//��������
			BYTE cbCardIndexTemp[MAX_INDEX];

			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

			//������ ������
			for (BYTE j=1;j<=cbCardIndex[i]&&cbCardIndex[i]>0;j++)
			{
				if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
					KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
					KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
				}
			}

			//���ϱ�
			if(i==MAX_INDEX-7)
			{
				for (BYTE j=1;j<=cbCardIndex[i]&&cbCardIndex[i]>0;j++)
				{
					if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+3]>=j))
					{
						ASSERT( cbKindItemCount < CountArray(KindItem) );
						KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
						KindItem[cbKindItemCount++].wWeaveKind=WIK_DNBL;
					}
				}

			}
			//������
			if(i==MAX_INDEX-7)
			{
				for (BYTE j=1;j<=cbCardIndex[i]&&cbCardIndex[i]>0;j++)
				{
					if ((cbCardIndex[i+2]>=j)&&(cbCardIndex[i+3]>=j))
					{
						ASSERT( cbKindItemCount < CountArray(KindItem) );
						KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+2;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
						KindItem[cbKindItemCount++].wWeaveKind=WIK_DXBL;
					}
				}
			}


		}	
		//�з���
		for (BYTE i=MAX_INDEX-3;i<MAX_INDEX;i++)
		{

			//�����ж�
			if ((i<(MAX_INDEX-2))&&(cbCardIndex[i]>0)&&((i%9)<7))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
					{
						KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
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
		BYTE cbIndex[MAX_WEAVE]={0,1,2,3};
		tagKindItem * pKindItem[MAX_WEAVE];
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

			//�¹��ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;

				for (BYTE i=0;i<MAX_INDEX;i++)
				{
					if (cbCardIndexTemp[i]==2)
					{
						//��������
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
						AnalyseItem.wWeaveKind[i]=WeaveItem[i].wWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
					}

					//��������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.wWeaveKind[i+cbWeaveCount]=pKindItem[i]->wWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
					}
					//�����滻
					AnalyseItem.cbWeaveKingReplace=0;

					//��������
					AnalyseItem.cbCardEye=cbCardEye;

					//�����滻
					AnalyseItem.cbEyeKingReplace=0;

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

	
	//Ӳ������
	if (AnalyseItemArray.GetCount()>0)
		return true;
	
	//Ӳ�����
	AnalyseItemArray.RemoveAll();

	//��������
	cbKindItemCount=0;
	ZeroMemory(KindItem,sizeof(KindItem));
	
	if(bZimo)
	{

		//��������Ŀ
		BYTE cbKingCount=cbCardIndex[m_cbKingCardIndex[0]];
		BYTE cbViceKingCount=cbCardIndex[m_cbKingCardIndex[1]];

		//�������
		cbTempCardIndex[m_cbKingCardIndex[0]]=0;
		cbTempCardIndex[m_cbKingCardIndex[1]]=0;

		//��������
		BYTE cbTotalKingCount=cbKingCount+cbViceKingCount;
		if(cbTotalKingCount==0) return false;
		//��ַ���
		//��ַ���
		if (cbCardCount>=3)
		{
			//3�������ȴտ�
			if(cbTotalKingCount>=3)
			{
				if(cbKingCount>0&&cbViceKingCount>0)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(m_cbKingCardIndex[0]);
					KindItem[cbKindItemCount].cbCardIndex[0]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount].cbCardIndex[1]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount].cbCardIndex[2]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
				}
			}
			for (BYTE i=0;i<MAX_INDEX;i++)
			{
				//ͬ���ж�
				if (cbTempCardIndex[i]>=3)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i;
					KindItem[cbKindItemCount].cbCardIndex[2]=i;
					KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
				}
				else if(cbTempCardIndex[i]==1)
				{
					//���˾���
					if(IsViceKingCardIndex(i)==false &&IsViceKingCardIndex(i)==false)
					{
						if(cbTotalKingCount>=2)
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i;
							KindItem[cbKindItemCount].cbCardIndex[2]=i;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
						}
					}

				}
				else if(cbTempCardIndex[i]==2)
				{
					//���˾���
					if(IsViceKingCardIndex(i)==false &&IsViceKingCardIndex(i)==false)
					{
						if(cbTotalKingCount>=1)
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i;
							KindItem[cbKindItemCount].cbCardIndex[2]=i;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
						}
					}

				}
				else
				{
					//ʲôҲ��ִ��
				}


				//�����ж�
				if ((i<(MAX_INDEX-2-7))&&((i%9)<7))
				{

					if (cbTempCardIndex[i]+cbTempCardIndex[i+1]+cbTempCardIndex[i+2]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbTempCardIndex[i],cbTempCardIndex[i+1],cbTempCardIndex[i+2]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

							BYTE cbWeaveKingReplace=0;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									//����
									cbWeaveKingReplace=1;
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
							}
							else
								break;

						}
					}

				}
			}
			//��������
			for (BYTE i=MAX_INDEX-7;i<MAX_INDEX-5;i++)
			{
				//��������
				BYTE cbCardIndexTemp[MAX_INDEX];

				//���ñ���
				CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

				//������ ������
				if (cbTempCardIndex[i]+cbTempCardIndex[i+1]+cbTempCardIndex[i+2]+cbTotalKingCount>=3)
				{
					BYTE cbIndex[3]={cbTempCardIndex[i],cbTempCardIndex[i+1],cbTempCardIndex[i+2]};
					INT cbTempTotalKingCount=cbTotalKingCount;

					while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
					{
						//����3��
						if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

						BYTE cbWeaveKingReplace=0;

						//�������
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
							}
							else 
							{
								//����
								cbWeaveKingReplace=1;
								cbTempTotalKingCount--;
							}
						}
						if( cbTempTotalKingCount >= 0 )
						{
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
						}
						else
							break;

					}
				}

				//���ϱ�
				if(i==MAX_INDEX-7)
				{
					if (cbTempCardIndex[i]+cbTempCardIndex[i+1]+cbTempCardIndex[i+3]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+3]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

							BYTE cbWeaveKingReplace=0;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									//����
									cbWeaveKingReplace=1;
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_DNBL;
							}
							else
								break;

						}
					}

				}
				//������
				if(i==MAX_INDEX-7)
				{
					if (cbTempCardIndex[i]+cbTempCardIndex[i+2]+cbTempCardIndex[i+3]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbTempCardIndex[i],cbTempCardIndex[i+2],cbTempCardIndex[i+3]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{

							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

							BYTE cbWeaveKingReplace=0;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									cbWeaveKingReplace=1;
									cbTempTotalKingCount--;

								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+2;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_DXBL;
							}
							else
								break;

						}
					}

				}


			}	



			for (BYTE i=MAX_INDEX-3;i<MAX_INDEX;i++)		//�з���
			{
				if ( i==MAX_INDEX-3)
				{
					if (cbTempCardIndex[i]+cbTempCardIndex[i+1]+cbTempCardIndex[i+2]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbTempCardIndex[i],cbTempCardIndex[i+1],cbTempCardIndex[i+2]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;
							BYTE cbWeaveKingReplace=0;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									//����
									cbWeaveKingReplace=1;
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
							}
							else
								break;

						}
					}
				}

			}

		}


		{
			//3�������ȴտ�
			if(cbTotalKingCount>=3)
			{
				if(cbKingCount>0&&cbViceKingCount>0)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(m_cbKingCardIndex[0]);
					KindItem[cbKindItemCount].cbCardIndex[0]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount].cbCardIndex[1]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount].cbCardIndex[2]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
				}
			}
			for (BYTE i=0;i<MAX_INDEX;i++)
			{
				//ͬ���ж�
				if (cbCardIndex[i]>=3)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i;
					KindItem[cbKindItemCount].cbCardIndex[2]=i;
					KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
				}
				else if(cbCardIndex[i]==1)
				{
					//���˾���
					if(IsViceKingCardIndex(i)==false &&IsViceKingCardIndex(i)==false)
					{
						if(cbTotalKingCount>=2)
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i;
							KindItem[cbKindItemCount].cbCardIndex[2]=i;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
						}
					}

				}
				else if(cbCardIndex[i]==2)
				{
					//���˾���
					if(IsViceKingCardIndex(i)==false &&IsViceKingCardIndex(i)==false)
					{
						if(cbTotalKingCount>=1)
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i;
							KindItem[cbKindItemCount].cbCardIndex[2]=i;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
						}
					}

				}
				else
				{
					//ʲôҲ��ִ��
				}


				//�����ж�
				if ((i<(MAX_INDEX-2-7))&&((i%9)<7))
				{

					if (cbCardIndex[i]+cbCardIndex[i+1]+cbCardIndex[i+2]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+2]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
							}
							else
								break;

						}
					}

				}
			}
			//��������
			for (BYTE i=MAX_INDEX-7;i<MAX_INDEX-5;i++)
			{
				//��������
				BYTE cbCardIndexTemp[MAX_INDEX];

				//���ñ���
				CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

				//������ ������
				if (cbCardIndex[i]+cbCardIndex[i+1]+cbCardIndex[i+2]+cbTotalKingCount>=3)
				{
					BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+2]};
					INT cbTempTotalKingCount=cbTotalKingCount;

					while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
					{
						//����3��
						if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

						//�������
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
							}
							else 
							{
								//����
								cbTempTotalKingCount--;
							}
						}
						if( cbTempTotalKingCount >= 0 )
						{
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
						}
						else
							break;

					}
				}

				//���ϱ�
				if(i==MAX_INDEX-7)
				{
					if (cbCardIndex[i]+cbCardIndex[i+1]+cbCardIndex[i+3]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+3]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;


							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									//����
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_DNBL;
							}
							else
								break;

						}
					}

				}
				//������
				if(i==MAX_INDEX-7)
				{
					if (cbCardIndex[i]+cbCardIndex[i+2]+cbCardIndex[i+3]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+2],cbCardIndex[i+3]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{

							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;


							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									//����
									cbTempTotalKingCount--;

								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+2;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_DXBL;
							}
							else
								break;

						}
					}

				}
			}	


			for (BYTE i=MAX_INDEX-3;i<MAX_INDEX;i++)		//�з���
			{
				if ( i==MAX_INDEX-3)
				{
					if (cbCardIndex[i]+cbCardIndex[i+1]+cbCardIndex[i+2]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+2]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
							}
							else
								break;

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
			BYTE cbIndex[MAX_WEAVE]={0,1,2,3};
			tagKindItem * pKindItem[MAX_WEAVE];
			ZeroMemory(&pKindItem,sizeof(pKindItem));

			//��ʼ���
			do
			{
				//���ñ���
				CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
				for (BYTE i=0;i<cbLessKindItem;i++)
					pKindItem[i]=&KindItem[cbIndex[i]];

				BYTE cbWeaveKingReplace=0;
				//�����ж�
				bool bEnoughCard=true;
				for (BYTE i=0;i<cbLessKindItem*3;i++)
				{
					//�����ж�
					BYTE cbCardIndex=pKindItem[i/3]->cbCardIndex[i%3]; 
					if (cbCardIndexTemp[cbCardIndex]==0)
					{
						cbWeaveKingReplace=1;
						if(cbCardIndexTemp[m_cbKingCardIndex[0]]==0)
						{
							if(cbCardIndexTemp[m_cbKingCardIndex[1]]==0)
							{
								bEnoughCard=false;
								break;

							}
							else
							
								cbCardIndexTemp[m_cbKingCardIndex[1]]--;

						}
						else
							cbCardIndexTemp[m_cbKingCardIndex[0]]--;
					}
					else 
						cbCardIndexTemp[cbCardIndex]--;
				}

				//�����ж�
				if (bEnoughCard==true)
				{
					//�����ж�
					BYTE cbCardEye=0;

					//�����滻
					BYTE cbEyeKingReplace=0;

					//�Ƿ����
					bool bContinue=true;

					if ((cbCardIndexTemp[m_cbKingCardIndex[0]]==1&& cbCardIndexTemp[m_cbKingCardIndex[1]])==1) //����+����
					{
						//�����滻
						cbEyeKingReplace=1;	

						//��������
						cbCardEye=SwitchToCardData(m_cbKingCardIndex[0]);

						//�Ƿ����
						bContinue=false;
					}
					if(bContinue==true)
					{
						if ((cbCardIndexTemp[m_cbKingCardIndex[0]]  + cbCardIndexTemp[m_cbKingCardIndex[1]])==1) //����+����
						{

							//�����滻
							cbEyeKingReplace=1;

							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								// ��������
								if(IsKingCardIndex(i)) continue;

								//��������
								if(IsViceKingCardIndex(i)) continue;

								//�ҵ�����
								if (cbCardIndex[i]==1)
								{
									cbCardEye=SwitchToCardData(i);
								}
							}
							//�Ƿ����
							bContinue=false;
						}


					}
					if(bContinue==true)
					{
						for (BYTE i=0;i<MAX_INDEX;i++)
						{
							if (cbCardIndexTemp[i]==2)
							{
								//��������
								cbCardEye=SwitchToCardData(i);

								//�����滻
								cbEyeKingReplace=0;

								//�Ƿ����
								bContinue=false;
								break;
							}
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
							AnalyseItem.wWeaveKind[i]=WeaveItem[i].wWeaveKind;
							AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
						}
						//�����滻
						AnalyseItem.cbWeaveKingReplace=cbWeaveKingReplace;

						//��������
						for (BYTE i=0;i<cbLessKindItem;i++) 
						{
							AnalyseItem.wWeaveKind[i+cbWeaveCount]=pKindItem[i]->wWeaveKind;
							AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
						}

						//��������
						AnalyseItem.cbCardEye=cbCardEye;

						//�����滻
						AnalyseItem.cbEyeKingReplace=cbEyeKingReplace;

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
	}
	else
	{

		//��������Ŀ
		BYTE cbKingCount=cbTempCardIndex[m_cbKingCardIndex[0]];
		BYTE cbViceKingCount=cbTempCardIndex[m_cbKingCardIndex[1]];

		//�������
		cbTempCardIndex[m_cbKingCardIndex[0]]=0;
		cbTempCardIndex[m_cbKingCardIndex[1]]=0;


		//��������
		BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		//����������
		if(bZimo==false &&(IsKingCardIndex(cbCurrentIndex)))
		{
			cbTempCardIndex[m_cbKingCardIndex[0]]=1;
			ASSERT(cbKingCount>=1);
			cbKingCount -=1;

		}
		if(bZimo==false &&(IsViceKingCardIndex(cbCurrentIndex)))
		{
			cbTempCardIndex[m_cbKingCardIndex[1]]=1;
			ASSERT(cbViceKingCount>=1);
			cbViceKingCount -=1;

		}
		//��������
		BYTE cbTotalKingCount=cbKingCount+cbViceKingCount;
		if(cbTotalKingCount==0) return false;
	
		//��ַ���
		if (cbCardCount>=3)
		{
			//3�������ȴտ�
			if(cbTotalKingCount>=3)
			{
				if(cbKingCount>0&&cbViceKingCount>0)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(m_cbKingCardIndex[0]);
					KindItem[cbKindItemCount].cbCardIndex[0]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount].cbCardIndex[1]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount].cbCardIndex[2]=m_cbKingCardIndex[0];
					KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
				}
			}
			for (BYTE i=0;i<MAX_INDEX;i++)
			{
				//ͬ���ж�
				if (cbTempCardIndex[i]>=3)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i;
					KindItem[cbKindItemCount].cbCardIndex[2]=i;
					KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
				}
				else if(cbTempCardIndex[i]==1)
				{
					//���˾���
					if(IsViceKingCardIndex(i)==false &&IsViceKingCardIndex(i)==false)
					{
						if(cbTotalKingCount>=2)
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i;
							KindItem[cbKindItemCount].cbCardIndex[2]=i;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
						}
					}

				}
				else if(cbTempCardIndex[i]==2)
				{
					//���˾���
					if(IsViceKingCardIndex(i)==false &&IsViceKingCardIndex(i)==false)
					{
						if(cbTotalKingCount>=1)
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i;
							KindItem[cbKindItemCount].cbCardIndex[2]=i;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
						}
					}

				}
				else
				{
					//ʲôҲ��ִ��
				}


				//�����ж�
				if ((i<(MAX_INDEX-2-7))&&((i%9)<7))
				{

					if (cbTempCardIndex[i]+cbTempCardIndex[i+1]+cbTempCardIndex[i+2]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbTempCardIndex[i],cbTempCardIndex[i+1],cbTempCardIndex[i+2]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

							BYTE cbWeaveKingReplace=0;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									//����
									cbWeaveKingReplace=1;
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
							}
							else
								break;

						}
					}

				}
			}
			//��������
			for (BYTE i=MAX_INDEX-7;i<MAX_INDEX-5;i++)
			{
				//��������
				BYTE cbCardIndexTemp[MAX_INDEX];

				//���ñ���
				CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

				//������ ������
				if (cbTempCardIndex[i]+cbTempCardIndex[i+1]+cbTempCardIndex[i+2]+cbTotalKingCount>=3)
				{
					BYTE cbIndex[3]={cbTempCardIndex[i],cbTempCardIndex[i+1],cbTempCardIndex[i+2]};
					INT cbTempTotalKingCount=cbTotalKingCount;

					while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
					{
						//����3��
						if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

						BYTE cbWeaveKingReplace=0;

						//�������
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
							}
							else 
							{
								//����
								cbWeaveKingReplace=1;
								cbTempTotalKingCount--;
							}
						}
						if( cbTempTotalKingCount >= 0 )
						{
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
						}
						else
							break;

					}
				}

				//���ϱ�
				if(i==MAX_INDEX-7)
				{
					if (cbTempCardIndex[i]+cbTempCardIndex[i+1]+cbTempCardIndex[i+3]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+3]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;
							
							BYTE cbWeaveKingReplace=0;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									//����
									cbWeaveKingReplace=1;
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_DNBL;
							}
							else
								break;

						}
					}

				}
				//������
				if(i==MAX_INDEX-7)
				{
					if (cbTempCardIndex[i]+cbTempCardIndex[i+2]+cbTempCardIndex[i+3]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbTempCardIndex[i],cbTempCardIndex[i+2],cbTempCardIndex[i+3]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{

							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

							BYTE cbWeaveKingReplace=0;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									cbWeaveKingReplace=1;
									cbTempTotalKingCount--;

								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+2;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_DXBL;
							}
							else
								break;

						}
					}

				}


			}	



			for (BYTE i=MAX_INDEX-3;i<MAX_INDEX;i++)		//�з���
			{
				if ( i==MAX_INDEX-3)
				{
					if (cbTempCardIndex[i]+cbTempCardIndex[i+1]+cbTempCardIndex[i+2]+cbTotalKingCount>=3)
					{
						BYTE cbIndex[3]={cbTempCardIndex[i],cbTempCardIndex[i+1],cbTempCardIndex[i+2]};
						INT cbTempTotalKingCount=cbTotalKingCount;

						while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
						{
							//����3��
							if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;
							BYTE cbWeaveKingReplace=0;

							//�������
							for( BYTE j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
								}
								else 
								{
									//����
									cbWeaveKingReplace=1;
									cbTempTotalKingCount--;
								}
							}
							if( cbTempTotalKingCount >= 0 )
							{
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
							}
							else
								break;

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
			BYTE cbIndex[MAX_WEAVE]={0,1,2,3};
			tagKindItem * pKindItem[MAX_WEAVE];
			ZeroMemory(&pKindItem,sizeof(pKindItem));

			//��ʼ���
			do
			{
				//���ñ���
				CopyMemory(cbCardIndexTemp,cbTempCardIndex,sizeof(cbCardIndexTemp));
				for (BYTE i=0;i<cbLessKindItem;i++)
					pKindItem[i]=&KindItem[cbIndex[i]];

				BYTE cbTempKingCount=cbKingCount;
				BYTE cbTempViceKingCount=cbViceKingCount;

				//�����ж�
				BYTE cbWeaveKingReplace=0;
				bool bEnoughCard=true;
				for (BYTE i=0;i<cbLessKindItem*3;i++)
				{
					//�����ж�
					BYTE cbCardIndex=pKindItem[i/3]->cbCardIndex[i%3]; 
					if (cbCardIndexTemp[cbCardIndex]==0)
					{
						cbWeaveKingReplace=1;
						if(cbTempKingCount==0)
						{
							if(cbTempViceKingCount==0)
							{
								bEnoughCard=false;
								break;

							}
							else
								cbTempViceKingCount--;

						}
						else
							cbTempKingCount--;
					}
					else 
						cbCardIndexTemp[cbCardIndex]--;
				}

				//�����ж�
				if (bEnoughCard==true)
				{
					//�����ж�
					BYTE cbCardEye=0;

					//�����滻
					BYTE cbEyeKingReplace=0;

					//�Ƿ����
					bool bContinue=true;

					if (cbTempViceKingCount==1&& cbTempKingCount==1) //����+����
					{
						//�����滻
						cbEyeKingReplace=1;	

						//��������
						cbCardEye=SwitchToCardData(m_cbKingCardIndex[0]);

						//�Ƿ����
						bContinue=false;
					}
					if(bContinue==true)
					{
						if (cbTempViceKingCount==1&&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[1]) //����+����
						{

							//�����滻
							cbEyeKingReplace=1;

							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								if(i==m_cbKingCardIndex[1]) continue;

								//�ҵ�����
								if (cbCardIndex[i]==1)
								{
									cbCardEye=SwitchToCardData(i);
								}
							}
							//�Ƿ����
							bContinue=false;
						}


						if (cbTempKingCount==1&&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[0]) //����+����
						{

							//�����滻
							cbEyeKingReplace=1;

							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								if(i==m_cbKingCardIndex[0]) continue;

								//�ҵ�����
								if (cbCardIndex[i]==1)
								{
									cbCardEye=SwitchToCardData(i);
								}
							}
							//�Ƿ����
							bContinue=false;
						}

					}
					if(bContinue==true)
					{
						for (BYTE i=0;i<MAX_INDEX;i++)
						{
							if (cbCardIndexTemp[i]==2)
							{
								//��������
								cbCardEye=SwitchToCardData(i);

								//�����滻
								cbEyeKingReplace=0;

								//�Ƿ����
								bContinue=false;
								break;
							}
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
							AnalyseItem.wWeaveKind[i]=WeaveItem[i].wWeaveKind;
							AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
						}

						//�����滻
						AnalyseItem.cbWeaveKingReplace=cbWeaveKingReplace;

						//��������
						for (BYTE i=0;i<cbLessKindItem;i++) 
						{
							AnalyseItem.wWeaveKind[i+cbWeaveCount]=pKindItem[i]->wWeaveKind;
							AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
						}

						//��������
						AnalyseItem.cbCardEye=cbCardEye;

						//�����滻
						AnalyseItem.cbEyeKingReplace=cbEyeKingReplace;

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
	}
	return false;


}


//�Ժ�����
WORD CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult,bool bZimo)
{
	//��������
	WORD wChiHuKind=CHK_NULL;

	//���ñ���
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//�����˿�
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	
	//��ģȨλ
	if(bZimo)
	{
		wChiHuRight|= CHR_ZI_MO;
	}

	//����Ȩλ
	if((bZimo) && (cbWeaveCount ==4) && ((cbCardIndex[m_cbKingCardIndex[0]]+cbCardIndex[m_cbKingCardIndex[1]])==1))
	{
		wChiHuRight |=CHR_KING_WAIT;
	}

	
	//Ȩλ���� �ܺ�+���� = ���ϻ�
	if((wChiHuRight&CHR_QIANG_GANG)&&(wChiHuRight&CHR_ZI_MO))
	{
		wChiHuRight &= ~CHR_QIANG_GANG;
		wChiHuRight &= ~CHR_ZI_MO;
		wChiHuRight |= CHR_GANG_FLOWER;
	}

	//�¹�����
	BYTE bGerman=0;

	//7��
	if(ServenStar(cbCardIndexTemp,bGerman,cbCurrentCard,bZimo)==true)
	{
		if(bGerman==1)
			wChiHuRight |=CHR_GERMAN;
		ChiHuResult.wChiHuKind = CHK_SERVEN;
		ChiHuResult.wChiHuRight |= wChiHuRight;

		return WIK_CHI_HU;

	}
	bGerman=0;

	//13��
	if(cbWeaveCount==0 &&IsNeatAlone(cbCardIndexTemp,bGerman,cbCurrentCard,bZimo)==true)
	{
		if(bGerman==1)
			wChiHuRight |=CHR_GERMAN;

		ChiHuResult.wChiHuKind=CHK_THIRTEEN;
		ChiHuResult.wChiHuRight |=wChiHuRight;
		return WIK_CHI_HU;
	}

	//����
	if((bZimo||wChiHuRight&CHR_DI) &&(cbCardIndex[m_cbKingCardIndex[0]]>0))
	{
		BYTE cbCardIndexTemp2[MAX_INDEX];
		CopyMemory(cbCardIndexTemp2,cbCardIndexTemp,sizeof(cbCardIndexTemp2));
		RemoveCard(cbCardIndexTemp2,cbCurrentCard);
		RemoveCard(cbCardIndexTemp2,SwitchToCardData(m_cbKingCardIndex[0]));

		bool bCurrentKing=IsKingCardData(cbCurrentCard)||IsViceKingCardData(cbCurrentCard);
		BYTE cbGerman=0;

		if(IsNeat2(cbCardIndexTemp2,WeaveItem,cbWeaveCount)==true)
		{
			ChiHuResult.wChiHuKind = CHK_QI_DUI;
			ChiHuResult.wChiHuRight =( CHR_GERMAN|CHR_KING_WAIT|wChiHuRight);
			return WIK_CHI_HU;
		}
		if(IsQiXiaoDui(cbCardIndexTemp2,WeaveItem,cbWeaveCount,cbCurrentCard,cbGerman,bZimo)==true)
		{
			if(cbGerman==1&&(bCurrentKing==true &&( SwitchToCardIndex((cbCurrentCard))==m_cbKingCardIndex[0]||(SwitchToCardIndex((cbCurrentCard))==m_cbKingCardIndex[1]&&cbCardIndex[m_cbKingCardIndex[1]]>0))))
				wChiHuRight |= CHR_GERMAN;
			ChiHuResult.wChiHuKind = CHK_QI_DUI;
			ChiHuResult.wChiHuRight =(CHR_KING_WAIT|wChiHuRight);
			return WIK_CHI_HU;
		}
		cbGerman=0;
		if(IsNeat3(cbCardIndexTemp2,cbGerman)==true)
		{
			if(cbGerman==1&&(bCurrentKing==true &&( SwitchToCardIndex((cbCurrentCard))==m_cbKingCardIndex[0]||(SwitchToCardIndex((cbCurrentCard))==m_cbKingCardIndex[1]&&cbCardIndex[m_cbKingCardIndex[1]]>0))))
				wChiHuRight |= CHR_GERMAN;
			wChiHuRight =(CHR_KING_WAIT|wChiHuRight);
		}
	}

	//����
	if((bZimo||wChiHuRight&CHR_DI) &&(cbCardIndex[m_cbKingCardIndex[1]]>0))
	{
		BYTE cbCardIndexTemp2[MAX_INDEX];
		CopyMemory(cbCardIndexTemp2,cbCardIndexTemp,sizeof(cbCardIndexTemp2));
		RemoveCard(cbCardIndexTemp2,cbCurrentCard);
		RemoveCard(cbCardIndexTemp2,SwitchToCardData(m_cbKingCardIndex[1]));

		bool bCurrentKing=IsKingCardData(cbCurrentCard)||IsViceKingCardData(cbCurrentCard);
		BYTE cbGerman=0;

		if(IsNeat2(cbCardIndexTemp2,WeaveItem,cbWeaveCount)==true)
		{
			ChiHuResult.wChiHuKind = CHK_QI_DUI;
			ChiHuResult.wChiHuRight =( CHR_GERMAN|CHR_KING_WAIT|wChiHuRight);
			return WIK_CHI_HU;
		}
		if(IsQiXiaoDui(cbCardIndexTemp2,WeaveItem,cbWeaveCount,cbCurrentCard,cbGerman,bZimo)==true)
		{
			if(cbGerman==1&&(bCurrentKing==true &&( SwitchToCardIndex((cbCurrentCard))==m_cbKingCardIndex[1]||(SwitchToCardIndex((cbCurrentCard))==m_cbKingCardIndex[0]&&cbCardIndex[m_cbKingCardIndex[0]]>0))))
				wChiHuRight |= CHR_GERMAN;
			ChiHuResult.wChiHuKind = CHK_QI_DUI;
			ChiHuResult.wChiHuRight =(CHR_KING_WAIT|wChiHuRight);
			return WIK_CHI_HU;
		}
		cbGerman=0;
		if(IsNeat3(cbCardIndexTemp2,cbGerman)==true)
		{
			if(cbGerman==1&&(bCurrentKing==true &&( SwitchToCardIndex((cbCurrentCard))==m_cbKingCardIndex[1]||(SwitchToCardIndex((cbCurrentCard))==m_cbKingCardIndex[0]&&cbCardIndex[m_cbKingCardIndex[0]]>0))))
				wChiHuRight |= CHR_GERMAN;			
			wChiHuRight =(CHR_KING_WAIT|wChiHuRight);
		}

	}
	
	bGerman=0;
	//7С�Է���
	if(IsQiXiaoDui(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,bGerman,bZimo))
	{
		if(bGerman>0)
			wChiHuKind=CHK_GERMAN_SERVEN;
		else
			wChiHuKind=CHK_QI_DUI;

		ChiHuResult.wChiHuKind = wChiHuKind;
		ChiHuResult.wChiHuRight |= wChiHuRight;

		return WIK_CHI_HU;
	}

    //��������
	static CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray,cbCurrentCard,bZimo);

	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			bool bLianCard=false,bPengCard=false;
			bool bGerman=true;
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//���ͷ���
			for (BYTE j=0;j<CountArray(pAnalyseItem->wWeaveKind);j++)
			{
				WORD wWeaveKind=pAnalyseItem->wWeaveKind[j];
				bPengCard=((wWeaveKind&(WIK_GANG|WIK_PENG))!=0)?true:bPengCard;
				bLianCard=((wWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianCard;

			}
			bGerman=(pAnalyseItem->cbWeaveKingReplace||pAnalyseItem->cbEyeKingReplace)?false:bGerman;
			//�����ж�
			ASSERT((bLianCard==true)||(bPengCard==true));	
			//����
			if ((bLianCard==true)&&(bPengCard==true)) 
				wChiHuKind|=CHK_JI_HU;
			if ((bLianCard==true)&&(bPengCard==false)) 
				wChiHuKind|=CHK_JI_HU;
			if((bLianCard==false)&&(bPengCard==true))
				wChiHuKind|=CHK_PENG_PENG;

			if(bGerman==true)
				wChiHuRight |=CHR_GERMAN;
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

//�Ƿ񾫵�
bool CGameLogic::IsKingWait(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[], BYTE cbWeaveCount,BYTE cbCurrentCard,bool bZimo)
{
	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//�����˿�
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//����
	if(cbCardIndex[m_cbKingCardIndex[0]]>0)
	{
		BYTE cbCardIndexTemp2[MAX_INDEX];
		CopyMemory(cbCardIndexTemp2,cbCardIndexTemp,sizeof(cbCardIndexTemp2));
		RemoveCard(cbCardIndexTemp2,cbCurrentCard);
		RemoveCard(cbCardIndexTemp2,SwitchToCardData(m_cbKingCardIndex[0]));

		bool bCurrentKing=IsKingCardData(cbCurrentCard)||IsViceKingCardData(cbCurrentCard);
		BYTE cbGerman=0;

		if(IsNeat2(cbCardIndexTemp2,WeaveItem,cbWeaveCount)==true)
		{
			return true;
		}
		if(IsQiXiaoDui(cbCardIndexTemp2,WeaveItem,cbWeaveCount,cbCurrentCard,cbGerman,bZimo)==true)
		{
			return true;
		}
		cbGerman=0;
		if(IsNeat3(cbCardIndexTemp2,cbGerman)==true)
		{
			return true;
		}
	}

	//����
	if(cbCardIndex[m_cbKingCardIndex[1]]>0)
	{
		BYTE cbCardIndexTemp2[MAX_INDEX];
		CopyMemory(cbCardIndexTemp2,cbCardIndexTemp,sizeof(cbCardIndexTemp2));
		RemoveCard(cbCardIndexTemp2,cbCurrentCard);
		RemoveCard(cbCardIndexTemp2,SwitchToCardData(m_cbKingCardIndex[1]));

		bool bCurrentKing=IsKingCardData(cbCurrentCard)||IsViceKingCardData(cbCurrentCard);
		BYTE cbGerman=0;

		if(IsNeat2(cbCardIndexTemp2,WeaveItem,cbWeaveCount)==true)
		{
			return true;
		}
		if(IsQiXiaoDui(cbCardIndexTemp2,WeaveItem,cbWeaveCount,cbCurrentCard,cbGerman,bZimo)==true)
		{
			return true;
		}
		cbGerman=0;
		if(IsNeat3(cbCardIndexTemp2,cbGerman)==true)
		{
			return true;
		}
	}


	return false;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<MAX_INDEX);

	ASSERT(cbCardIndex<34);
	return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{

	ASSERT(IsValidCard(cbCardData));
	////����λ��
	//BYTE cbValue=cbCardData&MASK_VALUE;
	//BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT],BYTE bMaxCount)
{
	//ת���˿� ���Ʒŵ�ԭ��λ��
	BYTE bPosition=0;
	if(cbCardIndex[m_cbKingCardIndex[0]] > 0)
	{
		for (BYTE j=0;j<cbCardIndex[m_cbKingCardIndex[0]];j++)
		{
			ASSERT(bPosition<bMaxCount);
			cbCardData[bPosition++]=SwitchToCardData(m_cbKingCardIndex[0]);
		}
	}
	if(cbCardIndex[m_cbKingCardIndex[1]] > 0)
	{
		for (BYTE j=0;j<cbCardIndex[m_cbKingCardIndex[1]];j++)
		{
			ASSERT(bPosition<bMaxCount);
			cbCardData[bPosition++]=SwitchToCardData(m_cbKingCardIndex[1]);
		}
	}
	for (BYTE i=0;i<MAX_INDEX;i++) 
	{
	
		if (cbCardIndex[i]!=0)
		{
			//����������
			if(i==m_cbKingCardIndex[0]) continue;
			if(i==m_cbKingCardIndex[1]) continue;

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
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}


//�������
LONG CGameLogic::CalScore(tagChiHuResult & ChihuResult,LONG &lScore,LONG &lTimes)
{
	//����У��
	ASSERT(ChihuResult.wChiHuKind!=CHK_NULL);
	if (ChihuResult.wChiHuKind== CHK_NULL) return 0;

	//��ֵ��ʼ��
	lTimes=1L;

	//����
	if(ChihuResult.wChiHuRight&CHR_ZI_MO)
	{
		lTimes *= 2;
	}
	//����
	if(ChihuResult.wChiHuRight&CHR_QIANG_GANG)
	{
		lTimes *= 2;
	}
	//�ܿ�
	if(ChihuResult.wChiHuRight&CHR_GANG_FLOWER)
	{
		lTimes *= 4;
	}
	//���
	if(ChihuResult.wChiHuRight&CHR_TIAN)
	{
		lTimes *= 20;
	}
	//�غ�
	if(ChihuResult.wChiHuRight&CHR_DI)
	{
		lTimes *= 20;
	}
	//����
	if(ChihuResult.wChiHuRight&CHR_KING_WAIT)
	{
		lTimes *= 2;
	}
	//�¹�
	if(ChihuResult.wChiHuRight&CHR_GERMAN)
	{
		lTimes *= 2;
	}
	//���е�
	if(ChihuResult.wChiHuRight&CHR_GERMAN_GERMAN)
	{
		lTimes *= 2;
	}
	//7��
	if(ChihuResult.wChiHuKind&CHK_QI_DUI)
	{
		lTimes *= 2;
	}
	//���߶ԣ���������
	if(ChihuResult.wChiHuKind&CHK_PENG_PENG)
	{
		lTimes *= 2;
	}
	//13��
	if(ChihuResult.wChiHuKind&CHK_THIRTEEN)
	{
		lTimes *= 2;
	}
	//����ʮ����
	if(ChihuResult.wChiHuKind&CHK_SERVEN)
	{
		lTimes *= 4;
	}
	//�¹�7�� ��7�Ժ͵¹�
	if(ChihuResult.wChiHuKind&CHK_GERMAN_SERVEN)
	{
		lTimes *= 4;
	}

	return lTimes;
}

//13��
bool CGameLogic::IsNeatAlone(BYTE cbCardIndex[MAX_INDEX],BYTE &cbGerman,BYTE cbCurrentCard,bool bZimo)
{
	BYTE cbTempCardIndex[MAX_INDEX];
	CopyMemory(cbTempCardIndex,cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//������Ŀ
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbTempCardIndex[i];
	
	//���Ʋ���ȫ����
	if(cbCardCount<14) return false;


	//����ֵ�ж�
	bool bRet=true;


	//��������
	const BYTE COMMON_TYPE_SUM = 9;
	for (BYTE i=0;i<34;i++) 
	{
		//�ظ�����
		if(cbTempCardIndex[i] > 1)
		{
			bRet=false;
			break;
		}
	}
	if(bRet)
	{
		BYTE* pKingIndex = 0;
		for(i=0; i<3; i++)
		{
			pKingIndex = &cbTempCardIndex[i*COMMON_TYPE_SUM];
			for(BYTE j=0; j<COMMON_TYPE_SUM; j++)
			{
				if(pKingIndex[j] > 0)
				{
					for(BYTE k=0; k<2; k++)
					{
						j ++;

						if(j<COMMON_TYPE_SUM)
						{
							if(pKingIndex[j] > 0)
							{
								bRet= false;
								break;
							}
						}
					}

				}
			}
		}
	}
	if(bRet)
	{
		//�¹��ж�
		cbGerman=1;

		return true;
	}

	//�¹��ж�
	cbGerman=0;

	for (BYTE i=0;i<34;i++) 
	{
		//���˾���
		if((i==m_cbKingCardIndex[0]) &&((bZimo )||(bZimo==false&&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[0]))) continue;
			
		//���˾���
		if((i==m_cbKingCardIndex[1]) &&((bZimo )||(bZimo==false&&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[1]))) continue;

		//�ظ�����
		if(cbTempCardIndex[i] > 1)
		{
			return false;
		}
	}

	BYTE* pKingIndex = 0;
	for(i=0; i<3; i++)
	{

		pKingIndex = &cbTempCardIndex[i*COMMON_TYPE_SUM];
		for(BYTE j=0; j<COMMON_TYPE_SUM; j++)
		{
			//���˾���
			if((i*COMMON_TYPE_SUM+j==m_cbKingCardIndex[0])&&((bZimo)||(bZimo==false&&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[0]))) continue;
			if((i*COMMON_TYPE_SUM+j==m_cbKingCardIndex[1])&&((bZimo)||(bZimo==false&&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[1]))) continue;

			if(pKingIndex[j] > 0)
			{
				for(BYTE k=0; k<2; k++)
				{
					j ++;
					//���˾���
					if((i*COMMON_TYPE_SUM+j==m_cbKingCardIndex[0])&&((bZimo)||(bZimo==false&&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[0]))) continue;
					if((i*COMMON_TYPE_SUM+j==m_cbKingCardIndex[1])&&((bZimo)||(bZimo==false&&SwitchToCardIndex(cbCurrentCard)!=m_cbKingCardIndex[1]))) continue;

					if(j<COMMON_TYPE_SUM)
					{
						if(pKingIndex[j] > 0)
						{
							return false;
						}
					}
				}

			}
		}
	}

	return true;
}

//3˳ 3��
bool CGameLogic::IsNeat3(const BYTE cbCardIndex[MAX_INDEX],BYTE &cbGerman)
{
	//�¹�
	cbGerman=1;

	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//��������
	BYTE cbIndex[4]={0,1,2,3};
	tagKindItem * pKindItem[4];
	ZeroMemory(&pKindItem,sizeof(pKindItem));


	//�����ж�
	BYTE cbCardCount = 0;
	for(BYTE i=0; i<MAX_INDEX; i++)
	{
		cbCardCount += cbCardIndexTemp[i];
	}
	//Ч����Ŀ
	ASSERT(cbCardCount<=14);
	if((cbCardCount%3) != 0)
	{
		return false;
	}

	BYTE cbLessKindItem = cbCardCount/3;
	if (cbLessKindItem == 0)
	{
		return true;
	}

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_INDEX-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//ͬ���ж�
		if (cbCardIndex[i]>=3)
		{
			KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
			KindItem[cbKindItemCount].cbCardIndex[0]=i;
			KindItem[cbKindItemCount].cbCardIndex[1]=i;
			KindItem[cbKindItemCount].cbCardIndex[2]=i;
			KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
		}

		//�����ж�
		if ((i<(MAX_INDEX-2-7))&&(cbCardIndex[i]>0)&&((i%9)<7))
		{
			for (BYTE j=1;j<=cbCardIndex[i];j++)
			{
				if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
				{
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
					KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
					KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
				}
			}
		}

	}
	//��������
	for (BYTE i=MAX_INDEX-7;i<MAX_INDEX-5;i++)
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];

		//���ñ���
		CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//������ ������
		for (BYTE j=1;j<=cbCardIndex[i]&&cbCardIndex[i]>0;j++)
		{
			if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
			{
				KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
				KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
				KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
			}
		}

		//���ϱ�
		if(i==MAX_INDEX-7)
		{
			for (BYTE j=1;j<=cbCardIndex[i]&&cbCardIndex[i]>0;j++)
			{
				if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+3]>=j))
				{
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
					KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
					KindItem[cbKindItemCount++].wWeaveKind=WIK_DNBL;
				}
			}

		}
		//������
		if(i==MAX_INDEX-7)
		{
			for (BYTE j=1;j<=cbCardIndex[i]&&cbCardIndex[i]>0;j++)
			{
				if ((cbCardIndex[i+2]>=j)&&(cbCardIndex[i+3]>=j))
				{
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i+2;
					KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
					KindItem[cbKindItemCount++].wWeaveKind=WIK_DXBL;
				}
			}
		}


	}	
	//�з���
	for (BYTE i=MAX_INDEX-3;i<MAX_INDEX;i++)
	{

		//�����ж�
		if ((i<(MAX_INDEX-2))&&(cbCardIndex[i]>0)&&((i%9)<7))
		{
			for (BYTE j=1;j<=cbCardIndex[i];j++)
			{
				if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
				{
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
					KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
					KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
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
			{
				pKindItem[i]=&KindItem[cbIndex[i]];
			}

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
				{
					cbCardIndexTemp[cbCardIndex]--;
				}
			}
			if(bEnoughCard)
			{
				return true;
			}

			//��������
			if (cbIndex[cbLessKindItem-1] == (cbKindItemCount-1))
			{
				for (BYTE i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1) != cbIndex[i])
					{
						BYTE cbNewIndex = cbIndex[i-1];
						for (BYTE j=(i-1); j<cbLessKindItem; j++) 
						{
							cbIndex[j]=cbNewIndex+j-i+2;
						}
						break;
					}
				}
				if (i==0)
				{
					break;
				}
			}
			else 
			{
				cbIndex[cbLessKindItem-1]++;
			}

		} while (true);

	}

	//�ǵ¹�
	cbGerman=0;

	//��������Ŀ
	BYTE cbKingCount=cbCardIndex[m_cbKingCardIndex[0]];
	BYTE cbViceKingCount=cbCardIndex[m_cbKingCardIndex[1]];

	//��������
	BYTE cbTotalKingCount=cbKingCount+cbViceKingCount;

	if(cbTotalKingCount==0) return false;
	//��������
	cbKindItemCount=0;
	ZeroMemory(KindItem,sizeof(KindItem));

	//��ַ���
	if (cbCardCount>=3)
	{
		if(cbTotalKingCount>=3)
		{
			if(cbKingCount>0&&cbViceKingCount>0)
			{
				ASSERT( cbKindItemCount < CountArray(KindItem) );
				KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(m_cbKingCardIndex[0]);
				KindItem[cbKindItemCount].cbCardIndex[0]=m_cbKingCardIndex[0];
				KindItem[cbKindItemCount].cbCardIndex[1]=m_cbKingCardIndex[0];
				KindItem[cbKindItemCount].cbCardIndex[2]=m_cbKingCardIndex[0];
				KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
			}
		}
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//ͬ���ж�
			if (cbCardIndex[i]>=3)
			{
				ASSERT( cbKindItemCount < CountArray(KindItem) );
				KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
			}
			else if(cbCardIndex[i]==1)
			{
				//���˾���
				if(IsViceKingCardIndex(i)==false &&IsViceKingCardIndex(i)==false)
				{
					if(cbTotalKingCount>=2)
					{
						ASSERT( cbKindItemCount < CountArray(KindItem) );
						KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i;
						KindItem[cbKindItemCount].cbCardIndex[2]=i;
						KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
					}
				}

			}
			else if(cbCardIndex[i]==2)
			{
				//���˾���
				if(IsViceKingCardIndex(i)==false &&IsViceKingCardIndex(i)==false)
				{
					if(cbTotalKingCount>=1)
					{
						ASSERT( cbKindItemCount < CountArray(KindItem) );
						KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i;
						KindItem[cbKindItemCount].cbCardIndex[2]=i;
						KindItem[cbKindItemCount++].wWeaveKind=WIK_PENG;
					}
				}

			}
			else
			{
				//ʲôҲ��ִ��
			}


			//�����ж�
			if ((i<(MAX_INDEX-2-7))&&((i%9)<7))
			{

				if (cbCardIndex[i]+cbCardIndex[i+1]+cbCardIndex[i+2]+cbTotalKingCount>=3)
				{
					BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+2]};
					INT cbTempTotalKingCount=cbTotalKingCount;

					while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
					{
						//����3��
						if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

						//�������
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
							}
							else 
							{
								cbTempTotalKingCount--;
							}
						}
						if( cbTempTotalKingCount >= 0 )
						{
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
						}
						else
							break;

					}
				}

			}

		}
		//��������
		for (BYTE i=MAX_INDEX-7;i<MAX_INDEX-5;i++)
		{
			//��������
			BYTE cbCardIndexTemp[MAX_INDEX];

			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

			//������ ������
			for (BYTE j=1;j<=cbCardIndex[i];j++)
			{
				if (cbCardIndex[i]+cbCardIndex[i+1]+cbCardIndex[i+2]+cbTotalKingCount>=3)
				{
					BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+2]};
					INT cbTempTotalKingCount=cbTotalKingCount;

					while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
					{
						//����3��
						if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

						//�������
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
							}
							else 
							{
								cbTempTotalKingCount--;
							}
						}
						if( cbTempTotalKingCount >= 0 )
						{
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
						}
						else
							break;

					}
				}
			}

			//���ϱ�
			if(i==MAX_INDEX-7)
			{
				if (cbCardIndex[i]+cbCardIndex[i+1]+cbCardIndex[i+3]+cbTotalKingCount>=3)
				{
					BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+3]};
					BYTE cbTempTotalKingCount=cbTotalKingCount;

					while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
					{
						//����3��
						if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

						//�������
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
							}
							else 
							{
								cbTempTotalKingCount--;
							}
						}
						if( cbTempTotalKingCount >= 0 )
						{
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_DNBL;
						}

					}
				}

			}
			//������
			if(i==MAX_INDEX-7)
			{
				if (cbCardIndex[i]+cbCardIndex[i+2]+cbCardIndex[i+3]+cbTotalKingCount>=3)
				{
					BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+2],cbCardIndex[i+3]};
					INT cbTempTotalKingCount=cbTotalKingCount;

					while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
					{
						//����3��
						if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

						//�������
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
							}
							else 
							{
								cbTempTotalKingCount--;
							}
						}
						if( cbTempTotalKingCount >= 0 )
						{
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+2;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+3;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_DXBL;
						}
						else
							break;

					}
				}

			}


		}	
		//�з���
		for (BYTE i=MAX_INDEX-3;i<MAX_INDEX;i++)
		{
			if(i==MAX_INDEX-3)			
			{
				if (cbCardIndex[i]+cbCardIndex[i+1]+cbCardIndex[i+2]+cbTotalKingCount>=3)
				{
					BYTE cbIndex[3]={cbCardIndex[i],cbCardIndex[i+1],cbCardIndex[i+2]};
					INT cbTempTotalKingCount=cbTotalKingCount;

					while (cbIndex[0]+cbIndex[1]+cbIndex[2]+cbTotalKingCount>=3)
					{
						//����3��
						if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) break;

						//�������
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
							}
							else 
							{
								cbTempTotalKingCount--;
							}
						}
						if( cbTempTotalKingCount >= 0 )
						{
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount++].wWeaveKind=WIK_LEFT;
						}
						else
							break;

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
		BYTE cbIndex[MAX_WEAVE]={0,1,2,3};
		tagKindItem * pKindItem[MAX_WEAVE];
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
					if(cbCardIndexTemp[m_cbKingCardIndex[0]]==0)
					{
						if(cbCardIndexTemp[m_cbKingCardIndex[1]]==0)
						{
							bEnoughCard=false;
							break;

						}
						else
							cbCardIndexTemp[m_cbKingCardIndex[1]]--;

					}
					else
						cbCardIndexTemp[m_cbKingCardIndex[0]]--;
				}
				else 
					cbCardIndexTemp[cbCardIndex]--;
			}
			if(bEnoughCard)
			{
				return true;
			}

			//��������
			if (cbIndex[cbLessKindItem-1] == (cbKindItemCount-1))
			{
				for (BYTE i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1) != cbIndex[i])
					{
						BYTE cbNewIndex = cbIndex[i-1];
						for (BYTE j=(i-1); j<cbLessKindItem; j++) 
						{
							cbIndex[j]=cbNewIndex+j-i+2;
						}
						break;
					}
				}
				if (i==0)
				{
					break;
				}
			}
			else 
			{
				cbIndex[cbLessKindItem-1]++;
			}



		} while (true);

	}


	return false;
}

// ���� ��������
//�Ƿ�ȫ2
bool CGameLogic::IsNeat2(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	//���ƹ���
	if(cbWeaveCount>0) return false;

	//�¹��ж�
	for(BYTE i=0; i<MAX_INDEX; i++)
	{
		if(cbCardIndex[i] != 0 && cbCardIndex[i] != 2 &&cbCardIndex[i] != 4)
		{
			return false;
		}
	}

	return true;
}
//��С����
bool CGameLogic::IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard,BYTE &cbGerman,bool bZimo)
{
	//����ж�
	if (cbWeaveCount!=0) return false;

	//������Ŀ
	BYTE cbReplaceCount = 0;

	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	
	//��Ŀ��֤
	ASSERT(GetCardCount(cbCardIndexTemp)%2==0);
	if(GetCardCount(cbCardIndexTemp)%2 !=0) return false;
	
	//��ǰ����
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//���㵥��
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbCardIndexTemp[i];

		//���ƹ���
		if( i == m_cbKingCardIndex[0] ) continue;
		if( i == m_cbKingCardIndex[1] ) continue;

		//����ͳ��
		if( cbCardCount == 1 || cbCardCount == 3 ) 	cbReplaceCount++;
	}
    
	//�����ж�
	if(bZimo==true)
	{

		//���Ʋ���
		if( cbReplaceCount > (cbCardIndexTemp[m_cbKingCardIndex[0]]+cbCardIndexTemp[m_cbKingCardIndex[1]])) return false;

		//�¹�7��
		if ((cbReplaceCount==0)&&(cbCardIndexTemp[m_cbKingCardIndex[0]]%2==0)&&(cbCardIndexTemp[m_cbKingCardIndex[1]]%2==0 ))
		{
			cbGerman=1;
		}
		return true;
	}

	//�����ж�  ����
	if(bZimo==false &&(IsKingCardIndex(cbCurrentIndex)||IsViceKingCardIndex(cbCurrentIndex)))
	{
		//���Ʋ���
		if( (cbReplaceCount+2) > (cbCardIndexTemp[m_cbKingCardIndex[0]]+cbCardIndexTemp[m_cbKingCardIndex[1]])) return false;

		//�¹�7��
		if ((cbReplaceCount==0)&&(cbCardIndexTemp[m_cbKingCardIndex[0]]%2==0)&&(cbCardIndexTemp[m_cbKingCardIndex[1]]%2==0 ))
		{
			cbGerman=1;
		}

	}

	//�����ж�  ����
	if(bZimo==false &&((!IsKingCardIndex(cbCurrentIndex))&&(!IsViceKingCardIndex(cbCurrentIndex))))
	{
		//���Ʋ���
		if( (cbReplaceCount) > (cbCardIndexTemp[m_cbKingCardIndex[0]]+cbCardIndexTemp[m_cbKingCardIndex[1]])) return false;

		//�¹�7��
		if ((cbReplaceCount==0)&&(cbCardIndexTemp[m_cbKingCardIndex[0]]%2==0)&&(cbCardIndexTemp[m_cbKingCardIndex[1]]%2==0 ))
		{
			cbGerman=1;
		}

	}

	return true;

}

//7��
bool CGameLogic::ServenStar(BYTE cbCardIndex[MAX_INDEX],BYTE &cbGerman,BYTE cbCurrentCard,bool bZimo)
{

	cbGerman=0;
	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//ȫ����
	bool bRet = false;
	bool bAlone=IsNeatAlone(cbCardIndexTemp,cbGerman,cbCurrentCard,bZimo);
	if (bAlone==false)
	{
		return false;
	}

	//7���¼
	BYTE cbHasFeng[7];
	ZeroMemory(cbHasFeng,sizeof(cbHasFeng));
	for(int i=27; i<34; i++)
	{
		cbHasFeng[i-27]=cbCardIndexTemp[i];

		//�ظ�����
		if(cbHasFeng[i-27]>1) return false;
	}

	//7��ȫ
	int j=0;
	for (int i=0;i<7;i++)
	{
		if(cbHasFeng[i]>0)
			j++;
	}
	if(j==7)
	{
		return true;
	}
	return false;

}
//���㸱��
BYTE CGameLogic::GetKingFromBrother(BYTE cbBrotherData)
{
	if(IsValidCard(cbBrotherData) == false)
	{
		return 0xFF;
	}
	BYTE cbColor = cbBrotherData & MASK_COLOR;
	BYTE cbValue = (cbBrotherData & MASK_VALUE) + 1;

	//�� �� Ͳ
	if(cbColor != 0x30)
	{
		if(cbValue > 0x09)
		{
			cbValue = 0x01;
		}
	}
	else if(cbValue < 0x06)
	{
		if(cbValue > 0x04)
		{
			cbValue = 0x01;
		}
	}
	else
	{
		if(cbValue > 0x07)
		{
			cbValue = 0x05;
		}
	}

	return (cbColor | cbValue);
}
//////////////////////////////////////////////////////////////////////////
