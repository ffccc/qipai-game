#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
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
	case 2:	//���ƻ��
		{
			//�����ж�
			if ((cbCardData[0]==0x4F)&&(cbCardData[1]==0x4E)) return CT_MISSILE_CARD;
			if (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1])) return CT_DOUBLE;

			return CT_ERROR;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//�����ж�
	if (AnalyseResult.cbFourCount>0)
	{
		//�����ж�
		if ((AnalyseResult.cbFourCount==1)&&(cbCardCount==4)) return CT_BOMB_CARD;
		//	if ((AnalyseResult.cbFourCount==1)&&(AnalyseResult.cbSignedCount==2)&&(cbCardCount==6)) return CT_FOUR_LINE_TAKE_ONE;
		if ((AnalyseResult.cbFourCount==1)&&(AnalyseResult.cbSignedCount==2)&&(cbCardCount==6)) return CT_FOUR_LINE_TAKE_ONE;
		if ((AnalyseResult.cbFourCount==1)&&(AnalyseResult.cbDoubleCount==2)&&(cbCardCount==8)) return CT_FOUR_LINE_TAKE_TWO;

		return CT_ERROR;
	}

	//�����ж�
	if (AnalyseResult.cbThreeCount>0)
	{
		//��������
		if(AnalyseResult.cbThreeCount==1 && cbCardCount==3) return CT_THREE ;

		//�����ж�
		if (AnalyseResult.cbThreeCount>1)
		{
			//��������
			BYTE cbCardData=AnalyseResult.cbThreeCardData[0];
			BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

			//�������
			if (cbFirstLogicValue>=15) return CT_ERROR;

			//�����ж�
			for (BYTE i=1;i<AnalyseResult.cbThreeCount;i++)
			{
				BYTE cbCardData=AnalyseResult.cbThreeCardData[i*3];
				if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
			}
		}

		//�����ж�
		if (AnalyseResult.cbThreeCount*3==cbCardCount) return CT_THREE_LINE;
		if (AnalyseResult.cbThreeCount*4==cbCardCount) return CT_THREE_LINE_TAKE_ONE;
		if ((AnalyseResult.cbThreeCount*5==cbCardCount)&&(AnalyseResult.cbDoubleCount==AnalyseResult.cbThreeCount)) return CT_THREE_LINE_TAKE_TWO;

		return CT_ERROR;
	}

	//��������
	if (AnalyseResult.cbDoubleCount>=3)
	{
		//��������
		BYTE cbCardData=AnalyseResult.cbDoubleCardData[0];
		BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue>=15) return CT_ERROR;

		//�����ж�
		for (BYTE i=1;i<AnalyseResult.cbDoubleCount;i++)
		{
			BYTE cbCardData=AnalyseResult.cbDoubleCardData[i*2];
			if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}

		//�����ж�
		if ((AnalyseResult.cbDoubleCount*2)==cbCardCount) return CT_DOUBLE_LINE;

		return CT_ERROR;
	}

	//�����ж�
	if ((AnalyseResult.cbSignedCount>=5)&&(AnalyseResult.cbSignedCount==cbCardCount))
	{
		//��������
		BYTE cbCardData=AnalyseResult.cbSignedCardData[0];
		BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue>=15) return CT_ERROR;

		//�����ж�
		for (BYTE i=1;i<AnalyseResult.cbSignedCount;i++)
		{
			BYTE cbCardData=AnalyseResult.cbSignedCardData[i];
			if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}

		return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	//��Ŀ����
	if (cbSortType==ST_COUNT)
	{
		//�����˿�
		BYTE cbIndex=0;
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

		//��������
		CopyMemory(&cbCardData[cbIndex],AnalyseResult.cbFourCardData,sizeof(BYTE)*AnalyseResult.cbFourCount*4);
		cbIndex+=AnalyseResult.cbFourCount*4;

		//��������
		CopyMemory(&cbCardData[cbIndex],AnalyseResult.cbThreeCardData,sizeof(BYTE)*AnalyseResult.cbThreeCount*3);
		cbIndex+=AnalyseResult.cbThreeCount*3;

		//��������
		CopyMemory(&cbCardData[cbIndex],AnalyseResult.cbDoubleCardData,sizeof(BYTE)*AnalyseResult.cbDoubleCount*2);
		cbIndex+=AnalyseResult.cbDoubleCount*2;

		//��������
		CopyMemory(&cbCardData[cbIndex],AnalyseResult.cbSignedCardData,sizeof(BYTE)*AnalyseResult.cbSignedCount);
		cbIndex+=AnalyseResult.cbSignedCount;
	}

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
	if ((cbCardData==0x4E)||(cbCardData==0x4F)) return true;
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
	if (cbCardColor==0x40) return cbCardValue+2;
	return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

	//�����ж�
	if (cbNextType==CT_ERROR) return false;
	if (cbNextType==CT_MISSILE_CARD) return true;

	//ը���ж�
	if ((cbFirstType!=CT_BOMB_CARD)&&(cbNextType==CT_BOMB_CARD)) return true;
	if ((cbFirstType==CT_BOMB_CARD)&&(cbNextType!=CT_BOMB_CARD)) return false;

	//�����ж�
	if ((cbFirstType!=cbNextType)||(cbFirstCount!=cbNextCount)) return false;

	//��ʼ�Ա�
	switch (cbNextType)
	{
	case CT_SINGLE:
	case CT_DOUBLE:
	case CT_THREE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:
	case CT_BOMB_CARD:
		{
			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	case CT_THREE_LINE_TAKE_ONE:
	case CT_THREE_LINE_TAKE_TWO:
		{
			//�����˿�
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(cbNextCard,cbNextCount,NextResult);
			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);

			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbThreeCardData[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbThreeCardData[0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	case CT_FOUR_LINE_TAKE_ONE:
	case CT_FOUR_LINE_TAKE_TWO:
		{
			//�����˿�
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(cbNextCard,cbNextCount,NextResult);
			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);

			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbFourCardData[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbFourCardData[0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	}
	
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

	//��ȡ����
	BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

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
	case CT_SINGLE:					//��������
	case CT_DOUBLE:					//��������
	case CT_THREE:					//��������
		{
			//��ȡ��ֵ
			BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

			//�����˿�
			tagAnalyseResult AnalyseResult;
			AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

			//Ѱ�ҵ���
			if (cbTurnCardCount<=1)
			{
				for (BYTE i=0;i<AnalyseResult.cbSignedCount;i++)
				{
					BYTE cbIndex=AnalyseResult.cbSignedCount-i-1;
					if (GetCardLogicValue(AnalyseResult.cbSignedCardData[cbIndex])>cbLogicValue)
					{
						//���ý��
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbSignedCardData[cbIndex],sizeof(BYTE)*cbTurnCardCount);

						return true;
					}
				}
			}

			//Ѱ�Ҷ���
			if (cbTurnCardCount<=2)
			{
				for (BYTE i=0;i<AnalyseResult.cbDoubleCount;i++)
				{
					BYTE cbIndex=(AnalyseResult.cbDoubleCount-i-1)*2;
					if (GetCardLogicValue(AnalyseResult.cbDoubleCardData[cbIndex])>cbLogicValue)
					{
						//���ý��
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbDoubleCardData[cbIndex],sizeof(BYTE)*cbTurnCardCount);

						return true;
					}
				}
			}

			//Ѱ������
			if (cbTurnCardCount<=3)
			{
				for (BYTE i=0;i<AnalyseResult.cbThreeCount;i++)
				{
					BYTE cbIndex=(AnalyseResult.cbThreeCount-i-1)*3;
					if (GetCardLogicValue(AnalyseResult.cbThreeCardData[cbIndex])>cbLogicValue)
					{
						//���ý��
						OutCardResult.cbCardCount=cbTurnCardCount;
						CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbThreeCardData[cbIndex],sizeof(BYTE)*cbTurnCardCount);

						return true;
					}
				}
			}

			break;
		}
	case CT_SINGLE_LINE:		//��������
		{
			//�����ж�
			if (cbCardCount<cbTurnCardCount) break;

			//��ȡ��ֵ
			BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

			//��������
			for (BYTE i=(cbTurnCardCount-1);i<cbCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE cbHandLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);

				//�����ж�
				if (cbHandLogicValue>=15) break;
				if (cbHandLogicValue<=cbLogicValue) continue;

				//��������
				BYTE cbLineCount=0;
				for (BYTE j=(cbCardCount-i-1);j<cbCardCount;j++)
				{
					if ((GetCardLogicValue(cbCardData[j])+cbLineCount)==cbHandLogicValue) 
					{
						//��������
						OutCardResult.cbResultCard[cbLineCount++]=cbCardData[j];

						//����ж�
						if (cbLineCount==cbTurnCardCount)
						{
							OutCardResult.cbCardCount=cbTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_DOUBLE_LINE:		//��������
		{
			//�����ж�
			if (cbCardCount<cbTurnCardCount) break;

			//��ȡ��ֵ
			BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

			//��������
			for (BYTE i=(cbTurnCardCount-1);i<cbCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE cbHandLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);

				//�����ж�
				if (cbHandLogicValue<=cbLogicValue) continue;
				if ((cbTurnCardCount>1)&&(cbHandLogicValue>=15)) break;

				//��������
				BYTE cbLineCount=0;
				for (BYTE j=(cbCardCount-i-1);j<(cbCardCount-1);j++)
				{
					if (((GetCardLogicValue(cbCardData[j])+cbLineCount)==cbHandLogicValue)
						&&((GetCardLogicValue(cbCardData[j+1])+cbLineCount)==cbHandLogicValue))
					{
						//��������
						OutCardResult.cbResultCard[cbLineCount*2]=cbCardData[j];
						OutCardResult.cbResultCard[(cbLineCount++)*2+1]=cbCardData[j+1];

						//����ж�
						if (cbLineCount*2==cbTurnCardCount)
						{
							OutCardResult.cbCardCount=cbTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_THREE_LINE:				//��������
	case CT_THREE_LINE_TAKE_ONE:	//����һ��
	//case CT_THREE_LINE_TAKE_TWO:	//����һ��
		{
			//�����ж�
			if (cbCardCount<cbTurnCardCount) break;

			//��ȡ��ֵ
			BYTE cbLogicValue=0;
			for (BYTE i=0;i<cbTurnCardCount-2;i++)
			{
				cbLogicValue=GetCardLogicValue(cbTurnCardData[i]);
				if (GetCardLogicValue(cbTurnCardData[i+1])!=cbLogicValue) continue;
				if (GetCardLogicValue(cbTurnCardData[i+2])!=cbLogicValue) continue;
				break;
			}

			//������ֵ
			BYTE cbTurnLineCount=0;
			if (cbTurnOutType==CT_THREE_LINE_TAKE_ONE) cbTurnLineCount=cbTurnCardCount/4;
			else if (cbTurnOutType==CT_THREE_LINE_TAKE_TWO) cbTurnLineCount=cbTurnCardCount/5;
			else cbTurnLineCount=cbTurnCardCount/3;

			//��������
			for (BYTE i=cbTurnLineCount*3-1;i<cbCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE cbHandLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);

				//�����ж�
				if (cbHandLogicValue<=cbLogicValue) continue;
				if ((cbTurnLineCount>1)&&(cbHandLogicValue>=15)) break;

				//��������
				BYTE cbLineCount=0;
				for (BYTE j=(cbCardCount-i-1);j<(cbCardCount-2);j++)
				{
					//���ñ���
					OutCardResult.cbCardCount=0;

					//�����ж�
					if ((GetCardLogicValue(cbCardData[j])+cbLineCount)!=cbHandLogicValue) continue;
					if ((GetCardLogicValue(cbCardData[j+1])+cbLineCount)!=cbHandLogicValue) continue;
					if ((GetCardLogicValue(cbCardData[j+2])+cbLineCount)!=cbHandLogicValue) continue;

					//��������
					OutCardResult.cbResultCard[cbLineCount*3]=cbCardData[j];
					OutCardResult.cbResultCard[cbLineCount*3+1]=cbCardData[j+1];
					OutCardResult.cbResultCard[(cbLineCount++)*3+2]=cbCardData[j+2];

					//����ж�
					if (cbLineCount==cbTurnLineCount)
					{
						//��������
						OutCardResult.cbCardCount=cbLineCount*3;

						//�����˿�
						BYTE cbLeftCardData[MAX_COUNT];
						BYTE cbLeftCount=cbCardCount-OutCardResult.cbCardCount;
						CopyMemory(cbLeftCardData,cbCardData,sizeof(BYTE)*cbCardCount);
						RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,cbLeftCardData,cbCardCount);

						//�����˿�
						tagAnalyseResult AnalyseResultLeft;
						AnalysebCardData(cbLeftCardData,cbLeftCount,AnalyseResultLeft);

						//���ƴ���
						if (cbTurnOutType==CT_THREE_LINE_TAKE_ONE)
						{
							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.cbSignedCount;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;

								//�����˿�
								BYTE cbIndex=AnalyseResultLeft.cbSignedCount-k-1;
								BYTE cbSignedCard=AnalyseResultLeft.cbSignedCardData[cbIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.cbDoubleCount*2;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;

								//�����˿�
								BYTE cbIndex=(AnalyseResultLeft.cbDoubleCount*2-k-1);
								BYTE cbSignedCard=AnalyseResultLeft.cbDoubleCardData[cbIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.cbThreeCount*3;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;

								//�����˿�
								BYTE cbIndex=(AnalyseResultLeft.cbThreeCount*3-k-1);
								BYTE cbSignedCard=AnalyseResultLeft.cbThreeCardData[cbIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.cbFourCount*4;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;

								//�����˿�
								BYTE cbIndex=(AnalyseResultLeft.cbFourCount*4-k-1);
								BYTE cbSignedCard=AnalyseResultLeft.cbFourCardData[cbIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
							}
						}

						//���ƴ���
						if (cbTurnOutType==CT_THREE_LINE_TAKE_TWO)
						{
							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.cbDoubleCount;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;

								//�����˿�
								BYTE cbIndex=(AnalyseResultLeft.cbDoubleCount-k-1)*2;
								BYTE cbCardData1=AnalyseResultLeft.cbDoubleCardData[cbIndex];
								BYTE cbCardData2=AnalyseResultLeft.cbDoubleCardData[cbIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.cbThreeCount;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;

								//�����˿�
								BYTE cbIndex=(AnalyseResultLeft.cbThreeCount-k-1)*3;
								BYTE cbCardData1=AnalyseResultLeft.cbThreeCardData[cbIndex];
								BYTE cbCardData2=AnalyseResultLeft.cbThreeCardData[cbIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
							}

							//��ȡ����
							for (BYTE k=0;k<AnalyseResultLeft.cbFourCount;k++)
							{
								//��ֹ�ж�
								if (OutCardResult.cbCardCount==cbTurnCardCount) break;

								//�����˿�
								BYTE cbIndex=(AnalyseResultLeft.cbFourCount-k-1)*4;
								BYTE cbCardData1=AnalyseResultLeft.cbFourCardData[cbIndex];
								BYTE cbCardData2=AnalyseResultLeft.cbFourCardData[cbIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
							}
						}

						//����ж�
						if (OutCardResult.cbCardCount==cbTurnCardCount) return true;
					}
				}
			}

			break;
		}
	}

	//����ը��
	if ((cbCardCount>=4)&&(cbTurnOutType!=CT_MISSILE_CARD))
	{
		//��������
		BYTE cbLogicValue=0;
		if (cbTurnOutType==CT_BOMB_CARD) cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

		//����ը��
		for (BYTE i=3;i<cbCardCount;i++)
		{
			//��ȡ��ֵ
			BYTE cbHandLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);

			//�����ж�
			if (cbHandLogicValue<=cbLogicValue) continue;

			//ը���ж�
			BYTE cbTempLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);
			for (BYTE j=1;j<4;j++)
			{
				if (GetCardLogicValue(cbCardData[cbCardCount+j-i-1])!=cbTempLogicValue) break;
			}
			if (j!=4) continue;

			//���ý��
			OutCardResult.cbCardCount=4;
			OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-i-1];
			OutCardResult.cbResultCard[1]=cbCardData[cbCardCount-i];
			OutCardResult.cbResultCard[2]=cbCardData[cbCardCount-i+1];
			OutCardResult.cbResultCard[3]=cbCardData[cbCardCount-i+2];

			return true;
		}
	}

	//�������
	if ((cbCardCount>=2)&&(cbCardData[0]==0x4F)&&(cbCardData[1]==0x4E))
	{
		//���ý��
		OutCardResult.cbCardCount=2;
		OutCardResult.cbResultCard[0]=cbCardData[0];
		OutCardResult.cbResultCard[1]=cbCardData[1];

		return true;
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
				BYTE cbIndex=AnalyseResult.cbSignedCount++;
				AnalyseResult.cbSignedCardData[cbIndex*cbSameCount]=cbCardData[i];
				break;
			}
		case 2:		//����
			{
				BYTE cbIndex=AnalyseResult.cbDoubleCount++;
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				break;
			}
		case 3:		//����
			{
				BYTE cbIndex=AnalyseResult.cbThreeCount++;
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				break;
			}
		case 4:		//����
			{
				BYTE cbIndex=AnalyseResult.cbFourCount++;
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				break;
			}
		}

		//��������
		i+=cbSameCount-1;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
