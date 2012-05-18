#include "Stdafx.h"
#include "Resource.h"
#include "DlgHint.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_HINT_TIME			10									//ʱ���ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgHint, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgHint::CDlgHint() : CDialog(IDD_USER_HINT)
{
	//���ñ���
	m_nResidualTime=9L;

	//������Դ
	m_ImageViewBack.SetLoadInfo(IDB_HINT_VIEW,AfxGetInstanceHandle());

	return;
}

//��������
CDlgHint::~CDlgHint()
{
}

//�ؼ���
VOID CDlgHint::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
}

//��Ϣ����
BOOL CDlgHint::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if ((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)||(pMsg->wParam==VK_SPACE))
		{
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//��ʼ������
BOOL CDlgHint::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	CRgn RgnImage;
	CImageHandle HandleViewBack(&m_ImageViewBack);
	m_ImageViewBack.CreateImageRegion(RgnImage,RGB(255,0,255));

	//���ô���
	SetWindowRgn(RgnImage,TRUE);
	SetWindowPos(NULL,0,0,m_ImageViewBack.GetWidth(),m_ImageViewBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//���ÿؼ�
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btOk.SetButtonImage(IDB_BT_HINT_OK,hResInstance,false);

	//����ʱ��
	SetTimer(IDI_HINT_TIME,1000,NULL);

	return FALSE;
}

//ȷ����Ϣ
VOID CDlgHint::OnOK()
{
	//���ٴ���
	DestroyWindow();

	return;
}

//�ػ�����
VOID CDlgHint::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);
	DCBuffer.SetTextColor(RGB(125,125,125));
	DCBuffer.SetTextAlign(TA_CENTER|TA_TOP);
	DCBuffer.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle HandleViewBack(&m_ImageViewBack);
	m_ImageViewBack.BitBlt(DCBuffer,0,0);

	//ʣ��ʱ��
	TCHAR szTimeLeave[32]=TEXT("");
	_sntprintf(szTimeLeave,CountChar(szTimeLeave),TEXT("�˶Ի����� %d ����Զ��ر�"),m_nResidualTime);
	DCBuffer.TextOut(rcClient.Width()/2,rcClient.bottom-75,szTimeLeave,lstrlen(szTimeLeave));

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//ʱ����Ϣ
VOID CDlgHint::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_HINT_TIME)
	{
		//�ر��ж�
		if (m_nResidualTime>0)
		{
			//���ñ���
			m_nResidualTime--;

			//���½���
			Invalidate(FALSE);
		}
		else PostMessage(WM_CLOSE,0,0);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//�滭����
BOOL CDlgHint::OnEraseBkgnd(CDC * pDC)
{
	//���´���
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//�����Ϣ
VOID CDlgHint::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//����ʱ��
VOID CDlgHint::InitTimerCount()
{
	m_nResidualTime=9L;

	return;
}

//////////////////////////////////////////////////////////////////////////
