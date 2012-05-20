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
	0x4E,0x4F,
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x4E,0x4F,
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
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE &cbStarLevel)
{
	//������ʼ��
	cbStarLevel=0;

	//������
	switch (cbCardCount)
	{
	case 0:	//����
		{
			return CT_ERROR;
		}
	case 1: //����
		{
			return CT_SINGLE;
		}
	}

	//��ը����
	if (cbCardCount>=12)
	{
		//��������
		BYTE cbCardIndex=0;
		BYTE cbBlockCount=0;

		//�˿˷���
		do
		{
			//��������
			BYTE cbSameCount=1;
			BYTE cbCardValue=GetCardLogicValue(cbCardData[cbCardIndex]);

			//ͬ������
			for (BYTE i=cbCardIndex+1;i<cbCardCount;i++)
			{
				if (GetCardLogicValue(cbCardData[i])==cbCardValue)
				{
					cbSameCount++;
				}
				else break;
			}

			//�����ж�
			if (cbSameCount>=4)
			{
				cbBlockCount++;
				cbCardIndex+=cbSameCount;
			}
			else break;

		} while (cbCardIndex<cbCardCount);

		//����ж�
		if ((cbBlockCount>=3)&&(cbCardIndex==cbCardCount)&&(cbCardCount%cbBlockCount==0)&&IsStructureLink(cbCardData,cbCardCount,cbCardCount/cbBlockCount)) 
		{
			cbStarLevel=cbBlockCount+cbCardCount/cbBlockCount;
			return CT_BOMB_LINK;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//ͬ���ж�
	if ((cbCardCount==3)&&(AnalyseResult.cbBlockCount[2]==1)) return CT_THREE; //��ͬ����
	if ((cbCardCount==2)&&(AnalyseResult.cbBlockCount[1]==1)) return CT_DOUBLE;//��������

	//����ը��
	if ((cbCardCount==4)&&(cbCardData[0]==0x4F)&&(cbCardData[3]==0x4E))
	{
		cbStarLevel= 7;
		return CT_BOMB_TW;
	}

	//ͬ��ը��
	if ((cbCardCount>=4)&&(cbCardCount<=8)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) 
	{
		cbStarLevel= cbCardCount;
		return CT_BOMB;
	}

	//��������
	if ((cbCardCount>=6)&&((AnalyseResult.cbBlockCount[1]*2)==cbCardCount))
	{
		BYTE cbDoubleCount=AnalyseResult.cbBlockCount[1]*2;
		if (IsStructureLink(AnalyseResult.cbCardData[1],cbDoubleCount,2)==true) return CT_DOUBLE_LINK;
	}

	//��������
	if ((cbCardCount>=9)&&((AnalyseResult.cbBlockCount[2]*3)==cbCardCount))
	{
		BYTE cbThreeCount=AnalyseResult.cbBlockCount[2]*3;
		if (IsStructureLink(AnalyseResult.cbCardData[2],cbThreeCount,3)==true) return CT_THREE_LINK;
	}

	//˳������
	if ((cbCardCount>=5)&&AnalyseResult.cbBlockCount[0]==cbCardCount)
	{
		if (IsStructureLink(AnalyseResult.cbCardData[0],cbCardCount,1)==true) return CT_SHUNZI;
	}

	return CT_ERROR;
}



//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbSortType==ST_VALUE) 
		{
			cbSortValue[i]=GetCardValue(cbCardData[i]);
		}
		else 
		{
			cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
		}
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

	//��Ŀ����
	if (cbSortType==ST_COUNT)
	{
		//��������
		BYTE cbCardIndex=0;

		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(&cbCardData[cbCardIndex],cbCardCount-cbCardIndex,AnalyseResult);

		//��ȡ�˿�
		for (BYTE i=0;i<CountArray(AnalyseResult.cbBlockCount);i++)
		{
			//�����˿�
			BYTE cbIndex=CountArray(AnalyseResult.cbBlockCount)-i-1;
			CopyMemory(&cbCardData[cbCardIndex],AnalyseResult.cbCardData[cbIndex],AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE));

			//��������
			cbCardIndex+=AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE);
		}
	}

	return;
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
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

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(cbRemoveCount<=cbCardCount);

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
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

	//�����˿�
	if (cbCardValue>=0x0E) return cbCardValue+2;

	//ת����ֵ
	return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//���ñ���
	BYTE cbFirstStarLevel;
	BYTE cbNextStarLevel;
	
	//�����ж�
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount,cbNextStarLevel);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount,cbFirstStarLevel);

	//ը������
	if ((cbFirstType>=CT_BOMB)||(cbNextType>=CT_BOMB))
	{
		//ը������
		if (cbNextType<CT_BOMB) return false;
		if (cbFirstType<CT_BOMB) return true;

		//�Ǽ��Ƚ�
		if (cbFirstStarLevel != cbNextStarLevel)
		{
			return cbNextStarLevel>cbFirstStarLevel;
		}
		else
		{		
			//45��Ƚ�
			if((cbFirstStarLevel<=5)&&(cbFirstStarLevel>=4))
			{
				//��������
				BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
				BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);
				return cbConsultNext>cbConsultFirst;
			}
			//6��Ƚ�
			if(cbFirstStarLevel==6)
			{
				//��������
				BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
				BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);
				return cbConsultNext>cbConsultFirst;

			}
			//7��Ƚ�
			if(cbFirstStarLevel==7)
			{
				
				//�׼�����
				if(cbFirstCount==4)
				{
					return false;
				}
				//�¼�����
				if(cbNextCount==4)
				{
					return true;
				}
				//ͬ��Ƚ�
				if ((cbFirstType==CT_BOMB)&&(cbNextType==CT_BOMB_LINK))
				{
					return true;
				}
				//ͬ��Ƚ�
				if ((cbFirstType==CT_BOMB_LINK)&&(cbNextType==CT_BOMB))
				{
					return false;
				}
				//ͬ��Ƚ�
				if ((cbFirstType==CT_BOMB)&&(cbNextType)==CT_BOMB)
				{
					//��������
					BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
					BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);
					return cbConsultNext>cbConsultFirst;

				}
				ASSERT(FALSE);
				return false;
			}

			//8�Ǽ�
			if(cbFirstStarLevel>=8)
			{
				//ͬ��Ƚ�
				if ((cbFirstType==CT_BOMB)&&(cbNextType==CT_BOMB_LINK))
				{
					return true;
				}
				//ͬ��Ƚ�
				if ((cbFirstType==CT_BOMB_LINK)&&(cbNextType==CT_BOMB))
				{
					return false;
				}
				//ͬ��Ƚ�
				if ((cbFirstType==CT_BOMB)&&(cbNextType)==CT_BOMB)
				{
					//��������
					BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
					BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);
					return cbConsultNext>cbConsultFirst;
				}
				//��ը�Ƚ�
				if((cbFirstType==CT_BOMB_LINK)&&(cbNextType==CT_BOMB_LINK))
				{
					//�ȱ�����
					if(cbFirstCount!=cbNextCount) return cbNextCount>cbFirstCount;

					//��������
					BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
					BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);

					//�˿˷���
					if (cbNextType==CT_BOMB_LINK)
					{
						//���ñ���
						cbConsultNext=0;

						//�˿�����
						for (BYTE i=0;i<cbNextCount;i++)
						{
							BYTE cbCardValue=GetCardValue(cbNextCard[i]);
							if (cbCardValue>cbConsultNext) cbConsultNext=cbCardValue;
						}
					}

					//�˿˷���
					if (cbFirstType==CT_BOMB_LINK)
					{
						//���ñ���
						cbConsultFirst=0;

						//�˿�����
						for (BYTE i=0;i<cbFirstCount;i++)
						{
							BYTE cbCardValue=GetCardValue(cbFirstCard[i]);
							if (cbCardValue>cbConsultFirst) cbConsultFirst=cbCardValue;
						}
					}
					return cbConsultNext>cbConsultFirst;

				}

				ASSERT(FALSE);
				return false;
			}
		}
	}

	//��ͬ����
	if (cbFirstType!=cbNextType) return false;

	//��ͬ����
	switch (cbFirstType)
	{
	case CT_SINGLE:				//��������
	case CT_DOUBLE:				//��������
	case CT_THREE:				//��������
	case CT_SHUNZI:				//˳������	
		{
			//��Ŀ�ж�
			if (cbFirstCount!=cbNextCount) return false;

			//��������
			BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
			BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);

			return cbConsultNext>cbConsultFirst;
		}
	case CT_DOUBLE_LINK:		//��������
	case CT_THREE_LINK:			//��������
		{
			//��Ŀ�ж�
			if (cbFirstCount!=cbNextCount) return false;

			//��������
			BYTE cbConsultNext=GetCardValue(cbNextCard[0]);
			BYTE cbConsultFirst=GetCardValue(cbFirstCard[0]);

			//��ֵת��
			if (cbConsultNext!=2)
			{
				//���ñ���
				cbConsultNext=0;

				//�˿�����
				for (BYTE i=0;i<cbNextCount;i++)
				{
					BYTE cbLogicValue=GetCardLogicValue(cbNextCard[i]);
					if (cbLogicValue>cbConsultNext) cbConsultNext=cbLogicValue;
				}
			}

			//��ֵת��
			if (cbConsultFirst!=2)
			{
				//���ñ���
				cbConsultFirst=0;

				//�˿�����
				for (BYTE i=0;i<cbFirstCount;i++)
				{
					BYTE cbLogicValue=GetCardLogicValue(cbFirstCard[i]);
					if (cbLogicValue>cbConsultFirst) cbConsultFirst=cbLogicValue;
				}
			}

			return cbConsultNext>cbConsultFirst;
		}

	}

	//�������
	ASSERT(FALSE);
	
	return false;
}

//�����˿�
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
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
		BYTE cbIndex=AnalyseResult.cbBlockCount[cbSameCount-1]++;
		for (BYTE j=0;j<cbSameCount;j++)
			AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];

		//��������
		i+=cbSameCount-1;
	}

	return;
}

//�����ֲ�
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
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
	
	//��ȡ����
	BYTE cbStarLevel;
	BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount,cbStarLevel);

	//���Ʒ���
	switch (cbTurnOutType)
	{
	case CT_ERROR:					//��������
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
				return true;
			}

			//���ƴ���
			OutCardResult.cbCardCount=1;
			OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-1];

			return true;
		}

	}


	return false;
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
bool CGameLogic::IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCellCount)
{
	//��Ŀ�ж�
	ASSERT((cbCardCount%cbCellCount)==0);
	if ((cbCardCount%cbCellCount)!=0) return false;
	
	//��������
	BYTE cbBlockCount=cbCardCount/cbCellCount;
	BYTE cbFirstValue=GetCardLogicValue(cbCardData[0]);

	//���ƹ���
	if(cbFirstValue>14) return false;

	//��������
	for (BYTE i=1;i<cbBlockCount;i++)
	{
		if (cbFirstValue!=(GetCardLogicValue(cbCardData[i*cbCellCount])+i)) return false;
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////
