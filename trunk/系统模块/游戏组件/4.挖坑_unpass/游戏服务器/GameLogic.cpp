#include "StdAfx.h"
#include "GameLogic.h"

//Ӧ�ó������
//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_bCardListData[ALL_CARD_NUM]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	//0x41,0x42,//��С��
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
//�Ƿ��ж���
//������>= 2
bool CGameLogic::IsHaveDuiCard(const BYTE bCardData[], BYTE bCardCount)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sIsHaveDuiCard.log",strTime);

			strTemp.Format("IsHaveDuiCard %d", bCardCount);

	bool resultBool = false;
	if (bCardCount < 2 )
	{
		return resultBool;
	}	

	//��������
	if (bCardCount>=3)
	{
		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(bCardData,bCardCount,AnalyseResult);

			strTemp.Format("2=%d, 4=%d, 3=%d", AnalyseResult.bDoubleCount,
			  AnalyseResult.bFourCount,
			  AnalyseResult.bThreeCount  );

		if ( (AnalyseResult.bDoubleCount > 0) 
			 || (AnalyseResult.bFourCount > 0)
			 || (AnalyseResult.bThreeCount > 0) )
		{
		
			resultBool = true;
		}

	}//End if (bCardCount>=3)
	return resultBool;

}

//�Ƿ�Ϊ˫��
//������>= 6
bool CGameLogic::IsShuangLianCardType(const BYTE bCardData[], BYTE bCardCount)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sIsShuangLianCardType.log",strTime);

			strTemp.Format("IsShuangLianCardType %d", bCardCount);

	bool resultBool = false;
//	if (bCardCount < 6 ||  (bCardCount % 2) != 0 )
	if (bCardCount < 6  )
	{
			strTemp.Format("(bCardCount=%d % 2 = %d", bCardCount, (bCardCount % 3) );
			//theApp.WriteLog(strFile, strTemp);

		return resultBool;
	}
	//��������
	if (bCardCount>=6)
	{
		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(bCardData,bCardCount,AnalyseResult);

			strTemp.Format("AnalyseResult.bDoubleCount=%d", AnalyseResult.bDoubleCount );
//			theApp.WriteLog(strFile, strTemp);

		if (AnalyseResult.bDoubleCount>=3)
		{
			//�����ж�
			if (AnalyseResult.bDoubleLogicVolue[0]!=15)
			{
				for (BYTE i=1;i<AnalyseResult.bDoubleCount;i++)
				{
					if (AnalyseResult.bDoubleLogicVolue[i]!=(AnalyseResult.bDoubleLogicVolue[0]-i)) 
					{
						return resultBool;
					}
				}
				if (AnalyseResult.bDoubleCount*2==bCardCount) 
				{
					resultBool = true;
					return resultBool;
				}
			}//End if (AnalyseResult.bDoubleLogicVolue[0]!=15)
			
		}//End if (AnalyseResult.bDoubleCount>=3)
		return resultBool;
	}
	return resultBool;
}

	//�Ƿ�Ϊ����
	//������>= 9
bool CGameLogic::IsThreeLianCardType(const BYTE bCardData[], BYTE bCardCount)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sIsThreeLianCardType.log",strTime);

	strTemp.Format("IsThreeLianCardType %d", bCardCount);

	bool resultBool = false;
//	if (bCardCount < 9 ||  (bCardCount % 3)!= 0 )
	if (bCardCount < 9 )
	{
	strTemp.Format("(bCardCount % 3) = %d", (bCardCount % 3));
	//theApp.WriteLog(strFile, strTemp);

		return resultBool;
	}
	//��������
	if (bCardCount>=9)
	{

//�����ж�
		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(bCardData,bCardCount,AnalyseResult);

	strTemp.Format("AnalyseResult.bThreeCount = %d", AnalyseResult.bThreeCount);

		if (AnalyseResult.bThreeCount>0)
		{
			//�����ж�
			if (AnalyseResult.bThreeCount>1)
			{
				if (AnalyseResult.bThreeLogicVolue[0]==15) 
				{
					return resultBool;
				}
				for (BYTE i=1;i<AnalyseResult.bThreeCount;i++)
				{
					if (AnalyseResult.bThreeLogicVolue[i]!=(AnalyseResult.bThreeLogicVolue[0]-i)) 
					{
						return resultBool;
					}
				}
			}//End if (AnalyseResult.bThreeCount>1)

			//�����ж�
			if (AnalyseResult.bThreeCount*3==bCardCount)
			{
				resultBool = true;
			}


		}//End if (AnalyseResult.bThreeCount>0)


	}//End if (bCardCount>=9)
	return resultBool;
}
//�Ƿ�Ϊ����
//������>= 3
bool CGameLogic::IsDanLianCardType(const BYTE bCardData[], BYTE bCardCount)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sIsDanLianCardType.log",strTime);

			strTemp.Format("IsDanLianCardType %d", bCardCount);

	bool resultBool = false;
	if (bCardCount < 3 )
	{
		return resultBool;
	}

	//����ж���
	if( IsHaveDuiCard( bCardData ,  bCardCount))
	{
			strTemp.Format("IsHaveDuiCard ");

		return resultBool;
	}


	//��������
	if (bCardCount>=3)
	{
		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(bCardData,bCardCount,AnalyseResult);

			strTemp.Format("AnalyseResult.bSignedCount=%d, bCardCount=%d ", AnalyseResult.bSignedCount, bCardCount);

		//�����ж�
		if ((AnalyseResult.bSignedCount>=3)&&(AnalyseResult.bSignedCount==bCardCount))
		{
			BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

			strTemp.Format("bLogicValue =%d ", bLogicValue);

			if (bLogicValue>=15) 
			{
				return resultBool;
			}//End if
			for (BYTE i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-i)) 
				{
			strTemp.Format("GetCardLogicValue(bCardData[i])=%d, (bLogicValue-i)=%d, i=%d", GetCardLogicValue(bCardData[i]), (bLogicValue-i), i);

					return resultBool;
				}
			}//End for

			resultBool = true;
			return resultBool;
		}//End if ((AnalyseResult.bSignedCount>=3)&&(AnalyseResult.bSignedCount==bCardCount))


	}//End if (bCardCount>=3)
	return resultBool;

}

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE bCardData[], BYTE bCardCount)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sGetCardType.log",strTime);

	strTemp.Format("GetCardType %d", bCardCount);

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
			//�����ж�
			if (IsDanLianCardType( bCardData,  bCardCount))
			{
	strTemp.Format("IsDanLianCardType %d", bCardCount);

				return CT_ONE_LINE;
			}

			BYTE bLogicValue=GetCardLogicValue(bCardData[0]);
			BYTE i;
			for ( i=1;i<bCardCount;i++)
			{
				if (bLogicValue!=GetCardLogicValue(bCardData[i])) break;
			}
			if (i==bCardCount) return (bCardCount==3)?CT_THREE:CT_BOMB_CARD;
			if (bCardCount==3) return CT_INVALID;
			break;
		}
	case 5:
		{
			//�����ж�
			if (IsDanLianCardType( bCardData,  bCardCount))
			{
	strTemp.Format("IsDanLianCardType %d", bCardCount);
				return CT_ONE_LINE;
			}		
			break;
		}
	}

	//��������
	if (bCardCount>=4)
	{
		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(bCardData,bCardCount,AnalyseResult);

	strTemp.Format("AnalyseResult.bFourCount %d, AnalyseResult.bThreeCount=%d, AnalyseResult.bDoubleCount=%d", AnalyseResult.bFourCount, AnalyseResult.bThreeCount,AnalyseResult.bDoubleCount );

		//�����ж�
		if (AnalyseResult.bFourCount>0)
		{

			return CT_INVALID;
		}

		//�����ж�
if (AnalyseResult.bThreeCount>0)
{
		if (IsThreeLianCardType(  bCardData,  bCardCount))
		{
			return CT_THREE_LINE;
		}
}
		/*
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
			if (AnalyseResult.bThreeCount==(bCardCount-AnalyseResult.bThreeCount*3)) return CT_THREE_LINE_TAKE_ONE;
			if ((AnalyseResult.bDoubleCount==AnalyseResult.bThreeCount)&&(bCardCount==(AnalyseResult.bThreeCount*3+AnalyseResult.bDoubleCount*2))) return CT_THREE_LINE_TAKE_DOUBLE;

			return CT_INVALID;
		}
*/
		//��������
		if (AnalyseResult.bDoubleCount>=3)
		{
		if ( IsShuangLianCardType(  bCardData,  bCardCount))
		{
			return CT_DOUBLE_LINE;
		}
		}
		/*
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
		*/
		//�����ж�
if ((AnalyseResult.bSignedCount>= 3 )&&(AnalyseResult.bSignedCount==bCardCount))
{
		if (IsDanLianCardType( bCardData,  bCardCount))
		{
			return CT_ONE_LINE;
		}
}
		/*
		if ((AnalyseResult.bSignedCount>= 3 )&&(AnalyseResult.bSignedCount==bCardCount))
		{
			BYTE bLogicValue=GetCardLogicValue(bCardData[0]);
			if (bLogicValue>=15) return CT_INVALID;
			for (BYTE i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-i)) return CT_INVALID;
			}

			return CT_ONE_LINE;
		}
		*/

		
		return CT_INVALID;
	}

	return CT_INVALID;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//ת����ֵ
	BYTE bLogicVolue[ZHUANG_CARD_NUM /*20*/];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);

	//�������
	bool bSorted=true;
	BYTE bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((bLogicVolue[i]<bLogicVolue[i+1])||
			   ((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//����λ��
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicVolue[i];
				bLogicVolue[i]=bLogicVolue[i+1];
				bLogicVolue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

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
	BYTE bDeleteCount=0,bTempCardData[ZHUANG_CARD_NUM /*20*/];
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
//�����Ĵ�С�� 3>2>A>K>Q>J>10>9>8>7>6>5>4
//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//ת����ֵ
	if (bCardColor==0x40) return bCardValue+15;
	return (bCardValue<=3)?(bCardValue+13):bCardValue;
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
	case CT_THREE_LINE_TAKE_ONE:
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
*/
//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE bFirstList[], const BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//�����ж�
	if (bFirstType==CT_INVALID) return false;
//	if (bFirstType==CT_MISSILE_CARD) return true;

	//ը���ж�
//Del by doctor 20071202	if ((bFirstType==CT_BOMB_CARD)&&(bNextType!=CT_BOMB_CARD)) return true;
//Del by doctor 20071202	if ((bFirstType!=CT_BOMB_CARD)&&(bNextType==CT_BOMB_CARD)) return false;

	//�����ж�
	if ((bFirstType!=bNextType)||(bFirstCount!=bNextCount)) return false;

	//��ʼ�Ա�
	switch (bNextType)
	{
	case CT_SINGLE://��
	case CT_DOUBLE://��
	case CT_THREE://��

	case CT_ONE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:

	case CT_BOMB_CARD://4�ƣ�ը��
		{
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			return bFirstLogicValue>bNextLogicValue;
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
		}//End if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))

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
			}//End if
			bSameCount=1;
			bLogicValue=bCardValueTemp;

		}//End if (bCardValueTemp!=bLogicValue)
	}//End for (BYTE i=1;i<bCardCount;i++)

	//������Ŀ
	BYTE bOtherCount=AnalyseResult.bDoubleCount*2+AnalyseResult.bThreeCount*3+AnalyseResult.bFourCount*4;
	return;
}

//////////////////////////////////////////////////////////////////////////
