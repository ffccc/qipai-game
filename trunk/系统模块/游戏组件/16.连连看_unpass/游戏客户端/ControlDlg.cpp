// ControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlDlg.h"
#include ".\controldlg.h"


// CControlDlg �Ի���

IMPLEMENT_DYNAMIC(CControlDlg, CDialog)
CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlDlg::IDD, pParent)
{
	m_ImageBg.SetLoadInfo(IDB_BK_RIGHT,AfxGetInstanceHandle());
}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CControlDlg ��Ϣ�������

int CControlDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CImageHandle ImageHandle(&m_ImageBg);
	SetWindowPos(NULL,0,0,m_ImageBg.GetWidth(),m_ImageBg.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

void CControlDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CImageHandle ImageHandle(&m_ImageBg);
	m_ImageBg.BitBlt(dc.m_hDC,0,0,m_ImageBg.GetWidth(),m_ImageBg.GetHeight()-28,0,28,SRCCOPY);
}
