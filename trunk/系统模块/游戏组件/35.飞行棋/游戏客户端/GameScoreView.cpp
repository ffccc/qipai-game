#include "Stdafx.h"
#include "GameScoreView.h"
#include "D3DWnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreView, CSkinDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScoreView::CGameScoreView() : CSkinDialogEx(IDD_SCORE_VIEW)
{

	return;
}

//��������
CGameScoreView::~CGameScoreView()
{
}

//�ؼ���
void CGameScoreView::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);

}

//��ʼ������
BOOL CGameScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��ҵ÷�"));


	CRect   rect;   
    GetWindowRect(&rect);   
	MoveWindow(270,300,rect.Width(),rect.Height());

	return TRUE;
}

//ȷ����Ϣ
void CGameScoreView::OnOK()
{
	for(BYTE i = 0; i < GAME_PLAYER +1; i++){
		GetDlgItem(IDC_STATIC1+i)->SetWindowText("");
	}
	__super::OnOK();
}

void CGameScoreView::SetScoreText(BYTE bTextIndex, const TCHAR *strName, long lScore)
{
	TCHAR temp[512];
	wsprintf(temp, "���������%s����ҵ÷֣�%ld", strName, lScore);
	GetDlgItem(IDC_STATIC1+bTextIndex)->SetWindowText(temp);
}

void CGameScoreView::SetWinnerText(const TCHAR *strName)
{
	TCHAR temp[512];
	wsprintf(temp, "��ʤ��ң�%s", strName);
	GetDlgItem(IDC_STATICWIN)->SetWindowText(temp);
}



void CGameScoreView::OnPaint()
{
	//CPaintDC dc(this);

	__super::OnPaint();
	TCHAR temp[512];
	for(BYTE i =0; i< GAME_PLAYER +1; i++){
		GetDlgItem(IDC_STATIC1+i)->GetWindowText(temp, 512);
		GetDlgItem(IDC_STATIC1+i)->SetWindowText(temp);
	}
	GetDlgItem(IDOK)->SetWindowText("ȷ��");

}
//////////////////////////////////////////////////////////////////////////
