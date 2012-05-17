#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE const CGameLogic::m_cbCardDataArray[80]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//Сд
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//��д
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//��д
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//��д
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//��д
};

/*BYTE const CGameLogic::m_cbCardDataArray[80]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x07,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x08,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x09,					//Сд
	0x01,0x02,0x03,0x04,0x05,0x06,0x0A,0x0A,0x0A,0x0A,					//Сд
	0x11,0x12,0x13,0x14,0x15,0x13,0x17,0x18,0x19,0x17,					//��д
	0x11,0x12,0x13,0x14,0x15,0x14,0x17,0x18,0x19,0x18,					//��д
	0x11,0x12,0x13,0x14,0x15,0x15,0x17,0x18,0x19,0x19,					//��д
	0x11,0x12,0x16,0x16,0x16,0x16,0x1A,0x1A,0x1A,0x1A,					//��д
};*/

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
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE bMaxCount)
{
	//����׼��
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bMaxCount-bRandCount);
		cbCardData[bRandCount++]=cbCardDataTemp[bPosition];
		cbCardDataTemp[bPosition]=cbCardDataTemp[bMaxCount-bRandCount];
	} while (bRandCount<bMaxCount);

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
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE bRemoveCount)
{
	//ɾ���˿�
	for (BYTE i=0;i<bRemoveCount;i++)
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
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE bCardCount, BYTE cbRemoveCard[], BYTE bRemoveCount)
{
	//��������
	ASSERT(bCardCount<=MAX_COUNT);
	ASSERT(bRemoveCount<=bCardCount);

	//�������
	BYTE bDeleteCount=0,cbTempCardData[MAX_COUNT];
	if (bCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,bCardCount*sizeof(cbCardData[0]));

	//�����˿�
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

	//�ɹ��ж�
	if (bDeleteCount!=bRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//�����˿�
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[bCardPos++]=cbTempCardData[i];
	}

	return true;
}

//�����ж�
BYTE CGameLogic::GetAcitonTiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbTiCardIndex[5])
{
	//��������
	BYTE cbTiCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]==4) cbTiCardIndex[cbTiCardCount++]=i;
	}

	return cbTiCardCount;
}

//η���ж�
BYTE CGameLogic::GetActionWeiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbWeiCardIndex[7])
{
	//η������
	BYTE cbWeiCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]==3) cbWeiCardIndex[cbWeiCardCount++]=i;
	}

	return cbWeiCardCount;
}

//�����ж�
BYTE CGameLogic::GetActionChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, tagChiCardInfo ChiCardInfo[6])
{
	//Ч���˿�
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return 0;

	//��������
	BYTE cbChiCardCount=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//�����ж�
	if (cbCardIndex[cbCurrentIndex]>=3) return cbChiCardCount;

	//��С���
	BYTE cbReverseIndex=(cbCurrentIndex+10)%MAX_INDEX;
	if ((cbCardIndex[cbCurrentIndex]>=1)&&(cbCardIndex[cbReverseIndex]>=1)&&(cbCardIndex[cbReverseIndex]!=3))
	{
		//�����˿�
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//ɾ���˿�
		cbCardIndexTemp[cbCurrentIndex]--;
		cbCardIndexTemp[cbReverseIndex]--;

		//��ȡ�ж�
		BYTE cbResultCount=1;
		while (cbCardIndexTemp[cbCurrentIndex]>0)
		{
			PBYTE pcbResult=ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
			if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,pcbResult)!=CK_NULL) cbResultCount++;
			else break;
		}

		//���ý��
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

	//��С���
	if (cbCardIndex[cbReverseIndex]==2)
	{
		//�����˿�
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//ɾ���˿�
		cbCardIndexTemp[cbReverseIndex]-=2;

		//��ȡ�ж�
		BYTE cbResultCount=1;
		while (cbCardIndexTemp[cbCurrentIndex]>0)
		{
			PBYTE pcbResult=ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
			if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,pcbResult)!=CK_NULL) cbResultCount++;
			else break;
		}

		//���ý��
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

	//����ʮ��
	BYTE bCardValue=cbCurrentCard&MASK_VALUE;
	if ((bCardValue==0x02)||(bCardValue==0x07)||(bCardValue==0x0A))
	{
		//��������
		BYTE cbExcursion[]={1,6,9};
		BYTE cbInceptIndex=((cbCurrentCard&MASK_COLOR)==0x00)?0:10;

		//�����ж�
		for (BYTE i=0;i<CountArray(cbExcursion);i++)
		{
			BYTE cbIndex=cbInceptIndex+cbExcursion[i];
			if ((cbIndex!=cbCurrentIndex)&&((cbCardIndex[cbIndex]==0)||(cbCardIndex[cbIndex]==3))) break;
		}

		//��ȡ�ж�
		if (i==CountArray(cbExcursion))
		{
			//�����˿�
			BYTE cbCardIndexTemp[MAX_INDEX];
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

			//ɾ���˿�
			for (BYTE j=0;j<CountArray(cbExcursion);j++)
			{
				BYTE cbIndex=cbInceptIndex+cbExcursion[j];
				if (cbIndex!=cbCurrentIndex) cbCardIndexTemp[cbIndex]--;
			}

			//��ȡ�ж�
			BYTE cbResultCount=1;
			while (cbCardIndexTemp[cbCurrentIndex]>0)
			{
				PBYTE pcbResult=ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
				if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,pcbResult)!=CK_NULL) cbResultCount++;
				else break;
			}

			//���ý��
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

	//˳������
	BYTE cbExcursion[3]={0,1,2};
	for (BYTE i=0;i<CountArray(cbExcursion);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%10;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=7))
		{
			//��������
			BYTE cbFirstIndex=cbCurrentIndex-cbExcursion[i];

			//�����ж�
			for (BYTE j=0;j<3;j++)
			{
				BYTE cbIndex=cbFirstIndex+j;
				if ((cbIndex!=cbCurrentIndex)&&((cbCardIndex[cbIndex]==0)||(cbCardIndex[cbIndex]==3))) break;
			}

			//��ȡ�ж�
			if (j==CountArray(cbExcursion))
			{
				//�����˿�
				BYTE cbCardIndexTemp[MAX_INDEX];
				CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

				//ɾ���˿�
				for (BYTE j=0;j<3;j++)
				{
					BYTE cbIndex=cbFirstIndex+j;
					if (cbIndex!=cbCurrentIndex) cbCardIndexTemp[cbIndex]--;
				}

				//��ȡ�ж�
				BYTE cbResultCount=1;
				while (cbCardIndexTemp[cbCurrentIndex]>0)
				{
					PBYTE pcbResult=ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
					if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,pcbResult)!=CK_NULL) cbResultCount++;
					else break;
				}

				//���ý��
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

//�Ƿ����
bool CGameLogic::IsChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//Ч���˿�
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return false;

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//�����˿�
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//��ȡ�ж�
	while (cbCardIndexTemp[cbCurrentIndex]>0)
	{
		BYTE cbResult[3];
		if (TakeOutChiCard(cbCardIndexTemp,cbCurrentCard,cbResult)==CK_NULL) break;
	}

	return (cbCardIndexTemp[cbCurrentIndex]==0);
}

//�Ƿ�����
bool CGameLogic::IsTiPaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//Ч���˿�
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return false;

	//ת������
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//�����ж�
	return (cbCardIndex[cbCurrentIndex]==3)?true:false;
}

//�Ƿ�����
bool CGameLogic::IsWeiPengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//Ч���˿�
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return false;

	//ת������
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//�����ж�
	return (cbCardIndex[cbCurrentIndex]==2)?true:false;
}

//���ƽ��
bool CGameLogic::GetHuCardInfo(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbHuXiWeave, tagHuCardInfo & HuCardInfo)
{
	//��������
	static CAnalyseItemArray AnalyseItemArray;	

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//���ý��
	ZeroMemory(&HuCardInfo,sizeof(HuCardInfo));	

	//��ȡ����
	if ((cbCurrentCard!=0)&&(IsWeiPengCard(cbCardIndexTemp,cbCurrentCard)==true))
	{	
		//�жϺ���	
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
		AnalyseItemArray.RemoveAll();
		if (AnalyseCard(cbCardIndexTemp,AnalyseItemArray)==true)
		{			
			//Ѱ������
			BYTE cbHuXiCard=0;
			INT_PTR nBestItem=-1;
			for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
			{
				//��ȡ����
				tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

				//��Ϣ����
				if (pAnalyseItem->cbHuXiCount>=cbHuXiCard)
				{
					nBestItem=i;
					cbHuXiCard=pAnalyseItem->cbHuXiCount;
				}
			}
			HuCardInfo.cbHuXiCount+=cbHuXiCard;

			//���ý��
			if (nBestItem>=0)
			{
				//��ȡ����
				tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[nBestItem];

				//���ñ���
				HuCardInfo.cbCardEye=pAnalyseItem->cbCardEye;

				//�������
				for (BYTE i=0;i<pAnalyseItem->cbWeaveCount;i++)
				{
					BYTE cbIndex=HuCardInfo.cbWeaveCount++;
					HuCardInfo.WeaveItemArray[cbIndex]=pAnalyseItem->WeaveItemArray[i];
				}
			}

			if((HuCardInfo.cbHuXiCount+cbHuXiWeave)>=15)
				return true;				
		}

		//�������
		BYTE cbIndex=HuCardInfo.cbWeaveCount++;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardCount=3;
		HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind=ACK_PENG;
		HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard=cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0]=cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1]=cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2]=cbCurrentCard;	

		//ɾ���˿�
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]=0;

		//���ú�Ϣ
		HuCardInfo.cbHuXiCount+=GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
	}
	else if (cbCurrentCard!=0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//��ȡ����
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndexTemp[i]==3)
		{
			//�����˿�
			cbCardIndexTemp[i]=0;

			//�������
			BYTE cbCardData=SwitchToCardData(i);
			BYTE cbIndex=HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardCount=3;
			HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind=ACK_WEI;
			HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard=cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0]=cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1]=cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2]=cbCardData;

			//���ú�Ϣ
			HuCardInfo.cbHuXiCount+=GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		}
	}

	//�����˿�
	AnalyseItemArray.RemoveAll();
	if (AnalyseCard(cbCardIndexTemp,AnalyseItemArray)==false) return false;

	//Ѱ������
	BYTE cbHuXiCard=0;
	INT_PTR nBestItem=-1;
	for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
	{
		//��ȡ����
		tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

		//��Ϣ����
		if (pAnalyseItem->cbHuXiCount>=cbHuXiCard)
		{
			nBestItem=i;
			cbHuXiCard=pAnalyseItem->cbHuXiCount;
		}
	}
	HuCardInfo.cbHuXiCount+=cbHuXiCard;

	//���ý��
	if (nBestItem>=0)
	{
		//��ȡ����
		tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[nBestItem];

		//���ñ���
		HuCardInfo.cbCardEye=pAnalyseItem->cbCardEye;

		//�������
		for (BYTE i=0;i<pAnalyseItem->cbWeaveCount;i++)
		{
			BYTE cbIndex=HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex]=pAnalyseItem->WeaveItemArray[i];
		}
	}

	return ((HuCardInfo.cbHuXiCount+cbHuXiWeave)>=15);
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return ((cbValue>=1)&&(cbValue<=10)&&(cbColor<=1));
}

//�˿���Ŀ
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCount+=cbCardIndex[i];

	return cbCount;
}

//��ȡ��Ϣ
BYTE CGameLogic::GetWeaveHuXi(const tagWeaveItem & WeaveItem)
{
	//�����Ϣ
	switch (WeaveItem.cbWeaveKind)
	{
	case ACK_TI:	//��
		{
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?12:9;
		}
	case ACK_PAO:	//��
		{
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?9:6;
		}
	case ACK_WEI:	//��
		{
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?6:3;
		}
	case ACK_PENG:	//��
		{
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?3:1;
		}
	case ACK_CHI:	//��
		{
			//��ȡ��ֵ
			BYTE cbValue1=WeaveItem.cbCardList[0]&MASK_VALUE;
			BYTE cbValue2=WeaveItem.cbCardList[1]&MASK_VALUE;
			BYTE cbValue3=WeaveItem.cbCardList[2]&MASK_VALUE;

			//һ������
			if ((cbValue1==1)&&(cbValue2==2)&&(cbValue3==3)) return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?6:3;

			//����ʮ��
			if ((cbValue1==2)&&(cbValue2==7)&&(cbValue3==10)) return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?6:3;

			return 0;
		}
	}

	return 0;
}

//�����˿�
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], CAnalyseItemArray & AnalyseItemArray)
{
	//��������
	BYTE cbWeaveItemCount=0;
	tagWeaveItem WeaveItem[76];
	ZeroMemory(WeaveItem,sizeof(WeaveItem));

	//��Ŀͳ��
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	//��Ŀ�ж�
	if (cbCardCount==0) return true;
	if ((cbCardCount%3!=0)&&((cbCardCount+1)%3!=0)) return false;

	//�������
	BYTE bLessWeavItem=cbCardCount/3;
	bool bNeedCardEye=((cbCardCount+1)%3==0);

	//�����ж�
	if ((bLessWeavItem==0)&&(bNeedCardEye==true))	
	{
		//�����ж�
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if (cbCardIndex[i]==2)
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				AnalyseItem.cbHuXiCount=0;
				AnalyseItem.cbWeaveCount=0;
				AnalyseItem.cbCardEye=SwitchToCardData(i);

				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
			
		}
		

		return false;
	}

	//��ִ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//��������
		if (cbCardIndex[i]==0) continue;

		//��������
		BYTE cbCardData=SwitchToCardData(i);

		//��С���
		if ((cbCardIndex[i]==2)&&(cbCardIndex[(i+10)%MAX_INDEX]>=1))
		{
			WeaveItem[cbWeaveItemCount].cbCardCount=3;
			WeaveItem[cbWeaveItemCount].cbWeaveKind=ACK_CHI;
			WeaveItem[cbWeaveItemCount].cbCenterCard=cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[0]=cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[1]=cbCardData;
			WeaveItem[cbWeaveItemCount++].cbCardList[2]=(cbCardData+16)%32;
		}

		//��С���
		if ((cbCardIndex[i]>=1)&&(cbCardIndex[(i+10)%MAX_INDEX]==2))
		{
			WeaveItem[cbWeaveItemCount].cbCardCount=3;
			WeaveItem[cbWeaveItemCount].cbWeaveKind=ACK_CHI;
			WeaveItem[cbWeaveItemCount].cbCenterCard=cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[0]=cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[1]=(cbCardData+16)%32;
			WeaveItem[cbWeaveItemCount++].cbCardList[2]=(cbCardData+16)%32;
		}

		//����ʮ��
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

		//˳���ж�
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

	//��Ϸ���
	if (cbWeaveItemCount>=bLessWeavItem)	
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[7]={0,1,2,3,4,5,6};
		tagWeaveItem * pWeaveItem[CountArray(cbIndex)];

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<bLessWeavItem;i++) pWeaveItem[i]=&WeaveItem[cbIndex[i]];

			//�����ж�
			bool bEnoughCard=true;
			for (BYTE i=0;i<bLessWeavItem*3;i++)
			{
				//�����ж�
				BYTE cbIndex=SwitchToCardIndex(pWeaveItem[i/3]->cbCardList[i%3]);
				if (cbCardIndexTemp[cbIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else cbCardIndexTemp[cbIndex]--;
			}

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
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

				//�������
				if ((bNeedCardEye==false)||(cbCardEye!=0))
				{
					//��������
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//���ý��
					AnalyseItem.cbHuXiCount=0;
					AnalyseItem.cbCardEye=cbCardEye;
					AnalyseItem.cbWeaveCount=bLessWeavItem;

					//�������
					for (BYTE i=0;i<bLessWeavItem;i++)
					{
						AnalyseItem.WeaveItemArray[i]=*pWeaveItem[i];
						AnalyseItem.cbHuXiCount+=GetWeaveHuXi(AnalyseItem.WeaveItemArray[i]);
					}

					//������
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//��������
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

//��ȡ����
BYTE CGameLogic::TakeOutChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbResultCard[3])
{
	//Ч���˿�
	ASSERT(cbCurrentCard!=0);
	if (cbCurrentCard==0) return 0;

	//��������
	BYTE cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//��С���
	BYTE cbReverseIndex=(cbCurrentIndex+10)%MAX_INDEX;
	if ((cbCardIndex[cbCurrentIndex]>=2)&&(cbCardIndex[cbReverseIndex]>=1)&&(cbCardIndex[cbReverseIndex]!=3))
	{
		//ɾ���˿�
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbReverseIndex]--;

		//���ý��
		cbResultCard[0]=cbCurrentCard;
		cbResultCard[1]=cbCurrentCard;
		cbResultCard[2]=SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR)==0x00)?CK_XXD:CK_XDD;
	}

	//��С���
	if (cbCardIndex[cbReverseIndex]==2)
	{
		//ɾ���˿�
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbReverseIndex]-=2;

		//���ý��
		cbResultCard[0]=cbCurrentCard;
		cbResultCard[1]=SwitchToCardData(cbReverseIndex);
		cbResultCard[2]=SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR)==0x00)?CK_XDD:CK_XXD;
	}

	//����ʮ��
	BYTE bCardValue=(cbCurrentCard&MASK_VALUE);
	if ((bCardValue==0x02)||(bCardValue==0x07)||(bCardValue==0x0A))
	{
		//��������
		BYTE cbExcursion[]={1,6,9};
		BYTE cbInceptIndex=((cbCurrentCard&MASK_COLOR)==0x00)?0:10;

		//�����ж�
		for (BYTE i=0;i<CountArray(cbExcursion);i++)
		{
			BYTE cbIndex=cbInceptIndex+cbExcursion[i];
			if ((cbCardIndex[cbIndex]==0)||(cbCardIndex[cbIndex]==3)) break;
		}

		//�ɹ��ж�
		if (i==CountArray(cbExcursion))
		{
			//ɾ���˿�
			cbCardIndex[cbInceptIndex+cbExcursion[0]]--;
			cbCardIndex[cbInceptIndex+cbExcursion[1]]--;
			cbCardIndex[cbInceptIndex+cbExcursion[2]]--;

			//���ý��
			cbResultCard[0]=SwitchToCardData(cbInceptIndex+cbExcursion[0]);
			cbResultCard[1]=SwitchToCardData(cbInceptIndex+cbExcursion[1]);
			cbResultCard[2]=SwitchToCardData(cbInceptIndex+cbExcursion[2]);

			return CK_EQS;
		}
	}

	//˳���ж�
	BYTE cbExcursion[3]={0,1,2};
	for (BYTE i=0;i<CountArray(cbExcursion);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%10;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=7))
		{
			//�����ж�
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];
			if ((cbCardIndex[cbFirstIndex]==0)||(cbCardIndex[cbFirstIndex]==3)) continue;
			if ((cbCardIndex[cbFirstIndex+1]==0)||(cbCardIndex[cbFirstIndex+1]==3)) continue;
			if ((cbCardIndex[cbFirstIndex+2]==0)||(cbCardIndex[cbFirstIndex+2]==3)) continue;

			//ɾ���˿�
			cbCardIndex[cbFirstIndex]--;
			cbCardIndex[cbFirstIndex+1]--;
			cbCardIndex[cbFirstIndex+2]--;

			//���ý��
			cbResultCard[0]=SwitchToCardData(cbFirstIndex);
			cbResultCard[1]=SwitchToCardData(cbFirstIndex+1);
			cbResultCard[2]=SwitchToCardData(cbFirstIndex+2);

			BYTE cbChiKind[3]={CK_LEFT,CK_CENTER,CK_RIGHT};
			return cbChiKind[i];
		}
	}

	return CK_NULL;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<MAX_INDEX);
	return ((cbCardIndex/10)<<4)|(cbCardIndex%10+1);
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*10+(cbCardData&MASK_VALUE)-1;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[], BYTE bMaxCount)
{
	//ת���˿�
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

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE bCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//ת���˿�
	for (BYTE i=0;i<bCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return bCardCount;
}

//////////////////////////////////////////////////////////////////////////
