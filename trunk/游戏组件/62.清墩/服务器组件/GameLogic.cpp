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
	m_bFirst2=false;
	return;
}

//��������
CGameLogic::~CGameLogic()
{
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
			if (GetCardValue(cbCardData[0])==GetCardValue(cbCardData[1])) return CT_DOUBLE;
			return CT_ERROR;
		}
	case 3:	//����
		{
			if(cbCardData[0]==cbCardData[1]&&cbCardData[0]==cbCardData[2]) return CT_QING_DUN;
			if (GetCardValue(cbCardData[0])==GetCardValue(cbCardData[2]))  return CT_THREE;
			return CT_ERROR;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//ը������
	if ((cbCardCount==4)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_4;
	if ((cbCardCount==5)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1)) return CT_BOMB_5;
	if ((cbCardCount>=6)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1))
	{
		if(cbCardCount==8)//���޶�
		{
			bool bHaveDun=false;
			for(BYTE i=0;i<6;i++)
			{
				if(cbCardData[i]==cbCardData[i+2]) bHaveDun=true;
			}
			if(bHaveDun==false) return CT_BOMB_8;
		}
		if(cbCardCount%3==0)//�ֵܶ�
		{
			bool bAllDun=true;
			for(BYTE i=0;i<=cbCardCount-3;i+=3)
			{
				if(cbCardData[i]!=cbCardData[i+2]) bAllDun=false;
			}
			if(bAllDun==true) return CT_XIONGDI_DUN;
		}
		return CT_BOMB_6;
	}

	//˳������
	if ((cbCardCount==5)&&(AnalyseResult.cbBlockCount[0]==cbCardCount))
	{
		//�˿�����
		BYTE cbSignedCount=AnalyseResult.cbBlockCount[0];
		BYTE cbCardColor=GetCardColor(AnalyseResult.cbCardData[0],cbSignedCount);
		bool bStructureLink=IsStructureLink(AnalyseResult.cbCardData[0],cbSignedCount,1);

		//�����ж�
		if ((bStructureLink==true)&&(cbCardColor==0xF0)) return CT_SINGLE_LINK;
		if ((bStructureLink==true)&&(cbCardColor!=0xF0)) return CT_TONG_HUA_SHUN;
	}

	//��������
	if ((cbCardCount>=6)&&((AnalyseResult.cbBlockCount[1]*2)==cbCardCount))
	{
		BYTE cbDoubleCount=AnalyseResult.cbBlockCount[1]*2;
		if (IsStructureLink(AnalyseResult.cbCardData[1],cbDoubleCount,2)==true) return CT_DOUBLE_LINK;
	}

	//��������
	if ((cbCardCount==5)&&(AnalyseResult.cbBlockCount[1]==1)&&(AnalyseResult.cbBlockCount[2]==1)) 
	{
		BYTE bDoubleValue=GetCardValue(AnalyseResult.cbCardData[1][0]);
		BYTE bThreeValue=GetCardValue(AnalyseResult.cbCardData[2][0]);
		if(bThreeValue==bDoubleValue+1||bThreeValue==bDoubleValue-1|| (bDoubleValue==1&&bThreeValue==13) || (bDoubleValue==13&&bThreeValue==1)) 
			return CT_THREE_DOUBLE;
	}

	//����
	if ((cbCardCount>=15)&&(AnalyseResult.cbBlockCount[1]==AnalyseResult.cbBlockCount[2])&&cbCardCount%5==0)
	{
		//����Ƿ���2��3
		bool lian3=false,lian2=false;
		BYTE cbThreeCount=AnalyseResult.cbBlockCount[2]*3;
		if (IsStructureLink(AnalyseResult.cbCardData[2],cbThreeCount,3)==true) lian3=true;
		BYTE cbDoubleCount=AnalyseResult.cbBlockCount[1]*2;
		if (IsStructureLink(AnalyseResult.cbCardData[1],cbDoubleCount,2)==true) lian2=true;
		
		if(cbThreeCount==0||cbDoubleCount==0) lian3=false;

		//���ֱ�Ϊ��2��3
		if(lian3&&lian2)
		{
			BYTE bLinkCount=0;
			BYTE bLinkData[40];
			ZeroMemory(bLinkData,sizeof(bLinkData));

			for(BYTE i=0;i<cbThreeCount;i+=3)
			{
				bLinkData[bLinkCount++]=AnalyseResult.cbCardData[2][i];
			}
			for(BYTE i=0;i<cbDoubleCount;i+=2)
			{
				bLinkData[bLinkCount++]=AnalyseResult.cbCardData[1][i];
			}

			//ȫ�����Ǻ���
			SortCardList(bLinkData,bLinkCount,ST_ORDER);
			if(IsStructureLink(bLinkData,bLinkCount,1)==true) return CT_HU_DIE;
		}
	}

	//����
	if ((cbCardCount>=6)&&((AnalyseResult.cbBlockCount[2]*3)==cbCardCount))
	{
		//��С������
		if( cbCardCount==6 && (AnalyseResult.cbCardData[2][0]&0x0F)==0x0F && (AnalyseResult.cbCardData[2][3]&0x0F)==0x0E )
			return CT_LIAN_DUN;
		BYTE cbThreeCount=AnalyseResult.cbBlockCount[2]*3;
		if (IsStructureLink(AnalyseResult.cbCardData[2],cbThreeCount,3)==true)
		{
			bool bSameColor=true;
			for(BYTE i=0;i<cbCardCount;i++)
			{
				if(GetCardColor(cbCardData[i])!=GetCardColor(cbCardData[0])) bSameColor=false;
			}
			if(bSameColor==true) return CT_LIAN_DUN;
		}
	}

	//��������
	if ((cbCardCount>=9)&&((AnalyseResult.cbBlockCount[2]*3)==cbCardCount))
	{
		BYTE cbThreeCount=AnalyseResult.cbBlockCount[2]*3;
		if (IsStructureLink(AnalyseResult.cbCardData[2],cbThreeCount,3)==true) return CT_THREE_LINK;
	}

	//��������
	if (cbCardCount>=8)
	{
		BYTE byCellCount = 1;
		BYTE byLogicValue = (cbCardData[0]&0x0F);
		for( BYTE i = 1; i < cbCardCount; i++ )
			if( byLogicValue == (cbCardData[i]&0x0F) ) byCellCount++;
			else break;
		if(byCellCount<4) return CT_ERROR;
		if( cbCardCount == AnalyseResult.cbBlockCount[byCellCount-1]*byCellCount )
		{
			if( IsStructureLink(AnalyseResult.cbCardData[byCellCount-1],cbCardCount,byCellCount) ) return CT_PAI_PAO;
		}	
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
	if (cbCardValue==m_cbMainValue) return 15;

	//�߼���ֵ
	switch (cbCardValue)
	{
	case 0x01: { return 14; }
	case 0x0E: { return 17; }
	case 0x0F: { return 18; }
	}

	//ת����ֵ
	return cbCardValue;
}
//�Ա��˿�
bool CGameLogic::CompareCard( const BYTE cbFirstCard,const BYTE cbNextCard )
{
	if(m_bFirst2==true&&GetCardValue(cbFirstCard)==2) 
		return GetCardValue(cbNextCard)>GetCardValue(cbFirstCard);
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
void CGameLogic::SetFirst2(bool bFirst2)
{
	m_bFirst2=bFirst2;return;
}
//�Աȵ���
bool CGameLogic::CompareCardMaxValue( const BYTE cbFirstCard[],const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount )
{
	BYTE bFirstMaxValue=0;
	BYTE bNextMaxValue=0;

	//ǰһ���Ƶ����
	for(BYTE i=0;i<cbFirstCount;i++)
	{
		if(GetCardValue(cbFirstCard[i])>bFirstMaxValue) bFirstMaxValue=GetCardValue(cbFirstCard[i]);
	}
	if(GetCardValue(cbFirstCard[0])==1&&GetCardValue(cbFirstCard[1])==13) bFirstMaxValue=14;

	//��һ���Ƶ����
	for(BYTE i=0;i<cbNextCount;i++)
	{
		if(GetCardValue(cbNextCard[i])>bNextMaxValue) bNextMaxValue=GetCardValue(cbNextCard[i]);
	}
	if(GetCardValue(cbNextCard[0])==1&&GetCardValue(cbNextCard[1])==13) bNextMaxValue=14;

	return bNextMaxValue>bFirstMaxValue;
}
//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//�����ж�
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

	//2�ȳ������⴦��
	if(m_bFirst2)
	{
		//����Ƿ�ȫ��2
		bool bPure2=true;
		for(BYTE i=0;i<cbFirstCount;i++) if(GetCardValue(cbFirstCard[i])!=2) bPure2=false;

		if(bPure2)
		{
			//ը���ж�
			if(cbNextType>=CT_BOMB_4&&cbFirstType<CT_BOMB_4) return true;
			if(cbNextType>=CT_BOMB_4&&cbFirstType>=CT_BOMB_4&&cbNextType!=cbFirstType) return cbNextType>cbFirstType;
			if(cbNextType>=CT_BOMB_4&&cbFirstType>=CT_BOMB_4&&cbNextType==cbFirstType&&cbNextCount!=cbFirstCount) return cbNextCount>cbFirstCount;
			//�����ж�
			if(cbFirstType!=cbNextType) return false;
			if(GetCardValue(cbNextCard[0])==1) return true;
			return GetCardValue(cbNextCard[0])>=GetCardValue(cbFirstCard[0]);
		}
	}

	//�ڶ�������
	if(cbNextType>=CT_BOMB_4)
	{
		//��ͬ����
		if(cbFirstType!=cbNextType)
		{
			return cbNextType>cbFirstType;
		}
		//��ͬ����
		if(cbFirstType==cbNextType)
		{
			//�������Ƚ�
			if(cbFirstCount!=cbNextCount) return cbNextCount>cbFirstCount;
			//���ڱȽ�
			if(cbNextType==CT_PAI_PAO||cbNextType==CT_LIAN_DUN)
			{
				BYTE bSameCount=1;
				//�ȳ�����
				tagAnalyseResult AnalyseResultFirst;
				AnalysebCardData(cbFirstCard,cbFirstCount,AnalyseResultFirst);
				BYTE bFirstCount=1;
				for(BYTE i=0;i<12;i++)	
				{
					if(AnalyseResultFirst.cbBlockCount[i]>bFirstCount) 
					{
						bFirstCount=AnalyseResultFirst.cbBlockCount[i];
						bSameCount=i;
						break;
					}
				}
				//�������
				tagAnalyseResult AnalyseResultNext;
				AnalysebCardData(cbNextCard,cbNextCount,AnalyseResultNext);
				BYTE bNextCount=1;
				for(BYTE i=0;i<12;i++)
				{
					if(AnalyseResultNext.cbBlockCount[i]>bNextCount) 
						bNextCount=AnalyseResultNext.cbBlockCount[i];
				}

				//�������Ĵ�
				if(bFirstCount!=bNextCount)
				{
					return bNextCount>bFirstCount;
				}
				
				BYTE bFirstLogicValue=GetCardLogicValue(AnalyseResultFirst.cbCardData[bSameCount][cbFirstCount-1]);
				BYTE bNextLogicValue=GetCardLogicValue(AnalyseResultNext.cbCardData[bSameCount][cbNextCount-1]);
				return  bNextLogicValue>bFirstLogicValue;

			}
			if(cbNextType==CT_TONG_HUA_SHUN)
			{
				return CompareCardMaxValue(cbFirstCard,cbNextCard,cbFirstCount,cbNextCount);
			}
			//����Ƚϵ�����С
			return GetCardLogicValue(cbNextCard[0])>GetCardLogicValue(cbFirstCard[0]);			
		}
	}


	//��ͬ����
	if (cbFirstType!=cbNextType) return false;

	//��ͬ����
	switch (cbFirstType)
	{
	case CT_SINGLE:				//��������
	case CT_DOUBLE:				//��������
	case CT_THREE:				//��������
		{
			//��������
			BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
			BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);

			return cbConsultNext>cbConsultFirst;
		}
	case CT_SINGLE_LINK:		//��������
	case CT_DOUBLE_LINK:		//��������
	case CT_THREE_LINK:			//��������
		{
			if(cbFirstCount!=cbNextCount) return false;
			return CompareCardByValue(cbFirstCard,cbNextCard,cbFirstCount,cbNextCount);
		}
	case CT_THREE_DOUBLE:		//��������
	case CT_HU_DIE:				//��������
		{
			if(cbFirstCount!=cbNextCount) return false;
			//�����˿�
			tagAnalyseResult AnalyseResultNext;
			tagAnalyseResult AnalyseResultFirst;
			AnalysebCardData(cbNextCard,cbNextCount,AnalyseResultNext);
			AnalysebCardData(cbFirstCard,cbFirstCount,AnalyseResultFirst);

			//��������
			BYTE cbThreeNext=GetCardLogicValue(AnalyseResultNext.cbCardData[2][0]);
			BYTE cbThreeFirst=GetCardLogicValue(AnalyseResultFirst.cbCardData[2][0]);
			BYTE cbDoubleNext=GetCardLogicValue(AnalyseResultNext.cbCardData[1][0]);
			BYTE cbDoubleFirst=GetCardLogicValue(AnalyseResultFirst.cbCardData[1][0]);
			
			//2��«�����⴦��
			if(cbNextType==CT_THREE_DOUBLE&&cbThreeNext==15&&cbDoubleNext==3)	//22233�ĺ�«
			{
				return false;		//��2���ĺ�«
			}
			if(cbFirstType==CT_THREE_DOUBLE&&cbThreeFirst==15&&cbDoubleFirst==3)	//22233�ĺ�«
			{
				if(cbThreeNext!=15) return true;		//��2���ĺ�«
			}

			//3����ͬʱ�Ƚ�3���Ĵ�С
			if(cbThreeNext!=cbThreeFirst) return cbThreeNext>cbThreeFirst;
			//3����ͬʱ�Ƚ϶��ƵĴ�С
			if(cbThreeNext==cbThreeFirst) return (cbDoubleFirst==15&&cbDoubleNext!=15)?true:(cbDoubleNext>cbDoubleFirst);
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
		BYTE cbIndex=AnalyseResult.cbBlockCount[cbSameCount-1]++;
		for (BYTE j=0;j<cbSameCount;j++) AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];

		//��������
		i+=cbSameCount-1;
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
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;
		Distributing.cbDistributing[cbCardValue-1][cbCardColor>>4]++;
	}

	return;
}

//��������
bool CGameLogic::SearchHuDie( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult ,BYTE TurnCardCount )
{
	//������������
	if(cbHandCardCount<TurnCardCount) return false;

	//��������
	BYTE tempHandData[MAX_COUNT];
	ZeroMemory(tempHandData,sizeof(tempHandData));
	CopyMemory(tempHandData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

	//��������
	ASSERT(TurnCardCount%5==0);
	BYTE bLinkCount=TurnCardCount/5;

	//ѭ��������3
	for(BYTE targetValue=GetCardValue(cbReferCard);targetValue<13;targetValue++)
	{
		//������3
		bool HaveLian3=SearchLinkCard(tempHandData,cbHandCardCount,targetValue, CT_THREE_LINK,OutCardResult,bLinkCount*3);

		//����3�ż�����⣬��������Ŀ�����Ѱ��
		if(HaveLian3==true)
		{
			//�Ƴ���3���ټ����2
			RemoveCard(OutCardResult.cbResultCard,bLinkCount*3,tempHandData,cbHandCardCount);
				
			//��������
			tagOutCardResult OutCardResult2;
			ZeroMemory(&OutCardResult2,sizeof(OutCardResult2));
			OutCardResult2.cbCardCount=bLinkCount*2;

			//��3��ͷβ
			BYTE bThreeValueMin=GetCardValue(OutCardResult.cbResultCard[8]);
			BYTE bThreeValueMax=GetCardValue(OutCardResult.cbResultCard[0]);

			//��2��ͷβ
			BYTE bDoubleValueMin=bThreeValueMax+1;
			BYTE bDoubleValueMax=bThreeValueMin-1;

			//��ǰ����
			bool HaveLian2=SearchLinkCard(tempHandData,cbHandCardCount-bLinkCount*3,bDoubleValueMax-bLinkCount+2, CT_DOUBLE_LINK,OutCardResult2,bLinkCount*2);
			if(HaveLian2==true)
			{
				OutCardResult.cbCardCount=bLinkCount*5;
				CopyMemory(&OutCardResult.cbResultCard[bLinkCount*3],OutCardResult2.cbResultCard,sizeof(BYTE)*bLinkCount*2);

				//��������
				tagAnalyseResult ResultAnalyse;
				AnalysebCardData(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ResultAnalyse);

				BYTE cbThreeCount=ResultAnalyse.cbBlockCount[2]*3;
				BYTE cbDoubleCount=ResultAnalyse.cbBlockCount[1]*2;

				BYTE bLinkCount=0;
				BYTE bLinkData[40];
				ZeroMemory(bLinkData,sizeof(bLinkData));

				for(BYTE i=0;i<cbThreeCount;i+=3)
				{
					bLinkData[bLinkCount++]=ResultAnalyse.cbCardData[2][i];
				}
				for(BYTE i=0;i<cbDoubleCount;i+=2)
				{
					bLinkData[bLinkCount++]=ResultAnalyse.cbCardData[1][i];
				}

				//ȫ�����Ǻ���
				SortCardList(bLinkData,bLinkCount,ST_ORDER);
				if(IsStructureLink(bLinkData,bLinkCount,1)==true) return true;
			}

			//�������
			HaveLian2=SearchLinkCard(tempHandData,cbHandCardCount-bLinkCount*3,bDoubleValueMin-bLinkCount, CT_DOUBLE_LINK,OutCardResult2,bLinkCount*2);
			if(HaveLian2==true)
			{
				OutCardResult.cbCardCount=bLinkCount*5;
				CopyMemory(&OutCardResult.cbResultCard[bLinkCount*3],OutCardResult2.cbResultCard,sizeof(BYTE)*bLinkCount*2);

				//��������
				tagAnalyseResult ResultAnalyse;
				AnalysebCardData(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ResultAnalyse);

				BYTE cbThreeCount=ResultAnalyse.cbBlockCount[2]*3;
				BYTE cbDoubleCount=ResultAnalyse.cbBlockCount[1]*2;

				BYTE bLinkCount=0;
				BYTE bLinkData[40];
				ZeroMemory(bLinkData,sizeof(bLinkData));

				for(BYTE i=0;i<cbThreeCount;i+=3)
				{
					bLinkData[bLinkCount++]=ResultAnalyse.cbCardData[2][i];
				}
				for(BYTE i=0;i<cbDoubleCount;i+=2)
				{
					bLinkData[bLinkCount++]=ResultAnalyse.cbCardData[1][i];
				}

				//ȫ�����Ǻ���
				SortCardList(bLinkData,bLinkCount,ST_ORDER);
				if(IsStructureLink(bLinkData,bLinkCount,1)==true) return true;
			}
		}
	}

	return false;
}
//��������
bool CGameLogic::SearchPaiPao( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[], BYTE cbTurnCardCount,tagOutCardResult &OutCardResult )
{
	//������������
	if(cbHandCardCount<cbTurnCardCount) return false;

	//�����˿�
	tagDistributing Distribute;
	AnalysebDistributing( cbHandCardData,cbHandCardCount,Distribute );

	//A�����ݸ���
	CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(BYTE)*6);

	BYTE bCellCount=4;	//��С��Ԫ��Ϊ4
	BYTE bLinkCoun=2;	//��С����Ϊ2

	for(bCellCount=4;bCellCount<8;bCellCount++) //��Ԫ��4~7 8������8�޶ջ������ 9��������һ�������
	{
		for(bLinkCoun=2;bLinkCoun<11;bLinkCoun++)//����2~10
		{
			for(BYTE i=0;i<14;i++)//����1��13(A��KA)
			{
				for(BYTE j=0;j<bLinkCoun;j++)
				{
					if((i+j)<15&&Distribute.cbDistributing[i+j][5]<bCellCount) break;
				}
				if(j==bLinkCoun)
				{
					//������
					for( BYTE k = i; k < i + j; k++ )
					{
						BYTE bCount = 0;
						for( BYTE n = 0; n < 4; n++ )
						{
							if( Distribute.cbDistributing[k][n] > 0 )
							{
								for( BYTE m = 0; m < Distribute.cbDistributing[k][n]; m++ )
								{
									if(k==13)
									{
										OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=((n<<4)|1);
										//(cbColorIndex<<4)|(cbValueIndex+1)
									}
									else 
										OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData( k,n );
									if( ++bCount == bCellCount ) break;
								}
							}
							if( bCount == bCellCount ) break;
						}
					}
					//������
					SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
					//�Ƚ�Ϊ��ͷ��� �����������
					if(CompareCard(cbTurnCardData, OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)==true)	return true;
					else 
					{
						ZeroMemory(&OutCardResult,sizeof(OutCardResult));
						continue;
					}
				}
			}
		}
	}
	

	
	return false;
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

//�������޶�
bool CGameLogic::SearchBomb8( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[], BYTE cbTurnCardCount,tagOutCardResult &OutCardResult )
{
	//������������
	if(cbHandCardCount<8) return false;

	//�����˿�
	tagDistributing Distribute;
	AnalysebDistributing( cbHandCardData,cbHandCardCount,Distribute );

	//ѭ������
	for(BYTE i=0;i<13;i++)
	{
		if(Distribute.cbDistributing[i][5]>=8)//��8������
		{
			if( Distribute.cbDistributing[i][0]>=2 && Distribute.cbDistributing[i][1]>=2 && Distribute.cbDistributing[i][2]>=2 && Distribute.cbDistributing[i][3]>=2)
			{
				OutCardResult.cbCardCount=8;
				OutCardResult.cbResultCard[0]=MakeCardData(i,0);
				OutCardResult.cbResultCard[1]=MakeCardData(i,0);
				OutCardResult.cbResultCard[2]=MakeCardData(i,1);
				OutCardResult.cbResultCard[3]=MakeCardData(i,1);
				OutCardResult.cbResultCard[4]=MakeCardData(i,2);
				OutCardResult.cbResultCard[5]=MakeCardData(i,2);
				OutCardResult.cbResultCard[6]=MakeCardData(i,3);
				OutCardResult.cbResultCard[7]=MakeCardData(i,3);
				if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)==true) return true;
			}
		}
	}

	return false;
}

//��������
bool CGameLogic::SearchLianDun( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[], BYTE cbTurnCardCount,tagOutCardResult &OutCardResult )
{
	//������������
	if(cbHandCardCount<cbTurnCardCount) return false;

	//�����˿�
	tagDistributing Distribute;
	AnalysebDistributing( cbHandCardData,cbHandCardCount,Distribute );

	//A�����ݸ���
	CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(BYTE)*6);

	for(BYTE bLinkCount=2;bLinkCount<13;bLinkCount++)//����2~13
	{
		for(BYTE i=0;i<14;i++)
		{
			for(BYTE bColor=0;bColor<4;bColor++)
			{
				for(BYTE j=0;j<bLinkCount;j++)
				{
					if((i+j)<15&&Distribute.cbDistributing[i+j][bColor]<3) break;
				}
				if(j==bLinkCount)
				{
					//������		
					for( BYTE k = i; k < i + j; k++ )
					{
						//��ȡ3��
						if(k==13)
						{
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=((bColor<<4)|1);
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=((bColor<<4)|1);
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=((bColor<<4)|1);
						}
						else 
						{
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData( k,bColor );	
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData( k,bColor );	
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData( k,bColor );	
						}
					}
					//������
					SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
					//�Ƚ�Ϊ��ͷ��� �����������
					if(CompareCard(cbTurnCardData, OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)==true)	return true;
					else 
					{
						ZeroMemory(&OutCardResult,sizeof(OutCardResult));
						continue;
					}
				}
			}		
		}
	}
		
	return false;
}
//�����ֵܶ�
bool CGameLogic::SearchXiongdiDun( const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbTurnCardData[], BYTE cbTurnCardCount,tagOutCardResult &OutCardResult )
{
	//������������
	if(cbHandCardCount<cbTurnCardCount) return false;

	//�����˿�
	tagDistributing Distribute;
	AnalysebDistributing( cbHandCardData,cbHandCardCount,Distribute );

	//ѭ������
	for(BYTE i=0;i<13;i++)
	{
		//6�Ż����ϲ����ֵܶ�
		if(Distribute.cbDistributing[i][5]>=6)
		{
			for(BYTE bColor=0;bColor<4;bColor++)
			{
				if(Distribute.cbDistributing[i][bColor]==3)
				{
					OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData( i,bColor );	
					OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData( i,bColor );	
					OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData( i,bColor );	
				}
			}
			//������
			SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
			//�Ƚ�Ϊ��ͷ��� �����������
			if(CompareCard(cbTurnCardData, OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)==true)	return true;
			else 
			{
				ZeroMemory(&OutCardResult,sizeof(OutCardResult));
				continue;
			}
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

	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

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
	case CT_THREE_DOUBLE:			//��«
		{
			//ֻ�г������Ͷ�Ӧ,������
			if( cbTurnOutType == CT_THREE_DOUBLE)
			{
				if( SearchHuLu(cbHandCardData,cbHandCardCount,cbTurnCardData[2],OutCardResult,cbTurnCardCount) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}
			}
		}
	case CT_HU_DIE:
		{
			//ֻ�г������Ͷ�Ӧ,������
			if( cbTurnOutType == CT_HU_DIE)
			{
				tagAnalyseResult TurnAnalyseResult;
				AnalysebCardData(cbTurnCardData,cbTurnCardCount,TurnAnalyseResult);

				if( SearchHuDie(cbHandCardData,cbHandCardCount,TurnAnalyseResult.cbCardData[2][0],OutCardResult,cbTurnCardCount) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}
			}
		}
	case CT_SINGLE_LINK:			//��˳
	case CT_DOUBLE_LINK:			//˫��
	case CT_THREE_LINK:				//����
		{
			//ֻ�г������Ͷ�Ӧ,������
			if( cbTurnOutType == CT_SINGLE_LINK || cbTurnOutType == CT_DOUBLE_LINK || cbTurnOutType == CT_THREE_LINK)
			{
				//Ѱ�ҶԱ���
				BYTE byReferCard = cbTurnCardData[0];
				if( GetCardValue(cbTurnCardData[0])==m_cbMainValue )
				{
					BYTE nCellCount = cbTurnOutType-4;
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
					if( SearchTongHuaShun(cbCardData,cbCardCount,0,	OutCardResult) )
					{
						ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
						return true;
					}
				}
			}
		}
	case CT_TONG_HUA_SHUN:
		{
			//ֻ�г������Ͷ�Ӧ,������
			if( cbTurnOutType==CT_TONG_HUA_SHUN )
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
				if( SearchTongHuaShun(cbCardData,cbCardCount,byReferCard,OutCardResult) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}
			}
		}
	case CT_BOMB_6:
		{
			if(cbTurnOutType<=CT_BOMB_6)
			{
				for(BYTE i=5;i<8;i++)
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
	case CT_PAI_PAO:
		{
			//ֻ�г������Ͷ�Ӧ,������
			if( cbTurnOutType <= CT_PAI_PAO )
			{
				if(SearchPaiPao(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult))
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}
			}
		}
	case CT_QING_DUN:
		{
			//Ѱ�Ҷ���
			BYTE byReferCard = cbTurnCardData[0];
			if( cbTurnOutType < CT_QING_DUN ) byReferCard=0;
			if( SearchDong(cbCardData,cbCardCount,byReferCard,OutCardResult) )
			{
				ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
				return true;
			}	
		}
	case CT_BOMB_8:
		{
			//Ѱ�Ҷ���
			BYTE byReferCard = cbTurnCardData[0];
			if( cbTurnOutType < CT_BOMB_8 ) byReferCard=0;		
			if( SearchBomb8(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult) )
			{
				ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
				return true;
			}	
		}
	case CT_LIAN_DUN:
		{
			//��������
			if( cbTurnOutType <= CT_LIAN_DUN )
			{
				if( SearchLianDun(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}	
			}
		}
	case CT_XIONGDI_DUN:
		{
			//�����ֵܶ�
			if( cbTurnOutType <= CT_XIONGDI_DUN )
			{
				if( SearchXiongdiDun(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult) )
				{
					ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
					return true;
				}	
			}
		}
	}

//	if( cbTurnOutType < CT_BOMB_TW )
//	{
//		//�������
//		if( SearchMagicOutCard(cbCardData,cbCardCount,cbTurnCardData,cbTurnCardCount,OutCardResult) )
//		{
//#ifdef _DEBUG
//			BYTE byCardData[MAX_COUNT];
//			ASSERT( MagicCardData(OutCardResult.cbResultCard,OutCardResult.cbCardCount,byCardData,0) );
//			ASSERT( CompareCard(cbTurnCardData,byCardData,cbTurnCardCount,OutCardResult.cbCardCount) );
//#endif
//			return true;
//		}
//		//����������
//		if( cbTurnOutType < CT_BOMB_TW && SearchTianWang(cbCardData,cbCardCount,OutCardResult) )
//		{
//			ASSERT( CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount) );
//			return true;
//		}
//	}
	return false;
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

	//SortCardList(cbCardDataTemp,cbCardCount,ST_ORDER);
	//if(GetCardValue(cbCardDataTemp[0])==2) return false;

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
//����ͬ��˳
bool CGameLogic::SearchTongHuaShun( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult )
{
	//A�ⶥ,ֱ�ӷ���
	if( GetCardValue(cbReferCard) == 1 ) return false;
	if( cbHandCardCount < 5 ) return false;

	//�����˿�
	tagDistributing Distribute;
	AnalysebDistributing( cbHandCardData,cbHandCardCount,Distribute );

	//ȷ��������ʼλ��
	BYTE byReferIndex = 0;
	if( cbReferCard != 0 ) 
		byReferIndex = GetCardValue(cbReferCard)-4;

	BYTE byColorCount[4] = {0,0,0,0};
	//��������ͬ��˳
	for( BYTE i = byReferIndex; i < 13; i++ )
	{
		if( Distribute.cbDistributing[i][cbIndexCount] == 0 ) 
		{
			ZeroMemory(byColorCount,sizeof(byColorCount));
			continue;
		}
		
		for( BYTE j = 0; j < 4; j++ )
		{
			if( Distribute.cbDistributing[i][j] == 0 )
			{
				byColorCount[j] = 0;
				continue;
			}
			//�ж��Ƿ������㹻
			if( ++byColorCount[j] == 5 )
			{
				OutCardResult.cbCardCount = 0;
				for( int k = i; k >= i-4; k-- )
					OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(k,j);
				SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
				return true;
			}
		}
	}
	//����10��A��ͬ��˳
	for( i = 0; i < 4; i++ )
	{
		//�����˳
		if( byColorCount[i] == 4 &&
			Distribute.cbDistributing[0][i] > 0 )
		{
			OutCardResult.cbCardCount = 1;
			OutCardResult.cbResultCard[0] = MakeCardData(0,i);
			for( BYTE j = 12; j >= 9; j-- )
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(j,i);
			return true;
		}
	}
	return false;
}

//��������
bool CGameLogic::SearchLinkCard( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,BYTE cbCardType,tagOutCardResult &OutCardResult  ,BYTE TurnCardCount)
{
	//A�ⶥ,ֱ�ӷ���
	if( GetCardValue(cbReferCard) == 1 ) return false;


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
	case CT_THREE_LINK:
		{
			byCellCount = 3;
			byLinkCount = TurnCardCount/byCellCount;
			break;
		}
	case CT_PAI_PAO:
		{
			byCellCount = 4;
			byLinkCount = TurnCardCount/byCellCount;
			break;	
		}
	default:
		ASSERT( FALSE );
		return false;
	}
	if( cbHandCardCount < byCellCount*byLinkCount ) return false;

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
	return false;
}

//������«
bool CGameLogic::SearchHuLu( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult  ,BYTE TurnCardCount)
{
	//��֤
	if( cbHandCardCount < TurnCardCount ) return false;

	//�����˿�
	tagDistributing Distributing;
	AnalysebDistributing(cbHandCardData,cbHandCardCount,Distributing);

	//��������
	BYTE cbThreeIndex=0xFF;
	BYTE cbDoubleIndex=0xFF;

	//������«,��2��K,��������
	for (BYTE cbValueIndex=0;cbValueIndex<13;cbValueIndex++)
	{
		//�ų�����
		if( cbValueIndex+1 == m_cbMainValue ) continue;
		//��������
		if ((cbThreeIndex==0xFF)&&(Distributing.cbDistributing[cbValueIndex][cbIndexCount]==3))		//ע:���Ǵ���3,��Ϊ����3��,����������ը��
		{
			//��С�ж�
			BYTE cbHeadCard=MakeCardData(cbValueIndex,0);
			if ((cbReferCard==0x00)||(CompareCard(cbReferCard,cbHeadCard)==true))
			{
				cbThreeIndex=cbValueIndex;
				//cbValueIndex = 0;
				//continue;
			}
		}

		if( cbThreeIndex <= 11&&( Distributing.cbDistributing[cbThreeIndex+1][5]>=2)&&( Distributing.cbDistributing[cbThreeIndex+1][5]<=3))
		{
			cbDoubleIndex = cbThreeIndex+1;
			break;
		}
		else if( cbThreeIndex >= 1&&Distributing.cbDistributing[cbThreeIndex-1][5]>=2&&Distributing.cbDistributing[cbThreeIndex-1][5]<=3 )
		{
			cbDoubleIndex = cbThreeIndex-1;
			break;
		}
		else if( cbThreeIndex != 0xFF ) cbThreeIndex = 0xFF;
		////��������
		//if ((cbDoubleIndex==0xFF)&&cbValueIndex!=cbThreeIndex&&(Distributing.cbDistributing[cbValueIndex][5]>=2)&&
		//	Distributing.cbDistributing[cbValueIndex][5]<=3)
		//{
		//	cbDoubleIndex=cbValueIndex;
		//	continue;
		//}

		//����ж�
		//if ((cbThreeIndex!=0xFF)&&(cbDoubleIndex!=0xFF))
		//{
		//	if(cbThreeIndex==(cbDoubleIndex+1) || cbThreeIndex==(cbDoubleIndex-1))
		//		break;
		//	else cbDoubleIndex = 0xFF;
		//}
	}
	//����A������
	//cbValueIndex = 0xFF;
	//if( cbThreeIndex == 0xFF )
	//{
	//	if( Distributing.cbDistributing[0][cbIndexCount] == 3 )
	//		cbValueIndex = 0;
	//	else if( Distributing.cbDistributing[m_cbMainValue-1][cbIndexCount] == 3 )
	//		cbValueIndex = m_cbMainValue-1;
	//	if( cbValueIndex != 0xFF )
	//	{
	//		BYTE cbHeadCard = MakeCardData(0,0);
	//		if( (cbReferCard==0x00) || CompareCard(cbReferCard,cbHeadCard) )
	//			cbThreeIndex = cbValueIndex;
	//	}
	//}
	//cbValueIndex = 0xFF;
	//if( cbDoubleIndex == 0xFF )
	//{
	//	if( Distributing.cbDistributing[0][cbIndexCount] >= 2 && Distributing.cbDistributing[0][cbIndexCount] <= 3 
	//		&& 0 != cbThreeIndex )
	//		cbValueIndex = 0;
	//	else if( Distributing.cbDistributing[m_cbMainValue-1][cbIndexCount] >= 2 && Distributing.cbDistributing[m_cbMainValue-1][cbIndexCount] <= 3
	//		&& cbThreeIndex != m_cbMainValue-1)
	//		cbValueIndex = m_cbMainValue-1;
	//	if( cbValueIndex != 0xFF )
	//	{
	//		cbDoubleIndex = cbValueIndex;
	//	}
	//}

	//�����«
	if ((cbThreeIndex!=0xFF)&&(cbDoubleIndex!=0xFF)&&cbThreeIndex==(cbDoubleIndex+1) || cbThreeIndex==(cbDoubleIndex-1))
	{
		//��������
		BYTE cbThreeCount=0;
		BYTE cbDoubleCount=0;

		//��������
		for (BYTE cbColorIndex=3;;cbColorIndex=(cbColorIndex+1)%4)
		{
			//�˿��ж�
			if (Distributing.cbDistributing[cbThreeIndex][cbColorIndex]==0) continue;
			
			//�����˿�
			//BYTE cbIndex=(cbThreeIndex>cbDoubleIndex)?0:2;
			BYTE cbIndex= 0;
			for( BYTE i = 0; i < Distributing.cbDistributing[cbThreeIndex][cbColorIndex]; i++ )
			{
				OutCardResult.cbResultCard[cbIndex+cbThreeCount++]=MakeCardData(cbThreeIndex,cbColorIndex);
				if( cbThreeCount == 3 ) break;
			}

			//����ж�
			if (cbThreeCount==3) break;
		}

		//�������
		for (BYTE cbColorIndex=3;;cbColorIndex=(cbColorIndex+1)%4)
		{
			//�˿��ж�
			if (Distributing.cbDistributing[cbDoubleIndex][cbColorIndex]==0) continue;
			
			//�����˿�
			//BYTE cbIndex=(cbThreeIndex>cbDoubleIndex)?3:0;
			BYTE cbIndex= 3;
			for( BYTE i = 0; i < Distributing.cbDistributing[cbDoubleIndex][cbColorIndex]; i++ )
			{
				OutCardResult.cbResultCard[cbIndex+cbDoubleCount++]=MakeCardData(cbDoubleIndex,cbColorIndex);
				if( cbDoubleCount == 2 ) break;
			}

			//����ж�
			if (cbDoubleCount==2) break;
		}
		OutCardResult.cbCardCount = 5;
		SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
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
	case CT_THREE:				//��ͬ��
		{
			BYTE cbSameCount = cbTurnCardType;
			//����ͬ��
			if(  cbTurnCardType!=CT_SINGLE && SearchMagicSameCard(cbHandCardData,cbHandCardCount,cbTurnCardData[0],cbSameCount,OutCardResult) )
				return true;
		}
	case CT_THREE_DOUBLE:		//��«
		{
			//ֻ�г�������Ϊ��«ʱ,�Ž�������
			if( cbTurnCardType == CT_THREE_DOUBLE )
			{
				if( SearchMagicHuLu(cbHandCardData,cbHandCardCount,cbTurnCardData[0],OutCardResult) )
					return true;
			}
		}
	case CT_SINGLE_LINK:		//����
	case CT_DOUBLE_LINK:		//˫��
	case CT_THREE_LINK:			//��˳
		{
			//ֻ�г����������Ӧ,������
			if( cbTurnCardType >= CT_SINGLE_LINK && cbTurnCardType <= CT_THREE_LINK )
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
				if( SearchMagicTongHuaShun(cbHandCardData,cbHandCardCount,0,OutCardResult) )
					return true;
			}
			//����ͬ��
			else if( SearchMagicSameCard(cbHandCardData,cbHandCardCount,cbReferCard,cbSameCount,OutCardResult) )
				return true;
			break;
		}
	}

	return false;
}

//���������«
bool CGameLogic::SearchMagicHuLu( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult )
{
	//��֤
	if( cbHandCardCount < 5 ) return false;

	//��������
	tagMagicResult MagicResult;
	tagAnalyseResult AnalyseResult;
	
	//�����˿�
	AnalyseMagicData(cbHandCardData,cbHandCardCount,MagicResult);
	if( MagicResult.cbMagicCardCount == 0 ) return false;

	//�����˿�
	AnalysebCardData(MagicResult.cbNormalCard,MagicResult.cbNormalCardCount,AnalyseResult);

	//����
	BYTE byMagicCount = 0;
	if( AnalyseResult.cbBlockCount[1] > 0 )
	{
		BYTE byThreeIndex = 0xFF;
		for( int i = AnalyseResult.cbBlockCount[1]-1; i >= 0; i-- )
		{
			//�ų���С��
			if( GetCardLogicValue(AnalyseResult.cbCardData[1][2*i]) > 14 ) continue;
			if( CompareCard(cbReferCard,AnalyseResult.cbCardData[1][2*i]) )
			{
				//��������
				byThreeIndex = i;
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MagicResult.cbMagicCard[byMagicCount++];
				CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount],
					&AnalyseResult.cbCardData[1][2*i],sizeof(BYTE)*2);
				OutCardResult.cbCardCount += 2;
				break;
			}
		}
		//����2��ʧ��ʱ
		if( i < 0 )
		{
			if( MagicResult.cbMagicCardCount == 2 )
			{
				for( i = AnalyseResult.cbBlockCount[0]-1; i >= 0; i-- )
				{
					//�ų���С��
					if( GetCardLogicValue(AnalyseResult.cbCardData[0][i]) > 14 ) continue;
					//��������
					if( CompareCard(cbReferCard,AnalyseResult.cbCardData[0][i]) )
					{
						CopyMemory(OutCardResult.cbResultCard,MagicResult.cbMagicCard,sizeof(BYTE)*2);
						byMagicCount = 2;
						OutCardResult.cbCardCount = 2;
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = AnalyseResult.cbCardData[0][i];
						break;
					}
				}
				if( i < 0 ) return false;
			}
			else return false;
		}
		//������ж�������Ż�����С�ڳ����ߵ�����
		if( AnalyseResult.cbBlockCount[1] > 1 || CompareCard(cbReferCard,AnalyseResult.cbCardData[1][0])==false )
		{
			for( int i = AnalyseResult.cbBlockCount[1]-1; i >= 0; i-- )
			{
				if( i != byThreeIndex )
				{
					CopyMemory(&OutCardResult.cbResultCard[OutCardResult.cbCardCount],
						&AnalyseResult.cbCardData[1][i*2],sizeof(BYTE)*2);
					OutCardResult.cbCardCount += 2;
					SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
					return true;
				}
			}
		}
		//������Ų���
		else if( MagicResult.cbMagicCardCount > byMagicCount )
		{
			if( GetCardLogicValue(AnalyseResult.cbCardData[0][0]) <= 15 )
			{
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MagicResult.cbMagicCard[byMagicCount++];
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = AnalyseResult.cbCardData[0][0];
				SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
				return true;
			}
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
	case CT_THREE_LINK:
		{
			byCellCount = 3;
			byLinkCount = 2;
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

//�������ͬ��˳
bool CGameLogic::SearchMagicTongHuaShun( const BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbReferCard,tagOutCardResult &OutCardResult )
{
	//A�ⶥ,ֱ�ӷ���
	if( GetCardValue(cbReferCard) == 1 ) return false;
	if( cbHandCardCount < 5 ) return false;

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
		byReferIndex = GetCardValue(cbReferCard)-4;

	//�����յ�
	BYTE byColorCount[4] = {0,0,0,0};
	BYTE byMagicIndex = 0xFF;
	BYTE byTmpMagicIndex = 0xFF;
	bool bTwice=byReferIndex==0?true:false;						//����byReferIndex==0ʱ
	for( BYTE byMagicCount = 1; byMagicCount <= MagicResult.cbMagicCardCount; byMagicCount++ )
	{
		//��������ͬ��˳
		BYTE i = byReferIndex;
		do
		{
			if( Distribute.cbDistributing[i][cbIndexCount] > 0 )
			{
				for( BYTE j = 0; j < 4; j++ )
				{
					if( Distribute.cbDistributing[i][j] == 0 ) continue;
					//�ɱ任
					if( ++byColorCount[j] == 5 - byMagicCount )
					{
						//�����
						BYTE byInvalidCount = 0;	//ȱλ��
						BYTE byLianCount = 1;		//������
						BYTE byBeginIndex = i==0?12:i-1;
						BYTE byEndIndex = byBeginIndex-3;
						for( BYTE k = byBeginIndex; k >= byEndIndex; k-- )
						{
							if( Distribute.cbDistributing[k][cbIndexCount]==0 || Distribute.cbDistributing[k][j]==0 )
							{
								if( ++byInvalidCount > byMagicCount ) break;
								if( byLianCount != 0 )
								{
									byColorCount[j] = byLianCount;
									byLianCount = 0;
								}
							}
							else if( byLianCount != 0 ) byLianCount++;
						}
						//�����ɱ任
						if( byInvalidCount <= byMagicCount )
						{
							//��λ�任λ��
							k = byEndIndex;
							if( byColorCount[j] == 5-byMagicCount ) k += byMagicCount;
							//��������
							BYTE byCount = 0;
							BYTE byMagicIndex = 0;
							do
							{
								if( Distribute.cbDistributing[k][cbIndexCount]>0 && Distribute.cbDistributing[k][j]>0 )
									OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(k,j);
								else OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MagicResult.cbMagicCard[byMagicIndex++];
								k = (k+1)%13;
							}while( ++byCount < 5 );
							//����
							SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount,ST_ORDER);
							return true;
						}
					}
				}
			}
			if( i==1 && bTwice ) bTwice = false;
			i = (i+1)%13;
		}while( i!=1 || bTwice );
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
