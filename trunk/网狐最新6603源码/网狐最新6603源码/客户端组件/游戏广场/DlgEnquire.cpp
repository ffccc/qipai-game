#include "Stdafx.h"
#include "DlgEnquire.h"
#include "GlobalUnits.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					5									//Բ�ǿ��
#define ROUND_CY					5									//Բ�Ǹ߶�

//��Ļλ��
#define LAYERED_SIZE				5									//�ֲ��С
#define CAPTION_SIZE				60									//�����С

//�ؼ���ʶ
#define IDC_WEB_PUBLICIZE			300									//����ؼ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgEnquire, CDialog)

	//ϵͳ��Ϣ
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_CLOSE_PLAZA, OnBnClickedClosePlaza)
	ON_BN_CLICKED(IDC_CLOSE_SERVER, OnBnClickedCloseServer)
	ON_BN_CLICKED(IDC_SWITCH_ACCOUNTS, OnBnClickedSwitchAccounts)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgEnquire::CDlgEnquire() : CDialog(IDD_DLG_ENQUIRE)
{
}

//��������
CDlgEnquire::~CDlgEnquire()
{
}

//�ؼ���
VOID CDlgEnquire::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//��ť�ؼ�
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_CLOSE_PLAZA, m_btClosePlaza);
	DDX_Control(pDX, IDC_CLOSE_SERVER, m_btCloseServer);
	DDX_Control(pDX, IDC_SWITCH_ACCOUNTS, m_btSwitchAccounts);
}

//���ú���
BOOL CDlgEnquire::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��ܰ��ʾ"));

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_ENQUIRE_BACK"));

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//���ư�ť
	CWndViewItemCtrl * pWndViewItemCtrl=CWndViewItemCtrl::GetInstance();
	m_btCloseServer.EnableWindow((pWndViewItemCtrl->GetActiveItem()>=1)?TRUE:FALSE);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btCancel.SetButtonImage(IDB_BT_ENQUIRE_CANCEL,TEXT("BT_ENQUIRE_CANCEL"),hInstance,false,false);
	m_btClosePlaza.SetButtonImage(IDB_BK_ENQUIRE_BUTTON,TEXT("BT_ENQUIRE_PLAZA"),hInstance,false,false);
	m_btCloseServer.SetButtonImage(IDB_BK_ENQUIRE_BUTTON,TEXT("BT_ENQUIRE_SERVER"),hInstance,false,false);
	m_btSwitchAccounts.SetButtonImage(IDB_BK_ENQUIRE_BUTTON,TEXT("BT_ENQUIRE_SWITCH"),hInstance,false,false);

	//�رհ�ť
	CRect rcCancel;
	m_btCancel.GetWindowRect(&rcCancel);
	m_btCancel.SetWindowPos(NULL,SizeWindow.cx-rcCancel.Width()-15,15,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);

	//��ť��С
	CRect rcButton;
	m_btClosePlaza.GetWindowRect(&rcButton);

	//���ܰ�ť
	INT nButtonSpace=(SizeWindow.cx-rcButton.Width()*3)/4;
	m_btClosePlaza.SetWindowPos(NULL,nButtonSpace*3+rcButton.Width()*2,148,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);
	m_btCloseServer.SetWindowPos(NULL,nButtonSpace*2+rcButton.Width()*1,148,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);
	m_btSwitchAccounts.SetWindowPos(NULL,nButtonSpace*1+rcButton.Width()*0,148,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);

	//���ؼ�
	CRect rcCreate(0,0,0,0);
	m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/ClosePlaza.asp"),szPlatformLink);

	//���ù��
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetWindowPos(NULL,7,58,SizeWindow.cx-14,80,SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);

	//���д���
	CenterWindow(this);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return FALSE;
}

//ȷ������
VOID CDlgEnquire::OnOK()
{
	return;
}

//�滭����
BOOL CDlgEnquire::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_ENQUIRE_BACK"));

	//�滭����
	ImageBack.DrawImage(pDC,0,0);

	return TRUE;
}

//��ʾ��Ϣ
VOID CDlgEnquire::OnShowWindow(BOOL bShow, UINT nStatus)
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
VOID CDlgEnquire::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//λ�øı�
VOID CDlgEnquire::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//�رմ���
VOID CDlgEnquire::OnBnClickedClosePlaza()
{
	EndDialog(IDC_CLOSE_PLAZA);

	return;
}

//�رշ���
VOID CDlgEnquire::OnBnClickedCloseServer()
{
	EndDialog(IDC_CLOSE_SERVER);

	return;
}

//�л��ʺ�
VOID CDlgEnquire::OnBnClickedSwitchAccounts()
{
	EndDialog(IDC_SWITCH_ACCOUNTS);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
