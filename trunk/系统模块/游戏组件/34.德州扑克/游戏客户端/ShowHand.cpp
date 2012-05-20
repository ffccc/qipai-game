#include "stdafx.h"
#include "ShowHand.h"
#include ".\showhand.h"

//��ʱ����ʶ
#define IDI_START				200									//��ʼ��ʱ��
#define IDI_COUNT				201									//��ʼ��ʱ��

#define TIMER_START				5									//��ʼ��ʱ��

IMPLEMENT_DYNAMIC(ShowHand, CDialog)

BEGIN_MESSAGE_MAP(ShowHand, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

//���캯��
ShowHand::ShowHand(): CDialog(IDD_DIALOG1)
{
	//������Դ
	m_ImageBack.SetLoadInfo(IDB_SHOWHAND_COMFIRE,AfxGetInstanceHandle());
	m_bTimes = 0;
}

//��������
ShowHand::~ShowHand()
{
}

//�ؼ���
void ShowHand::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDOK, m_btOK);
}

//��ʼ������
BOOL ShowHand::OnInitDialog()
{
	__super::OnInitDialog();	

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//���ð�ť
	HINSTANCE Hinstance = AfxGetInstanceHandle();
	m_btOK.SetButtonImage(IDB_BT_YES1,Hinstance,false);
	m_btCancel.SetButtonImage(IDB_BT_NO1,Hinstance,false);

	//���д���
	CenterWindow(GetParent());

	//�ƶ�����
	CImageHandle HandleBack(&m_ImageBack);
	SetWindowPos(NULL,200,200,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//���ö�ʱ��
	m_bTimes = TIMER_START;
	SetTimer(IDI_START,TIMER_START*1000,NULL);
	SetTimer(IDI_COUNT,1000,NULL);

	return TRUE;
}

//�ػ�����
void ShowHand::OnPaint() 
{
	CPaintDC dc(this); 

	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.AlphaDrawImage(&dc,0,0,RGB(255,255,255));

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//��������
	CFont ViewFont;
	ViewFont.CreateFont(-15,0,0,0,400,0,0,0,134,3,2,1,1,TEXT("Arial"));
	CFont *pOldFont=dc.SelectObject(&ViewFont);

	TCHAR tCh[128]=TEXT("");
	_snprintf(tCh,sizeof(tCh),TEXT("%ld"),m_bTimes);

	CRect rcScore(85,10,85+65,10+19);
	dc.DrawText(tCh,lstrlen(tCh),&rcScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	//��ԭ����
	dc.SelectObject(pOldFont);
	ViewFont.DeleteObject();

	return;
}

//�滭����
BOOL ShowHand::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//ʱ����Ϣ
void ShowHand::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==IDI_START)
	{
		KillTimer(IDI_START);
		KillTimer(IDI_COUNT);
		PostMessage(WM_CLOSE);
	}
	else if(nIDEvent==IDI_COUNT)
	{
		m_bTimes--;
		Invalidate(FALSE);
		UpdateWindow();
	}

	CDialog::OnTimer(nIDEvent);
}

//�����Ϣ
void ShowHand::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

void ShowHand::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void ShowHand::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////