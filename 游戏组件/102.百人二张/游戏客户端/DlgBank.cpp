// DlgBank.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBank.h"
#include "resource.h"

//定时器I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//更新界面
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//更新界面

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
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)

END_MESSAGE_MAP()

//提取按钮
void CDlgBank::OnBnClickedOk()
{

	
	//参数验证
	if (false==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"), m_lStorageGold);
		else strMessage.Format(TEXT("你的存储游戏币数目为0,不能进行提取操作!"));
		MessageBox(strMessage,TEXT("温馨提示"));

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}


	//参数验证
	if (true==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"),m_lGameGold );
		else strMessage.Format(TEXT("你的当前游戏币数目为0,不能进行存储操作!"));
		MessageBox(strMessage,TEXT("温馨提示"));

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}
	//密码验证
	TCHAR szPassword[PASS_LEN]=TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	if (szPassword[0]==0&&m_bBankStorage==false)
	{
		MessageBox(TEXT("密码不能为空，请重新输入密码！"),TEXT("温馨提示"));
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}
	if(m_bBankStorage==false)
	{
		TCHAR szTempPassword[PASS_LEN]=TEXT("");
		CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
		CMD5Encrypt::EncryptData(szTempPassword,szPassword);

	}
	if (false==m_bBankStorage)
	{
		//发送信息
		CMD_GF_BankGet BankGetGold;
		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
		BankGetGold.lGetValue = (LONGLONG)m_lInCount;
		BankGetGold.cbGameAction=TRUE;
		CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	}
	else
	{
		//状态判断
		if (US_PLAY==m_pMeUserData->cbUserStatus)
		{
			MessageBox(TEXT("请结束游戏后再存款！"),TEXT("温馨提示"));
			return;	
		}

		//发送信息
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.cbGameAction=TRUE;
		BankStorage.lStorageValue = (LONGLONG)m_lInCount;
		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	return;
}

//初始化函数
BOOL CDlgBank::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	UpdateData(FALSE);

	SetWindowText(TEXT("银行"));

	//定时更新
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	return TRUE;
}

//更新界面
void CDlgBank::UpdateView()
{
	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//设置信息
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	CString static strGameGold=TEXT(""), strStorageGold=TEXT(""),strTmpNumber1,strTmpNumber2;
	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strGameGold.Empty();
	strStorageGold.Empty();
	if(m_lGameGold==0) strGameGold=TEXT("0");
	int nNumberCount=0;
	int count=0;
	while(m_lGameGold>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),m_lGameGold%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if(nNumberCount==3)
		{
			count++;
			if(count==1)
			{
				strTmpNumber2 +=strGameGold;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strGameGold);
			}
			strGameGold=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		m_lGameGold/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(strGameGold.IsEmpty()==FALSE)
			strTmpNumber2 += (TEXT(",") +strGameGold);
		strGameGold=strTmpNumber2;
	}

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();

	if(m_lStorageGold==0) strStorageGold=TEXT("0");
	nNumberCount=0;
	count=0;
	while(m_lStorageGold>0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),m_lStorageGold%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if(nNumberCount==3)
		{
			count++;
			if(count==1)
			{
				strTmpNumber2 +=strStorageGold;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strStorageGold);
			}
			strStorageGold=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		m_lStorageGold/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		if(strStorageGold.IsEmpty()==FALSE)
			strTmpNumber2 += (TEXT(",") +strStorageGold);
		strStorageGold=strTmpNumber2;
	}

	GetDlgItem(IDC_GAME_GOLD)->SetWindowText(strGameGold);
	GetDlgItem(IDC_STORAGE_GOLD)->SetWindowText(strStorageGold);

	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	UpdateData(FALSE);
}

//鼠标信息
void CDlgBank::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnLButtonDown(nFlags, point);
}

//鼠标信息
void CDlgBank::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnMouseMove(nFlags, point);
}

//命令信息
BOOL CDlgBank::PreTranslateMessage(MSG* pMsg)
{
	//过滤字母
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}

	////过滤回车
	//if(pMsg->message==WM_KEYDOWN)
	//{
	//	BYTE bMesValue = (BYTE)(pMsg->wParam);
	//	if(bMesValue==VK_RETURN) return TRUE;
	//}

	return __super::PreTranslateMessage(pMsg);
}

//时间消息
void CDlgBank::OnTimer(UINT nIDEvent)
{
	//时间消息
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//更新界面
		{
			//更新界面
			UpdateView();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//输入信息
void CDlgBank::OnEnChangeInCount()
{
	CString strText,strTmpText;
	strText = TEXT("");
	INT64 lGold;
	if (GetDlgItemText(IDC_IN_COUNT,strText)==0) return;
	strText.Remove(',');
	lGold = _tstoi64(strText);
	int NumCount=0;
	int lenCount=0;
	if(lGold==0) strText=TEXT("");
	if(lGold>0)
	{
		strTmpText.Format(TEXT("%I64d"),lGold);
		if(strTmpText.GetLength() > 3)
		{
			for(int i=0;i<strTmpText.GetLength();i++)
			{
				if(i%4==0)
				{
					strTmpText.Insert(strTmpText.GetLength()-i,',');
				}
			}
		}
		int nFind = strTmpText.ReverseFind(',');
		if(-1==nFind)
		{
			strText=strTmpText;
		}
		else
		{
			strText = strTmpText.Left(strTmpText.ReverseFind(','));
		}
		GetDlgItem(IDC_IN_COUNT)->SetWindowText(strText);
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strText.GetLength(),-1);
	}
	strText.Remove(',');
	m_lInCount=_tstoi64(strText);

	return ;
}

//是否选中
bool CDlgBank::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//选中按钮
void CDlgBank::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//设置信息
void CDlgBank::SetSendInfo(IClientKernel *pClientKernel,tagUserData const*pMeUserData)
{
	ASSERT(pClientKernel!=NULL);

	//设置信息
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
//发送信息
void CDlgBank::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIClientKernel!=NULL);

	//发送信息
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

//设置类型
void CDlgBank::SetBankerActionType(bool bStorage) 
{
	//设置变量
	m_bBankStorage=bStorage;
	//设置标题
	if (m_bBankStorage)
	{
		m_Edit.ShowWindow(SW_HIDE);
		m_Static.ShowWindow(SW_HIDE);
		GetDlgItem(IDOK)->SetWindowText(TEXT("存款"));

	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText(TEXT("取款"));
		m_Edit.ShowWindow(SW_SHOW);
		m_Static.ShowWindow(SW_SHOW);
	}
	
}
