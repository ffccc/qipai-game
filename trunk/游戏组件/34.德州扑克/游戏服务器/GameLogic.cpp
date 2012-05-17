#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardData[FULL_COUNT]=
{
	//0x05,0x36, 0x15,0x0d, 0x31,0x12,0x23,0x04,0x27,0x08,0x0B,0x0C,0x0D,	//���� A - K
	//0x11,0x12,0x13,0x14,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x01,0x3C,0x3D,	//���� A - K

	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
};

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//���Դ���
	//CopyMemory(cbCardBuffer,m_cbCardData,cbBufferCount);

	//����׼��
	BYTE bCardCount = CountArray(m_cbCardData);
	BYTE cbCardData[CountArray(m_cbCardData)];
	CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(bCardCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[bCardCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//��ȡ����
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	//����У��
	ASSERT(cbCardCount == MAX_CENTERCOUNT);
	if(cbCardCount !=MAX_CENTERCOUNT) return 0;

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

	//��Сͬ��˳
	if((bLineCard == false)&&(cbFirstValue == 14))
	{
		BYTE i=1;
		for (i=1;i<cbCardCount;i++)
		{
			BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);
			if ((cbFirstValue!=(cbLogicValue+i+8))) break;
		}
		if( i == cbCardCount)
			bLineCard =true;
	}

	//�ʼ�ͬ��˳
	if ((cbSameColor==true)&&(bLineCard==true)&&(GetCardLogicValue(cbCardData[1]) ==13 )) 
		return CT_KING_TONG_HUA_SHUN;

	//˳������
	if ((cbSameColor==false)&&(bLineCard==true)) 
		return CT_SHUN_ZI;

	//ͬ������
	if ((cbSameColor==true)&&(bLineCard==false)) 
		return CT_TONG_HUA;

	//ͬ��˳����
	if ((cbSameColor==true)&&(bLineCard==true))
		return CT_TONG_HUA_SHUN;

	//�˿˷���
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//�����ж�
	if (AnalyseResult.cbFourCount==1) 
		return CT_TIE_ZHI;
	if (AnalyseResult.cbLONGCount==2) 
		return CT_TWO_LONG;
	if ((AnalyseResult.cbLONGCount==1)&&(AnalyseResult.cbThreeCount==1))
		return CT_HU_LU;
	if ((AnalyseResult.cbThreeCount==1)&&(AnalyseResult.cbLONGCount==0))
		return CT_THREE_TIAO;
	if ((AnalyseResult.cbLONGCount==1)&&(AnalyseResult.cbSignedCount==3)) 
		return CT_ONE_LONG;

	return CT_SINGLE;
}

//�������
BYTE CGameLogic::FiveFromSeven(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbCenterCardData[],BYTE cbCenterCardCount,BYTE cbLastCardData[],BYTE cbLastCardCount)
{
	//��ʱ����
	BYTE cbTempCardData[MAX_COUNT+MAX_CENTERCOUNT],cbTempLastCardData[5];
	ZeroMemory(cbTempCardData,sizeof(cbTempCardData));
	ZeroMemory(cbTempLastCardData,sizeof(cbTempLastCardData));

	//��������
	CopyMemory(cbTempCardData,cbHandCardData,sizeof(BYTE)*MAX_COUNT);
	CopyMemory(&cbTempCardData[2],cbCenterCardData,sizeof(BYTE)*MAX_CENTERCOUNT);

	//�����˿�
	SortCardList(cbTempCardData,CountArray(cbTempCardData));

	CopyMemory(cbLastCardData,cbTempCardData,sizeof(BYTE)*MAX_CENTERCOUNT);
	BYTE cbCardKind = GetCardType(cbLastCardData,sizeof(BYTE)*MAX_CENTERCOUNT);
	BYTE cbTempCardKind = 0;

	//����㷨
	for (int i=0;i<3;i++)
	{
		for (int j= i+1;j<4;j++)
		{
			for (int k = j+1;k<5;k++)
			{
				for (int l =k+1;l<6;l++)
				{
					for (int m=l+1;m<7;m++)
					{
						//��ȡ����
						cbTempLastCardData[0]=cbTempCardData[i];
						cbTempLastCardData[1]=cbTempCardData[j];
						cbTempLastCardData[2]=cbTempCardData[k];
						cbTempLastCardData[3]=cbTempCardData[l];
						cbTempLastCardData[4]=cbTempCardData[m];

						//��ȡ����
						cbTempCardKind = GetCardType(cbTempLastCardData,CountArray(cbTempLastCardData));

						//���ʹ�С
						if(CompareCard(cbTempLastCardData,cbLastCardData,CountArray(cbTempLastCardData))==2)
						{
							cbCardKind = cbTempCardKind;
							CopyMemory(cbLastCardData,cbTempLastCardData,sizeof(BYTE)*CountArray(cbTempLastCardData));
						}
					}
				}
			}
		}
	}

	return cbCardKind;
}

//�������
bool CGameLogic::SelectMaxUser(BYTE bCardData[GAME_PLAYER][MAX_CENTERCOUNT],UserWinList &EndResult,const LONG lAddScore[])
{
	//��������
	ZeroMemory(&EndResult,sizeof(EndResult));

	//First����
	WORD wWinnerID;
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(bCardData[i][0]!=0)
		{
			wWinnerID=i;
			break;
		}
	}

	//����ȫ��
	if(i==GAME_PLAYER)return false;

	//��������û�
	WORD wTemp = wWinnerID;
	for(WORD i=1;i<GAME_PLAYER;i++)
	{
		WORD j=(i+wTemp)%GAME_PLAYER;
		if(bCardData[j][0]==0)continue;
		if(CompareCard(bCardData[j],bCardData[wWinnerID],MAX_CENTERCOUNT)>1)
		{
			wWinnerID=j;
		}
	}

	//������ͬ����
	EndResult.wWinerList[EndResult.bSameCount++] = wWinnerID;
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(i==wWinnerID || bCardData[i][0]==0)continue;
		if(CompareCard(bCardData[i],bCardData[wWinnerID],MAX_CENTERCOUNT)==FALSE)
		{
			EndResult.wWinerList[EndResult.bSameCount++] = i;
		}
	}

	//��С����
	if(EndResult.bSameCount>1 && lAddScore!=NULL)
	{
		int iSameCount=(int)EndResult.bSameCount;
		while((iSameCount--)>0)
		{
			LONG lTempSocre = lAddScore[EndResult.wWinerList[iSameCount]];
			for(int i=iSameCount-1;i>=0;i--)
			{
				ASSERT(lAddScore[EndResult.wWinerList[i]]>0);
				if(lTempSocre < lAddScore[EndResult.wWinerList[i]])
				{
					lTempSocre = lAddScore[EndResult.wWinerList[i]];
					WORD wTemp = EndResult.wWinerList[iSameCount];
					EndResult.wWinerList[iSameCount] = EndResult.wWinerList[i];
					EndResult.wWinerList[i] = wTemp;
				}
			}
		}
	}

	return true;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//ת����ֵ
	BYTE cbLogicValue[FULL_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) 
		cbLogicValue[i]=GetCardLogicValue(cbCardData[i]);	

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

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	return (bCardValue==1)?(bCardValue+13):bCardValue;
}

//�Ա��˿�
BYTE CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
	BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

	//�����ж�
	//��
	if(cbFirstType>cbNextType)
		return 2;

	//С
	if(cbFirstType<cbNextType)
		return 1;

	//������
	switch(cbFirstType)
	{
	case CT_SINGLE:			//����
		{
			//�Ա���ֵ
			BYTE i=0;
			for (i=0;i<cbCardCount;i++)
			{
				BYTE cbNextValue=GetCardLogicValue(cbNextData[i]);
				BYTE cbFirstValue=GetCardLogicValue(cbFirstData[i]);

				//��
				if(cbFirstValue > cbNextValue)
					return 2;
				//С
				else if(cbFirstValue <cbNextValue)
					return 1;
				//��
				else
					continue;
			}
			//ƽ
			if( i == cbCardCount)
				return 0;
			ASSERT(FALSE);
		}
	case CT_ONE_LONG:		//����
	case CT_TWO_LONG:		//����
	case CT_THREE_TIAO:		//����
	case CT_TIE_ZHI:		//��֧
	case CT_HU_LU:			//��«
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

				//�Ƚ�����
				if(cbFirstValue != cbNextValue)return (cbFirstValue > cbNextValue)?2:1;

				//�Ƚϵ���
				ASSERT(AnalyseResultFirst.cbSignedCount==1 && AnalyseResultNext.cbSignedCount==1);
				cbFirstValue = AnalyseResultFirst.cbSignedLogicVolue[0];
				cbNextValue = AnalyseResultNext.cbSignedLogicVolue[0];
				if(cbFirstValue != cbNextValue)return (cbFirstValue > cbNextValue)?2:1;
				else return 0;
			}

			//������ֵ
			if (AnalyseResultFirst.cbThreeCount>0)
			{
				BYTE cbNextValue=AnalyseResultNext.cbThreeLogicVolue[0];
				BYTE cbFirstValue=AnalyseResultFirst.cbThreeLogicVolue[0];

				//�Ƚ�����
				if(cbFirstValue != cbNextValue)return (cbFirstValue > cbNextValue)?2:1;

				//��«����
				if(CT_HU_LU == cbFirstType)
				{
					//�Ƚ϶���
					ASSERT(AnalyseResultFirst.cbLONGCount==1 && AnalyseResultNext.cbLONGCount==1);
					cbFirstValue = AnalyseResultFirst.cbLONGLogicVolue[0];
					cbNextValue = AnalyseResultNext.cbLONGLogicVolue[0];
					if(cbFirstValue != cbNextValue)return (cbFirstValue > cbNextValue)?2:1;
					else return 0;
				}
				else //��������
				{
					//�Ƚϵ���
					ASSERT(AnalyseResultFirst.cbSignedCount==2 && AnalyseResultNext.cbSignedCount==2);

					//ɢ����ֵ
					BYTE i=0;
					for (i=0;i<AnalyseResultFirst.cbSignedCount;i++)
					{
						BYTE cbNextValue=AnalyseResultNext.cbSignedLogicVolue[i];
						BYTE cbFirstValue=AnalyseResultFirst.cbSignedLogicVolue[i];
						//��
						if(cbFirstValue > cbNextValue)
							return 2;
						//С
						else if(cbFirstValue <cbNextValue)
							return 1;
						//��
						else continue;
					}
					if( i == AnalyseResultFirst.cbSignedCount)
						return 0;
					ASSERT(FALSE);
				}
			}

			//������ֵ
			BYTE i=0;
			for ( i=0;i<AnalyseResultFirst.cbLONGCount;i++)
			{
				BYTE cbNextValue=AnalyseResultNext.cbLONGLogicVolue[i];
				BYTE cbFirstValue=AnalyseResultFirst.cbLONGLogicVolue[i];
				//��
				if(cbFirstValue > cbNextValue)
					return 2;
				//С
				else if(cbFirstValue <cbNextValue)
					return 1;
				//ƽ
				else
					continue;
			}

			//�Ƚϵ���
			ASSERT( i == AnalyseResultFirst.cbLONGCount);
			{
				ASSERT(AnalyseResultFirst.cbSignedCount==AnalyseResultNext.cbSignedCount
					&& AnalyseResultNext.cbSignedCount>0);
				//ɢ����ֵ
				for (i=0;i<AnalyseResultFirst.cbSignedCount;i++)
				{
					BYTE cbNextValue=AnalyseResultNext.cbSignedLogicVolue[i];
					BYTE cbFirstValue=AnalyseResultFirst.cbSignedLogicVolue[i];
					//��
					if(cbFirstValue > cbNextValue)
						return 2;
					//С
					else if(cbFirstValue <cbNextValue)
						return 1;
					//��
					else continue;
				}
				//ƽ
				if( i == AnalyseResultFirst.cbSignedCount)
					return 0;
			}
			break;
		}
	case CT_SHUN_ZI:		//˳��
	case CT_TONG_HUA_SHUN:	//ͬ��˳
		{
			//��ֵ�ж�
			BYTE cbNextValue=GetCardLogicValue(cbNextData[0]);
			BYTE cbFirstValue=GetCardLogicValue(cbFirstData[0]);

			bool bFirstmin= (cbFirstValue ==(GetCardLogicValue(cbFirstData[1])+9));
			bool bNextmin= (cbNextValue ==(GetCardLogicValue(cbNextData[1])+9));

			//��С˳��
			if ((bFirstmin==true)&&(bNextmin == false))
			{
				return 1;
			}
			//��С˳��
			else if ((bFirstmin==false)&&(bNextmin == true))
			{
				return 2;
			}
			//��ͬ˳��
			else
			{
				//ƽ
				if(cbFirstValue == cbNextValue)return 0;		
				return (cbFirstValue > cbNextValue)?2:1;		
			}
		}
	case CT_TONG_HUA:		//ͬ��
		{	
			//ɢ����ֵ
			for (BYTE i=0;i<cbCardCount;i++)
			{
				BYTE cbNextValue=GetCardLogicValue(cbNextData[i]);
				BYTE cbFirstValue=GetCardLogicValue(cbFirstData[i]);

				if(cbFirstValue == cbNextValue)continue;
				return (cbFirstValue > cbNextValue)?2:1;
			}
			//ƽ
			if( i == cbCardCount) return 0;
		}
	}

	return  0;
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
				AnalyseResult.cbLONGLogicVolue[AnalyseResult.cbLONGCount]=cbLogicValue;
				CopyMemory(&AnalyseResult.cbLONGCardData[(AnalyseResult.cbLONGCount++)*cbSameCount],cbSameCardData,cbSameCount);
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
