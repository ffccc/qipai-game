#include "Stdafx.h"
#include "GameOption.h"
#include "D3DWnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameOption, CSkinDialogEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameOption::CGameOption() : CSkinDialogEx(IDD_OPTION)
{
	m_bEnableSound=true;
	m_bEnableMusic = true;
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
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CGameOption::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ϸ����"));

	//���ÿؼ�
	if (m_bEnableSound==true) ((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->SetCheck(BST_CHECKED);
	if (m_bEnableMusic==true) ((CButton *)GetDlgItem(IDC_ENABLE_MUSIC))->SetCheck(BST_CHECKED);

	return TRUE;
}

//ȷ����Ϣ
void CGameOption::OnOK()
{
	//��ȡ����
	m_bEnableSound=(((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->GetCheck()==BST_CHECKED);
	if(((CButton *)GetDlgItem(IDC_ENABLE_MUSIC))->GetCheck()==BST_UNCHECKED){
		AfxGetMainWnd()->SendMessage(IDM_D3D_ALLMUSIC,IDM_MUSIC_STP,0);
		m_bEnableMusic = false;
	} else {
		AfxGetMainWnd()->SendMessage(IDM_D3D_ALLMUSIC,IDM_MUSIC_BG,1);
		m_bEnableMusic = true;
	}
	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
