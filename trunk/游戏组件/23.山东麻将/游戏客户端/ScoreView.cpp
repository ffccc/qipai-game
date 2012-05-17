#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BT_OK,OnBtOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
    m_szFang= new char[256];
	memset(m_szFang,0,sizeof(m_szFang));
	//������Դ
	//m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
	m_ImageBack.SetLoadInfo(IDB_TOL3,AfxGetInstanceHandle());
	m_ImageHead.SetLoadInfo(IDB_TOL,AfxGetInstanceHandle());
	m_ImageBody.SetLoadInfo(IDB_TOL2,AfxGetInstanceHandle());
	m_ImageLine.SetLoadInfo(IDB_TOL4,AfxGetInstanceHandle());

	
}

//��������
CScoreView::~CScoreView(void)
{
	delete m_szFang;
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
	CImageHandle ImageHBack(&m_ImageBack);
	CImageHandle ImageHHead(&m_ImageHead);
	CImageHandle ImageHBody(&m_ImageBody);
	CImageHandle ImageHLine(&m_ImageLine);
	SetWindowPos(NULL,0,0,m_ImageHead.GetWidth(),m_ImageHead.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);


	CRect CreateRect( m_ImageHead.GetWidth()/2-30,m_ImageHead.GetHeight()-40, m_ImageHead.GetWidth()/2, m_ImageHead.GetHeight() );
	//������Ť
	//m_ButtonOK.Create( TEXT(""),WS_CHILD, CreateRect, this, IDC_BT_OK );
	//���ð�Ť( ��ͼ )
	//m_ButtonOK.SetButtonImage( IDB_ENT, AfxGetInstanceHandle(), 0 );

	HDWP hDwp=BeginDeferWindowPos(7);
	//m_ButtonOK.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	//DeferWindowPos( hDwp, m_ButtonOK, NULL,m_ImageHead.GetWidth()/2-5,m_ImageHead.GetHeight(), 0,0, uFlags );	

	//m_ButtonOK.ShowWindow(SW_SHOW);
	return TRUE;
}
void CScoreView::OnBtOK()
{
	CDialog::OnOK();
}
//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 


   // SetupRgn( &dc, m_ImageBack, RGB( 255, 0, 255) );

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle ImageHBack(&m_ImageBack);
	CImageHandle ImageHHead(&m_ImageHead);
	CImageHandle ImageHBody(&m_ImageBody);
	CImageHandle ImageHLine(&m_ImageLine);
	//m_ImageBack.BitBlt(dc,0,0);
	//m_ImageBack.AlphaDrawImage( &dc, 0,0,RGB( 255, 0, 255 ) );
	m_ImageHead.AlphaDrawImage( &dc, 0,0,RGB( 255, 0, 255 ) );

	//for(int i=0;i<LINE_COUNT;i++)
	 // m_ImageBack.AlphaDrawImage( &dc, 0,m_ImageHead.GetHeight()+i*m_ImageBack.GetHeight(),RGB( 255, 0, 255 ) );

	// m_ImageLine.AlphaDrawImage( &dc, (m_ImageBack.GetWidth()-m_ImageLine.GetWidth())/2,m_ImageHead.GetHeight()+(LINE_COUNT-20)*m_ImageBack.GetHeight(),RGB( 255, 0, 255 ) );
	// m_ImageBody.AlphaDrawImage( &dc, 0,m_ImageHead.GetHeight()+LINE_COUNT*m_ImageBack.GetHeight(),RGB( 255, 0, 255 ) );
	

	//��ʾ����
	CRect rcDraw;
	TCHAR szBuffer[64]=TEXT("");

	CFont font;
	font.CreateFont(20,10,0,0,FW_THIN,true,false,false,
		CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
		FF_MODERN,"����");
	/*dc.SetTextColor(RGB(0,255,0));

	rcDraw.left=90;
	rcDraw.right=194;
	rcDraw.top=130;
	rcDraw.bottom=rcDraw.top+82;
	dc.DrawText(_T("UserName"),lstrlen(_T("UserName")),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	rcDraw.left=200;
	rcDraw.right=m_ImageBack.GetWidth();
	dc.DrawText(_T("Score"),lstrlen(_T("Score")),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);*/

	dc.SetTextColor(RGB(255,255,255));

	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		//�û�����
		rcDraw.left=50;
		rcDraw.right=90;
		rcDraw.top=i*32+85;
		rcDraw.bottom=rcDraw.top+22;
		dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		//�û�����
		rcDraw.left=90;
		rcDraw.right=m_ImageBack.GetWidth();
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lGameScore[i]);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	}

	//��ʾ˰��
	/*rcDraw.left=90;
	rcDraw.right=194;
	rcDraw.top=i*22+150;
	rcDraw.bottom=rcDraw.top+82;
	LPCTSTR pszTax=TEXT("����˰");
	dc.DrawText(pszTax,lstrlen(pszTax),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);*/

	//��ʾ˰��
	rcDraw.left=140;
	rcDraw.right=170;
	rcDraw.top=i*22+160;
	rcDraw.bottom=rcDraw.top+32;
	_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_lGameTax);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);


	//strcpy(m_szFang,"ѵڬ���� 1 ���������� 1 ������ ���� 1 ����");
	//dc.SelectObject( &font);
	dc.SetTextColor(RGB(255,255,255));
	rcDraw.left= 20;
	rcDraw.right=m_ImageBack.GetWidth();
	rcDraw.top=i*32+125;
	rcDraw.bottom=rcDraw.top+80;
	
	int max =lstrlen(m_szFang);
	if(lstrlen(m_szFang)<rcDraw.right/8)
	{
		dc.DrawText( m_szFang, lstrlen(m_szFang), &rcDraw,DT_LEFT|DT_WORDBREAK);
	}
	else
	{
		dc.DrawText( m_szFang, rcDraw.right/8, &rcDraw,DT_LEFT|DT_WORDBREAK);
		rcDraw.top+=22;
		rcDraw.bottom=rcDraw.top+80;
		dc.DrawText( m_szFang+rcDraw.right/8, lstrlen(m_szFang)-(rcDraw.right/8), &rcDraw,DT_LEFT|DT_WORDBREAK);
	}
	//dc.DrawTextEx()
	return;
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
//͸������
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

void CScoreView::SetFangText( char *pszFang )
{
	strcpy( m_szFang, pszFang);
}

//////////////////////////////////////////////////////////////////////////
