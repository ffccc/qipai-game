#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//��������
const BYTE cbIndexCount=5;

//�˿�����
const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
	     0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,			//���� A - K
		 0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,			//÷�� A - K
		 0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,			//���� A - K
	0x31,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
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
	case 2:	//����
		{
			if (GetCardValue(cbCardData[0])==GetCardValue(cbCardData[1])) return CT_DOUBLE;
			return CT_ERROR;
		}
	case 3:	//����
		{
			if (GetCardValue(cbCardData[0])==GetCardValue(cbCardData[2])) return CT_THREE;
			return CT_ERROR;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//ը������
	if ((AnalyseResult.cbBlockCount[3]==1)&&((cbCardCount==4)||(cbCardCount==5))) return CT_BOMB;
	if ((AnalyseResult.cbBlockCount[2]==1)&&(cbCardCount==4)&&((AnalyseResult.cbCardData[2][0]&MASK_VALUE)==1)) return CT_BOMB;
	if ((AnalyseResult.cbBlockCount[2]==1)&&(cbCardCount==4)&&((AnalyseResult.cbCardData[2][0]&MASK_VALUE)==0x0D)) return CT_BOMB;

	//��������
	if ((cbCardCount>=5)&&(AnalyseResult.cbBlockCount[0]==cbCardCount))
	{
		BYTE cbSignedCount=AnalyseResult.cbBlockCount[0];
		if (IsStructureLink(AnalyseResult.cbCardData[0],cbSignedCount,1)==true) return CT_SINGLE_LINK;
	}

	//��������
	if ((cbCardCount>=4)&&((AnalyseResult.cbBlockCount[1]*2)==cbCardCount))
	{
		BYTE cbDoubleCount=AnalyseResult.cbBlockCount[1]*2;
		if (IsStructureLink(AnalyseResult.cbCardData[1],cbDoubleCount,2)==true) return CT_DOUBLE_LINK;
	}

	//��������
	if ((cbCardCount>=6)&&((AnalyseResult.cbBlockCount[2]*3)==cbCardCount))
	{
		BYTE cbThreeCount=AnalyseResult.cbBlockCount[2]*3;
		if (IsStructureLink(AnalyseResult.cbCardData[2],cbThreeCount,3)==true) return CT_THREE_LINK;
	}

	//��������
	if ((cbCardCount==5)&&((AnalyseResult.cbBlockCount[1]*2+AnalyseResult.cbBlockCount[2]*3)==cbCardCount))
	{
		if (AnalyseResult.cbBlockCount[1]!=AnalyseResult.cbBlockCount[2]) return CT_ERROR;
		if (IsStructureLink(AnalyseResult.cbCardData[1],AnalyseResult.cbBlockCount[1]*2,2)==false) return CT_ERROR;
		if (IsStructureLink(AnalyseResult.cbCardData[2],AnalyseResult.cbBlockCount[2]*3,3)==false) return CT_ERROR;
		return CT_THREE_DOUBLE_LINK;
	}

	return CT_ERROR;
}

//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) 
	{
		cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
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

	return (cbFirstValue>cbNextValue);
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//�����ж�
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

	//��ͬ����
	if (cbFirstType!=cbNextType)
	{
		//ը������
		if (cbNextType==CT_BOMB) return false;
		if (cbFirstType==CT_BOMB) return true;
		return false;
	}

	//��ͬ����
	switch (cbFirstType)
	{
	case CT_SINGLE:				//��������
	case CT_DOUBLE:				//��������
	case CT_THREE:				//��������
	case CT_SINGLE_LINK:		//��������
	case CT_DOUBLE_LINK:		//���ö���
	case CT_THREE_LINK:			//��������
		{
			//��Ŀ�ж�
			if (cbFirstCount!=cbNextCount) return false;

			//�˿˶Ա�
			return CompareCard(cbFirstCard[0],cbNextCard[0]);
		}
	case CT_BOMB:				//ը������
		{
			return CompareCard(cbFirstCard[1],cbNextCard[1]);
		}
	case CT_THREE_DOUBLE_LINK:	//��������
		{
			//�����˿�
			tagAnalyseResult AnalyseResultNext;
			tagAnalyseResult AnalyseResultFirst;
			AnalysebCardData(cbNextCard,cbNextCount,AnalyseResultNext);
			AnalysebCardData(cbFirstCard,cbFirstCount,AnalyseResultFirst);

			//�Ա��˿�
			return CompareCard(AnalyseResultFirst.cbCardData[2][0],AnalyseResultNext.cbCardData[2][0]);
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
	SortCardList(cbCardData,cbCardCount);

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

	//���⴦��
	if (cbFirstValue==15)
	{
		for (BYTE i=1;i<cbBlockCount;i++)
		{
			//��ȡ��ֵ
			BYTE cbLogicValue=GetCardLogicValue(cbCardData[i*cbCellCount]);

			//�����ж�
			if ((cbLogicValue==14)&&(i!=1)) return false;
			if ((cbLogicValue!=14)&&((cbLogicValue+i)!=(cbBlockCount+2))) return false;
		}

		return true;
	}

	//��������
	for (BYTE i=1;i<cbBlockCount;i++)
	{
		if (cbFirstValue!=(GetCardLogicValue(cbCardData[i*cbCellCount])+i)) return false;
	}

	return true;
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
		switch (cbSameCount)
		{
		case 1:		//����
			{
				BYTE cbIndex=AnalyseResult.cbBlockCount[0]++;
				AnalyseResult.cbCardData[0][cbIndex*cbSameCount]=cbCardData[i];
				break;
			}
		case 2:		//����
			{
				BYTE cbIndex=AnalyseResult.cbBlockCount[1]++;
				AnalyseResult.cbCardData[1][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[1][cbIndex*cbSameCount+1]=cbCardData[i+1];
				break;
			}
		case 3:		//����
			{
				BYTE cbIndex=AnalyseResult.cbBlockCount[2]++;
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbCardData[2][cbIndex*cbSameCount+2]=cbCardData[i+2];
				break;
			}
		case 4:		//����
			{
				BYTE cbIndex=AnalyseResult.cbBlockCount[3]++;
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbCardData[3][cbIndex*cbSameCount+3]=cbCardData[i+3];
				break;
			}
		}

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

//////////////////////////////////////////////////////////////////////////
