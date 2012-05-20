#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_bCardListData[54]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x41,0x42
};
const BYTE CGameLogic::m_bBankListData[12]=
{
	0x21,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D
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
	
	if(IsFour(bCardData,bCardCount)) return CT_FOUR;
	if (IsSanTiao(bCardData,bCardCount)) return CT_THREE_TIAO;
	if (IsDouble(bCardData,bCardCount)) return CT_DOUBLE;
	if (bCardCount==1) return CT_SINGLE;

	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//ת����ֵ
	BYTE bLogicValue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	
		bLogicValue[i]=GetCardLogicValue(bCardData[i]);	

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

//�����˿�
void CGameLogic::RandBankCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//����׼��
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bBankListData,sizeof(m_bBankListData));

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
//�����˿�
void CGameLogic::RandCardList(BYTE bDCardData[],BYTE &bDCardCount,BYTE bSCardData[],BYTE &bSCardCount)
{
		//�����˿�
	//����ǰ���
	if(!bDCardCount)return ;
	BYTE bRecuitCardCount=0;
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%bSCardCount;
		bDCardData[bRandCount++]=bSCardData[bPosition];
		bSCardData[bPosition]=bSCardData[bSCardCount-1];
		bSCardCount--;
		bRecuitCardCount++;
	} while (bRandCount<bDCardCount);
		bDCardCount=bRandCount;
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
	if(bCardColor==0x40)return bCardValue+17;
	if(bCardValue==0x01)return bCardValue+13;	
	if(bCardValue==0x03)return bCardValue+12;
	if(bCardValue==0x02)return bCardValue+14;
	if(bCardValue==0x05)return bCardValue+12;
	if(bCardValue==0x07)return bCardValue+13;
	
	return bCardValue;
}
//�ϲ�����
void CGameLogic::CombinationArray(BYTE bFirstCardData[],BYTE bFirstCardCount,BYTE bNextCardData[],BYTE bNextCardCount)
{
	BYTE bStartPosition=bFirstCardCount;
	for(BYTE i=0;i<bNextCardCount;i++)
	{
		bFirstCardData[bStartPosition++]=bNextCardData[i];
	}
}

//�Ƿ����
bool CGameLogic::IsDouble(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount!=2) return false;
	
	//��������
	BYTE bNextValue=GetCardLogicValue(bCardData[1]);
	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
	if(bCardData[0]==0x42&&bCardData[1]==0x41) return true;//˫���Ƕ�
	if(GetCardValue(bCardData[0])==0x0A&&GetCardValue(bCardData[1])==0x0A&&(abs(GetCardColor(bCardData[0])-GetCardColor(bCardData[1]))==0x10||abs(GetCardColor(bCardData[0])-GetCardColor(bCardData[1]))==0x30))
		return true;

	return (bNextValue==bFirstValue);
}

////�Ƿ���֧
//bool CGameLogic::IsTieZhi(BYTE bCardData[], BYTE bCardCount)
//{
//	//��Ŀ�ж�
//	if (bCardCount!=5) return false;
//	
//	//��������
//	BYTE bNoFixCount=0;
//	BYTE bCenterValue=GetCardLogicValue(bCardData[1]);
//
//	//��������
//	for (BYTE i=0;i<bCardCount;i++)
//	{
//		if (GetCardLogicValue(bCardData[i])!=bCenterValue)
//		{
//			bNoFixCount++;
//			if (bNoFixCount>1) return false;
//		}
//	}
//
//	return (bNoFixCount==1);
//}

////�Ƿ��«
//bool CGameLogic::IsHuLu(BYTE bCardData[], BYTE bCardCount)
//{
//	//��Ŀ�ж�
//	if (bCardCount!=5) return false;
//
//	//��������
//	BYTE bMidValue=GetCardLogicValue(bCardData[2]);
//	BYTE bLastValue=GetCardLogicValue(bCardData[4]);
//	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
//	if (GetCardLogicValue(bCardData[3])!=bLastValue) return false;
//	if (GetCardLogicValue(bCardData[1])!=bFirstValue) return false;
//	if ((bMidValue!=bFirstValue)&&(bMidValue!=bLastValue)) return false;
//	
//	return true;
//}

////�Ƿ�ͬ��
//bool CGameLogic::IsTongHua(BYTE bCardData[], BYTE bCardCount)
//{
//	//��Ŀ�ж�
//	if (bCardCount!=5) return false;
//	
//	//��������
//	BYTE bFirstColor=GetCardColor(bCardData[0]);
//
//	//��������
//	for (BYTE i=1;i<bCardCount;i++)
//	{
//		if (GetCardColor(bCardData[i])!=bFirstColor) return false;
//	}
//
//	return true;
//}

//�Ƿ�˳��
bool CGameLogic::IsShunZhi(BYTE bCardData[], BYTE bCardCount)
{
	//��Ŀ�ж�
	if (bCardCount<3) return false;
	for(WORD i=0;i+1<bCardCount;i++)
	{
		if(GetCardLogicValue(bCardData[i])>=15||(GetCardLogicValue(bCardData[i])!=GetCardLogicValue(bCardData[i+1]+1)))return false;
	}
	
	////��������
	//BYTE bFirstValue=GetCardLogicValue(bCardData[0]);

	////���⴦��
	/////////////////////////////////////////////////////////////////
	//BYTE value[5];
	//value[0]=GetCardValue(bCardData[0]);
	//value[1]=GetCardValue(bCardData[1]);
	//value[2]=GetCardValue(bCardData[2]);
	//value[3]=GetCardValue(bCardData[3]);
	//value[4]=GetCardValue(bCardData[4]);

	//if(value[0]==2)
	//{
	//	//if(  value[1]==1 && value[2]==4  && value[3]==3  && value[4]==5 ) return true;
	//	//if(  value[1]==6 && value[2]==5  && value[3]==4  && value[4]==3 ) return true;
	//	if(  value[1]==1 && value[2]==13 && value[3]==12 && value[4]==11) return false;
	//}
	/////////////////////////////////////////////////////////////////
	//if (bFirstValue==15)
	//{
	//	for (BYTE i=1;i<bCardCount;i++)
	//	{
	//		BYTE cbLogicValue=GetCardLogicValue(bCardData[i]);
	//		if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
	//	}

	//	return true;
	//}

	////��������
	//for (BYTE i=1;i<bCardCount;i++)
	//{
	//	if (bFirstValue!=(GetCardLogicValue(bCardData[i])+i)) return false;
	//}

	return true;
}
//�Ƿ�˫��
bool CGameLogic::IsDoubleDragon(BYTE bCardData[],BYTE bCardCount)
{
	if(bCardCount<6)return false;
	for(WORD i=0;i<bCardCount;i++)
	{
		if(GetCardLogicValue(bCardData[i])!=GetCardLogicValue(bCardData[i+1])||GetCardLogicValue(bCardData[i])>=15)
			return false;
		else
		    i++;	
	}
	if(i<2)return false;

	return true ;
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
bool CGameLogic::IsFour(BYTE bCardData[],BYTE bCardCount)
{
	if(bCardCount!=4)return false;
	for(WORD i=0;i+1<bCardCount;i++)
		if(GetCardLogicValue(bCardData[i])!=GetCardLogicValue(bCardData[i+1]))
			return false;

	return true;

}
//�Ƿ�ͬ��˳
//bool CGameLogic::IsTongHuaShun(BYTE bCardData[], BYTE bCardCount)
//{
//	//��Ŀ�ж�
//	if (bCardCount!=5) return false;
//	
//	//��������
//	BYTE bFirstColor=GetCardColor(bCardData[0]);
//	BYTE bFirstValue=GetCardLogicValue(bCardData[0]);
//
//	//���⴦��
//	if (bFirstValue==15)
//	{
//		for (BYTE i=1;i<bCardCount;i++)
//		{
//			BYTE cbLogicValue=GetCardLogicValue(bCardData[i]);
//			if (GetCardColor(bCardData[i])!=bFirstColor) return false;
//			if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
//		}
//
//		return true;
//	}
//
//	//��������
//	for (BYTE i=1;i<bCardCount;i++)
//	{
//		if (GetCardColor(bCardData[i])!=bFirstColor) return false;
//		if (bFirstValue!=(GetCardLogicValue(bCardData[i])+i)) return false;
//	}
//
//	return true;
//}


//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstCardData, BYTE bNextCardData)
{
	//��ֵ�ж�
	BYTE cbNextValue=GetCardLogicValue(bNextCardData);
	BYTE cbFirstValue=GetCardLogicValue(bFirstCardData);
	if (cbFirstValue!=cbNextValue) return (cbFirstValue>cbNextValue);
	else return false;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{

	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//���͹���
	ASSERT(bFirstType!=CT_ERROR);
	if ((bFirstType==CT_ERROR)||(bNextType==CT_ERROR)) return false;
	if(bFirstType!=bNextType)return  false;

	return GetCardLogicValue(bFirstList[0])>=GetCardLogicValue(bNextList[0]);

}

//�����ж�
bool CGameLogic::SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT])
{
	//��������
	if (bCardCount<bTurnCardCount) return false;

	return false;
}

//////////////////////////////////////////////////////////////////////////
