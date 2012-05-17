
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
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameBank::CGameBank():CSkinDialogEx(IDD_BACK)
{
	m_lInCount=0;
	m_lGameGold=0;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_lMeJetton=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_lApplyBankerCondition=0;
	m_bMeApplyBanker=false;
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
}

//��ȡ��ť
void CGameBank::OnBnClickedOk()
{
	//������֤
	if (IsButtonSelected(IDC_DRAWOUT) && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"), m_lStorageGold);
		else strMessage.Format(TEXT("������н����ĿΪ0,���ܽ�����ȡ����!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("��ܰ��ʾ")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//������֤
	if (IsButtonSelected(IDC_STORAGE) && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"),m_lGameGold );
		else strMessage.Format(TEXT("��ĵ�ǰ��ұ���ĿΪ0,���ܽ��д洢����!"));
		
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("��ܰ��ʾ")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}
	
	//������֤
	if (IsButtonSelected(IDC_STORAGE) && ( m_lInCount > (m_lGameGold-m_lMeJetton)))
	{
		CString strMessage;
		if ((m_lGameGold-m_lMeJetton)==0)
		{
			strMessage.Format(TEXT("���������Ϸ��ֵ������0��%ld֮��"),0 );
		}
		else
		{
			strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"),m_lGameGold-m_lMeJetton );
		}
		
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("��ܰ��ʾ")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//������֤
	if (IsButtonSelected(IDC_STORAGE) && ((m_pMeUserData->wChairID==m_wBankerUser))
		&&( m_lInCount > (m_lGameGold-m_lApplyBankerCondition)))
	{
		CString strMessage;
		if ((m_lGameGold-m_lApplyBankerCondition)==0)
		{
			strMessage.Format(TEXT("���������Ϸ��ֵ������0��%ld֮��"),0 );
		}
		else
		{
			strMessage.Format(TEXT("���������Ϸ��ֵ������1��%ld֮��"),m_lGameGold-m_lApplyBankerCondition );
		}

		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("��ܰ��ʾ")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}
	
	//������֤
	if (m_pMeUserData->cbUserStatus==US_PLAY)
	{
		CString strMessage;

		strMessage.Format(TEXT("������Ϸ����ʱ���ڣ����в���!"));

		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("��ܰ��ʾ")) ;
		return;
	}

	//������֤
	TCHAR szPassword[PASS_LEN]=TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	if (szPassword[0]==0)
	{
		ShowInformationEx(TEXT("���벻��Ϊ�գ��������������룡"),0 ,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ")) ;
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}


	TCHAR szTempPassword[PASS_LEN]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CMD5Encrypt::EncryptData(szTempPassword,szPassword);

	if (IsButtonSelected(IDC_DRAWOUT))
	{
		//������Ϣ
		CMD_GF_BankGet BankGetGold;
		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
		BankGetGold.lGetValue = m_lInCount;
		BankGetGold.cbIsGameAction = TRUE;
		CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	}
	else
	{
		//������Ϣ
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.lStorageValue = m_lInCount;
		BankStorage.cbIsGameAction = TRUE;
		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));

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
	SetButtonSelected(IDC_STORAGE,true);

	//��ʱ����
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	return TRUE;
}

//���½���
void CGameBank::UpdateView()
{
	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//������Ϣ
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

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
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
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
	switch(nIDEvent)
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
	m_lInCount=_tstol(strCount);

	return ;
}

//�Ƿ�ѡ��
bool CGameBank::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//ѡ�а�ť
void CGameBank::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//������Ϣ
void CGameBank::SetSendInfo(IClientKernel *pClientKernel,const tagUserData *pMeUserData,LONG lMeJetton, WORD wBankerUser, LONG lApplyBankerCondition, bool bMeApplyBanker)
{
	ASSERT(pClientKernel==NULL);

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pMeUserData = pMeUserData;
	m_lMeJetton = lMeJetton;
	m_wBankerUser =  wBankerUser;
	m_lApplyBankerCondition = lApplyBankerCondition;
	m_bMeApplyBanker = bMeApplyBanker;
	return;
}

//������Ϣ
void CGameBank::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIClientKernel==NULL); 
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	return ;
}
//////////////////////////////////////////////////////////////////////////
