#include "StdAfx.h"
#include ".\androidai.h"


/////////////////////////////////////////////////////////////////////////////////////
//CAndroidAIBase

CAndroidAIBase::CAndroidAIBase()
{
	memset(m_byCardData,0xff,sizeof(m_byCardData));
}

CAndroidAIBase::~CAndroidAIBase()
{
}

//������
void CAndroidAIBase::SetCardData( const BYTE cbCardData[],BYTE byCardCount )
{
	//������
	CopyMemory(m_byCardData,cbCardData,sizeof(BYTE)*byCardCount);
	m_byCardCount = byCardCount;

	//��ʼ��
	ZeroMemory(m_bSelect,sizeof(m_bSelect));

	//
	ZeroMemory(m_byThreeCard,sizeof(m_byThreeCard));
	m_byThreeCount = 0;
	ZeroMemory(m_byGoodThreeCard,sizeof(m_byGoodThreeCard));
	m_byGoodThreeCount = 0;

	//
	ZeroMemory(m_byTwoCard,sizeof(m_byTwoCard));
	m_byTwoCount = 0;
	ZeroMemory(m_byGoodTwoCard,sizeof(m_byGoodTwoCard));
	m_byGoodTwoCount = 0;

	//
	ZeroMemory(m_byRemainThree,sizeof(m_byRemainThree));
	m_byRemainThreeCount = 0;
	ZeroMemory(m_byRemainTwo,sizeof(m_byRemainTwo));
	m_byRemainTwoCount = 0;

	//
	m_nMaxScoreThree = m_nMaxScoreTwo = 0;
	m_nActionScore = 0;
	m_nScoreThree = m_nScoreTwo = 0;

	//
	m_bHaveJiang = false;

	//
	m_byBadlyCard = 0xff;
}

//ģ�����
void CAndroidAIBase::SetAction( BYTE byActionMask,BYTE byActionCard )
{
	//��֤
	ASSERT( byActionCard >=0 && byActionCard < 34 );
	//ö��
	BYTE byIndex = 0xff;
	switch( byActionMask )
	{
	case WIK_LEFT:
		{
			m_nActionScore = 300;
			byIndex = FindIndex(byActionCard+1);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			byIndex = FindIndex(byActionCard+2);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			break;
		}
	case WIK_CENTER:
		{
			m_nActionScore = 300;
			byIndex = FindIndex(byActionCard-1);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			byIndex = FindIndex(byActionCard+1);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			break;
		}
	case WIK_RIGHT:
		{
			m_nActionScore = 300;
			byIndex = FindIndex(byActionCard-2);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			byIndex = FindIndex(byActionCard-1);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			break;
		}
	case WIK_PENG:
		{
			m_nActionScore = 300;
			byIndex = FindIndex(byActionCard);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			byIndex = FindIndex(byActionCard);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			break;
		}
	case WIK_GANG:
		{
			m_nActionScore = 300;
			byIndex = FindIndex(byActionCard);
			ASSERT( byIndex != 0xff );
			VERIFY( RemoveCard(byIndex) );
			byIndex = FindIndex(byActionCard);
			if( byIndex != 0xff )
			{
				VERIFY( RemoveCard(byIndex) );
				byIndex = FindIndex(byActionCard);
				ASSERT( byIndex != 0xff );
				VERIFY( RemoveCard(byIndex) );
				byIndex = FindIndex(byActionCard);
				if( byIndex != 0xff )
					VERIFY( RemoveCard(byIndex) );
			}
			break;
		}
	default:
		ASSERT( FALSE );
	}
}

//��ȡ�����
BYTE CAndroidAIBase::GetBadlyCard()
{
	return m_byBadlyCard;
}

//��ȡ����
int CAndroidAIBase::GetMaxScore()
{
	return m_nActionScore+m_nMaxScoreThree+m_nMaxScoreTwo;
}

//��ֻ���Ƿ��Ǳߵ�
bool CAndroidAIBase::IsEdge( BYTE byCard1,BYTE byCard2 )
{
	if( 0 == byCard1%9 || 8 == byCard2%9 )
		return true;
	return false;
}

//������ͬ��
bool CAndroidAIBase::SearchSameCard( BYTE byCardData,BYTE &byIndex1,BYTE &byIndex2 )
{
	//
	byIndex1 = FindIndex(byCardData);
	if( byIndex1 == 0xff ) return false;
	byIndex2 = FindIndex(byCardData,byIndex1+1);
	if( byIndex2 == 0xff ) return false;
	return true;
}

//��������
bool CAndroidAIBase::SearchLinkCard( BYTE byCardData,BYTE &byIndex1,BYTE &byIndex2 )
{
	//Ч��
	if( byCardData >= 27 ) return false;
	//�ڶ�,��ֻ
	BYTE byCard1 = byCardData+1;
	BYTE byCard2 = byCardData+2;
	if( byCard1 >= 27 || byCard2 >= 27 || byCardData/9 != byCard1/9 || byCardData/9 != byCard2/9 )
		return false;
	//Ѱ��
	byIndex1 = FindIndex(byCard1);
	if( byIndex1 == 0xff ) return false;
	byIndex2 = FindIndex(byCard2);
	if( byIndex2 == 0xff ) return false;

	return true;
}

//������ֻͬ��
bool CAndroidAIBase::SearchSameCardRemain( BYTE byCardData,BYTE &byIndex,BYTE byStart )
{
	byIndex = FindIndexRemain(byCardData,byStart);
	return 0xff==byIndex?false:true;
}

//�����п�����
bool CAndroidAIBase::SearchLinkCardRemain( BYTE byCardData,BYTE byLinkType,BYTE &byIndex,BYTE byStart )
{
	//��֤
	if( byCardData >= 27 ) return false;
	//�ж�����
	BYTE byCard1 = 0xff;
	if( 0 == byLinkType )			//����
		byCard1 = byCardData+1;
	else if( 1 == byLinkType )		//�п�
		byCard1 = byCardData+2;
	//����
	if( byCard1 >= 27 || byCardData/9 != byCard1/9 ) return false;
	byIndex = FindIndexRemain(byCard1,byStart);
	return 0xff==byIndex?false:true;
}

//������
BYTE CAndroidAIBase::FindIndex( BYTE byCardData,BYTE byStart )
{
	for( BYTE i = byStart; i < m_byCardCount; i++ )
	{
		if( byCardData == m_byCardData[i] && !m_bSelect[i] )
			return i;
	}
	return 0xff;
}

//���Ƴ������ֻ��������
BYTE CAndroidAIBase::FindIndexRemain( BYTE byCardData,BYTE byStart )
{
	for( BYTE i = byStart; i < m_byRemainThreeCount; i++ )
	{
		if( byCardData == m_byRemainThree[i] && !m_bSelect[i] )
			return i;
	}
	return 0xff;
}

//�Ƴ���
bool CAndroidAIBase::RemoveCard( BYTE byCardIndex )
{
	//Ч��
	ASSERT( m_byCardCount > 0 );
	if( m_byCardCount == 0 ) return false;
	ASSERT( byCardIndex >= 0 && byCardIndex < MAX_COUNT );
	if( byCardIndex < 0 || byCardIndex >= MAX_COUNT ) return false;

	//ɾ��
	BYTE byCount = m_byCardCount;
	m_byCardCount = 0;
	for( BYTE i = 0; i < byCount; i++ )
	{
		if( i == byCardIndex ) continue;
		m_byCardData[m_byCardCount++] = m_byCardData[i];
	}
	m_byCardData[byCount-1] = 0xff;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//CAndroidAI

CAndroidAI::CAndroidAI(void)
{
	ZeroMemory(m_byEnjoinOutCard,sizeof(m_byEnjoinOutCard));
	m_byEnjoinOutCount = 0;
}

CAndroidAI::~CAndroidAI(void)
{
}

//˼��
void CAndroidAI::Think()
{
	//������ֻ
	AnalyseThree();
	//���û��ֻ
	BYTE i;
	if( m_nMaxScoreThree == 0 || m_byRemainThreeCount == 0 )
	{
		m_byRemainThreeCount = m_byCardCount;
		for( i = 0; i < m_byRemainThreeCount; i++ )
			m_byRemainThree[i] = m_byCardData[i];
	}
	//������ֻ
	AnalyseTwo();
	if( m_nMaxScoreTwo == 0 )
	{
		m_byRemainTwoCount = m_byRemainThreeCount;
		for( i = 0; i < m_byRemainTwoCount; i++ )
			m_byRemainTwo[i] = m_byRemainThree[i];
	}
	//���ȫ������ֻ
	if( m_byRemainTwoCount == 0 )
	{
		SearchTwo();
		return;
	}
	//����һֻ
	AnalyseOne();
}

////
//BYTE CAndroidAI::GetThreeCard( BYTE byCard[],BYTE byMaxCount )
//{
//	//
//	BYTE byCopyCount = byMaxCount>m_byGoodThreeCount?m_byGoodThreeCount:byMaxCount;
//	CopyMemory(byCard,m_byGoodThreeCard,sizeof(BYTE)*byCopyCount);
//	return byCopyCount;
//}
//
////
//BYTE CAndroidAI::GetTwoCard( BYTE byCard[],BYTE byMaxCount )
//{
//	//
//	BYTE byCopyCount = byMaxCount>m_byGoodTwoCount?m_byGoodThreeCount:byMaxCount;
//	CopyMemory(byCard,m_byGoodTwoCard,sizeof(BYTE)*byCopyCount);
//	return byCopyCount;
//}

//�������ֻ����������������
BYTE CAndroidAI::GetBadlyIn2Card()
{
	BYTE byBadly = 0xff;
	int nMin = 33;
	int nScore;
	BYTE byCard;
	for( BYTE i = 0; i < m_byGoodTwoCount*2; i++ )
	{
		byCard = m_byGoodTwoCard[i];
		if( byCard >= 27 )						//�������
		{
			nScore = 2;
		}
		else if( byCard%9 == 0 || byCard%9 == 8 )	//�����һ���߾�
		{
			nScore = 6;
		}
		else 
		{
			nScore = 10;
		}
		nScore += AddScore(byCard);
		if( nScore < nMin )
		{
			nMin = nScore;
			byBadly = byCard;
		}
	}
	return byBadly;
}

//�������ֻ����������������
BYTE CAndroidAI::GetBadlyIn3Card()
{
	BYTE byBadly = 0xff;
	int nMin = 33;
	int nScore;
	BYTE byCard;
	for( BYTE i = 0; i < m_byGoodThreeCount*3; i++ )
	{
		byCard = m_byGoodThreeCard[i];
		if( byCard >= 27 )						//�������
		{
			nScore = 2;
		}
		else if( byCard%9 == 0 || byCard%9 == 8 )	//�����һ���߾�
		{
			nScore = 6;
		}
		else 
		{
			nScore = 10;
		}
		nScore += AddScore(byCard);
		if( nScore < nMin )
		{
			nMin = nScore;
			byBadly = byCard;
		}
	}
	return byBadly;
}

//���ý�ֹ������
void CAndroidAI::SetEnjoinOutCard( const BYTE cbEnjoinOutCard[],BYTE cbEnjoinOutCount )
{
	m_byEnjoinOutCount = cbEnjoinOutCount;
	if( m_byEnjoinOutCount > 0 )
	{
		CopyMemory(m_byEnjoinOutCard,cbEnjoinOutCard,sizeof(BYTE)*cbEnjoinOutCount);
	}
}

//��Ȩ
int CAndroidAI::AddScore( BYTE byCardData )
{
	int nScore = 0;
	if( byCardData >= 27 )
	{
		return 0;
	}
	if( byCardData%9 != 0 && FindIndex(byCardData-1) != 0xff )	//���ʣ��������б�����һ����
	{
		if( byCardData%9 != 1 )						//�����ǰ�Ʋ��Ƕ��༴��3
		{
			nScore += 3;
		}
		else
		{
			nScore += 1;
		}
	}
	if( byCardData%9 != 8 && FindIndex(byCardData+1) != 0xff )	//���ʣ��������б����һ������
	{
		if( byCardData%9 != 7 )
		{
			nScore += 3;
		}
		else
		{
			nScore += 1;
		}
	}
	if( byCardData%9 > 1 && FindIndex(byCardData-2) != 0xff )		//���ʣ��������б����ٶ����ƣ���3��5,1_3�ȣ�
	{
		nScore += 2;
	}
	
	if( byCardData%9 < 7 && FindIndex(byCardData+2) != 0xff )		//���ʣ��������б���������
	{
		nScore += 2;
	}
	//
	if( IsEnjoinOutCard(byCardData) ) nScore = 33;
	return nScore;
}

//������ֻ
void CAndroidAI::AnalyseThree()
{
	BYTE byIndex1,byIndex2;
	for( BYTE i = 0; i < m_byCardCount; i++ )
	{
		if( !m_bSelect[i] )
		{
			m_bSelect[i] = true;
			//������ֻ
			if( SearchSameCard(m_byCardData[i],byIndex1,byIndex2) )
			{
				//��ʱ��¼
				m_byThreeCard[m_byThreeCount*3] = m_byCardData[i];
				m_byThreeCard[m_byThreeCount*3+1] = m_byCardData[i];
				m_byThreeCard[m_byThreeCount*3+2] = m_byCardData[i];
				
				//�ݹ�
				m_byThreeCount++;
				m_nScoreThree += 300;
				m_bSelect[byIndex1] = true;
				m_bSelect[byIndex2] = true;
				AnalyseThree();
				m_bSelect[byIndex1] = false;
				m_bSelect[byIndex2] = false;
				m_nScoreThree -= 300;
				m_byThreeCount--;
			}
			//��������
			if( SearchLinkCard(m_byCardData[i],byIndex1,byIndex2) )
			{
				//��ʱ��¼
				m_byThreeCard[m_byThreeCount*3] = m_byCardData[i];
				m_byThreeCard[m_byThreeCount*3+1] = m_byCardData[i]+1;
				m_byThreeCard[m_byThreeCount*3+2] = m_byCardData[i]+2;

				//�ݹ�
				m_byThreeCount++;
				m_nScoreThree += 300;
				m_bSelect[byIndex1] = true;
				m_bSelect[byIndex2] = true;
				AnalyseThree();
				m_bSelect[byIndex1] = false;
				m_bSelect[byIndex2] = false;
				m_nScoreThree -= 300;
				m_byThreeCount--;
			}
			m_bSelect[i] = false;
		}
	}
	//����������������ߵ�
	if( m_nScoreThree > m_nMaxScoreThree )
	{
		//��¼ʣ�µ�
		m_byRemainThreeCount = 0;
		m_nMaxScoreThree = m_nScoreThree;
		for( i = 0; i < m_byCardCount; i++ )
		{
			if( !m_bSelect[i] )
				m_byRemainThree[m_byRemainThreeCount++] = m_byCardData[i];
		}
		//��¼�����ֻ���
		m_byGoodThreeCount = m_byThreeCount;
		CopyMemory(m_byGoodThreeCard,m_byThreeCard,sizeof(m_byThreeCard));
	}
}

//������ֻ
void CAndroidAI::AnalyseTwo()
{
	BYTE byIndex;
	for( BYTE i = 0; i < m_byRemainThreeCount; i++ )
	{
		if( !m_bSelect[i] )
		{
			m_bSelect[i] = true;
			//������ֻ��ͬ
			if( SearchSameCardRemain(m_byRemainThree[i],byIndex,i+1) )
			{
				//��ʱ��¼
				m_byTwoCard[m_byTwoCount*2] = m_byRemainThree[i];
				m_byTwoCard[m_byTwoCount*2+1] = m_byRemainThree[byIndex];

				//�жϽ�
				m_byTwoCount++;
				int nGoodSame = 90;
				if( !m_bHaveJiang )
				{
					m_bHaveJiang = true;
					nGoodSame = 120;
				}
				//�ݹ�
				m_nScoreTwo += nGoodSame;
				m_bSelect[byIndex] = true;
				AnalyseTwo();
				m_bSelect[byIndex] = false;
				if( 120 == nGoodSame )
					m_bHaveJiang = false;
				m_nScoreTwo -= nGoodSame;
				m_byTwoCount--;
			}
			//����������
			if( SearchLinkCardRemain(m_byRemainThree[i],0,byIndex,i+1) )
			{
				//��ʱ��¼
				m_byTwoCard[m_byTwoCount*2] = m_byRemainThree[i];
				m_byTwoCard[m_byTwoCount*2+1] = m_byRemainThree[byIndex];

				//�жϱ�
				m_byTwoCount++;
				int nGoodLink;
				if( IsEdge(m_byRemainThree[i],m_byRemainThree[byIndex]) )
					nGoodLink = 80;
				else nGoodLink = 100;
				//�ݹ�
				m_nScoreTwo += nGoodLink;
				m_bSelect[byIndex] = true;
				AnalyseTwo();
				m_bSelect[byIndex] = false;
				m_nScoreTwo -= nGoodLink;
				m_byTwoCount--;
			}
			//�����п�������
			if( SearchLinkCardRemain(m_byRemainThree[i],1,byIndex,i+1) )
			{				
				//��ʱ��¼
				m_byTwoCard[m_byTwoCount*2] = m_byRemainThree[i];
				m_byTwoCard[m_byTwoCount*2+1] = m_byRemainThree[byIndex];

				//�жϱ�
				m_byTwoCount++;
				int nGoodLink;
				if( IsEdge(m_byRemainThree[i],m_byRemainThree[byIndex]) )
					nGoodLink = 70;
				else nGoodLink = 90;
				//�ݹ�
				m_nScoreTwo += nGoodLink;
				m_bSelect[byIndex] = true;
				AnalyseTwo();
				m_bSelect[byIndex] = false;
				m_nScoreTwo -= nGoodLink;
				m_byTwoCount--;
			}
			m_bSelect[i] = false;
		}
	}
	//����з������ߵ�
	if( m_nScoreTwo > m_nMaxScoreTwo )
	{
		//��¼ʣ�µ�
		m_nMaxScoreTwo = m_nScoreTwo;
		m_byRemainTwoCount = 0;
		for( i = 0; i < m_byRemainThreeCount; i++ )
		{
			if( !m_bSelect[i] )
				m_byRemainTwo[m_byRemainTwoCount++] = m_byRemainThree[i];
		}
		//��¼�����ֻ���
		m_byGoodTwoCount = m_byTwoCount;
		CopyMemory(m_byGoodTwoCard,m_byTwoCard,sizeof(m_byTwoCard));
	}
}

//������ֻ
void CAndroidAI::AnalyseOne()
{
	BYTE byCard;
	int nScore;
	int nMin = 33;
	for(int i = 0;i < m_byRemainTwoCount;i++ )	//�ҳ�����һ����
	{
		byCard = m_byRemainTwo[i];

		if( byCard >= 27 )						//�������
		{
			nScore = 2;
		}
		else if( byCard%9 == 0 || byCard%9 == 8 )	//�����һ���߾�
		{
			nScore = 6;
		}
		else 
		{
			nScore = 10;
		}

		nScore += AddScore(byCard);
		if( nScore < nMin )
		{
			nMin = nScore;
			m_byBadlyCard = byCard;
		}
	}
}

//����ֻ����з���
void CAndroidAI::SearchTwo()
{
	//�������
	BYTE byCardTwo[14];
	BYTE byCardTwoCount = 0;
	bool bTeamHave = false;
	//���ñ���
	for( int i = 0;i < m_byRemainThreeCount;i++ )
		byCardTwo[byCardTwoCount++] = m_byRemainThree[i];
	//����һ��
	if( byCardTwoCount == 2 )												//���ֻ��������
	{
		if( byCardTwo[0] == byCardTwo[1] )								//����
		{
			m_byBadlyCard = byCardTwo[0];								//��ɽ�齫���Ʊ�����̨
			return;
		}
		for( int j = 0;j < m_byCardCount-1;j++ )						//��һ�Խ�
		{
			if( m_byCardData[j] == m_byCardData[j+1] 
				&& m_byCardData[j] == m_byCardData[j+2] )			//���Ƿ���������
			{
				if( !IsEnjoinOutCard(m_byCardData[j]) )
				{
					bTeamHave = true;
					m_byBadlyCard = m_byCardData[j];
					break;
				}
			}
		}
		if( bTeamHave == false )
		{
			m_byRemainTwoCount = 2;
			m_byRemainTwo[0] = byCardTwo[0];
			m_byRemainTwo[1] = byCardTwo[1];
			AnalyseOne();
		}
	}
	else
	{
		bool bSameHave = false;
		int nMinScore = 33;
		int nScore[8];
		memset(nScore,33,sizeof(nScore));
		for( BYTE j = 0;j < byCardTwoCount/2;j++ )					//ѭ����¼ÿ���Ƶķ���
		{
			if( IsEnjoinOutCard(byCardTwo[j*2]) || IsEnjoinOutCard(byCardTwo[j*2+1]) ) continue;
			if( byCardTwo[j*2] == byCardTwo[j*2 + 1] )				//����
			{
				if( bSameHave )
				{
					nScore[j] = 6;
				}
				else 
				{
					nScore[j] = 8;
					bSameHave = true;
				}
			}
			else if( byCardTwo[j*2] == byCardTwo[j*2+1] - 1 )		//��������
			{
				if( byCardTwo[j*2]%9 == 0 || byCardTwo[j*2+1]%9 == 8 )
				{
					nScore[j] = 4;
				}
				else
				{
					nScore[j] = 7;
				}
			}
			else													//©һ������
			{
				if( byCardTwo[j*2]%9 == 0 || byCardTwo[j*2+1]%9 == 8 )
					nScore[j] = 3;
				else
					nScore[j] = 5;
			}
			
		}
		for( BYTE k = 0;k < byCardTwoCount/2;k++)						//�ҳ�������С��һ���ƾ�������һ��
		{
			if( nScore[k] < nMinScore )
			{
				nMinScore = nScore[k];
				if( byCardTwo[k*2] % 9 == 0 )
				{
					m_byBadlyCard = byCardTwo[k*2];
				}
				else if( byCardTwo[k*2+1] % 9 == 8 )
				{
					m_byBadlyCard = byCardTwo[k*2+1];
				}
				else 
				{
					m_byBadlyCard = byCardTwo[k*2 + rand()%2];
				}
			}
		}
	}
}

//�ж����Ƿ��ֹ��
bool CAndroidAI::IsEnjoinOutCard( BYTE byCard )
{
	for( BYTE i = 0; i < m_byEnjoinOutCount; i++ )
	{
		if( byCard == m_byEnjoinOutCard[i] )
			return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////
