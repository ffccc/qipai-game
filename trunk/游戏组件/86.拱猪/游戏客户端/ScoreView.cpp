#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
}

//��������
CScoreView::~CScoreView(void)
{
}

//��ʼ������
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return TRUE;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,0,0));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());
	//����͸������
	SetupRgn(&dc,m_ImageBack,RGB(255,0,255));


	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	//m_ImageBack.BitBlt(dc,0,0);
	m_ImageBack.AlphaDrawImage(&dc,0,0,RGB(255,0,255));


	//��ʾ����
	dc.SetTextColor(RGB(255,255,102));
	CRect rcDraw;
	CString strScore ;
	char szBuffer[64] ;
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		//�û�����
		rcDraw.left=10+20+10;
		rcDraw.right=130;
		rcDraw.top=i*23+80+8;
		rcDraw.bottom=rcDraw.top+12;
		dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);

		//�û�����
		rcDraw.left=150-30-20-10-10;
		rcDraw.right=290;
		strScore.Format("%ld" , m_lGameScore[i]) ;
		//_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lGameScore[i]);
		dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
	}

	if(m_lGameTax>0)
	{
		//��ʾ˰��
		rcDraw.left=30;
		rcDraw.right=114;
		rcDraw.top=i*22+51;
		rcDraw.bottom=rcDraw.top+12;
		LPCTSTR pszTax=TEXT("����˰");
		dc.DrawText(pszTax,lstrlen(pszTax),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);

		//��ʾ˰��
		rcDraw.left=120;
		rcDraw.right=190;
		rcDraw.top=i*22+51;
		rcDraw.bottom=rcDraw.top+12;
	//	_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_lGameTax);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
	}
	return;
}


void CScoreView::SetupRgn(CDC *pDC,CSkinImage bImage,COLORREF TransColor)
{
	//bImage.GetWidth()/5;
	tagImageLoadInfo ImageIDB;
	bImage.GetLoadInfo(ImageIDB);
	CBitmap cBitmap;
	cBitmap.LoadBitmap(ImageIDB.uResourceID);//���BITMAP��5�����ģ����Ի�������������
	CDC memDC;
	//�����봫��DC���ݵ���ʱDC
	memDC.CreateCompatibleDC(pDC);

	CBitmap *pOldMemBmp=NULL;
	//��λͼѡ����ʱDC
	pOldMemBmp=memDC.SelectObject(&cBitmap);

	CRgn wndRgn;
	//�����ܵĴ������򣬳�ʼregionΪ0
	wndRgn.CreateRectRgn(0,0,0,0);


	BITMAP bit;   
	cBitmap.GetBitmap (&bit);//ȡ��λͼ����������Ҫ�õ�λͼ�ĳ��Ϳ�     

	int y;
	for(y=0;y<=bit.bmHeight  ;y++)
	{
		CRgn rgnTemp; //������ʱregion

		int iX = 0;
		do
		{
			//����͸��ɫ�ҵ���һ����͸��ɫ�ĵ�.
			while (iX <= bit.bmWidth  && memDC.GetPixel(iX, y) == TransColor)
				iX++;

			//��ס�����ʼ��
			int iLeftX = iX;

			//Ѱ���¸�͸��ɫ�ĵ�
			while (iX <= bit.bmWidth  && memDC.GetPixel(iX, y) != TransColor)
				++iX;

			//����һ������������ص���Ϊ1���ص���ʱ��region��
			rgnTemp.CreateRectRgn(iLeftX, y, iX, y+1);

			//�ϲ�����"region".
			wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_OR);

			//ɾ����ʱ"region",�����´δ���ʱ�ͳ���
			rgnTemp.DeleteObject();
		}while(iX <bit.bmWidth );
		iX = 0;
	}
	if(pOldMemBmp)
		memDC.SelectObject(pOldMemBmp);
	CWnd * pWnd = pDC->GetWindow();
	pWnd->SetWindowRgn(wndRgn,TRUE);
	pWnd->SetForegroundWindow();   
} 

//�滭����
BOOL CScoreView::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreView::SetGameTax(LONG lGameTax)
{
	//���ñ���
	if (m_lGameTax!=lGameTax)
	{
		m_lGameTax=lGameTax;
		Invalidate(FALSE);
	}
	
	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore)
{
	//���ñ���
	if (wChairID<CountArray(m_lGameScore))
	{
		m_lGameScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
