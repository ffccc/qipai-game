#include "Stdafx.h"
#include "Resource.h"
#include "DlgRequestLeave.h"
#include ".\dlgrequestleave.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRequestLeave, CSkinDialogEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRequestLeave::CDlgRequestLeave() : CSkinDialogEx(IDD_REQUEST_LEAVE)
{
	//���ñ���
	m_pGameFrameDlg=NULL;

	return;
}

//��������
CDlgRequestLeave::~CDlgRequestLeave()
{
}

//�ؼ���
void CDlgRequestLeave::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CDlgRequestLeave::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�����뿪��"));

	//��������
	((CEdit *)GetDlgItem(IDC_LEAVE_REASON))->LimitText(127);

	return FALSE;
}

//ȷ������
VOID CDlgRequestLeave::OnOK()
{
	//��ȡ��Ϣ
	TCHAR szLeaveReason[128]=TEXT("");
	GetDlgItemText(IDC_LEAVE_REASON,szLeaveReason,CountArray(szLeaveReason));

	//������Ϣ
	if (szLeaveReason[0]==0) lstrcpyn(szLeaveReason,TEXT("���û��Ƚ�����û����д�κ��뿪����"),CountArray(szLeaveReason));

	//������Ϣ
	ASSERT(m_pGameFrameDlg!=NULL);
	m_pGameFrameDlg->PostMessage(IDM_REQUEST_LEAVE,(WPARAM)szLeaveReason,0);

	//�رմ���
	DestroyWindow();

	return;
}

//ȡ����Ϣ
VOID CDlgRequestLeave::OnCancel()
{
	//�رմ���
	DestroyWindow();

	return;
}

//////////////////////////////////////////////////////////////////////////
