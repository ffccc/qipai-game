#include "Stdafx.h"
#include "ChessSelect.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChessSelect, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_ROOK, OnRook)
	ON_BN_CLICKED(IDC_HORSE, OnHorse)
	ON_BN_CLICKED(IDC_QUEEN, OnQueen)
	ON_BN_CLICKED(IDC_ELEPHANT, OnElephant)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CChessSelect::CChessSelect() : CDialog(IDD_CHESS_SELECT)
{
	//������Դ
	m_ImageBack.SetLoadInfo(IDB_CHESS_SELECT,AfxGetInstanceHandle());

	return;
}

//��������
CChessSelect::~CChessSelect()
{
}

//�ؼ���
void CChessSelect::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROOK, m_btRook);
	DDX_Control(pDX, IDC_HORSE, m_btHorse);
	DDX_Control(pDX, IDC_QUEEN, m_btQueen);
	DDX_Control(pDX, IDC_ELEPHANT, m_btElephant);
}

//��ʼ������
BOOL CChessSelect::OnInitDialog()
{
	__super::OnInitDialog();

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btRook.SetButtonImage(IDB_CHESS_ROOK,hInstance,false);
	m_btQueen.SetButtonImage(IDB_CHESS_QUEEN,hInstance,false);
	m_btHorse.SetButtonImage(IDB_CHESS_HORSE,hInstance,false);
	m_btElephant.SetButtonImage(IDB_CHESS_ELEPHANT,hInstance,false);

	//����λ��
	CImageHandle ImageBack(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//��ȡλ��
	CRect rcButton;
	m_btRook.GetWindowRect(&rcButton);
	int nXPos=(m_ImageBack.GetWidth()-rcButton.Width()*4-15)/2;

	//�ƶ���ť
	HDWP hDwp=BeginDeferWindowPos(4);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btQueen,NULL,nXPos,33,0,0,uFlags);
	DeferWindowPos(hDwp,m_btRook,NULL,nXPos+rcButton.Width()+5,33,0,0,uFlags);
	DeferWindowPos(hDwp,m_btHorse,NULL,nXPos+rcButton.Width()*2+10,33,0,0,uFlags);
	DeferWindowPos(hDwp,m_btElephant,NULL,nXPos+rcButton.Width()*3+15,33,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//���д���
	CenterWindow(GetParent());

	return TRUE;
}

//�ػ�����
void CChessSelect::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CImageHandle ImageBack(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	return;
}

//�滭����
BOOL CChessSelect::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//�����Ϣ
void CChessSelect::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�ⰴ����
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//���Ӱ�ť
void CChessSelect::OnRook()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(WM_SELECT_CHESS,CHESS_ROOK,CHESS_ROOK);

	//���ش���
	ShowWindow(SW_HIDE);

	return;
}

//���Ӱ�ť
void CChessSelect::OnHorse()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(WM_SELECT_CHESS,CHESS_HORSE,CHESS_HORSE);

	//���ش���
	ShowWindow(SW_HIDE);

	return;
}

//���Ӱ�ť
void CChessSelect::OnQueen()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(WM_SELECT_CHESS,CHESS_QUEEN,CHESS_QUEEN);

	//���ش���
	ShowWindow(SW_HIDE);

	return;
}

//���Ӱ�ť
void CChessSelect::OnElephant()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(WM_SELECT_CHESS,CHESS_ELEPHANT,CHESS_ELEPHANT);

	//���ش���
	ShowWindow(SW_HIDE);

	return;
}

//////////////////////////////////////////////////////////////////////////
