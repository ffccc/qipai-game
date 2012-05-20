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
	0x41,0x42,															//���С��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x41,0x42,															//���С��
};

//////////////////////////////////////////////////////////////////////////



//��ȡ����
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount, bool bDoubleBomo)
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
			//˫���ж�
			if ((bCardData[0]==0x42)&&(bCardData[1]==0x42)) return CT_KING_BIG;
			if ((bCardData[0]==0x41)&&(bCardData[1]==0x41)) return CT_KING_LITTET;

			//�����ж�
			BYTE bLastLogicValue=GetCardLogicValue(bCardData[1]);
			BYTE bFirstLogicValue=GetCardLogicValue(bCardData[0]);
			return (bFirstLogicValue==bLastLogicValue)?CT_DOUBLE:CT_ERROR;
		}
	default:	//ը������
		{
			//�����ж�
			if (bCardCount==3)
			{
				//��ֵ�Ա�
				bool bFixValue=true;
				if ((bCardData[2]&LOGIC_MASK_VALUE)!=5) bFixValue=false;
				if ((bCardData[1]&LOGIC_MASK_VALUE)!=10) bFixValue=false;
				if ((bCardData[0]&LOGIC_MASK_VALUE)!=13) bFixValue=false;

				//��ɫ�Ա�
				if (bFixValue==true)
				{
					//��ȡ��ɫ
					BYTE cbCardColor[3];
					cbCardColor[0]=bCardData[0]&LOGIC_MASK_COLOR;
					cbCardColor[1]=bCardData[1]&LOGIC_MASK_COLOR;
					cbCardColor[2]=bCardData[2]&LOGIC_MASK_COLOR;

					//��ɫ�ж�
					if (cbCardColor[0]!=cbCardColor[1]) return CT_CRURA_FAKE;
					if (cbCardColor[0]!=cbCardColor[2]) return CT_CRURA_FAKE;

					return CT_CRURA_PURE;
				}
			}

			//ը���ж�
			BYTE bFirstLogicValue=GetCardLogicValue(bCardData[0]);
			BYTE bLastLogicValue=GetCardLogicValue(bCardData[bCardCount-1]);
			if (bFirstLogicValue==bLastLogicValue) return CT_BOMB;

			break;
		}
	}

	//˫ը����
	if ((bDoubleBomo==true)&&(bCardCount>=6))
	{
		//��������
		BYTE cbCount[2]={1,1};
		BYTE cbValue[2]={GetCardValue(bCardData[0]),GetCardValue(bCardData[bCardCount-1])};

		//�����ж�
		for (int i=1;i<bCardCount-1;i++)
		{
			BYTE cbTemp=GetCardValue(bCardData[i]);
			if (cbTemp==cbValue[0]) cbCount[0]++;
			else if (cbTemp==cbValue[1]) cbCount[1]++;
			else break;
		}

		//����ж�
		if (((cbCount[0]+cbCount[1])==bCardCount)&&(cbCount[0]>=3)&&(cbCount[1]>=3)) return CT_DOUBLE_BOMB;
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(bCardData,bCardCount,AnalyseResult);

	//�����ж�
	if ((AnalyseResult.bDoubleCount>=3)&&(AnalyseResult.bDoubleCount*2==bCardCount))
	{
		//�߼�����
		BYTE bLogicValue=AnalyseResult.bDoubleLogicVolue[0];

		//�����ж�
		if (bLogicValue<=14)
		{
			//�����ж�
			bool bSeriesCard=false;
			for (BYTE i=1;i<AnalyseResult.bDoubleCount;i++)
			{
				if (AnalyseResult.bDoubleLogicVolue[i]!=(bLogicValue-i)) break;
			}

			//�����ж�
			if (i==AnalyseResult.bDoubleCount) return CT_DOUBLE_LINE;
		}
	}

	//�����ж�
	if ((AnalyseResult.bSignedCount>=3)&&(AnalyseResult.bSignedCount==bCardCount))
	{
		//�߼�����
		BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

		//�����ж�
		if (bLogicValue<=14)
		{
			//�����ж�
			bool bSeriesCard=false;
			for (BYTE i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-i)) break;
			}

			//�����ж�
			if (i==AnalyseResult.bSignedCount) return CT_SINGLE_LINE;
		}
	}

	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount, BYTE cbSortType)
{
	if (bCardCount==0) return;

	switch (cbSortType)
	{
	case ST_ORDER:
		{
			//ת����ֵ
			BYTE bLogicValue[MAX_COUNT];
			for (BYTE i=0;i<bCardCount;i++) bLogicValue[i]=GetCardLogicValue(bCardData[i]);

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
	case ST_VIEW:
	case ST_BOMB:
		{
			//��ȡ�˿�
			BYTE cbResultCount=0;
			BYTE cbTempCard[MAX_COUNT];
			CopyMemory(cbTempCard,bCardData,sizeof(BYTE)*bCardCount);

			//��ȡ����
			BYTE cbColorMask[]={0x20,0x20,0x00,0x00,0x30,0x30,0x10,0x10};
			for (BYTE j=0;j<CountArray(cbColorMask);j++)
			{
				BYTE cbIndex[3]={0xFF,0xFF,0xFF};
				for (BYTE i=0;i<bCardCount;i++)
				{
					//��ȡ�ж�
					if (cbTempCard[i]==0) continue;

					//�����ж�
					BYTE cbCardColor=GetCardColor(cbTempCard[i]);
					BYTE cbCardValue=GetCardValue(cbTempCard[i]);
					if (cbCardColor==cbColorMask[j])
					{
						if (cbCardValue==5) cbIndex[0]=i;
						else if (cbCardValue==10) cbIndex[1]=i;
						else if (cbCardValue==13) cbIndex[2]=i;
					}

					//�������
					if ((cbIndex[0]!=0xFF)&&(cbIndex[1]!=0xFF)&&(cbIndex[2]!=0xFF))
					{
						bCardData[cbResultCount++]=cbTempCard[cbIndex[0]];
						bCardData[cbResultCount++]=cbTempCard[cbIndex[1]];
						bCardData[cbResultCount++]=cbTempCard[cbIndex[2]];
						cbTempCard[cbIndex[0]]=0;
						cbTempCard[cbIndex[1]]=0;
						cbTempCard[cbIndex[2]]=0;
						break;
					}
				}
			}

			//��ȡ����
			bool bSuccess=false;
			do
			{
				bSuccess=false;
				BYTE cbIndex[3]={0xFF,0xFF,0xFF};
				for (BYTE i=0;i<bCardCount;i++)
				{
					//��ȡ�ж�
					if (cbTempCard[i]==0) continue;

					//�����ж�
					BYTE cbCardValue=GetCardValue(cbTempCard[i]);
					if (cbCardValue==5) cbIndex[0]=i;
					else if (cbCardValue==10) cbIndex[1]=i;
					else if (cbCardValue==13) cbIndex[2]=i;

					//�������
					if ((cbIndex[0]!=0xFF)&&(cbIndex[1]!=0xFF)&&(cbIndex[2]!=0xFF))
					{
						bSuccess=true;
						bCardData[cbResultCount++]=cbTempCard[cbIndex[0]];
						bCardData[cbResultCount++]=cbTempCard[cbIndex[1]];
						bCardData[cbResultCount++]=cbTempCard[cbIndex[2]];
						cbTempCard[cbIndex[0]]=0;
						cbTempCard[cbIndex[1]]=0;
						cbTempCard[cbIndex[2]]=0;
						break;
					}
				}
			} while (bSuccess==true);

			//��ȡ�˿�
			if (cbSortType==ST_BOMB)
			{
				//��ȡ�ֲ�
				tagDistributing Distributing;
				GetDistributingInfo(cbTempCard,bCardCount,Distributing);

				//��ȡ�˿�
				BYTE cbLeftCount=0;
				for (int i=8;i>0;i--)
				{
					for (int j=14;j>=0;j--)
					{
						if (Distributing.cbDistributing[j][5]==i)
						{
							for (int k=0;k<5;k++)
							{
								for (int l=0;l<Distributing.cbDistributing[j][k];l++)
								{
									BYTE cbCardTemp=MakeCardData(j,k);
									bCardData[cbResultCount+cbLeftCount++]=cbCardTemp;
								}
							}
						}
					}
				}
			}
			else
			{
				//��ȡ�˿�
				BYTE cbLeftCount=0;
				for (BYTE i=0;i<bCardCount;i++)
				{
					if (cbTempCard[i]!=0)
					{
						bCardData[cbResultCount+cbLeftCount++]=cbTempCard[i];
						cbTempCard[i]=0;
					}
				}

				//��ͨ����
				SortCardList(&bCardData[cbResultCount],cbLeftCount,ST_ORDER);
			}

			return;
		}
	}
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
	if ((cbColor==0x40)&&(cbValue>=0x01)&&(cbValue<=0x02)) return true;

	return false;
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

//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	//������ֵ
	if (cbColorIndex==4)
	{
		return (0x40|(cbValueIndex-12));
	}

	BYTE cbCardValue=(cbValueIndex+3);
	if ((cbValueIndex>=11)&&(cbValueIndex<=12)) cbCardValue=cbValueIndex-10;

	return ((cbColorIndex<<4)|cbCardValue);
}

//��ȡ�ֲ�
void CGameLogic::GetDistributingInfo(BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
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
		Distributing.cbDistributing[cbLogicValue-3][5]++;
		Distributing.cbDistributing[cbLogicValue-3][cbCardColor>>4]++;
	}

	return;
}

//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,bNextCount,true);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount,((bNextType==CT_DOUBLE_BOMB)||(bNextType==CT_DOUBLE_LINE)));

	//�����ж�
	if (bFirstType==CT_ERROR) return false;

	//������
	switch (bFirstType)
	{
	case CT_SINGLE:
	case CT_DOUBLE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
		{
			//�����ж�
			if (bNextType!=bFirstType) return false;
			if (bFirstCount!=bNextCount) return false;
			
			//��С�Ա�
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			return (bFirstLogicValue>bNextLogicValue);
		}
	}

	//�����ж�
	if (bFirstCount>=6)
	{
		//���ը��
		if ((bFirstType==CT_BOMB)&&(bNextType==CT_DOUBLE_BOMB))
		{
			//��Ŀ�Ա�
			if (bFirstCount<bNextCount) return false;

			//��ֵ�Ա�
			BYTE cbNextValue=GetCardLogicValue(bNextList[0]);
			BYTE cbFirstValue=GetCardLogicValue(bFirstList[0]);
			return cbFirstValue>cbNextValue;
		}

		//ը��˫˳
		if ((bFirstType==CT_BOMB)&&(bNextType==CT_DOUBLE_LINE)) return true;
	}

	//ͬ�ͶԱ�
	if (bFirstType==bNextType)
	{
		switch (bFirstType)
		{
		case CT_BOMB:			//ը������
			{
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
		case CT_CRURA_PURE:		//��������
			{
				//��ȡ��ɫ
				BYTE cbNextColor=GetCardColor(bNextList[0]);
				BYTE cbFirstColor=GetCardColor(bFirstList[0]);

				//��ɫ�Ա�
				BYTE cbFirstOrder=0x00;
				if (cbFirstColor==0x00) cbFirstOrder=0x03;
				else if (cbFirstColor==0x10) cbFirstOrder=0x01;
				else if (cbFirstColor==0x20) cbFirstOrder=0x04;
				else if (cbFirstColor==0x30) cbFirstOrder=0x02;

				//��ɫ�Ա�
				BYTE cbNextOrder=0x00;
				if (cbNextColor==0x00) cbNextOrder=0x03;
				else if (cbNextColor==0x10) cbNextOrder=0x01;
				else if (cbNextColor==0x20) cbNextOrder=0x04;
				else if (cbNextColor==0x30) cbNextOrder=0x02;

				return cbFirstOrder>cbNextOrder;
			}
		case CT_DOUBLE_BOMB:	//˫ը����
			{
				//��������
				BYTE cbNextCount[2]={1,1};
				BYTE cbFirstCount[2]={1,1};
				BYTE cbNextValue[2]={GetCardValue(bNextList[0]),GetCardValue(bNextList[bNextCount-1])};
				BYTE cbFirstValue[2]={GetCardValue(bFirstList[0]),GetCardValue(bFirstList[bFirstCount-1])};

				//�����ռ�
				for (int i=1;i<bNextCount-1;i++)
				{
					BYTE cbTemp=GetCardValue(bNextList[i]);
					if (cbTemp==cbNextValue[0]) cbNextCount[0]++;
					else if (cbTemp==cbNextValue[1]) cbNextCount[1]++;
					else return true;
				}

				//�����ռ�
				for (int i=1;i<bFirstCount-1;i++)
				{
					BYTE cbTemp=GetCardValue(bFirstList[i]);
					if (cbTemp==cbFirstValue[0]) cbFirstCount[0]++;
					else if (cbTemp==cbFirstValue[1]) cbFirstCount[1]++;
					else return false;
				}

				//λ�ô���
				if ((cbNextCount[0]>=cbNextCount[1])&&(cbFirstCount[0]>=cbFirstCount[1]))
				{
					//��Ŀ�Ա�
					if (cbNextCount[0]>cbFirstCount[0]) return false;
					if (cbNextCount[1]>cbFirstCount[1]) return false;

					//��ֵ�Ա�
					if (cbNextCount[0]==cbFirstCount[0])
					{
						BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
						BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
						if (bFirstLogicValue<=bNextLogicValue) return false;
					}

					//��ֵ�Ա�
					if (cbNextCount[1]==cbFirstCount[1])
					{
						BYTE bNextLogicValue=GetCardLogicValue(bNextList[bNextCount-1]);
						BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[bFirstCount-1]);
						if (bFirstLogicValue<=bNextLogicValue) return false;
					}
				}
				else
				{
					//��Ŀ�Ա�
					if (cbNextCount[0]>cbFirstCount[1]) return false;
					if (cbNextCount[1]>cbFirstCount[0]) return false;

					//��ֵ�Ա�
					if (cbNextCount[0]==cbFirstCount[1])
					{
						BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
						BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[bFirstCount-1]);
						if (bFirstLogicValue<=bNextLogicValue) return false;
					}

					//��ֵ�Ա�
					if (cbNextCount[1]==cbFirstCount[0])
					{
						BYTE bNextLogicValue=GetCardLogicValue(bNextList[bNextCount-1]);
						BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
						if (bFirstLogicValue<=bNextLogicValue) return false;
					}
				}

				return true;
			}
		}

		return false;
	}

	//ը��˫˳
	if ((bNextType==CT_DOUBLE_LINE)&&(bFirstType==CT_BOMB)) return false;

	return bFirstType>bNextType;
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
#include <algorithm>
bool _cmp1(BYTE _bL, BYTE _bR)
{
	BYTE l = CGameLogic::GetCardColor(_bL);
	BYTE r = CGameLogic::GetCardColor(_bR);
	if(l == r)
		return CGameLogic::GetCardValue(_bL) < CGameLogic::GetCardValue(_bR);
	else
		return l<r; 
}
bool _cmp2(BYTE _bL, BYTE _bR)
{
	BYTE l = CGameLogic::GetCardValue(_bL);
	BYTE r = CGameLogic::GetCardValue(_bR);
	if(l == r)
		return CGameLogic::GetCardColor(_bL) < CGameLogic::GetCardColor(_bR);
	else
		return l<r;
}
void	CGameLogic::_DeleteCards(BYTE bCards[], BYTE& bCardCount, BYTE bDelete[], BYTE bDeleteCount)
{
	BYTE i,j,k;
	for(i=0; i<bDeleteCount; ++i){
		for(k = 0; k<bCardCount; ++k)
			if(bCards[k] == bDelete[i]){
				for(j=k; j<bCardCount; ++j)
					bCards[j] = bCards[j+1];
				--bCardCount;
				break;
			}
	}

}


void CGameLogic::_GetPrivateCard(BYTE bCards[], BYTE& bCardCount, BYTE bPrivateCards[], BYTE& bPrivateCount)
{
	bPrivateCount = 0;
	//�Ƿ��Ѿ����
	bool bisOut[MAX_COUNT];
	memset(bisOut, 0, sizeof(bisOut));
	BYTE  bBufCard1[MAX_COUNT];
	BYTE bBufCard2[MAX_COUNT];
	BYTE bBufCardCount1 = 0, bBufCardCount2 = 0;
	
	//���Ȳ�����
	BYTE i,j,k;
	for(i=0; i<bCardCount; ++i){
		k = GetCardValue(bCards[i]);
		if( k == 5 || k == 10 || k==13)
		{
			//bisOut[i] = true;
			bBufCard1[bBufCardCount1] = bCards[i];
			++bBufCardCount1;
		}
	}
	//����ȡ�����ư��ջ�ɫ����
	if(bBufCardCount1>2){
		bPrivateCards[bPrivateCount++] = 0xff;
		BYTE bb[3] = {0};
		std::sort(bBufCard1, bBufCard1+bBufCardCount1, _cmp1);
		j = GetCardColor(bBufCard1[0]);
		k = GetCardValue(bBufCard1[0]);
		if(k == 5) ++bb[0];
		else if(k == 10) ++bb[1];
		else  ++bb[2];
		
		bool bEqual = false;
		//�����������
		for(i=1; i<bBufCardCount1; ++i){
			if(GetCardColor(bBufCard1[i]) == j){
				bEqual = true;
				k = GetCardValue(bBufCard1[i]);
				if(k == 5) ++bb[0];
				else if(k == 10) ++bb[1];
				else  ++bb[2];
			}else{
				bEqual = false;
				BYTE bMin = 255;
				for( k=0; k<3; ++k)
					if(bMin > bb[k]) bMin = bb[k];
				if(bMin){
					for(k=0; k<bMin; ++k){
						bPrivateCards[bPrivateCount++] = j|5;
						bPrivateCards[bPrivateCount++] = j|10;
						bPrivateCards[bPrivateCount++] = j|13;
					}
				}
				memset(bb, 0, sizeof(bb));
				j = GetCardColor(bBufCard1[i]);
				k = GetCardValue(bBufCard1[i]);
				if(k == 5) ++bb[0];
				else if(k == 10) ++bb[1];
				else  ++bb[2];
			}
		}
		if(bEqual){
			BYTE bMin = 255;
			for( k=0; k<3; ++k)
				if(bMin > bb[k]) bMin = bb[k];
			if(bMin){
				for(k=0; k<bMin; ++k){
					bPrivateCards[bPrivateCount++] = j|5;
					bPrivateCards[bPrivateCount++] = j|10;
					bPrivateCards[bPrivateCount++] = j|13;
				}
			}
		}
		_DeleteCards(bBufCard1, bBufCardCount1, bPrivateCards, bPrivateCount);
		if(bBufCardCount1>2){
			//��ֵ����
			std::sort(bBufCard1, bBufCard1+bBufCardCount1, _cmp2);
			//����
			memset(bb, 0, sizeof(bb));
			for(i=1; i<bBufCardCount1; ++i){
				k = GetCardValue(bBufCard1[i]);
				if(k == 5) ++bb[0];
				else if(k == 10) ++bb[1];
				else  ++bb[2];
			}

			BYTE bMin = 255;
			for( k=0; k<3; ++k)
				if(bMin > bb[k]) bMin = bb[k];
			if(bMin){
				k = bBufCard1[0];
				for(i=1; i<bBufCardCount1; ++i){
					//if(i>0) k = GetCardValue(bBufCard1[i-1]);
					if(GetCardValue(bBufCard1[i]) != GetCardValue(bBufCard1[i-1])){
						j = i;
						for(BYTE ii=0; ii<bMin; ++ii)
							bPrivateCards[bPrivateCount++] = bBufCard1[i+ii-1];
					}
				}
				for(BYTE ii=0; ii<bMin; ++ii)
					bPrivateCards[bPrivateCount++] = bBufCard1[j+ii];

				_DeleteCards(bBufCard1, bBufCardCount1, bPrivateCards+bPrivateCount-bMin*3, bMin*3);

			}

		}
	}
	bPrivateCards[bPrivateCount++] = 0xfe;
	//�����ը��..
	BYTE bSerialCount = 0;
	k = -1;
	for(i=1; i<bCardCount+1; ++i){
		if(GetCardLogicValue(bCards[i-1]) != GetCardLogicValue(bCards[i])){
			if((bSerialCount>=2) || (bSerialCount == 1 && GetCardColor(bCards[i-1])== 0x40)){
				++bSerialCount;
				memcpy(bPrivateCards+bPrivateCount, bCards+i-bSerialCount, bSerialCount);
				bPrivateCount += bSerialCount;
			}
			bSerialCount = 0;

		}else
			++bSerialCount;
		
	}
	if((bSerialCount>=2) || (bSerialCount == 1 && GetCardColor(bCards[i-1])== 0x40)){
		++bSerialCount;
		memcpy(bPrivateCards+bPrivateCount, bCards+i-bSerialCount, bSerialCount);
		bPrivateCount += bSerialCount;
	}
	
	_DeleteCards(bCards, bCardCount, bPrivateCards, bPrivateCount);

}
void	CGameLogic::_AddCards(BYTE bCards[], BYTE& bCardCount, BYTE bAdd[], BYTE bAddCount)
{
	BYTE i,j,k,pos=0xff;
	for(i=0; i<bAddCount; ++i)
		if(bAdd[i] == 0xfe)
		{pos = i; break;}
	//��������ը��..
	BYTE bKeep[20] = {0};
	BYTE bKeepCount = 0;
	if(pos != 0xff){
		k = GetCardLogicValue(bAdd[0]);
		bKeep[bKeepCount++] = k;
		for(i=pos+1; i<bAddCount; ++i){
			BYTE bValue = GetCardLogicValue(bAdd[i]);
			if(k != bValue){
				bKeep[bKeepCount++] = bValue;
			}
			bCards[bCardCount++] = bAdd[i];
		}
	}else
		 pos=bAddCount;
	
	for(i=1; i<pos; ++i){
		BYTE bValue = GetCardLogicValue(bAdd[i]);
		bool bIn = false;
		for(j=0; j<bKeepCount; ++j)
			if(bValue == bKeep[j]) 
			{
				bIn = true; break;
			}
		if(!bIn)
			bCards[bCardCount++] = bAdd[i];

	}
}
//�����ж�
bool CGameLogic::SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, tagOutCardResult & OutCardResult)
{
	//���ý��
	OutCardResult.cbCardCount=0;

	static BYTE _bBufCard[MAX_COUNT];
	static BYTE _bPrivateBufCard[MAX_COUNT*2];
	BYTE _bCardCount,_bPriCardCount;
	_bCardCount = bCardCount;
	memcpy(_bBufCard, bCardData, bCardCount*sizeof(BYTE));
	memset(_bPrivateBufCard, 0, sizeof(_bPrivateBufCard));
	_GetPrivateCard(_bBufCard, _bCardCount, _bPrivateBufCard, _bPriCardCount);

	BYTE bPreCount = bCardCount;
	if(_bCardCount != bCardCount && _bCardCount && bTurnOutType != CT_BOMB){
		//ȥ��ը��
		bCardData = _bBufCard;
		bCardCount = _bCardCount;
	}

	//��������
	switch (bTurnOutType)
	{
	case CT_ERROR:					//��������
		{
			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bCardData[bCardCount-1]);

			//�����ж�
			BYTE cbSameCount=1;
			for (BYTE i=1;i<bCardCount;i++)
			{
				if (GetCardLogicValue(bCardData[bCardCount-i-1])==bLogicValue) cbSameCount++;
				else break;
			}

			//��ɴ���
			if (cbSameCount>1)
			{
				OutCardResult.cbCardCount=cbSameCount;
				for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=bCardData[bCardCount-1-j];
				return true;
			}

			//���ƴ���
			OutCardResult.cbCardCount=1;
			OutCardResult.cbResultCard[0]=bCardData[bCardCount-1];

			return true;
		}
	case CT_SINGLE:					//��������
		{
			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//�Աȴ�С
			for (BYTE i=0;i<bCardCount;i++)
			{
				if (GetCardLogicValue(bCardData[bCardCount-i-1])>bLogicValue)
				{
					OutCardResult.cbCardCount=1;
					OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
					return true;
				}
			}

			break;
		}
	case CT_DOUBLE:					//��������
		{
			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//�Աȴ�С
			for (BYTE i=1;i<bCardCount;i++)
			{
				//��ȡ�˿�
				BYTE cbCardValue1=GetCardLogicValue(bCardData[bCardCount-i]);
				BYTE cbCardValue2=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//�����ж�
				if ((cbCardValue1==cbCardValue2)&&(cbCardValue1>bLogicValue))
				{
					OutCardResult.cbCardCount=2;
					OutCardResult.cbResultCard[0]=bCardData[bCardCount-i];
					OutCardResult.cbResultCard[1]=bCardData[bCardCount-i-1];
					return true;
				}
			}

			break;
		}
	case CT_SINGLE_LINE:			//��������
		{
			//�����ж�
			if (bCardCount<bTurnCardCount) break;

			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//��������
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//�����ж�
				if (bHandLogicValue>=15) break;
				if (bHandLogicValue<=bLogicValue) continue;

				//��������
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<bCardCount;j++)
				{
					if ((GetCardLogicValue(bCardData[j])+bLineCount)==bHandLogicValue) 
					{
						//��������
						OutCardResult.cbResultCard[bLineCount++]=bCardData[j];

						//����ж�
						if (bLineCount==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_DOUBLE_LINE:	//��������
		{
			//�����ж�
			if (bCardCount<bTurnCardCount) break;

			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//��������
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//�����ж�
				if (bHandLogicValue<=bLogicValue) continue;
				if ((bTurnCardCount>2)&&(bHandLogicValue>=15)) break;

				//��������
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<(bCardCount-1);j++)
				{
					if (((GetCardLogicValue(bCardData[j])+bLineCount)==bHandLogicValue)
						&&((GetCardLogicValue(bCardData[j+1])+bLineCount)==bHandLogicValue))
					{
						//��������
						OutCardResult.cbResultCard[bLineCount*2]=bCardData[j];
						OutCardResult.cbResultCard[(bLineCount++)*2+1]=bCardData[j+1];

						//����ж�
						if (bLineCount*2==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_BOMB:		//ը������
		{
			//�����ж�
			if (bCardCount<bTurnCardCount) break;

			//��ȡ��ֵ
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//����ը��
			for (BYTE i=bTurnCardCount-1;i<bCardCount;i++)
			{
				//��ȡ��ֵ
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//ը���ж�
				BYTE cbCount=1;
				for (BYTE j=bCardCount-i-2;j>=0;j--)
				{
					if (GetCardLogicValue(bCardData[j])==bHandLogicValue) cbCount++;
					else break;
					if (j==0) break;
				}

				//�����ж�
				if ((cbCount>bTurnCardCount)||((cbCount==bTurnCardCount)&&(bHandLogicValue>bLogicValue)))
				{
					//��ɴ���
					OutCardResult.cbCardCount=cbCount;
					for (BYTE j=0;j<cbCount;j++)
					{
						OutCardResult.cbResultCard[j]=bCardData[bCardCount-i-cbCount+j];
					}

					return true;
				}
			}

			break;
		}
	}
	//��֮ǰɾ����ը�����¼���..
	if(bCardCount != bPreCount){
		_AddCards(bCardData, bCardCount, _bPrivateBufCard, _bPriCardCount);
		SortCardList(bCardData, bCardCount, ST_ORDER);
	}

	//˫˳����
	if ((bTurnOutType==CT_DOUBLE_LINE)||(bTurnOutType==CT_DOUBLE_BOMB))
	{
		//��ȡ�ֲ�
		tagDistributing Distributing;
		GetDistributingInfo(bCardData,bCardCount,Distributing);

		//˫ը����
		if (bTurnOutType==CT_DOUBLE_BOMB)
		{
			//��������
			BYTE cbBombCount=0;
			BYTE cbBombIndex[2]={0xFF,0xFF};

			//��������
			BYTE cbTurnBombCount[2]={1,1};
			BYTE cbTurnBombValue[2]={GetCardValue(bTurnCardData[0]),GetCardValue(bTurnCardData[bTurnCardCount-1])};

			//�����ռ�
			for (int i=1;i<bTurnCardCount-1;i++)
			{
				BYTE cbTempValue=GetCardValue(bTurnCardData[i]);
				if (cbTempValue==cbTurnBombValue[0]) cbTurnBombCount[0]++;
				if (cbTempValue==cbTurnBombValue[1]) cbTurnBombCount[1]++;
			}

			//����˫ը
			for (BYTE i=0;i<CountArray(Distributing.cbDistributing);i++)
			{
				//����δ���
			}
		}

		//˫˳����
		if (bTurnOutType==CT_DOUBLE_LINE)
		{
			//��������
			BYTE cbBombCount=0;
			BYTE cbBombIndex[2]={0xFF,0xFF};

			//����˫ը
			for (BYTE i=0;i<CountArray(Distributing.cbDistributing);i++)
			{
				//˫˳����
				if (Distributing.cbDistributing[i][5]>=3)
				{
					cbBombIndex[cbBombCount++]=i;
					if (cbBombCount>=2) break;
					continue;
				}
			}

			//����ж�
			if (cbBombCount==2)
			{
				for (BYTE cbColorIndex=0;cbColorIndex<5;cbColorIndex++)
				{
					for (BYTE i=0;i<Distributing.cbDistributing[cbBombIndex[0]][cbColorIndex];i++)
					{
						BYTE cbCardTemp=MakeCardData(cbBombIndex[0],cbColorIndex);
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardTemp;
					}
				}

				for (BYTE cbColorIndex=0;cbColorIndex<5;cbColorIndex++)
				{
					for (BYTE i=0;i<Distributing.cbDistributing[cbBombIndex[1]][cbColorIndex];i++)
					{
						BYTE cbCardTemp=MakeCardData(cbBombIndex[1],cbColorIndex);
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardTemp;
					}
				}

				return true;
			}
		}
	}

	//����ը��
	if ((bTurnOutType<=CT_SINGLE_LINE)&&(bCardCount>=3))
	{
		//���ñ���
		ZeroMemory(&OutCardResult,sizeof(OutCardResult));

		//ը������
		for (BYTE i=2;i<bCardCount;i++)
		{
			//��ȡ��ֵ
			BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

			//ը���ж�
			BYTE bTempLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);
			for (BYTE j=1;j<3;j++)
			{
				if (GetCardLogicValue(bCardData[bCardCount+j-i-1])!=bTempLogicValue) break;
			}
			if (j!=3) continue;

			//��ɴ���
			OutCardResult.cbCardCount=3;
			OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
			OutCardResult.cbResultCard[1]=bCardData[bCardCount-i];
			OutCardResult.cbResultCard[2]=bCardData[bCardCount-i+1];

			return true;
		}
	}

	

	//����С��
	if (bTurnOutType<CT_KING_LITTET)
	{
		//���ñ���
		ZeroMemory(&OutCardResult,sizeof(OutCardResult));

		for (BYTE i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x41) 
			{
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=0x41;
			}
		}
		if (OutCardResult.cbCardCount==2) return true;
	}

	//��������
	if (bTurnOutType<CT_KING_BIG)
	{
		//���ñ���
		ZeroMemory(&OutCardResult,sizeof(OutCardResult));

		for (BYTE i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x42) 
			{
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=0x42;
			}
		}
		if (OutCardResult.cbCardCount==2) return true;
	}

	//��������..
	//���ñ���
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	for (BYTE i=0;i<bCardCount;i++)
	{
		if (GetCardValue(bCardData[i])==5) OutCardResult.cbResultCard[2]=bCardData[i];
		if (GetCardValue(bCardData[i])==10) OutCardResult.cbResultCard[1]=bCardData[i];
		if (GetCardValue(bCardData[i])==13) OutCardResult.cbResultCard[0]=bCardData[i];
	}
	if ((OutCardResult.cbResultCard[0]!=0)&&(OutCardResult.cbResultCard[1]!=0)&&(OutCardResult.cbResultCard[2]!=0))
	{
		OutCardResult.cbCardCount=3;
		return true;
	}
	//��ɫ����
	BYTE cbColorOrder=0x00;
	if (bTurnOutType==CT_CRURA_PURE)
	{
		BYTE cbColor=GetCardColor(bTurnCardData[0]);
		if (cbColor==0x00) cbColorOrder=0x03;
		else if (cbColor==0x10) cbColorOrder=0x01;
		else if (cbColor==0x20) cbColorOrder=0x04;
		else if (cbColor==0x30) cbColorOrder=0x02;
	}

	//��������
	for (BYTE j=0;j<4;j++)
	{
		//���ñ���
		ZeroMemory(&OutCardResult,sizeof(OutCardResult));

		//��ɫ�ж�
		BYTE cbSearchColor=j<<4;
		BYTE cbSearchOrder=0xFF;
		if (cbColorOrder!=0x00)
		{
			if (cbSearchColor==0x00) cbSearchOrder=0x03;
			else if (cbSearchColor==0x10) cbSearchOrder=0x01;
			else if (cbSearchColor==0x20) cbSearchOrder=0x04;
			else if (cbSearchColor==0x30) cbSearchOrder=0x02;
		}
		if (cbSearchOrder<=cbColorOrder) continue;

		for (BYTE i=0;i<bCardCount;i++)
		{
			if (GetCardColor(bCardData[i])!=cbSearchColor) continue;
			if (GetCardValue(bCardData[i])==5) OutCardResult.cbResultCard[2]=bCardData[i];
			if (GetCardValue(bCardData[i])==10) OutCardResult.cbResultCard[1]=bCardData[i];
			if (GetCardValue(bCardData[i])==13) OutCardResult.cbResultCard[0]=bCardData[i];
		}
		if ((OutCardResult.cbResultCard[0]!=0)&&(OutCardResult.cbResultCard[1]!=0)&&(OutCardResult.cbResultCard[2]!=0))
		{
			OutCardResult.cbCardCount=3;
			return true;
		}
	}

	//���ñ���
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	return false;
}

//////////////////////////////////////////////////////////////////////////
