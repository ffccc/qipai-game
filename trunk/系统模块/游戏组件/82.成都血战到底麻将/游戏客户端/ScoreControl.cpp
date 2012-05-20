#include "StdAfx.h"
#include "ScoreControl.h"

//////////////////////////////////////////////////////////////////////////


//构造函数
CScoreControl::CScoreControl()
{
	//设置变量
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));
	m_ChiHuRight.SetEmpty();
	m_ptBenchmark.SetPoint(0,0);
	m_bShow = false;

	//加载资源
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageGameLost.LoadImage( hResInstance,TEXT("GAME_LOST") );
	m_ImageGameWin.LoadImage( hResInstance,TEXT("GAME_WIN") );
	m_PngNum.LoadImage( hResInstance,TEXT("NUM_ORDER") );

	return;
}

//析构函数
CScoreControl::~CScoreControl()
{
}

//复位数据
void CScoreControl::RestorationData()
{
	//设置变量
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));
	m_ChiHuRight.SetEmpty();
	m_bShow = false;

	return;
}

//设置积分
void CScoreControl::SetScoreInfo(const tagScoreInfo & ScoreInfo)
{
	//设置变量
	m_ScoreInfo=ScoreInfo;

	m_ChiHuRight.SetRightData( ScoreInfo.dwChiHuRight,MAX_RIGHT_COUNT );

	//显示窗口
	m_bShow = true;

	return;
}

//重画函数
void CScoreControl::DrawScoreControl( CDC *pDC )
{
	if( !m_bShow ) return;

	pDC->SetBkMode(TRANSPARENT);

	int nXPos = m_ptBenchmark.x;
	int nYPos = m_ptBenchmark.y;

	//绘画背景
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

	//胡牌名次
	if( m_ScoreInfo.cbWinOrder > 0 )
	{
		if( m_ScoreInfo.lGameScore > 0 )
			DrawNumberString( pDC,m_PngNum,m_ptBenchmark.x+58,m_ptBenchmark.y+30,m_ScoreInfo.cbWinOrder,true );
		else
			DrawNumberString( pDC,m_PngNum,m_ptBenchmark.x+20,m_ptBenchmark.y+27,m_ScoreInfo.cbWinOrder,true );
	}

	//胡牌信息
	TCHAR szBuffer[256] = TEXT("");
	if( !m_ChiHuRight.IsEmpty() )
	{
		if( m_ScoreInfo.wMeChairId == m_ScoreInfo.wProvideUser )
			lstrcpy( szBuffer,TEXT("自摸") );
		else 
		{
			lstrcpy( szBuffer,TEXT("赢：") );
			TCHAR szName[10] = TEXT("");
			lstrcpyn( szName,m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],sizeof(szName)/sizeof(TCHAR));
			lstrcat( szBuffer,szName );
			if( lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]) > 8 )
				lstrcat( szBuffer,TEXT("...") );
			lstrcat( szBuffer,TEXT(" 放炮") );
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
			TEXT("素番 1番 "),TEXT("天胡 6番 "),TEXT("地胡 6番 "),
			TEXT("大对子 2番 "),TEXT("清一色 3番 "),TEXT("七小对 3番 "),TEXT("带幺 3番 "),TEXT("将对 4番 "),
			TEXT("清对 4番 "),TEXT("龙七对 5番 "),TEXT("清七对 5番 "),TEXT("清幺九 5番 "),TEXT("清龙七对 6番 "),
			TEXT("杠上炮 1翻 "),TEXT("杠上花 1翻 ")
		};
		szBuffer[0] = 0;
		for( BYTE j = 0; j < CountArray(pszRight); j++ )
		{
			if( !(m_ChiHuRight&dwRight[j]).IsEmpty() )
				lstrcat( szBuffer,pszRight[j] );
		}
		//根
		if( m_ScoreInfo.cbGenCount > 0 )
		{
			TCHAR szGen[8];
			_snprintf( szGen,CountArray(szGen),TEXT("根 %d番"),m_ScoreInfo.cbGenCount );
			lstrcat( szBuffer,szGen );
		}
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),nXPos,nYPos,false );
		nYPos += 19;
	}

	//输给玩家
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
				lstrcpy( szBuffer,TEXT("输给 ") );
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

	//刮风下雨
	if( m_ScoreInfo.lGangScore != 0 )
	{
		_snprintf( szBuffer,CountArray(szBuffer),TEXT("刮风下雨：%ld"),m_ScoreInfo.lGangScore );
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),nXPos,nYPos,false );
	}

	//总计
	_snprintf( szBuffer,CountArray(szBuffer),TEXT("%ld"),m_ScoreInfo.lGameScore );
	if( m_ScoreInfo.lGameScore > 0 )
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptBenchmark.x+151,m_ptBenchmark.y+117,false );
	else
		DrawTextString( pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptBenchmark.x+115,m_ptBenchmark.y+114,false );

	return;
}


//艺术字体
void CScoreControl::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, bool bDrawCenter)
{
	//变量定义
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

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//绘画数字
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
