#include "StdAfx.h"
#include "BankServiceDlg.h"
#include "Resource.h"
#include ".\bankservicedlg.h"
//////////////////////////////////////////////////////////////////////////

//操作方式
#define BANK_SAVING						0						//存入游戏币
#define BANK_DRAWING					1						//修改密码
#define BANK_TRANSFER					2						//转帐服务
#define BANK_CHANGE_PASS				3						//修改密码


#define SELECT_COLOR		RGB(37,56,220)						//选择颜色

#define IDI_CHARMVALUE_UPDATE_VIEW		1								//更新界面
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//更新界面
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTabCtrlBank, CTabCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CBankItemDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_TYPE,OnBnClickedTargetUser)
	ON_BN_CLICKED(IDC_TYPE2,OnBnClickedTargetUser2)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CBankServiceDlg, CSkinDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SERVICE_TYPE, OnTcnSelchangeServiceType)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CBankFillDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//构造函数
CTabCtrlBank::CTabCtrlBank()
{
}

//析够函数
CTabCtrlBank::~CTabCtrlBank()
{
}

//重画消息
void CTabCtrlBank::OnPaint()
{
	//变量定义
	CPaintDC dc(this);
	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();

	//绘画背景
	CRect ClipRect,ClientRect;
	dc.GetClipBox(&ClipRect);
	dc.FillSolidRect(&ClipRect,pSkinAttribute->m_crBackGround);

	//设置 DC
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(pSkinAttribute->GetDefaultFont());
	
	//绘画边框
	GetClientRect(&ClientRect);
	ClientRect.top+=18;
	dc.Draw3dRect(&ClientRect,pSkinAttribute->m_crInsideBorder,pSkinAttribute->m_crInsideBorder);

	//获取信息
	TCITEM ItemInfo;
	TCHAR szBuffer[100];
	memset(&ItemInfo,0,sizeof(ItemInfo));
	ItemInfo.mask=TCIF_TEXT|TCIF_IMAGE;
	ItemInfo.pszText=szBuffer;
	ItemInfo.cchTextMax=sizeof(szBuffer);

	//绘画按钮
	CRect rcItem;
	int iCursel=GetCurSel();
	CPen LinePen(PS_SOLID,1,pSkinAttribute->m_crInsideBorder);
	CPen * pOldPen=dc.SelectObject(&LinePen);
	for (int i=0;i<GetItemCount();i++)
	{
		GetItem(i,&ItemInfo);
		GetItemRect(i,&rcItem);
		dc.FillSolidRect(&rcItem,pSkinAttribute->m_crBackGround);
		dc.MoveTo(rcItem.left,rcItem.bottom-1);
		dc.LineTo(rcItem.left,rcItem.top+2);
		dc.LineTo(rcItem.left+2,rcItem.top);
		dc.LineTo(rcItem.right-2,rcItem.top);
		dc.LineTo(rcItem.right,rcItem.top+2);
		dc.LineTo(rcItem.right,rcItem.bottom-1);
		
		if (iCursel!=i)
		{
			rcItem.top+=2;
			dc.MoveTo(rcItem.left,rcItem.bottom-1);
			dc.LineTo(rcItem.right,rcItem.bottom-1);
			dc.SetTextColor(pSkinAttribute->m_crControlTXColor);
			DrawText(dc,szBuffer,lstrlen(szBuffer),&rcItem,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else 
		{
			rcItem.top+=3;
			dc.SetTextColor(SELECT_COLOR);
			DrawText(dc,szBuffer,lstrlen(szBuffer),&rcItem,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
	}

	//清理资源
	dc.SelectObject(pOldPen);
	LinePen.DeleteObject();

	return;
}

//构造函数
CBankItemDlg::CBankItemDlg() : CDialog(IDD_BANK_ITEM)
{
	m_lInputCount=0;
	m_nSelectType=0 ;
	m_dwUserID=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_pClientSocket=NULL;
}

//析构函数
CBankItemDlg::~CBankItemDlg()
{
}

//初始化函数
BOOL CBankItemDlg::OnInitDialog()
{
	__super::OnInitDialog();

	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	if (NULL==m_brBakcground.m_hObject) m_brBakcground.CreateSolidBrush(pSkinAttribute->m_crBackGround);

	if (NULL==m_pMeUserData) return TRUE;

	m_lInputCount=m_pMeUserData->lInsureScore;

	//设置信息
	CString szGoldInfo;
	MakeString(szGoldInfo,m_pMeUserData->lGameGold);
	SetDlgItemText(IDC_GAME_GOLD,szGoldInfo);

	MakeString(szGoldInfo,m_pMeUserData->lInsureScore);
	SetDlgItemText(IDC_STORAGE_GOLD,szGoldInfo);

	UpdateData(FALSE);

	((CEdit*)(GetDlgItem(IDC_NICKNAME)))->LimitText(NAME_LEN-1);

	SetDlgItemText(IDC_NICKNAME,TEXT(""));
	SetDlgItemText(IDC_IN_COUNT,TEXT(""));
	SetDlgItemText(IDC_BANK_PSWD,TEXT(""));

	//定时更新
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	return FALSE;
}

//大小消息
void CBankItemDlg::OnSize(UINT nType, int cx, int cy)
{
	return;
}

void CBankItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_NICKNAME,m_strNickname);
	DDX_Text(pDX,IDC_BANK_PSWD,m_strPassword);
	DDX_Radio(pDX,IDC_TYPE,m_nSelectType);
	//DDX_Text(pDX,IDC_IN_COUNT,m_lInputCount);
}

void CBankItemDlg::OnBnClickedTargetUser()
{
	TCHAR szInputCount[128];
	GetDlgItemText(IDC_IN_COUNT,szInputCount,CountArray(szInputCount));
	if (0==szInputCount[0]) SetDlgItemText(IDC_IN_COUNT,TEXT("0"));
	UpdateData(TRUE);

	if (1==m_nSelectType) SetDlgItemText(IDC_TARGET_USER,TEXT("接受用户ID："));
	else SetDlgItemText(IDC_TARGET_USER,TEXT("接受用户昵称："));
}

void CBankItemDlg::OnBnClickedTargetUser2()
{
	TCHAR szInputCount[128];
	GetDlgItemText(IDC_IN_COUNT,szInputCount,CountArray(szInputCount));
	if (0==szInputCount[0]) SetDlgItemText(IDC_IN_COUNT,TEXT("0"));
	UpdateData(TRUE);

	if (1==m_nSelectType) SetDlgItemText(IDC_TARGET_USER,TEXT("接受用户ID："));
	else SetDlgItemText(IDC_TARGET_USER,TEXT("接受用户昵称："));
}



bool CBankItemDlg::CheckInput()
{
	TCHAR szInputCount[128];
	GetDlgItemText(IDC_IN_COUNT,szInputCount,CountArray(szInputCount));
	if (0==szInputCount[0]) SetDlgItemText(IDC_IN_COUNT,TEXT("0"));

	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	LONG OutNum = 0;
	MakeStringToNum(strinCount,OutNum);
	m_lInputCount=OutNum;

	UpdateData(TRUE);

	if (m_lInputCount<=0)
	{
		AfxMessageBox("请输入合法的转账数目");
		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		return false;
	}
	if (NULL!=m_pMeUserData && m_pMeUserData->lInsureScore<m_lInputCount)
	{
		AfxMessageBox("转账数目不能大于银行存款");
		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		return false;
	}
	if (m_strPassword.IsEmpty())
	{
		AfxMessageBox("请输入银行密码");
		GetDlgItem(IDC_BANK_PSWD)->SetFocus();
		return false;
	}

	if (m_strNickname.IsEmpty())
	{
		AfxMessageBox("请输入合法的昵称");
		GetDlgItem(IDC_NICKNAME)->SetFocus();
		return false;
	}
	else
	{
		if (1==m_nSelectType)
		{
			for (int i=0; i<m_strNickname.GetLength(); ++i)
			{
				CHAR ch=m_strNickname[i];
				if (!isdigit(ch))
				{
					AfxMessageBox("请输入合法的游戏ID");
					GetDlgItem(IDC_NICKNAME)->SetFocus();
					return false;
				}
			}
			m_dwUserID=GetDlgItemInt(IDC_NICKNAME);
		}
	}

	CString strMessage;
	if (0==m_nSelectType)
	{
		strMessage.Format(TEXT("您确实要转账给昵称为[ %s ]的玩家转账 %ld ？"), m_strNickname, m_lInputCount);
	}
	else
	{
		strMessage.Format(TEXT("您确实要转账给帐号ID为[ %s ]的玩家转账 %ld ？"), m_strNickname, m_lInputCount);
	}

	if (IDNO==AfxMessageBox(strMessage,MB_YESNO|MB_ICONINFORMATION))
	{
		return false;
	}

	return true;
}

void CBankItemDlg::OnPaint()
{
	CPaintDC dc(this); 

	//绘画背景
	CRect ClipRect,ClientRect;
	dc.GetClipBox(&ClipRect);
	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	dc.FillSolidRect(&ClipRect,pSkinAttribute->m_crBackGround);
}

HBRUSH CBankItemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (CTLCOLOR_STATIC==nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brBakcground;
	}
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

//设置信息
void CBankItemDlg::SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData)
{
	ASSERT((pClientKernel==NULL && pClientSocket!=NULL)||(pClientKernel!=NULL && pClientSocket==NULL));

	//设置信息
	m_pIClientKernel = pClientKernel;
	m_pClientSocket= pClientSocket;
	m_pMeUserData= pMeUserData;

	return;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CBankServiceDlg::CBankServiceDlg() : CSkinDialogEx(IDD_BANK_SERVICE)
{
	//设置变量
	m_pActiveItem=NULL;

	return;
}

//析构函数
CBankServiceDlg::~CBankServiceDlg()
{
}

//控件绑定
void CBankServiceDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICE_TYPE, m_TabServiceMode);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//初始化函数
BOOL CBankServiceDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("银行服务"));

	//加入标签
	m_TabServiceMode.InsertItem(BANK_SAVING,TEXT("存入游戏币"));
	m_TabServiceMode.InsertItem(BANK_DRAWING,TEXT("取出游戏币"));
	m_TabServiceMode.InsertItem(BANK_TRANSFER,TEXT("转账"));
	m_TabServiceMode.InsertItem(BANK_CHANGE_PASS,TEXT("修改密码"));

	//设置变量
	m_BankSavingDlg.SetSendInfo(m_pIClientKernel,m_pClientSocket,m_pMeUserData);
	m_BankDrawingDlg.SetSendInfo(m_pIClientKernel,m_pClientSocket,m_pMeUserData);
	m_BankTransferDlg.SetSendInfo(m_pIClientKernel,m_pClientSocket,m_pMeUserData);

	//创建控件
	m_BankSavingDlg.Create(IDD_BANK_STORAGE,this);
	m_BankDrawingDlg.Create(IDD_BANK_DRAWING,this);
	m_BankTransferDlg.Create(IDD_BANK_ITEM,this);
	m_BankChangePassDlg.Create(IDD_BANK_FILL,this);

	//计算位置
	CRect rcClient,rcItem;
	GetClientRect(&rcClient);
	rcItem.top=GetYExcursionPos()+28;
	rcItem.left=GetXExcursionPos()+13;
	rcItem.bottom=rcClient.Height()-60;
	rcItem.right=rcClient.Width()-GetXExcursionPos()-13;

	//移动位置
	m_BankSavingDlg.MoveWindow(&rcItem);
	m_BankDrawingDlg.MoveWindow(&rcItem);
	m_BankTransferDlg.MoveWindow(&rcItem);
	m_BankChangePassDlg.MoveWindow(&rcItem);

	//显示窗口
	m_pActiveItem=&m_BankSavingDlg;
	m_BankSavingDlg.ShowWindow(SW_SHOW);
	m_BankSavingDlg.BringWindowToTop();

	return FALSE;
}

//确定函数
void CBankServiceDlg::OnOK()
{
	switch (m_TabServiceMode.GetCurSel())
	{
	case BANK_SAVING:	{ m_BankSavingDlg.OnBnClickedOk(); return;}
	case BANK_DRAWING:	{ m_BankDrawingDlg.OnBnClickedOk(); return;}
	case BANK_CHANGE_PASS:	
		{
			if (true==m_BankChangePassDlg.CheckInput())
			{
				CMD_GF_ChangePassword ChangePassword={0};
				TCHAR szTempPassword[PASS_LEN]=TEXT("");
				CMD5Encrypt::EncryptData(LPCTSTR(m_BankChangePassDlg.m_strOriginPassword),szTempPassword);

				lstrcpyn(ChangePassword.szOriginPassword,szTempPassword,CountArray(ChangePassword.szOriginPassword));

				CMD5Encrypt::EncryptData(LPCTSTR(m_BankChangePassDlg.m_strNewPassword),szTempPassword);
				lstrcpyn(ChangePassword.szNewPassword,szTempPassword,CountArray(ChangePassword.szNewPassword));

				SendData(MDM_GF_BANK,SUB_GF_CHANGE_PASSWORD,&ChangePassword,sizeof(ChangePassword));

				m_BankChangePassDlg.SetDlgItemText(IDC_ORIGIN_PASSWORD,TEXT(""));
				m_BankChangePassDlg.SetDlgItemText(IDC_CONFIRM_PASSWORD,TEXT(""));
				m_BankChangePassDlg.SetDlgItemText(IDC_NEW_PASSWORD,TEXT(""));
			}
			return;
		}
	case BANK_TRANSFER:	
		{
			if (true==m_BankTransferDlg.CheckInput())
			{
				CMD_GF_Transfer Transfer={0};
				Transfer.lInputCount=m_BankTransferDlg.m_lInputCount;
				if (0==m_BankTransferDlg.m_nSelectType)
				{
					lstrcpyn(Transfer.szNickname,m_BankTransferDlg.m_strNickname,CountArray(Transfer.szNickname));
				}
				else
				{
					Transfer.dwUserID=m_BankTransferDlg.m_dwUserID;
				}
				TCHAR szTempPassword[PASS_LEN]=TEXT("");
				CMD5Encrypt::EncryptData(LPCTSTR(m_BankTransferDlg.m_strPassword),szTempPassword);

				lstrcpyn(Transfer.szPassword,szTempPassword,CountArray(Transfer.szPassword));

				SendData(MDM_GF_BANK,SUB_GF_TRANSFER,&Transfer,sizeof(Transfer));

				m_BankTransferDlg.SetDlgItemText(IDC_IN_COUNT,TEXT(""));
				m_BankTransferDlg.SetDlgItemText(IDC_BANK_PSWD,TEXT(""));
				m_BankTransferDlg.SetDlgItemText(IDC_NICKNAME,TEXT(""));
			}
			return;
		}
	}
}

//发送信息
void CBankServiceDlg::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT((m_pIClientKernel==NULL && m_pClientSocket!=NULL)||(m_pIClientKernel!=NULL && m_pClientSocket==NULL));

	//发送信息
	if(m_pClientSocket!=NULL)m_pClientSocket->SendData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);
	else m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

//类型改变
void CBankServiceDlg::OnTcnSelchangeServiceType(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	CDialog * pBankItemDlg=NULL;

	//获取目标
	switch (m_TabServiceMode.GetCurSel())
	{
	case BANK_SAVING:	{ pBankItemDlg=&m_BankSavingDlg; break; }
	case BANK_DRAWING:	{ pBankItemDlg=&m_BankDrawingDlg; break; }						
	case BANK_TRANSFER:	{ pBankItemDlg=&m_BankTransferDlg; break; }
	case BANK_CHANGE_PASS: { pBankItemDlg=&m_BankChangePassDlg; break; }
	}

	//激活页面
	if (pBankItemDlg!=m_pActiveItem)
	{
		//控制窗口
		if (pBankItemDlg!=NULL) 
		{
			pBankItemDlg->ShowWindow(SW_SHOW);
			pBankItemDlg->BringWindowToTop();
		}

		//隐藏窗口
		if (m_pActiveItem!=NULL) m_pActiveItem->ShowWindow(SW_HIDE);

		//设置变量
		m_pActiveItem=pBankItemDlg;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CBankFillDlg::CBankFillDlg() : CDialog(IDD_BANK_FILL)
{
	return;
}

//析构函数
CBankFillDlg::~CBankFillDlg()
{
}

//初始化函数
BOOL CBankFillDlg::OnInitDialog()
{
	__super::OnInitDialog();

	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	if (NULL==m_brBakcground.m_hObject) m_brBakcground.CreateSolidBrush(pSkinAttribute->m_crBackGround);

	return FALSE;
}

//兑换魅力
extern "C" PROPERTY_MODULE_CLASS VOID __cdecl ShowBankStorageDlg(IClientKernel *pIClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData)
{
	//兑换窗口
	CBankServiceDlg BankStorageDlg;

	//设置信息
	//BankStorageDlg.SetSendInfo(pIClientKernel,pClientSocket,pMeUserData);
	BankStorageDlg.m_pMeUserData=pMeUserData;
	BankStorageDlg.m_pIClientKernel=pIClientKernel;
	BankStorageDlg.m_pClientSocket=pClientSocket;
	BankStorageDlg.DoModal();

	return ;
}

//////////////////////////////////////////////////////////////////////////

void CBankFillDlg::OnPaint()
{
	CPaintDC dc(this); 

	//绘画背景
	CRect ClipRect,ClientRect;
	dc.GetClipBox(&ClipRect);
	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();
	dc.FillSolidRect(&ClipRect,pSkinAttribute->m_crBackGround);
}

HBRUSH CBankFillDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (CTLCOLOR_STATIC==nCtlColor)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_brBakcground;
	}

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CBankFillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_ORIGIN_PASSWORD,m_strOriginPassword);
	DDX_Text(pDX,IDC_NEW_PASSWORD,m_strNewPassword);
	DDX_Text(pDX,IDC_CONFIRM_PASSWORD,m_strConfirmPassword);

}


bool CBankFillDlg::CheckInput()
{
	UpdateData(TRUE);

	if (m_strOriginPassword.IsEmpty())
	{
		AfxMessageBox("请输入旧密码");
		GetDlgItem(IDC_ORIGIN_PASSWORD)->SetFocus();
		return false;
	}
	if (m_strNewPassword.IsEmpty())
	{
		AfxMessageBox("请输入新密码");
		GetDlgItem(IDC_NEW_PASSWORD)->SetFocus();
		return false;
	}
	if (m_strNewPassword!=m_strNewPassword)
	{
		AfxMessageBox("确认密码有误");
		GetDlgItem(IDC_CONFIRM_PASSWORD)->SetFocus();
		return false;
	}
	return true;
}

void CBankItemDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CBankItemDlg::OnTimer(UINT nIDEvent)
{
	//时间消息
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//更新界面
		{
			CString strinCount;
			GetDlgItemText(IDC_IN_COUNT,strinCount);
			LONG OutNum = 0;
			MakeStringToNum(strinCount,OutNum);
			m_lInputCount=OutNum;
			if(OutNum!=0)
			{
				MakeString(strinCount,OutNum);
				SetDlgItemText(IDC_IN_COUNT,strinCount);
				((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
			}

			//设置信息
			CString szGoldInfo;
			MakeString(szGoldInfo,m_pMeUserData->lGameGold);
			SetDlgItemText(IDC_GAME_GOLD,szGoldInfo);

			MakeString(szGoldInfo,m_pMeUserData->lInsureScore);
			SetDlgItemText(IDC_STORAGE_GOLD,szGoldInfo);
			return;
		}
	}
	CDialog::OnTimer(nIDEvent);
}
