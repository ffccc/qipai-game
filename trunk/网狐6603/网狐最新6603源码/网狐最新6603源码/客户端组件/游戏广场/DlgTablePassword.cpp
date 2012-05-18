#include "Stdafx.h"
#include "DlgTablePassword.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgTablePassword, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgTablePassword::CDlgTablePassword() : CSkinDialog(IDD_DLG_TABLE_PASSWORD)
{
	//���ñ���
	ZeroMemory(m_szString,sizeof(m_szString));
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//��������
CDlgTablePassword::~CDlgTablePassword()
{
}

//�ؼ���
VOID CDlgTablePassword::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//��ť�ؼ�
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);

	//�����ؼ�
	DDX_Control(pDX, IDC_STRING, m_stString);
	DDX_Control(pDX, IDC_PASSWORD, m_edPassword);
}

//���ú���
BOOL CDlgTablePassword::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��������"));

	//������ʾ
	m_stString.SetWindowText(m_szString);

	//��������
	m_edPassword.LimitText(LEN_PASSWORD-1);

	return FALSE;
}

//ȷ������
VOID CDlgTablePassword::OnOK()
{
	//��ȡ����
	m_edPassword.GetWindowText(m_szPassword,CountArray(m_szPassword));

	//�����ж�
	if (m_szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��Ϸ�����벻��Ϊ�գ����������룡"),MB_ICONINFORMATION);

		//���ý���
		m_edPassword.SetFocus();

		return;
	}

	__super::OnOK();
}

//�滭��Ϣ
VOID CDlgTablePassword::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//������Դ
	CPngImage ImageTablePasswd;
	ImageTablePasswd.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_PASSWORD"));

	//�滭ͼ��
	ImageTablePasswd.DrawImage(pDC,25,70);

	return;
}

//������ʾ
VOID CDlgTablePassword::SetPromptString(LPCTSTR pszString)
{
	//���ÿؼ�
	if (m_stString.m_hWnd!=NULL)
	{
		m_stString.SetWindowText(pszString);
	}

	//������ʾ
	lstrcpyn(m_szString,pszString,CountArray(m_szString));

	return;
}

//////////////////////////////////////////////////////////////////////////
