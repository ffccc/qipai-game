// AdminControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "AdminControlDlg.h"


// CAdminControlDlg 对话框

IMPLEMENT_DYNAMIC(CAdminControlDlg, CDialog)
CAdminControlDlg::CAdminControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminControlDlg::IDD, pParent)
{
}

CAdminControlDlg::~CAdminControlDlg()
{
}

void CAdminControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SYN, OnBnClickedButtonSyn)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAdminControlDlg 消息处理程序

BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//设置信息
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("控制局数："));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("区域控制："));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("控制说明："));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("区域输赢控制比游戏库存控制策略优先！"));
	SetDlgItemText(IDC_RADIO_BEN_40,TEXT("奔驰×40"));
	SetDlgItemText(IDC_RADIO_BAO_30,TEXT("宝马×30"));
	SetDlgItemText(IDC_RADIO_AO_20,TEXT("奥迪×20"));
	SetDlgItemText(IDC_RADIO_DA_10,TEXT("大众×10"));
	SetDlgItemText(IDC_RADIO_BEN_5,TEXT("奔驰×5"));
	SetDlgItemText(IDC_RADIO_BAO_5,TEXT("宝马×5"));
	SetDlgItemText(IDC_RADIO_AO_5,TEXT("奥迪×5"));
	SetDlgItemText(IDC_RADIO_DA_5,TEXT("大众×5"));
	SetDlgItemText(IDC_BUTTON_RESET,TEXT("取消控制"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("本局"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("执行"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("取消"));
	return TRUE;  
}

//设置颜色
HBRUSH CAdminControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_STATIC_INFO) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 
	return hbr;
}


//取消控制
void CAdminControlDlg::OnBnClickedButtonReset()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_CANCELS;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//本局控制
void CAdminControlDlg::OnBnClickedButtonSyn()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_UPDATE;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//开启控制
void CAdminControlDlg::OnBnClickedButtonOk()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));

	BYTE cbSelectedIndex = 0xff;
	if(((CButton*)GetDlgItem(IDC_RADIO_BEN_40))->GetCheck())
		cbSelectedIndex = 0;
	else if(((CButton*)GetDlgItem(IDC_RADIO_BAO_30))->GetCheck())
		cbSelectedIndex = 1;
	else if(((CButton*)GetDlgItem(IDC_RADIO_AO_20))->GetCheck())
		cbSelectedIndex = 2;
	else if(((CButton*)GetDlgItem(IDC_RADIO_DA_10))->GetCheck())
		cbSelectedIndex = 3;
	else if(((CButton*)GetDlgItem(IDC_RADIO_BEN_5))->GetCheck())
		cbSelectedIndex = 4;
	else if(((CButton*)GetDlgItem(IDC_RADIO_BAO_5))->GetCheck())
		cbSelectedIndex = 5;
	else if(((CButton*)GetDlgItem(IDC_RADIO_AO_5))->GetCheck())
		cbSelectedIndex = 6;
	else if(((CButton*)GetDlgItem(IDC_RADIO_DA_5))->GetCheck())
		cbSelectedIndex = 7;

	int nSelectTimes = ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

	if ( cbSelectedIndex != 0xff && nSelectTimes >= 0 && nSelectTimes != 0xffffffff)
	{
		ControlApplication.cbControlAppType = C_CA_SET;
		ControlApplication.cbControlArea = cbSelectedIndex;
		ControlApplication.cbControlTimes = static_cast<BYTE>(nSelectTimes + 1);
		AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_INFO,TEXT("请选择受控次数以及受控区域！"));
	}

}

//取消关闭
void CAdminControlDlg::OnBnClickedButtonCancel()
{
	ShowWindow(SW_HIDE);
}


//更新控制
void CAdminControlDlg::UpdateControl( CMD_S_ControlReturns* pControlReturns )
{
	switch(pControlReturns->cbReturnsType)
	{
	case S_CR_FAILURE:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("操作失败！"));
			break;
		}
	case S_CR_UPDATE_SUCCES:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			_snprintf(zsText,CountArray(zsText),TEXT("更新数据成功！\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			_snprintf(zsText,CountArray(zsText),TEXT("设置数据成功！\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_CANCEL_SUCCESS:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("取消设置成功！"));
			break;
		}
	}
}

//信息
void CAdminControlDlg::PrintingInfo( TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes )
{
	if (cbArea == 0xff )
	{
		_snprintf(pText,cbCount,TEXT("暂时无控制。"));
		return;
	}
	TCHAR szDesc[32] = TEXT("");
	_snprintf(pText,cbCount,TEXT("胜利区域："));
	if ( cbArea == 0 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("奔驰×40，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 1 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("宝马×30，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 2 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("奥迪×20，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 3 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("大众×10，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 4 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("奔驰×5，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 5 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("宝马×5，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 6 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("奥迪×5，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 7 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("大众×5，") );
		lstrcat( pText,szDesc );
	}
	_snprintf(szDesc,CountArray(szDesc),TEXT("执行次数：%d。"), cbTimes);
	lstrcat( pText,szDesc );
}
