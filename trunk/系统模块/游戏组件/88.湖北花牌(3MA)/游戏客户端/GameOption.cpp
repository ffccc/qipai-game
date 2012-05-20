#include "Stdafx.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameOption, CSkinDialogEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameOption::CGameOption() : CSkinDialogEx(IDD_OPTION)
{
	m_bEnableSound=true;
	m_bPlayNativeSound=1;

	return;
}

//��������
CGameOption::~CGameOption()
{
}

//�ؼ���
void CGameOption::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDOK, m_btOK);
	//DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CGameOption::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ϸ����"));

	//���ÿؼ�
	if (m_bEnableSound==true) ((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->SetCheck(BST_CHECKED);
	if(m_bPlayNativeSound==1)	((CButton *)GetDlgItem(IDC_PLAY_NATIVE_SOUND))->SetCheck(BST_CHECKED);

	return TRUE;
}

//ȷ����Ϣ
void CGameOption::OnOK()
{
	//��ȡ����
	m_bEnableSound=(((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->GetCheck()==BST_CHECKED);
//�����û����õ�������Ϣ
	//�ط���
	if((((CButton *)GetDlgItem(IDC_PLAY_NATIVE_SOUND))->GetCheck()==BST_CHECKED))
	{
		m_bPlayNativeSound=1;
		CWinApp * pApp=AfxGetApp();
		pApp->WriteProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),1);
	}
	else	//��ͨ��
	{
		m_bPlayNativeSound=0;
		CWinApp * pApp=AfxGetApp();
		pApp->WriteProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),0);
	}
	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
