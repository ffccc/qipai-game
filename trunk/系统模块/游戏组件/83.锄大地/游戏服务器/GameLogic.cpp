#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//��������
const BYTE cbIndexCount=5;

//�˿�����
const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
};

const BYTE CGameLogic::m_cbBigCardData[BIG_COUNT]=
{
	0x01,0x02,0x0A,0x0B,0x0C,0x0D,
	0x11,0x12,0x1A,0x1B,0x1C,0x1D,
	0x21,0x22,0x2A,0x2B,0x2C,0x2D,
	0x31,0x32,0x3A,0x3B,0x3C,0x3D
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

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ч����
	if ((cbCardCount==0)||(cbCardCount>5)) return CT_ERROR;

	//������
	if (cbCardCount<=4)
	{
		//��ȡ����
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[0]);
		BYTE cbCardType[]={CT_SINGLE,CT_DOUBLE,CT_THREE_TIAO,CT_FOUR_TIAO};

		//ͬ�ƶԱ�
		for (BYTE i=1;i<cbCardCount;i++)
		{
			if (GetCardLogicValue(cbCardData[i])!=cbLogicValue) return CT_ERROR;
		}

		return cbCardType[cbCardCount-1];
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//˳������
	if (AnalyseResult.cbCardCount[0]==5)
	{
		//��������
		BYTE cbCardColor=GetCardColor(cbCardData,cbCardCount);
		bool bStructureLink=IsStructureLink(cbCardData,cbCardCount);

		//�����ж�
		if ((cbCardColor==0xF0)&&(bStructureLink==true)) return CT_SHUN_ZI;
		if ((cbCardColor!=0xF0)&&(bStructureLink==false)) return CT_TONG_HUA;
		if ((cbCardColor!=0xF0)&&(bStructureLink==true)) return CT_TONG_HUA_SHUN;
	}
	else
	{
		//�����ж�
		if ((AnalyseResult.cbCardCount[2]==1)&&(AnalyseResult.cbCardCount[1]==1)) return CT_HU_LU;
		if ((AnalyseResult.cbCardCount[3]==1)&&(AnalyseResult.cbCardCount[0]==1)) return CT_TIE_ZHI;
	}

	return CT_ERROR;
}

//��ȡ��ɫ
BYTE CGameLogic::GetCardColor(const BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч�����
	ASSERT(cbCardCount>0);
	if (cbCardCount==0) return 0xF0;

	//���ƻ�ɫ
	BYTE cbCardColor=GetCardColor(cbCardData[0]);

	//��ɫ�ж�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (GetCardColor(cbCardData[i])!=cbCardColor) return 0xF0;
	}

	return cbCardColor;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[FULL_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) 
	{
		if (cbSortType==ST_ORDER) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
		else cbSortValue[i]=GetCardColor(cbCardData[i])+GetCardLogicValue(cbCardData[i]);
	}

	//�������
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
				//���ñ�־
				bSorted=false;

				//�˿�����
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;

				//����Ȩλ
				cbSwitchData=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardData)];
	CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//�����˿�
void CGameLogic::RandCard(const BYTE bCardData[],BYTE bCardCount,BYTE bCardBuffer[],BYTE bBufferCount)
{
	//����׼��
	BYTE cbCardData[FULL_COUNT];
	CopyMemory(cbCardData,bCardData,sizeof(BYTE)*bCardCount);

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(bBufferCount-cbRandCount);
		bCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[bBufferCount-cbRandCount];
	} while (cbRandCount<bBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(cbRemoveCount<=cbCardCount);
	if( cbRemoveCount > cbCardCount )
		return false;

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[FULL_COUNT];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
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
	if (cbDeleteCount!=cbRemoveCount) return false;

	//�����˿�
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
	//��ȡ����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//��Ч�ж�
	if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) return true;

	return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstCard, BYTE cbNextCard)
{
	//��ȡ��ֵ
	BYTE cbNextValue=GetCardLogicValue(cbNextCard);
	BYTE cbFirstValue=GetCardLogicValue(cbFirstCard);

	//��ֵ�ж�
	if (cbFirstValue==cbNextValue)
	{
		//��ɫ�ж�
		BYTE cbNextColor=GetCardColor(cbNextCard);
		BYTE cbFirstColor=GetCardColor(cbFirstCard);

		return (cbNextColor>cbFirstColor);
	}
		
	return (cbNextValue>cbFirstValue);
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//��Ŀ�ж�
	if (cbFirstCount!=cbNextCount) return false;

	//�����ж�
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);
	if (cbFirstType!=cbNextType) return (cbNextType>cbFirstType);

	//��ʼ�Ա�
	switch (cbFirstType)
	{
	case CT_SINGLE:			//��������
	case CT_DOUBLE:			//��������
	case CT_THREE_TIAO:		//��������
	case CT_FOUR_TIAO:		//��������
	case CT_SHUN_ZI:		//˳������
	case CT_TONG_HUA:		//ͬ������
	case CT_TONG_HUA_SHUN:	//ͬ��˳��
		{
			return CompareCard(cbFirstCard[0],cbNextCard[0]);
		}
	case CT_HU_LU:			//��«����
		{
			return CompareCard(cbFirstCard[2],cbNextCard[2]);
		}
	case CT_TIE_ZHI:		//��֧����
		{
			return CompareCard(cbFirstCard[1],cbNextCard[1]);
		}
	}

	//�������
	ASSERT(FALSE);
	
	return false;
}

//��������
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
	//���ý��
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//�����ж�
	if (cbTurnCardCount!=0)
	{
		//��ȡ����
		BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

		//������
		switch (cbTurnOutType)
		{
		case CT_SINGLE:					//��������,�����
			{
				//�����˿�
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

				//��ȡ��ֵ
				BYTE cbTurnCard=cbTurnCardData[0];

				//Ѱ���˿�
				for (BYTE i=cbTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
				{
					if( AnalyseResult.cbCardCount[i] == 0 ) continue;
					if (CompareCard(cbTurnCard,AnalyseResult.cbCardData[i][0])==false) continue;
					else cbTurnCard = AnalyseResult.cbCardData[i][0];	
				}
				if( cbTurnCard != cbTurnCardData[0] )
				{
					//���ý��
					OutCardResult.cbCardCount=cbTurnCardCount;
					OutCardResult.cbResultCard[0] = cbTurnCard;
					return true;
				}
				return false;
			}
		case CT_DOUBLE:					//��������
		case CT_THREE_TIAO:				//��������
		case CT_FOUR_TIAO:				//��������
			{										//����С
				//�����˿�
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

				//��ȡ��ֵ
				BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

				//Ѱ���˿�
				for (BYTE i=cbTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
				{
					for (BYTE j=0;j<AnalyseResult.cbCardCount[i];j++)
					{
						//��С�Ա�
						BYTE cbIndex=(AnalyseResult.cbCardCount[i]-j-1)*(i+1);
						if (CompareCard(cbTurnCardData[0],AnalyseResult.cbCardData[i][cbIndex])==false) continue;

						//���ý��
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][cbIndex],sizeof(BYTE)*cbTurnCardCount);
						SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );

						return true;
					}
				}

				return false;
			}
		}

		//�������
		if (cbCardCount>=5)
		{
			//�����ֲ�
			tagDistributing Distributing;
			AnalysebDistributing(cbCardData,cbCardCount,Distributing);

			//����˳��
			if (cbTurnOutType<=CT_SHUN_ZI)
			{
				//����˳��
				BYTE cbReferCard=(cbTurnOutType==CT_SHUN_ZI)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchShunZi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//����ͬ��
			if (cbTurnOutType<=CT_TONG_HUA)
			{
				//����ͬ��
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHua(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//������«
			if (cbTurnOutType<=CT_HU_LU)
			{
				//������«
				BYTE cbReferCard=(cbTurnOutType==CT_HU_LU)?cbTurnCardData[2]:0x00;
				OutCardResult.cbCardCount=SearchHuLu(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//������֧
			if (cbTurnOutType<=CT_TIE_ZHI)
			{
				//������֧
				BYTE cbReferCard=(cbTurnOutType==CT_TIE_ZHI)?cbTurnCardData[1]:0x00;
				OutCardResult.cbCardCount=SearchTieZhi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//����ͬ��˳
			if (cbTurnOutType<=CT_TONG_HUA_SHUN)
			{
				//����ͬ��˳
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA_SHUN)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHuaShun(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}
		}

		return false;
	}
	else
	{
		//��ȡ��ֵ
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[cbCardCount-1]);

		//�����ж�
		BYTE cbSameCount=1;
		for (BYTE i=1;i<cbCardCount;i++)
		{
			if (GetCardLogicValue(cbCardData[cbCardCount-i-1])==cbLogicValue) cbSameCount++;
			else break;
		}

		//��ɴ���
		if (cbSameCount>1)
		{
			OutCardResult.cbCardCount=cbSameCount;
			for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=cbCardData[cbCardCount-1-j];
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//���ƴ���
		OutCardResult.cbCardCount=1;
		OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-1];

		return true;
	}

	return false;
}

//������«
BYTE CGameLogic::SearchHuLu(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//��������
	BYTE cbThreeIndex=0xFF;
	BYTE cbDoubleIndex=0xFF;

	//������«
	for (BYTE cbValueIndex=0;cbValueIndex<12;cbValueIndex++)
	{
		//��������
		if ((cbThreeIndex==0xFF)&&(Distributing.cbDistributing[cbValueIndex][5]>=3))
		{
			//��С�ж�
			BYTE cbHeadCard=MakeCardData(cbValueIndex,0);
			if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
			{
				cbThreeIndex=cbValueIndex;
				cbValueIndex = 0;
				continue;
			}
		}

		//��������
		if ((cbDoubleIndex==0xFF)&&cbValueIndex!=cbThreeIndex&&(Distributing.cbDistributing[cbValueIndex][5]>=2))
		{
			cbDoubleIndex=cbValueIndex;
			continue;
		}

		//����ж�
		if ((cbThreeIndex!=0xFF)&&(cbDoubleIndex!=0xFF)) break;
	}

	//�����«
	if ((cbThreeIndex!=0xFF)&&(cbDoubleIndex!=0xFF))
	{
		//��������
		BYTE cbThreeCount=0;
		BYTE cbDoubleCount=0;

		//��������
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//�˿��ж�
			if (Distributing.cbDistributing[cbThreeIndex][cbColorIndex]==0) continue;
			
			//�����˿�
			BYTE cbIndex= 0;
			cbResultCard[cbIndex+cbThreeCount++]=MakeCardData(cbThreeIndex,cbColorIndex);

			//����ж�
			if (cbThreeCount==3) break;
		}

		//�������
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//�˿��ж�
			if (Distributing.cbDistributing[cbDoubleIndex][cbColorIndex]==0) continue;
			
			//�����˿�
			BYTE cbIndex= 3;
			cbResultCard[cbIndex+cbDoubleCount++]=MakeCardData(cbDoubleIndex,cbColorIndex);

			//����ж�
			if (cbDoubleCount==2) break;
		}

		return cbThreeCount+cbDoubleCount;
	}

	return 0;
}

//������֧
BYTE CGameLogic::SearchTieZhi(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//��������
	BYTE cbFourIndex=0xFF;
	BYTE cbSignedIndex=0xFF;

	//������«
	for (BYTE cbValueIndex=0;cbValueIndex<12;cbValueIndex++)
	{
		//��������
		if ((cbFourIndex==0xFF)&&(Distributing.cbDistributing[cbValueIndex][5]>=4))
		{
			//��С�ж�
			BYTE cbHeadCard=MakeCardData(cbValueIndex,0);
			if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
			{
				cbFourIndex=cbValueIndex;
				continue;
			}
		}

		//��������
		if ((cbSignedIndex==0xFF)&&(Distributing.cbDistributing[cbValueIndex][5]>=1))
		{
			cbSignedIndex=cbValueIndex;
			continue;
		}

		//����ж�
		if ((cbFourIndex!=0xFF)&&(cbSignedIndex!=0xFF)) break;
	}

	//�����«
	if ((cbFourIndex!=0xFF)&&(cbSignedIndex!=0xFF))
	{
		//��������
		BYTE cbFourCount=0;
		BYTE cbSignedCount=0;

		//��������
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//�˿��ж�
			if (Distributing.cbDistributing[cbFourIndex][3-cbColorIndex]==0) continue;
			
			//�����˿�
			BYTE cbIndex= 0;
			cbResultCard[cbIndex+cbFourCount++]=MakeCardData(cbFourIndex,3-cbColorIndex);

			//����ж�
			if (cbFourCount==4) break;
		}

		//���쵥��
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//�˿��ж�
			if (Distributing.cbDistributing[cbSignedIndex][cbColorIndex]==0) continue;
			
			//�����˿�
			BYTE cbIndex= 4;
			cbResultCard[cbIndex+cbSignedCount++]=MakeCardData(cbSignedIndex,cbColorIndex);

			//����ж�
			if (cbSignedCount==1) break;
		}

		return cbFourCount+cbSignedCount;
	}

	return 0;
}

//����˳��
BYTE CGameLogic::SearchShunZi(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//��������
	BYTE cbLinkCount=0;
	BYTE cbResultCount=0;

	//��ͨ˳��
	for (BYTE cbValueIndex=0;cbValueIndex<12;cbValueIndex++)
	{
		//�����ж�
		if (Distributing.cbDistributing[cbValueIndex][5]==0)
		{
			cbLinkCount=0;
			continue;
		}

		//�����ж�
		if ((++cbLinkCount)>=5)
		{
			//��ȡ��ɫ
			BYTE cbHeadColor=0xFF;
			for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
			{
				if (Distributing.cbDistributing[cbValueIndex][3-cbColorIndex]>0)
				{
					cbHeadColor=3-cbColorIndex;
					break;
				}
			}

			//����˳��
			BYTE cbHeadCard=MakeCardData(cbValueIndex,cbHeadColor);
			if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
			{
				//����˳��
				for (BYTE k=0;k<5;k++)
				{
					for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
					{
						BYTE bTempColorIndex = (k == 0 ? 3-cbColorIndex:cbColorIndex);
						if (Distributing.cbDistributing[cbValueIndex-k][bTempColorIndex]>0)
						{
							cbResultCard[cbResultCount++]=MakeCardData(cbValueIndex-k,bTempColorIndex);
							break;
						}
					}
				}

				return cbResultCount;
			}
		}
	}

	//����˳��
	if (Distributing.cbDistributing[12][5]>0)
	{
		//��ȡ��ɫ
		BYTE cbHeadColor=0xFF;
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			if (Distributing.cbDistributing[12][3-cbColorIndex]>0)
			{
				cbHeadColor=3-cbColorIndex;
				break;
			}
		}

		//����˳��
		BYTE cbHeadCard=MakeCardData(12,cbHeadColor);
		if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
		{
			//��������
			BYTE cbBackCount=0;
			BYTE cbForwardCount=0;

			//ǰ������
			for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
			{
				if (Distributing.cbDistributing[cbColorIndex][5]>0)
				{
					cbForwardCount++;
				}
				else break;
			}

			//��������
			if ((cbForwardCount<4)&&(Distributing.cbDistributing[11][5]>0)) cbBackCount=1;

			//˳���ж�
			if ((cbForwardCount+cbBackCount)>=4)
			{
				//��������
				BYTE cbValueCount=1;
				BYTE cbValueIndex[5]={12};

				//��������
				if (cbBackCount>=1) cbValueIndex[cbValueCount++]=11;
				for (BYTE i=0;i<cbForwardCount;i++) cbValueIndex[cbValueCount++]=(cbForwardCount-i-1);

				//����˳��
				for (BYTE i=0;i<5;i++)
				{
					for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
					{
						BYTE bTempColorIndex = (i == 0 ? 3-cbColorIndex:cbColorIndex);
						if (Distributing.cbDistributing[cbValueIndex[i]][bTempColorIndex]>0)
						{
							cbResultCard[cbResultCount++]=MakeCardData(cbValueIndex[i],bTempColorIndex);
							break;
						}
					}
				}

				return cbResultCount;
			}
		}
	}

	return cbResultCount;
}

//����ͬ��
BYTE CGameLogic::SearchTongHua(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//��ɫ����
	BYTE cbColorCount[4];
	ZeroMemory(cbColorCount,sizeof(cbColorCount));

	//��ɫ����
	for (BYTE cbValueIndex=0;cbValueIndex<13;cbValueIndex++)
	{
		//��Ŀ�ж�
		if (Distributing.cbDistributing[cbValueIndex][5]==0) continue;

		//��ɫ�ж�
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//�˿��ж�
			if (Distributing.cbDistributing[cbValueIndex][cbColorIndex]==0) continue;

			//�����ж�
			if ((++cbColorCount[cbColorIndex])>=5)
			{
				//��С�ж�
				BYTE cbHeadCard=MakeCardData(cbValueIndex,cbColorIndex);
				if ((cbReferCard!=0x00)&&(CompareCard(cbReferCard,cbHeadCard)==false)) continue;

				//������
				BYTE cbResultCount=0;
				cbResultCard[cbResultCount++]=cbHeadCard;

				//ʣ���˿�
				for (BYTE k=0;k<cbValueIndex;k++)
				{
					if (Distributing.cbDistributing[k][cbColorIndex]>0)
					{
						//�����˿�
						cbResultCard[5-cbResultCount++]=MakeCardData(k,cbColorIndex);

						//����ж�
						if (cbResultCount==5) break;
					}
				}

				return cbResultCount;
			}
		}
	}

	return 0;
}

//����ͬ��˳
BYTE CGameLogic::SearchTongHuaShun(const tagDistributing & Distributing, BYTE cbReferCard, BYTE cbResultCard[MAX_COUNT])
{
	//��������
	BYTE cbLinkCount[4];
	ZeroMemory(cbLinkCount,sizeof(cbLinkCount));

	//��ͨ˳��
	for (BYTE cbValueIndex=0;cbValueIndex<12;cbValueIndex++)
	{
		//�����ж�
		if (Distributing.cbDistributing[cbValueIndex][5]==0)
		{
			ZeroMemory(cbLinkCount,sizeof(cbLinkCount));
			continue;
		}

		//ͬ������
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			//�˿��ж�
			if (Distributing.cbDistributing[cbValueIndex][cbColorIndex]==0) 
			{
				cbLinkCount[cbColorIndex]=0;
				continue;
			}
		
			//�����ж�
			if ((++cbLinkCount[cbColorIndex])>=5)
			{
				//����˳��
				BYTE cbHeadCard=MakeCardData(cbValueIndex,cbColorIndex);
				if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
				{
					//���ý��
					BYTE cbResultCount=0;
					for (BYTE k=0;k<5;k++) cbResultCard[cbResultCount++]=MakeCardData(cbValueIndex-k,cbColorIndex);

					return cbResultCount;
				}
			}
		}
	}

	//����˳��
	if (Distributing.cbDistributing[12][5]>0)
	{
		for (BYTE cbColorIndex=0;cbColorIndex<4;cbColorIndex++)
		{
			if( Distributing.cbDistributing[12][cbColorIndex]==0 ) continue;

			//��С�ж�
			BYTE cbHeadCard=MakeCardData(12,cbColorIndex);
			if ((cbReferCard!=0x00)&&(CompareCard(cbReferCard,cbHeadCard)==false)) continue;

			//��������
			BYTE cbBackCount=0;
			BYTE cbForwardCount=0;

			//ǰ������
			for (BYTE i=0;i<4;i++)
			{
				if (Distributing.cbDistributing[i][cbColorIndex]>0)
				{
					cbForwardCount++;
				}
				else break;
			}

			//��������
			if ((cbForwardCount<4)&&(Distributing.cbDistributing[11][cbColorIndex]>0)) cbBackCount=1;

			//˳���ж�
			if ((cbForwardCount+cbBackCount)>=4)
			{
				//��������
				BYTE cbValueCount=1;
				BYTE cbValueIndex[5]={12};

				//��������
				if (cbBackCount>=1) cbValueIndex[cbValueCount++]=11;
				for (BYTE i=0;i<cbForwardCount;i++) cbValueIndex[cbValueCount++]=(cbForwardCount-i-1);

				//����˳��
				BYTE cbResultCount=0;
				for (BYTE i=0;i<5;i++) cbResultCard[cbResultCount++]=MakeCardData(cbValueIndex[i],cbColorIndex);

				return cbResultCount;
			}
		}
	}

	return 0;
}

//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	//�����˿�
	switch (cbValueIndex)
	{
	case 11:	//A �˿�
		{
			return (cbColorIndex<<4)|0x01;
		}
	case 12:	//2 �˿�
		{
			return (cbColorIndex<<4)|0x02;
		}
	case 13:	//С���˿�
		{
			return (cbColorIndex<<4)|0x01;
		}
	case 14:	//����˿�
		{
			return (cbColorIndex<<4)|0x02;
		}
	default:	//�����˿�
		{
			return (cbColorIndex<<4)|(cbValueIndex+3);
		}
	}

	return 0x00;
}

//�Ƿ�����
bool CGameLogic::IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ�ж�
	if (cbCardCount!=5) return false;
	
	//��������
	BYTE cbFirstValue=GetCardLogicValue(cbCardData[0]);

	//���⴦��
	if (cbFirstValue==15)
	{
		for (BYTE i=1;i<cbCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);
			if ((cbLogicValue>=7)&&(cbLogicValue<=13)) return false;
			if ((cbFirstValue!=(cbLogicValue+i))&&(cbFirstValue!=(cbLogicValue+i+8))) return false;
		}

		return true;
	}

	//��������
	for (BYTE i=1;i<cbCardCount;i++)
	{
		if (cbFirstValue!=(GetCardLogicValue(cbCardData[i])+i)) return false;
	}

	return true;
}

//�����˿�
void CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��������
		BYTE cbSameCount=1,cbCardValueTemp=0;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//����ͬ��
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//���ñ���
			cbSameCount++;
		}

		//���ý��
		switch (cbSameCount)
		{
		case 1:		//����
			{
				BYTE cbIndex=AnalyseResult.cbCardCount[0]++;
				AnalyseResult.cbCardData[0][cbIndex*cbSameCount]=cbCardData[i];
				break;
			}
		case 2:		//����
			{
				BYTE cbIndex=AnalyseResult.cbCardCount[1]++;
				AnalyseResult.cbCardData[1][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[1][cbIndex*cbSameCount+1]=cbCardData[i+1];
				break;
			}
		case 3:		//����
			{
				BYTE cbIndex=AnalyseResult.cbCardCount[2]++;
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount+2]=cbCardData[i+2];
				break;
			}
		case 4:		//����
			{
				BYTE cbIndex=AnalyseResult.cbCardCount[3]++;
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+3]=cbCardData[i+3];
				break;
			}
		}

		//��������
		i+=(cbSameCount-1);
	}

	return;
}

//�����ֲ�
void CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//���ñ���
	ZeroMemory(&Distributing,sizeof(Distributing));

	//���ñ���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbCardData[i]==0) continue;

		//��ȡ����
		BYTE cbCardColor=GetCardColor(cbCardData[i]);
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//�ֲ���Ϣ
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbLogicValue-3][cbIndexCount]++;
		Distributing.cbDistributing[cbLogicValue-3][cbCardColor>>4]++;
	}

	return;
}

//�����˿�
void CGameLogic::AnalyseCardData( const BYTE cbCardData[],BYTE cbCardCount,tagAnalysedCard &AnalysedCard )
{
	//��ʼ��
	ZeroMemory( &AnalysedCard,sizeof(tagAnalysedCard) );

	BYTE bCardData[MAX_COUNT];
	BYTE bCardCount = cbCardCount;
	CopyMemory( bCardData,cbCardData,sizeof(BYTE)*cbCardCount );

	//��������
	BYTE bDoubleCardCount = 0;
	BYTE bDoubleCardKCount = 0;
	tagAnalyseResult AnalyseResult;

	SortCardList( bCardData,bCardCount );
	AnalysebCardData( bCardData,bCardCount,AnalyseResult );

	BYTE bTempData[MAX_COUNT];
	BYTE bTempCount = 0;

	tagDistributing Distributing;
	AnalysebDistributing( bCardData,bCardCount,Distributing );
	//����ͬ��˳
	while( SearchTongHuaShun( Distributing,0x00,bTempData ) != 0 )
	{
		//����
		bTempCount = AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1][bTempCount*5],bTempData,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData,5,bCardData,bCardCount ) );
		bCardCount -= 5;
		//���·����ֲ�
		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}

	//��������������
	AnalysebCardData( bCardData,bCardCount,AnalyseResult );
	bTempCount = AnalyseResult.cbCardCount[3]*4;
	CopyMemory( bTempData,AnalyseResult.cbCardData[3],sizeof(BYTE)*bTempCount );
	CopyMemory( &bTempData[bTempCount],AnalyseResult.cbCardData[2],sizeof(BYTE)*AnalyseResult.cbCardCount[2]*3 );
	bTempCount += AnalyseResult.cbCardCount[2]*3;
	//�Ƴ�����������������ͬ����˳��
	VERIFY( RemoveCard( bTempData,bTempCount,bCardData,bCardCount ) );
	bCardCount -= bTempCount;


	//����ͬ����˳��
	AnalysebDistributing( bCardData,bCardCount,Distributing );
	BYTE bTempData1[5];
	BYTE bTempCount1 = 0;
	while( SearchTongHua( Distributing,0x00,bTempData1 ) == 5 )
	{
		//����
		bTempCount1 = AnalysedCard.bCardCount[CT_TONG_HUA-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_TONG_HUA-1][bTempCount1*5],bTempData1,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData1,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData1,5,bCardData,bCardCount ) );
		bCardCount -= 5;
		//���·����ֲ�
		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}
	while( SearchShunZi( Distributing,0x00,bTempData1 ) == 5 )
	{
		//����
		bTempCount1 = AnalysedCard.bCardCount[CT_SHUN_ZI-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_SHUN_ZI-1][bTempCount1*5],bTempData1,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData1,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData1,5,bCardData,bCardCount ) );
		bCardCount -= 5;

		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}

	//��������
	int nCount4 = 0;	//��������
	tagAnalyseResult AnalyseResult1;
	AnalysebCardData( bCardData,bCardCount,AnalyseResult1 );
	if( AnalyseResult.cbCardCount[3] > 0 && 
		(AnalyseResult1.cbCardCount[0]+AnalyseResult1.cbCardCount[1]*2) < AnalyseResult.cbCardCount[3] )		
	{
		//�ͷ�˳��
		CopyMemory( &bCardData[bCardCount],AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*AnalysedCard.bCardCount[CT_SHUN_ZI-1]*5 );
		ZeroMemory( AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*MAX_COUNT );
		bCardCount += AnalysedCard.bCardCount[CT_SHUN_ZI-1]*5;
		AnalysedCard.bCardCount[CT_SHUN_ZI-1] = 0;

		SortCardList( bCardData,bCardCount );
		tagAnalyseResult AnalyseResult2;
		AnalysebCardData( bCardData,bCardCount,AnalyseResult2 );
		
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = 0; i < 2; i++ )
			for( int j = AnalyseResult2.cbCardCount[i]*(i+1)-1; j >= 0 ; j-- )
				bTempCard[bTempCardCount++] = AnalyseResult2.cbCardData[i][j];

		nCount4 = AnalyseResult.cbCardCount[3];
		BYTE bTempIndex = 0;
		while( bTempCardCount-- && nCount4 )
		{
			if( GetCardLogicValue( bTempCard[bTempIndex] ) != 15 )		//�ų�2
			{
				CopyMemory( &AnalysedCard.bCardData[CT_TIE_ZHI-1][5*AnalysedCard.bCardCount[CT_TIE_ZHI-1]++],
					&AnalyseResult.cbCardData[3][(nCount4-1)*4],sizeof(BYTE)*4 );
				AnalysedCard.bCardData[CT_TIE_ZHI-1][5*AnalysedCard.bCardCount[CT_TIE_ZHI-1]-1] = bTempCard[bTempIndex];
				VERIFY( RemoveCard( &bTempCard[bTempIndex++],1,bCardData,bCardCount ) );
				bCardCount--;
				nCount4--;
			}
			else bTempIndex++;
		}

	}
	else if( AnalyseResult.cbCardCount[3] > 0 )
	{
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = 0; i < 3; i++ )
			for( int j = AnalyseResult1.cbCardCount[i]*(i+1)-1; j >= 0; j-- )
				bTempCard[bTempCardCount++] = AnalyseResult1.cbCardData[i][j];

		nCount4 = AnalyseResult.cbCardCount[3];
		BYTE bTempIndex = 0;
		while( bTempCardCount-- && nCount4 )
		{
			if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )		//�ų�2
			{
				CopyMemory( &AnalysedCard.bCardData[CT_TIE_ZHI-1][5*AnalysedCard.bCardCount[CT_TIE_ZHI-1]++],
					&AnalyseResult.cbCardData[3][(nCount4-1)*4],sizeof(BYTE)*4 );
				AnalysedCard.bCardData[CT_TIE_ZHI-1][5*AnalysedCard.bCardCount[CT_TIE_ZHI-1]-1] = bTempCard[bTempIndex];
				VERIFY( RemoveCard( &bTempCard[bTempIndex++],1,bCardData,bCardCount ) );
				bCardCount--;
				nCount4--;
			}
			else bTempIndex++;
		}
	}
	//�����ջ�ʣ������
	if( nCount4 > 0 )
	{
		while( nCount4-- )
		{
			CopyMemory( &bCardData[bCardCount],&AnalyseResult.cbCardData[3][nCount4*4],sizeof(BYTE)*4 );
			bCardCount += 4;
		}
		//��������
		SortCardList( bCardData,bCardCount );
	}


	//�����«
	int nCount3 = 0;	//��������
	tagAnalyseResult AnalyseResult3;
	AnalysebCardData( bCardData,bCardCount,AnalyseResult3 );
	BYTE bHuLuCard[MAX_COUNT];
	BYTE bHuLuCardCount = 0;
	bool bBreakCard = true;			
	if( AnalyseResult.cbCardCount[2]>0 && GetCardLogicValue(AnalyseResult.cbCardData[2][0])>=11 &&
		AnalyseResult3.cbCardCount[1]<AnalyseResult.cbCardCount[2] )
	{
		if( AnalyseResult3.cbCardCount[1] > 0 )
		{
			BYTE bIndex = 0;
			while( bIndex<AnalyseResult3.cbCardCount[1] && GetCardLogicValue(AnalyseResult3.cbCardData[1][bIndex*2]) == 15 ) bIndex++;
			if( bIndex<AnalyseResult3.cbCardCount[1] && GetCardLogicValue(AnalyseResult3.cbCardData[1][bIndex*2]) != 15 )
			{
				CopyMemory( &bHuLuCard[5*bHuLuCardCount++],
					AnalyseResult.cbCardData[2],sizeof(BYTE)*3 );
				CopyMemory( &bHuLuCard[5*bHuLuCardCount-2],&AnalyseResult3.cbCardData[1][bIndex*2],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( &AnalyseResult3.cbCardData[1][bIndex*2],2,bCardData,bCardCount ) );
				VERIFY( RemoveCard( AnalyseResult.cbCardData[2],3,AnalyseResult.cbCardData[2],AnalyseResult.cbCardCount[2]*3 ) );
				AnalyseResult.cbCardCount[2]--;
				bCardCount -= 2;
				bBreakCard = false;
			}
		}
		if( bBreakCard == true )
		{
			BYTE bCount = AnalyseResult.cbCardCount[2];
			if( bCount > 1 )
			{
				BYTE bFirstIndex,bNextIndex;
				if( GetCardLogicValue(AnalyseResult.cbCardData[2][(bCount-1)*3])>=11 ) 
				{
					bFirstIndex = bCount-1;
					bNextIndex = bCount-2;
				}else
				{
					bNextIndex = bCount-1;
					bFirstIndex = bCount-2;
					while( GetCardLogicValue(AnalyseResult.cbCardData[2][3*bFirstIndex])<11 ) bFirstIndex--;
					ASSERT( bFirstIndex>=0 && bFirstIndex<(BYTE)-1 );
				}
				CopyMemory( &bHuLuCard[5*bHuLuCardCount++],
					&AnalyseResult.cbCardData[2][bFirstIndex*3],sizeof(BYTE)*3 );
				CopyMemory( &bHuLuCard[5*bHuLuCardCount-2],
					&AnalyseResult.cbCardData[2][bNextIndex*3+1],sizeof(BYTE)*2 );
				bCardData[bCardCount++] = AnalyseResult.cbCardData[2][bNextIndex*3];
				VERIFY( RemoveCard( &AnalyseResult.cbCardData[2][bFirstIndex*3],3,AnalyseResult.cbCardData[2],
					AnalyseResult.cbCardCount[2]*3 ) );
				AnalyseResult.cbCardCount[2]--;
				VERIFY( RemoveCard( &AnalyseResult.cbCardData[2][bNextIndex*3],3,AnalyseResult.cbCardData[2],
					AnalyseResult.cbCardCount[2]*3 ) );
				AnalyseResult.cbCardCount[2]--;
				bBreakCard = false;
			}
			//��˫֧
			else if( AnalyseResult3.cbCardCount[1] > 0 )
			{
				CopyMemory( &bHuLuCard[5*bHuLuCardCount++],
					AnalyseResult.cbCardData[2],sizeof(BYTE)*3 );
				CopyMemory( &bHuLuCard[5*bHuLuCardCount-2],AnalyseResult3.cbCardData[1],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( AnalyseResult3.cbCardData[1],2,bCardData,bCardCount ) );
				bCardCount -= 2;
				VERIFY( RemoveCard( AnalyseResult.cbCardData[2],3,AnalyseResult.cbCardData[2],AnalyseResult.cbCardCount[2]*3 ) );
				AnalyseResult.cbCardCount[2]--;
				bBreakCard = false;
			}
		}

		//���·����ֲ�
		if( bBreakCard == false )
		{
			SortCardList(bCardData,bCardCount);
			AnalysebCardData( bCardData,bCardCount,AnalyseResult3 );
		}	
	}

	if( AnalyseResult.cbCardCount[2] > 0 && 
		(AnalyseResult3.cbCardCount[1]) < AnalyseResult.cbCardCount[2] /*&& bBreakCard*/ )
	{
		//�ͷ�˳��
		CopyMemory( &bCardData[bCardCount],AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*AnalysedCard.bCardCount[CT_SHUN_ZI-1]*5 );
		ZeroMemory( AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*MAX_COUNT );
		bCardCount += AnalysedCard.bCardCount[CT_SHUN_ZI-1]*5;
		AnalysedCard.bCardCount[CT_SHUN_ZI-1] = 0;

		SortCardList( bCardData,bCardCount );
		tagAnalyseResult AnalyseResult4;
		AnalysebCardData( bCardData,bCardCount,AnalyseResult4 );
		
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = AnalyseResult4.cbCardCount[1]*2-2; i >= 0 ; i-=2 )
		{
			bTempCard[bTempCardCount++] = AnalyseResult4.cbCardData[1][i];
			bTempCard[bTempCardCount++] = AnalyseResult4.cbCardData[1][i+1];
		}

		nCount3 = AnalyseResult.cbCardCount[2];
		BYTE bTempIndex = 0;
		while( (bTempIndex < bTempCardCount) && nCount3 )
		{
			if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )
			{
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
					&AnalyseResult.cbCardData[2][(nCount3-1)*3],sizeof(BYTE)*3 );
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]-2],&bTempCard[bTempIndex],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( &bTempCard[bTempIndex],2,bCardData,bCardCount ) );
				bTempIndex += 2;
				bCardCount -= 2;
				nCount3--;
			}
			else bTempIndex += 2;
		}

		if( nCount3 > 0 )		//�ͷ�ͬ��
		{
			CopyMemory( &bCardData[bCardCount],AnalysedCard.bCardData[CT_TONG_HUA-1],sizeof(BYTE)*AnalysedCard.bCardCount[CT_TONG_HUA-1]*5 );
			ZeroMemory( AnalysedCard.bCardData[CT_TONG_HUA-1],sizeof(BYTE)*MAX_COUNT );
			bCardCount += AnalysedCard.bCardCount[CT_TONG_HUA-1]*5;
			AnalysedCard.bCardCount[CT_TONG_HUA-1] = 0;

			SortCardList( bCardData,bCardCount );
			AnalysebCardData( bCardData,bCardCount,AnalyseResult4 );
			
			bTempCardCount = 0;
			for( int i = AnalyseResult4.cbCardCount[1]*2-2; i >= 0 ; i-=2 )
			{
				bTempCard[bTempCardCount++] = AnalyseResult4.cbCardData[1][i];
				bTempCard[bTempCardCount++] = AnalyseResult4.cbCardData[1][i+1];
			}

			bTempIndex = 0;
			while( (bTempIndex < bTempCardCount) && nCount3 )
			{
				if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )
				{
					CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
						&AnalyseResult.cbCardData[2][(nCount3-1)*3],sizeof(BYTE)*3 );
					CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]-2],&bTempCard[bTempIndex],sizeof(BYTE)*2 );
					VERIFY( RemoveCard( &bTempCard[bTempIndex],2,bCardData,bCardCount ) );
					bTempIndex += 2;
					bCardCount -= 2;
					nCount3--;
				}
				else bTempIndex += 2;
			}
		}

	}
	else if( AnalyseResult.cbCardCount[2] > 0 )
	{
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = AnalyseResult3.cbCardCount[1]*2-2; i >= 0 ; i-=2 )
		{
			bTempCard[bTempCardCount++] = AnalyseResult3.cbCardData[1][i];
			bTempCard[bTempCardCount++] = AnalyseResult3.cbCardData[1][i+1];
		}

		nCount3 = AnalyseResult.cbCardCount[2];
		BYTE bTempIndex = 0;
		while( (bTempIndex < bTempCardCount) && nCount3 )
		{
			if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )
			{
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
					&AnalyseResult.cbCardData[2][(nCount3-1)*3],sizeof(BYTE)*3 );
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]-2],&bTempCard[bTempIndex],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( &bTempCard[bTempIndex],2,bCardData,bCardCount ) );
				bTempIndex += 2;
				bCardCount -= 2;
				nCount3--;
			}
			else bTempIndex += 2;
		}
	}
	//�����ջ�ʣ������
	if( nCount3 > 0 )
	{
		while( nCount3-- )
		{
			CopyMemory( &bCardData[bCardCount],&AnalyseResult.cbCardData[2][nCount3*3],sizeof(BYTE)*3 );
			bCardCount += 3;
		}
		//��������
		SortCardList( bCardData,bCardCount );
	}
	//
	if( bHuLuCardCount > 0 )
	{
		CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
			bHuLuCard,sizeof(BYTE)*5 );
	}


	AnalysebDistributing( bCardData,bCardCount,Distributing );
	//����ͬ��
	while( SearchTongHua( Distributing,0x00,bTempData ) == 5 )
	{
		//����
		bTempCount = AnalysedCard.bCardCount[CT_TONG_HUA-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_TONG_HUA-1][bTempCount*5],bTempData,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData,5,bCardData,bCardCount ) );
		bCardCount -= 5;
		//���·����ֲ�
		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}
	//����˳��
	while( SearchShunZi( Distributing,0x00,bTempData ) == 5 )
	{
		//����
		bTempCount = AnalysedCard.bCardCount[CT_SHUN_ZI-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_SHUN_ZI-1][bTempCount*5],bTempData,5*sizeof(BYTE) );
		ASSERT( GetCardType(bTempData,5)>CT_ERROR );
		VERIFY( RemoveCard( bTempData,5,bCardData,bCardCount ) );
		bCardCount -= 5;
		//���·����ֲ�
		AnalysebDistributing( bCardData,bCardCount,Distributing );
	}
	
	AnalysebCardData( bCardData,bCardCount,AnalyseResult );
	//��«
	nCount3=0;
	if( AnalyseResult.cbCardCount[2] > 0 && AnalyseResult.cbCardCount[1]>0 )
	{
		BYTE bTempCard[MAX_COUNT];
		BYTE bTempCardCount = 0;
		for( int i = AnalyseResult.cbCardCount[1]*2-2; i >= 0 ; i-=2 )
		{
			bTempCard[bTempCardCount++] = AnalyseResult.cbCardData[1][i];
			bTempCard[bTempCardCount++] = AnalyseResult.cbCardData[1][i+1];
		}

		nCount3 = AnalyseResult.cbCardCount[2];
		BYTE bTempIndex = 0;
		while( (bTempIndex < bTempCardCount) && nCount3 )
		{
			if( GetCardLogicValue(bTempCard[bTempIndex]) != 15 )
			{
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]++],
					&AnalyseResult.cbCardData[2][(nCount3-1)*3],sizeof(BYTE)*3 );
				CopyMemory( &AnalysedCard.bCardData[CT_HU_LU-1][5*AnalysedCard.bCardCount[CT_HU_LU-1]-2],&bTempCard[bTempIndex],sizeof(BYTE)*2 );
				VERIFY( RemoveCard( &bTempCard[bTempIndex],2,bCardData,bCardCount ) );
				bCardCount -= 2;
				VERIFY( RemoveCard( &AnalyseResult.cbCardData[2][(nCount3-1)*3],3,bCardData,bCardCount ) );
				bCardCount -= 3;
				bTempIndex += 2;
				nCount3--;
			}
			else bTempIndex += 2;
		}
	}
	SortCardList(bCardData,bCardCount);
	AnalysebCardData( bCardData,bCardCount,AnalyseResult );
	//���������
	//CT_FOUR_TIAO
	for( int i = AnalyseResult.cbCardCount[3]-1; i >= 0; i-- )
	{
		bTempCount = AnalysedCard.bCardCount[CT_FOUR_TIAO-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_FOUR_TIAO-1][bTempCount*4],&AnalyseResult.cbCardData[3][i*4],4*sizeof(BYTE) );
	}
	//CT_THREE_TIAO
	for( int i = AnalyseResult.cbCardCount[2]-1; i >= 0; i-- )
	{
		bTempCount = AnalysedCard.bCardCount[CT_THREE_TIAO-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_THREE_TIAO-1][bTempCount*3],&AnalyseResult.cbCardData[2][i*3],3*sizeof(BYTE) );
	}
	//CT_DOUBLE
	for( int i = AnalyseResult.cbCardCount[1]-1; i >= 0; i-- )
	{
		bTempCount = AnalysedCard.bCardCount[CT_DOUBLE-1]++;
		CopyMemory( &AnalysedCard.bCardData[CT_DOUBLE-1][bTempCount*2],&AnalyseResult.cbCardData[1][i*2],2*sizeof(BYTE) );
	}
	//CT_SINGLE
	for( int i = AnalyseResult.cbCardCount[0]-1; i >= 0; i-- )
	{
		bTempCount = AnalysedCard.bCardCount[CT_SINGLE-1]++;
		AnalysedCard.bCardData[CT_SINGLE-1][bTempCount] = AnalyseResult.cbCardData[0][i];
	}

	//����������¼�����>=3,�����2����������
	int nSimpleCount = 0;
	for( int i = CT_DOUBLE-1; i >= 0; i-- )
		nSimpleCount += AnalysedCard.bCardCount[i];
	if( nSimpleCount >= 3 )
	{
		for( int i = CT_HU_LU-1; i < CT_TIE_ZHI; i++ )
		{
			if( AnalysedCard.bCardCount[i]==0 ) continue;
			if( GetCardLogicValue(AnalysedCard.bCardData[i][(AnalysedCard.bCardCount[i]-1)*5]) == 15 )
			{
				//�ͷ�
				CopyMemory( bCardData,&AnalysedCard.bCardData[i][(AnalysedCard.bCardCount[i]-1)*5],sizeof(BYTE)*5 );
				AnalysedCard.bCardCount[i]--;

				BYTE bTempType = 7 - i;
				CopyMemory( &AnalysedCard.bCardData[bTempType][AnalysedCard.bCardCount[bTempType]*(bTempType+1)],
					&bCardData[4-bTempType],(bTempType+1)*sizeof(BYTE) );
				ASSERT( GetCardType(&bCardData[4-bTempType],(bTempType+1))>CT_ERROR );
				AnalysedCard.bCardCount[bTempType]++;

				BYTE bCount = i==CT_HU_LU-1?1:2;
				//��
				CopyMemory( &AnalysedCard.bCardData[0][AnalysedCard.bCardCount[0]],
					bCardData,(bCount)*sizeof(BYTE) );
				AnalysedCard.bCardCount[0] += bCount;
				//˫
				CopyMemory( &AnalysedCard.bCardData[1][AnalysedCard.bCardCount[1]*2],
					&bCardData[bCount],2*sizeof(BYTE) );
				ASSERT( GetCardType(&bCardData[bCount],2)>CT_ERROR );
				AnalysedCard.bCardCount[1]++;
			}

		}
		for( int i = CT_THREE_TIAO-1; i < CT_FOUR_TIAO; i++ )
		{
			if( AnalysedCard.bCardCount[i] == 0 ) continue;
			if( GetCardLogicValue(AnalysedCard.bCardData[i][(AnalysedCard.bCardCount[i]-1)*(i+1)]) == 15 )
			{
				//�ͷ�
				CopyMemory( bCardData,&AnalysedCard.bCardData[i][(AnalysedCard.bCardCount[i]-1)*(i+1)],sizeof(BYTE)*(i+1) );
				AnalysedCard.bCardCount[i]--;
				//˫
				BYTE bTempType = 1;
				CopyMemory( &AnalysedCard.bCardData[bTempType][AnalysedCard.bCardCount[bTempType]*(bTempType+1)],
					&bCardData[i-1],(bTempType+1)*sizeof(BYTE) );
				ASSERT( GetCardType(&bCardData[i-1],(bTempType+1))>CT_ERROR );
				AnalysedCard.bCardCount[bTempType]++;
				//��
				BYTE bCount = i==CT_THREE_TIAO-1?1:2;
				CopyMemory( &AnalysedCard.bCardData[0][AnalysedCard.bCardCount[0]],
					bCardData,(bCount)*sizeof(BYTE) );
				AnalysedCard.bCardCount[0] += bCount;
			}
		}
	}
	if( AnalysedCard.bCardCount[0] >= 5 )
	{
		if( AnalysedCard.bCardCount[1]>0 && GetCardLogicValue(AnalysedCard.bCardData[1][(AnalysedCard.bCardCount[1]-1)*2]) == 15 )
		{
			//�ͷ�
			CopyMemory( bCardData,&AnalysedCard.bCardData[1][(AnalysedCard.bCardCount[1]-1)*2],sizeof(BYTE)*2 );
			AnalysedCard.bCardCount[1]--;
			ASSERT( GetCardType(bCardData,2)>CT_ERROR );
			//��
			BYTE bCount = 2;
			CopyMemory( &AnalysedCard.bCardData[0][AnalysedCard.bCardCount[0]],
				bCardData,(bCount)*sizeof(BYTE) );
			AnalysedCard.bCardCount[0] += bCount;
		}
	}

	return;

}

//��������
bool CGameLogic::SearchFromAnalysedCard( const tagAnalysedCard& AnalysedCard,WORD wChairId,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE bOutCardRecord[],BYTE bOutCardCount,
										const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardResult &OutCardResult,bool &bSplit )
{
	//
	ZeroMemory( &OutCardResult,sizeof(tagOutCardResult) );

	/**
	*���Ʋ��ԣ�
	*1.��˳��˳
	*2.ͬ��,ͬ��˳
	*3.��������Q���º�«;������Q������֧
	*3.������,���ֻʣ2�ţ����ȳ���������ƣ��������������>3
	*4.Q���Ϻ�«��Q������֧
	*5.������
	*
	*
	*ѹ�Ʋ��ԣ�
	*1.��ѹ��ѹ,��������
	**/

	BYTE bMostCount=0,bLessCount=0;
	bool bTurn = bTurnCardCount==0?true:false;
	bool bFinish = IsOutCardFinish(wChairId,AnalysedCard,bOutCardRecord,bOutCardCount,bTurn,cbCardData,cbCardCount,bMostCount,bLessCount);

	WORD wPreChairId = wChairId==0?GAME_PLAYER-1:wChairId-1;
	WORD wLastPreChairId = wPreChairId==0?GAME_PLAYER-1:wPreChairId-1;
	WORD wNextChairId = (wChairId+1)%GAME_PLAYER;

	if( bTurnCardCount == 0 )					//����
	{
		BYTE bMostCard = GetMostCardValue( AnalysedCard,bOutCardRecord,bOutCardCount );

		//���Գ�������
		if( bFinish )
		{
			if( bLessCount<=1 )	//�ȳ���
			{
				for( int i = CountArray(AnalysedCard.bCardCount)-1; i>=0 ; i-- )
				{
					BYTE bCount = i>=CT_SHUN_ZI-1?5:i+1;
					for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
					{
						if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,
							cbCardData,cbCardCount) )
						{
							OutCardResult.cbCardCount = bCount;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[i][j*bCount],
								sizeof(BYTE)*OutCardResult.cbCardCount );
							SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
							ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
							return true;
						}
					}
				}
			}
			else				//�ȳ�С�����
			{
				int bIndex = -1,bType=0;
				BYTE bCount;
				//С������������ջ���Ȩ
				for( int i = 0; i < CountArray(AnalysedCard.bCardCount); i++ )
				{
					bCount = i>=CT_SHUN_ZI-1?5:i+1;
					if( AnalysedCard.bCardCount[i] == 0 ) continue;
					if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*(AnalysedCard.bCardCount[i]-1)],bCount,
						cbCardData,cbCardCount) )
						continue;
					if( bIndex == -1 )
						for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
						{
							if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*j],bCount,
								cbCardData,cbCardCount) )
							{
								bType = i+1;
								bIndex = j;
								break;
							}
						}
					for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
					{
						if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*j],bCount,
								cbCardData,cbCardCount) && 
							CompareCard(AnalysedCard.bCardData[i][bIndex*bCount],AnalysedCard.bCardData[i][j*bCount]) )
						{
							bType = i+1;
							bIndex = j;
						}
					}
					if( bIndex != -1 )
						break;
				}
				//�����������С������Ƶ�����
				if( bIndex == -1 )
				{
					for( int i = 1; i < CountArray(AnalysedCard.bCardCount); i++ )
					{
						BYTE bCount = i>=CT_SHUN_ZI-1?5:i+1;
						if( AnalysedCard.bCardCount[i]>0 && 
							(CompareCard(GetCardLogicValue(AnalysedCard.bCardData[i][0]),bMostCard) &&
							GetCardLogicValue(AnalysedCard.bCardData[i][0])<13 ||
							i>=CT_SHUN_ZI-1) &&
							IsTurnCardMost(wChairId,AnalysedCard.bCardData[i],bCount,cbCardData,cbCardCount)
							)
						{
							bType = i + 1;
							bIndex = 0;
							break;
						}
					}
				}
				//С���������
				if( bIndex == -1 )
				{
					for( int i = 0; i < CountArray(AnalysedCard.bCardCount); i++ )
					{
						bCount = i>=CT_SHUN_ZI-1?5:i+1;
						if( bIndex == -1 )
							for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
							{
								if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*j],bCount,
									cbCardData,cbCardCount) )
								{
									bType = i+1;
									bIndex = j;
									break;
								}
							}
						for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
						{
							if( !IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][bCount*j],bCount,
									cbCardData,cbCardCount) && 
								CompareCard(AnalysedCard.bCardData[i][bIndex*bCount],AnalysedCard.bCardData[i][j*bCount]) )
							{
								bType = i+1;
								bIndex = j;
							}
						}
						if( bIndex != -1 )
							break;
					}
				}
				if( bIndex != -1 )
				{
					bCount = bType>=CT_SHUN_ZI?5:bType;
					OutCardResult.cbCardCount = bCount;
					CopyMemory(OutCardResult.cbResultCard,&AnalysedCard.bCardData[bType-1][bIndex*bCount],sizeof(BYTE)*bCount);
					SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
					ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
					return true;
				}
			}
		}

		//˳��
		if( AnalysedCard.bCardCount[CT_SHUN_ZI-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//ͬ��
		if( AnalysedCard.bCardCount[CT_TONG_HUA-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TONG_HUA-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//Q��������
		if( AnalysedCard.bCardCount[CT_THREE_TIAO-1] > 0 && GetCardLogicValue(AnalysedCard.bCardData[CT_THREE_TIAO-1][0]) < 12 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_THREE_TIAO-1],sizeof(BYTE)*3 );
			OutCardResult.cbCardCount = 3;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q���º�«
		if( AnalysedCard.bCardCount[CT_HU_LU-1] > 0 && GetCardLogicValue(AnalysedCard.bCardData[CT_HU_LU-1][0]) < 12 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_HU_LU-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q��������
		if( AnalysedCard.bCardCount[CT_FOUR_TIAO-1] > 0 && GetCardLogicValue(AnalysedCard.bCardData[CT_FOUR_TIAO-1][0]) < 12 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_FOUR_TIAO-1],sizeof(BYTE)*4 );
			OutCardResult.cbCardCount = 4;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		if( cbCardCount[wChairId]<=10 && (cbCardCount[wPreChairId]<8 ||
			cbCardCount[wLastPreChairId]<8 || cbCardCount[wNextChairId]<8)
			)
		{
			//Q������֧
			if( AnalysedCard.bCardCount[CT_TIE_ZHI-1] > 0 && GetCardLogicValue(AnalysedCard.bCardData[CT_TIE_ZHI-1][0]) < 12 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_TIE_ZHI-1],sizeof(BYTE)*5 );
				OutCardResult.cbCardCount = 5;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}
			//ͬ��˳
			if( AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1] > 0 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1],sizeof(BYTE)*5 );
				OutCardResult.cbCardCount = 5;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}
		}

		//��������ͣ������͵�֧)>3�����������
		int nSimpleCount = 0;
		for( int i = 0; i < CT_DOUBLE; i++ )
			nSimpleCount += AnalysedCard.bCardCount[i];
		if( nSimpleCount >= 3 )
		{
			if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 2 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
				OutCardResult.cbCardCount = 2;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}else if( AnalysedCard.bCardCount[0] > AnalysedCard.bCardCount[CT_DOUBLE-1]*2 &&
				( GetCardLogicValue(AnalysedCard.bCardData[0][0]) < GetCardLogicValue(bMostCard) ||
				CompareCard(AnalysedCard.bCardData[0][0],bMostCard) )  )
			{
				OutCardResult.cbResultCard[0] = AnalysedCard.bCardData[0][0];
				OutCardResult.cbCardCount = 1;
				return true;
			}

			if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 0 && 
				GetCardLogicValue(AnalysedCard.bCardData[CT_DOUBLE-1][0])<14 &&
				!IsTurnCardMost(wChairId,AnalysedCard.bCardData[CT_DOUBLE-1],2,
				cbCardData,cbCardCount)
				)
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
				OutCardResult.cbCardCount = 2;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}

			
			if( AnalysedCard.bCardCount[0] > 0 )
			{
				OutCardResult.cbResultCard[0] = AnalysedCard.bCardData[0][0];
				OutCardResult.cbCardCount = 1;
				return true;
			}

			if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 0 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
				OutCardResult.cbCardCount = 2;
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}

		}

		//Q����������
		if( AnalysedCard.bCardCount[CT_THREE_TIAO-1] > 0 && 
			( GetCardLogicValue(AnalysedCard.bCardData[CT_THREE_TIAO-1][0])<GetCardLogicValue(bMostCard) ||
			CompareCard(AnalysedCard.bCardData[CT_THREE_TIAO-1][0],bMostCard) ) &&
			GetCardLogicValue(AnalysedCard.bCardData[CT_THREE_TIAO-1][0])<14 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_THREE_TIAO-1],sizeof(BYTE)*3 );
			OutCardResult.cbCardCount = 3;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q�����Ϻ�«
		if( AnalysedCard.bCardCount[CT_HU_LU-1] > 0 )
		{
			if( GetCardLogicValue(AnalysedCard.bCardData[CT_HU_LU-1][0])<GetCardLogicValue(bMostCard) ||
				CompareCard(AnalysedCard.bCardData[CT_HU_LU-1][0],bMostCard) &&
				GetCardLogicValue(AnalysedCard.bCardData[CT_HU_LU-1][0])<14 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_HU_LU-1],sizeof(BYTE)*5 );
				OutCardResult.cbCardCount = 5;
			}else
			{
				CopyMemory( OutCardResult.cbResultCard,
					&AnalysedCard.bCardData[CT_HU_LU-1][3],sizeof(BYTE)*2 );
				OutCardResult.cbCardCount = 2;
				bSplit = true;
			}
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q����������
		if( AnalysedCard.bCardCount[CT_FOUR_TIAO-1] > 0 &&
			( GetCardLogicValue(AnalysedCard.bCardData[CT_FOUR_TIAO-1][0])<GetCardLogicValue(bMostCard)||
			  CompareCard(AnalysedCard.bCardData[CT_FOUR_TIAO-1][0],bMostCard) ) &&
			  GetCardLogicValue(AnalysedCard.bCardData[CT_FOUR_TIAO-1][0])<14 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_FOUR_TIAO-1],sizeof(BYTE)*4 );
			OutCardResult.cbCardCount = 4;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q��������֧
		if( AnalysedCard.bCardCount[CT_TIE_ZHI-1] > 0 )
		{
			if( GetCardLogicValue(AnalysedCard.bCardData[CT_TIE_ZHI-1][0])<GetCardLogicValue(bMostCard) ||
				CompareCard(AnalysedCard.bCardData[CT_TIE_ZHI-1][0],bMostCard) &&
				GetCardLogicValue(AnalysedCard.bCardData[CT_TIE_ZHI-1][0])<14 )
			{
				CopyMemory( OutCardResult.cbResultCard,
					AnalysedCard.bCardData[CT_TIE_ZHI-1],sizeof(BYTE)*5 );
				OutCardResult.cbCardCount = 5;
			}else
			{
				OutCardResult.cbResultCard[0] = AnalysedCard.bCardData[CT_TIE_ZHI-1][4];
				OutCardResult.cbCardCount = 1;
				bSplit = true;
			}
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//ͬ��˳
		if( AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//˫֧
		if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
			OutCardResult.cbCardCount = 2;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//Q����������
		if( AnalysedCard.bCardCount[CT_THREE_TIAO-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_THREE_TIAO-1],sizeof(BYTE)*3 );
			OutCardResult.cbCardCount = 3;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q�����Ϻ�«
		if( AnalysedCard.bCardCount[CT_HU_LU-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_HU_LU-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q����������
		if( AnalysedCard.bCardCount[CT_FOUR_TIAO-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_FOUR_TIAO-1],sizeof(BYTE)*4 );
			OutCardResult.cbCardCount = 4;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//Q��������֧
		if( AnalysedCard.bCardCount[CT_TIE_ZHI-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TIE_ZHI-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//��֧
		if( AnalysedCard.bCardCount[0] > 0 )
		{
			OutCardResult.cbResultCard[0] = AnalysedCard.bCardData[0][0];
			OutCardResult.cbCardCount = 1;
			return true;
		}

		ASSERT( FALSE );
	}
	else										//ѹ��
	{
		BYTE bTurnCardType = GetCardType( bTurnCardData,bTurnCardCount );
		ASSERT( bTurnCardType > CT_ERROR );
		if( bTurnCardType == CT_ERROR ) return false;
		//�жϳ���
		if( bFinish )
		{
			if( SearchOutCard(cbCardData[wChairId],cbCardCount[wChairId],bTurnCardData,bTurnCardCount,
				OutCardResult) )
			{
				ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
				if( IsTurnCardMost(wChairId,OutCardResult.cbResultCard,OutCardResult.cbCardCount,
					cbCardData,cbCardCount) )
				{
					tagAnalysedCard tmpAnalysedCard;
					BYTE bHandCard[MAX_COUNT],bHandCardCount;
					BYTE bTmpOutCardRecord[FULL_COUNT],bTmpOutCardRecordCount;
					BYTE bTmpMostCount=0,bTmpLessCount=0;

					bHandCardCount = cbCardCount[wChairId];
					CopyMemory(bHandCard,cbCardData[wChairId],sizeof(BYTE)*bHandCardCount);
					VERIFY( RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bHandCardCount) );
					bHandCardCount -= OutCardResult.cbCardCount;

					AnalyseCardData(bHandCard,bHandCardCount,tmpAnalysedCard);

					CopyMemory(bTmpOutCardRecord,bOutCardRecord,sizeof(BYTE)*bOutCardCount);
					bTmpOutCardRecordCount = bOutCardCount;
					CopyMemory(&bTmpOutCardRecord[bTmpOutCardRecordCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
					bTmpOutCardRecordCount += OutCardResult.cbCardCount;

					if( IsOutCardFinish(wChairId,tmpAnalysedCard,bTmpOutCardRecord,bTmpOutCardRecordCount,
						true,cbCardData,cbCardCount,bTmpMostCount,bTmpLessCount) )
					{
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
							return false;
						bSplit = true;
						return true;
					}
				}
			}
			if( SearchTurnCard(cbCardData[wChairId],cbCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult) )
			{
				ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
				tagAnalysedCard tmpAnalysedCard;
				BYTE bHandCard[MAX_COUNT],bHandCardCount;
				BYTE bTmpOutCardRecord[FULL_COUNT],bTmpOutCardRecordCount;
				BYTE bTmpMostCount=0,bTmpLessCount=0;

				bHandCardCount = cbCardCount[wChairId];
				CopyMemory(bHandCard,cbCardData[wChairId],sizeof(BYTE)*bHandCardCount);
				VERIFY( RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bHandCardCount) );
				bHandCardCount -= OutCardResult.cbCardCount;

				AnalyseCardData(bHandCard,bHandCardCount,tmpAnalysedCard);

				CopyMemory(bTmpOutCardRecord,bOutCardRecord,sizeof(BYTE)*bOutCardCount);
				bTmpOutCardRecordCount = bOutCardCount;
				CopyMemory(&bTmpOutCardRecord[bTmpOutCardRecordCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
				bTmpOutCardRecordCount += OutCardResult.cbCardCount;

				if( IsOutCardFinish(wChairId,tmpAnalysedCard,bTmpOutCardRecord,bTmpOutCardRecordCount,
					false,cbCardData,cbCardCount,bTmpMostCount,bTmpLessCount) )
				{
					ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
					if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
							return false;
					bSplit = true;
					return true;
				}
			}
		}
		switch( bTurnCardType )
		{
		case CT_SINGLE:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);
					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_DOUBLE:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					if( IsTurnCardMost(wChairId,OutCardType.bCardData,OutCardType.bCardCount,
						cbCardData,cbCardCount) ||
						GetCardLogicValue(OutCardType.bCardData[0])==15 )
					{
						tagOutCardType SingleType;
						AnalyseOutCardType(AnalysedCard,wChairId,CT_SINGLE,NULL,0,cbCardData,cbCardCount,SingleType);
						if( SingleType.bRemainCount > 2 ) return false;
					}
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);
					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_THREE_TIAO:
		case CT_FOUR_TIAO:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					if( GetCardLogicValue(OutCardType.bCardData[0])>=14 )
					{
						tagOutCardType SingleType,DoubleType;
						AnalyseOutCardType(AnalysedCard,wChairId,CT_SINGLE,NULL,0,cbCardData,cbCardCount,SingleType);
						AnalyseOutCardType(AnalysedCard,wChairId,CT_DOUBLE,NULL,0,cbCardData,cbCardCount,DoubleType);
						if( SingleType.bRemainCount+DoubleType.bRemainCount > 3 ) return false;
					}
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_SHUN_ZI:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_TONG_HUA:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_HU_LU:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					if( GetCardLogicValue(OutCardType.bCardData[0])>=14 )
					{
						tagOutCardType SingleType,DoubleType;
						AnalyseOutCardType(AnalysedCard,wChairId,CT_SINGLE,NULL,0,cbCardData,cbCardCount,SingleType);
						AnalyseOutCardType(AnalysedCard,wChairId,CT_DOUBLE,NULL,0,cbCardData,cbCardCount,DoubleType);
						if( SingleType.bRemainCount+DoubleType.bRemainCount > 3 ) return false;
					}

					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_TIE_ZHI:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					if( GetCardLogicValue(OutCardType.bCardData[0])>=14 )
					{
						tagOutCardType SingleType,DoubleType;
						AnalyseOutCardType(AnalysedCard,wChairId,CT_SINGLE,NULL,0,cbCardData,cbCardCount,SingleType);
						AnalyseOutCardType(AnalysedCard,wChairId,CT_DOUBLE,NULL,0,cbCardData,cbCardCount,DoubleType);
						if( SingleType.bRemainCount+DoubleType.bRemainCount > 3 ) return false;
					}

					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		case CT_TONG_HUA_SHUN:
			{
				tagOutCardType OutCardType;
				if( AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,bTurnCardData,bTurnCardCount,
					cbCardData,cbCardCount,OutCardType) )
				{
					tagOutCardType OutCardType1;
					AnalyseOutCardType(AnalysedCard,wChairId,bTurnCardType,NULL,0,
						cbCardData,cbCardCount,OutCardType1);

					if( OutCardType.bPower || OutCardType.bOutAll )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					if( OutCardType1.bRemainCount >= OutCardType.bRemainCount )
					{
						OutCardResult.cbCardCount = OutCardType.bCardCount;
						CopyMemory(OutCardResult.cbResultCard,OutCardType.bCardData,sizeof(BYTE)*OutCardResult.cbCardCount);
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
					return false;
				}
				else return false;
			}
		}
	}


	return false;
}


//���ȳ�������
bool CGameLogic::SearchFirstOutCard( const tagAnalysedCard &AnalysedCard,tagOutCardResult &OutCardResult,bool &bSplit )
{
	//
	ZeroMemory( &OutCardResult,sizeof(tagOutCardResult) );

	/**
	*���ȳ��Ʋ���
	*1.������3�������ͳ���,��Ϊ��«�������Ҵ���Q�������
	**/
	BYTE bOutCardData[MAX_COUNT];
	BYTE bOutCardCount = 0;
	for( int i = 0; i < CT_TONG_HUA_SHUN; i++ )
	{
		int nCount = (i >= CT_SHUN_ZI-1)?5:i+1;
		for( int j = 0; j < AnalysedCard.bCardCount[i]*nCount; j++ )
		{
			if( AnalysedCard.bCardData[i][j] == 0x03 )
			{
				BYTE bIndex = j / nCount;
				bOutCardCount = nCount;
				CopyMemory( bOutCardData,&AnalysedCard.bCardData[i][bIndex*nCount],nCount*sizeof(BYTE) );
				SortCardList( bOutCardData,bOutCardCount );
				break;
			}
		}
	}
	
	if( bOutCardCount > 0 )
	{
		BYTE bCardType = GetCardType( bOutCardData,bOutCardCount );
		if( bCardType == CT_HU_LU && GetCardLogicValue(bOutCardData[0]) >= 12 )
		{
			OutCardResult.cbCardCount = 2;
			CopyMemory( OutCardResult.cbResultCard,&bOutCardData[5-OutCardResult.cbCardCount],sizeof(BYTE)*OutCardResult.cbCardCount );
			bSplit = true;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		if( bCardType == CT_TIE_ZHI && GetCardLogicValue(bOutCardData[0]) != 3 )
		{
			OutCardResult.cbCardCount = 1;
			OutCardResult.cbResultCard[0] = 0x03;
			bSplit = true;
			return true;
		}
		OutCardResult.cbCardCount = bOutCardCount;
		CopyMemory( OutCardResult.cbResultCard,bOutCardData,bOutCardCount*sizeof(BYTE) );
		return true;
	}
	ASSERT(FALSE);
	return false;
}

//ǿ�Ƴ����������
bool CGameLogic::SearchForcedOutCard(const tagAnalysedCard& AnalysedCard,WORD wChairId,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE bOutCardRecord[],BYTE bOutCardCount,
		const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardResult& OutCardResult,bool &bReAnalyse)
{
	//
	ZeroMemory( &OutCardResult,sizeof(tagOutCardResult) );
	bReAnalyse = false;

	/**
	*ǿ�Ƴ�����Ʋ���
	*1.�����ȳ���:
	*	a.�����ų��������
	*	b.�����������
	*2.�����״γ��ƣ�
	*	a.���ϼҳ�����>1,��ѹ�Ʋ���
	*	b.���ϼҳ����ţ���ѹ���֧
	**/

	BYTE bMostCount=0,bLessCount=0;
	bool bTurn = bTurnCardCount==0?true:false;
	bool bFinish = IsOutCardFinish(wChairId,AnalysedCard,bOutCardRecord,bOutCardCount,bTurn,cbCardData,cbCardCount,bMostCount,bLessCount);


	if( bTurnCardCount == 0 )
	{
		//�жϳ���
		if( bFinish && bLessCount<=1 )
		{
			for( int i = CountArray(AnalysedCard.bCardCount)-1; i > 0 ; i-- )
			{
				BYTE bCount = i>=CT_SHUN_ZI-1?5:i+1;
				for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
				{
					if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,
						cbCardData,cbCardCount) )
					{
						OutCardResult.cbCardCount = bCount;
						CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[i][j*bCount],
							sizeof(BYTE)*OutCardResult.cbCardCount );
						SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
						ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
						return true;
					}
				}
			}
		}
		//˳��
		if( AnalysedCard.bCardCount[CT_SHUN_ZI-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_SHUN_ZI-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//ͬ��
		if( AnalysedCard.bCardCount[CT_TONG_HUA-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TONG_HUA-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//��«
		if( AnalysedCard.bCardCount[CT_HU_LU-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_HU_LU-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//��֧
		if( AnalysedCard.bCardCount[CT_TIE_ZHI-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TIE_ZHI-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//����
		if( AnalysedCard.bCardCount[CT_FOUR_TIAO-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_FOUR_TIAO-1],sizeof(BYTE)*4 );
			OutCardResult.cbCardCount = 4;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//ͬ��˳
		if( AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1],sizeof(BYTE)*5 );
			OutCardResult.cbCardCount = 5;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//����
		if( AnalysedCard.bCardCount[CT_THREE_TIAO-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_THREE_TIAO-1],sizeof(BYTE)*3 );
			OutCardResult.cbCardCount = 3;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}
		//����
		if( AnalysedCard.bCardCount[CT_DOUBLE-1] > 0 )
		{
			CopyMemory( OutCardResult.cbResultCard,
				AnalysedCard.bCardData[CT_DOUBLE-1],sizeof(BYTE)*2 );
			OutCardResult.cbCardCount = 2;
			SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
			return true;
		}

		//�������
		BYTE bHandCardData[MAX_COUNT],bHandCardCount;
		CopyMemory(bHandCardData,cbCardData[wChairId],sizeof(BYTE)*cbCardCount[wChairId]);
		bHandCardCount = cbCardCount[wChairId];
		SortCardList( bHandCardData,bHandCardCount );
		OutCardResult.cbResultCard[0] = bHandCardData[0];
		OutCardResult.cbCardCount = 1;
		bReAnalyse = true;
		return true;
	}
	else
	{
		if( bTurnCardCount == 1 )
		{
			//�������
			BYTE bHandCardData[MAX_COUNT],bHandCardCount;
			CopyMemory(bHandCardData,cbCardData[wChairId],sizeof(BYTE)*cbCardCount[wChairId]);
			bHandCardCount = cbCardCount[wChairId];
			SortCardList( bHandCardData,bHandCardCount );
			if( CompareCard( bTurnCardData[0],bHandCardData[0] ) )
			{
				OutCardResult.cbResultCard[0] = bHandCardData[0];
				OutCardResult.cbCardCount = 1;
				bReAnalyse = true;
				return true;
			}
			return false;
		}
		else						//��ѹ�Ʋ���
		{
			BYTE bTurnCardType = GetCardType( bTurnCardData,bTurnCardCount );
			ASSERT( bTurnCardType > CT_SINGLE );
			if( bTurnCardType <= CT_SINGLE ) return false;

			if( bFinish )
			{
				if( SearchOutCard(cbCardData[wChairId],cbCardCount[wChairId],bTurnCardData,bTurnCardCount,
					OutCardResult) )
				{
					ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
					if( IsTurnCardMost(wChairId,OutCardResult.cbResultCard,OutCardResult.cbCardCount,
						cbCardData,cbCardCount) )
					{
						tagAnalysedCard tmpAnalysedCard;
						BYTE bHandCard[MAX_COUNT],bHandCardCount;
						BYTE bTmpOutCardRecord[FULL_COUNT],bTmpOutCardRecordCount;
						BYTE bTmpMostCount=0,bTmpLessCount=0;

						bHandCardCount = cbCardCount[wChairId];
						CopyMemory(bHandCard,cbCardData[wChairId],sizeof(BYTE)*bHandCardCount);
						VERIFY( RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bHandCardCount) );
						bHandCardCount -= OutCardResult.cbCardCount;

						AnalyseCardData(bHandCard,bHandCardCount,tmpAnalysedCard);

						CopyMemory(bTmpOutCardRecord,bOutCardRecord,sizeof(BYTE)*bOutCardCount);
						bTmpOutCardRecordCount = bOutCardCount;
						CopyMemory(&bTmpOutCardRecord[bTmpOutCardRecordCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
						bTmpOutCardRecordCount += OutCardResult.cbCardCount;

						if( IsOutCardFinish(wChairId,tmpAnalysedCard,bTmpOutCardRecord,bTmpOutCardRecordCount,
							true,cbCardData,cbCardCount,bTmpMostCount,bTmpLessCount) )
						{
							ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
							if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
								return false;
							bReAnalyse = true;
							return true;
						}
					}
				}
				if( SearchTurnCard(cbCardData[wChairId],cbCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult) )
				{
					ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
					tagAnalysedCard tmpAnalysedCard;
					BYTE bHandCard[MAX_COUNT],bHandCardCount;
					BYTE bTmpOutCardRecord[FULL_COUNT],bTmpOutCardRecordCount;
					BYTE bTmpMostCount=0,bTmpLessCount=0;

					bHandCardCount = cbCardCount[wChairId];
					CopyMemory(bHandCard,cbCardData[wChairId],sizeof(BYTE)*bHandCardCount);
					VERIFY( RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bHandCardCount) );
					bHandCardCount -= OutCardResult.cbCardCount;

					AnalyseCardData(bHandCard,bHandCardCount,tmpAnalysedCard);

					CopyMemory(bTmpOutCardRecord,bOutCardRecord,sizeof(BYTE)*bOutCardCount);
					bTmpOutCardRecordCount = bOutCardCount;
					CopyMemory(&bTmpOutCardRecord[bTmpOutCardRecordCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);
					bTmpOutCardRecordCount += OutCardResult.cbCardCount;

					if( IsOutCardFinish(wChairId,tmpAnalysedCard,bTmpOutCardRecord,bTmpOutCardRecordCount,
						true,cbCardData,cbCardCount,bTmpMostCount,bTmpLessCount) )
					{
						ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
						if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
							return false;
						bReAnalyse = true;
						return true;
					}
				}
			}
			switch( bTurnCardType )
			{
			case CT_DOUBLE:
			case CT_THREE_TIAO:
			case CT_FOUR_TIAO:
				{
					int nCount = AnalysedCard.bCardCount[bTurnCardType-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[bTurnCardType-1][i*bTurnCardType],
							bTurnCardType,bTurnCardType ) )
						{
							OutCardResult.cbCardCount = bTurnCardType;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[bTurnCardType-1][i*bTurnCardType],
								sizeof(BYTE)*bTurnCardType );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
					return false;
				}
			case CT_SHUN_ZI:
				{
					int nCount = AnalysedCard.bCardCount[CT_SHUN_ZI-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_SHUN_ZI-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_SHUN_ZI-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
				}
			case CT_TONG_HUA:
				{
					int nCount = AnalysedCard.bCardCount[CT_TONG_HUA-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_TONG_HUA-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_TONG_HUA-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
				}
			case CT_HU_LU:
				{
					int nCount = AnalysedCard.bCardCount[CT_HU_LU-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_HU_LU-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_HU_LU-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
				}
			case CT_TIE_ZHI:
				{
					int nCount = AnalysedCard.bCardCount[CT_TIE_ZHI-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_TIE_ZHI-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_TIE_ZHI-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
				}
			case CT_TONG_HUA_SHUN:
				{
					int nCount = AnalysedCard.bCardCount[CT_TONG_HUA_SHUN-1];
					for( int i = 0; i < nCount; i++ )
					{
						if( CompareCard( bTurnCardData,&AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1][i*5],
							5,5 ) )
						{
							OutCardResult.cbCardCount = 5;
							CopyMemory( OutCardResult.cbResultCard,&AnalysedCard.bCardData[CT_TONG_HUA_SHUN-1][i*5],
								sizeof(BYTE)*5 );
							SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
							return true;
						}
					}
					return false;
				}
			}
		}
	}
	return false;
}


//ץ��
bool CGameLogic::HuntUser( const tagAnalysedCard &AnalysedCard,BYTE bHandCardData[],BYTE bHandCardCount,BYTE bNextUserCardCount,tagOutCardResult &OutCardResult,bool &bSplit )
{
	if( bNextUserCardCount == 1 )				//ǿ�Ƴ����
	{
		SortCardList( bHandCardData,bHandCardCount );
		OutCardResult.cbCardCount = 1;
		OutCardResult.cbResultCard[0] = bHandCardData[0];
		bSplit = true;
		return true;
	}

	if( bNextUserCardCount < 5 && AnalysedCard.bCardCount[bNextUserCardCount-1] > 0 )
	{
		OutCardResult.cbCardCount = bNextUserCardCount;
		CopyMemory( OutCardResult.cbResultCard,AnalysedCard.bCardData[bNextUserCardCount-1],sizeof(BYTE)*bNextUserCardCount );
		SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
		return true;
	}
	else if( bNextUserCardCount == 5 )
	{
		BYTE bCardType = CT_SHUN_ZI;
		for( int i = bCardType; i <= CT_TONG_HUA_SHUN; i++ )
		{
			if( AnalysedCard.bCardCount[i-1] > 0 )
			{
				OutCardResult.cbCardCount = bNextUserCardCount;
				CopyMemory( OutCardResult.cbResultCard,AnalysedCard.bCardData[i-1],sizeof(BYTE)*bNextUserCardCount );
				SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				return true;
			}
		}
	}
	return false;
}

//�Զ�����
bool CGameLogic::AutomatismPlay( tagAnalysedCard &AnalysedCard,WORD wChairId,WORD wOutCardUser,const BYTE bTurnCardData[],BYTE bTurnCardCount,const BYTE (&bCardData)[GAME_PLAYER][13],
								const BYTE bCardCount[],const BYTE bOutCardRecord[],BYTE bOutCardCount,tagOutCardResult &OutCardResult,bool bOutCard3 )
{
	if( wChairId == INVALID_CHAIR ) return false;

	//��ʼ��
	ZeroMemory( &OutCardResult,sizeof(tagOutCardResult) );

	WORD wPreChairId = wChairId == 0 ? 3:wChairId-1;			//�ϼ�
	WORD wLastPreChair = wPreChairId == 0 ? 3:wPreChairId-1;	//�������
	WORD wNextChairId = (wChairId+1) % GAME_PLAYER;				//�¼�

	//
	BYTE bHandCard[MAX_COUNT];
	CopyMemory( bHandCard,bCardData[wChairId],sizeof(BYTE)*bCardCount[wChairId] );
	SortCardList(bHandCard,bCardCount[wChairId]);

	//�Ƿ���Գ�����
	BYTE bMostCount=0,bLessCount=0;
	bool bTurn = bTurnCardCount==0?true:false;
	bool bFinish = IsOutCardFinish(wChairId,AnalysedCard,bOutCardRecord,bOutCardCount,bTurn,bCardData,bCardCount,bMostCount,bLessCount);

	//�ж�ץ��
	if( bCardCount[wChairId]<8 && bCardCount[wChairId]>2 && !bFinish &&
		(bCardCount[wPreChairId]>7||bCardCount[wLastPreChair]>7||bCardCount[wNextChairId]>7) &&
		(bCardCount[wPreChairId]<=2||bCardCount[wLastPreChair]<=2||bCardCount[wNextChairId]<=2)
		)
	{
		if( bCardCount[wOutCardUser]==2 && (wNextChairId==wOutCardUser ||
			wLastPreChair==wOutCardUser) && bCardCount[wNextChairId]<8 &&(bCardCount[wNextChairId]>1||bTurnCardCount>1) )
			return false;
		if( bCardCount[wOutCardUser]==1 && wNextChairId==wOutCardUser && bTurnCardCount > 1 )
			return false;
		if( bCardCount[wOutCardUser]==1 && wLastPreChair==wOutCardUser && 
			(bCardCount[wNextChairId]>1||bTurnCardCount>1) && bCardCount[wNextChairId]<8 )
			return false;
		if( bTurnCardCount == 0 )
		{
			BYTE bIndex = 0;
			if( bCardCount[wNextChairId]==1 )
				bIndex = 0;
			else if( bCardCount[wNextChairId]==2 || bCardCount[wLastPreChair]<=2&&bCardCount[wPreChairId]>7&&bCardCount[wNextChairId]<8 )
			{
				bIndex = bCardCount[wChairId]-1;
				WORD wId;
				if( bCardCount[wNextChairId]==2 ) wId = wNextChairId;
				else wId = wLastPreChair;
				BYTE bCardType = GetCardType(bCardData[wId],bCardCount[wId]);
				if( bCardType > CT_ERROR )
				{
					ASSERT( bCardType <= CT_DOUBLE );
					if( AnalysedCard.bCardCount[bCardType-1] > 0 )
					{
						if( CompareCard(AnalysedCard.bCardData[bCardType-1][0],bCardData[wId][0]) )
						{
							OutCardResult.cbCardCount = bCardType;
							CopyMemory(OutCardResult.cbResultCard,AnalysedCard.bCardData[bCardType-1],sizeof(BYTE)*bCardType);
							VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
							//���·����ֲ�
							AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
							return true;
						}
					}
				}
			}
			OutCardResult.cbCardCount = 1;
			OutCardResult.cbResultCard[0] = bHandCard[bIndex];
			VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
			//���·����ֲ�
			AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
			return true;
		}else
		{
			BYTE bTurnCardType=GetCardType(bTurnCardData,bTurnCardCount);
			ASSERT( bTurnCardType>CT_ERROR );
			if( (bCardCount[wNextChairId]==2||
				(bCardCount[wLastPreChair]==2&&bCardCount[wPreChairId]>7&&bCardCount[wNextChairId]<8&&
				 (bCardCount[wNextChairId]>1||bTurnCardType==CT_DOUBLE))) &&
				bTurnCardType<=CT_DOUBLE )
				return false;
			if( bCardCount[wLastPreChair]==1&&bCardCount[wNextChairId]>1&&bCardCount[wNextChairId]<8 &&
				bTurnCardType==CT_SINGLE )
				return false;
			//�������
			BYTE bReferCard=0;
			switch(bTurnCardType)
			{
			case CT_SINGLE:					//��������,�����
				{
					//�����˿�
					tagAnalyseResult AnalyseResult;
					AnalysebCardData(bHandCard,bCardCount[wChairId],AnalyseResult);
					//��ȡ��ֵ
					BYTE cbTurnCard=bTurnCardData[0];
					//Ѱ���˿�
					for (BYTE i=0;i<CountArray(AnalyseResult.cbCardCount);i++)
					{
						//��С�Ա�
						if( AnalyseResult.cbCardCount[i] == 0 ) continue;
						if (CompareCard(cbTurnCard,AnalyseResult.cbCardData[i][0])==false) continue;
						else cbTurnCard = AnalyseResult.cbCardData[i][0];
					}
					if( cbTurnCard != bTurnCardData[0] )
					{
						OutCardResult.cbCardCount=1;
						OutCardResult.cbResultCard[0]=cbTurnCard;
						VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
						//���·����ֲ�
						AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
						return true;
					}
					return false;
				}
			case CT_DOUBLE:					//��������
			case CT_THREE_TIAO:				//��������
			case CT_FOUR_TIAO:				//��������
				{										//�����
					//�����˿�
					tagAnalyseResult AnalyseResult;
					AnalysebCardData(bHandCard,bCardCount[wChairId],AnalyseResult);
					//��ȡ��ֵ
					BYTE cbTurnCard=bTurnCardData[0];
					BYTE bIndex = 0;
					//Ѱ���˿�
					for (BYTE i=bTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
					{
						if( AnalyseResult.cbCardCount[i] == 0 ) continue;
						if( i==CT_SHUN_ZI-1 || i==CT_TONG_HUA-1 || i==CT_TONG_HUA_SHUN-1 || (bTurnCardType==CT_FOUR_TIAO&&i==CT_HU_LU-1) )
							continue;
						if (CompareCard(bTurnCardData[0],AnalyseResult.cbCardData[i][0])==false) continue;
						else 
						{
							cbTurnCard = AnalyseResult.cbCardData[i][0];
							bIndex = i;
						}
					}
					if( cbTurnCard != bTurnCardData[0] )
					{
						//���ý��
						OutCardResult.cbCardCount=bTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,AnalyseResult.cbCardData[bIndex],sizeof(BYTE)*bTurnCardCount);
						SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
						VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
						//���·����ֲ�
						AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
						return true;
					}
					return false;
				}
			case CT_SHUN_ZI:
			case CT_TONG_HUA:
			case CT_HU_LU:
			case CT_TIE_ZHI:
			case CT_TONG_HUA_SHUN:
				{					//�������
					BYTE bOutCard[5],bTempData[5];
					bool bFound = false;
					tagDistributing Distributing;
					AnalysebDistributing( bHandCard,bCardCount[wChairId],Distributing );
					bReferCard = bTurnCardType < CT_TONG_HUA_SHUN ? 0x00:bTurnCardData[0];
					while( SearchTongHuaShun( Distributing,bReferCard,bTempData ) != 0 )
					{
						bFound = true;
						bReferCard = bTempData[0];
						CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
					}
					if( !bFound && bTurnCardType < CT_TONG_HUA_SHUN )
					{
						bReferCard = bTurnCardType < CT_TIE_ZHI ? 0x00:bTurnCardData[1];
						while( SearchTieZhi(Distributing,bReferCard,bTempData) != 0 )
						{
							bFound = true;
							bReferCard = bTempData[0];
							CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
						}
					}
					if( !bFound && bTurnCardType < CT_TIE_ZHI )
					{
						bReferCard = bTurnCardType < CT_HU_LU ? 0x00:bTurnCardData[2];
						while( SearchHuLu(Distributing,bReferCard,bTempData) != 0 )
						{
							bFound = true;
							bReferCard = bTempData[0];
							CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
						}
					}
					if( !bFound && bTurnCardType < CT_HU_LU )
					{
						bReferCard = bTurnCardType < CT_TONG_HUA ? 0x00:bTurnCardData[0];
						while( SearchTongHua(Distributing,bReferCard,bTempData) != 0 )
						{
							bFound = true;
							bReferCard = bTempData[0];
							CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
						}
					}
					if( !bFound && bTurnCardType < CT_TONG_HUA )
					{
						bReferCard = bTurnCardData[0];
						while( SearchShunZi(Distributing,bReferCard,bTempData) != 0 )
						{
							bFound = true;
							bReferCard = bTempData[0];
							CopyMemory(bOutCard,bTempData,sizeof(BYTE)*5);
						}
					}
					if( bFound )
					{
						OutCardResult.cbCardCount=5;
						CopyMemory(OutCardResult.cbResultCard,bTempData,sizeof(BYTE)*5);
						SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
						VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
						//���·����ֲ�
						AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
						return true;
					}
					return false;
				}
			}
			
			ASSERT(FALSE);
			return false;
		}
	}

	if ( bOutCard3 )								//�״γ���
	{
		bool bReAnalyse = false;
		if( SearchFirstOutCard( AnalysedCard,OutCardResult,bReAnalyse ) )
		{
			if( bReAnalyse == false )
			{
				BYTE bType = GetCardType( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				ASSERT( bType > CT_ERROR );
				int nCount = (bType >= 5)?5:bType;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,
					AnalysedCard.bCardData[bType-1],AnalysedCard.bCardCount[bType-1]*nCount ) );
				AnalysedCard.bCardCount[bType-1]--;
			}else
			{
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
			}
			return true;
		}

		ASSERT(FALSE);

		return false;
	}
	else if( bCardCount[wNextChairId]==1 )			//ǿ�Ƴ����
	{
		bool bReAnalyse = false;
		if( SearchForcedOutCard( AnalysedCard,wChairId,bTurnCardData,bTurnCardCount,bOutCardRecord,bOutCardCount,
			bCardData,bCardCount,OutCardResult,bReAnalyse ) )
		{
			ASSERT( bTurnCardCount==0 || CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
			if( bTurnCardCount>1 && CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
				return false;
			if( bReAnalyse == false )
			{
				BYTE bType = GetCardType( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				ASSERT( bType > CT_ERROR );
				int nCount = (bType >= 5)?5:bType;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,
					AnalysedCard.bCardData[bType-1],AnalysedCard.bCardCount[bType-1]*nCount ) );
				AnalysedCard.bCardCount[bType-1]--;
			}else
			{
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
			}
			return true;
		}
		else if( bTurnCardCount > 1 && bCardCount[wOutCardUser] <= 5 )
		{
			//�������
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		else return false;

	}
	else							//�������
	{
		//ֻʣ��2��ʱ
		if( bCardCount[wChairId] == 2 )
		{
			if( bTurnCardCount == 0 )
			{
				if( GetCardLogicValue(bHandCard[0]) == GetCardLogicValue(bHandCard[1]) )
				{
					OutCardResult.cbCardCount = 2;
					CopyMemory( OutCardResult.cbResultCard,bHandCard,sizeof(BYTE)*OutCardResult.cbCardCount );
					return true;
				}
				BYTE bMostCard = GetMostCardValue(AnalysedCard,bOutCardRecord,bOutCardCount);
				BYTE bIndex = CompareCard(bMostCard,bHandCard[0])||bMostCard==bHandCard[0] ? 0:1;
				OutCardResult.cbCardCount = 1;
				OutCardResult.cbResultCard[0] = bHandCard[bIndex];
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else if( bTurnCardCount == 1 )
			{
				BYTE bMostCard = GetMostCardValue(AnalysedCard,bOutCardRecord,bOutCardCount);
				BYTE bIndex = CompareCard(bMostCard,bHandCard[0])||bMostCard==bHandCard[0] ? 0:1;
				if( CompareCard(bTurnCardData[0],bHandCard[bIndex]))
				{
					OutCardResult.cbCardCount = 1;
					OutCardResult.cbResultCard[0] = bHandCard[bIndex];
					VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
					//���·����ֲ�
					AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
					return true;
				}
				else return false;
			}
			else if( GetCardLogicValue(bHandCard[1])==GetCardLogicValue(bHandCard[0]) && CompareCard(bTurnCardData,bHandCard,bTurnCardCount,2) )
			{
				OutCardResult.cbCardCount = 2;
				OutCardResult.cbResultCard[0] = bHandCard[0];
				OutCardResult.cbResultCard[1] = bHandCard[1];
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		//������Ϊ�����Լ���С�ڣ�����������ҿɼӱ�
		if( bCardCount[wOutCardUser]==3 && bCardCount[wChairId]<8 && wOutCardUser!=wChairId && bCardCount[wChairId]>1 &&
			(bCardCount[wPreChairId]>7||bCardCount[wLastPreChair]>7||bCardCount[wNextChairId]>7) &&
			!bFinish && GetCardLogicValue(bTurnCardData[0])>=11 && bTurnCardCount<=CT_DOUBLE)
			return false;
		bool bReAnalyse = false;
		if( SearchFromAnalysedCard( AnalysedCard,wChairId,bTurnCardData,bTurnCardCount,bOutCardRecord,bOutCardCount,
			bCardData,bCardCount,OutCardResult,bReAnalyse ) )
		{
			ASSERT( bTurnCardCount==0 || CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==true );
			if( bTurnCardCount>0 && CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
				return false;
			if( bReAnalyse == false )
			{
				BYTE bType = GetCardType( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
				ASSERT( bType > CT_ERROR );
				int nCount = (bType >= 5)?5:bType;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,
					AnalysedCard.bCardData[bType-1],AnalysedCard.bCardCount[bType-1]*nCount ) );
				AnalysedCard.bCardCount[bType-1]--;
			}else
			{
				ASSERT( GetCardType(OutCardResult.cbResultCard,OutCardResult.cbCardCount)>CT_ERROR );
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
			}

			return true;

		}
		//�Լ����ڣ���������С�ڣ�
		else if( bCardCount[wChairId]>7 && bCardCount[wOutCardUser]<8 )
		{
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		//�Լ����ڣ���������С�ڣ�
		else if( bCardCount[wChairId]>7 )
		{
			BYTE bLessCount = 0;
			for( int i=0; i < 4; i++ )
			{
				if( i == wChairId ) continue;
				if( bCardCount[i] < 8 ) bLessCount++;
			}
			if( bLessCount > 1 )
			{
				if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
				{
					ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
					if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
						return false;
					VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
					//���·����ֲ�
					AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
					return true;
				}
				else return false;
			}
		}
		//�Լ�С�ڣ��������
		else if( bCardCount[wChairId]<8 && bCardCount[wChairId]>bCardCount[wNextChairId] &&
			bCardCount[wChairId]>bCardCount[wLastPreChair] && bCardCount[wChairId]>bCardCount[wPreChairId] )
		{
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		//�Լ�С��8����������С���Լ�
		else if( bCardCount[wChairId]<8 &&
			( bCardCount[wChairId]>bCardCount[wNextChairId]&&bCardCount[wChairId]>bCardCount[wLastPreChair] ||
			bCardCount[wChairId]>bCardCount[wNextChairId]&&bCardCount[wChairId]>bCardCount[wPreChairId] ||
			bCardCount[wChairId]>bCardCount[wPreChairId]&&bCardCount[wChairId]>bCardCount[wLastPreChair] )
			)
		{
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}
		//����ȫ��С��5
		else if( /*bCardCount[wChairId]<8 &&*/ bCardCount[wPreChairId]<5 && bCardCount[wLastPreChair]<5 &&
			bCardCount[wNextChairId]<5 )
		{
			if( SearchOutCard( bHandCard,bCardCount[wChairId],bTurnCardData,bTurnCardCount,OutCardResult ) )
			{
				ASSERT( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount) );
				if( CompareCard(bTurnCardData,OutCardResult.cbResultCard,bTurnCardCount,OutCardResult.cbCardCount)==false )
					return false;
				VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bHandCard,bCardCount[wChairId] ) );
				//���·����ֲ�
				AnalyseCardData( bHandCard,bCardCount[wChairId]-OutCardResult.cbCardCount,AnalysedCard );
				return true;
			}
			else return false;
		}

	}
	return false;
}


//��ȡ�����
BYTE CGameLogic::GetMostCardValue( const tagAnalysedCard &AnalysedCard,const BYTE bOutCardRecord[],BYTE bOutCardCount )
{
	BYTE bCardRecord[FULL_COUNT],bCardCount,bMostCard;
	ZeroMemory( bCardRecord,FULL_COUNT*sizeof(BYTE) );

	int i = 1;
	bCardCount = 0;
	for( i = 0; i < CountArray(AnalysedCard.bCardCount); i++ )
		for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
			bCardRecord[bCardCount++] = AnalysedCard.bCardData[i][j];

	CopyMemory( &bCardRecord[bCardCount],bOutCardRecord,sizeof(BYTE)*bOutCardCount );
	bCardCount += bOutCardCount;
	ASSERT( bCardCount <= FULL_COUNT );
	if( bCardCount > FULL_COUNT ) 
		return false;
	SortCardList( bCardRecord,bCardCount,ST_ORDER );

	BYTE bColorIndex[4] = { 0,0,0,0 };
	BYTE bMostIndex = 0;
	BYTE bSameCount = 1;
	BYTE bMostValue = 15;	// 2
	for( i = 1; i < bCardCount; i++ )
	{
		if( GetCardLogicValue(bCardRecord[bMostIndex]) < bMostValue ) 
		{
			bSameCount = 0;
			break;
		}
		if( GetCardLogicValue(bCardRecord[bMostIndex]) == GetCardLogicValue(bCardRecord[i]) ) 
			bSameCount++;
		else break;
		if( bSameCount == 4 )
		{
			bMostValue--;
			bMostIndex = i+1;
			bSameCount = 1;
			i++;
		}
	}
	for( i = bMostIndex; i < bMostIndex+bSameCount; i++ )
	{
		BYTE bIndex = (bCardRecord[i]>>4);
		bColorIndex[bIndex]++;
	}
	for( i = 3; i >= 0; i-- )
		if( bColorIndex[i]==0 ) break;

	ASSERT( i>=0 );
	bMostCard = MakeCardData(bMostValue-3,i );

	return bMostCard;
}

//�ж��Ƿ���Գ���
bool CGameLogic::IsOutCardFinish(WORD wChairId,const tagAnalysedCard &AnalysedCard,const BYTE bOutCardRecord[],BYTE bOutCardCount,bool bTurn,
								 const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],BYTE &bMostCount,BYTE &bLessCount )
{
	ASSERT( wChairId != INVALID_CHAIR );
	if( wChairId == INVALID_CHAIR ) return false;

	BYTE bMostCard = GetMostCardValue(AnalysedCard,bOutCardRecord,bOutCardCount);
	bLessCount = 0;
	bMostCount = 0;
	bLessCount = 0;
	for( int i = 0; i < CountArray(AnalysedCard.bCardCount); i++ )
	{
		BYTE bCount = i>=CT_SHUN_ZI-1?5:i+1;
		for( int j = 0; j < AnalysedCard.bCardCount[i]; j++ )
		{
			if( i==CT_SHUN_ZI-1 || i==CT_TONG_HUA-1 || i==CT_TONG_HUA_SHUN-1 )
			{
				if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,
					cbCardData,cbCardCount) )
					bMostCount++;
				else
					bLessCount++;
				continue;
			}
			if( (i==CT_HU_LU-1||i==CT_TIE_ZHI-1) && CompareCard(AnalysedCard.bCardData[i][j*bCount],bMostCard) )
			{
				if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,
					cbCardData,cbCardCount) )
					bMostCount++;
				else
					bLessCount++;
				continue;
			}
			if( CompareCard(bMostCard,AnalysedCard.bCardData[i][j*bCount]) || bMostCard==AnalysedCard.bCardData[i][j*bCount] )
			{
				if( i==CT_HU_LU-1 || i==CT_TIE_ZHI-1 )
				{
					bMostCount += (i-3);
					bLessCount++;
				}else
					bMostCount += bCount;
			}
			else 
			{
				if( IsTurnCardMost(wChairId,&AnalysedCard.bCardData[i][j*bCount],bCount,cbCardData,cbCardCount) )
					bMostCount++;
				else
					bLessCount++;
			}
		}
	}
	
	//�ȳ���
	if( bTurn ) 
		return bMostCount+1>=bLessCount;
	else return bMostCount>=bLessCount;
}

//��������
bool CGameLogic::AnalyseOutCardType( const tagAnalysedCard &cAnalysedCard,WORD wMeChairId,BYTE bCardType,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
									const BYTE (&cbCardData)[GAME_PLAYER][13],const BYTE cbCardCount[],tagOutCardType &OutCardType )
{
	//��ʼ��
	ZeroMemory( &OutCardType,sizeof(OutCardType) );

	tagAnalysedCard AnalysedCard;
	CopyMemory( &AnalysedCard,&cAnalysedCard,sizeof(cAnalysedCard) );

	int bRemainCount=0;
	int bMostCount = 0;

	BYTE bCardData[GAME_PLAYER][13];
	BYTE bCardCount[GAME_PLAYER];
	CopyMemory( bCardData,cbCardData,sizeof(bCardData) );
	CopyMemory( bCardCount,cbCardCount,sizeof(bCardCount) );

	tagOutCardResult OutCardResult;
	BYTE bTempData[MAX_COUNT],bTempCount=0;
	BYTE bLessTypeIndex=0,bLessIndex = 0;

	//˳����������
	bLessTypeIndex = bCardType-1;
	if( bCardType >= CT_SHUN_ZI && bCardType <= CT_TONG_HUA_SHUN )
	{
		for( int k = CT_TONG_HUA_SHUN-1; k >= bCardType-1; k-- )
		{
			for( int i=AnalysedCard.bCardCount[k]-1; i>=0; i-- )
			{
				bTempCount = 5;
				CopyMemory( bTempData,&AnalysedCard.bCardData[k][i*bTempCount],sizeof(BYTE)*bTempCount );
				ASSERT( GetCardType(bTempData,bTempCount)>CT_ERROR );
				for( WORD j = (wMeChairId+1)%GAME_PLAYER; j != wMeChairId; j = (j+1)%GAME_PLAYER )
				{
					if( bCardCount[j]==0 ) continue;
					if( SearchTurnCard(bCardData[j],bCardCount[j],bTempData,bTempCount,OutCardResult) )
					{
						VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bCardData[j],bCardCount[j] ) );
						bCardCount[j] -= OutCardResult.cbCardCount;
						break;
					}
				}
				if( j == wMeChairId ) bMostCount++;
				else	bRemainCount++;
			}
		}
	}
	else						//˳����������
	{
		bTempCount = bCardType>=5?5:bCardType;
		for( int i=AnalysedCard.bCardCount[bCardType-1]-1; i>=0; i-- )
		{
			CopyMemory( bTempData,&AnalysedCard.bCardData[bCardType-1][i*bTempCount],sizeof(BYTE)*bTempCount );
			ASSERT( GetCardType(bTempData,bTempCount)>CT_ERROR );
			for( WORD j = (wMeChairId+1)%GAME_PLAYER; j != wMeChairId; j = (j+1)%GAME_PLAYER )
			{
				if( bCardCount[j]==0 ) continue;
				if( SearchTurnCard(bCardData[j],bCardCount[j],bTempData,bTempCount,OutCardResult) )
				{
					VERIFY( RemoveCard( OutCardResult.cbResultCard,OutCardResult.cbCardCount,bCardData[j],bCardCount[j] ) );
					bCardCount[j] -= OutCardResult.cbCardCount;
					break;
				}
			}
			if( j == wMeChairId ) bMostCount++;
			else bRemainCount++;
		}
	}

	ASSERT( bCardType > CT_ERROR && bCardType <=CT_TONG_HUA_SHUN );
	bool bFound = false;
	//ѹ��ʱ
	if( cbTurnCardCount > 0 )
	{
		BYTE bTurnCardType = GetCardType(cbTurnCardData,cbTurnCardCount);
		int i=bTurnCardType-1,j=0;
		switch(bTurnCardType)
		{
		case CT_SINGLE:
		case CT_DOUBLE:
		case CT_THREE_TIAO:
		case CT_FOUR_TIAO:
			{
				bTempCount = bTurnCardType;
				for( j = 0; j < AnalysedCard.bCardCount[bTurnCardType-1]; j++ )
				{
					if( CompareCard(cbTurnCardData,&AnalysedCard.bCardData[bTurnCardType-1][j*bTempCount],bTempCount,bTempCount) )
					{
						CopyMemory( bTempData,&AnalysedCard.bCardData[bTurnCardType-1][j*bTempCount],
							sizeof(BYTE)*bTempCount );
						ASSERT( GetCardType(bTempData,bTempCount)>CT_ERROR );
						bFound = true;
						break;
					}
				}
				if( !bFound ) return false;
				break;
			}
		case CT_SHUN_ZI:
		case CT_TONG_HUA:
		case CT_HU_LU:
		case CT_TIE_ZHI:
		case CT_TONG_HUA_SHUN:
			{
				bTempCount = 5;
				for( i = bTurnCardType-1; i < CT_TONG_HUA_SHUN; i++ )
				{
					for( j = 0; j < AnalysedCard.bCardCount[i]; j++ )
					{
						if( CompareCard(cbTurnCardData,&AnalysedCard.bCardData[i][j*5],5,5) )
						{
							CopyMemory( bTempData,&AnalysedCard.bCardData[i][j*5],
								sizeof(BYTE)*5 );
							ASSERT( GetCardType(bTempData,bTempCount)>CT_ERROR );
							bFound = true;
							break;
						}
					}
					if( bFound ) break;
				}
				if( !bFound ) return false;
				break;
			}
		}
		if( bFound )
		{
			bLessTypeIndex = i;
			bLessIndex = j;
			for( WORD n = (wMeChairId+1)%GAME_PLAYER; n != wMeChairId; n = (n+1)%GAME_PLAYER )
			{
				if( SearchTurnCard(cbCardData[n],cbCardCount[n],bTempData,bTempCount,OutCardResult) )
					break;
			}
			if( n == wMeChairId ) 
				bMostCount--;
			else bRemainCount--;
		}
	}
	
	//��������ʣ����
	if( bMostCount >= bRemainCount )
	{
		OutCardType.bPower = true;
		OutCardType.bOutAll = true;
		OutCardType.bRemainCount = 0;
		OutCardType.bCardCount = bLessTypeIndex>=4?5:bLessTypeIndex+1;
		CopyMemory( OutCardType.bCardData,&AnalysedCard.bCardData[bLessTypeIndex][bLessIndex*OutCardType.bCardCount],sizeof(BYTE)*OutCardType.bCardCount );
		SortCardList( OutCardType.bCardData,OutCardType.bCardCount );
	}
	else if( bMostCount >= bRemainCount-1 )
	{
		OutCardType.bPower = false;
		OutCardType.bOutAll = true;
		OutCardType.bRemainCount = 0;
		OutCardType.bCardCount = bLessTypeIndex>=4?5:bLessTypeIndex+1;
		CopyMemory( OutCardType.bCardData,&AnalysedCard.bCardData[bLessTypeIndex][bLessIndex*OutCardType.bCardCount],sizeof(BYTE)*OutCardType.bCardCount );
		SortCardList( OutCardType.bCardData,OutCardType.bCardCount );
	}
	else
	{
		OutCardType.bPower = false;
		OutCardType.bOutAll = false;
		OutCardType.bRemainCount = bRemainCount - bMostCount;
		OutCardType.bCardCount = bLessTypeIndex>=4?5:bLessTypeIndex+1;
		CopyMemory( OutCardType.bCardData,&AnalysedCard.bCardData[bLessTypeIndex][bLessIndex*OutCardType.bCardCount],sizeof(BYTE)*OutCardType.bCardCount );
		SortCardList( OutCardType.bCardData,OutCardType.bCardCount );
	}

	return bFound;
}


//��������
bool CGameLogic::SearchTurnCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,tagOutCardResult &OutCardResult)
{
	//��ʼ��
	ZeroMemory( &OutCardResult,sizeof(OutCardResult) );

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//�����ж�
	if (cbTurnCardCount>0)
	{
		//��ȡ����
		BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

		//������
		switch (cbTurnOutType)
		{
		case CT_SINGLE:					//��������
		case CT_DOUBLE:					//��������
		case CT_THREE_TIAO:				//��������
		case CT_FOUR_TIAO:				//��������
			{										//����С
				//�����˿�
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

				//��ȡ��ֵ
				BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

				//Ѱ���˿�
				for (BYTE i=cbTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
				{
					for (BYTE j=0;j<AnalyseResult.cbCardCount[i];j++)
					{
						//��С�Ա�
						BYTE cbIndex=(AnalyseResult.cbCardCount[i]-j-1)*(i+1);
						if (CompareCard(cbTurnCardData[0],AnalyseResult.cbCardData[i][cbIndex])==false) continue;
						//���ý��
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][cbIndex],sizeof(BYTE)*cbTurnCardCount);
						SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
						return true;
					}
				}

				return false;
			}
		}

		//�������
		if (cbCardCount>=5)
		{
			//�����ֲ�
			tagDistributing Distributing;
			AnalysebDistributing(cbCardData,cbCardCount,Distributing);

			//����˳��
			if (cbTurnOutType<=CT_SHUN_ZI)
			{
				//����˳��
				BYTE cbReferCard=(cbTurnOutType==CT_SHUN_ZI)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchShunZi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					//���ý��
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//����ͬ��
			if (cbTurnOutType<=CT_TONG_HUA)
			{
				//����ͬ��
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHua(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//������«
			if (cbTurnOutType<=CT_HU_LU)
			{
				//������«
				BYTE cbReferCard=(cbTurnOutType==CT_HU_LU)?cbTurnCardData[2]:0x00;
				OutCardResult.cbCardCount=SearchHuLu(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//������֧
			if (cbTurnOutType<=CT_TIE_ZHI)
			{
				//������֧
				BYTE cbReferCard=(cbTurnOutType==CT_TIE_ZHI)?cbTurnCardData[1]:0x00;
				OutCardResult.cbCardCount=SearchTieZhi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}

			//����ͬ��˳
			if (cbTurnOutType<=CT_TONG_HUA_SHUN)
			{
				//����ͬ��˳
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA_SHUN)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHuaShun(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					SortCardList( OutCardResult.cbResultCard,OutCardResult.cbCardCount );
					return true;
				}
			}
		}

		return false;
	}
	return false;
}

//�ж��Ƿ����
bool CGameLogic::IsTurnCardMost(WORD wChairId,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,const BYTE (&cbCardData)[GAME_PLAYER][13],
								const BYTE cbCardCount[])
{
	BYTE bTurnCardData[MAX_COUNT];
	CopyMemory( bTurnCardData,cbTurnCardData,sizeof(BYTE)*cbTurnCardCount );
	SortCardList( bTurnCardData,cbTurnCardCount );
	ASSERT( GetCardType(bTurnCardData,cbTurnCardCount)>CT_ERROR );
	if( GetCardType(bTurnCardData,cbTurnCardCount) == CT_ERROR )
		return false;
	ASSERT( wChairId != INVALID_CHAIR );
	if( wChairId == INVALID_CHAIR ) 
		return false;

	//�ж���������Ƿ����ѹ��
	tagOutCardResult OutCardResult;
	for( WORD i = (wChairId+1)%GAME_PLAYER; i != wChairId; i = (i+1)%GAME_PLAYER )
		if( SearchTurnCard(cbCardData[i],cbCardCount[i],bTurnCardData,cbTurnCardCount,
			OutCardResult) )
			break;
	if( i == wChairId ) return true;
	return false;
}

//��������
bool CGameLogic::StrusteeOutCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
							   bool bOutCard3,bool bOutMost,tagOutCardResult &OutCardResult )
{
	//���ý��
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//�����ж�
	if (cbTurnCardCount!=0)
	{
		//��ȡ����
		BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

		//������
		switch (cbTurnOutType)
		{
		case CT_SINGLE:					//��������
			{
				//�����
				if( bOutMost )
				{
					if( CompareCard(cbTurnCardData[0],cbCardData[0]) == false ) return false;
					else
					{
						OutCardResult.cbCardCount = 1;
						OutCardResult.cbResultCard[0] = cbCardData[0];
						return true;
					}
				}
				else
				{
					//����С����
					for( BYTE i = 0; i < cbCardCount; i++ )
					{
						BYTE cbCard = cbCardData[cbCardCount-i-1];
						if( CompareCard(cbTurnCardData[0],cbCard) )
						{
							OutCardResult.cbCardCount = 1;
							OutCardResult.cbResultCard[0] = cbCard;
							return true;
						}
					}
				}
				return false;
			}
		case CT_DOUBLE:					//��������
		case CT_THREE_TIAO:				//��������
		case CT_FOUR_TIAO:				//��������
			{										//����С
				//�����˿�
				tagAnalyseResult AnalyseResult;
				AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

				//��ȡ��ֵ
				BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

				//Ѱ���˿�
				for (BYTE i=cbTurnCardCount-1;i<CountArray(AnalyseResult.cbCardCount);i++)
				{
					for (BYTE j=0;j<AnalyseResult.cbCardCount[i];j++)
					{
						//��С�Ա�
						BYTE cbIndex=(AnalyseResult.cbCardCount[i]-j-1)*(i+1);
						if (CompareCard(cbTurnCardData[0],AnalyseResult.cbCardData[i][cbIndex])==false) continue;

						//���ý��
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][cbIndex],sizeof(BYTE)*cbTurnCardCount);

						return true;
					}
				}

				return false;
			}
		}

		//�������
		if (cbCardCount>=5)
		{
			//�����ֲ�
			tagDistributing Distributing;
			AnalysebDistributing(cbCardData,cbCardCount,Distributing);

			//����˳��
			if (cbTurnOutType<=CT_SHUN_ZI)
			{
				//����˳��
				BYTE cbReferCard=(cbTurnOutType==CT_SHUN_ZI)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchShunZi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}

			//����ͬ��
			if (cbTurnOutType<=CT_TONG_HUA)
			{
				//����ͬ��
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHua(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}

			//������«
			if (cbTurnOutType<=CT_HU_LU)
			{
				//������«
				BYTE cbReferCard=(cbTurnOutType==CT_HU_LU)?cbTurnCardData[2]:0x00;
				OutCardResult.cbCardCount=SearchHuLu(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}

			//������֧
			if (cbTurnOutType<=CT_TIE_ZHI)
			{
				//������֧
				BYTE cbReferCard=(cbTurnOutType==CT_TIE_ZHI)?cbTurnCardData[1]:0x00;
				OutCardResult.cbCardCount=SearchTieZhi(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}

			//����ͬ��˳
			if (cbTurnOutType<=CT_TONG_HUA_SHUN)
			{
				//����ͬ��˳
				BYTE cbReferCard=(cbTurnOutType==CT_TONG_HUA_SHUN)?cbTurnCardData[0]:0x00;
				OutCardResult.cbCardCount=SearchTongHuaShun(Distributing,cbReferCard,OutCardResult.cbResultCard);

				//����ж�
				if (OutCardResult.cbCardCount==cbTurnCardCount) 
				{
					return true;
				}
			}
		}

		return false;
	}
	else
	{
		//���������3
		if( bOutCard3 )
		{
			for( BYTE i = 0; i < cbCardCount; i++ )
			{
				if( cbCardData[cbCardCount-i-1] == 0x03 )
				{
					OutCardResult.cbCardCount = 1;
					OutCardResult.cbResultCard[0] = 0x03;
					//����ͬ��
					while( ++i < cbCardCount )
					{
						if( GetCardLogicValue(cbCardData[cbCardCount-i-1]) == 0x03 )
						{
							BYTE cbIndex = OutCardResult.cbCardCount++;
							OutCardResult.cbResultCard[cbIndex] = cbCardData[cbCardCount-i-1];
						}
						else break;
					}
					return true;
				}
			}
		}

		//��ȡ��ֵ
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[cbCardCount-1]);

		//�����ж�
		BYTE cbSameCount=1;
		for (BYTE i=1;i<cbCardCount;i++)
		{
			if (GetCardLogicValue(cbCardData[cbCardCount-i-1])==cbLogicValue) cbSameCount++;
			else break;
		}

		//��ɴ���
		if (cbSameCount>1)
		{
			OutCardResult.cbCardCount=cbSameCount;
			for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=cbCardData[cbCardCount-1-j];
			return true;
		}

		//���ƴ���
		OutCardResult.cbCardCount=1;
		OutCardResult.cbResultCard[0]=bOutMost?cbCardData[0]:cbCardData[cbCardCount-1];

		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
