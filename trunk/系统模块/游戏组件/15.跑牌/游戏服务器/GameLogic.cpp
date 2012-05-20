#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_bCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
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
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount)
{
	//�����ж�

	if (bCardCount==1)						return CT_SINGLE;
	if(IsDouble(bCardData,bCardCount))		return CT_DOUBLE;
	if(IsSanTiao(bCardData,bCardCount))		return CT_THREE_TIAO;
	if(IsFourTiao(bCardData,bCardCount))	return CT_FOUR_TIAO;
	if(IsFourLine(bCardData,bCardCount))	return CT_FOUR_LINE;
	if(IsAllBig(bCardData,bCardCount))		return CT_ALL_BIG;
	if(IsAllSmall(bCardData,bCardCount))	return CT_ALL_SMALL;
	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//ת����ֵ
	BYTE bLogicValue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	bLogicValue[i]=GetCardLogicValue(bCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((bLogicValue[i]<bLogicValue[i+1])||
				((bLogicValue[i]==bLogicValue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//����λ��
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicValue[i];
				bLogicValue[i]=bLogicValue[i+1];
				bLogicValue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//����׼��
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//��������
	ASSERT(bRemoveCount<=bCardCount);

	//�������
	BYTE bDeleteCount=0,bTempCardData[MAX_COUNT];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//�����˿�
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//�����˿�
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//��������
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//��Ч�ж�
	if ((cbColor<=0x30)&&(cbValue>=0x01)&&(cbValue<=0x0D)) return true;

	return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//ת����ֵ
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//�Ƿ����
bool CGameLogic::IsDouble(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=2) return false;
	
	//��������
	BYTE bNextValue=GetCardLogicValue(bCardData[1]);
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);

	return (bNextValue==bFirstValue);
}
//�Ƿ�����
bool CGameLogic::IsSanTiao(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=3) return false;

	//��������
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetCardLogicValue(bCardData[i])!=bFirstValue) return false;
	}

	return true;
}
//�Ƿ�����
bool CGameLogic::IsFourTiao(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=4) return false;

	//��������
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (GetCardLogicValue(bCardData[i])!=bFirstValue) return false;
	}

	return true;
}
//�Ƿ�����
bool CGameLogic::IsFourLine(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=4) return false;

	//��������
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
///////////////////////////////////////////////////////////////
	BYTE value[4];
	value[0]=GetCardValue(bCardData[0]);
	value[1]=GetCardValue(bCardData[1]);
	value[2]=GetCardValue(bCardData[2]);
	value[3]=GetCardValue(bCardData[3]);

	if(value[0]==2)
	{
		if(  value[1]==1 && value[2]==4  &&value[3]==3   ) return true;
		if(  value[1]==5 && value[2]==4  &&value[3]==3   ) return true;
		if(  value[1]==1 && value[2]==13 &&value[3]==12  ) return false;
	}
///////////////////////////////////////////////////////////////
	//���⴦��
	if (bFirstValue==15)
	{
		for (BYTE i=1;i<bCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(bCardData[i]);
			if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+10))) return false;
		}
		return true;
	}
	//if (bFirstValue==14)
	//{
	//	if( (GetCardValue(bCardData[1])==2) && (GetCardValue(bCardData[2])==3) && (GetCardValue(bCardData[3])==4) )
	//		return true;
	//	else
	//		return false;
	//}
	//��������
	for (BYTE i=1;i<bCardCount;i++)
	{
		if (bFirstValue!=(GetCardLogicValue(bCardData[i])+i)) return false;
	}

	return true;
}

//�Ƿ�4��2
bool CGameLogic::IsAllBig(BYTE bCardData[],BYTE bCardCount)
{
	for (BYTE i=0;i<bCardCount;i++) 
	{
		if(GetCardValue(bCardData[i])!=2)
			return false;
	}
	return true;
}
//�Ƿ�ȫС
bool CGameLogic::IsAllSmall(BYTE bCardData[],BYTE bCardCount)
{
	for (BYTE i=0;i<bCardCount;i++) 
	{
		if(GetCardLogicValue(bCardData[i])>10)
			return false;
	}
    return true;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstCardData, BYTE bNextCardData)
{
	//��ֵ�ж�
	BYTE cbNextValue=GetCardLogicValue(bNextCardData);
	BYTE cbFirstValue=GetCardLogicValue(bFirstCardData);
	if (cbFirstValue!=cbNextValue) return (cbFirstValue>cbNextValue);
	if (cbFirstValue==cbNextValue) return false;

	////��ɫ�ж�
	//BYTE cbNextColor=GetCardColor(bNextCardData);
	//BYTE cbFirstColor=GetCardColor(bFirstCardData);

	//return (cbFirstColor>cbNextColor);
	return false;
}
bool CGameLogic::MaxValue(BYTE FirstData[],BYTE NextData[],BYTE Count)
{
	BYTE FirstBig,NextBig;
	for(BYTE i=0;i<Count;i++)
	{
		if(GetCardValue(FirstData[i])>GetCardValue(FirstData[i+1]))
			FirstBig=GetCardValue(FirstData[i]);
		if(GetCardValue(NextData[i])>GetCardValue(NextData[i+1]))
			NextBig=GetCardValue(NextData[i]);
	}
	return (FirstBig>NextBig);
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//��Ŀ�ж�
	if (bFirstCount!=bNextCount) return false;

	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//���͹���
	ASSERT(bFirstType!=CT_ERROR);
	if ((bFirstType==CT_ERROR)||(bNextType==CT_ERROR)) return false;
	
	//������
	switch(bFirstType)
	{
	case CT_SINGLE:			//����
	case CT_DOUBLE:			//����
	case CT_THREE_TIAO:		//����
	case CT_FOUR_TIAO:
		{
			if (bNextType!=bFirstType) return false;
			return CompareCard(bFirstList[0],bNextList[0]);
		}
	case CT_FOUR_LINE:
		{
			if (bNextType!=bFirstType) return false;
			if(GetCardValue(bNextList[0])==2) return MaxValue(bFirstList,bNextList,bFirstCount);
			return CompareCard(bFirstList[0],bNextList[0]);
		}
	}

	////�����ж�
	//if (bFirstType!=bNextType) return (bFirstType>bNextType);

	////��������
	//switch(bFirstType)
	//{
	//case CT_SHUN_ZI:		//˳��
	//case CT_TONG_HUA:		//ͬ��
	//case CT_TONG_HUA_SHUN:	//ͬ��˳
	//	{
	//		return CompareCard(bFirstList[0],bNextList[0]);
	//	}
	//case CT_HU_LU:			//��«
	//	{
	//		return CompareCard(bFirstList[2],bNextList[2]);
	//	}
	//case CT_TIE_ZHI:		//��֧
	//	{
	//		return CompareCard(bFirstList[1],bNextList[1]);
	//	}
	//}
	
	return false;
}

//�����ж�
bool CGameLogic::SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT])
{
	//��������
	if (bCardCount<bTurnCardCount) return false;

	return false;
}

//////////////////////////////////////////////////////////////////////////
//�����˿�
void CGameLogic::AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult)
{
	//��������
	BYTE bSameCount=1,bCardValueTemp=0;
	BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

	//���ý��
	memset(&AnalyseResult,0,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=1;i<bCardCount;i++)
	{
		//��ȡ�˿�
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//������
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 2:		//����
				{
					CopyMemory(&AnalyseResult.m_bDCardData[AnalyseResult.bDoubleCount*2],&bCardData[i-2+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],2);
					AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount++]=bLogicValue;
					break;
				}
			case 3:		//����
				{
					CopyMemory(&AnalyseResult.m_bTCardData[AnalyseResult.bThreeCount*3],&bCardData[i-3+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],3);
					AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount++]=bLogicValue;
					break;
				}
			case 4:		//����
				{
					CopyMemory(&AnalyseResult.m_bFCardData[AnalyseResult.bFourCount*4],&bCardData[i-4+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],4);
					AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount++]=bLogicValue;
					break;
				}
			}
		}

		//���ñ���
		if (bCardValueTemp!=bLogicValue)
		{
			if(bSameCount==1)
			{
				if(i!=bCardCount-1)
					AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i-1];
				else
				{
					AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i-1];
					AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i];
				}
			}
			else
			{
				if(i==bCardCount-1)
					AnalyseResult.m_bSCardData[AnalyseResult.bSignedCount++]=bCardData[i];
			}
			bSameCount=1;
			bLogicValue=bCardValueTemp;

		}
	}

	//������Ŀ
	BYTE bOtherCount=AnalyseResult.bDoubleCount*2+AnalyseResult.bThreeCount*3+AnalyseResult.bFourCount*4;
	return;
}