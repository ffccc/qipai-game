
#include "stdafx.h"
#include "GameBank.h"

//定时器I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//更新界面
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//更新界面

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameBank, CSkinDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDOK,OnBnClickedOk)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
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

//析构函数
CGameBank::~CGameBank()
{
}

//控件绑定
void CGameBank::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Text(pDX, IDC_GAME_GOLD, m_lGameGold);
	DDX_Text(pDX, IDC_STORAGE_GOLD, m_lStorageGold);
}

//提取按钮
void CGameBank::OnBnClickedOk()
{
	//参数验证
	if (IsButtonSelected(IDC_DRAWOUT) && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"), m_lStorageGold);
		else strMessage.Format(TEXT("你的银行金币数目为0,不能进行提取操作!"));
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("温馨提示")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//参数验证
	if (IsButtonSelected(IDC_STORAGE) && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"),m_lGameGold );
		else strMessage.Format(TEXT("你的当前金币币数目为0,不能进行存储操作!"));
		
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("温馨提示")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}
	
	//参数验证
	if (IsButtonSelected(IDC_STORAGE) && ( m_lInCount > (m_lGameGold-m_lMeJetton)))
	{
		CString strMessage;
		if ((m_lGameGold-m_lMeJetton)==0)
		{
			strMessage.Format(TEXT("你输入的游戏币值必须在0和%ld之间"),0 );
		}
		else
		{
			strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"),m_lGameGold-m_lMeJetton );
		}
		
		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("温馨提示")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}

	//参数验证
	if (IsButtonSelected(IDC_STORAGE) && ((m_pMeUserData->wChairID==m_wBankerUser))
		&&( m_lInCount > (m_lGameGold-m_lApplyBankerCondition)))
	{
		CString strMessage;
		if ((m_lGameGold-m_lApplyBankerCondition)==0)
		{
			strMessage.Format(TEXT("你输入的游戏币值必须在0和%ld之间"),0 );
		}
		else
		{
			strMessage.Format(TEXT("你输入的游戏币值必须在1和%ld之间"),m_lGameGold-m_lApplyBankerCondition );
		}

		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("温馨提示")) ;

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		return;
	}
	
	//参数验证
	if (m_pMeUserData->cbUserStatus==US_PLAY)
	{
		CString strMessage;

		strMessage.Format(TEXT("请在游戏空闲时间内，进行操作!"));

		ShowInformationEx(strMessage,0 ,MB_ICONINFORMATION,TEXT("温馨提示")) ;
		return;
	}

	//密码验证
	TCHAR szPassword[PASS_LEN]=TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	if (szPassword[0]==0)
	{
		ShowInformationEx(TEXT("密码不能为空，请重新输入密码！"),0 ,MB_ICONINFORMATION,TEXT("系统消息")) ;
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText("");
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}


	TCHAR szTempPassword[PASS_LEN]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CMD5Encrypt::EncryptData(szTempPassword,szPassword);

	if (IsButtonSelected(IDC_DRAWOUT))
	{
		//发送信息
		CMD_GF_BankGet BankGetGold;
		ZeroMemory(&BankGetGold, sizeof(BankGetGold));
		BankGetGold.lGetValue = m_lInCount;
		BankGetGold.cbIsGameAction = TRUE;
		CopyMemory(BankGetGold.szPassword,szPassword,sizeof(BankGetGold.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_GET, &BankGetGold, sizeof(BankGetGold));
	}
	else
	{
		//发送信息
		CMD_GF_BankStorage BankStorage;
		ZeroMemory(&BankStorage, sizeof(BankStorage));
		BankStorage.lStorageValue = m_lInCount;
		BankStorage.cbIsGameAction = TRUE;
		CopyMemory(BankStorage.szPassword,szPassword,sizeof(BankStorage.szPassword));

		SendData(MDM_GF_BANK, SUB_GF_BANK_STORAGE, &BankStorage, sizeof(BankStorage));
	}

	return;
}

//初始化函数
BOOL CGameBank::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();
	::SetWindowPos(this->GetSafeHwnd(), CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE);
	UpdateData(FALSE);

	SetWindowText(TEXT("银行"));

	//设置数值
	SetButtonSelected(IDC_STORAGE,true);

	//定时更新
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	return TRUE;
}

//更新界面
void CGameBank::UpdateView()
{
	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//设置信息
	m_lGameGold=((m_pMeUserData->lGameGold<0)?0:m_pMeUserData->lGameGold);
	m_lStorageGold=m_pMeUserData->lInsureScore;

	UpdateData(FALSE);
}

//鼠标信息
void CGameBank::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnLButtonDown(nFlags, point);
}

//鼠标信息
void CGameBank::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialogEx::OnMouseMove(nFlags, point);
}

//命令信息
BOOL CGameBank::PreTranslateMessage(MSG* pMsg)
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
void CGameBank::OnTimer(UINT nIDEvent)
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
void CGameBank::OnEnChangeInCount()
{
	CString strCount;
	GetDlgItem(IDC_IN_COUNT)->GetWindowText(strCount);
	m_lInCount=_tstol(strCount);

	return ;
}

//是否选中
bool CGameBank::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//选中按钮
void CGameBank::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//设置信息
void CGameBank::SetSendInfo(IClientKernel *pClientKernel,const tagUserData *pMeUserData,LONG lMeJetton, WORD wBankerUser, LONG lApplyBankerCondition, bool bMeApplyBanker)
{
	ASSERT(pClientKernel==NULL);

	//设置信息
	m_pIClientKernel = pClientKernel;
	m_pMeUserData = pMeUserData;
	m_lMeJetton = lMeJetton;
	m_wBankerUser =  wBankerUser;
	m_lApplyBankerCondition = lApplyBankerCondition;
	m_bMeApplyBanker = bMeApplyBanker;
	return;
}

//发送信息
void CGameBank::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIClientKernel==NULL); 
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	return ;
}
//////////////////////////////////////////////////////////////////////////
