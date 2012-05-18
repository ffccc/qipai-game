#include "StdAfx.h"
#include "RecordGameList.h"

//构造函数
CGameRecord::CGameRecord()
{
	for( int i=0; i<MAX_SCORE_HISTORY; i++ )
	{
		m_RecordArray[i].cbGameRecord = 0xFF;
		m_RecordArray[i].cbGameTimes = -1;
	}
	m_nRecordCount = 0;
	m_nReadPos = 0;
	m_nWritePos = 0;
}

//析构函数
CGameRecord::~CGameRecord()
{
}

//插入列表
void CGameRecord::FillGameRecord(tagGameRecord * pGameRecord, int nRecordNum)
{
	for( int i=0; i< nRecordNum; i++ )
	{
		bool bRepeat = false;
		for(int j=0; j<m_nRecordCount; j++ )
		{
			//如果存在重复记录信息，则跳过，检查下一个新记录
			if( m_RecordArray[j].cbGameTimes == pGameRecord[i].cbGameTimes )
			{
				bRepeat = true;
				break;
			}
		}
		if( !bRepeat )	//如果不重复，则把当前新记录插入
		{
			m_nWritePos = (m_nWritePos)%MAX_SCORE_HISTORY;
			tagGameRecord & newRecord = m_RecordArray[m_nWritePos];
			newRecord.cbGameTimes = pGameRecord[i].cbGameTimes;
			newRecord.cbGameRecord = pGameRecord[i].cbGameRecord;
			if( m_nRecordCount < RECORD_COUNT_LIMIT)
				m_nRecordCount++;
			if( m_nRecordCount<MAX_SCORE_HISTORY)
			{
				m_nReadPos = m_nWritePos;
			}
			m_nWritePos++;
		}
	}
	return;
}

void CGameRecord::ShowHistory(CDC * pDC, CSkinImage skinImage, CRect * pRectArray, int nRectCount)
{
	if( pDC == NULL || pRectArray == NULL || nRectCount<=0 )
		return;

	BYTE cbRecord = 0;
	CImageHandle  ImageRecordHandle(&skinImage);
	int nRecordWidth = skinImage.GetWidth()/8;
	int nShowCount = (m_nRecordCount >= MAX_SHOW_HISTORY ? MAX_SHOW_HISTORY:m_nRecordCount);
	int k = nShowCount-1;
	int nCurIndex = 0;

	for( int i=0; i<nShowCount; i++)
	{
		//取得当前历史记录代表的动物图案
		nCurIndex = (m_nReadPos - i + MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;
		cbRecord = m_RecordArray[nCurIndex].cbGameRecord;
		cbRecord = cbRecord % MAX_SHOW_HISTORY;
		skinImage.BitBlt(pDC->GetSafeHdc(), pRectArray[k].left, pRectArray[k].top, nRecordWidth, skinImage.GetHeight(), cbRecord*nRecordWidth, 0);
		k--;
	}
}

void CGameRecord::ClearAllRecord()
{
	for( int i=0; i<MAX_SCORE_HISTORY; i++ )
	{
		m_RecordArray[i].cbGameRecord = 0xFF;
		m_RecordArray[i].cbGameTimes = -1;
	}
	m_nRecordCount = 0;
	m_nReadPos = 0;
}

int CGameRecord::ScrollLeft()
{
	int nTemp = 0;
	if(m_nRecordCount <= MAX_SCORE_HISTORY )
	{
		nTemp = m_nReadPos-MAX_SHOW_HISTORY;
		if( nTemp >=0 )
		{
			m_nReadPos -= 1;
			return m_nReadPos;
		}
		else
			return -1;
	}
	else
	{
		nTemp = (m_nReadPos-MAX_SHOW_HISTORY+MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;
		if(nTemp != (m_nWritePos-1)%MAX_SCORE_HISTORY)
		{
			m_nReadPos = (m_nReadPos-1+MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;
			return m_nReadPos;
		}
		else
		{
			return -1;
		}

	}
}

int CGameRecord::ScrollRight()
{
	if( m_nReadPos != m_nWritePos-1 )
	{
		m_nReadPos = (m_nReadPos+1)%MAX_SCORE_HISTORY;
		return m_nReadPos;
	}
	else
	{
		//表示已经移动到最后面，即最新的一个记录
		return -1;
	}
}
//////////////////////////////////////////////////////////////////////////
