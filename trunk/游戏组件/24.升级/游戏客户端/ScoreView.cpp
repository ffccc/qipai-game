#include "StdAfx.h"
#include "ScoreView.h"
#include ".\scoreview.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView()
{
	//������Ϣ
	m_wGameScore=0;
	m_wConcealTime=0;
	m_wConcealScore=0;
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//��������
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//������Դ
	m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());
	m_ImageBack.SetLoadInfo(IDB_SCORE_BACK,AfxGetInstanceHandle());
	m_ImageWinLose.SetLoadInfo(IDB_WIN_LOSE,AfxGetInstanceHandle());

	//��ȡ��С
	CImageHandle CardImageHandle(&m_ImageCard);
	m_CardSize.cx=m_ImageCard.GetWidth()/13;
	m_CardSize.cy=m_ImageCard.GetHeight()/5;

	return;
}

//��������
CScoreView::~CScoreView()
{
}

//������Ϣ
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),/*SWP_NOMOVE|*/SWP_NOZORDER);

	return 0;
}

//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//������Դ
	CImageHandle HandleBack(&m_ImageBack);
	CImageHandle HandleCard(&m_ImageCard);

	//��������
	CRect rt;
	GetClientRect(&rt);
	CRgn WndRgn;
	WndRgn.CreateRoundRectRgn(rt.left,rt.top,rt.right+1,rt.bottom+1,10,10);
	SetWindowRgn(WndRgn,TRUE);
	WndRgn.DeleteObject();

	//�滭����
	m_ImageBack.AlphaDrawImage(&dc,0,0,RGB(255,0,255));
	//m_ImageBack.BlendDrawImage(&dc,0,0,RGB(255,0,255),64);

	//�滭�˿�
	if (m_cbConcealCount>0)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//�����С
		CSize ControlSize;
		ControlSize.cy=m_CardSize.cy;
		ControlSize.cx=m_CardSize.cx+(m_cbConcealCount-1)*18;

		//����λ��
		int nYPos=48;
		int nXPos=(rcClient.Width()-ControlSize.cx)/2;

		//�滭�˿�
		for (BYTE i=0;i<m_cbConcealCount;i++)
		{
			//��������
			int nXImagePos=0,nYImagePos=0;

			//ͼƬλ��
			if (m_cbConcealCard[i]!=0)
			{
				if ((m_cbConcealCard[i]==0x4E)||(m_cbConcealCard[i]==0x4F))
				{
					nYImagePos=((m_cbConcealCard[i]&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
					nXImagePos=((m_cbConcealCard[i]&CARD_MASK_VALUE)%14)*m_CardSize.cx;
				}
				else
				{
					nXImagePos=((m_cbConcealCard[i]&CARD_MASK_VALUE)-1)*m_CardSize.cx;
					nYImagePos=((m_cbConcealCard[i]&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
				}
			}
			else
			{
				nXImagePos=m_CardSize.cx*2;
				nYImagePos=m_CardSize.cy*4;
			}

			//�滭�˿�
			m_ImageCard.AlphaDrawImage(&dc,nXPos+i*18,nYPos,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));
		}
	}

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//��ʾ��Ϣ
	TCHAR szBuffer[128]=TEXT("");
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("��Ϸ�÷� %d + ���Ʒ� %d �� %d �� = %d"),m_wGameScore,
		m_wConcealScore,m_wConcealTime,m_wGameScore+m_wConcealScore*m_wConcealTime);

	//�滭��ʾ
	CRect rcString(25,166,307,184);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcString,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//��ʾ����
	CImageHandle HandleWinLose(&m_ImageWinLose);
	INT nWinLoseWidth = m_ImageWinLose.GetWidth()/3;
	INT nWinLostHeight = m_ImageWinLose.GetHeight();
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			//�������
			CRect rcName(26,i*29+225,126,(i+1)*29+225);
			dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//����ɼ�
			CRect rcScore(127,i*29+225,216,(i+1)*29+225);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lScore[i]);
			dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//ʤ����־
			BYTE byIndex = 0;
			if( m_lScore[i] == 0 ) byIndex = 2;
			else if( m_lScore[i] < 0 ) byIndex = 1;
			m_ImageWinLose.AlphaDrawImage(&dc,261-nWinLoseWidth/2,i*29+239-nWinLostHeight/2,nWinLoseWidth,nWinLostHeight,
				byIndex*nWinLoseWidth,0,RGB(255,0,255));
		}
	}

	return;
}

//������Ϣ
void CScoreView::SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo)
{
	//���ñ���
	m_wGameScore=ScoreViewInfo.wGameScore;
	m_wConcealTime=ScoreViewInfo.wConcealTime;
	m_wConcealScore=ScoreViewInfo.wConcealScore;
	m_cbConcealCount=ScoreViewInfo.cbConcealCount;
	CopyMemory(m_cbConcealCard,ScoreViewInfo.cbConcealCard,m_cbConcealCount*sizeof(BYTE));

	//ˢ�½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore)
{
	if (wChairID<CountArray(m_lScore))
	{
		m_lScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}
	return;
}

//��꼤��
int CScoreView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	//Invalidate(FALSE);
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////



