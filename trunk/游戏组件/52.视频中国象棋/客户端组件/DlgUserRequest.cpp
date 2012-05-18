#include "Stdafx.h"
#include "Resource.h"
#include "DlgUserRequest.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_REQUEST_TIME			10									//ʱ���ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgUserRequest, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgUserRequest::CDlgUserRequest() : CDialog(IDD_USER_REQUEST)
{
	//���ñ���
	m_cbRequestType=0;
	m_nResidualTime=30L;

	//������Դ
	m_ImageViewBack.SetLoadInfo(IDB_REQUEST_VIEW,AfxGetInstanceHandle());

	return;
}

//��������
CDlgUserRequest::~CDlgUserRequest()
{
}

//�ؼ���
VOID CDlgUserRequest::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��Ϣ����
BOOL CDlgUserRequest::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if ((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��ʼ������
BOOL CDlgUserRequest::OnInitDialog()
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
	m_btOk.SetButtonImage(IDB_REQUEST_BT_OK,hResInstance,false);
	m_btCancel.SetButtonImage(IDB_REQUEST_BT_CANCEL,hResInstance,false);

	//����ʱ��
	SetTimer(IDI_REQUEST_TIME,1000,NULL);

	return FALSE;
}

//ȷ����Ϣ
VOID CDlgUserRequest::OnOK()
{
	//���ٴ���
	DestroyWindow();

	//��ѯ�ӿ�
	//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetGlobalUnits();
	//IGameFrameEngine * pIGameFrameEngine=(IGameFrameEngine *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_ENGINE,IID_IGameFrameEngine,VER_IGameFrameEngine);

	//������Ϣ
	//ASSERT(pIGameFrameEngine!=NULL);
	/*if (pIGameFrameEngine!=NULL) */AfxGetMainWnd()->SendMessage(IDM_USER_REQUEST_RESPOND,m_cbRequestType,TRUE);

	return;
}

//ȷ����Ϣ
VOID CDlgUserRequest::OnCancel()
{
	//���ٴ���
	DestroyWindow();

	//��ѯ�ӿ�
	//CGlobalUnits * pGlobalUnits=CGlobalUnits::GetGlobalUnits();
	//IGameFrameEngine * pIGameFrameEngine=(IGameFrameEngine *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_ENGINE,IID_IGameFrameEngine,VER_IGameFrameEngine);

	//������Ϣ
	//ASSERT(pIGameFrameEngine!=NULL);
	/*if (pIGameFrameEngine!=NULL) */AfxGetMainWnd()->SendMessage(IDM_USER_REQUEST_RESPOND,m_cbRequestType,FALSE);
	
	return;
}

//�ػ�����
VOID CDlgUserRequest::OnPaint()
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

	//��������
	if (m_cbRequestType==REQUEST_PEACE)
	{
		//������Դ
		CSkinImage RequestStringImage;
		RequestStringImage.LoadFromResource(AfxGetInstanceHandle(),IDB_REQUEST_PEACE_STRING);

		//�滭��Ϣ
		INT nImageWidth=RequestStringImage.GetWidth();
		INT nImageHeight=RequestStringImage.GetHeight();
		RequestStringImage.BitBlt(DCBuffer,(rcClient.Width()-nImageWidth)/2,88);
	}

	//��������
	if (m_cbRequestType==REQUEST_REGRET)
	{
		//������Դ
		CSkinImage RequestStringImage;
		RequestStringImage.LoadFromResource(AfxGetInstanceHandle(),IDB_REQUEST_REGRET_STRING);

		//�滭��Ϣ
		INT nImageWidth=RequestStringImage.GetWidth();
		INT nImageHeight=RequestStringImage.GetHeight();
		RequestStringImage.BitBlt(DCBuffer,(rcClient.Width()-nImageWidth)/2,88);
	}

	//ʣ��ʱ��
	TCHAR szTimeLeave[32]=TEXT("");
	_sntprintf(szTimeLeave,CountChar(szTimeLeave),TEXT("�˶Ի����� %d ����Զ��ر�"),m_nResidualTime);
	DCBuffer.TextOut(rcClient.Width()/2,rcClient.bottom-85,szTimeLeave,lstrlen(szTimeLeave));

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//ʱ����Ϣ
VOID CDlgUserRequest::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_REQUEST_TIME)
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
BOOL CDlgUserRequest::OnEraseBkgnd(CDC * pDC)
{
	//���´���
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//�����Ϣ
VOID CDlgUserRequest::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//////////////////////////////////////////////////////////////////////////
