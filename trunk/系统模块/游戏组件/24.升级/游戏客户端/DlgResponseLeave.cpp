// DlgResponseLeave.cpp : implementation file
//

#include "stdafx.h"
#include "DlgResponseLeave.h"

//////////////////////////////////////////////////////////////////////////

#define			IDI_DESTROY_DLG						500			//�رնԻ���ʱ��

#define			TIME_CLOSE_DLG						15			//�رնԻ���ʱ��

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgResponseLeave, CSkinDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////


//���캯��
CDlgResponseLeave::CDlgResponseLeave(CWnd * pParentWnd):CSkinDialogEx(IDD_REQUEST_LEAVE,pParentWnd)
{
	m_wRequestUser = INVALID_CHAIR;
	m_pGameFrameDlg = NULL;
	m_wTimer = 0;
	m_strCaption = TEXT("��������뿪");
}

//��������
CDlgResponseLeave::~CDlgResponseLeave()
{
}

//�ؼ���
void CDlgResponseLeave::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CDlgResponseLeave::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(m_strCaption);


	return FALSE;
}

//ȷ������
VOID CDlgResponseLeave::OnOK()
{
	//������Ϣ
	m_pGameFrameDlg->PostMessage(IDM_RESPONSE_LEAVE,(WPARAM)this,(LPARAM)TRUE);
	//�رմ���
	DestroyWindow();
	return;
}

//ȡ����Ϣ
VOID CDlgResponseLeave::OnCancel()
{
	//������Ϣ
	m_pGameFrameDlg->PostMessage(IDM_RESPONSE_LEAVE,(WPARAM)this,FALSE);
	//�رմ���
	DestroyWindow();
	return;
}

//��ʾ��Ϣ
void CDlgResponseLeave::ShowRequestMessage(WORD wChairId,LPCTSTR pszCaption,LPCTSTR pszMessage)
{
	//Ч��
	ASSERT( m_pGameFrameDlg != NULL && m_wRequestUser == INVALID_CHAIR );
	//���ñ���
	m_wRequestUser = wChairId;
	//��������
	((CStatic *)GetDlgItem(IDC_TEXT_CAPTION))->SetWindowText(pszCaption);
	((CEdit *)GetDlgItem(IDC_EDIT_REQUEST))->SetWindowText(pszMessage);
	//����ʱ��
	m_wTimer = TIME_CLOSE_DLG;
	SetTimer(IDI_DESTROY_DLG,1000,NULL);
	//��ʾ
	ShowWindow(SW_SHOW);
	return ;
}

//////////////////////////////////////////////////////////////////////////

void CDlgResponseLeave::OnTimer(UINT nIDEvent)
{
	switch( nIDEvent )
	{
	case IDI_DESTROY_DLG:
		{
			if( --m_wTimer > 0 )
			{
				TCHAR szBuffer[32];
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s(%d)"),m_strCaption,m_wTimer);
				SetWindowText(szBuffer);
				SetTimer(IDI_DESTROY_DLG,1000,NULL);
				return;
			}
			//������Ϣ
			m_pGameFrameDlg->PostMessage(IDM_RESPONSE_LEAVE,(WPARAM)this,(LPARAM)FALSE);
			//�رնԻ���
			PostMessage(WM_CLOSE);
			return ;
		}
	}
	__super::OnTimer(nIDEvent);
}
