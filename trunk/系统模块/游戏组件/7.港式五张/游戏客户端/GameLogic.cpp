#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardListData[32]=
{
	0x01,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,							//���� A - K
	0x11,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,							//÷�� A - K
	0x21,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,							//���� A - K
	0x31,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,							//���� A - K
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
	//������
	switch (cbCardCount)
	{
	case 1: //����
		{
			return CT_SINGLE;
		}
	case 2:	//����
		{
			return (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1]))?CT_ONE_DOUBLE:CT_SINGLE;
		}
	}

	//��������
	if (cbCardCount==5)
	{
		//��������
		bool cbSameColor=true,bLineCard=true;
		BYTE cbFirstColor=GetCardColor(cbCardData[0]);
		BYTE cbFirstValue=GetCardLogicValue(cbCardData[0]);

		//���η���
		for (BYTE i=1;i<cbCardCount;i++)
		{
			//���ݷ���
			if (GetCardColor(cbCardData[i])!=cbFirstColor) cbSameColor=false;
			if (cbFirstValue!=(GetCardLogicValue(cbCardData[i])+i)) bLineCard=false;

			//�����ж�
			if ((cbSameColor==false)&&(bLineCard==false)) break;
		}

		//˳������
		if ((cbSameColor==false)&&(bLineCard==true)) return CT_SHUN_ZI;
		
		//ͬ������
		if ((cbSameColor==true)&&(bLineCard==false)) return CT_TONG_HUA;

		//ͬ��˳����
		if ((cbSameColor==true)&&(bLineCard==true)) return CT_TONG_HUA_SHUN;
	}

	//�˿˷���
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//�����ж�
	if (AnalyseResult.cbFourCount==1) return CT_TIE_ZHI;
	if (AnalyseResult.cbDoubleCount==2) return CT_TWO_DOUBLE;
	if ((AnalyseResult.cbDoubleCount==1)&&(AnalyseResult.cbThreeCount==1)) return CT_HU_LU;
	if ((AnalyseResult.cbThreeCount==1)&&(AnalyseResult.cbDoubleCount==0)) return CT_THREE_TIAO;
	if ((AnalyseResult.cbDoubleCount==1)&&(AnalyseResult.cbSignedCount==3)) return CT_ONE_DOUBLE;

	return CT_SINGLE;
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
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(CountArray(m_cbCardListData)-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[CountArray(m_cbCardListData)-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
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

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
	BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

	//�����ж�
	if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);

	//������
	switch(cbFirstType)
	{
	case CT_SINGLE:			//����
		{
			//�Ա���ֵ
			for (BYTE i=0;i<cbCardCount;i++)
			{
				BYTE cbNextValue=GetCardLogicValue(cbNextData[i]);
				BYTE cbFirstValue=GetCardLogicValue(cbFirstData[i]);
				if (cbFirstValue!=cbNextValue) return cbFirstValue>cbNextValue;
			}

			//�ԱȻ�ɫ
			return GetCardColor(cbFirstData[0])>GetCardColor(cbNextData[0]);
		}
	case CT_HU_LU:			//��«
	case CT_TIE_ZHI:		//��֧
	case CT_ONE_DOUBLE:		//����
	case CT_TWO_DOUBLE:		//����
	case CT_THREE_TIAO:		//����
		{
			//�����˿�
			tagAnalyseResult AnalyseResultNext;
			tagAnalyseResult AnalyseResultFirst;
			AnalysebCardData(cbNextData,cbCardCount,AnalyseResultNext);
			AnalysebCardData(cbFirstData,cbCardCount,AnalyseResultFirst);

			//������ֵ
			if (AnalyseResultFirst.cbFourCount>0)
			{
				BYTE cbNextValue=AnalyseResultNext.cbFourLogicVolue[0];
				BYTE cbFirstValue=AnalyseResultFirst.cbFourLogicVolue[0];
				return cbFirstValue>cbNextValue;
			}

			//������ֵ
			if (AnalyseResultFirst.cbThreeCount>0)
			{
				BYTE cbNextValue=AnalyseResultNext.cbThreeLogicVolue[0];
				BYTE cbFirstValue=AnalyseResultFirst.cbThreeLogicVolue[0];
				return cbFirstValue>cbNextValue;
			}

			//������ֵ
			for (BYTE i=0;i<AnalyseResultFirst.cbDoubleCount;i++)
			{
				BYTE cbNextValue=AnalyseResultNext.cbDoubleLogicVolue[i];
				BYTE cbFirstValue=AnalyseResultFirst.cbDoubleLogicVolue[i];
				if (cbFirstValue!=cbNextValue) return cbFirstValue>cbNextValue;
			}

			//ɢ����ֵ
			for (BYTE i=0;i<AnalyseResultFirst.cbSignedCount;i++)
			{
				BYTE cbNextValue=AnalyseResultNext.cbSignedLogicVolue[i];
				BYTE cbFirstValue=AnalyseResultFirst.cbSignedLogicVolue[i];
				if (cbFirstValue!=cbNextValue) return cbFirstValue>cbNextValue;
			}

			//���ӻ�ɫ
			if (AnalyseResultFirst.cbDoubleCount>0)
			{
				BYTE cbNextColor=GetCardColor(AnalyseResultNext.cbDoubleCardData[0]);
				BYTE cbFirstColor=GetCardColor(AnalyseResultFirst.cbDoubleCardData[0]);
				return cbFirstColor>cbNextColor;
			}

			//ɢ�ƻ�ɫ
			if (AnalyseResultFirst.cbSignedCount>0)
			{
				BYTE cbNextColor=GetCardColor(AnalyseResultNext.cbSignedCardData[0]);
				BYTE cbFirstColor=GetCardColor(AnalyseResultFirst.cbSignedCardData[0]);
				return cbFirstColor>cbNextColor;
			}

			break;
		}
	case CT_SHUN_ZI:		//˳��
	case CT_TONG_HUA:		//ͬ��
	case CT_TONG_HUA_SHUN:	//ͬ��˳
		{
			//��ֵ�ж�
			BYTE cbNextValue=GetCardLogicValue(cbNextData[0]);
			BYTE cbFirstValue=GetCardLogicValue(cbFirstData[0]);
			if (cbFirstValue!=cbNextValue) return (cbFirstValue>cbNextValue);

			//��ɫ�ж�
			BYTE cbNextColor=GetCardColor(cbNextData[0]);
			BYTE cbFirstColor=GetCardColor(cbFirstData[0]);

			return (cbFirstColor>cbNextColor);
		}
	}

	return false;
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
		BYTE cbSameCount=1;
		BYTE cbSameCardData[4]={cbCardData[i],0,0,0};
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//��ȡͬ��
		for (int j=i+1;j<cbCardCount;j++)
		{
			//�߼��Ա�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//�����˿�
			cbSameCardData[cbSameCount++]=cbCardData[j];
		}

		//������
		switch (cbSameCount)
		{
		case 1:		//����
			{
				AnalyseResult.cbSignedLogicVolue[AnalyseResult.cbSignedCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbSignedCardData[(AnalyseResult.cbSignedCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 2:		//����
			{
				AnalyseResult.cbDoubleLogicVolue[AnalyseResult.cbDoubleCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbDoubleCardData[(AnalyseResult.cbDoubleCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 3:		//����
			{
				AnalyseResult.cbThreeLogicVolue[AnalyseResult.cbThreeCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbThreeCardData[(AnalyseResult.cbThreeCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		case 4:		//����
			{
				AnalyseResult.cbFourLogicVolue[AnalyseResult.cbFourCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbFourCardData[(AnalyseResult.cbFourCount++)*cbSameCount],cbSameCardData,cbSameCount);
				break;
			}
		}

		//���õ���
		i+=cbSameCount-1;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
