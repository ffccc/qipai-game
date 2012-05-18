#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const BYTE	CGameLogic::m_bCardArray[52] =									//�˿�����
{
	 1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
	14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
};

//��ȡ�˿���ֵ
BYTE CGameLogic::GetCardValue(BYTE bCard)
{
	BYTE bValue = bCard%13;
	if (bValue == 0 || bValue == 11 || bValue == 12) bValue=10;
	if (bValue == 1) bValue = 11;
	return bValue;
}

//��ȡ�˿���ֵ
BYTE CGameLogic::GetCardSum(const BYTE bCardBuffer[], const BYTE bCardCount)
{
	BYTE cbCardSum = 0;
	
	for(int i = 0; i < bCardCount; i++)
	{
		cbCardSum += GetCardValue(bCardBuffer[i]);
	}
	
	if(cbCardSum > 21)
	{
		for(int i = 0; i < bCardCount; i++)
		{
			//�Ƿ���A
			if(GetCardValue(bCardBuffer[i]) == 11)
			{
				//��1����
				cbCardSum -= 10;
			}
		}
	}

	return cbCardSum;
}

//�Ƿ���
bool CGameLogic::IsBurst(const BYTE bCardBuffer[], BYTE bCardCount)
{
	WORD wCardSum = 0;
	
	for(int i = 0; i < bCardCount; i++)
	{
		wCardSum += GetCardValue(bCardBuffer[i]);
	}

	if(wCardSum > 21)
	{
		for(int i = 0; i < bCardCount; i++)
		{
			//�Ƿ���A
			if(GetCardValue(bCardBuffer[i]) == 11)
			{
				//��1����
				wCardSum -= 10;
			}
		}
	}

	if(wCardSum > 21) return true;
	
	return false;
}

//�ܷ����
bool CGameLogic::CanCutCard(const BYTE bFirstCard, const BYTE bLastCard)
{
	BYTE bFirstValue = bFirstCard % 13;
	BYTE bLastValue  = bLastCard  % 13;
	if (bFirstValue == bLastValue) return true;
	return false;
}

//�Ƿ�BlackJack
bool CGameLogic::IsBlackJack(const BYTE bCardBuffer[],BYTE bCardCount)
{
	if(bCardCount!=2)
		return false;
	BYTE bCardSum=GetCardSum(bCardBuffer,bCardCount);
	if(bCardSum==21)
		return true;
	//if((GetCardValue(bCardBuffer[0]) == 11) && (GetCardValue(bCardBuffer[1]) == 10)) return true;
	//if((GetCardValue(bCardBuffer[1]) == 10) && (GetCardValue(bCardBuffer[0]) == 11)) return true;
	return false;
}

//�Աȵ�ֻ�˿�
bool CGameLogic::CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard)
{
	BYTE bFirstValue = GetCardValue(bFirstCard);
	BYTE bLastValue  = GetCardValue(bLastCard);
	if (bFirstValue == bLastValue) return bFirstValue > bLastValue;
	return bFirstValue > bLastValue;
}

//�Ƿ�777��
bool CGameLogic::IsSevensCard(const BYTE bCardBuffer[],BYTE bCardCunt)
{
	if(bCardCunt!=3)
		return false;
	if((GetCardValue(bCardBuffer[0])==7)
		&&(GetCardValue(bCardBuffer[1])==7)
		&&(GetCardValue(bCardBuffer[2])==7))
		return true;
	return false;
}

//�Ƿ�NС��
BYTE CGameLogic::IsNDragon(const BYTE bCardBuffer[],BYTE bCardCount)
{
	if(bCardCount<5)
		return 0;
	else if(GetCardSum(bCardBuffer,bCardCount)<=21)
		return bCardCount;
	return 0;
}
//�ƶ�ʤ��
BYTE CGameLogic::DeduceWiner(const BYTE bBankerCard[],const BYTE bBankerCardCount,
							 const BYTE bIdleCard[],  const BYTE bIdleCardCount,BYTE *times)
{
	//777���
	if(IsSevensCard(bBankerCard,bBankerCardCount))
	{
		*times=3;
		return 1;
	}
	if(IsSevensCard(bIdleCard,bIdleCardCount))
	{
		*times=3;
		return 2;
	}
	//�Ƿ�NС��
	BYTE nBankerDragon=IsNDragon(bBankerCard,bBankerCardCount);
	BYTE nIdleDragon=IsNDragon(bIdleCard,bIdleCardCount);
	if(nBankerDragon>0)
	{
		if(nIdleDragon>0)
		{
			if(bBankerCardCount>bIdleCardCount)
			{
				MessageBox(0,"ׯNС��",0,0);
				*times=3;
				return 1;
			}
			else if(bBankerCardCount==bIdleCardCount)
				return 0;
			else
			{
				//MessageBox(0,"ׯNС��",0,0);
				*times=3;
				return 2;
			}
		}
		else
		{
			//MessageBox(0,"ׯNС��",0,0);
			*times=3;
			return 1;
		}
	}
	//�м�NС��
	if(nIdleDragon>0)
	{
		if(nBankerDragon>0)
		{
			if(bBankerCardCount>bIdleCardCount)
			{
				//MessageBox(0,"��NС��",0,0);
				*times=3;
				return 1;
			}
			else if(bBankerCardCount==bIdleCardCount)
				return 0;
			else
			{
				//MessageBox(0,"��NС��",0,0);
				*times=3;
				return 2;
			}
		}
		else
		{
			//MessageBox(0,"��NС��",0,0);
			*times=3;
			return 2;
		}
	}
	//ƽ
	if(IsBlackJack(bBankerCard,bBankerCardCount) && IsBlackJack(bIdleCard,bIdleCardCount))
	{
		return 0;
	}
		

	//ׯ��Ӯ
	if(IsBlackJack(bBankerCard,bBankerCardCount) && !IsBlackJack(bIdleCard,bIdleCardCount)) 
	{
		//MessageBox(0,"ׯ�ڽܿ�",0,0);
		*times=2;
		return 1;
	}
	//�м�Ӯ
	if(!IsBlackJack(bBankerCard,bBankerCardCount) && IsBlackJack(bIdleCard,bIdleCardCount)) 
	{
		//MessageBox(0,"�кڽܿ�",0,0);
		*times=2;
		return 2;
	}
	//�Ƚϴ�С
	BYTE bBankerSum = GetCardSum(bBankerCard,bBankerCardCount);
	BYTE bIdleSum   = GetCardSum(bIdleCard,bIdleCardCount);
	
	//for(int i=0;i<bBankerCardCount;i++)
	//{
	//	bBankerSum += GetCardValue(bBankerCard[i]);
	//}

	//for(int i=0;i<bIdleCardCount;i++)
	//{
	//	bIdleSum += GetCardValue(bIdleCard[i]);
	//}

	if((bBankerSum<=21)&&(bIdleSum<=21))
	{
		if(bBankerSum>bIdleSum)
		{
			return 1;	
		}
		else if(bBankerSum < bIdleSum)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}
	else if((bBankerSum<=21&&bIdleSum>21))
	{
		return 1;
	}
	else if((bBankerSum>21&&bIdleSum<=21))
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

//�����˿�
void CGameLogic::RandCard(BYTE bCardBuffer[], BYTE bBufferCount)
{
	BYTE bSend = 0,bStation = 0, bCardList[CountArray(m_bCardArray)];

	CopyMemory(bCardList, m_bCardArray, sizeof(m_bCardArray));
	
	static long int dwRandCount = 0L;
	
	srand((unsigned)time(NULL) + dwRandCount++);
	
	do
	{
		bStation = rand() % (CountArray(m_bCardArray) - bSend);

		bCardBuffer[bSend++] = bCardList[bStation];
		bCardList[bStation]  = bCardList[CountArray(m_bCardArray)-bSend];
	
	} while (bSend < bBufferCount);	
}

//�����˿�
void CGameLogic::SortCard(BYTE bCardList[], BYTE bCardCount)
{
	bool bSorted     = true;
	BYTE bTemp,bLast = bCardCount - 1;
	do
	{
		bSorted = true;
		for (BYTE i = 0; i < bLast; i++) 
		{
			if (CompareOnlyOne(bCardList[i],bCardList[i+1]) == FALSE)
			{	
				bTemp = bCardList[i];
				bCardList[i]   = bCardList[i+1];
				bCardList[i+1] = bTemp;
				bSorted = false;
			}	
		}
		bLast--;
	} while (bSorted == false);

	return;
}
//////////////////////////////////////////////////////////////////////////