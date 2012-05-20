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
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	//�����ж�
	if (IsTongHuaShun(cbCardData,cbCardCount)) return CT_TONG_HUA_SHUN;
	if (IsTieZhi(cbCardData,cbCardCount)) return CT_TIE_ZHI;
	if (IsHuLu(cbCardData,cbCardCount)) return CT_HU_LU;
	if (IsTongHua(cbCardData,cbCardCount)) return CT_TONG_HUA;
	if (IsShunZhi(cbCardData,cbCardCount)) return CT_SHUN_ZI;
	if (IsSanTiao(cbCardData,cbCardCount)) return CT_THREE_TIAO;
	if (IsDouble(cbCardData,cbCardCount)) return CT_DOUBLE;
	if (cbCardCount==1) return CT_SINGLE;

	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//ת����ֵ
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbLogicValue[i]<cbLogicValue[i+1])||
				((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[sizeof(m_bCardListData)];
	CopyMemory(cbCardData,m_bCardListData,sizeof(m_bCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(cbBufferCount-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[cbBufferCount-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(bRemoveCount<=cbCardCount);

	//�������
	BYTE bDeleteCount=0,bTempCardData[MAX_COUNT];
	if (cbCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
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
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (bTempCardData[i]!=0) cbCardData[bCardPos++]=bTempCardData[i];
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
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//�Ƿ����
bool CGameLogic::IsDouble(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ�ж�
	if (cbCardCount!=2) return false;
	
	//��������
	BYTE bNextValue=GetCardLogicValue(cbCardData[1]);
	BYTE bFirstValue=GetCardLogicValue(cbCardData[0]);

	return (bNextValue==bFirstValue);
}

//�Ƿ���֧
bool CGameLogic::IsTieZhi(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ�ж�
	if (cbCardCount!=5) return false;
	
	//��������
	BYTE bNoFixCount=0;
	BYTE bCenterValue=GetCardLogicValue(cbCardData[1]);

	//��������
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (GetCardLogicValue(cbCardData[i])!=bCenterValue)
		{
			bNoFixCount++;
			if (bNoFixCount>1) return false;
		}
	}

	return (bNoFixCount==1);
}

//�Ƿ��«
bool CGameLogic::IsHuLu(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ�ж�
	if (cbCardCount!=5) return false;

	//��������
	BYTE bMidValue=GetCardLogicValue(cbCardData[2]);
	BYTE bLastValue=GetCardLogicValue(cbCardData[4]);
	BYTE bFirstValue=GetCardLogicValue(cbCardData[0]);
	if (GetCardLogicValue(cbCardData[3])!=bLastValue) return false;
	if (GetCardLogicValue(cbCardData[1])!=bFirstValue) return false;
	if ((bMidValue!=bFirstValue)&&(bMidValue!=bLastValue)) return false;
	
	return true;
}

//�Ƿ�ͬ��
bool CGameLogic::IsTongHua(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ�ж�
	if (cbCardCount!=5) return false;
	
	//��������
	BYTE bFirstColor=GetCardColor(cbCardData[0]);

	//��������
	for (BYTE i=1;i<cbCardCount;i++)
	{
		if (GetCardColor(cbCardData[i])!=bFirstColor) return false;
	}

	return true;
}

//�Ƿ�˳��
bool CGameLogic::IsShunZhi(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ�ж�
	if (cbCardCount!=5) return false;
	
	//��������
	BYTE bFirstValue=GetCardLogicValue(cbCardData[0]);

	//���⴦��
	if (bFirstValue==15)
	{
		for (BYTE i=1;i<cbCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);
			if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
		}

		return true;
	}

	//��������
	for (BYTE i=1;i<cbCardCount;i++)
	{
		if (bFirstValue!=(GetCardLogicValue(cbCardData[i])+i)) return false;
	}

	return true;
}

//�Ƿ�����
bool CGameLogic::IsSanTiao(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ�ж�
	if (cbCardCount!=3) return false;
	
	//��������
	BYTE bFirstValue=GetCardLogicValue(cbCardData[0]);
	for (BYTE i=1;i<cbCardCount;i++)
	{
		if (GetCardLogicValue(cbCardData[i])!=bFirstValue) return false;
	}

	return true;
}

//�Ƿ�ͬ��˳
bool CGameLogic::IsTongHuaShun(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ�ж�
	if (cbCardCount!=5) return false;
	
	//��������
	BYTE bFirstColor=GetCardColor(cbCardData[0]);
	BYTE bFirstValue=GetCardLogicValue(cbCardData[0]);

	//���⴦��
	if (bFirstValue==15)
	{
		for (BYTE i=1;i<cbCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);
			if (GetCardColor(cbCardData[i])!=bFirstColor) return false;
			if ((bFirstValue!=(cbLogicValue+i))&&(bFirstValue!=(cbLogicValue+i+8))) return false;
		}

		return true;
	}

	//��������
	for (BYTE i=1;i<cbCardCount;i++)
	{
		if (GetCardColor(cbCardData[i])!=bFirstColor) return false;
		if (bFirstValue!=(GetCardLogicValue(cbCardData[i])+i)) return false;
	}

	return true;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstCardData, BYTE cbNextCardData)
{
	//��ֵ�ж�
	BYTE cbNextValue=GetCardLogicValue(cbNextCardData);
	BYTE cbFirstValue=GetCardLogicValue(cbFirstCardData);
	if (cbFirstValue!=cbNextValue) return (cbFirstValue>cbNextValue);

	//��ɫ�ж�
	BYTE cbNextColor=GetCardColor(cbNextCardData);
	BYTE cbFirstColor=GetCardColor(cbFirstCardData);

	return (cbFirstColor>cbNextColor);
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//��Ŀ�ж�
	if (cbFirstCount!=cbNextCount) return false;

	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,cbNextCount);
	BYTE bFirstType=GetCardType(bFirstList,cbFirstCount);

	//���͹���
	ASSERT(bFirstType!=CT_ERROR);
	if ((bFirstType==CT_ERROR)||(bNextType==CT_ERROR)) return false;
	
	//������
	switch(bFirstType)
	{
	case CT_SINGLE:			//����
	case CT_DOUBLE:			//����
	case CT_THREE_TIAO:		//����
		{
			if (bNextType!=bFirstType) return false;
			return CompareCard(bFirstList[0],bNextList[0]);
		}
	}

	//�����ж�
	if (bFirstType!=bNextType) return (bFirstType>bNextType);

	//��������
	switch(bFirstType)
	{
	case CT_SHUN_ZI:		//˳��
	case CT_TONG_HUA:		//ͬ��
	case CT_TONG_HUA_SHUN:	//ͬ��˳
		{
			return CompareCard(bFirstList[0],bNextList[0]);
		}
	case CT_HU_LU:			//��«
		{
			return CompareCard(bFirstList[2],bNextList[2]);
		}
	case CT_TIE_ZHI:		//��֧
		{
			return CompareCard(bFirstList[1],bNextList[1]);
		}
	}
	
	return false;
}

//�����ж�
bool CGameLogic::SearchOutCard(BYTE cbCardData[], BYTE cbCardCount, BYTE bTurnCardData[], BYTE cbTurnCardCount, BYTE cbTurnOutType, BYTE cbOutCard[MAX_COUNT])
{
	//��������
	if (cbCardCount<cbTurnCardCount) return false;

	return false;
}

//////////////////////////////////////////////////////////////////////////