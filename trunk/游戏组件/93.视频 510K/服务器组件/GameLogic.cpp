#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//��������
const BYTE cbIndexCount=5;

//�˿�����
const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x4E,0x4F,
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
	//�߼�����
	m_cbMainValue=2;

	return;
}

//��������
CGameLogic::~CGameLogic()
{
}

//����˿�
BYTE CGameLogic::GetRandomCard(void)
{
	size_t cbIndex = rand()%(sizeof(m_cbCardData)) ;
	return m_cbCardData[cbIndex] ;
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
	case 2:	//����
		{
			if (GetCardValue(cbCardData[0])==GetCardValue(cbCardData[1])) 
			{
				if(GetCardValue(cbCardData[0])!=0x0E && GetCardValue(cbCardData[0])!=0x0F)
					return CT_DOUBLE;		//һ���
				else if(GetCardValue(cbCardData[0])==0x0E)
					return CT_KING_ST;		//��С��
				else if(GetCardValue(cbCardData[0])==0x0F)
					return CT_KING_SK;		//������
			}else if((GetCardValue(cbCardData[0])==0x0E && GetCardValue(cbCardData[1])==0x0F)
				|| (GetCardValue(cbCardData[0])==0x0F && GetCardValue(cbCardData[1])==0x0E))
				return CT_KING_DC;			//��С��
			else
				return CT_ERROR;
		}
	case 3:	//����
		{
			if (GetCardValue(cbCardData[0])==GetCardValue(cbCardData[2]))  return CT_BOMB_3;
			if (GetCardValue(cbCardData[0])==13&&GetCardValue(cbCardData[1])==10&&GetCardValue(cbCardData[2])==5)
			{
				if(GetCardColor(cbCardData[0])==GetCardColor(cbCardData[1])&&GetCardColor(cbCardData[0])==GetCardColor(cbCardData[2]))
					return CT_510K_SC;	//ͬɫ510K
				return CT_510K_DC;		//��ͨ510K
			}
			if ((GetCardValue(cbCardData[0])==0x0E || GetCardValue(cbCardData[0])==0x0F) &&
				(GetCardValue(cbCardData[1])==0x0E || GetCardValue(cbCardData[1])==0x0F) &&
				(GetCardValue(cbCardData[2])==0x0E || GetCardValue(cbCardData[2])==0x0F)
				)
				return CT_KING_THREE;	//3��
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//ը������
	if ((cbCardCount==4)&&(cbCardData[0]==0x4F)&&(cbCardData[3]==0x4E)) return CT_KING_FOUR;//4��
	if ((cbCardCount==4)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_4;
	if ((cbCardCount==5)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_5;
	if ((cbCardCount==6)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_6;
	if ((cbCardCount==7)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_7;
	if ((cbCardCount==8)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_8;

	//˳������
	if ((cbCardCount>=3)&&(AnalyseResult.cbBlockCount[0]==cbCardCount))
	{
		//�˿�����
		BYTE cbSignedCount=AnalyseResult.cbBlockCount[0];
		BYTE cbCardColor=GetCardColor(AnalyseResult.cbCardData[0],cbSignedCount);
		if ( IsStructureLink(AnalyseResult.cbCardData[0],cbSignedCount,1) )
			return CT_SINGLE_LINK;		
	}

	//��������
	if ((cbCardCount>=6)&&((AnalyseResult.cbBlockCount[1]*2)==cbCardCount))
	{
		BYTE cbDoubleCount=AnalyseResult.cbBlockCount[1]*2;
		if (IsStructureLink(AnalyseResult.cbCardData[1],cbDoubleCount,2)) 
			return CT_DOUBLE_LINK;
	}
	return CT_ERROR;
}

//��ȡ��ɫ
BYTE CGameLogic::GetCardColor(const BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч�����
	ASSERT(cbCardCount>0);
	if (cbCardCount==0) return 0xF0;

	//���ƻ�ɫ
	BYTE cbCardColor=GetCardColor(cbCardData[0]);

	//��ɫ�ж�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (GetCardColor(cbCardData[i])!=cbCardColor) return 0xF0;
	}

	return cbCardColor;
}

//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//�������
	if (cbCardCount==0) return;
	if (cbSortType==ST_CUSTOM) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++)
	{
		switch (cbSortType)
		{
		case ST_COUNT:	//��Ŀ����
		case ST_ORDER:	//�ȼ�����
			{
				cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
				break;
			}
		case ST_VALUE:	//��ֵ����
			{
				cbSortValue[i]=GetCardValue(cbCardData[i]);
				break;
			}
		case ST_COLOR:	//��ɫ����
			{
				cbSortValue[i]=GetCardColor(cbCardData[i])+GetCardLogicValue(cbCardData[i]);
				break;
			}
		}
	}

	//�������
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//���ñ�־
				bSorted=false;

				//�˿�����
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;

				//����Ȩλ
				cbSwitchData=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	//��Ŀ����
	if (cbSortType==ST_COUNT)
	{
		//��������
		BYTE cbCardIndex=0;

		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

		//��ȡ�˿�
		for (BYTE i=0;i<CountArray(AnalyseResult.cbBlockCount);i++)
		{
			//�����˿�
			BYTE cbIndex=CountArray(AnalyseResult.cbBlockCount)-i-1;
			CopyMemory(&cbCardData[cbCardIndex],AnalyseResult.cbCardData[cbIndex],AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE));

			//��������
			cbCardIndex+=AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE);
		}
	}

	return;
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
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

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardValue=GetCardValue(cbCardData);

	//�����˿�
	//if (cbCardValue==m_cbMainValue) return 15;

	//�߼���ֵ
	switch (cbCardValue)
	{
	case 0x01: { return 14; }
	case 0x02: { return 15;	}
	case 0x0E: { return 16; }
	case 0x0F: { return 17; }
	}

	//ת����ֵ
	return cbCardValue;
}
//�Ա��˿�
bool CGameLogic::CompareCard( const BYTE cbFirstCard,const BYTE cbNextCard )
{
	return GetCardLogicValue(cbNextCard)>GetCardLogicValue(cbFirstCard);
}
//������Ʒ����˿�
VOID CGameLogic::AnalyseMagicData( const BYTE cbHandCardData[],BYTE cbHandCardCount,tagMagicResult &MagicResult )
{
	//��0
	ZeroMemory( &MagicResult,sizeof(MagicResult) );
	//��֤
	if( cbHandCardCount <= 0 ) return ;

	for( BYTE i = 0; i < cbHandCardCount; i++ )
	{
		BYTE cbCardValue = GetCardValue(cbHandCardData[i]);
		BYTE cbCardColor = GetCardColor(cbHandCardData[i]);
		if( cbCardValue==m_cbMainValue && cbCardColor==0x20 )
			MagicResult.cbMagicCard[MagicResult.cbMagicCardCount++] = cbHandCardData[i];
		else
			MagicResult.cbNormalCard[MagicResult.cbNormalCardCount++] = cbHandCardData[i];
	}
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//�����ж�
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

	//ը������һ�����ڵ��ơ����Ӻ͵���
	if(cbNextType>=CT_BOMB_3 && cbFirstType<=CT_SINGLE_LINK) return true;

	//�ڶ�������
	if(cbNextType>=CT_BOMB_3 && cbFirstType!=CT_DOUBLE_LINK)
	{
		if(cbNextType!=cbFirstType) //��ͬ����ֱ�ӱȽ�
			return cbNextType>cbFirstType;
		else if(cbNextType==CT_510K_SC) //������ͬ���Ǵ�510Kʱ����Ҫ�Ƚϻ�ɫ
			return GetCardColor(cbNextCard[0])>GetCardColor(cbFirstCard[0]);
		else
			return GetCardLogicValue(cbNextCard[cbNextCount-1])>GetCardLogicValue(cbFirstCard[cbFirstCount-1]);
	}

	//��һ�����͵Ĳ�ͬ���ͱȽ�
	if (cbFirstType!=cbNextType)
	{
		if (cbFirstType==CT_DOUBLE_LINK)
		{
			if (cbNextType>=CT_BOMB_4)
				return true;
			else
				return false;
		}
		//���⴦�� ��Ŀ��ͬ���Ƚ�

		if(cbNextCount!=cbFirstCount) return false;
		else return cbNextType>cbFirstType;
	}

	//��ͬ����
	switch (cbFirstType)
	{
	case CT_SINGLE:				//��������
	case CT_DOUBLE:				//��������
	{
			//��������
			BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
			BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);

			return cbConsultNext>cbConsultFirst;
		}
	case CT_SINGLE_LINK:		//��������
	case CT_DOUBLE_LINK:		//��������
		{
			if(cbNextCount!=cbFirstCount) return false;
			return CompareCardByValue(cbFirstCard,cbNextCard,cbFirstCount,cbNextCount);
		}
	}

	//�������
	ASSERT(FALSE);
	
	return false;
}

//����˿�
bool CGameLogic::MagicCardData(BYTE cbCardData[], BYTE cbCardCount, BYTE cbResultCard[MAX_COUNT], BYTE cbCardType)
{
	//���任
	//�����˿�
	CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));

	return false;

	//��������
	BYTE cbNormalCard[MAX_COUNT];
	BYTE cbMagicCardCount=0,cbNormalCardCount=0;

	//���׼��
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//�˿�����
		BYTE cbCardValue=GetCardValue(cbCardData[i]);
		BYTE cbCardColor=GetCardColor(cbCardData[i]);

		//���ͳ��
		if ((cbCardValue==m_cbMainValue)&&(cbCardColor==0x20)) cbMagicCardCount++;
		else cbNormalCard[cbNormalCardCount++]=cbCardData[i];
	}

	//ʧ���ж�
	if ((cbMagicCardCount==0)||(cbNormalCardCount==0))
	{
		CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));
		return false;
	}

	//��ȡ��ֵ
	BYTE cbLogicValue=GetCardLogicValue(cbNormalCard[0]);

	//ͬ�Ʊ��
	if (cbLogicValue<=15)
	{
		for (BYTE i=1;i<cbNormalCardCount;i++)
		{
			if (GetCardLogicValue(cbNormalCard[i])!=cbLogicValue) break;
		}
		if (i==cbNormalCardCount)
		{
			//���ý��
			for (BYTE j=0;j<cbMagicCardCount;j++) cbResultCard[j]=cbNormalCard[0];
			for (BYTE j=0;j<cbNormalCardCount;j++) cbResultCard[j+cbMagicCardCount]=cbNormalCard[j];

			return true;
		}
	}

	//�˿˷���
	BYTE cbMaxSameCount=1,cbSameCount=1;
	for (BYTE i=1;i<cbNormalCardCount;i++)
	{
		//��ȡ�˿�
		BYTE cbCardValueTemp=GetCardLogicValue(cbNormalCard[i]);
		if (cbCardValueTemp==cbLogicValue) cbSameCount++;

		//�������
		if ((cbCardValueTemp!=cbLogicValue)||(i==(cbNormalCardCount-1)))
		{
			cbMaxSameCount=__max(cbSameCount,cbMaxSameCount);
		}

		//�ָ�����
		if ((cbCardValueTemp!=cbLogicValue)&&(i!=(cbNormalCardCount-1)))
		{
			cbSameCount=1;
			cbLogicValue=cbCardValueTemp;
		}
	}

	//��������
	if ((cbCardCount==5)&&(cbMaxSameCount<=3))
	{
		//��������
		BYTE cbHeadCount=1;
		BYTE cbTailCount=1;

		//��������
		BYTE cbHeadValue=GetCardValue(cbNormalCard[0]);
		BYTE cbTailValue=GetCardValue(cbNormalCard[cbNormalCardCount-1]);

		//����ж�
		for (BYTE i=1;i<cbNormalCardCount-1;i++)
		{
			//��ȡ��ֵ
			BYTE cbCardValue=GetCardValue(cbNormalCard[i]);

			//�˿��ж�
			if (cbCardValue==cbHeadValue) 
			{
				cbHeadCount++;
				continue;
			}

			//�˿��ж�
			if (cbCardValue==cbTailValue) 
			{
				cbTailCount++;
				continue;
			}

			break;
		}

		//�ɹ��ж�
		if (i==(cbNormalCardCount-1))
		{
			//�仯��ֵ
			BYTE cbMagicValue=cbHeadValue;
			if (cbHeadCount==3) cbMagicValue=cbTailValue;

			//�����˿�
			CopyMemory(cbResultCard,cbNormalCard,cbHeadCount*sizeof(BYTE));
			CopyMemory(&cbResultCard[cbCardCount-cbTailCount],&cbNormalCard[cbHeadCount],cbTailCount*sizeof(BYTE));

			//����˿�
			for (BYTE j=0;j<cbMagicCardCount;j++)
			{
				cbResultCard[cbHeadCount+j]=MakeCardData(cbMagicValue-1,0);
			}

			return true;
		}
	}

	//�����ж�
	bool bMagicLink=false;
	if ((cbMaxSameCount==1)&&(cbCardCount==5)) bMagicLink=true;
	if (((cbMaxSameCount==2)||(cbMaxSameCount==3))&&(cbCardCount==6)) bMagicLink=true;

	//���Ʊ��
	if (bMagicLink==true)
	{
		//�����ֲ�
		tagDistributing Distributing;
		AnalysebDistributing(cbNormalCard,cbNormalCardCount,Distributing);

		//������
		for (BYTE cbTimes=0;cbTimes<2;cbTimes++)
		{
			//��������
			BYTE cbFillCount=0;
			BYTE cbLeaveCount=cbNormalCardCount;
			BYTE cbUseableCount=cbMagicCardCount;

			//���״̬
			BYTE cbFillStatus[13];
			ZeroMemory(cbFillStatus,sizeof(cbFillStatus));

			//����˿�
			for (BYTE i=0;i<14;i++)
			{
				//��������
				BYTE cbIndex=((cbTimes==1)?(13-i)%13:i)%13;

				//����ж�
				if ((cbLeaveCount!=cbNormalCardCount)||(Distributing.cbDistributing[cbIndex][cbIndexCount]!=0))
				{
					//ʧ���ж�
					if ((Distributing.cbDistributing[cbIndex][cbIndexCount]+cbUseableCount)<cbMaxSameCount) break;

					//��������
					BYTE cbTurnFillCount=0;

					//�����˿�
					for (BYTE j=0;j<4;j++)
					{
						for (BYTE k=0;k<Distributing.cbDistributing[cbIndex][j];k++)
						{
							cbLeaveCount--;
							cbTurnFillCount++;
							cbResultCard[cbFillCount++]=MakeCardData(cbIndex,j);
						}
					}

					//�����
					for (BYTE i=cbTurnFillCount;i<cbMaxSameCount;i++)
					{
						cbUseableCount--;
						cbResultCard[cbFillCount++]=MakeCardData(cbIndex,GetCardColor(cbNormalCard[0])>>4);
					}

					//���ñ���
					cbFillStatus[cbIndex]=cbMaxSameCount;
				}
			}

			//�ɹ��ж�
			if ((cbUseableCount==0)&&(cbLeaveCount==0)) 
			{
				SortCardList(cbResultCard,cbCardCount,ST_ORDER);
				return true;
			}
		}
	}

	//�����˿�
	CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));

	return false;
}

//�����˿�
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	//for (BYTE i=0;i<cbCardCount;i++)
	for (BYTE i=0;i<cbCardCount;)
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
		BYTE cbIndex=AnalyseResult.cbBlockCount[cbSameCount-1]++;
		for (BYTE j=0;j<cbSameCount;j++) 
			AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];

		//��������
		i+=cbSameCount;
		//i+=cbSameCount-1;
	}

	return;
}

//�����ֲ�
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//���ñ���
	ZeroMemory(&Distributing,sizeof(Distributing));

	//���ñ���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbCardData[i]==0) continue;

		//��ȡ����
		BYTE cbCardColor=GetCardColor(cbCardData[i]);
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//�ֲ���Ϣ
		Distributing.cbCardCount++;//�����Ƶ��ܸ���
		Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;		//ĳһ��ֵ�Ƶ��ܸ���
		Distributing.cbDistributing[cbCardValue-1][cbCardColor>>4]++;	//ĳһ��ֵ�ƣ�ĳ����ɫ���ܸ���
	}

	return;
}
//��������
bool CGameLogic::SearchDong( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult )
{
	BYTE tempCardData[MAX_COUNT];ZeroMemory(tempCardData,sizeof(tempCardData));
	CopyMemory(tempCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	SortCardList(tempCardData,cbHandCardCount,ST_ORDER);

	for(BYTE i=cbHandCardCount;i>=2;i--)
	{
		if(tempCardData[i]==tempCardData[i-2])
		{
			if(GetCardLogicValue(tempCardData[i])>GetCardLogicValue(cbReferCard))
			{
				OutCardResult.cbCardCount=3;
				OutCardResult.cbResultCard[0]=tempCardData[i-2];
				OutCardResult.cbResultCard[1]=tempCardData[i-1];
				OutCardResult.cbResultCard[2]=tempCardData[i-0];
				return true;
			}
		}
	}
	return false;
}


bool CGameLogic::SearchPromptCard(const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,
								  tagOutCardResult& OutCardResult,BYTE & cbPromptType,const tagAnalyseResult & AnalyseResult)
{
	BYTE cbType = cbPromptType,cbSameCount = 0;
	int i;
	switch(cbType)
	{
		//��������
		case CT_SINGLE:
			{
				BYTE cbTurnCardLogicValue = GetCardLogicValue(cbTurnCardData[0]);
				for(i=cbHandCardCount-1;i>=0;i--)
				{
					if(GetCardLogicValue(cbHandCardData[i])>cbTurnCardLogicValue)
					{
						OutCardResult.cbCardCount = 1;
						OutCardResult.cbResultCard[0] = cbHandCardData[i];
						return true;
					}
				}
				cbPromptType = CT_BOMB_3;	//��3ը����ʼ����
			}
			break;
		case CT_DOUBLE:
			{
				if(AnalyseResult.cbBlockCount[1]>0)
				{
					for(BYTE j=0;j<AnalyseResult.cbBlockCount[1];j++)
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[1][2*j],2,2))
						{
							OutCardResult.cbCardCount = 2;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[1][2*j],sizeof(BYTE)*2);
							return true;
						}
				}
				cbPromptType = CT_BOMB_3;
			}
			break;
		case CT_SINGLE_LINK:
			{
				BYTE cbRefCard = cbTurnCardData[0];
				if( SearchLinkCard(cbHandCardData,cbHandCardCount,cbRefCard,CT_SINGLE_LINK,OutCardResult,cbTurnCardCount))
				{
					return true;
				}
				cbPromptType = CT_BOMB_3;
			}
			break;
		case CT_DOUBLE_LINK:
			{
				BYTE cbRefCard = cbTurnCardData[0];
				if( SearchLinkCard(cbHandCardData,cbHandCardCount,cbRefCard,CT_DOUBLE_LINK,OutCardResult,cbTurnCardCount))
				{
					return true;
				}
				cbPromptType = CT_BOMB_4;
			}
			break;
		case CT_510K_DC:
		case CT_510K_SC:
			{
				if( Search510K(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount,cbType,OutCardResult) )
				{
				//		ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
						return true;
				}
				cbPromptType = cbType+1;
			}
			break;
		//��С������
		case CT_KING_DC:	//��С��
		case CT_KING_ST:	//��С��
		case CT_KING_SK:	//������
		case CT_KING_THREE:	//3��
		case CT_KING_FOUR:	//4��
			if(SearchKings(cbHandCardData,cbHandCardCount,OutCardResult,cbType))
				return true;
			cbPromptType = cbType+1;
			break;
		//3��8ը������
		case CT_BOMB_3:
			cbSameCount = 3;
			goto LABLE_BOMB3_8;
		case CT_BOMB_4:
			cbSameCount = 4;
			goto LABLE_BOMB3_8;
		case CT_BOMB_5:
			cbSameCount = 5;
			goto LABLE_BOMB3_8;
		case CT_BOMB_6:
			cbSameCount = 6;
			goto LABLE_BOMB3_8;
		case CT_BOMB_7:
			cbSameCount = 7;
			goto LABLE_BOMB3_8;
		case CT_BOMB_8:
			cbSameCount = 8;
			{
LABLE_BOMB3_8:
				BYTE cbIndex=cbSameCount-1;
				if( AnalyseResult.cbBlockCount[cbIndex] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[cbIndex]-1; j >= 0; j-- )
					{
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[cbIndex][j*cbSameCount],cbTurnCardCount,cbSameCount) )
						{
							OutCardResult.cbCardCount = cbSameCount;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[cbIndex][j*cbSameCount],sizeof(BYTE)*cbSameCount);
							ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
							return true;
						}
					}
				}
				cbPromptType = cbType+1;//�ڸ��߼��������в���
			}
			break;
	}
	if(cbPromptType>CT_BOMB_8)
		cbPromptType = CT_ERROR;
	return false;
}

//��������
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, BYTE& cbPromptType,tagOutCardResult & OutCardResult)
{
	//���ý��
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));
	bool bOK=false;
	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//�����˿�
	SortCardList(cbCardData,cbCardCount,ST_ORDER);

	//��ȡ����
	BYTE cbTurnOutType = GetCardType(cbTurnCardData,cbTurnCardCount);
	cbPromptType  = cbTurnOutType;
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	do{

		//bOK=���ҳ�����ʾ(const BYTE cbHandCardData[],BYTE cbHandCardCout,const BYTE cbTurnCardData[]
		//BYTE cbTurnCardCout,tagOutCardResult& OutCardResult BYTE & cbPromptType);
		bOK=SearchPromptCard(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult,cbPromptType,AnalyseResult);
	}while(!bOK && cbPromptType !=CT_ERROR);

	return bOK;
	//���Ʒ���
	/*switch (cbTurnOutType)
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
		{
			//BYTE byTurnCardLogicValue = GetCardLogicValue(cbTurnCardData[0]);
			//for( BYTE i = 0; i < cbCardCount; i++ )
			//{
			//	if( GetCardLogicValue(cbCardData[cbCardCount-i-1]) > byTurnCardLogicValue )
			//	{
			//		OutCardResult.cbCardCount = 1;
			//		OutCardResult.cbResultCard[0] = cbCardData[cbCardCount-i-1];
			//		return true;
			//	}
			//}
			//�ӵ���������
			//for( BYTE i = 0; i < CountArray(AnalyseResult.cbBlockCount); i++ )
			//{
			//	if( AnalyseResult.cbBlockCount[i] > 0 )
			//	{
			//		for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0 ; j-- )
			//		{
			//			if( CompareCard(cbTurnCardData[0],AnalyseResult.cbCardData[i][(i+1)*j]) )
			//			{
			//				OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = AnalyseResult.cbCardData[i][(i+1)*j];
			//				return true;
			//			}
			//		}
			//	}
			//}
			//return false;
		}
	case CT_DOUBLE:					//����
		{
			for( BYTE i = cbTurnCardCount-1; i < 3; i++ )
			{
				if( AnalyseResult.cbBlockCount[i] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0; j-- )
					{
						//�Ƚ��˿�
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[i][(i+1)*j],cbTurnCardCount,cbTurnCardCount) )
						{
							//��������
							OutCardResult.cbCardCount = cbTurnCardCount;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][(i+1)*j],sizeof(BYTE)*(cbTurnCardCount));
							return true;
						}
					}
				}
			}
		}
	case CT_SINGLE_LINK:			//��˳
		{
			//ֻ�г������Ͷ�Ӧ,������
			if(cbTurnOutType == CT_SINGLE_LINK)
			{
				//Ѱ�ҶԱ���
				BYTE byReferCard = cbTurnCardData[0];
				if( GetCardValue(cbTurnCardData[0])==m_cbMainValue )
				{
					BYTE nCellCount = 1;
					if( GetCardValue(cbTurnCardData[nCellCount])<6 && GetCardValue(cbTurnCardData[cbTurnCardCount])<6 )
					{
						byReferCard = IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount)?
							MakeCardData(5,0):MakeCardData(4,0);
					}
					else if( IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount) )
					{
						byReferCard = GetCardValue(cbTurnCardData[nCellCount])==1?
							cbTurnCardData[nCellCount]:MakeCardData(GetCardValue(cbTurnCardData[nCellCount])%13,0);
					}
					else byReferCard = cbTurnCardData[nCellCount];
				}
				if( SearchLinkCard(cbCardData,cbCardCount,byReferCard,cbTurnOutType,OutCardResult,cbTurnCardCount) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}
			}
		}
	case CT_THREE:					//������
		{
			for( BYTE i = cbTurnCardCount-1; i < 3; i++ )
			{
				if( AnalyseResult.cbBlockCount[i] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0; j-- )
					{
						//�Ƚ��˿�
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[i][(i+1)*j],cbTurnCardCount,cbTurnCardCount) )
						{
							//��������
							OutCardResult.cbCardCount = cbTurnCardCount;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][(i+1)*j],sizeof(BYTE)*(cbTurnCardCount));
							return true;
						}
					}
				}
			}
		}
	
	case CT_DOUBLE_LINK:			//˫��
		{
			//ֻ�г������Ͷ�Ӧ,������
			if( cbTurnOutType <= CT_DOUBLE_LINK )
			{
				//Ѱ�ҶԱ���
				BYTE byReferCard = cbTurnCardData[0];
				if(cbTurnOutType<CT_DOUBLE_LINK ) byReferCard=0;
				if( GetCardValue(cbTurnCardData[0])==m_cbMainValue )
				{
					BYTE nCellCount = 2;
					if( GetCardValue(cbTurnCardData[nCellCount])<6 && GetCardValue(cbTurnCardData[cbTurnCardCount])<6 )
					{
						byReferCard = IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount)?
							MakeCardData(5,0):MakeCardData(4,0);
					}
					else if( IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount) )
					{
						byReferCard = GetCardValue(cbTurnCardData[nCellCount])==1?
							cbTurnCardData[nCellCount]:MakeCardData(GetCardValue(cbTurnCardData[nCellCount])%13,0);
					}
					else byReferCard = cbTurnCardData[nCellCount];
				}
				if( SearchLinkCard(cbCardData,cbCardCount,byReferCard,cbTurnOutType,OutCardResult,cbTurnCardCount) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}
			}
		}
	
	case CT_BOMB_4:					//ը��
		{
			//ȷ������
			BYTE bSameCount = 0;
			bSameCount = 4;

			for( BYTE i = bSameCount-1; i < CountArray(AnalyseResult.cbBlockCount); i++ )
			{
				if( AnalyseResult.cbBlockCount[i] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0; j-- )
					{
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[i][j*(i+1)],cbTurnCardCount,i+1) )
						{
							OutCardResult.cbCardCount = i+1;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][j*(i+1)],sizeof(BYTE)*(i+1));
							ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
							return true;
						}
					}
				}
			}
		}
	case CT_510K_DC:
	case CT_510K_SC:
		{
			if( Search510K(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult) )
			{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
			}
			
		}
	case CT_BOMB_5:
		{
			//ȷ������
			BYTE bSameCount = 0;
			bSameCount = cbTurnOutType>=CT_BOMB_4?cbTurnCardCount:4;

			for( BYTE i = bSameCount-1; i < CountArray(AnalyseResult.cbBlockCount); i++ )
			{
				if( AnalyseResult.cbBlockCount[i] > 0 )
				{
					for( int j = AnalyseResult.cbBlockCount[i]-1; j >= 0; j-- )
					{
						if( CompareCard(cbTurnCardData,&AnalyseResult.cbCardData[i][j*(i+1)],cbTurnCardCount,i+1) )
						{
							OutCardResult.cbCardCount = i+1;
							CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[i][j*(i+1)],sizeof(BYTE)*(i+1));
							ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
							return true;
						}
					}
				}
				//���5����������,������ͬ��˳,������6��
				if( i == 4 )
				{
					if( SearchTongHuaShun(cbCardData,cbCardCount,0,	OutCardResult,cbTurnCardData) )
					{
						ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
				}
			}
		}
	case CT_BOMB_6:
	case CT_BOMB_7:
	case CT_BOMB_8:
		{
			if(cbTurnOutType<=CT_BOMB_6)
			{
				for(BYTE i=5;i<12;i++)
				{
					if(AnalyseResult.cbCardData[i][0]!=0)
					{
						if(CompareCard(cbTurnCardData,AnalyseResult.cbCardData[i],cbTurnCardCount,i)==true)
						{
							OutCardResult.cbCardCount=i;
							CopyMemory(OutCardResult.cbResultCard,AnalyseResult.cbCardData[i],sizeof(BYTE)*i);
							return true;
						}
					}
				}
			}
		}

	}*/
	//return false;
}

//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	return (cbColorIndex<<4)|(cbValueIndex+1);
}

//�Ƿ�����
bool CGameLogic::IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCellCount)
{
	//��Ŀ�ж�
	ASSERT((cbCardCount%cbCellCount)==0);
	if ((cbCardCount%cbCellCount)!=0) return false;
	
	//�����˿�
	BYTE cbCardDataTemp[MAX_COUNT];
	CopyMemory(cbCardDataTemp,cbCardData,cbCardCount*sizeof(BYTE));

	SortCardList(cbCardDataTemp,cbCardCount,ST_ORDER);
	if(GetCardValue(cbCardDataTemp[0])==2) return false;

	//�˿�����
	SortCardList(cbCardDataTemp,cbCardCount,ST_VALUE);


	//��������
	BYTE cbBlockCount=cbCardCount/cbCellCount;
	BYTE cbFirstValue=GetCardValue(cbCardDataTemp[0]);

	//��Ч����
	if (cbFirstValue>=14) return false;

	//�˿�����
	for (BYTE i=1;i<cbBlockCount;i++)
	{
		//�˿���ֵ
		BYTE cbCardValue=GetCardValue(cbCardDataTemp[i*cbCellCount]);

		//�������
		if ((cbCardValue==1)&&(cbFirstValue==13)) continue;

		//�����ж�
		if (cbFirstValue!=(cbCardValue+i)) return false;
	}

	//����ж�
	return true;
}

//�Ա��˿�
bool CGameLogic::CompareCardByValue(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//��������
	bool bHaveTwoNext=false;
	BYTE cbConsultNext[2]={0x00,0x00};

	//�����˿�
	for (BYTE i=0;i<cbNextCount;i++)
	{
		//��ȡ��ֵ
		BYTE cbConsultValue=GetCardValue(cbNextCard[i]);

		//���ñ���
		if ((bHaveTwoNext==false)&&(cbConsultValue==0x02)) bHaveTwoNext=true;

		//�����˿�
		if (cbConsultValue==0x01)
		{
			if (14>cbConsultNext[0]) cbConsultNext[0]=14;
			if (cbConsultValue>cbConsultNext[1]) cbConsultNext[1]=cbConsultValue;
		}
		else
		{
			if (cbConsultValue>cbConsultNext[0]) cbConsultNext[0]=cbConsultValue;
			if (cbConsultValue>cbConsultNext[1]) cbConsultNext[1]=cbConsultValue;
		}
	}

	//��������
	bool bHaveTwoFirst=false;
	BYTE cbConsultFirst[2]={0x00,0x00};

	//�����˿�
	for (BYTE i=0;i<cbFirstCount;i++)
	{
		//��ȡ��ֵ
		BYTE cbConsultValue=GetCardValue(cbFirstCard[i]);

		//���ñ���
		if ((bHaveTwoFirst==false)&&(cbConsultValue==0x02)) bHaveTwoFirst=true;

		//�����˿�
		if (cbConsultValue==0x01)
		{
			if (14>cbConsultFirst[0]) cbConsultFirst[0]=14;
			if (cbConsultValue>cbConsultFirst[1]) cbConsultFirst[1]=cbConsultValue;
		}
		else
		{
			if (cbConsultValue>cbConsultFirst[0]) cbConsultFirst[0]=cbConsultValue;
			if (cbConsultValue>cbConsultFirst[1]) cbConsultFirst[1]=cbConsultValue;
		}
	}

	//�Ա��˿�
	BYTE cbResultNext=(bHaveTwoNext==false)?cbConsultNext[0]:cbConsultNext[1];
	BYTE cbResultFirst=(bHaveTwoFirst==false)?cbConsultFirst[0]:cbConsultFirst[1];

	return cbResultNext>cbResultFirst;
}

//��������
bool CGameLogic::SearchLinkCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbCardType,tagOutCardResult &OutCardResult  ,BYTE TurnCardCount)
{
	//A�ⶥ,ֱ�ӷ��ػ���������������
	if( GetCardValue(cbReferCard) == 1 || cbHandCardCount<TurnCardCount)
	{
		//if(cbCardType==CT_THREE_LINK)	return false;
		//else
		//{
		//	if(cbCardType==CT_SINGLE_LINK&&TurnCardCount%2==0) cbCardType=CT_DOUBLE_LINK;
		//	else if(cbCardType==CT_DOUBLE_LINK&&TurnCardCount%3==0) cbCardType=CT_THREE_LINK;
		//	else return false;
		//}
		return false;
	}
research:

	//ȷ����Ԫ����,������Ŀ
	BYTE byCellCount,byLinkCount;
	switch( cbCardType )
	{
	case CT_SINGLE_LINK:
		{
			byCellCount = 1;
			byLinkCount = TurnCardCount/byCellCount;
			break;
		}
	case CT_DOUBLE_LINK:
		{
			byCellCount = 2;
			byLinkCount = TurnCardCount/byCellCount;
			break;
		}
	default:
		ASSERT( FALSE );
		return false;
	}
	//if( cbHandCardCount < byCellCount*byLinkCount ) return false;

	//ȷ��������ʼλ��
	BYTE byReferIndex = 0;
	if( cbReferCard != 0 ) 
		byReferIndex = GetCardValue(cbReferCard)-byLinkCount+1;

	//�����˿�
	tagDistributing Distribute;
	AnalysebDistributing( cbHandCardData,cbHandCardCount,Distribute );

	//��������˳��
	BYTE byTempLinkCount = 0;
	for( BYTE i = byReferIndex; i < 13; i++ )
	{
		if( Distribute.cbDistributing[i][cbIndexCount] < byCellCount )
		{
			byTempLinkCount = 0;
			continue;
		}
		//������
		if( ++byTempLinkCount == byLinkCount )
		{
			//��������
			OutCardResult.cbCardCount = 0;
			for( int j = i; j >= i-byLinkCount+1; j-- )
			{
				BYTE byCount = 0;
				for( BYTE k = 3; ; k = (k+1)%4 )
				{
					if( Distribute.cbDistributing[j][k] > 0 )
					{
						for( BYTE n = 0; n < Distribute.cbDistributing[j][k]; n++ )
						{
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(j,k);
							if( ++byCount == byCellCount ) break;
						}
						if( byCount == byCellCount ) break;
					}
				}
			}
			return true;
		}
	}
	//������A��˳��
	if( byTempLinkCount == byLinkCount-1 && Distribute.cbDistributing[0][cbIndexCount] >= byCellCount )
	{
		//��������
		OutCardResult.cbCardCount = 0;
		BYTE byCount = 0;
		//����A
		for( BYTE k = 3; ; k = (k+1)%4 )
		{
			if( Distribute.cbDistributing[0][k] > 0 )
			{
				for( BYTE n = 0; n < Distribute.cbDistributing[0][k]; n++ )
				{
					OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(0,k);
					if( ++byCount == byCellCount ) break;
				}
				if( byCount == byCellCount ) break;
			}
		}
		//����������
		for( i = 12; i >= 14-byLinkCount; i-- )
		{
			byCount = 0;
			for( k = 3; ; k = (k+1)%4 )
			{
				if( Distribute.cbDistributing[i][k] > 0 )
				{
					for( BYTE n = 0; n < Distribute.cbDistributing[i][k]; n++ )
					{
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(i,k);
						if( ++byCount == byCellCount ) break;
					}
					if( byCount == byCellCount ) break;
				}
			}
		}
		return true;
	}

	/*if(OutCardResult.cbCardCount==0)
	{
		//if(cbCardType==CT_SINGLE_LINK&&TurnCardCount%2==0) cbCardType=CT_DOUBLE_LINK;
		//else if(cbCardType==CT_DOUBLE_LINK&&TurnCardCount%3==0) cbCardType=CT_THREE_LINK;
		//else return false;
		return false;

		goto research;
	}*/
	return false;
}


//����510K
bool CGameLogic::Search510K( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,BYTE cbType,tagOutCardResult &OutCardResult )
{
	//�ȹ��˵�ǰ����С��510K
	/*if(GetCardType(cbTurnCardData,cbTurnCardCount)<CT_510K_DC)
	{
		BYTE Card5=0;
		BYTE Card10=0;
		BYTE CardK=0;
		for(BYTE i=0;i<cbHandCardCount;i++)
		{
			if(GetCardValue(cbHandCardData[i])==5) Card5=cbHandCardData[i];
			if(GetCardValue(cbHandCardData[i])==10) Card10=cbHandCardData[i];
			if(GetCardValue(cbHandCardData[i])==13) CardK=cbHandCardData[i];
		}

		if(Card5!=0&&Card10!=0&&CardK!=0)
		{
			OutCardResult.cbCardCount=3;
			OutCardResult.cbResultCard[0]=CardK;
			OutCardResult.cbResultCard[1]=Card10;
			OutCardResult.cbResultCard[2]=Card5;
			return true;
		}
	}

	//������ɫ510K
	BYTE bTurnColor=GetCardColor(cbTurnCardData[0]);

	//����510K���
	if(bTurnColor==0x30) return false;

	//��¼5
	BYTE Card5[8];
	BYTE Card5Count=0;
	ZeroMemory(Card5,sizeof(Card5));

	//��¼10
	BYTE Card10[8];
	BYTE Card10Count=0;
	ZeroMemory(Card10,sizeof(Card10));

	//��¼K
	BYTE CardK[8];
	BYTE CardKCount=0;
	ZeroMemory(CardK,sizeof(CardK));

	//����510K
	for(BYTE i=0;i<cbHandCardCount;i++)
	{
		if(GetCardValue(cbHandCardData[i])==5) Card5[Card5Count++]=cbHandCardData[i];
		if(GetCardValue(cbHandCardData[i])==10) Card10[Card10Count++]=cbHandCardData[i];
		if(GetCardValue(cbHandCardData[i])==13) CardK[CardKCount++]=cbHandCardData[i];
	}

	BYTE temp510K[3];	ZeroMemory(temp510K,sizeof(temp510K));
	for(BYTE a=0;a<Card5Count;a++)
	for(BYTE b=0;b<Card10Count;b++)
	for(BYTE c=0;c<CardKCount;c++)
	{
		temp510K[0]=CardK[c];	
		temp510K[1]=Card10[b];  
		temp510K[2]=Card5[a];	

		if(CompareCard(cbTurnCardData,temp510K,cbTurnCardCount,3)==true)
		{
			OutCardResult.cbCardCount=3;
			OutCardResult.cbResultCard[0]=CardK[c];
			OutCardResult.cbResultCard[1]=Card10[b];
			OutCardResult.cbResultCard[2]=Card5[a];
			return true;
		}
	}*/
	//��ɫ����
	BYTE COLOR_MASK=0;
	//5��10��k�Ļ�ɫ
	BYTE COLOR_5=0,COLOR_10=0,COLOR_K=0;
	//��510k�Ļ�ɫ
	BYTE COLOR_510K=0;
	//5,10,k���ڱ�ʶ
	BYTE FLAG_510K=0;
	//���Ҫ���ѯ�������Ƿ���ȷ
	if(cbType!=CT_510K_DC && cbType!=CT_510K_SC) return false;
	//����Ѿ��������Ǻ���510K���Ͳ��ò���
	if((GetCardType(cbTurnCardData,cbTurnCardCount)==CT_510K_SC)
		&& (GetCardColor(cbTurnCardData[0])==0x30))
		return false;
	for(BYTE i=0;i<cbHandCardCount;i++)
	{
		FLAG_510K=0;
		if(GetCardValue(cbHandCardData[i])==0x05)
		{
			FLAG_510K=1;
		}
		else if(GetCardValue(cbHandCardData[i])==0x0A)
		{
			FLAG_510K=2;
		}
		else if(GetCardValue(cbHandCardData[i])==0x0D)
		{
			FLAG_510K=4;
		}
		if(FLAG_510K)
		{
			switch(GetCardColor(cbHandCardData[i]))
			{
			case 0:	//����
				COLOR_MASK=0x01;
				break;
			case 1:	//÷��
				COLOR_MASK=0x02;
				break;
			case 2:	//����
				COLOR_MASK=0x04;
				break;
			case 3:	//����
				COLOR_MASK=0x08;
				break;
			}
			switch(FLAG_510K)
			{
			case 1:	//5
				COLOR_5|=COLOR_MASK;
				break;
			case 2:	//10
				COLOR_10|=COLOR_MASK;
				break;
			case 4:	//k
				COLOR_K|=COLOR_MASK;
				break;
			}
		}
	}
	//5��10��k������
	if(COLOR_5>0 && COLOR_10>0 && COLOR_K>0)
	{
		COLOR_510K=COLOR_5 & COLOR_10 & COLOR_K;	//�õ���510k�Ļ�ɫ
		//��ѯ��510k
		if(cbType==CT_510K_SC)
		{
			switch(GetCardColor(cbTurnCardData[0]))
			{
			case 0:	//����
				COLOR_MASK=0x01;
				break;
			case 1:	//÷��
				COLOR_MASK=0x02;
				break;
			case 2:	//����
				COLOR_MASK=0x04;
				break;
			case 3:	//����
				COLOR_MASK=0x08;
				break;
			}
			//û�л���Ҳ������
			if(COLOR_510K==0 || COLOR_510K<=COLOR_MASK) return false;
			for(BYTE i=COLOR_MASK<<1;i<0x10;i<<=1)
				if((COLOR_510K & i)>0)
				{
					COLOR_MASK = i;
					break;
				}
			switch(COLOR_MASK)
			{
			case 1:
				FLAG_510K =0;
				break;
			case 2:
				FLAG_510K =0x10;
				break;
			case 4:
				FLAG_510K =0x20;
				break;
			case 8:
				FLAG_510K =0x30;
				break;
			default:
				return false;
			}
			OutCardResult.cbCardCount = 3;
			//���촿510k
			OutCardResult.cbResultCard[0] = FLAG_510K+0x0D;
			OutCardResult.cbResultCard[1] = FLAG_510K+0x0A;
			OutCardResult.cbResultCard[2] = FLAG_510K+0x05;
			return true;
		}
		//��ѯ��510K
		else if(cbType == CT_510K_DC)
		{
			//�����510k�Ļ�ɫ
			COLOR_5 ^=COLOR_510K;
			COLOR_10 ^=COLOR_510K;
			COLOR_K ^=COLOR_510K;
			BYTE Cards[3],Data510K[]={0x0D,0x0A,0x05};
			Cards[0]=COLOR_K;
			Cards[1]=COLOR_10;
			Cards[2]=COLOR_5;
			OutCardResult.cbCardCount=3;
			for(BYTE i=0;i<3;i++)
			{
				for(BYTE j=0x01;j<0x10;j<<=1)
				{
					if(Cards[i]&j)
					{
						switch(Cards[i]&j)
						{
						case 1:
							FLAG_510K =0;
							break;
						case 2:
							FLAG_510K =0x10;
							break;
						case 4:
							FLAG_510K =0x20;
							break;
						case 8:
							FLAG_510K =0x30;
							break;
						default:
							return false;
						}
						OutCardResult.cbResultCard[i]=FLAG_510K+Data510K[i];
						break;
					}
				}
			}
			return true;
		}
	}
	return false;
}


//������������������
bool CGameLogic::SearchKings(const BYTE cbHandCardData[],BYTE cbHandCardCount,tagOutCardResult &OutCardResult,BYTE cbType)
{
	if(cbHandCardCount<2) return false;
	if(cbHandCardCount<3 && cbType ==CT_KING_THREE) return false;
	if(cbHandCardCount<4 && cbType ==CT_KING_FOUR) return false;
	BYTE cbKingLData[2]={0},cbKingLCount=0;
	BYTE cbKingBData[2]={0},cbKingBCount=0;
	for(BYTE i=0;i<cbHandCardCount;i++)
	{
		if(GetCardValue(cbHandCardData[i])==0x0E)
			cbKingLData[cbKingLCount++]=cbHandCardData[i];
		if(GetCardValue(cbHandCardData[i])==0x0F)
			cbKingBData[cbKingBCount++]=cbHandCardData[i];
	}

	if(cbType == CT_KING_DC && cbKingLCount>0 && cbKingBCount>0) //��С��
	{
		OutCardResult.cbCardCount=2;
		OutCardResult.cbResultCard[0]=cbKingBData[0];
		OutCardResult.cbResultCard[1]=cbKingLData[0];
		return true;
	}
	if(cbType == CT_KING_ST && cbKingLCount==2) //��С��
	{
		OutCardResult.cbCardCount=2;
		OutCardResult.cbResultCard[0]=OutCardResult.cbResultCard[1]=cbKingLData[0];
		return true;
	}
	
	if(cbType == CT_KING_SK && cbKingBCount==2) //������
	{
		OutCardResult.cbCardCount=2;
		OutCardResult.cbResultCard[0]=OutCardResult.cbResultCard[1]=cbKingBData[0];
		return true;
	}

	if(cbType == CT_KING_THREE && (cbKingLCount+cbKingBCount)>=3) //3��
	{
		OutCardResult.cbCardCount=3;
		CopyMemory(OutCardResult.cbResultCard,cbKingBData,sizeof(BYTE)*cbKingBCount);
		CopyMemory(OutCardResult.cbResultCard+sizeof(BYTE)*cbKingBCount,cbKingLData,sizeof(BYTE)*cbKingLCount);
		return true;
	}

	if(cbType == CT_KING_FOUR && (cbKingLCount+cbKingBCount)==4) //4��
	{
		OutCardResult.cbCardCount=4;
		OutCardResult.cbResultCard[0]=OutCardResult.cbResultCard[1]=cbKingBData[0];
		OutCardResult.cbResultCard[2]=OutCardResult.cbResultCard[3]=cbKingLData[0];
		return true;
	}

	return false;
}
//��������
bool CGameLogic::SearchTianWang( const BYTE cbHandCardData[],BYTE cbHandCardCount,tagOutCardResult &OutCardResult )
{
	//��֤
	if( cbHandCardCount < 4 ) return false;

	//����
	if( cbHandCardData[0] == 0x4F && cbHandCardData[3] == 0x4E )
	{
		OutCardResult.cbCardCount = 4;
		CopyMemory(OutCardResult.cbResultCard,cbHandCardData,sizeof(BYTE)*4);
		return true;
	}
	return false;
}

//�����������
bool CGameLogic::SearchMagicOutCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[],BYTE cbTurnCardCount,tagOutCardResult &OutCardResult )
{
	//��ȡ����
	BYTE cbTurnCardType = GetCardType(cbTurnCardData,cbTurnCardCount);
	ASSERT( cbTurnCardType > CT_ERROR );
	//if( cbTurnCardType == CT_BOMB_TW ) return false;

	//����������������
	switch( cbTurnCardType )
	{
	case CT_SINGLE:				//����,����ը����ʼ����
	case CT_DOUBLE:				//����
	//case CT_THREE:				//��ͬ��
		{
			BYTE cbSameCount = cbTurnCardType;
			//����ͬ��
			if(  cbTurnCardType!=CT_SINGLE && SearchMagicSameCard(cbHandCardData,cbHandCardCount,cbTurnCardData[0],cbSameCount,OutCardResult) )
				return true;
		}
	case CT_SINGLE_LINK:		//����
	case CT_DOUBLE_LINK:		//˫��
		{
			//ֻ�г����������Ӧ,������
			if( cbTurnCardType >= CT_SINGLE_LINK && cbTurnCardType <= CT_DOUBLE_LINK )
			{
				//Ѱ�ҶԱ���
				BYTE byReferCard = cbTurnCardData[0];
				if( GetCardValue(cbTurnCardData[0])==m_cbMainValue )
				{				
					BYTE nCellCount = cbTurnCardType-3;
					if( GetCardValue(cbTurnCardData[nCellCount])<6 && GetCardValue(cbTurnCardData[cbTurnCardCount])<6 )
					{
						byReferCard = IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount)?
							MakeCardData(5,0):MakeCardData(4,0);
					}
					else if( IsStructureLink(&cbTurnCardData[nCellCount],cbTurnCardCount-nCellCount,nCellCount) )
					{
						byReferCard = GetCardValue(cbTurnCardData[nCellCount])==1?
							cbTurnCardData[nCellCount]:MakeCardData(GetCardValue(cbTurnCardData[nCellCount])%13,0);
					}
					else byReferCard = cbTurnCardData[nCellCount];
				}
				if( SearchMagicLink(cbHandCardData,cbHandCardCount,byReferCard,cbTurnCardType,OutCardResult) )
					return true;
			}
		}
	case CT_BOMB_4:				//ը��
		{
			//ȷ��ը����Ŀ
			BYTE cbSameCount = 0,cbReferCard = 0;
			cbSameCount = cbTurnCardType>=CT_BOMB_4?cbTurnCardCount:4;
			cbReferCard = cbTurnCardType>=CT_BOMB_4?cbTurnCardData[0]:0;

			//���ը����С�ڵ���5
			if( cbSameCount <= 5 )
			{
				if( SearchMagicSameCard(cbHandCardData,cbHandCardCount,cbReferCard,cbSameCount,OutCardResult) )
					return true;
				//if( SearchMagicTongHuaShun(cbHandCardData,cbHandCardCount,0,OutCardResult) )
				//	return true;
			}
			//����ͬ��
			else if( SearchMagicSameCard(cbHandCardData,cbHandCardCount,cbReferCard,cbSameCount,OutCardResult) )
				return true;
			break;
		}
	}

	return false;
}



//�����������
bool CGameLogic::SearchMagicLink( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbCardType,tagOutCardResult &OutCardReslut )
{
	//A�ⶥ,ֱ�ӷ���
	if( GetCardValue(cbReferCard) == 1 ) return false;

	//ȷ����Ԫ��,������
	BYTE byCellCount,byLinkCount;
	switch(cbCardType)
	{
	case CT_SINGLE_LINK:
		{
			byCellCount = 1;
			byLinkCount = 5;
			break;
		}
	case CT_DOUBLE_LINK:
		{
			byCellCount = 2;
			byLinkCount = 3;
			break;
		}
	default:
		ASSERT( FALSE );
		return false;
	}
	//���������ڱ�����,ֱ�ӷ���
	if( byCellCount*byLinkCount > cbHandCardCount ) return false;

	//�����˿�
	tagMagicResult MagicResult;
	AnalyseMagicData(cbHandCardData,cbHandCardCount,MagicResult);
	if( MagicResult.cbMagicCardCount == 0 ) return false;

	//�����˿�
	tagDistributing Distribute;
	AnalysebDistributing( MagicResult.cbNormalCard,MagicResult.cbNormalCardCount,Distribute );

	//ȷ����ʼ����λ��
	BYTE byReferIndex = 0;
	if( cbReferCard != 0 ) 
		byReferIndex = GetCardValue(cbReferCard)-byLinkCount+1;

	//����
	for( BYTE byMagicCount = 1; byMagicCount <= MagicResult.cbMagicCardCount; byMagicCount++ )
	{
		//��������
		int nMagicCount = byMagicCount;
		BYTE byTmpLinkCount = 0;
		//����˳��
		BYTE i = byReferIndex;
		do
		{
			if( Distribute.cbDistributing[i][cbIndexCount] < byCellCount )
			{
				nMagicCount -= (byCellCount-Distribute.cbDistributing[i][cbIndexCount]);
				//�ж��Ƿ��㹻
				if( nMagicCount < 0 )
				{
					byTmpLinkCount = 0;
					//�����ʱ,���׷˷
					if( byMagicCount >= (byCellCount-Distribute.cbDistributing[i][cbIndexCount]) )
					{
						int nTmpMagicCount = byMagicCount;
						nTmpMagicCount -= (byCellCount-Distribute.cbDistributing[i][cbIndexCount]);
						byTmpLinkCount++;
						if( nTmpMagicCount > 0 )
						{
							BYTE byIndex = i-1;
							do
							{
								if( Distribute.cbDistributing[byIndex][cbIndexCount] < byCellCount )
								{
									nTmpMagicCount -= (byCellCount-Distribute.cbDistributing[i][cbIndexCount]);
									if( nTmpMagicCount < 0 ) break;
									byTmpLinkCount++;
								}
								else byTmpLinkCount++;
								byIndex--;					//ע:byIndexʼ�մ���byReferIndex
							}while(true);
						}
						nMagicCount = 0;
					}
					//�����ʱ,��ǰ��������
					else nMagicCount = byMagicCount;

					i = (i+1)%13;
					if( i == 1 ) break;
					continue;
				}
				else byTmpLinkCount++;
			}
			else byTmpLinkCount++;
			//�ɱ任
			if( byTmpLinkCount == byLinkCount )
			{
				//��������
				BYTE byIndex = (i==0?14-byLinkCount:i-byLinkCount+1);
				BYTE byCount = 0;
				nMagicCount = 0;
				do
				{
					BYTE byTmpCount = 0;
					if( Distribute.cbDistributing[byIndex][cbIndexCount] > 0 )
					{
						for( BYTE j = 0; j < 4; j++ )
						{
							if( Distribute.cbDistributing[byIndex][j] > 0 )
							{
								for( BYTE n = 0; n < Distribute.cbDistributing[byIndex][j]; n++ )
								{
									OutCardReslut.cbResultCard[OutCardReslut.cbCardCount++] = MakeCardData(byIndex,j);
									if( ++byTmpCount == byCellCount ) break;
								}
								if( byTmpCount == byCellCount ) break;
							}
						}
					}
					//�
					if( byTmpCount < byCellCount )
					{
						for( BYTE k = 0; k < byCellCount-byTmpCount; k++ )
						{
							OutCardReslut.cbResultCard[OutCardReslut.cbCardCount++] = MagicResult.cbMagicCard[nMagicCount++];
							ASSERT( nMagicCount <= byMagicCount );
						}
					}
					byIndex = (byIndex+1)%13;
				}while( ++byCount < byLinkCount );
				//����
				SortCardList(OutCardReslut.cbResultCard,OutCardReslut.cbCardCount,ST_ORDER);
				return true;
			}
			i = (i+1)%13;
		}while( i != 1 );		//������AΪֹ
	}
	return false;
}

//�������ͬ��
bool CGameLogic::SearchMagicSameCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbLessSameCount,tagOutCardResult &OutCardResult )
{
	//��֤
	if( cbHandCardCount < cbLessSameCount ) return false;

	//��������
	tagMagicResult MagicResult;
	tagAnalyseResult AnalyseResult;
	
	//�����˿�
	AnalyseMagicData(cbHandCardData,cbHandCardCount,MagicResult);
	if( MagicResult.cbMagicCardCount == 0 ) return false;

	//�����˿�
	AnalysebCardData(MagicResult.cbNormalCard,MagicResult.cbNormalCardCount,AnalyseResult);

	//����
	for( BYTE byMagicCount = 1; byMagicCount <= MagicResult.cbMagicCardCount; byMagicCount++ )
	{
		//ȷ����������λ��
		int nIndex = cbLessSameCount-byMagicCount-1;
		if( nIndex < 0 ) break;
		if( AnalyseResult.cbBlockCount[nIndex] > 0 )
		{
			for( int i = AnalyseResult.cbBlockCount[nIndex]-1; i >= 0; i-- )
			{ 
				//��С�����ܱ��
				if( GetCardLogicValue(AnalyseResult.cbCardData[nIndex][(nIndex+1)*i]) > 14 ) continue;
				//�Ƚϴ�С
				if( cbReferCard==0 || CompareCard(cbReferCard,AnalyseResult.cbCardData[nIndex][(nIndex+1)*i]) )
				{
					CopyMemory(OutCardResult.cbResultCard,MagicResult.cbMagicCard,sizeof(BYTE)*byMagicCount);
					OutCardResult.cbCardCount = byMagicCount;
					CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount],
						&AnalyseResult.cbCardData[nIndex][(nIndex+1)*i],sizeof(BYTE)*(nIndex+1));
					OutCardResult.cbCardCount += nIndex+1;
					SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
					return true;
				}
			}
		}
		//������ͬ����,��С��cbReferCardʱ
		if( byMagicCount == 1 && nIndex != 0 /*ȷ������������������*/  
			&& AnalyseResult.cbBlockCount[nIndex+1] > 0 )
		{
			ASSERT( CompareCard(AnalyseResult.cbCardData[nIndex+1][0],cbReferCard) );
			OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MagicResult.cbMagicCard[0];
			CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount],
				&AnalyseResult.cbCardData[nIndex+1][(nIndex+2)*(AnalyseResult.cbBlockCount[nIndex+1]-1)],
				sizeof(BYTE)*(nIndex+2));
			OutCardResult.cbCardCount += nIndex+2;
			SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
			return true;
		}
		//������С�ڳ�����һ�ŵ���ͬ��,�������������ʱ
		if( byMagicCount == 2 && AnalyseResult.cbBlockCount[nIndex+1] > 0 )
		{
			if( GetCardLogicValue(AnalyseResult.cbCardData[nIndex+1][(nIndex+2)*(AnalyseResult.cbBlockCount[nIndex+1]-1)]) < 15 )
			{
				CopyMemory(OutCardResult.cbResultCard,MagicResult.cbMagicCard,sizeof(BYTE)*byMagicCount);
				OutCardResult.cbCardCount = 2;
				CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount],
					&AnalyseResult.cbCardData[nIndex+1][(nIndex+2)*(AnalyseResult.cbBlockCount[nIndex+1]-1)],
					sizeof(BYTE)*(nIndex+2));
				SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
				return true;
			}
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
