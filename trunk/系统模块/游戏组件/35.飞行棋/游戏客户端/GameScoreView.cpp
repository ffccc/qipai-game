#include "Stdafx.h"
#include "GameScoreView.h"
#include "D3DWnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreView, CSkinDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameScoreView::CGameScoreView() : CSkinDialogEx(IDD_SCORE_VIEW)
{

	return;
}

//析构函数
CGameScoreView::~CGameScoreView()
{
}

//控件绑定
void CGameScoreView::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);

}

//初始化函数
BOOL CGameScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("玩家得分"));


	CRect   rect;   
    GetWindowRect(&rect);   
	MoveWindow(270,300,rect.Width(),rect.Height());

	return TRUE;
}

//确定消息
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
	wsprintf(temp, "玩家姓名：%s，玩家得分：%ld", strName, lScore);
	GetDlgItem(IDC_STATIC1+bTextIndex)->SetWindowText(temp);
}

void CGameScoreView::SetWinnerText(const TCHAR *strName)
{
	TCHAR temp[512];
	wsprintf(temp, "获胜玩家：%s", strName);
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
	GetDlgItem(IDOK)->SetWindowText("确定");

}
//////////////////////////////////////////////////////////////////////////
