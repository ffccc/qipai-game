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
	if (IsOnlyOne(cbCardData,cbCardCount)) return CT_SINGLE;//���ơ� 

	if (IsDouble(cbCardData,cbCardCount)) return CT_DOUBLE;//���� 

	if (IsSingleContinue(cbCardData,cbCardCount)) return CT_SINGLE_LINE;//��˳  

	if (IsDoubleContinue(cbCardData,cbCardCount)) return CT_DOUBLE_LINE;//˫˳ 

	if (IsThree(cbCardData,cbCardCount)) return CT_THREE;//��ͬ��ը��

	if (IsBomb(cbCardData,cbCardCount)) return CT_FOUR;//����ը��

	if (IsDoubleKong(cbCardData,cbCardCount)) return CT_MISSILE_CARD;//˫��

	if ( Is44A( cbCardData,cbCardCount ) )
	{
		if (cbCardCount != 3)  return CT_ERROR;
		BYTE cbFourCount = 0;   //4����Ŀ
		BYTE cbRed = 0;         //��4����Ŀ
		BYTE cbBlack = 0;       //���ĵ���Ŀ
		BYTE cbOne = 0;         //A����Ŀ		

		for (BYTE i=0; i<3; i++)
		{
			if (cbCardData[i] == 0x04 || cbCardData[i] == 0x14 || cbCardData[i] == 0x24||cbCardData[i] == 0x34)
			{
				++cbFourCount;
				if (cbCardData[i] == 0x04 || cbCardData[i] == 0x24)
				{
					++cbRed;
				}
				if (cbCardData[i] == 0x14 || cbCardData[i] == 0x34)
				{
					++cbBlack;
				}
			}

			if (cbCardData[i] == 0x01 || cbCardData[i] == 0x11 || cbCardData[i] == 0x21||cbCardData[i] == 0x31)
			{
				++cbOne;
			}
		}

		if (cbFourCount == 2 && cbOne == 1)
		{
			if (cbRed == 2) return CT_A44_red;
			else if (cbBlack == 2) return CT_A44_black;
			else return CT_A44_red_black;

		}
	}
	return CT_ERROR;
}

//�����˿�//���Դ�С����ĿST_COUNTΪ��ĿST_ORDER��С
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
	if (cbCardColor==0x40) return cbCardValue+3;///////////////��A443��
	return (cbCardValue<=3)?(cbCardValue+13):cbCardValue;///////////////��A443��
}

//�Ա��˿�	//�����б�//�Լ��˿�����//������Ŀ//�Լ�������
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{

	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);//�Լ�
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);//�ϸ����Ƶ����

	//�����ж�
	if (cbNextType == CT_ERROR) return false;
	//�Լ�����
	if (cbNextType == CT_A44_red || cbNextType == CT_A44_red_black) return true;
	//�ϼҳ���
	if ( cbFirstType == CT_A44_red || cbFirstType == CT_A44_red_black ) return false;

	//����һ�������
	if(cbNextType == cbFirstType)
	{
		switch (cbFirstType)
		{
		case CT_SINGLE:		//��������
		case CT_DOUBLE:		//��������
		case CT_SINGLE_LINE://��������
		case CT_THREE:		//��������
		case CT_DOUBLE_LINE://��������
		case CT_FOUR:		//��������
		{
			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[0]);
			
			if(cbFirstCount == cbNextCount)//�Ա��˿�
				return cbNextLogicValue>cbFirstLogicValue;
			
		}
		}
	}
	//���Ͳ�һ�������
	else
	{
		switch (cbFirstType)
		{
		case CT_SINGLE:		//��������
		case CT_DOUBLE:		//��������
			{
				if(cbNextType >= 4 && cbNextType != 5)
				return true;
			}
		case CT_SINGLE_LINE://��������
			{
				if(cbNextType >= 4 /*&& cbNextType != 5*/)
					return true;
				if(cbNextType == 5)
				{
					BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[0]);
					BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[0]);

					//�Ա��˿�
					return cbNextLogicValue>cbFirstLogicValue;
				}
			}
		case CT_THREE:		//��������
		case CT_DOUBLE_LINE://��������
			{
				if(cbNextType >= 6)
					return true;
			}
		case CT_FOUR:		//��������
			{
				if(cbNextType >= 7)
					return true;
			}
		case CT_MISSILE_CARD:		//��������
			{
				if(cbNextType >= 8)
					return true;
			}
		case CT_A44_black:		//��������
			{
				if(cbNextType >= 10)
					return true;
			}
		}
	
	
	}
	return false;
}

//��������//��ʾ����===����(�����˿�//�˿���Ŀ//�����б�//������Ŀ)
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
	//���ý��
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//�����˿�(����С)
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//��ȡ����
	BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

	if((cbTurnOutType == CT_A44_red_black)||(cbTurnOutType ==CT_A44_red))return false;

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
			if (cbTurnCardCount<=4)
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
	case CT_SINGLE_LINE:			//��������
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
	case CT_DOUBLE_LINE:			//��������
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
	}
	//����ը��
	if ((cbCardCount==4)&&(cbTurnOutType < CT_MISSILE_CARD))
	{
		//��������
		BYTE cbLogicValue=0;
		if (cbTurnOutType==CT_FOUR) cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

		//�����˿�
		SortCardList(cbCardData,cbCardCount,ST_ORDER);
		//����ը��
		for(int i = 0; i <  cbCardCount - 3 ;i++)
		{
			if((GetCardValue(cbCardData[i]) == GetCardValue(cbCardData[i-1])) && (GetCardValue(cbCardData[i])== GetCardValue(cbCardData[i-2]))&& (GetCardValue(cbCardData[i])== GetCardValue(cbCardData[i-3]))) 
			{
				if( GetCardLogicValue(cbCardData[i])>cbLogicValue)
				{
					OutCardResult.cbCardCount=4;
					OutCardResult.cbResultCard[0]=cbCardData[i];
					OutCardResult.cbResultCard[1]=cbCardData[i-1];
					OutCardResult.cbResultCard[2]=cbCardData[i-2];
					OutCardResult.cbResultCard[3]=cbCardData[i-3];
					return true;
				}
				else
				{
					OutCardResult.cbCardCount=4;
					OutCardResult.cbResultCard[0]=cbCardData[i];
					OutCardResult.cbResultCard[1]=cbCardData[i-1];
					OutCardResult.cbResultCard[2]=cbCardData[i-2];
					OutCardResult.cbResultCard[3]=cbCardData[i-3];
					return true;				
				}
			}
			
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
//�Ƿ����
BOOL CGameLogic::IsDouble(const BYTE iCardList[], int iCardCount)
{
	if (iCardCount!=2) return FALSE;
	if (GetCardValue(iCardList[0]) == GetCardValue(iCardList[1]))
		return true;
	else 
		return false;
}
//�Ƿ�����
BOOL CGameLogic::IsThree(const BYTE iCardList[], int iCardCount)
{
	if (iCardCount!=3) return FALSE;
	return ((GetCardValue(iCardList[0])==GetCardValue(iCardList[1]))&&(GetCardValue(iCardList[0])==GetCardValue(iCardList[2])));
}
//�Ƿ�˳(����3��)
BOOL CGameLogic::IsSingleContinue(const BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 3) return FALSE;

	BYTE cbCardData[MAX_COUNT];
	CopyMemory(cbCardData,iCardList,sizeof(BYTE)*iCardCount);

	//�����˿�
	SortCardList(cbCardData,iCardCount,ST_ORDER);

	for (int i = 0;i < iCardCount - 1;i++)
	{
		if (GetCardValue(cbCardData[i]) == 2 || cbCardData[i] == 0x4E || cbCardData[i] == 0x4F||GetCardValue(cbCardData[i]) == 3 )
			return FALSE;
		if ((GetCardLogicValue(cbCardData[i]) % 15) != ((GetCardLogicValue(cbCardData[i+1]) + 1) % 15))
		{

			return FALSE;

		}
	}
	return TRUE;
}
//�Ƿ�44��
BOOL CGameLogic::Is44A(const BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) return FALSE;

	int Number4 = 0;
	int NumberA = 0;
	for(int i = 0; i < 3; i++)
	{
		if(GetCardValue(iCardList[i]) == GetCardValue(0x04))
		{
			Number4++;
		}
		if(GetCardValue(iCardList[i]) == GetCardValue(0x01))
		{
			NumberA++;
		}
	}
	if( Number4 == 2 && NumberA == 1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//�Ƿ�˫��
BOOL CGameLogic::IsDoubleKong(const BYTE iCardList[], int iCardCount)
{
	if(iCardCount != 2)  return FALSE;
	if((iCardList[0] == 0x4E && iCardList[1] == 0x4F)||(iCardList[0] == 0x4F && iCardList[1] == 0x4E))
		return TRUE;
	else
		return FALSE;
}
//�Ƿ�ը��
BOOL CGameLogic::IsBomb(const BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 4) return FALSE;

	int pd1 = GetCardValue(iCardList[0]);

	for (int i = 0;i < iCardCount;i++)
	{
		if (GetCardValue(iCardList[i]) != pd1) return FALSE;
		else continue;
	}

	return TRUE;
}
//�Ƿ�˫˳ 6�Ż�6�����ϣ��������Զ�Ϊ��������ɫ���ޣ����磺334455��66778899�ȣ�
BOOL CGameLogic::IsDoubleContinue(const BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 6 || iCardCount % 2 != 0) return FALSE;

	BYTE cbCardData[MAX_COUNT];
	CopyMemory(cbCardData,iCardList,sizeof(BYTE)*iCardCount);

	//�����˿�
	SortCardList(cbCardData,iCardCount,ST_ORDER);

	for (int i = 0;i < iCardCount - 2;i++)
	{
		if (GetCardValue(cbCardData[i]) == 2 || cbCardData[i] == 0x4E || cbCardData[i] == 0x4F||GetCardValue(cbCardData[i]) == 3 )
			return FALSE;
		if ((GetCardLogicValue(cbCardData[i]) % 15) != ((GetCardLogicValue(cbCardData[i+2]) + 1) % 15))
			return FALSE;
	}
	return TRUE;
}
//�Ƿ���Բ�
bool CGameLogic::CompareChaCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	if(cbNextCount != 2) return false;

	if((GetCardValue(cbFirstCard[0] )== GetCardValue(cbNextCard[0]))&&(GetCardValue(cbFirstCard[0]) == GetCardValue(cbNextCard[1])))
		return TRUE;
	return false;
}
//�Ƿ���Բ�
bool CGameLogic::CompareDianCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	if(cbNextCount != 1) return false;

	if(GetCardValue(cbFirstCard[0]) == GetCardValue(cbNextCard[0]))
		return TRUE;
	return false;
}
