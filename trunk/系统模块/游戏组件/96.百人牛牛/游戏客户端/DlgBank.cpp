// DlgBank.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBank.h"
#include "resource.h"
#include ".\dlgbank.h"
#include "..\��Ϸ������\GameLogic.h"

//��ʱ��I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//���½���
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//���½���


void MakeString(CString &strNum,LONGLONG lNumber)
{
	CString strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	for (int i = 0; i < int((nLength-1)/3); i++)
		strTempNum.Insert(nLength - 3*(i+1), _T(","));
	strNum.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
}

void MakeStringToNum(CString str,LONGLONG &Num)
{
	CString strNumber = str;
	strNumber.Remove(',');
	strNumber.Trim();
	_snscanf(strNumber.GetBuffer(),strNumber.GetLength(),_TEXT("%I64d"),&Num);

	return;
}

IMPLEMENT_DYNAMIC(CDlgBank, CSkinDialogEx)
CDlgBank::CDlgBank(CWnd* pParent /*=NULL*/)
	: CSkinDialogEx(IDD_BANK_STORAGE, pParent)
{
	m_lInCount=0;
	m_lGameGold=0;
	m_bCanDraw=true;
//	m_bDrawAllGold=false;
//	m_bStorageAllGold=false;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_bBankStorage=true;
}

CDlgBank::~CDlgBank()
{
}

void CDlgBank::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_USER_PASSWORD, m_Edit);
	DDX_Control(pDX, IDC_STATIC_PASS, m_Static);
}


BEGIN_MESSAGE_MAP(CDlgBank, CSkinDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
END_MESSAGE_MAP()


void CDlgBank::OnEnRadioStore()
{
	SetBankerActionType(true);
}

void CDlgBank::OnEnRadioGet()
{

	SetBankerActionType(false);
}

void CDlgBank::AllowStorage(bool bAllow)
{
	m_blCanStore=bAllow;
	MyDebug(_T("Bank Set :%d"), m_blCanStore);
	ShowItem();
}

void CDlgBank::AllowDraw(bool bAllow)
{
    m_bCanDraw=bAllow;
	ShowItem();
}

//��ȡ��ť
void CDlgBank::OnBnClickedOk()
{

	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	MakeStringToNum(strinCount,m_lInCount);

	if(((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck()==1)
	{
		if(!m_bBankStorage)
			m_lInCount=m_pMeUserData->lInsureScore;
		else
			m_lInCount=m_pMeUserData->lGameGold;
	}
	
	//������֤
	if (false==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%I64d֮��"), m_lStorageGold);
		else strMessage.Format(TEXT("��Ĵ洢��Ϸ����ĿΪ0,���ܽ�����ȡ����!"));
		MessageBox(strMessage,TEXT("��ܰ��ʾ"));

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		SetDlgItemText(IDC_USER_PASSWORD,"");
		return;
	}


	//������֤
	if (true==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%I64d֮��"),m_lGameGold );
		else strMessage.Format(TEXT("��ĵ�ǰ��Ϸ����ĿΪ0,���ܽ��д洢����!"));
		MessageBox(strMessage,TEXT("��ܰ��ʾ"));

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		SetDlgItemText(IDC_USER_PASSWORD,"");
		return;
	}
	//������֤
	TCHAR szPassword[PASS_LEN]=TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	
	if (szPassword[0]==0&&m_blUsingPassWord)
	{
		MessageBox(TEXT("���벻��Ϊ�գ��������������룡"),TEXT("��ܰ��ʾ"));
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}
	
	if(m_blUsingPassWord)
	{
		TCHAR szTempPassword[PASS_LEN]=TEXT("");
		CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
		CMD5Encrypt::EncryptData(szTempPassword,szPassword);
	}

	if (false==m_bBankStorage)
	{
		//������Ϣ
		CMD_GF_BankGet BankGetGold;
		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
		BankGetGold.lGetValue = (LONGLONG)m_lInCount;
		BankGetGold.cbGameAction=TRUE;
		CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));
		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	}
	else
	{
		//״̬�ж�
		if (US_PLAY==m_pMeUserData->cbUserStatus)
		{
			MessageBox(TEXT("�������Ϸ���ٴ�"),TEXT("��ܰ��ʾ"));
			return;	
		}
		//������Ϣ
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.cbGameAction=TRUE;
		BankStorage.lStorageValue = (LONGLONG)m_lInCount;
		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));
		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	SetDlgItemText(IDC_USER_PASSWORD,"");
	return;
}

//��ʼ������
BOOL CDlgBank::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();
	UpdateData(FALSE);
	SetWindowText(TEXT("����"));
	SetBankerActionType(true);
	//��ʱ����
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);
	return TRUE;
}

//���½���
void CDlgBank::UpdateView()
{
	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	LONGLONG OutNum = 0;
	MakeStringToNum(strinCount,OutNum);

	if(m_OrInCount!= OutNum)
	{
		m_OrInCount = OutNum;
		if(OutNum!=0)
		{
			MakeString(strinCount,OutNum);
			SetDlgItemText(IDC_IN_COUNT,strinCount);
			((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
		}
		//������֤
		if ((OutNum < 0 || OutNum > m_lStorageGold ))
			return;	
	}
	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//������Ϣ
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	CString strlGameGold;

	MakeString(strlGameGold,m_lGameGold);
	SetDlgItemText(IDC_GAME_GOLD,strlGameGold);

	CString strlStorageGold;
	MakeString(strlStorageGold,m_lStorageGold);
	SetDlgItemText(IDC_STORAGE_GOLD,strlStorageGold);

	if(((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck()==1)
	{
		if(!m_bBankStorage)
			SetDlgItemText(IDC_IN_COUNT,strlStorageGold);
		else
			SetDlgItemText(IDC_IN_COUNT,strlGameGold);
	}

	UpdateData(FALSE);
}

//�����Ϣ
void CDlgBank::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnLButtonDown(nFlags, point);
}

//�����Ϣ
void CDlgBank::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnMouseMove(nFlags, point);
}

//������Ϣ
BOOL CDlgBank::PreTranslateMessage(MSG* pMsg)
{
	//������ĸ
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

//ʱ����Ϣ
void CDlgBank::OnTimer(UINT nIDEvent)
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
void CDlgBank::OnEnChangeInCount()
{
	CString strInCount;
	CString strStorageGold;
	CString strGameGold;
	GetDlgItem(IDC_IN_COUNT)->GetWindowText(strInCount);
	MakeStringToNum(strInCount, m_lInCount);
	MakeString(strStorageGold, m_lStorageGold);
	MakeString(strGameGold, m_lGameGold);

	//������֤
	if (false==m_bBankStorage && (m_lInCount < 0 || m_lInCount > m_lStorageGold || strInCount.GetLength() > strStorageGold.GetLength()) )
	{
		m_lInCount = m_lStorageGold;
		MakeString(strInCount,m_lInCount);
		GetDlgItem(IDC_IN_COUNT)->SetWindowText(strInCount);
	}

	//������֤
	if (true==m_bBankStorage && (m_lInCount < 0 || m_lInCount > m_lGameGold || strInCount.GetLength() > strGameGold.GetLength()) )
	{
		m_lInCount = m_lGameGold;
		MakeString(strInCount,m_lInCount);
		GetDlgItem(IDC_IN_COUNT)->SetWindowText(strInCount);
	}

	return ;
}

//�Ƿ�ѡ��
bool CDlgBank::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

int CDlgBank::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	return 0;
}

//ѡ�а�ť
void CDlgBank::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//������Ϣ
void CDlgBank::SetSendInfo(IClientKernel *pClientKernel,tagUserData const*pMeUserData)
{
	ASSERT(pClientKernel!=NULL);

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pMeUserData=const_cast<tagUserData *>(pMeUserData);

	return;
}
void CDlgBank::SetPostPoint(CPoint Point)
{
	CRect Rect;
	GetWindowRect(&Rect);
	SetWindowPos(NULL,Point.x,Point.y-Rect.bottom+Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,SWP_NOZORDER);
}
//������Ϣ
void CDlgBank::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIClientKernel!=NULL);

	//������Ϣ
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

void CDlgBank::ShowItem()
{
	
	if(m_bBankStorage)
	{
		GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PASSWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_HIDE);
	    GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(false);
		m_btOK.EnableWindow(m_blCanStore);
		MyDebug(_T("Bank Show :%d"), m_blCanStore);
	}

	else
	{
		GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PASSWORD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(TRUE);
		m_btOK.EnableWindow(m_bCanDraw);
	}

}
//��������
void CDlgBank::SetBankerActionType(bool bStorage) 
{
	//���ñ���
	m_bBankStorage=bStorage;
	//���ñ���
	if (m_bBankStorage)
	{
//		m_bDrawAllGold=((CButton*)(GetDlgItem(IDC_CHECK_ALL)))->GetCheck()==1?true:false;
	//	((CButton*)(GetDlgItem(IDC_CHECK_ALL)))->SetCheck(m_bStorageAllGold?1:0);
		SetDlgItemText(IDC_IN_COUNT,"");
		SetDlgItemText(IDC_USER_PASSWORD,"");
		GetDlgItem(IDOK)->SetWindowText(TEXT("���"));
		GetDlgItem(IDC_CHECK_ALL)->SetWindowText("�������н��");
		m_blUsingPassWord = false;
	}
	else
	{
	//	m_bStorageAllGold=((CButton*)(GetDlgItem(IDC_CHECK_ALL)))->GetCheck()==1?true:false;
	//	((CButton*)(GetDlgItem(IDC_CHECK_ALL)))->SetCheck(m_bDrawAllGold?1:0);
		SetDlgItemText(IDC_IN_COUNT,"");
		SetDlgItemText(IDC_USER_PASSWORD,"");
		GetDlgItem(IDOK)->SetWindowText(TEXT("ȡ��"));
		GetDlgItem(IDC_CHECK_ALL)->SetWindowText("ȡ�����н��");
		m_blUsingPassWord = true;
	}
	ShowItem();
}

void CDlgBank::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	((CButton*)GetDlgItem(IDC_CHECK_ALL))->SetCheck(0);
	CSkinDialogEx::OnCancel();
}
