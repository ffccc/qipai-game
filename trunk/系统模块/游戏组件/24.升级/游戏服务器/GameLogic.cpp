#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��������
#define INDEX_OUT					0									//��������
#define INDEX_HAND					1									//��������
#define INDEX_FIRST					2									//�׳�����

//////////////////////////////////////////////////////////////////////////
//��̬����

//������ֵ
const BYTE	CGameLogic::m_cbNTValue=VALUE_ERROR;						//������ֵ

//�˿�����
const BYTE	CGameLogic::m_cbCardData[CELL_PACK]=
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
	//���Ա���
	m_cbPackCount=0;
	m_cbDispatchCount=0;

	//�߼�����
	m_cbMainValue=VALUE_ERROR;
	m_cbMainColor=COLOR_ERROR;

	//��������
	for (BYTE i=0;i<CountArray(m_cbSortRight);i++) m_cbSortRight[i]=i*COLOR_RIGHT;

	return;
}

//��������
CGameLogic::~CGameLogic()
{
}

//���ø���
bool CGameLogic::SetPackCount(BYTE cbPackCount)
{
	//Ч�����
	ASSERT((cbPackCount>0)&&(cbPackCount<=PACK_COUNT));
	if ((cbPackCount==0)||(cbPackCount>PACK_COUNT)) return false;

	//������Ŀ
	m_cbPackCount=cbPackCount;

	//�ɷ���Ŀ
	BYTE cbConcealCount=((m_cbPackCount%2)==0)?8:6;
	m_cbDispatchCount=((m_cbPackCount*CELL_PACK)-cbConcealCount)/4;

	return false;
}

//���ƻ�ɫ
bool CGameLogic::SetMainColor(BYTE cbMainColor)
{
	//Ч�����
	ASSERT((cbMainColor==COLOR_ERROR)||(cbMainColor<=COLOR_NT));
	if ((cbMainColor!=COLOR_ERROR)&&(cbMainColor>COLOR_NT)) return false;

	//���ñ���
	m_cbMainColor=cbMainColor;

	//����Ȩλ
	UpdateSortRight();

	return true;
}

//������ֵ
bool CGameLogic::SetMainValue(BYTE cbMainValue)
{
	//Ч�����
	ASSERT((cbMainValue==VALUE_ERROR)||(cbMainValue<=0x0D));
	if ((cbMainValue!=VALUE_ERROR)&&(cbMainValue>0x0D)) return false;

	//���ñ���
	m_cbMainValue=cbMainValue;

	//����Ȩλ
	UpdateSortRight();

	return true;
}

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//״̬Ч��
	ASSERT(m_cbMainColor!=COLOR_ERROR);
	ASSERT(m_cbMainValue!=VALUE_ERROR);

	//��Ŀ�ж�
	ASSERT((cbCardCount>0)&&(cbCardCount<=MAX_COUNT));
	if ((cbCardCount==0)||(cbCardCount>MAX_COUNT)) return CT_ERROR;

	//ͬ���ж�
	if (GetCardLogicColor(cbCardData,cbCardCount)==COLOR_ERROR) return CT_ERROR;

	//ͬ���ж�
	BYTE cbSameCount=1;
	for (BYTE i=1;i<cbCardCount;i++) 
	{
		if (cbCardData[i]==cbCardData[0]) cbSameCount++;
		else break;
	}
	if (cbSameCount==cbCardCount) return CT_SINGLE+cbSameCount-1;

	//�����ж�
	if ((cbSameCount>=2)&&(cbCardCount%cbSameCount)==0)
	{
		//��������
		BYTE cbBlockCount=cbCardCount/cbSameCount;

		//��Ԫ�ж�
		for (BYTE i=1;i<cbBlockCount;i++)
		{
			//��������
			BYTE cbIndex=i*cbSameCount;
			BYTE cbFirstValue=GetCardValue(cbCardData[0]);

			//��ͬ����
			for (BYTE j=0;j<cbSameCount;j++)
			{
				if (cbCardData[cbIndex+j]!=cbCardData[cbIndex]) break;
			}
			if (j!=cbSameCount) break;

			//��������
			if (IsLineValue(cbCardData[cbIndex-cbSameCount],cbCardData[cbIndex])==false) break;
		}

		//�ɹ��ж�
		if (i==cbBlockCount) return CT_TRACKOR_2+cbSameCount-2;
	}

	return CT_THROW_CARD;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardSortOrder(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE bTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				bTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=bTempData;
				bTempData=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=bTempData;
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
	BYTE cbCardData[CELL_PACK*PACK_COUNT];
	for (BYTE i=0;i<PACK_COUNT;i++) CopyMemory(&cbCardData[CELL_PACK*i],m_cbCardData,sizeof(m_cbCardData));

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

//��ȡ����
WORD CGameLogic::GetCardScore(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	WORD wCardScore=0;

	//�˿��ۼ�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��ȡ��ֵ
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//�ۼƻ���
		if (cbCardValue==5) 
		{
			wCardScore+=5;
			continue;
		}

		//�ۼƻ���
		if ((cbCardValue==10)||(cbCardValue==13)) 
		{
			wCardScore+=10;
			continue;
		}
	}

	return wCardScore;
}

//��ȡ����
BYTE CGameLogic::GetScoreCard(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbScoreCard[], BYTE cbMaxCount)
{
	//��������
	BYTE cbPostion=0;

	//�˿��ۼ�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��ȡ��ֵ
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//�ۼ��˿�
		if ((cbCardValue==5)||(cbCardValue==10)||(cbCardValue==13))
		{
			ASSERT(cbPostion<cbMaxCount);
			cbScoreCard[cbPostion++]=cbCardData[i];
		}
	}

	return cbPostion;
}

//Ч��˦��
bool CGameLogic::EfficacyThrowCard(const BYTE cbOutCardData[], BYTE cbOutCardCount, WORD wOutCardIndex, const BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT], BYTE cbHandCardCount, tagOutCardResult & OutCardResult)
{
	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalyseCardData(cbOutCardData,cbOutCardCount,AnalyseResult);

	//ѭ���Ƚ�
	for (WORD i=0;i<GAME_PLAYER-1;i++)
	{
		//��ȡ����
		WORD wUserIndex=(wOutCardIndex+i+1)%GAME_PLAYER;

		//��ȡ�˿�
		BYTE cbDistillCardHand[MAX_COUNT];
		BYTE cbDistillCountHand=DistillCardByColor(cbHandCardData[wUserIndex],cbHandCardCount,AnalyseResult.cbCardColor,cbDistillCardHand);

		//�жϹ���
		if (cbDistillCountHand==0) continue;

		//�����ж�
		for (BYTE j=PACK_COUNT;j>=2;j--)
		{
			//��������
			tagSameDataInfo * pSameDataInfo=&AnalyseResult.SameDataInfo[j-1];
			tagTractorDataInfo * pTractorDataInfo=&AnalyseResult.TractorDataInfo[j-2];

			//�����ж�
			if (pTractorDataInfo->cbTractorCount>0)
			{
				//��������
				BYTE cbUserTractorIndex=0;

				//��ȡ����
				tagTractorDataInfo TractorDataInfo;
				DistillTrackorByCount(cbDistillCardHand,cbDistillCountHand,j,TractorDataInfo);

				//ѭ���Ա�
				for (BYTE cbUserRound=0;cbUserRound<TractorDataInfo.cbTractorCount;cbUserRound++)
				{
					//��������
					BYTE cbOutTractorIndex=0;
					//��ȡ����
					BYTE cbLineCountUser=TractorDataInfo.cbTractorData[cbUserTractorIndex];
					BYTE cbSameCountUser=TractorDataInfo.cbTractorData[cbUserTractorIndex+1];

					//�Ա�����
					for (BYTE cbOutRound=0;cbOutRound<pTractorDataInfo->cbTractorCount;cbOutRound++)
					{
						//��ȡ����
						BYTE cbLineCountOut=pTractorDataInfo->cbTractorData[cbOutTractorIndex];
						BYTE cbSameCountOut=pTractorDataInfo->cbTractorData[cbOutTractorIndex+1];

						//���ƶԱ�
						if (cbLineCountUser>=cbLineCountOut)
						{
							//�˿˶���
							BYTE cbNextCardData=TractorDataInfo.cbTractorData[cbUserTractorIndex+2];
							BYTE cbFisrtCardData=pTractorDataInfo->cbTractorData[cbOutTractorIndex+2];

							//�Աȴ�С
							if (CompareCardData(cbFisrtCardData,cbNextCardData)==true)
							{
								OutCardResult.cbCardCount=cbLineCountOut*cbSameCountOut;
								CopyMemory(OutCardResult.cbResultCard,&pTractorDataInfo->cbTractorData[cbOutTractorIndex+2],sizeof(BYTE)*OutCardResult.cbCardCount);

								return false;
							}
						}

						//��������
						cbOutTractorIndex+=(2+cbLineCountOut*cbSameCountOut);
					}

					//��������
					cbUserTractorIndex+=(2+cbLineCountUser*cbSameCountUser);
				}
			}

			//ͬ���ж�
			if (pSameDataInfo->cbBlockCount>0)
			{
				//��ȡͬ��
				tagSameDataInfo SameDataInfo;
				DistillCardByCount(cbDistillCardHand,cbDistillCountHand,j,SameDataInfo);

				//ͬ���ж�
				if (SameDataInfo.cbBlockCount>0)
				{
					//�˿˶���
					BYTE cbLessSameIndex=pSameDataInfo->cbCardCount-j;
					BYTE cbFisrtCardData=pSameDataInfo->cbSameData[cbLessSameIndex];

					//�Աȴ�С
					if (CompareCardData(cbFisrtCardData,SameDataInfo.cbSameData[0])==true)
					{
						OutCardResult.cbCardCount=j;
						CopyMemory(OutCardResult.cbResultCard,&pSameDataInfo->cbSameData[cbLessSameIndex],sizeof(BYTE)*OutCardResult.cbCardCount);

						return false;
					}
				}
			}
		}

		//�����ж�
		if (AnalyseResult.SameDataInfo[0].cbCardCount!=0)
		{
			//��������
			BYTE cbCardCount=AnalyseResult.SameDataInfo[0].cbCardCount;
			BYTE cbLessCardData=AnalyseResult.SameDataInfo[0].cbSameData[cbCardCount-1];

			//�Ա��˿�
			if (CompareCardData(cbLessCardData,cbDistillCardHand[0])==true)
			{
				OutCardResult.cbCardCount=1;
				OutCardResult.cbResultCard[0]=cbLessCardData;

				return false;
			}
		}
	}

	return true;
}

//�Ա��˿�
bool CGameLogic::CompareCardData(BYTE cbFirstCardData, BYTE cbNextCardData)
{
	//��ȡ��ɫ
	BYTE cbLogicColorNext=GetCardLogicColor(cbNextCardData);
	BYTE cbLogicColorFirst=GetCardLogicColor(cbFirstCardData);

	//�Աȴ�С
	if ((cbLogicColorNext==COLOR_NT)||(cbLogicColorNext==cbLogicColorFirst))
	{
		//��ȡ��С
		BYTE cbLogicValueNext=GetCardLogicValue(cbNextCardData);
		BYTE cbLogicValueFirst=GetCardLogicValue(cbFirstCardData);

		//��С�ж�
		if (cbLogicValueNext>cbLogicValueFirst) return true;
	}

	return false;
}

//�Ա��˿�
bool CGameLogic::CompareCardResult(const tagAnalyseResult WinnerResult, const tagAnalyseResult UserResult)
{
	//�Ա�����
	for (BYTE i=PACK_COUNT;i>=2;i--)
	{
		//��������
		const tagTractorDataInfo * pTractorDataInfoUser=&UserResult.TractorDataInfo[i-2];
		const tagTractorDataInfo * pTractorDataInfoWinner=&WinnerResult.TractorDataInfo[i-2];

		//�Ա�����
		if ((pTractorDataInfoWinner->cbTractorCount>0)&&(pTractorDataInfoUser->cbTractorCount>0))
		{
			//��ȡ��С
			BYTE cbNextCardData=pTractorDataInfoUser->cbTractorData[2];
			BYTE cbFisrtCardData=pTractorDataInfoWinner->cbTractorData[2];

			//�Աȴ�С
			if (CompareCardData(cbFisrtCardData,cbNextCardData)==true) return true;
		}
	}

	//�Ա�ͬ��
	for (BYTE i=PACK_COUNT;i>=1;i--)
	{
		//��������
		const tagSameDataInfo * pSameDataInfoUser=&UserResult.SameDataInfo[i-1];
		const tagSameDataInfo * pSameDataInfoWinner=&WinnerResult.SameDataInfo[i-1];

		//�Ա�ͬ��
		if ((pSameDataInfoWinner->cbCardCount>0)&&(pSameDataInfoUser->cbCardCount>0))
		{
			//��ȡ��С
			BYTE cbNextCardData=pSameDataInfoUser->cbSameData[0];
			BYTE cbFisrtCardData=pSameDataInfoWinner->cbSameData[0];

			//�Աȴ�С
			if (CompareCardData(cbFisrtCardData,cbNextCardData)==true) return true;
		}
	}

	return false;
}

//�Ա��˿�
WORD CGameLogic::CompareCardArray(const BYTE cbOutCardData[GAME_PLAYER][MAX_COUNT], BYTE cbCardCount, WORD wFirstIndex)
{
	//��������
	WORD wWinnerIndex=wFirstIndex;
	BYTE cbCardType=GetCardType(cbOutCardData[wFirstIndex],cbCardCount);

	//�Ա��˿�
	switch (cbCardType)
	{
	case CT_SINGLE:		//��������
	case CT_SAME_2:		//��������
	case CT_SAME_3:		//��������
	case CT_SAME_4:		//��������
	case CT_TRACKOR_2:	//��������
	case CT_TRACKOR_3:	//��������
	case CT_TRACKOR_4:	//��������
		{
			//ѭ���Ƚ�
			for (WORD i=0;i<GAME_PLAYER-1;i++)
			{
				//�û�����
				WORD wUserIndex=(wFirstIndex+i+1)%GAME_PLAYER;

				//�����ж�
				if (GetCardType(cbOutCardData[wUserIndex],cbCardCount)!=cbCardType) continue;

				//�Աȴ�С
				if (CompareCardData(cbOutCardData[wWinnerIndex][0],cbOutCardData[wUserIndex][0])==true) 
				{
					wWinnerIndex=wUserIndex;
					continue;
				}
			}

			return wWinnerIndex;
		}
	case CT_THROW_CARD:	//˦������
		{
			//��ɫ�ж�
			if (GetCardLogicColor(cbOutCardData[wWinnerIndex][0])==COLOR_NT) return wFirstIndex;

			//��������
			tagAnalyseResult WinnerResult;
			AnalyseCardData(cbOutCardData[wFirstIndex],cbCardCount,WinnerResult);

			//ѭ���Ƚ�
			for (WORD i=0;i<GAME_PLAYER-1;i++)
			{
				//�����ж�
				WORD wUserIndex=(wFirstIndex+i+1)%GAME_PLAYER;
				if (GetCardLogicColor(cbOutCardData[wUserIndex],cbCardCount)!=COLOR_NT) continue;

				//��������
				tagAnalyseResult UserResult;
				if (RectifyCardWeave(cbOutCardData[wUserIndex],cbCardCount,WinnerResult,UserResult)==false) continue;

				//�ԱȽ��
				if (CompareCardResult(WinnerResult,UserResult)==true)
				{
					wWinnerIndex=wUserIndex;
					CopyMemory(&WinnerResult,&UserResult,sizeof(WinnerResult));
				}
			}

			return wWinnerIndex;
		}
	}

	//�������
	ASSERT(FALSE);

	return wFirstIndex;
}

//�����˿�
bool CGameLogic::AnalyseCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//��ɫ�ж�
	AnalyseResult.cbCardColor=GetCardLogicColor(cbCardData,cbCardCount);
	if (AnalyseResult.cbCardColor==COLOR_ERROR) return false;

	//��������
	BYTE cbAnalyseCard[MAX_COUNT];
	BYTE cbAnalyseCount=cbCardCount;
	CopyMemory(cbAnalyseCard,cbCardData,cbCardCount*sizeof(BYTE));

	//��ȡ�˿�
	for (BYTE i=PACK_COUNT;i>=2;i--)
	{
		//��ȡ����
		tagTractorDataInfo * pTractorDataInfo=&AnalyseResult.TractorDataInfo[i-2];
		DistillTrackorByCount(cbAnalyseCard,cbAnalyseCount,i,AnalyseResult.TractorDataInfo[i-2]);

		//���ƴ���
		if (pTractorDataInfo->cbTractorCount>0)
		{
			BYTE cbIndex=0,cbRemoveCount=0;
			for (BYTE j=0;j<pTractorDataInfo->cbTractorCount;j++)
			{
				//��������
				cbRemoveCount=pTractorDataInfo->cbTractorData[cbIndex]*i;

				//ɾ���˿�
				cbAnalyseCount-=cbRemoveCount;
				RemoveCard(&pTractorDataInfo->cbTractorData[cbIndex+2],cbRemoveCount,cbAnalyseCard,cbAnalyseCount+cbRemoveCount);

				//���ñ���
				cbIndex+=cbRemoveCount+2;
			}
		}

		//��ȡͬ��
		tagSameDataInfo * pSameDataInfo=&AnalyseResult.SameDataInfo[i-1];
		DistillCardByCount(cbAnalyseCard,cbAnalyseCount,i,AnalyseResult.SameDataInfo[i-1]);

		//ɾ���˿�
		if (pSameDataInfo->cbBlockCount>0)
		{
			cbAnalyseCount-=pSameDataInfo->cbCardCount;
			RemoveCard(pSameDataInfo->cbSameData,pSameDataInfo->cbCardCount,cbAnalyseCard,cbAnalyseCount+pSameDataInfo->cbCardCount);
		}
	}

	//��ȡ����
	AnalyseResult.SameDataInfo[0].cbCardCount=cbAnalyseCount;
	AnalyseResult.SameDataInfo[0].cbBlockCount=cbAnalyseCount;
	CopyMemory(AnalyseResult.SameDataInfo[0].cbSameData,cbAnalyseCard,cbAnalyseCount*sizeof(BYTE));

	return true;
}

//�����ж�
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount,const  BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
	//���ñ���
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//�׳��û�
	if (cbTurnCardCount==0)
	{
		for (BYTE i=PACK_COUNT;i>=2;i--)
		{
			//��ȡ����
			tagTractorDataInfo TractorDataInfo;
			DistillTrackorByCount(cbHandCardData,cbHandCardCount,i,TractorDataInfo);

			//�����ж�
			if (TractorDataInfo.cbTractorCount>0)
			{
				//��ȡ����
				BYTE cbLineCount=TractorDataInfo.cbTractorData[0];
				BYTE cbSameCount=TractorDataInfo.cbTractorData[1];
				
				//������
				OutCardResult.cbCardCount=cbLineCount*cbSameCount;
				CopyMemory(OutCardResult.cbResultCard,&TractorDataInfo.cbTractorData[2],sizeof(BYTE)*OutCardResult.cbCardCount);

				return true;
			}

			//��ȡͬ��
			tagSameDataInfo SameDataInfo;
			DistillCardByCount(cbHandCardData,cbHandCardCount,i,SameDataInfo);

			//ͬ���ж�
			if (SameDataInfo.cbCardCount>0)
			{
				//������
				OutCardResult.cbCardCount=i;
				CopyMemory(OutCardResult.cbResultCard,&SameDataInfo.cbSameData[0],sizeof(BYTE)*i);

				return true;
			}
		}

		//��ȡ����
		OutCardResult.cbCardCount=1;
		OutCardResult.cbResultCard[0]=cbHandCardData[0];

		return true;
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalyseCardData(cbTurnCardData,cbTurnCardCount,AnalyseResult);

	//��ȡ�˿�
	BYTE cbAnalyseCard[MAX_COUNT];
	BYTE cbAnalyseCardCount=DistillCardByColor(cbHandCardData,cbHandCardCount,GetCardLogicColor(cbTurnCardData[0]),cbAnalyseCard);

	//��Ŀ�ж�
	if (cbTurnCardCount>=cbAnalyseCardCount)
	{
		//�����˿�
		OutCardResult.cbCardCount=cbAnalyseCardCount;
		CopyMemory(OutCardResult.cbResultCard,cbAnalyseCard,sizeof(BYTE)*cbAnalyseCardCount);

		//ʣ���˿�
		if (cbTurnCardCount>cbAnalyseCardCount)
		{
			//��������
			BYTE cbSourceCount=OutCardResult.cbCardCount;
			BYTE cbLeaveCount=(cbTurnCardCount-cbAnalyseCardCount);

			//�����˿�
			BYTE cbCardDataTemp[MAX_COUNT];
			BYTE cbCardCountTemp=cbHandCardCount;
			CopyMemory(cbCardDataTemp,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

			//ɾ���˿�
			cbCardCountTemp-=cbAnalyseCardCount;
			RemoveCard(cbAnalyseCard,cbAnalyseCardCount,cbCardDataTemp,cbCardCountTemp+cbAnalyseCardCount);

			//������
			OutCardResult.cbCardCount+=cbLeaveCount;
			CopyMemory(&OutCardResult.cbResultCard[cbSourceCount],&cbCardDataTemp[cbCardCountTemp-cbLeaveCount],sizeof(BYTE)*cbLeaveCount);
		}

		return true;
	}

	//�������
	tagDemandInfo DemandInfo;
	ZeroMemory(&DemandInfo,sizeof(DemandInfo));

	//�������
	for (BYTE i=PACK_COUNT;i>=2;i--)
	{
		//��������
		tagSameDataInfo * pSameDataInfo=&AnalyseResult.SameDataInfo[i-1];
		tagTractorDataInfo * pTractorDataInfo=&AnalyseResult.TractorDataInfo[i-2];

		//��������
		if (pTractorDataInfo->cbTractorCount>0)
		{
			DemandInfo.cbTractorCardCount[i-2]=pTractorDataInfo->cbCardCount;
			DemandInfo.cbMaxTractor[i-2]=pTractorDataInfo->cbTractorMaxLength;
		}

		//ͬ������
		if (pSameDataInfo->cbBlockCount>0) DemandInfo.cbSameCardCount[i-1]=pSameDataInfo->cbCardCount;
	}

	//��������
	if (AnalyseResult.SameDataInfo[0].cbCardCount>0) DemandInfo.cbSameCardCount[0]=AnalyseResult.SameDataInfo[0].cbCardCount;

	//��������
	for (BYTE i=PACK_COUNT;i>=2;i--)
	{
		//�����ж�
		if (DemandInfo.cbTractorCardCount[i-2]>0)
		{
			//��ȡ����
			tagTractorDataInfo TractorDataInfo;
			DistillTrackorByCount(cbAnalyseCard,cbAnalyseCardCount,i,TractorDataInfo);

			//��������
			BYTE cbIndex=0;
			BYTE cbLeaveCount=DemandInfo.cbTractorCardCount[i-2];

			//��ȡ�˿�
			for (BYTE j=0;j<TractorDataInfo.cbTractorCount;j++)
			{
				//��������
				if (j==0) cbIndex=TractorDataInfo.cbTractorMaxIndex;
				if ((j==1)&&(TractorDataInfo.cbTractorMaxIndex!=0)) cbIndex=0;

				//��������
				if ((j!=0)&&(cbIndex==TractorDataInfo.cbTractorMaxIndex))
				{
					//��ȡ����
					BYTE cbLineCount=TractorDataInfo.cbTractorData[cbIndex];
					BYTE cbSameCount=TractorDataInfo.cbTractorData[cbIndex+1];

					//��������
					cbIndex+=cbLineCount*cbSameCount+2;
				}

				//��ȡ����
				BYTE cbLineCount=TractorDataInfo.cbTractorData[cbIndex];
				BYTE cbSameCount=TractorDataInfo.cbTractorData[cbIndex+1];

				//��ȡ����
				BYTE cbTractorCardCount=cbLineCount*cbSameCount;
				BYTE cbTakeCardCount=__min(cbTractorCardCount,cbLeaveCount);

				//��ȡ�˿�
				OutCardResult.cbCardCount+=cbTakeCardCount;
				CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount-cbTakeCardCount],
					&TractorDataInfo.cbTractorData[cbIndex+2+cbTractorCardCount-cbTakeCardCount],sizeof(BYTE)*cbTakeCardCount);

				//ɾ���˿�
				cbLeaveCount-=cbTakeCardCount;
				cbAnalyseCardCount-=cbTakeCardCount;
				RemoveCard(&TractorDataInfo.cbTractorData[cbIndex+2+cbTractorCardCount-cbTakeCardCount],cbTakeCardCount,
					cbAnalyseCard,cbAnalyseCardCount+cbTakeCardCount);

				//��������
				cbIndex+=cbLineCount*cbSameCount+2;
			}

			//��������
			DemandInfo.cbSameCardCount[i-1]+=cbLeaveCount;
		}

		//ͬ���ж�
		if (DemandInfo.cbSameCardCount[i-1]>0)
		{
			//��ȡͬ��
			tagSameDataInfo SameDataInfo;
			DistillCardByCount(cbAnalyseCard,cbAnalyseCardCount,i,SameDataInfo);

			//��ȡ����
			BYTE cbTakeCardCount=__min(SameDataInfo.cbCardCount,DemandInfo.cbSameCardCount[i-1]);
			cbTakeCardCount=cbTakeCardCount-cbTakeCardCount%i;

			//������
			OutCardResult.cbCardCount+=cbTakeCardCount;
			CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount-cbTakeCardCount],
				&SameDataInfo.cbSameData[SameDataInfo.cbCardCount-cbTakeCardCount],sizeof(BYTE)*cbTakeCardCount);

			//ɾ���˿�
			cbAnalyseCardCount-=cbTakeCardCount;
			RemoveCard(&SameDataInfo.cbSameData[SameDataInfo.cbCardCount-cbTakeCardCount],cbTakeCardCount,cbAnalyseCard,cbAnalyseCardCount+cbTakeCardCount);

			//��������
			DemandInfo.cbSameCardCount[i-2]+=(DemandInfo.cbSameCardCount[i-1]-cbTakeCardCount);
		}
	}

	//��ȡ����
	if (DemandInfo.cbSameCardCount[0]>0)
	{
		//��������
		BYTE cbSourceCount=OutCardResult.cbCardCount;
		BYTE cbLeaveCount=DemandInfo.cbSameCardCount[0];

		//������
		OutCardResult.cbCardCount+=cbLeaveCount;
		CopyMemory(&OutCardResult.cbResultCard[cbSourceCount],&cbAnalyseCard[cbAnalyseCardCount-cbLeaveCount],sizeof(BYTE)*cbLeaveCount);
	}

	return true;
}

//Ч�����
bool CGameLogic::EfficacyOutCard(const BYTE cbOutCardData[], BYTE cbOutCardCount, const BYTE cbFirstCardData[], BYTE cbFirstCardCount, const BYTE cbHandCardData[], BYTE cbHandCardCount)
{
	//״̬Ч��
	ASSERT(m_cbMainColor!=COLOR_ERROR);
	ASSERT(m_cbMainValue!=VALUE_ERROR);

	//��Ŀ�ж�
	if (cbOutCardCount!=cbFirstCardCount) return false;

	//�˿�����
	BYTE cbOutCardType=GetCardType(cbOutCardData,cbOutCardCount);
	BYTE cbFirstCardType=GetCardType(cbFirstCardData,cbFirstCardCount);

	//�߼���ɫ
	BYTE cbOutLogicColor=GetCardLogicColor(cbOutCardData,cbOutCardCount);
	BYTE cbFirstLogicColor=GetCardLogicColor(cbFirstCardData,cbFirstCardCount);

	//ͬ���ж�
	ASSERT(cbFirstCardType!=CT_ERROR);
	if ((cbFirstCardType!=CT_THROW_CARD)&&(cbOutLogicColor==cbFirstLogicColor)&&(cbOutCardType==cbFirstCardType)) return true;

	//��ȡ�˿�
	BYTE cbDistillCardOut[MAX_COUNT];
	BYTE cbDistillCardHand[MAX_COUNT];
	BYTE cbDistillCountOut=DistillCardByColor(cbOutCardData,cbOutCardCount,cbFirstLogicColor,cbDistillCardOut);
	BYTE cbDistillCountHand=DistillCardByColor(cbHandCardData,cbHandCardCount,cbFirstLogicColor,cbDistillCardHand);

	//��ɫ�ж�
	if (cbFirstLogicColor!=cbOutLogicColor) return (cbDistillCountOut==cbDistillCountHand);

	//��������
	tagSameDataInfo SameDataInfo[3];
	tagTractorDataInfo TractorDataInfo[3];

	//�˿˷���
	for (BYTE i=PACK_COUNT;i>=2;i--)
	{
		//��ȡ����
		DistillTrackorByCount(cbFirstCardData,cbFirstCardCount,i,TractorDataInfo[INDEX_FIRST]);

		//�����ж�
		if (TractorDataInfo[INDEX_FIRST].cbTractorCount>0)
		{
			//��ȡ����
			DistillTrackorByCount(cbDistillCardOut,cbDistillCountOut,i,TractorDataInfo[INDEX_OUT]);
			DistillTrackorByCount(cbDistillCardHand,cbDistillCountHand,i,TractorDataInfo[INDEX_HAND]);

			//�����ж�
			if (TractorDataInfo[INDEX_HAND].cbCardCount>TractorDataInfo[INDEX_OUT].cbCardCount)
			{
				//��Ŀ�ж�
				//if (TractorDataInfo[INDEX_FIRST].cbCardCount==(TractorDataInfo[INDEX_OUT].cbCardCount+i))
				//{
				//	//��ȡͬ��
				//	tagSameDataInfo SameDataInfoOut;
				//	DistillCardByCount(cbDistillCardOut,cbDistillCountOut,i,SameDataInfoOut);

				//	//��ȡ����
				//	BYTE cbHandTractorData[MAX_COUNT];
				//	BYTE cbHandTractorCardCount=0,cbIndex=0;
				//	for (BYTE j=0;j<TractorDataInfo[INDEX_HAND].cbTractorCount;j++)
				//	{
				//		//��ȡ����
				//		BYTE cbLineCount=TractorDataInfo[INDEX_HAND].cbTractorData[cbIndex];
				//		BYTE cbSameCount=TractorDataInfo[INDEX_HAND].cbTractorData[cbIndex+1];

				//		//�����˿�
				//		BYTE cbSourceCount=cbHandTractorCardCount;
				//		cbHandTractorCardCount+=cbLineCount*cbSameCount;
				//		CopyMemory(&cbHandTractorData[cbSourceCount],&TractorDataInfo[INDEX_HAND].cbTractorData[cbIndex+2],cbLineCount*cbSameCount*sizeof(BYTE));

				//		//��������
				//		cbIndex+=(2+cbLineCount*cbSameCount);
				//	}

				//	//�����ж�
				//	BYTE cbIntersectionCount=GetIntersectionCount(SameDataInfoOut.cbSameData,SameDataInfoOut.cbCardCount,cbHandTractorData,cbHandTractorCardCount);
				//	if (cbIntersectionCount<TractorDataInfo[INDEX_FIRST].cbCardCount) return false;
				//}

				//��Ŀ�ж�
				if (TractorDataInfo[INDEX_FIRST].cbCardCount>(TractorDataInfo[INDEX_OUT].cbCardCount+i)) 
					return false;
				else if( TractorDataInfo[INDEX_FIRST].cbCardCount == TractorDataInfo[INDEX_OUT].cbCardCount+i &&
					TractorDataInfo[INDEX_HAND].cbTractorMaxLength*i >= TractorDataInfo[INDEX_FIRST].cbCardCount )
					return false;
			}
		}

		//��ȡͬ��
		DistillCardByCount(cbFirstCardData,cbFirstCardCount,i,SameDataInfo[INDEX_FIRST]);

		//ͬ���ж�
		if (SameDataInfo[INDEX_FIRST].cbBlockCount>0)
		{
			//��ȡͬ��
			DistillCardByCount(cbDistillCardOut,cbDistillCountOut,i,SameDataInfo[INDEX_OUT]);
			DistillCardByCount(cbDistillCardHand,cbDistillCountHand,i,SameDataInfo[INDEX_HAND]);

			//��Ŀ�ж�
			BYTE cbBlockCountOut=SameDataInfo[INDEX_OUT].cbBlockCount;
			BYTE cbBlockCountHand=SameDataInfo[INDEX_HAND].cbBlockCount;
			BYTE cbBlockCountFirst=SameDataInfo[INDEX_FIRST].cbBlockCount;
			if ((cbBlockCountFirst>cbBlockCountOut)&&(cbBlockCountHand>cbBlockCountOut)) return false;
		}
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

//����ȼ�
BYTE CGameLogic::GetCardSortOrder(BYTE cbCardData)
{
	//�߼���ֵ
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//���ƹ���
	if ((cbCardData==0x4E)||(cbCardData==0x4F))
	{
		BYTE cbSortValue=cbCardValue+11;
		return cbSortValue+m_cbSortRight[4];
	}

	//��������
	if (cbCardValue==m_cbNTValue)
	{
		BYTE cbSortValue=(cbCardColor>>4)+15;
		if (cbCardColor==m_cbMainColor) cbSortValue=19;
		return cbSortValue+m_cbSortRight[4];
	}

	//��ɫ�任
	BYTE cbSortColor=cbCardColor;
	if ((cbCardColor==m_cbMainColor)||(cbCardValue==m_cbMainValue)) cbSortColor=0x40;

	//��ֵ�任
	BYTE cbSortValue=cbCardValue;
	if (cbCardValue==1) cbSortValue+=13;

	//���Ʊ任
	if (cbCardValue==m_cbMainValue)
	{
		if (cbCardColor==m_cbMainColor) cbSortValue=24;
		else cbSortValue=(cbCardColor>>4)+20;
	}

	return cbSortValue+m_cbSortRight[cbSortColor>>4];
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�߼���ֵ
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);
	BYTE cbNTRight=((m_cbNTValue==VALUE_ERROR)||(m_cbNTValue==m_cbMainValue))?0:2;

	//���ƹ���
	if ((cbCardData==0x4E)||(cbCardData==0x4F))
	{
		BYTE cbLogicValue=cbCardValue+3+cbNTRight;
		BYTE cbDecreaseOrder=(m_cbMainColor==COLOR_NT)?1:0;
		return cbLogicValue+m_cbSortRight[4]-cbDecreaseOrder;
	}

	//��������
	if (cbCardValue==m_cbNTValue)
	{
		BYTE cbLogicValue=(cbCardColor==m_cbMainColor)?16:15;
		return cbLogicValue+m_cbSortRight[4];
	}

	//��ɫ�任
	BYTE cbLogicColor=cbCardColor;
	if ((cbCardColor==m_cbMainColor)||(cbCardValue==m_cbMainValue)) cbLogicColor=0x40;

	//ת���任
	BYTE cbLogicValue=cbCardValue;
	if (cbCardValue==1) cbLogicValue+=13;

	//���Ʊ任
	if (cbCardValue==m_cbMainValue)
	{
		if (cbCardColor==m_cbMainColor) cbLogicValue=16+cbNTRight;
		else cbLogicValue=15+cbNTRight;
	}

	//���մ���
	if ((cbCardValue!=1)&&(cbCardValue!=m_cbMainValue)&&(cbCardValue!=m_cbNTValue))
	{
		if (cbCardValue<m_cbMainValue) cbLogicValue++;
		if ((m_cbNTValue!=VALUE_ERROR)&&(m_cbNTValue!=m_cbMainValue)&&(cbCardValue<m_cbNTValue)) cbLogicValue++;
	}

	return cbLogicValue+m_cbSortRight[cbLogicColor>>4];
}

//�߼���ɫ
BYTE CGameLogic::GetCardLogicColor(BYTE cbCardData)
{
	//�߼���ֵ
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//��ɫ�ж�
	if (cbCardColor==COLOR_NT) return COLOR_NT;
	if (cbCardColor==m_cbMainColor) return COLOR_NT;

	//��ֵ�ж�
	if (cbCardValue==m_cbNTValue) return COLOR_NT;
	if (cbCardValue==m_cbMainValue) return COLOR_NT;

	return cbCardColor;
}

//���л�ɫ
BYTE CGameLogic::GetCardLogicColor(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��ȡ��ɫ
	BYTE cbFirstColor=GetCardLogicColor(cbCardData[0]);
	BYTE cbSecondColor=GetCardLogicColor(cbCardData[cbCardCount-1]);

	return (cbFirstColor==cbSecondColor)?cbFirstColor:COLOR_ERROR;
}

//�Ƿ�����
bool CGameLogic::IsLineValue(BYTE cbFirstCard, BYTE cbSecondCard)
{
	//��ȡ��ֵ
	BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard);
	BYTE cbSecondLogicValue=GetCardLogicValue(cbSecondCard);

	//�����ж�
	return (cbFirstLogicValue==(cbSecondLogicValue+1));
}

//�Ƿ�ͬ��
bool CGameLogic::IsSameColor(BYTE cbFirstCard, BYTE cbSecondCard)
{
	//��ȡ��ɫ
	BYTE cbFirstLogicColor=GetCardLogicColor(cbFirstCard);
	BYTE cbSecondLogicColor=GetCardLogicColor(cbSecondCard);

	//ͬ���ж�
	return (cbFirstLogicColor==cbSecondLogicColor);
}

//��ȡ�˿�
BYTE CGameLogic::DistillCardByColor(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardColor, BYTE cbResultCard[])
{
	//��������
	BYTE cbResultCount=0;

	//��ȡ�˿�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��ɫ�ж�
		if (GetCardLogicColor(cbCardData[i])==cbCardColor) 
		{
			cbResultCard[cbResultCount++]=cbCardData[i];
			continue;
		}

		//��ֹ�ж�
		if (cbResultCount>0) break;
	}

	return cbResultCount;
}

//��ȡ�˿�
BYTE CGameLogic::DistillCardByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagSameDataInfo & SameDataInfo)
{
	//����Ч��
	ASSERT((cbSameCount>=2)&&(cbSameCount<=PACK_COUNT));
	if ((cbSameCount<2)||(cbSameCount>PACK_COUNT)) return 0;

	//���ñ���
	ZeroMemory(&SameDataInfo,sizeof(SameDataInfo));

	//��ȡ�ж�
	if (cbCardCount<cbSameCount) return 0;

	//��ȡ�˿�
	for (BYTE i=0;i<cbCardCount-cbSameCount+1;i++)
	{
		//��Ŀ�ж�
		for (BYTE j=1;j<cbSameCount;j++)
		{
			if (cbCardData[i]!=cbCardData[i+j]) break;
		}

		//�ɹ�����
		if (j==cbSameCount)
		{
			//ͬ������
			SameDataInfo.cbBlockCount++;

			//�˿�����
			for (BYTE k=0;k<cbSameCount;k++)
			{
				SameDataInfo.cbSameData[SameDataInfo.cbCardCount++]=cbCardData[i+k];
			}
		}

		//��������
		i+=j-1;
	}

	return SameDataInfo.cbBlockCount;
}

//��ȡ�˿�
BYTE CGameLogic::DistillTrackorByCount(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbSameCount, tagTractorDataInfo & TractorDataInfo)
{
	//����Ч��
	ASSERT((cbSameCount>=2)&&(cbSameCount<=PACK_COUNT));
	if ((cbSameCount<2)||(cbSameCount>PACK_COUNT)) return 0;

	//���ñ���
	ZeroMemory(&TractorDataInfo,sizeof(TractorDataInfo));

	//��ȡͬ��
	tagSameDataInfo SameDataInfo;
	DistillCardByCount(cbCardData,cbCardCount,cbSameCount,SameDataInfo);

	//�ƶ��ж�
	if (SameDataInfo.cbBlockCount<2) return 0;

	//��������
	BYTE cbResultIndex=0;
	for (BYTE i=0;i<SameDataInfo.cbBlockCount-1;i++)
	{
		//��������
		if (SameDataInfo.cbSameData[i*cbSameCount]==0) continue;

		//��������
		BYTE cbLineCount=1;
		BYTE cbTrackorIndex[MAX_TRACKOR]={i*cbSameCount};

		//�����ж�
		for (BYTE j=i;j<SameDataInfo.cbBlockCount-1;j++)
		{
			//��������
			if (SameDataInfo.cbSameData[(j+1)*cbSameCount]==0) continue;

			//��������
			BYTE cbSecondIndex=(j+1)*cbSameCount;
			BYTE cbFirstIndex=cbTrackorIndex[cbLineCount-1];
			BYTE cbFirstCardData=SameDataInfo.cbSameData[cbFirstIndex];
			BYTE cbSecondCardData=SameDataInfo.cbSameData[cbSecondIndex];

			//�����ж�
			if (IsLineValue(cbFirstCardData,cbSecondCardData)==true)
			{
				ASSERT(cbLineCount<MAX_TRACKOR);
				cbTrackorIndex[cbLineCount++]=cbSecondIndex;
				continue;
			}

			//�������
			if (cbFirstCardData==cbSecondCardData) continue;
			if (GetCardValue(cbSecondCardData)==m_cbNTValue) continue;
			if (GetCardValue(cbSecondCardData)==m_cbMainValue) continue;

			break;
		}

		//�ɹ��ж�
		if (cbLineCount>=2)
		{
			//��󳤶�
			if (cbLineCount>TractorDataInfo.cbTractorMaxLength)
			{
				TractorDataInfo.cbTractorMaxLength=cbLineCount;
				TractorDataInfo.cbTractorMaxIndex=cbResultIndex;
			}

			//���ý��
			TractorDataInfo.cbTractorCount++;
			TractorDataInfo.cbCardCount+=cbLineCount*cbSameCount;
			TractorDataInfo.cbTractorData[cbResultIndex++]=cbLineCount;
			TractorDataInfo.cbTractorData[cbResultIndex++]=cbSameCount;

			//��������
			for (BYTE k=0;k<cbLineCount;k++)
			{
				BYTE cbIndex=cbTrackorIndex[k];
				for (BYTE l=0;l<cbSameCount;l++)
				{
					TractorDataInfo.cbTractorData[cbResultIndex++]=SameDataInfo.cbSameData[cbIndex+l];
					SameDataInfo.cbSameData[cbIndex+l]=0;
				}
			}
		}
	}

	return TractorDataInfo.cbTractorCount;
}

//����Ȩλ
void CGameLogic::UpdateSortRight()
{
	//����Ȩλ
	for (BYTE i=0;i<CountArray(m_cbSortRight);i++) m_cbSortRight[i]=i*COLOR_RIGHT;

	//����Ȩλ
	if ((m_cbMainColor!=COLOR_ERROR)&&(m_cbMainColor!=COLOR_NT)) m_cbSortRight[m_cbMainColor>>4]=4*COLOR_RIGHT;

	//����Ȩλ
	switch (m_cbMainColor)
	{
	case COLOR_MEI_HUA:		//÷��
		{
			m_cbSortRight[COLOR_HEI_TAO>>4]=COLOR_RIGHT;
			break;
		}
	case COLOR_HONG_TAO:	//����
		{
			m_cbSortRight[COLOR_FANG_KUAI>>4]=2*COLOR_RIGHT;
			break;
		}
	}

	return;
}

//�����˿�
bool CGameLogic::RectifyCardWeave(const BYTE cbCardData[], BYTE cbCardCount, const tagAnalyseResult & TargetResult, tagAnalyseResult & RectifyResult)
{
	//���ñ���
	ZeroMemory(&RectifyResult,sizeof(RectifyResult));

	//��ɫ�ж�
	RectifyResult.cbCardColor=GetCardLogicColor(cbCardData,cbCardCount);
	if (RectifyResult.cbCardColor==COLOR_ERROR) return false;

	//�����˿�
	BYTE cbTempCardData[MAX_COUNT];
	BYTE cbTempCardCount=cbCardCount;
	CopyMemory(cbTempCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//ѭ������
	for (BYTE i=PACK_COUNT;i>=2;i--)
	{
		//��������
		tagSameDataInfo * pSameDataInfoRectify=&RectifyResult.SameDataInfo[i-1];
		tagTractorDataInfo * pTractorDataInfoRectify=&RectifyResult.TractorDataInfo[i-2];

		//Ŀ�����
		const tagSameDataInfo * pSameDataInfoTarget=&TargetResult.SameDataInfo[i-1];
		const tagTractorDataInfo * pTractorDataInfoTarget=&TargetResult.TractorDataInfo[i-2];

		//��������
		if (pTractorDataInfoTarget->cbTractorCount>0)
		{
			//��ȡ����
			tagTractorDataInfo TractorDataInfo;
			DistillTrackorByCount(cbTempCardData,cbTempCardCount,i,TractorDataInfo);

			//�����ж�
			if (pTractorDataInfoTarget->cbCardCount>TractorDataInfo.cbCardCount) return false;
			if (pTractorDataInfoTarget->cbTractorMaxLength>TractorDataInfo.cbTractorMaxLength) return false;

			//��������
			BYTE cbCollectCount=0;
			BYTE cbCollectCardData[MAX_COUNT];

			//��������
			pTractorDataInfoRectify->cbTractorMaxIndex=255;
			pTractorDataInfoRectify->cbTractorMaxLength=pTractorDataInfoTarget->cbTractorMaxLength;

			//���ý��
			BYTE cbIndex=0;
			for (BYTE j=0;j<TractorDataInfo.cbTractorCount;j++)
			{
				//��������
				BYTE cbRectifyLineCount=TractorDataInfo.cbTractorData[cbIndex];
				BYTE cbResidualLineCount=(pTractorDataInfoTarget->cbCardCount-cbCollectCount)/i;

				//���Զ���
				BYTE cbSameCount=i;
				BYTE cbLineCount=__min(cbRectifyLineCount,cbResidualLineCount);

				//��������
				pTractorDataInfoRectify->cbTractorCount++;
				pTractorDataInfoRectify->cbCardCount+=cbLineCount*cbSameCount;

				//��������
				pTractorDataInfoRectify->cbTractorData[cbIndex]=cbLineCount;
				pTractorDataInfoRectify->cbTractorData[cbIndex+1]=cbSameCount;

				//�����˿�
				BYTE cbInsertCount=cbLineCount*cbSameCount;
				CopyMemory(&cbCollectCardData[cbCollectCount],&TractorDataInfo.cbTractorData[cbIndex+2],cbInsertCount*sizeof(BYTE));
				CopyMemory(&pTractorDataInfoRectify->cbTractorData[cbIndex+2],&TractorDataInfo.cbTractorData[cbIndex+2],cbInsertCount*sizeof(BYTE));

				//���ñ���
				cbIndex+=(cbInsertCount+2);
				cbCollectCount+=cbInsertCount;

				//����ж�
				if (cbCollectCount>=pTractorDataInfoTarget->cbCardCount) break;
			}

			//ɾ���˿�
			cbTempCardCount-=cbCollectCount;
			RemoveCard(cbCollectCardData,cbCollectCount,cbTempCardData,cbTempCardCount+cbCollectCount);
		}

		//����ͬ��
		if (pSameDataInfoTarget->cbCardCount>0)
		{
			//��ȡͬ��
			tagSameDataInfo SameDataInfo;
			DistillCardByCount(cbTempCardData,cbTempCardCount,i,SameDataInfo);

			//�����ж�
			if (pSameDataInfoTarget->cbCardCount>SameDataInfo.cbCardCount) return false;
			if (pSameDataInfoTarget->cbBlockCount>SameDataInfo.cbBlockCount) return false;

			//�����˿�
			pSameDataInfoRectify->cbCardCount=pSameDataInfoTarget->cbCardCount;
			pSameDataInfoRectify->cbBlockCount=pSameDataInfoTarget->cbBlockCount;
			CopyMemory(pSameDataInfoRectify->cbSameData,SameDataInfo.cbSameData,sizeof(BYTE)*pSameDataInfoRectify->cbCardCount);

			//��������
			BYTE cbRemoveCount=pSameDataInfoRectify->cbCardCount;
			LPBYTE pcbRemoveCard=&pSameDataInfoRectify->cbSameData[0];

			//ɾ���˿�
			cbTempCardCount-=cbRemoveCount;
			RemoveCard(pcbRemoveCard,cbRemoveCount,cbTempCardData,cbTempCardCount+cbRemoveCount);
		}
	}

	//���õ���
	if (cbTempCardCount>0)
	{
		RectifyResult.SameDataInfo[0].cbCardCount=cbTempCardCount;
		RectifyResult.SameDataInfo[0].cbBlockCount=cbTempCardCount;
		CopyMemory(RectifyResult.SameDataInfo[0].cbSameData,cbTempCardData,cbTempCardCount);
	}

	return true;
}

//������Ŀ
BYTE CGameLogic::GetIntersectionCount(const BYTE cbCardData1[], BYTE cbCardCount1, const BYTE cbCardData2[], BYTE cbCardCount2)
{
	//��������
	BYTE cbAnalyseCard[MAX_COUNT];
	CopyMemory(cbAnalyseCard,cbCardData2,sizeof(BYTE)*cbCardCount2);

	//�����ж�
	BYTE cbIntersectionCount=0;
	for (BYTE i=0;i<cbCardCount1;i++)
	{
		for (BYTE j=0;j<cbCardCount2;j++)
		{
			if (cbCardData1[i]==cbAnalyseCard[j])
			{
				//���ñ���
				cbAnalyseCard[j]=0;
				cbIntersectionCount++;

				break;
			}
		}
	}

	return cbIntersectionCount;
}

//////////////////////////////////////////////////////////////////////////
