// DlgBank.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBank.h"
#include "resource.h"

//��ʱ��I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//���½���
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//���½���


void MakeString(CString &strNum,LONGLONG lNumber)
{
	CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	bool blfirst = true;
	bool bLongNum = false;
	if(lNumber>=1000)
	{
		bLongNum = true;
	}
	while (lNumber>0)
	{

		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==3)
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;

			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);

			}

			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(bLongNum)
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	strNum = strNumber ;


}

void MakeStringToNum(CString str,LONGLONG &Num)
{

	CString str1;
	int Pos = 0;
	int len = 0;
	int Start = 0;
	while (1)
	{
		len = str.GetLength();
		Pos = str.Find(",",Pos);
		if(Pos==-1)
		{
			str1+=str;
			break;
		}else
		{

			char Tmp[126];
			memset(Tmp,0,126);
			memcpy(Tmp,&str.GetBuffer()[Start],Pos);
			if(Pos+1<=str.GetLength())
			{
				str = &str.GetBuffer()[Pos+1];
				str1+=Tmp;
				Start =  0;
				Pos = 0;

			}else
			{
				break;
			}


		}
	}

	_snscanf(str1.GetBuffer(),str1.GetLength(),_TEXT("%I64d"),&Num);
}

IMPLEMENT_DYNAMIC(CDlgBank, CSkinDialogEx)
CDlgBank::CDlgBank(CWnd* pParent /*=NULL*/)
: CSkinDialogEx(IDD_BANK_STORAGE, pParent)
{
	m_lInCount=0;
	m_lGameGold=0;
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
	DDX_Control(pDX, STATIC_PASS, m_Static);

	//DDX_Text(pDX, IDC_GAME_GOLD, m_lGameGold);
	//DDX_Text(pDX, IDC_STORAGE_GOLD, m_lStorageGold);
}


BEGIN_MESSAGE_MAP(CDlgBank, CSkinDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_WM_CREATE()
	ON_BN_CLICKED(IDCANCEL,OnBnClickedCancel)
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
	ON_MESSAGE(WM_CREADED,OnCreateEd)

END_MESSAGE_MAP()


void CDlgBank::OnEnRadioStore()
{
	SetBankerActionType(true);

}
void CDlgBank::OnEnRadioGet()
{

	SetBankerActionType(false);
}
//��ȡ��ť
void CDlgBank::OnBnClickedOk()
{

	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	CPoint Point = GetDlgItem(IDC_IN_COUNT)->GetCaretPos();
	LONGLONG OutNum = 0;
	MakeStringToNum(strinCount,OutNum);
	m_lInCount= OutNum;

	//������֤
	if (false==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%I64d֮��"), m_lStorageGold);
		else strMessage.Format(TEXT("��Ĵ洢��Ϸ����ĿΪ0,���ܽ�����ȡ����!"));
		MessageBox(strMessage,TEXT("��ܰ��ʾ"));

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
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
#ifdef __SPECIAL___
		BankGetGold.cbGameAction=TRUE;
#endif
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
#ifdef __SPECIAL___
		BankStorage.cbGameAction=TRUE;
#endif
		BankStorage.lStorageValue = (LONGLONG)m_lInCount;
		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	ClearBank();

	return;
}

//��ʼ������
BOOL CDlgBank::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	UpdateData(FALSE);

	SetWindowText(TEXT("����"));


	SetBankerActionType(true);

	HideStorage();



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
		if (false==m_bBankStorage && (OutNum < 0 || OutNum > m_lStorageGold ))
		{
	
			return;
		}


		//������֤
		if (true==m_bBankStorage && (OutNum < 0 || OutNum > m_lGameGold ))
		{
			

			return;

		}
	}else
	{

	}
	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//������Ϣ
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	CString strlGameGold;

	MakeString(strlGameGold,m_lGameGold);
	SetDlgItemText(IDC_GAME_GOLD,strlGameGold);

	MakeString(strlGameGold,m_lStorageGold);
	SetDlgItemText(IDC_STORAGE_GOLD,strlGameGold);

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
	CString strCount;
	GetDlgItem(IDC_IN_COUNT)->GetWindowText(strCount);
	m_lInCount=_tstoi64(strCount);

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
afx_msg LRESULT  CDlgBank::OnCreateEd(WPARAM wparam,LPARAM lparam)
{



	return 1;

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
	SetWindowPos(NULL,Point.x,Point.y-Rect.bottom+Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,/*SWP_NOMOVE|*/SWP_NOZORDER);

}
//������Ϣ
void CDlgBank::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIClientKernel!=NULL);

	//������Ϣ
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}
void CDlgBank::OnBnClickedCancel()
{

	ClearBank();

	__super::OnCancel();

}
void CDlgBank::HideStorage()
{
	//m_RadioStore.ShowWindow(SW_HIDE);
	//m_RadioGet.ShowWindow(SW_HIDE);


}
void CDlgBank::ShowItem()
{

	if(m_bBankStorage)
	{
		GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(m_blCanStore&&m_blUsingPassWord);
		m_btOK.EnableWindow(m_blCanStore);
	}

	else
	{
		GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(TRUE);
		m_btOK.EnableWindow(TRUE);
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

		GetDlgItem(IDOK)->SetWindowText(TEXT("���"));
		m_blUsingPassWord = false;


	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText(TEXT("ȡ��"));



		m_blUsingPassWord = true;
	}

	//m_blUsingPassWord = true;
	if(m_blUsingPassWord)
	{

		GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(m_blCanStore);
		/*m_Edit.EnableWindow(TRUE);
		m_Static.EnableWindow(TRUE);*/
	}else
	{

		/*m_Edit.EnableWindow(FALSE);
		m_Static.EnableWindow(FALSE);*/
	}
	ShowItem();

}
void CDlgBank::ClearBank()
{
	return;
	GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
	GetDlgItem(IDC_IN_COUNT)->SetWindowText("");


}
