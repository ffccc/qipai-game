#include "StdAfx.h"
#include "ApplyUserList.h"

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

//构造函数
CApplyUser::CApplyUser()
{
	ZeroMemory(&m_BankerUserArray, sizeof(m_BankerUserArray));
	m_nApplyCount = 0;
	m_nPos = 0;
}

//析构函数
CApplyUser::~CApplyUser()
{
}

//插入列表
void CApplyUser::InserUser(tagApplyUser & ApplyUser)
{
	if( m_nApplyCount < GAME_PLAYER)
	{
		m_BankerUserArray[m_nApplyCount].nSortNo = m_nApplyCount+1;
		m_BankerUserArray[m_nApplyCount].dwUserID = ApplyUser.dwUserID;
		m_BankerUserArray[m_nApplyCount].iUserScore = ApplyUser.iUserScore;
		lstrcpyn(m_BankerUserArray[m_nApplyCount].szUserName, ApplyUser.szUserName, sizeof(m_BankerUserArray[m_nApplyCount].szUserName));
		m_nApplyCount++;
	}
	return;
}


//删除列表
void CApplyUser::DeleteUser(tagApplyUser & ApplyUser)
{
	for(int i=0; i<m_nApplyCount; i++)
	{
		if( m_BankerUserArray[i].dwUserID == ApplyUser.dwUserID )
		{
			for( int j = i+1; j<m_nApplyCount; j++ )
			{
				m_BankerUserArray[j-1] = m_BankerUserArray[j];
				m_BankerUserArray[j-1].nSortNo -= 1;
			}
			m_nApplyCount--;
			break;
		}
	}
}

//更新列表
void CApplyUser::UpdateUser( tagApplyUser & ApplyUser )
{
	for( int i=0; i<m_nApplyCount; i++)
	{
		if( m_BankerUserArray[i].dwUserID == ApplyUser.dwUserID )
		{
			m_BankerUserArray[i].iUserScore = ApplyUser.iUserScore;
			break;
		}
	}
}

//清空列表
void CApplyUser::ClearAll()
{
	ZeroMemory(&m_BankerUserArray, sizeof(m_BankerUserArray));
	m_nApplyCount = 0;
}
//////////////////////////////////////////////////////////////////////////

CString CApplyUser::FormatNumber(__int64 i64)
{
	CString str;
	str.Format(TEXT("%I64d"),i64>=0?i64:-i64);
	/*int p=str.GetLength()-4;
	while(p>0)
	{
		str.Insert(p,TEXT(" "));
		p-=4;
	}*/
	if(i64<0)
		str=TEXT("-")+str;
	return str;
}

void CApplyUser::ShowApplayBankerUser(CDC * pDC, CRect * pSortNoRect, CRect * pNameRect, CRect * pScoreRect)
{
	if( pDC == NULL || pSortNoRect == NULL || pNameRect == NULL || pScoreRect == NULL )
		return;
	if( m_nApplyCount <=0 )
		return;

	int nEnd = __min((m_nApplyCount-m_nPos), MAX_SCORE_BANKER);
	CString strMsg;
	int j=0,k=0,m=0;
	for( int i=m_nPos; i<m_nPos+nEnd; i++)
	{
		strMsg.Format("%d", m_BankerUserArray[i].nSortNo);
		pDC->DrawText(strMsg, pSortNoRect[j++], DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
		pDC->DrawText(m_BankerUserArray[i].szUserName,lstrlen(m_BankerUserArray[i].szUserName), pNameRect[k++], DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
		strMsg.Format("%I64d", m_BankerUserArray[i].iUserScore);
		pDC->DrawText(strMsg, pScoreRect[m++], DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
	}
}

int CApplyUser::ScrollUp()
{
	if( m_nPos > 0 )
	{
		m_nPos--;
	}
	return m_nPos;
}

int CApplyUser::ScrollDown()
{
	if( m_nPos < m_nApplyCount )
	{
		m_nPos++;
	}
	return m_nPos;
}