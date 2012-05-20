#include "Stdafx.h"
#include "Resource.h"
#include "DlgUserRequest.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define IDI_TIME					10									//��ʱ�� ID

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgUserRequest, CSkinDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AGREE, OnBnClickedApprove)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgUserRequest::CDlgUserRequest() : CSkinDialogEx(IDD_USER_REQ)
{
	//���ñ���
	m_nElapse=15;
	m_uMessage=0;
	m_nResidualTime=0;

	return;
}

//��������
CDlgUserRequest::~CDlgUserRequest()
{
}

//�ؼ���
void CDlgUserRequest::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AGREE, m_btApprove);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CDlgUserRequest::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ�
	SetWindowText(TEXT("�û�����"));
	SetDlgItemText(IDC_REQUEST_TEXT,m_strMessage);
	
	//���ö�ʱ��
	m_nResidualTime=m_nElapse;
	SetTimer(IDI_TIME,1000,NULL);

	return TRUE;
}

//ȷ����Ϣ
void CDlgUserRequest::OnOK()
{
	return;
}

//ȡ����Ϣ
void CDlgUserRequest::OnCancel()
{
	AfxGetMainWnd()->PostMessage(m_uMessage,0,0);
	DestroyWindow();

	return;
}

//��������
void CDlgUserRequest::InitRequest(UINT uMessage, UINT nElapse, LPCTSTR pszMessage)
{
	//���ñ���
	m_nElapse=nElapse;
	m_uMessage=uMessage;
	m_strMessage=pszMessage;

	return;
}

//ͬ�ⰴť
void CDlgUserRequest::OnBnClickedApprove()
{
	AfxGetMainWnd()->PostMessage(m_uMessage,TRUE,TRUE);
	DestroyWindow();

	return;
}

//��ʱ����Ϣ
void CDlgUserRequest::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_TIME)
	{
		//���ñ���
		if (m_nResidualTime>0)
		{
			TCHAR szTitle[64]=TEXT("");
			_snprintf(szTitle,sizeof(szTitle),TEXT("�û����� -- %ld ��"),m_nResidualTime);
			SetWindowText(szTitle);
		}
		else 
		{
			KillTimer(IDI_TIME);
			PostMessage(WM_CLOSE,0,0);
		}

		//���ñ���
		m_nResidualTime--;

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
