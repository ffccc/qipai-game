#include "StdAfx.h"
#include "DlgStatus.h"
#include "PlatformFrame.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//ʱ���ʶ
#define IDT_STATUS					10									//״̬ʱ��
#define TIMER_STATUS				20									//״̬ʱ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgStatus, CDialog)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//�ֲ���Ϣ
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()

	//�ؼ���Ϣ
	ON_BN_CLICKED(IDC_GIVE_UP, OnBnClickedGiveUp)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgStatus::CDlgStatus() : CDialog(IDD_DLG_STATUS)
{
	//״̬����
	m_uImageIndex=0;

	//�ӿڱ���
	m_pIStatusViewSink=NULL;

	return;
}

//��������
CDlgStatus::~CDlgStatus()
{
}

//�ؼ���
VOID CDlgStatus::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GIVE_UP, m_btGiveUp);
}

//��Ϣ����
BOOL CDlgStatus::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CDlgStatus::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(szProduct);

	//���ÿؼ�
	SetTimer(IDT_STATUS,TIMER_STATUS,NULL);

	//������Դ
	CPngImage ImageStatus;
	ImageStatus.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_IMAGE"));

	//���ð�ť
	m_btGiveUp.SetWindowPos(NULL,348,39,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btGiveUp.SetButtonImage(IDB_BT_STATUS_CANCEL,AfxGetInstanceHandle(),false,false);

	//���ô�С
	CSize SizeWindow(ImageStatus.GetWidth(),ImageStatus.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//���д���
	CenterWindow();

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=35;
	rcUnLayered.left=30;
	rcUnLayered.right=rcWindow.Width()-10;
	rcUnLayered.bottom=rcWindow.Height()-10;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageStatus,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//�رմ���
VOID CDlgStatus::HideStatusWindow()
{
	//�رմ���
	if (m_hWnd!=NULL) DestroyWindow();

	return;
}

//������Ϣ
VOID CDlgStatus::ShowStatusWindow(LPCTSTR pszDescribe)
{
	//��������
	if (m_hWnd==NULL)
	{
		if (CPlatformFrame::GetInstance()->IsWindowVisible()==FALSE)
		{
			Create(IDD_DLG_STATUS,GetDesktopWindow());
		}
		else
		{
			Create(IDD_DLG_STATUS,CPlatformFrame::GetInstance());
		}
	}

	//��������
	m_strDescribe=pszDescribe;

	//��ʾ����
	ShowWindow(SW_SHOW);

	//���´���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return;
}

//���ýӿ�
VOID CDlgStatus::SetStatusViewSink(IStatusViewSink * pIStatusViewSink)
{
	//���ñ���
	m_pIStatusViewSink=pIStatusViewSink;

	return;
}

//ʱ����Ϣ
VOID CDlgStatus::OnTimer(UINT nIDEvent)
{
	//���ý���
	if (nIDEvent==IDT_STATUS)
	{
		//��������
		m_uImageIndex=(m_uImageIndex>=100)?0:(m_uImageIndex+1L);

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//ȡ����ť
VOID CDlgStatus::OnBnClickedGiveUp()
{
	//���ؽ���
	HideStatusWindow();

	//Ͷ����Ϣ
	ASSERT(m_pIStatusViewSink!=NULL);
	if (m_pIStatusViewSink!=NULL) m_pIStatusViewSink->OnStatusCancel();

	return;
}

//�滭����
BOOL CDlgStatus::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CBitImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//�����豸
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	CPngImage ImageStatus;
	ImageStatus.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_IMAGE"));
	ImageStatus.DrawImage(pBufferDC,0,0,rcClient.Width(),rcClient.Height(),0,0);

	//������Դ
	CPngImage ImageStatusLine;
	CPngImage ImageStatusPoint;
	ImageStatusLine.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_LINE"));
	ImageStatusPoint.LoadImage(AfxGetInstanceHandle(),TEXT("STATUS_POINT"));

	//��ȡ��С
	CSize SizeStatusLine;
	CSize SizeStatusPoint;
	SizeStatusLine.SetSize(ImageStatusLine.GetWidth(),ImageStatusLine.GetHeight()/2);
	SizeStatusPoint.SetSize(ImageStatusPoint.GetWidth(),ImageStatusPoint.GetHeight());

	//�滭����
	ImageStatusLine.DrawImage(pBufferDC,105,45,SizeStatusLine.cx,SizeStatusLine.cy,0,0);
	ImageStatusLine.DrawImage(pBufferDC,105,45,SizeStatusLine.cx*m_uImageIndex/100L,SizeStatusLine.cy,0,SizeStatusLine.cy);

	//�滭����
	INT nXPoint=SizeStatusLine.cx*m_uImageIndex/100L;
	ImageStatusPoint.DrawImage(pBufferDC,105+nXPoint-SizeStatusPoint.cx/2,45+(SizeStatusLine.cy-SizeStatusPoint.cy)/2);

	//����λ��
	CRect rcDescribe;
	rcDescribe.SetRect(115,72,320,84);

	//�滭����
	pBufferDC->SetTextColor(RGB(0,0,0));
	pBufferDC->DrawText(m_strDescribe,rcDescribe,DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//��ʾ��Ϣ
VOID CDlgStatus::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ�ֲ�
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//�����Ϣ
VOID CDlgStatus::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//λ�øı�
VOID CDlgStatus::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
