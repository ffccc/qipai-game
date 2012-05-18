#include "StdAfx.h"
#include "WebPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//���ȶ���
#define MAX_LOAD_INDEX				20									//�������
#define IDI_LOAD_TIMER				100									//����ʱ��

//��������
#define IDC_WEB_BROWSER				100									//���ڱ�ʶ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWebPublicize, CWnd)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CWebPublicize, CWnd)
	ON_EVENT(CWebPublicize, IDC_WEB_BROWSER, 259, OnDocumentCompleteWeb, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CWebPublicize, IDC_WEB_BROWSER, 271, OnNavigateErrorWeb, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CWebPublicize, IDC_WEB_BROWSER, 250, OnBeforeNavigate2Web, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWebPublicize::CWebPublicize()
{
	//���ñ���
	m_wIndex=0;
	m_cbStatus=BROWSER_STATUS_LOAD;

	return;
}

//��������
CWebPublicize::~CWebPublicize()
{
}

//�������
VOID CWebPublicize::Navigate(LPCTSTR pszURL)
{
	//��������
	if (m_WebBrowser.m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		m_WebBrowser.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_WEB_BROWSER);
	}

	//���ҳ��
	m_WebBrowser.Navigate(pszURL,NULL,NULL,NULL,NULL);

	return;
}

//�������
VOID CWebPublicize::Navigate(LPCTSTR pszURL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers)
{
	//��������
	if (m_WebBrowser.m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		m_WebBrowser.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_WEB_BROWSER);
	}

	//���ҳ��
	m_WebBrowser.Navigate(pszURL,NULL,NULL,NULL,NULL);

	return;
}

//״̬֪ͨ
VOID CWebPublicize::OnBrowserStatus(BYTE cbStatus)
{
	switch (cbStatus)
	{
	case BROWSER_STATUS_LOAD:		//����״̬
		{
			SetTimer(IDI_LOAD_TIMER,50,NULL);
			return;
		}
	case BROWSER_STATUS_FINISH:		//���״̬
		{
			KillTimer(IDI_LOAD_TIMER);
			return;
		}
	}

	return;
}

//λ�ú���
VOID CWebPublicize::GetWebBrowserRect(CRect&rcWebBrowser)
{
	//��ȡλ��
	GetClientRect(&rcWebBrowser);

	return;
}

//�滭����
VOID CWebPublicize::DrawPublicizeView(CDC * pDC, INT nWidth, INT nHeight, BYTE cbStatus)
{
	//�滭����
	pDC->FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

	//������Դ
	CPngImage ImageLoadLine1;
	CPngImage ImageLoadLine2;
	ImageLoadLine1.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("WEB_LOAD_LINE_1"));
	ImageLoadLine2.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("WEB_LOAD_LINE_2"));

	//��ȡ��С
	CSize SizeLoadLine1;
	CSize SizeLoadLine2;
	SizeLoadLine1.SetSize(ImageLoadLine1.GetWidth(),ImageLoadLine1.GetHeight());
	SizeLoadLine2.SetSize(ImageLoadLine2.GetWidth(),ImageLoadLine2.GetHeight());

	//�滭����
	INT nYDrawPos=(nHeight-SizeLoadLine1.cy)/2;
	INT nXDrawPos=(nWidth-SizeLoadLine1.cx-20)/2;
	ImageLoadLine1.DrawImage(pDC,nXDrawPos,nYDrawPos);

	//λ�ü���
	INT nCompartPos=SizeLoadLine1.cx*m_wIndex/MAX_LOAD_INDEX;
	INT nLoadLineDraw=__min(SizeLoadLine2.cx,SizeLoadLine1.cx-nCompartPos);

	//�滭����
	INT nYExcursion=(SizeLoadLine1.cy-SizeLoadLine2.cy)/2;
	ImageLoadLine2.DrawImage(pDC,nXDrawPos+nCompartPos,nYDrawPos+nYExcursion,nLoadLineDraw,SizeLoadLine1.cy,0,0);
	ImageLoadLine2.DrawImage(pDC,nXDrawPos,nYDrawPos+nYExcursion,(SizeLoadLine2.cx-nLoadLineDraw),SizeLoadLine1.cy,nLoadLineDraw,0);

	//��������
	CFont Font;
	Font.CreateFont(-12,0,0,0,800,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));

	//�����豸
	pDC->SelectObject(Font);
	pDC->SetTextColor(RGB(30,30,30));

	//�������
	LPCTSTR pszString=TEXT("���ڼ���ҳ�棬���Ժ�...");
	pDC->TextOut(nXDrawPos,nYDrawPos+20,pszString,lstrlen(pszString));

	//��ԭ�豸
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	return;
}

//�ػ���Ϣ
VOID CWebPublicize::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//�滭����
	CDC * pBufferDC=CDC::FromHandle(ImageBuffer.GetDC());
	DrawPublicizeView(pBufferDC,rcClient.Width(),rcClient.Height(),m_cbStatus);

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	//�ͷ���Դ
	ImageBuffer.ReleaseDC();

	return;
}

//ʱ����Ϣ
VOID CWebPublicize::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);

	//ʱ�䴦��
	if (nIDEvent==IDI_LOAD_TIMER)
	{
		//���ñ���
		m_wIndex=((m_wIndex+1)>=MAX_LOAD_INDEX)?0:(m_wIndex+1);

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		return;
	}

	return;
}

//λ����Ϣ
VOID CWebPublicize::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�ƶ�����
	if (m_WebBrowser.m_hWnd!=NULL)
	{
		if (m_cbStatus==BROWSER_STATUS_FINISH)
		{
			//��ȡλ��
			CRect rcWebBrowser;
			GetWebBrowserRect(rcWebBrowser);

			//�ƶ�λ��
			m_WebBrowser.SetWindowPos(NULL,rcWebBrowser.left,rcWebBrowser.top,rcWebBrowser.Width(),rcWebBrowser.Height(),SWP_NOZORDER);
		}
		else
		{
			//�ƶ�λ��
			m_WebBrowser.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER);
		}
	}

	return;
}

//�������
VOID CWebPublicize::OnDocumentCompleteWeb(LPDISPATCH pDisp, VARIANT FAR * URL)
{
	//���ñ���
	if (m_cbStatus!=BROWSER_STATUS_ERROR)
	{
		//���ñ���
		m_cbStatus=BROWSER_STATUS_FINISH;

		//״̬֪ͨ
		OnBrowserStatus(BROWSER_STATUS_FINISH);

		//�ƶ�����
		if (m_WebBrowser.m_hWnd!=NULL)
		{
			//��ȡλ��
			CRect rcWebBrowser;
			GetWebBrowserRect(rcWebBrowser);

			//�ƶ�λ��
			m_WebBrowser.SetWindowPos(NULL,rcWebBrowser.left,rcWebBrowser.top,rcWebBrowser.Width(),rcWebBrowser.Height(),SWP_NOZORDER);
		}
	}

	return;
}

//��������
VOID CWebPublicize::OnNavigateErrorWeb(LPDISPATCH pDisp, VARIANT FAR * URL, VARIANT FAR * Frame, VARIANT FAR * StatusCode, BOOL FAR * Cancel)
{
	//���ñ���
	m_cbStatus=BROWSER_STATUS_ERROR;

	//״̬֪ͨ
	OnBrowserStatus(BROWSER_STATUS_ERROR);

	//�ƶ�����
	if (m_WebBrowser.m_hWnd!=NULL) m_WebBrowser.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER);

	return;
}

//������ʼ
VOID CWebPublicize::OnBeforeNavigate2Web(LPDISPATCH pDisp, VARIANT FAR * URL, VARIANT FAR * Flags, VARIANT FAR * TargetFrameName, VARIANT FAR * PostData, VARIANT FAR * Headers, BOOL FAR * Cancel)
{
	//���ñ���
	m_cbStatus=BROWSER_STATUS_LOAD;

	//״̬֪ͨ
	OnBrowserStatus(BROWSER_STATUS_LOAD);

	//�ƶ�����
	if (m_WebBrowser.m_hWnd!=NULL) m_WebBrowser.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
