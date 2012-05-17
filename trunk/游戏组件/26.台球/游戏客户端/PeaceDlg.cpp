//
//PeaceDlg.cpp : ʵ���ļ�
//


#include "stdafx.h"
#include "PeaceDlg.h"
#include "Define.h"
#include ".\peacedlg.h"


IMPLEMENT_DYNAMIC(CPeaceDlg, CDialog)
CPeaceDlg::CPeaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPeaceDlg::IDD, pParent)
{
}

CPeaceDlg::~CPeaceDlg()
{
}

void CPeaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPeaceDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK,		OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL	CPeaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  
}

void	CPeaceDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void	CPeaceDlg::OnBnClickedOk()
{
	//
	//�򸸴��ڷ�����Ϣ
	//
	CWnd *pWnd = this->GetParent();
	if(!pWnd)
	{
		return ;
	}

	::SendMessage(pWnd->m_hWnd,
					UM_AGREEPEACE,
					1,
					0);

	this->OnOK();
}



//-----------------------------------------------
//					the end
//-----------------------------------------------

void CPeaceDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd *pWnd = this->GetParent();
	if(!pWnd)
	{
		return ;
	}

	::SendMessage(pWnd->m_hWnd,
					UM_AGREEPEACE,
					0,
					0);
	OnCancel();
}
