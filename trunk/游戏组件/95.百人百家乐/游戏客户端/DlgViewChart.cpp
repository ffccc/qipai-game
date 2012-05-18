// DlgViewChart.cpp : implementation file
//

#include "stdafx.h"
#include "DlgViewChart.h"
#include ".\dlgviewchart.h"

BEGIN_MESSAGE_MAP(CDlgViewChart, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CDlgViewChart, CDialog)
CDlgViewChart::CDlgViewChart():CDialog(IDD_VIEW_CHART, NULL)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_CHART_BACK,hInstance,false);

	//设置图片
	m_PngPlayerFlag.LoadImage(hInstance,TEXT("CHART_XIAN"));
	m_PngPlayerEXFlag.LoadImage(hInstance,TEXT("CHART_XIAN_EX"));
	m_PngBankerFlag.LoadImage(hInstance,TEXT("CHART_ZHUANG"));
	m_PngBankerEXFlag.LoadImage(hInstance,TEXT("CHART_ZHUANG_EX"));
	m_PngTieFlag.LoadImage(hInstance,TEXT("CHART_PING"));
	m_PngTieEXFlag.LoadImage(hInstance,TEXT("CHART_PING_EX"));
	m_PngTwopairFlag.LoadImage(hInstance,TEXT("TWO_PAIR_FLAG"));

	//初始变量
	ZeroMemory(m_TraceGameRecordArray,sizeof(m_TraceGameRecordArray));
	ZeroMemory(m_GameRecordArray,sizeof(m_GameRecordArray));
	m_TraceGameRecordCount=0;
	m_GameRecordCount=0;
	ZeroMemory(m_bFillTrace,sizeof(m_bFillTrace));
}

CDlgViewChart::~CDlgViewChart()
{
}

void CDlgViewChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDOK,m_btClose);
}

BOOL CDlgViewChart::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btClose.SetButtonImage(IDB_BT_CHART_CLOSE,AfxGetInstanceHandle(),false);

	CImageHandle ImageHandleBack(&m_ImageBack);
	MoveWindow(0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	CRect rcButton;
	m_btClose.GetWindowRect(rcButton);
	m_btClose.MoveWindow(m_ImageBack.GetWidth()-rcButton.Width(),0,rcButton.Width(),rcButton.Height());

	return TRUE;
}

void CDlgViewChart::OnPaint()
{
	CPaintDC dc(this); 

	CImageHandle ImageHandleBack(&m_ImageBack);
	m_ImageBack.BitBlt(dc.GetSafeHdc(),0,0);

	DrawPrecent(&dc);
	DrawChart(&dc);
}

void CDlgViewChart::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);

	//消息模拟
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}

//绘画表格
void CDlgViewChart::DrawChart(CDC *pDC)
{
	//位置变量
	int nPosX=27;
	int nPosY=167;
	int nGridWidth=31;
	int nGridHeight=31;

	//绘画结果
	int nVerCount=0, nHorCount=0;

	for (int nIndex=0; nIndex<m_GameRecordCount; ++nIndex)
	{
		tagClientGameRecord &ClientGameRecord=m_GameRecordArray[nIndex];

		if (ClientGameRecord.cbPlayerCount > ClientGameRecord.cbBankerCount)
		{
			m_PngPlayerFlag.DrawImage(pDC,nPosX+nHorCount*nGridWidth,nPosY+nVerCount*nGridHeight);
		}
		else if (ClientGameRecord.cbPlayerCount < ClientGameRecord.cbBankerCount)
		{
			m_PngBankerFlag.DrawImage(pDC,nPosX+nHorCount*nGridWidth,nPosY+nVerCount*nGridHeight);
		}
		else
		{
			m_PngTieFlag.DrawImage(pDC,nPosX+nHorCount*nGridWidth,nPosY+nVerCount*nGridHeight);
		}

		INT nWidth=m_PngTwopairFlag.GetWidth()/2;
		INT nHeight=m_PngTwopairFlag.GetHeight();
		if (ClientGameRecord.bBankerTwoPair) m_PngTwopairFlag.DrawImage(pDC,nPosX+nHorCount*nGridWidth+2,nPosY+nVerCount*nGridHeight+2,nWidth,nHeight,0,0);
		if (ClientGameRecord.bPlayerTwoPair) m_PngTwopairFlag.DrawImage(pDC,nPosX+(nHorCount+1)*nGridWidth-3-nWidth,nPosY+(nVerCount+1)*nGridHeight-3-nHeight,nWidth,nHeight,nWidth,0);

		//递增数目
		nVerCount++;		
		if (nVerCount==6)
		{
			nVerCount=0;
			nHorCount++;
		}
	}

	//绘画路径
	nPosX=27;
	nPosY=64;
	nGridWidth=16;
	nGridHeight=16;
	nVerCount=0, nHorCount=0;
	BYTE cbPreWinSide=0, cbCurWinSide=0;
	ZeroMemory(m_bFillTrace,sizeof(m_bFillTrace));
	for (int nIndex=0; nIndex<m_TraceGameRecordCount; ++nIndex)
	{
		tagClientGameRecord &ClientGameRecord=m_TraceGameRecordArray[nIndex];

		if (ClientGameRecord.cbPlayerCount > ClientGameRecord.cbBankerCount) cbCurWinSide=ID_ZHUANG_JIA;
		else if (ClientGameRecord.cbPlayerCount < ClientGameRecord.cbBankerCount) cbCurWinSide=ID_XIAN_JIA;
		else cbCurWinSide=ID_PING_JIA;

		//递增数目
		if (cbPreWinSide!=0 && (cbPreWinSide==cbCurWinSide || cbCurWinSide==ID_PING_JIA))
		{
			nVerCount++;
			if (m_bFillTrace[nVerCount][nHorCount]==true || nVerCount==6)
			{
				nVerCount--;
				nHorCount++;
			}
		}
		else
		{
			nVerCount=0;

			//第一次
			if (cbPreWinSide!=0)
			{
				for (int i=0; i<29; ++i)
				{
					if (m_bFillTrace[0][i]==false)
					{
						nHorCount=i;
						break;
					}
				}				
			}

			cbPreWinSide=cbCurWinSide;
		}

		//设置标识
		m_bFillTrace[nVerCount][nHorCount]=true;

		//清零判断
		if (nHorCount==29)
		{
			m_TraceGameRecordCount=0;
			ZeroMemory(m_bFillTrace,sizeof(m_bFillTrace));
			InvalidateRect(NULL);
			break;
		}

		if (cbCurWinSide==ID_ZHUANG_JIA)
		{
			m_PngPlayerEXFlag.DrawImage(pDC,nPosX+nHorCount*nGridWidth,nPosY+nVerCount*nGridHeight);
		}
		else if (cbCurWinSide==ID_XIAN_JIA)
		{
			m_PngBankerEXFlag.DrawImage(pDC,nPosX+nHorCount*nGridWidth,nPosY+nVerCount*nGridHeight);
		}
		else
		{
			m_PngTieEXFlag.DrawImage(pDC,nPosX+nHorCount*nGridWidth,nPosY+nVerCount*nGridHeight);
		}
	}
}

//绘画表格
void CDlgViewChart::DrawPrecent(CDC *pDC)
{
	//创建资源
	CFont InfoFont;
	InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));

	//设置 DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_CENTER|TA_TOP);
	CFont * pOldFont=pDC->SelectObject(&InfoFont);

	//获取大小
	CRect rcClient;
	GetClientRect(rcClient);

	//数目统计
	int nBankerCount=0, nPlayerCount=0, nTieCount=0, nBankerTwoPairCount=0, nPlayerTwoPairCount=0, nBankerKingCount=0, nPlayerKingCount=0;

	for (int nIndex=0; nIndex<m_GameRecordCount; ++nIndex)
	{
		tagClientGameRecord &ClientGameRecord=m_GameRecordArray[nIndex];

		if (ClientGameRecord.cbBankerCount > ClientGameRecord.cbPlayerCount)
		{
			nBankerCount++;
		}
		else if (ClientGameRecord.cbBankerCount < ClientGameRecord.cbPlayerCount)
		{
			nPlayerCount++;
		}
		else
		{
			nTieCount++;
		}
		
		//对子判断
		if (ClientGameRecord.bBankerTwoPair) nBankerTwoPairCount++;
		if (ClientGameRecord.bPlayerTwoPair) nPlayerTwoPairCount++;

		if (ClientGameRecord.cbKingWinner==ID_XIAN_TIAN_WANG) nPlayerKingCount++;
		else if (ClientGameRecord.cbKingWinner==ID_ZHUANG_TIAN_WANG) nBankerKingCount++;
	}

	CString strCount;
	float fTatolCount=float(nBankerCount+nPlayerCount+nTieCount);
	if (fTatolCount<=0) fTatolCount=1.0;

	int nSpace=45,nPosX=0,nPosY=46;

	//闲家
	nPosX=85;
	strCount.Format(TEXT("%d"),nPlayerCount);
	DrawTextString(pDC,strCount,RGB(0,0,255),RGB(0,0,0),nPosX,22);
	strCount.Format(TEXT("%.2f%%"),100*nPlayerCount/fTatolCount);
	DrawTextString(pDC,strCount,RGB(0,0,255),RGB(0,0,0),nPosX+nSpace,22);

	//闲对子
	nPosX=85;
	strCount.Format(TEXT("%d"),nPlayerTwoPairCount);
	DrawTextString(pDC,strCount,RGB(0,0,255),RGB(0,0,0),nPosX,nPosY);

	//平家
	nPosX=242;
	strCount.Format(TEXT("%d"),nTieCount);
	DrawTextString(pDC,strCount,RGB(0,255,0),RGB(0,0,0),nPosX,22);
	strCount.Format(TEXT("%.2f%%"),100*nTieCount/fTatolCount);
	DrawTextString(pDC,strCount,RGB(0,255,0),RGB(0,0,0),nPosX+nSpace,22);

	//庄家
	nPosX=399;
	strCount.Format(TEXT("%d"),nBankerCount);
	DrawTextString(pDC,strCount,RGB(255,0,0),RGB(0,0,0),nPosX,22);
	strCount.Format(TEXT("%.2f%%"),100*nBankerCount/fTatolCount);
	DrawTextString(pDC,strCount,RGB(255,0,0),RGB(0,0,0),nPosX+nSpace,22);	

	//庄对子
	nPosX=210;
	strCount.Format(TEXT("%d"),nBankerTwoPairCount);
	DrawTextString(pDC,strCount,RGB(255,0,0),RGB(0,0,0),nPosX,nPosY);

	//庄天王
	nPosX=448;
	strCount.Format(TEXT("%d"),nBankerKingCount);
	DrawTextString(pDC,strCount,RGB(255,0,0),RGB(0,0,0),nPosX,nPosY);

	//闲天王
	nPosX=330;
	strCount.Format(TEXT("%d"),nPlayerKingCount);
	DrawTextString(pDC,strCount,RGB(0,0,255),RGB(0,0,0),nPosX,nPosY);

	//清理资源
	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();
}

//艺术字体
void CDlgViewChart::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//设置结果
void CDlgViewChart::SetGameRecord(const tagClientGameRecord &ClientGameRecord)
{
	//保存结果
	m_TraceGameRecordArray[m_TraceGameRecordCount++]=ClientGameRecord;
	m_GameRecordArray[m_GameRecordCount++]=ClientGameRecord;

	//清空判断
	if (m_TraceGameRecordCount>=100) m_TraceGameRecordCount=0;
	if (m_GameRecordCount>=80) m_GameRecordCount=0;

	//更新界面
	if (IsWindowVisible()) InvalidateRect(NULL);
}