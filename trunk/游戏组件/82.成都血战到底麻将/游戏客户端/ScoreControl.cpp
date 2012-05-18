#include "StdAfx.h"
#include "ScoreControl.h"

//////////////////////////////////////////////////////////////////////////


//���캯��
CScoreControl::CScoreControl()
{
	//���ñ���
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));
	m_ChiHuRight.SetEmpty();
	m_ptBenchmark.SetPoint(0,0);
	m_bShow = false;

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageGameLost.LoadImage( hResInstance,TEXT("GAME_LOST") );
	m_ImageGameWin.LoadImage( hResInstance,TEXT("GAME_WIN") );
	m_PngNum.LoadImage( hResInstance,TEXT("NUM_ORDER") );

	return;
}

//��������
CScoreControl::~CScoreControl()
{
}

//��λ����
void CScoreControl::RestorationData()
{
	//���ñ���
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));
	m_ChiHuRight.SetEmpty();
	m_bShow = false;

	return;
}

//���û���
void CScoreControl::SetScoreInfo(const tagScoreInfo & ScoreInfo)
{
	//���ñ���
	m_ScoreInfo=ScoreInfo;

	m_ChiHuRight.SetRightData( ScoreInfo.dwChiHuRight,MAX_RIGHT_COUNT );

	//��ʾ����
	m_bShow = true;

	return;
}

//�ػ�����
void CScoreControl::DrawScoreControl( CDC *pDC )
{
	if( !m_bShow ) return;

	pDC->SetBkMode(TRANSPARENT);

	int nXPos = m_ptBenchmark.x;
	int nYPos = m_ptBenchmark.y;

	//�滭����
	if( m_ScoreInfo.lGameScore > 0 )
		m_ImageGameWin.DrawImage( pDC,nXPos,nYPos );
	else m_ImageGameLost.DrawImage( pDC,nXPos,nYPos );

	if( m_ScoreInfo.lGameScore > 0 )
	{
		nXPos += 84;
		nYPos += 17;
	}
	else
	{
		nXPos += 46;
		nYPos += 14;
	}

	//��������
	if( m_ScoreInfo.cbWinOrder > 0 )
	{
		if( m_ScoreInfo.lGameScore > 0 )
			DrawNumberString( pDC,m_PngNum,m_ptBenchmark.x+58,m_ptBenchmark.y+30,m_ScoreInfo.cbWinOrder,true );
		else
			DrawNumberString( pDC,m_PngNum,m_ptBenchmark.x+20,m_ptBenchmark.y+27,m_ScoreInfo.cbWinOrder,true );
	}

	//������Ϣ
	TCHAR szBuffer[256] = TEXT("");
	if( !m_ChiHuRight.IsEmpty() )
	{
		if( m_ScoreInfo.wMeChairId == m_ScoreInfo.wProvideUser )
			lstrcpy( szBuffer,TEXT("����") );
		else 
		{
			lstrcpy( szBuffer,TEXT("Ӯ��") );
			TCHAR szName[10] = TEXT("");
			lstrcpyn( szName,m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],sizeof(szName)/sizeof(TCHAR));
			lstrcat( szBuffer,szName );
			if( lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]) > 8 )
				lstrcat( szBuffer,TEXT("...") );
			lstrcat( szBuffer,TEXT(" ����") );
		}
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),nXPos,nYPos,false );
		nYPos += 19;

		DWORD dwRight[] = {
			CHR_SHU_FAN,CHR_TIAN_HU,CHR_DI_HU,
			CHR_DA_DUI_ZI,CHR_QING_YI_SE,CHR_QI_XIAO_DUI,CHR_DAI_YAO,CHR_JIANG_DUI,CHR_QING_DUI,
			CHR_LONG_QI_DUI,CHR_QING_QI_DUI,CHR_QING_YAO_JIU,CHR_QING_LONG_QI_DUI,
			CHR_GANG_SHANG_PAO,CHR_GANG_KAI
		};
		LPCTSTR pszRight[] = {
			TEXT("�ط� 1�� "),TEXT("��� 6�� "),TEXT("�غ� 6�� "),
			TEXT("����� 2�� "),TEXT("��һɫ 3�� "),TEXT("��С�� 3�� "),TEXT("���� 3�� "),TEXT("���� 4�� "),
			TEXT("��� 4�� "),TEXT("���߶� 5�� "),TEXT("���߶� 5�� "),TEXT("���۾� 5�� "),TEXT("�����߶� 6�� "),
			TEXT("������ 1�� "),TEXT("���ϻ� 1�� ")
		};
		szBuffer[0] = 0;
		for( BYTE j = 0; j < CountArray(pszRight); j++ )
		{
			if( !(m_ChiHuRight&dwRight[j]).IsEmpty() )
				lstrcat( szBuffer,pszRight[j] );
		}
		//��
		if( m_ScoreInfo.cbGenCount > 0 )
		{
			TCHAR szGen[8];
			_snprintf( szGen,CountArray(szGen),TEXT("�� %d��"),m_ScoreInfo.cbGenCount );
			lstrcat( szBuffer,szGen );
		}
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),nXPos,nYPos,false );
		nYPos += 19;
	}

	//������
	bool bFirstDraw = true;
	szBuffer[0] = 0;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( i == m_ScoreInfo.wMeChairId ) continue;

		if( m_ScoreInfo.wLostFanShu[i] > 0 )
		{
			if( bFirstDraw )
			{
				bFirstDraw = false;
				lstrcpy( szBuffer,TEXT("��� ") );
			}
			else lstrcpy( szBuffer,TEXT("     ") );
			TCHAR szName[10] = TEXT("");
			lstrcpyn( szName,m_ScoreInfo.szUserName[i],sizeof(szName)/sizeof(TCHAR) );
			lstrcat( szBuffer,szName );
			if( lstrlen(m_ScoreInfo.szUserName[i]) > 8 )
				lstrcat( szBuffer,TEXT("...") );
			_snprintf( szBuffer,CountArray(szBuffer),TEXT("%s %d"),szBuffer,m_ScoreInfo.wLostFanShu[i] );
			DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),nXPos,nYPos,false );
			nYPos += 19;
		}
	}

	//�η�����
	if( m_ScoreInfo.lGangScore != 0 )
	{
		_snprintf( szBuffer,CountArray(szBuffer),TEXT("�η����꣺%ld"),m_ScoreInfo.lGangScore );
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),nXPos,nYPos,false );
	}

	//�ܼ�
	_snprintf( szBuffer,CountArray(szBuffer),TEXT("%ld"),m_ScoreInfo.lGameScore );
	if( m_ScoreInfo.lGameScore > 0 )
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptBenchmark.x+151,m_ptBenchmark.y+117,false );
	else
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptBenchmark.x+115,m_ptBenchmark.y+114,false );

	return;
}


//��������
void CScoreControl::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, bool bDrawCenter)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	if( bDrawCenter )
	{
		TEXTMETRIC tm;
		ZeroMemory( &tm,sizeof(tm) );
		GetTextMetrics(pDC->m_hDC,&tm );
		nXPos -= tm.tmAveCharWidth*nStringLength/2;
		nYPos -= tm.tmHeight/2;
	}

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//�滭����
void CScoreControl::DrawNumberString( CDC *pDC, CPngImage &PngNumber, int nXPos, int nYPos, LONG lNumber, bool bDrawCenter)
{
	int nWidthNumber = PngNumber.GetWidth()/10;
	int nHeightNumber = PngNumber.GetHeight();

	int nXDrawPos = nXPos, nYDrawPos = nYPos;
	nYDrawPos -= nHeightNumber/2;

	CHAR szNum[256] = {0};
	_snprintf(szNum,CountArray(szNum),"%+ld",lNumber);

	if( bDrawCenter )
	{
		nXDrawPos -= (lstrlen(szNum)-1)*nWidthNumber/2;
	}

	for( int i = 1; i < lstrlen(szNum); i++ )
	{
		if( ::isdigit(szNum[i]) )
		{
			PngNumber.DrawImage( pDC,nXDrawPos,nYDrawPos,nWidthNumber,nHeightNumber,(((int)szNum[i]-48))*nWidthNumber,
				0,nWidthNumber,nHeightNumber );
		}
		else
		{
			break;
		}
		nXDrawPos += nWidthNumber;
	}
}

//
void CScoreControl::SetBenchmarkPos( int nXPos, int nYPos )
{
	m_ptBenchmark.SetPoint( nXPos,nYPos );
}

//////////////////////////////////////////////////////////////////////////
