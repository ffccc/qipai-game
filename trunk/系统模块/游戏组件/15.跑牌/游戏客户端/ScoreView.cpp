#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	bImage.LoadBitmap(IDB_GAME_END);
	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
}

//��������
CScoreView::~CScoreView(void)
{
}

//������Ϣ
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//��������ﵽ͸��Ч��
	SetupRgn(&dc,bImage,RGB(255,0,255));

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//��ʾ����
	WORD wStation=0;
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			//�������
			CRect rcName(15,i*19+72,141,i*19+91);
			dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//����ɼ�
			CRect rcScore(150,i*19+72,206,i*19+91);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lScore[i]);
			dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//���ñ���
			wStation++;
		}
	}

	//��ʾ˰��
	if (wStation>0)
	{
		LPCTSTR pszTax=TEXT("����˰");
		dc.TextOut(65,(int)(wStation*20+90),pszTax,lstrlen(pszTax));
		_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_lTax);
		dc.TextOut(175,(int)(wStation*20+90),szBuffer,lstrlen(szBuffer));
	}
	return;
}

void CScoreView::SetupRgn(CDC *pDC,				//�����DCָ��
						  CBitmap &cBitmap,		//���д�����״��λͼ����
						  COLORREF TransColor)	//͸��ɫ
{

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

//���û���
void CScoreView::ResetScore()
{
	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreView::SetTax(LONG lTax)
{
	if (m_lTax!=lTax)
	{
		m_lTax=lTax;
		Invalidate(NULL);
	}
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

//////////////////////////////////////////////////////////////////////////
