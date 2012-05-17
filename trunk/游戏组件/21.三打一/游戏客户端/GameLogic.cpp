#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_bCardListData[108]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x41,0x42,
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x41,0x42,
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
/*
//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE bCardData[], BYTE bCardCount)
{
	//��ʼ����
	switch (bCardCount)
	{
	case 1: //����
		{
			return CT_SINGLE;
		}
	case 2:	//���ƺͻ��
		{
			return (GetCardLogicValue(bCardData[0])==GetCardLogicValue(bCardData[1]))?CT_DOUBLE:CT_INVALID;
		}
	case 3:
	case 4:	//���ƺ�ը��
		{
			BYTE bLogicValue=GetCardLogicValue(bCardData[0]);
			for (BYTE i=1;i<bCardCount;i++)
			{
				if (bLogicValue!=GetCardLogicValue(bCardData[i])) break;
			}
			if (i==bCardCount) return (bCardCount==3)?CT_THREE:CT_BOMB_CARD;
			if (bCardCount==3) return CT_INVALID;
			break;
		}
	}

	//�����ж�
	if ((bCardCount==4)&&(bCardData[0]==0x42)&&(bCardData[3]==0x41)) return CT_MISSILE_CARD;

	//��������
	if (bCardCount>=4)
	{
		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(bCardData,bCardCount,AnalyseResult);

		//�����ж�
		if (AnalyseResult.bFourCount>0)
		{
			if ((AnalyseResult.bFourCount==1)&&(bCardCount==5)) return CT_FOUR_LINE_TAKE_ONE;
			if ((AnalyseResult.bFourCount==1)&&(AnalyseResult.bDoubleCount==1)&&(bCardCount==6)) return CT_FOUR_LINE_TAKE_ONE;
			if ((AnalyseResult.bFourCount==1)&&(AnalyseResult.bDoubleCount==2)&&(bCardCount==8)) return CT_FOUR_LINE_TAKE_DOUBLE;
			if ((AnalyseResult.bFourCount==1)&&(bCardCount==6)) return CT_FOUR_LINE_TAKE_DOUBLE;

			return CT_INVALID;
		}

		//�����ж�
		if (AnalyseResult.bThreeCount>0)
		{
			//�����ж�
			if (AnalyseResult.bThreeCount>1)
			{
				if (AnalyseResult.bThreeLogicVolue[0]==15) return CT_INVALID;
				for (BYTE i=1;i<AnalyseResult.bThreeCount;i++)
				{
					if (AnalyseResult.bThreeLogicVolue[i]!=(AnalyseResult.bThreeLogicVolue[0]-i)) return CT_INVALID;
				}
			}

			//�����ж�
			if (AnalyseResult.bThreeCount*3==bCardCount) return CT_THREE_LINE;
			if ((AnalyseResult.bDoubleCount==AnalyseResult.bThreeCount)&&(bCardCount==(AnalyseResult.bThreeCount*3+AnalyseResult.bDoubleCount*2))) return CT_THREE_LINE_TAKE_DOUBLE;

			return CT_INVALID;
		}

		//��������
		if (AnalyseResult.bDoubleCount>=3)
		{
			//�����ж�
			if (AnalyseResult.bDoubleLogicVolue[0]!=15)
			{
				for (BYTE i=1;i<AnalyseResult.bDoubleCount;i++)
				{
					if (AnalyseResult.bDoubleLogicVolue[i]!=(AnalyseResult.bDoubleLogicVolue[0]-i)) return CT_INVALID;
				}
				if (AnalyseResult.bDoubleCount*2==bCardCount) return CT_DOUBLE_LINE;
			}

			return CT_INVALID;
		}
		
		//�����ж�
		if ((AnalyseResult.bSignedCount>=5)&&(AnalyseResult.bSignedCount==bCardCount))
		{
			BYTE bLogicValue=GetCardLogicValue(bCardData[0]);
			if (bLogicValue>=15) return CT_INVALID;
			for (BYTE i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-i)) return CT_INVALID;
			}

			return CT_ONE_LINE;
		}
		
		return CT_INVALID;
	}

	return CT_INVALID;
}*/

//�����˿�
//void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
//{
//	//ת����ֵ
//	BYTE bLogicVolue[33];
//	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);
//
//	//�������
//	bool bSorted=true;
//	BYTE bTempData,bLast=bCardCount-1;
//	do
//	{
//		bSorted=true;
//		for (BYTE i=0;i<bLast;i++)
//		{
//			if ((bLogicVolue[i]<bLogicVolue[i+1])||
//			   ((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
//			{
//				//����λ��
//				bTempData=bCardData[i];
//				bCardData[i]=bCardData[i+1];
//				bCardData[i+1]=bTempData;
//				bTempData=bLogicVolue[i];
//				bLogicVolue[i]=bLogicVolue[i+1];
//				bLogicVolue[i+1]=bTempData;
//				bSorted=false;
//			}	
//		}
//		bLast--;
//	} while(bSorted==false);
//
//	return;
//}


//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount, bool bLargeToSmall)
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
			if(bLargeToSmall){
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
			} else {
				if ((bLogicValue[i]>bLogicValue[i+1])||
					((bLogicValue[i]==bLogicValue[i+1])&&(bCardData[i]>bCardData[i+1])))
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
		}
		bLast--;
	} while(bSorted==false);

	return;
}


//�����˿�
void CGameLogic::SortCardByNum(BYTE bCardData[], BYTE bCardCount, bool bMoreToLess)
{

	//�������
	bool bSorted=true;
	BYTE bLast, bCardNum =1;
	tagOneCardNum bTempData;
	//���ư���С����
	SortCardList(bCardData, bCardCount, bMoreToLess);
	//ת����ֵ
	BYTE bLogicValue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	bLogicValue[i]=GetCardLogicValue(bCardData[i]);	
	//ȡ��ÿ���Ƶ������߼�ֵ����Ŀ
	m_atCardNumList.RemoveAll();
	BYTE bCurLogicCardData = bLogicValue[0];
	for(BYTE i =1; i< bCardCount; i++){
		if(bCurLogicCardData != bLogicValue[i]){
			m_atCardNumList.Add(tagOneCardNum(bCurLogicCardData, bCardNum));
			bCurLogicCardData = bLogicValue[i];
			bCardNum =1;
		} else bCardNum ++;
	}
	m_atCardNumList.Add(tagOneCardNum(bCurLogicCardData, bCardNum));

	//��ÿ���Ƶ������߼�ֵ����Ŀ ������Ŀ���߼�ֵ�ķ�ʽ����
	bLast = (BYTE)(m_atCardNumList.GetCount()-1);
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if(bMoreToLess){
				if ((m_atCardNumList[i].m_bCardNum <m_atCardNumList[i+1].m_bCardNum)||
					((m_atCardNumList[i].m_bCardNum==m_atCardNumList[i+1].m_bCardNum)&&(m_atCardNumList[i].m_bCardLogicValue <m_atCardNumList[i+1].m_bCardLogicValue)))
				{
					//����λ��
					bTempData=m_atCardNumList[i];
					m_atCardNumList[i]=m_atCardNumList[i+1];
					m_atCardNumList[i+1]=bTempData;

					bSorted=false;
				}	
			} else {
				if ((m_atCardNumList[i].m_bCardNum >m_atCardNumList[i+1].m_bCardNum)||
					((m_atCardNumList[i].m_bCardNum==m_atCardNumList[i+1].m_bCardNum)&&(m_atCardNumList[i].m_bCardLogicValue >m_atCardNumList[i+1].m_bCardLogicValue)))
				{
					//����λ��
					bTempData=m_atCardNumList[i];
					m_atCardNumList[i]=m_atCardNumList[i+1];
					m_atCardNumList[i+1]=bTempData;

					bSorted=false;
				}	
			}
		}
		bLast--;
	} while(bSorted==false);

	//����ÿ���Ƶ������߼�ֵ����Ŀ ���뵽ԭ��������ȥ
	BYTE bTempCardData[MAX_COUNT], l=0;
	CopyMemory(bTempCardData, bCardData, bCardCount);
	BYTE j,k;
	for(BYTE i =0; i< m_atCardNumList.GetCount(); i++){
		
		for(j=0; j <bCardCount; j++){
			if(m_atCardNumList[i].m_bCardLogicValue == bLogicValue[j]){
				
				break;
			}
		}
		ASSERT(j != bCardCount);
		k = j+m_atCardNumList[i].m_bCardNum;
		for(; j < k; j++){
			bCardData[l++] = bTempCardData[j];
		}
	}

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
	BYTE bDeleteCount=0,bTempCardData[33];
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

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//ת����ֵ
	if (bCardColor==0x40) return bCardValue+15;
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}
/*
//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE bFirstList[], const BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//�����ж�
	if (bFirstType==CT_INVALID) return false;
	if (bFirstType==CT_MISSILE_CARD) return true;

	//ը���ж�
	if ((bFirstType==CT_BOMB_CARD)&&(bNextType!=CT_BOMB_CARD)) return true;
	if ((bFirstType!=CT_BOMB_CARD)&&(bNextType==CT_BOMB_CARD)) return false;

	//�����ж�
	if ((bFirstType!=bNextType)||(bFirstCount!=bNextCount)) return false;

	//��ʼ�Ա�
	switch (bNextType)
	{
	case CT_SINGLE:
	case CT_DOUBLE:
	case CT_THREE:
	case CT_ONE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:
	case CT_BOMB_CARD:
		{
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			return bFirstLogicValue>bNextLogicValue;
		}
	case CT_THREE_LINE_TAKE_DOUBLE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(bNextList,bNextCount,NextResult);
			AnalysebCardData(bFirstList,bFirstCount,FirstResult);
			return FirstResult.bThreeLogicVolue[0]>NextResult.bThreeLogicVolue[0];
		}
	case CT_FOUR_LINE_TAKE_ONE:
	case CT_FOUR_LINE_TAKE_DOUBLE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(bNextList,bNextCount,NextResult);
			AnalysebCardData(bFirstList,bFirstCount,FirstResult);
			return FirstResult.bFourLogicVolue[0]>NextResult.bFourLogicVolue[0];
		}
	}
	
	return false;
}

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
*/

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE bCardData[], BYTE bCardCount)
{
	//������
	switch (bCardCount)
	{
	case 1:		//����
		{
			return CT_SINGLE;
		}
	case 2:		//����
		{
			BYTE bLastLogicValue=GetCardLogicValue(bCardData[1]);
			BYTE bFirstLogicValue=GetCardLogicValue(bCardData[0]);
			return (bFirstLogicValue==bLastLogicValue)?CT_DOUBLE:CT_ERROR;
		}
	case 3:		//��ͷ
		{
			BYTE bLastLogicValue=GetCardLogicValue(bCardData[2]);
			BYTE bFirstLogicValue=GetCardLogicValue(bCardData[0]);
			if (bFirstLogicValue==bLastLogicValue) return CT_THREE_TOU;
			break;
		}
	default:	//ը��
		{
			BYTE bFirstLogicValue=GetCardLogicValue(bCardData[0]);
			BYTE bLastLogicValue=GetCardLogicValue(bCardData[bCardCount-1]);
			if ((bFirstLogicValue==bLastLogicValue)) return CT_BOMB_CARD;
			else if(GetCardColor(bCardData[0]) == GetCardColor(bCardData[bCardCount-1]) && GetCardColor(bCardData[0]) ==0x40) return CT_MISSILE_CARD;
			break;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(bCardData,bCardCount,AnalyseResult);


	//��ū�ж�
	if (AnalyseResult.bThreeCount==1)
	{

		if ((AnalyseResult.bDoubleCount==AnalyseResult.bThreeCount)&&(bCardCount==(AnalyseResult.bThreeCount*3+AnalyseResult.bDoubleCount*2))) return CT_THREE_LINE_TAKE_DOUBLE;

	}


	//�����ж�
	if ((AnalyseResult.bDoubleCount>=3)&&(AnalyseResult.bDoubleCount*2==bCardCount))
	{
		//�߼�����
		BYTE bLogicValue=AnalyseResult.bDoubleLogicVolue[0];
		BYTE bStartID =1;
		if(bLogicValue ==15){
			bLogicValue = GetCardLogicValue(AnalyseResult.bDoubleLogicVolue[1]);
			bStartID =2;
			if(bLogicValue ==14){
				bLogicValue = GetCardLogicValue(AnalyseResult.bDoubleLogicVolue[2]);
				bStartID =3;
			}
		}

		//�����ж�
		if (bLogicValue<15)
		{
			//�����ж�
			bool bSeriesCard=false;
			for (BYTE i=bStartID;i<AnalyseResult.bDoubleCount;i++)
			{
				if (AnalyseResult.bDoubleLogicVolue[i]!=(bLogicValue-(i-bStartID+1))) break;
			}

			//�����ж�
			if (i==AnalyseResult.bDoubleCount) return CT_DOUBLE_LINE;
		}
	}
	//�����ж�
	if ((AnalyseResult.bThreeCount>=3)&&(AnalyseResult.bThreeCount*3==bCardCount))
	{
		//�߼�����
		BYTE bLogicValue=AnalyseResult.bThreeLogicVolue[0];
		BYTE bStartID =1;
		if(bLogicValue ==15){
			bLogicValue = GetCardLogicValue(AnalyseResult.bThreeLogicVolue[1]);
			bStartID =2;
			if(bLogicValue ==14){
				bLogicValue = GetCardLogicValue(AnalyseResult.bThreeLogicVolue[2]);
				bStartID =3;
			}
		}


		//�����ж�
		if (bLogicValue<15)
		{
			//�����ж�
			bool bSeriesCard=false;
			for (BYTE i=bStartID;i<AnalyseResult.bThreeCount;i++)
			{
				if (AnalyseResult.bThreeLogicVolue[i]!=(bLogicValue-(i-bStartID+1))) break;
			}

			//�����ж�
			if (i==AnalyseResult.bThreeCount) return CT_THREE_LINE;
		}
	}


	//�����ж�
	if ((AnalyseResult.bSignedCount>=5)&&(AnalyseResult.bSignedCount==bCardCount))
	{
		//�߼�����
		BYTE bLogicValue=GetCardLogicValue(bCardData[0]);
		BYTE bStartID =1;
		if(bLogicValue ==15){
			bLogicValue = GetCardLogicValue(bCardData[1]);
			bStartID =2;
			if(bLogicValue ==14){
				bLogicValue = GetCardLogicValue(bCardData[2]);
				bStartID =3;
			}
		}

		//�����ж�
		if (bLogicValue<15)
		{
			//�����ж�
			bool bSeriesCard=false;
			for (BYTE i=bStartID;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-(i-bStartID+1))) break;
			}

			//�����ж�
			if (i==AnalyseResult.bSignedCount) return CT_SINGLE_LINE;
		} 
	}

	return CT_ERROR;
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE bFirstList[], const BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//��ȡ���ͣ�1>2 ����true��1<2 ����false
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//�����ж�
	if (bFirstType==CT_ERROR) return false;

	if(bFirstType ==CT_MISSILE_CARD)return true;
	else if(bNextType==CT_MISSILE_CARD)return false;
	//ը���ж�
	if ((bFirstType==CT_BOMB_CARD)&&(bNextType!=CT_BOMB_CARD)) return true;
	if ((bFirstType!=CT_BOMB_CARD)&&(bNextType==CT_BOMB_CARD)) return false;

	//ը���Ա�
	if ((bFirstType==CT_BOMB_CARD)&&(bNextType==CT_BOMB_CARD))
	{
		//����ը��
		if (bFirstCount==4)
		{
			BYTE bLastLogicValue=GetCardLogicValue(bFirstList[3]);
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			if ((bFirstLogicValue==16)&&(bLastLogicValue==16)) return true;
		}

		//����ը��
		if (bNextCount==4)
		{
			BYTE bLastLogicValue=GetCardLogicValue(bNextList[3]);
			BYTE bFirstLogicValue=GetCardLogicValue(bNextList[0]);
			if ((bFirstLogicValue==16)&&(bLastLogicValue==16)) return false;
		}

		//��Ŀ�Ա�
		if (bFirstCount>bNextCount) return true;

		//��ֵ�Ա�
		if (bFirstCount==bNextCount)
		{
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			return (bFirstLogicValue>bNextLogicValue);
		}

		return false;
	}

	//�����ж�
	if ((bFirstType!=bNextType)||(bFirstCount!=bNextCount)) return false;

	//��ʼ�Ա�
	switch (bNextType)
	{
	case CT_SINGLE:
	case CT_DOUBLE:
	case CT_THREE_TOU:
		{
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			return bFirstLogicValue>bNextLogicValue;
		}
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:
		{
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			if(bNextLogicValue == 15 && bFirstLogicValue != 15) return true;
			else if(bFirstLogicValue == 15 && bNextLogicValue != 15) return false;
			else if(bFirstLogicValue ==15 && bNextLogicValue ==15){
				BYTE bFirstLogicValidValue=0, bNextLogicValidValue=0;
				for(int i=1; i<bFirstCount; i++){
					if(GetCardLogicValue(bFirstList[i]) <14){
						bFirstLogicValidValue = GetCardLogicValue(bFirstList[i]);
						break;
					}
				}
				for(int i=1; i<bNextCount; i++){
					if(GetCardLogicValue(bNextList[i]) <14){
						bNextLogicValidValue = GetCardLogicValue(bNextList[i]);
						break;
					}
				}
				return bFirstLogicValidValue >bNextLogicValidValue; 
			}
			return bFirstLogicValue>bNextLogicValue;
		}
	case CT_THREE_LINE_TAKE_DOUBLE:
		{
			BYTE bFirstLogicValue = GetCardLogicValue(bFirstList[0]);
			BYTE bCount =1;
			BYTE bFirstLogicValidValue=0, bNextLogicValidValue=0;
			for(int i=1; i<bFirstCount; i++){
				if(bFirstLogicValue == GetCardLogicValue(bFirstList[i])){
					bCount++;
				} else {
					bCount =1;
					bFirstLogicValue = GetCardLogicValue(bFirstList[i]);
				}
				if(bCount ==3){
					bFirstLogicValidValue = GetCardLogicValue(bFirstList[i]);
					break;
				}
			}
			BYTE bNextLogicValue = GetCardLogicValue(bNextList[0]);
			bCount =1;
			for(int i=1; i<bNextCount; i++){
				if(bNextLogicValue == GetCardLogicValue(bNextList[i])){
					bCount++;
				} else {
					bCount =1;
					bNextLogicValue = GetCardLogicValue(bNextList[i]);
				}
				if(bCount ==3){
					bNextLogicValidValue = GetCardLogicValue(bNextList[i]);
					break;
				}
			}
			ASSERT(bFirstLogicValidValue &&bNextLogicValidValue);
			return bFirstLogicValidValue>bNextLogicValidValue;
		}
	}
	
	return false;
}

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
					AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount++]=bLogicValue;
					break;
				}
			case 3:		//����
				{
					AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount++]=bLogicValue;
					break;
				}
			case 4:		//����
				{
					AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount++]=bLogicValue;
					break;
				}
			}
		}

		//���ñ���
		if ((bCardValueTemp!=bLogicValue)&&(i!=(bCardCount-1)))
		{
			bSameCount=1;
			bLogicValue=bCardValueTemp;
		}
	}

	//������Ŀ
	BYTE bOtherCount=AnalyseResult.bDoubleCount*2+AnalyseResult.bThreeCount*3+AnalyseResult.bFourCount*4;
	AnalyseResult.bSignedCount=bCardCount-bOtherCount;

	return;
}
//////////////////////////////////////////////////////////////////////////
