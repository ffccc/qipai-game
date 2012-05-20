
#include "stdafx.h"
#include "GameBank.h"

//��ʱ��I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//���½���
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//���½���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameBank, CSkinDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
	ON_BN_CLICKED(IDC_DRAWOUT, OnDrawOutSelected)
	ON_BN_CLICKED(IDC_STORAGE, OnSaveSelected)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameBank::CGameBank(): CSkinDialogEx(IDD_BACK)
{
	m_lInCount = 0;
	m_lGameGold = 0;
	m_lStorageGold = 0;
	m_pMeUserData = NULL;
	m_pIClientKernel = NULL;
}

//��������
CGameBank::~CGameBank()
{
}

//�ؼ���
void CGameBank::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Text(pDX, IDC_GAME_GOLD, m_lGameGold);
	DDX_Text(pDX, IDC_STORAGE_GOLD, m_lStorageGold);
	DDX_Control(pDX, IDC_TEXT_RED, m_textTip);
}

//��ȡ��ť
void CGameBank::OnBnClickedOk()
{
	//������֤
	if (IsButtonSelected(IDC_DRAWOUT) && (m_lInCount <= 0 || m_lInCount > m_lStorageGold))
	{
		CString strMessage;
		if (m_lStorageGold > 0)
			strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"), m_lStorageGold);
		else
			strMessage.Format(TEXT("��Ĵ洢��Ϸ����ĿΪ0,���ܽ�����ȡ����!"));
		MessageBox(strMessage, TEXT("ϵͳ��Ϣ") , MB_ICONINFORMATION) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0, -1);
		return;
	}

	//������֤
	if (IsButtonSelected(IDC_STORAGE) && (m_lInCount <= 0 || m_lInCount > m_lGameGold))
	{
		CString strMessage;
		if (m_lGameGold > 0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"), m_lGameGold);
		else strMessage.Format(TEXT("��ĵ�ǰ��Ϸ����ĿΪ0,���ܽ��д洢����!"));

		MessageBox(strMessage, TEXT("ϵͳ��Ϣ") , MB_ICONINFORMATION) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0, -1);
		return;
	}

	//������֤
	TCHAR szPassword[PASS_LEN] = TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD, szPassword, CountArray(szPassword));
	if (szPassword[0] == 0 && IsButtonSelected(IDC_DRAWOUT))
	{
		MessageBox(TEXT("���벻��Ϊ�գ��������������룡"), TEXT("ϵͳ��Ϣ") , MB_ICONINFORMATION) ;
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}
	TCHAR szTempPassword[PASS_LEN] = TEXT("");
	CopyMemory(szTempPassword, szPassword, sizeof(szTempPassword));
	CMD5Encrypt::EncryptData(szTempPassword, szPassword);

	if (IsButtonSelected(IDC_DRAWOUT))
	{
		//������Ϣ
		CMD_GF_BankGet BankGetGold;
		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
		BankGetGold.lGetValue = m_lInCount;
		BankGetGold.cbGameAction = TRUE;
		CopyMemory(BankGetGold.szPassword, szPassword, sizeof(BankGetGold.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	}
	else
	{
		//������Ϣ
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.lStorageValue = m_lInCount;
		BankStorage.cbGameAction = TRUE;
		CopyMemory(BankStorage.szPassword, szPassword, sizeof(BankStorage.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	return;
}

//��ʼ������
BOOL CGameBank::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();
	::SetWindowPos(this->GetSafeHwnd(), CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE);
	UpdateData(FALSE);

	SetWindowText(TEXT("����"));

	//������ֵ
	SetButtonSelected(IDC_STORAGE, true);
	m_textTip.SetBackGroundColor(RGB(255, 235, 229));
	m_textTip.EnableWindow(FALSE);
	m_textTip.SetUnderline(false);
	//m_textTip.SetTClr(RGB(255, 0, 0));

	//��ʱ����
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW, TIME_CHARMVALUE_UPDATE_VIEW, NULL);

	return TRUE;
}

//���½���
void CGameBank::UpdateView()
{
	UpdateData(TRUE);

	if (m_pMeUserData == NULL)return;

	//������Ϣ
	m_lGameGold = ((m_pMeUserData->lGameGold < 0) ? 0 : m_pMeUserData->lGameGold);
	m_lStorageGold = m_pMeUserData->lInsureScore;

	UpdateData(FALSE);
}

//�����Ϣ
void CGameBank::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnLButtonDown(nFlags, point);
}

//�����Ϣ
void CGameBank::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnMouseMove(nFlags, point);
}

//������Ϣ
BOOL CGameBank::PreTranslateMessage(MSG* pMsg)
{
	//������ĸ
	if (GetFocus() == GetDlgItem(IDC_IN_COUNT) && pMsg->message == WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue - '0';
		if ((bTemp < 0 || bTemp > 9) && bMesValue != VK_BACK) return TRUE;
	}

	////���˻س�
	//if(pMsg->message==WM_KEYDOWN)
	//{
	//	BYTE bMesValue = (BYTE)(pMsg->wParam);
	//	if(bMesValue==VK_RETURN) return TRUE;
	//}

	return __super::PreTranslateMessage(pMsg);
}

//ʱ����Ϣ
void CGameBank::OnTimer(UINT nIDEvent)
{
	//ʱ����Ϣ
	switch (nIDEvent)
	{
		case IDI_CHARMVALUE_UPDATE_VIEW:		//���½���
		{
			//���½���
			UpdateView();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//������Ϣ
void CGameBank::OnEnChangeInCount()
{
	CString strCount;
	GetDlgItem(IDC_IN_COUNT)->GetWindowText(strCount);
	m_lInCount = _tstol(strCount);

	return ;
}

//�Ƿ�ѡ��
bool CGameBank::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck() == BST_CHECKED;
}

//ѡ�а�ť
void CGameBank::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton = (CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//������Ϣ
void CGameBank::SetSendInfo(IClientKernel *pClientKernel, const tagUserData *pMeUserData)
{
	ASSERT(pClientKernel == NULL);

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pMeUserData = pMeUserData;

	return;
}

//������Ϣ
void CGameBank::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIClientKernel == NULL);
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	return ;
}
//////////////////////////////////////////////////////////////////////////
void CGameBank::OnDrawOutSelected()
{
	GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(TRUE);
}
void CGameBank::OnSaveSelected()
{
	GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(FALSE);
}