#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE	CGameLogic::m_bCardListData[52]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
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
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount)
{
	//����У��
	ASSERT(bCardCount==3 || bCardCount==5) ;
	if(bCardCount!=3 && bCardCount!=5) return CT_INVALID ;

	tagAnalyseData AnalyseData ;
	memset(&AnalyseData , 0 , sizeof(tagAnalyseData)) ;

	ASSERT(3==bCardCount || 5==bCardCount) ;

	AnalyseCard(bCardData , bCardCount , AnalyseData) ;

	//��ʼ����
	switch (bCardCount)
	{
	case 3:	//��������
		{
			//��������
			if(3==AnalyseData.bOneCount) return CT_SINGLE ;
			
			//�Դ�һ��
			if(1==AnalyseData.bTwoCount && 1==AnalyseData.bOneCount) return CT_ONE_DOUBLE ;

			//��������
			if(1==AnalyseData.bThreeCount) return CT_THREE ;

			//��������
			return CT_INVALID ;
		}
	case 5:	//��������
		{
			bool bFlushNoA	    = false , 
				 bFlushFirstA	= false ,
				 bFlushBackA	= false ;

			//A���ں�
			if(14==GetCardLogicValue(bCardData[0]) && 10==GetCardLogicValue(bCardData[4]))
				bFlushBackA = true ;
			else
				bFlushNoA = true ;
			for(BYTE i=0 ; i<4 ; ++i)
				if(1!=GetCardLogicValue(bCardData[i])-GetCardLogicValue(bCardData[i+1])) 
				{
					bFlushBackA = false ;
					bFlushNoA   = false ;
				}
			//A����ǰ
			if(false==bFlushBackA && false==bFlushNoA && 14==GetCardLogicValue(bCardData[0]))
			{
				bFlushFirstA = true ;
				for(BYTE i=1 ; i<4 ; ++i) if(1!=GetCardLogicValue(bCardData[i])-GetCardLogicValue(bCardData[i+1])) bFlushFirstA = false ;
				if(2!=GetCardLogicValue(bCardData[4])) bFlushFirstA = false ;
			}

			//ͬ������
			if(false==bFlushBackA && false==bFlushNoA && false==bFlushFirstA)
			{
				if(true==AnalyseData.bStraight) return CT_FIVE_FLUSH ;
			}
			else if(true==bFlushNoA)
			{
				//��˳����
				if(false==AnalyseData.bStraight) return CT_FIVE_MIXED_FLUSH_NO_A;
				//ͬ��˳��
				else							 return CT_FIVE_STRAIGHT_FLUSH ;
			}
			else if(true==bFlushFirstA)
			{
				//��˳����
				if(false==AnalyseData.bStraight) return CT_FIVE_MIXED_FLUSH_FIRST_A;
				//ͬ��˳��
				else							 return CT_FIVE_STRAIGHT_FLUSH_FIRST_A ;
			}
			else if(true==bFlushBackA)
			{
				//��˳����
				if(false==AnalyseData.bStraight) return CT_FIVE_MIXED_FLUSH_BACK_A;
				//ͬ��˳��
				else							 return CT_FIVE_STRAIGHT_FLUSH ;
			}

			//�Ĵ�����
			if(1==AnalyseData.bFourCount && 1==AnalyseData.bOneCount) return	 CT_FIVE_FOUR_ONE ;

			//����һ��
			if(1==AnalyseData.bThreeCount && 1==AnalyseData.bTwoCount) return	 CT_FIVE_THREE_DEOUBLE ;

			//��������
			if(1==AnalyseData.bThreeCount && 2==AnalyseData.bOneCount) return	 CT_THREE ;


			//��������
			if(2==AnalyseData.bTwoCount && 1==AnalyseData.bOneCount) return		 CT_FIVE_TWO_DOUBLE ;


			//ֻ��һ��
			if(1==AnalyseData.bTwoCount && 3==AnalyseData.bOneCount) return		 CT_ONE_DOUBLE ;

			//��������
			if(5==AnalyseData.bOneCount && false==AnalyseData.bStraight) return  CT_SINGLE ;

			//��������
			return CT_INVALID;
		}
	}

	return CT_INVALID;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType)
{
	ASSERT(bCardCount>=1 && bCardCount<=13) ;
	if(bCardCount<1 || bCardCount>13) return ;

	//ת����ֵ
	BYTE bLogicVolue[13];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);	

	if(enDescend==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
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
	}
	else if(enAscend==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
		do
		{
			bSorted=true;
			for (BYTE i=0;i<bLast;i++)
			{
				if ((bLogicVolue[i]>bLogicVolue[i+1])||
					((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]>bCardData[i+1])))
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
	}
	else if(enColor==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
		BYTE bColor[13];
		for (BYTE i=0;i<bCardCount;i++)	bColor[i]=GetCardColor(bCardData[i]);	
		do
		{
			bSorted=true;
			for (BYTE i=0;i<bLast;i++)
			{
				if ((bColor[i]<bColor[i+1])||
					((bColor[i]==bColor[i+1])&&(GetCardLogicValue(bCardData[i])<GetCardLogicValue(bCardData[i+1]))))
				{
					//����λ��
					bTempData=bCardData[i];
					bCardData[i]=bCardData[i+1];
					bCardData[i+1]=bTempData;
					bTempData=bColor[i];
					bColor[i]=bColor[i+1];
					bColor[i+1]=bTempData;
					bSorted=false;
				}	
			}
			bLast--;
		} while(bSorted==false);
	}
#ifdef _DEBUG
	else AfxMessageBox("�����������ͣ�") ;
#endif
	

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//����׼��
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));

	srand((unsigned)time(NULL));

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
	BYTE bDeleteCount=0,bTempCardData[13];
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
	BYTE bCardValue=GetCardValue(bCardData); 

	//ת����ֵ
	return (bCardValue==1)?(bCardValue+13):bCardValue; 
}

/*
	����ֵ:
 *	bNextList>bFirstList:true
	bNextList<bFirstList:false
 */
//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther)
{
	tagAnalyseData FirstAnalyseData , NextAnalyseData;

	memset(&FirstAnalyseData , 0 , sizeof(tagAnalyseData)) ;
	memset(&NextAnalyseData , 0 , sizeof(tagAnalyseData)) ;

	//�����˿�
	BYTE bFirstList[13], bNextList[13] ;
	CopyMemory(bFirstList , bInFirstList , bFirstCount) ;
	CopyMemory(bNextList , bInNextList , bNextCount) ;
	SortCardList(bFirstList , bFirstCount , enDescend) ;
	SortCardList(bNextList , bNextCount , enDescend) ;

	ASSERT(3==bFirstCount || 5==bFirstCount || 3==bNextCount || 5==bNextCount) ;

	AnalyseCard(bFirstList , bFirstCount , FirstAnalyseData) ;

	AnalyseCard(bNextList  , bNextCount  , NextAnalyseData) ;

	ASSERT(bFirstCount==(FirstAnalyseData.bOneCount+FirstAnalyseData.bTwoCount*2+FirstAnalyseData.bThreeCount*3+FirstAnalyseData.bFourCount*4+FirstAnalyseData.bFiveCount*5)) ;
	ASSERT(bNextCount=(NextAnalyseData.bOneCount+NextAnalyseData.bTwoCount*2+NextAnalyseData.bThreeCount*3+NextAnalyseData.bFourCount*4+NextAnalyseData.bFiveCount*5)) ;
	if(bFirstCount!=(FirstAnalyseData.bOneCount+FirstAnalyseData.bTwoCount*2+FirstAnalyseData.bThreeCount*3+FirstAnalyseData.bFourCount*4+FirstAnalyseData.bFiveCount*5))
	{
	//	AfxMessageBox("error") ;
		return false ;
	}
	if(bNextCount != (NextAnalyseData.bOneCount + NextAnalyseData.bTwoCount*2 + NextAnalyseData.bThreeCount*3+NextAnalyseData.bFourCount*4+NextAnalyseData.bFiveCount*5))
	{
	//	AfxMessageBox("error") ;
		return false ;
	}
	//������֤
	ASSERT((bFirstCount==bNextCount) || (bFirstCount!=bNextCount && (3==bFirstCount && 5==bNextCount || 5==bFirstCount && 3==bNextCount))) ;
	if(!((bFirstCount==bNextCount) || (bFirstCount!=bNextCount && (3==bFirstCount && 5==bNextCount || 5==bFirstCount && 3==bNextCount)))) return false ;

	//��ȡ����
	ASSERT(3==bNextCount || 5==bNextCount) ;
	ASSERT(3==bFirstCount || 5==bFirstCount) ;

	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	ASSERT(CT_INVALID!=bNextType && CT_INVALID!=bFirstType) ;
	if(CT_INVALID==bFirstType || CT_INVALID==bNextType) return false ;

	//ͷ�αȽ�
	if(true==bComperWithOther)
	{
		if(3==bFirstCount)
		{
			//��ʼ�Ա�
			if(bNextType==bFirstType) 
			{
				switch(bFirstType) 
				{
				case CT_SINGLE:				//��������
					{
						//������֤
						//������֤
						ASSERT(bNextList[0]!=bFirstList[0]) ;
						if(bNextList[0]==bFirstList[0]) return false ;

						bool bAllSame=true ;

						for(BYTE i=0 ; i<3 ; ++i)
							if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
							{
								bAllSame = false ;
								break; 
							}

							if(true==bAllSame)
								return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
							else
							{
								for(BYTE i=0 ; i<3 ; ++i)
									if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
										return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;
								return false ;
							}

							return false ;

					}
				case CT_ONE_DOUBLE:			//�Դ�һ��
					{
						//������֤
						ASSERT(bNextList[NextAnalyseData.bTwoFirst[0]]!=bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;
						if(bNextList[NextAnalyseData.bTwoFirst[0]]==bFirstList[FirstAnalyseData.bTwoFirst[0]]) return false ;

						if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
						{
							if(GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) != GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
								return GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]) ;
							else 
								return GetCardColor(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;
						}
						else 
							return GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;
					}	

				case CT_THREE:				//��������
					{
						//������֤
						if(bNextList[NextAnalyseData.bThreeFirst[0]]==bFirstList[FirstAnalyseData.bThreeFirst[0]])
						{
#ifdef _DEBUG

							AfxMessageBox("bNextList[NextAnalyseData.bThreeFirst[0]]!=bFirstList[FirstAnalyseData.bThreeFirst[0]]") ;
#endif
						}
						ASSERT(bNextList[NextAnalyseData.bThreeFirst[0]]!=bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;
						if(bNextList[NextAnalyseData.bThreeFirst[0]]==bFirstList[FirstAnalyseData.bThreeFirst[0]]) return false ;

						if(GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
							return GetCardColor(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;			//�Ƚϻ�ɫ
						else
							return GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]); 	//�Ƚ���ֵ
					}		

				}

			}
			else
				return bNextType>bFirstType ;
		}		
		else
		{
			//��ʼ�Ա�
			if(bNextType==bFirstType) 
			{

				switch(bFirstType) 
				{
				case CT_SINGLE:				//��������
					{
						//������֤
						ASSERT(bNextList[0]!=bFirstList[0]) ;
						if(bNextList[0]==bFirstList[0]) return false ;

						bool bAllSame=true ;

						for(BYTE i=0 ; i<5 ; ++i)
							if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
							{
								bAllSame = false ;
								break; 
							}

							if(true==bAllSame)
								return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
							else
							{
								for(BYTE i=0 ; i<5 ; ++i)
									if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
										return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;
								return false ;
							}

							return false ;

					}
				case CT_ONE_DOUBLE:			//�Դ�һ��
					{
						//������֤
						ASSERT(bNextList[NextAnalyseData.bTwoFirst[0]]!=bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;
						if(bNextList[NextAnalyseData.bTwoFirst[0]]==bFirstList[FirstAnalyseData.bTwoFirst[0]]) return false ;

						if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
						{
							//�Աȵ���
							for(BYTE i=0; i<3; ++i)
							{
								if(GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[i]])!=GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[i]]))
									return GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[i]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[i]]) ;
							}
							return GetCardColor(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;			//�Ƚϻ�ɫ
						}
						else
							return GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]); 	//�Ƚ���ֵ
					}
				case CT_FIVE_TWO_DOUBLE:	//��������
					{
						//������֤
						ASSERT(bNextList[NextAnalyseData.bTwoFirst[0]]!=bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;
						if(bNextList[NextAnalyseData.bTwoFirst[0]]==bFirstList[FirstAnalyseData.bTwoFirst[0]]) return false ;

						if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
						{
							if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[1]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[1]]))
							{
								if(GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]])!=GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
									return GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]) ;

								return GetCardColor(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;			//�Ƚϻ�ɫ
							}
							else
								return GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[1]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[1]]); 	//�Ƚ���ֵ
						}							
						else
							return GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]); 	//�Ƚ���ֵ
					}

				case CT_THREE:				//��������
					{
						//������֤
						if(bNextList[NextAnalyseData.bThreeFirst[0]]==bFirstList[FirstAnalyseData.bThreeFirst[0]])
						{
#ifdef _DEBUG

							AfxMessageBox("bNextList[NextAnalyseData.bThreeFirst[0]]!=bFirstList[FirstAnalyseData.bThreeFirst[0]]") ;
#endif
						}
						ASSERT(bNextList[NextAnalyseData.bThreeFirst[0]]!=bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;
						if(bNextList[NextAnalyseData.bThreeFirst[0]]==bFirstList[FirstAnalyseData.bThreeFirst[0]]) return false ;

						if(GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
							return GetCardColor(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;			//�Ƚϻ�ɫ
						else
							return GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]); 	//�Ƚ���ֵ
					}

				case CT_FIVE_MIXED_FLUSH_FIRST_A:				//A��ǰ˳��
					{
						//������֤
						ASSERT(bNextList[0]!=bFirstList[0]) ;
						if(bNextList[0]==bFirstList[0]) return false ;

						if(GetCardLogicValue(bNextList[0]) == GetCardLogicValue(bFirstList[0]))
							return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
						else
							return GetCardLogicValue(bNextList[0]) > GetCardLogicValue(bFirstList[0]) ;	//�Ƚ���ֵ

					}
				case CT_FIVE_MIXED_FLUSH_NO_A:			//ûA��˳
					{
						//������֤
						ASSERT(bNextList[0]!=bFirstList[0]) ;
						if(bNextList[0]==bFirstList[0]) return false ;

						if(GetCardLogicValue(bNextList[0])==GetCardLogicValue(bFirstList[0]))
							return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
						else
							return GetCardLogicValue(bNextList[0]) > GetCardLogicValue(bFirstList[0]); 	//�Ƚ���ֵ
					}
				case CT_FIVE_MIXED_FLUSH_BACK_A:		//A�ں�˳��
					{
						//������֤
						ASSERT(bNextList[0]!=bFirstList[0]) ;
						if(bNextList[0]==bFirstList[0]) return false ;

						if(GetCardLogicValue(bNextList[0])==GetCardLogicValue(bFirstList[0]))
							return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
						else
							return GetCardLogicValue(bNextList[0]) > GetCardLogicValue(bFirstList[0]); 	//�Ƚ���ֵ

					}

				case CT_FIVE_FLUSH:				//ͬ������
					{
						//������֤
						ASSERT(bNextList[0]!=bFirstList[0]) ;
						if(bNextList[0]==bFirstList[0]) return false ;

						//�Ƚ���ֵ
						for(BYTE i=0; i<5; ++i)
							if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
								return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;

						//�Ƚϻ�ɫ
						return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			
					}

				case CT_FIVE_THREE_DEOUBLE:			//����һ��
					{
						//������֤
						ASSERT(bNextList[NextAnalyseData.bThreeFirst[0]]!=bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;
						if(bNextList[NextAnalyseData.bThreeFirst[0]]==bFirstList[FirstAnalyseData.bThreeFirst[0]]) return false ;

						if(GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
							return GetCardColor(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;			//�Ƚϻ�ɫ
						else
							return GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]); 	//�Ƚ���ֵ
					}

				case CT_FIVE_FOUR_ONE:			//�Ĵ�һ��
					{
						//������֤
						ASSERT(bNextList[NextAnalyseData.bFourFirst[0]]!=bFirstList[FirstAnalyseData.bFourFirst[0]]) ;
						if(bNextList[NextAnalyseData.bFourFirst[0]]==bFirstList[FirstAnalyseData.bFourFirst[0]]) return false ;

						if(GetCardLogicValue(bNextList[NextAnalyseData.bFourFirst[0]]) == GetCardLogicValue(bFirstList[FirstAnalyseData.bFourFirst[0]]))
							return GetCardColor(bNextList[NextAnalyseData.bFourFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bFourFirst[0]]) ;			//�Ƚϻ�ɫ
						else
							return GetCardLogicValue(bNextList[NextAnalyseData.bFourFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bFourFirst[0]]) ;	//�Ƚ���ֵ

					}

				case CT_FIVE_STRAIGHT_FLUSH_FIRST_A://A��ǰͬ��˳
					{
						//������֤
						ASSERT(bNextList[0]!=bFirstList[0]) ;
						if(bNextList[0]==bFirstList[0]) return false ;

						//�Ƚ���ֵ
						for(BYTE i=0; i<5; ++i)
							if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
								return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;

						//�Ƚϻ�ɫ
						return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;

					}
				case CT_FIVE_STRAIGHT_FLUSH:		//ͬ��˳��
					{
						//������֤
						ASSERT(bNextList[0]!=bFirstList[0]) ;
						if(bNextList[0]==bFirstList[0]) return false ;

						//�Ƚ���ֵ
						for(BYTE i=0; i<5; ++i)
							if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
								return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;

						//�Ƚϻ�ɫ
						return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;
					}

				default:
#ifdef _DEBUG
					AfxMessageBox("�����˿����ͣ�") ;
#endif
					return false ;
				}

			}
			else
			{
				//ͬ��˳��
				if( bNextType==CT_FIVE_STRAIGHT_FLUSH_FIRST_A || bNextType==CT_FIVE_STRAIGHT_FLUSH)
				{
					if(CT_FIVE_STRAIGHT_FLUSH_FIRST_A==bFirstType || CT_FIVE_STRAIGHT_FLUSH==bFirstType)
					{
						if(bNextType!=bFirstType)
							return bNextType > bFirstType ;

						//�Ƚ���ֵ
						for(BYTE i=0; i<5; ++i)
							if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
								return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;

						//�Ƚϻ�ɫ
						return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;
					}
				}
				return bNextType>bFirstType ;
			}
		}
	}
	else
	{
		//��ʼ�Ա�
		if(bNextType==bFirstType) 
		{
			switch(bFirstType)
			{
			case CT_SINGLE:				//��������
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return false ;

					bool bAllSame=true ;

					for(BYTE i=0 ; i<3 ; ++i)
						if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
						{
							bAllSame = false ;
							break; 
						}

						if(true==bAllSame)
							return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
						else
						{
							for(BYTE i=0 ; i<3 ; ++i)
								if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
									return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;
							return false ;
						}

						return bNextCount < bFirstCount ;

				}
			case CT_ONE_DOUBLE:			//�Դ�һ��
				{
					//������֤
					ASSERT(bNextList[NextAnalyseData.bTwoFirst[0]]!=bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;
					if(bNextList[NextAnalyseData.bTwoFirst[0]]==bFirstList[FirstAnalyseData.bTwoFirst[0]]) return false ;

					if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
					{
						if(GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]])!=GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
							return GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]) ;

						return GetCardColor(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;			//�Ƚϻ�ɫ
					}
					else
						return GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]); 	//�Ƚ���ֵ
					
					return bNextCount < bFirstCount ;
				}
			case CT_FIVE_TWO_DOUBLE:	//��������
				{
					//������֤
					ASSERT(bNextList[NextAnalyseData.bTwoFirst[0]]!=bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;
					if(bNextList[NextAnalyseData.bTwoFirst[0]]==bFirstList[FirstAnalyseData.bTwoFirst[0]]) return false ;

						if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]))
						{
							if(GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[1]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[1]]))
							{
								//�Աȵ���
								if(GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]])!=GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]))
									return GetCardLogicValue(bNextList[NextAnalyseData.bOneFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bOneFirst[0]]) ;

								return GetCardColor(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bTwoFirst[0]]) ;			//�Ƚϻ�ɫ
							}
							else
								return GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[1]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[1]]); 	//�Ƚ���ֵ
						}							
						else
							return GetCardLogicValue(bNextList[NextAnalyseData.bTwoFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bTwoFirst[0]]); 	//�Ƚ���ֵ
				}

			case CT_THREE:				//��������
				{
					//������֤
					if(bNextList[NextAnalyseData.bThreeFirst[0]]==bFirstList[FirstAnalyseData.bThreeFirst[0]])
					{
#ifdef _DEBUG

						AfxMessageBox("bNextList[NextAnalyseData.bThreeFirst[0]]!=bFirstList[FirstAnalyseData.bThreeFirst[0]]") ;
#endif
					}
					ASSERT(bNextList[NextAnalyseData.bThreeFirst[0]]!=bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;
					if(bNextList[NextAnalyseData.bThreeFirst[0]]==bFirstList[FirstAnalyseData.bThreeFirst[0]]) return false ;

					if(GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
						return GetCardColor(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;			//�Ƚϻ�ɫ
					else
						return GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]); 	//�Ƚ���ֵ
				}

			case CT_FIVE_MIXED_FLUSH_FIRST_A:				//A��ǰ˳��
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return false ;

					if(GetCardLogicValue(bNextList[0]) == GetCardLogicValue(bFirstList[0]))
						return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
					else
						return GetCardLogicValue(bNextList[0]) > GetCardLogicValue(bFirstList[0]) ;	//�Ƚ���ֵ

				}
			case CT_FIVE_MIXED_FLUSH_NO_A:			//ûA��˳
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return false ;

					if(GetCardLogicValue(bNextList[0])==GetCardLogicValue(bFirstList[0]))
						return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
					else
						return GetCardLogicValue(bNextList[0]) > GetCardLogicValue(bFirstList[0]); 	//�Ƚ���ֵ
				}
			case CT_FIVE_MIXED_FLUSH_BACK_A:		//A�ں�˳��
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return false ;

					if(GetCardLogicValue(bNextList[0])==GetCardLogicValue(bFirstList[0]))
						return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;			//�Ƚϻ�ɫ
					else
						return GetCardLogicValue(bNextList[0]) > GetCardLogicValue(bFirstList[0]); 	//�Ƚ���ֵ

				}

			case CT_FIVE_FLUSH:				//ͬ������
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return false ;

					//�Ƚ���ֵ
					for(BYTE i=0; i<5; ++i)
						if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
							return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;

					//�Ƚϻ�ɫ
					return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;
				}

			case CT_FIVE_THREE_DEOUBLE:			//����һ��
				{
					//������֤
					ASSERT(bNextList[NextAnalyseData.bThreeFirst[0]]!=bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;
					if(bNextList[NextAnalyseData.bThreeFirst[0]]==bFirstList[FirstAnalyseData.bThreeFirst[0]]) return false ;

					if(GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]])==GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]))
						return GetCardColor(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bThreeFirst[0]]) ;			//�Ƚϻ�ɫ
					else
						return GetCardLogicValue(bNextList[NextAnalyseData.bThreeFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bThreeFirst[0]]); 	//�Ƚ���ֵ
				}

			case CT_FIVE_FOUR_ONE:			//�Ĵ�һ��
				{
					//������֤
					ASSERT(bNextList[NextAnalyseData.bFourFirst[0]]!=bFirstList[FirstAnalyseData.bFourFirst[0]]) ;
					if(bNextList[NextAnalyseData.bFourFirst[0]]==bFirstList[FirstAnalyseData.bFourFirst[0]]) return false ;

					if(GetCardLogicValue(bNextList[NextAnalyseData.bFourFirst[0]]) == GetCardLogicValue(bFirstList[FirstAnalyseData.bFourFirst[0]]))
						return GetCardColor(bNextList[NextAnalyseData.bFourFirst[0]]) > GetCardColor(bFirstList[FirstAnalyseData.bFourFirst[0]]) ;			//�Ƚϻ�ɫ
					else
						return GetCardLogicValue(bNextList[NextAnalyseData.bFourFirst[0]]) > GetCardLogicValue(bFirstList[FirstAnalyseData.bFourFirst[0]]) ;	//�Ƚ���ֵ

				}

			case CT_FIVE_STRAIGHT_FLUSH_FIRST_A://A��ǰͬ��˳
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return false ;


					//�Ƚ���ֵ
					for(BYTE i=0; i<5; ++i)
						if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
							return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;

					//�Ƚϻ�ɫ
					return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;

				}
			case CT_FIVE_STRAIGHT_FLUSH:		//ͬ��˳��
				{
					//������֤
					ASSERT(bNextList[0]!=bFirstList[0]) ;
					if(bNextList[0]==bFirstList[0]) return false ;

					//�Ƚ���ֵ
					for(BYTE i=0; i<5; ++i)
						if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
							return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;

					//�Ƚϻ�ɫ
					return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;
				}

			default:
#ifdef _DEBUG
				AfxMessageBox("�����˿����ͣ�") ;
#endif
				return false ;
			}

		}
		else
		{
			//ͬ��˳��
			if( bNextType==CT_FIVE_STRAIGHT_FLUSH_FIRST_A || bNextType==CT_FIVE_STRAIGHT_FLUSH)
			{
				if(CT_FIVE_STRAIGHT_FLUSH_FIRST_A==bFirstType || CT_FIVE_STRAIGHT_FLUSH==bFirstType)
				{
					if(bNextType!=bFirstType)
							return bNextType > bFirstType ;

					//�Ƚ���ֵ
					for(BYTE i=0; i<5; ++i)
						if(GetCardLogicValue(bNextList[i]) != GetCardLogicValue(bFirstList[i]))
							return GetCardLogicValue(bNextList[i]) > GetCardLogicValue(bFirstList[i]) ;

					//�Ƚϻ�ɫ
					return GetCardColor(bNextList[0]) > GetCardColor(bFirstList[0]) ;
									
				}
			}
			return bNextType>bFirstType ;
		}
	}


	return false;
}

/*
 *	�����˿˵ĵ��ţ�һ�ԡ���������Ŀ������¼��ÿ�������˿˵ĵ�һ���ƣ�Ҳ���������ƣ�λ���Ա�Ƚϴ�С��ͬʱ�ж�ͬһ��ɫ�Ƿ�������
 */
//�����˿�
/*
void CGameLogic::AnalyseCard(const BYTE bCardData[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) 
{
	//������֤
	ASSERT(bCardCount>=1 && bCardCount<=13) ;
	if(bCardCount<1 || bCardCount>13) return ;

	memset(&AnalyseData , 0 , sizeof(AnalyseData)) ;
	BYTE bCardValue = GetCardValue(bCardData[0]) ;
	BYTE bCardColor = GetCardColor(bCardData[0]) ;

	BYTE bSameValueCount = 1 ,
		 bSameColorCount = 1 ,
		 bFirstCardIndex = 0 ;	//��¼�±�

	for(BYTE i=1 ; i<bCardCount ; ++i)
	{
		if(GetCardValue(bCardData[i])==bCardValue) ++bSameValueCount ;

        if(GetCardValue(bCardData[i])!=bCardValue || i==bCardCount-1)
		{
			switch(bSameValueCount) 
			{
			case 1:
				AnalyseData.bOneFirst[AnalyseData.bOneCount]	= bFirstCardIndex ;
				AnalyseData.bOneCount++ ;
				break;

			case 2:
				AnalyseData.bTwoFirst[AnalyseData.bTwoCount]	 = bFirstCardIndex ;
				AnalyseData.bTwoCount++ ;
				break;

			case 3:
				AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
				AnalyseData.bThreeCount++ ;
				break;

			case 4:
				AnalyseData.bFourFirst[AnalyseData.bFourCount]   = bFirstCardIndex ;
				AnalyseData.bFourCount++ ;
				break;

			case 5:
				AnalyseData.bFiveCount++ ;
				break;

			default:
				AfxMessageBox("������") ;
				return;
			}

			if(i==bCardCount-1) 
			{
				AnalyseData.bOneFirst[AnalyseData.bOneCount]	= i ;
				AnalyseData.bOneCount++ ;
			}

            bCardValue = GetCardValue(bCardData[i]) ;
			bSameValueCount = 1 ;
			bFirstCardIndex = i ;
		}

		if(GetCardColor(bCardData[i])!=bCardColor) bSameColorCount = 1 ;
		else									   ++bSameColorCount ;
	}

	//�Ƿ�ͬ��
	AnalyseData.bStraight = (5==bSameColorCount) ? true : false ;
}*/


//�����˿�
void CGameLogic::AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) 
{
	//�����˿�
	BYTE bCardData[13] ;
	CopyMemory(bCardData , bCardDataList , bCardCount) ;
	SortCardList(bCardData , bCardCount , enDescend) ;

	//��������
	BYTE bSameCount = 1 ,
		 bCardValueTemp=0,
		 bSameColorCount = 1 ,
		 bFirstCardIndex = 0 ;	//��¼�±�

	BYTE bLogicValue=GetCardLogicValue(bCardData[0]);
	BYTE bCardColor = GetCardColor(bCardData[0]) ;

//	if(bCardCount!=3 && bCardCount!=5) 
//	{
//#ifdef _DEBUG
//		AfxMessageBox("ok") ;
//		return ;
//#endif
//	}

	ASSERT(3==bCardCount || 5==bCardCount) ;



	//���ý��
	memset(&AnalyseData,0,sizeof(AnalyseData));

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
			case 1:		//һ��
				break; 
			case 2:		//����
				{
					AnalyseData.bTwoFirst[AnalyseData.bTwoCount]	 = bFirstCardIndex ;
					AnalyseData.bTwoCount++ ;
					break;
				}
			case 3:		//����
				{
					AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
					AnalyseData.bThreeCount++ ;
					break;
				}
			case 4:		//����
				{
					AnalyseData.bFourFirst[AnalyseData.bFourCount]   = bFirstCardIndex ;
					AnalyseData.bFourCount++ ;
					break;
				}
			default:
				CString str ;
				str.Format("AnalyseCard�������˿ˣ�: %d" , bSameCount) ;
#ifdef _DEBUG
				AfxMessageBox(str) ;
#endif				
				break;
			}
		}

		//���ñ���
		if (bCardValueTemp!=bLogicValue)
		{
			if(bSameCount==1)
			{
				if(i!=bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
				}
				else
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= i ;
					AnalyseData.bOneCount++ ;				
				}
			}
			else
			{
				if(i==bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= i ;
					AnalyseData.bOneCount++ ;
				}
			}
			bSameCount=1;
			bLogicValue=bCardValueTemp;
			bFirstCardIndex = i ;

		}
		if(GetCardColor(bCardData[i])!=bCardColor) bSameColorCount = 1 ;
		else									   ++bSameColorCount ;
	}

	//�Ƿ�ͬ��
	AnalyseData.bStraight = (5==bSameColorCount) ? true : false ;

	return;
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
