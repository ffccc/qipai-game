// AdminControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "AdminControlDlg.h"


// CAdminControlDlg �Ի���

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


// CAdminControlDlg ��Ϣ�������

BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//������Ϣ
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ�����"));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("������ƣ�"));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵����"));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ������ȣ�"));
	SetDlgItemText(IDC_RADIO_BEN_40,TEXT("���ۡ�40"));
	SetDlgItemText(IDC_RADIO_BAO_30,TEXT("�����30"));
	SetDlgItemText(IDC_RADIO_AO_20,TEXT("�µϡ�20"));
	SetDlgItemText(IDC_RADIO_DA_10,TEXT("���ڡ�10"));
	SetDlgItemText(IDC_RADIO_BEN_5,TEXT("���ۡ�5"));
	SetDlgItemText(IDC_RADIO_BAO_5,TEXT("�����5"));
	SetDlgItemText(IDC_RADIO_AO_5,TEXT("�µϡ�5"));
	SetDlgItemText(IDC_RADIO_DA_5,TEXT("���ڡ�5"));
	SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("����"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));
	return TRUE;  
}

//������ɫ
HBRUSH CAdminControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_STATIC_INFO) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 
	return hbr;
}


//ȡ������
void CAdminControlDlg::OnBnClickedButtonReset()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_CANCELS;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//���ֿ���
void CAdminControlDlg::OnBnClickedButtonSyn()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_UPDATE;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//��������
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
		SetDlgItemText(IDC_STATIC_INFO,TEXT("��ѡ���ܿش����Լ��ܿ�����"));
	}

}

//ȡ���ر�
void CAdminControlDlg::OnBnClickedButtonCancel()
{
	ShowWindow(SW_HIDE);
}


//���¿���
void CAdminControlDlg::UpdateControl( CMD_S_ControlReturns* pControlReturns )
{
	switch(pControlReturns->cbReturnsType)
	{
	case S_CR_FAILURE:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("����ʧ�ܣ�"));
			break;
		}
	case S_CR_UPDATE_SUCCES:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			_snprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			_snprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_CANCEL_SUCCESS:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("ȡ�����óɹ���"));
			break;
		}
	}
}

//��Ϣ
void CAdminControlDlg::PrintingInfo( TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes )
{
	if (cbArea == 0xff )
	{
		_snprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
		return;
	}
	TCHAR szDesc[32] = TEXT("");
	_snprintf(pText,cbCount,TEXT("ʤ������"));
	if ( cbArea == 0 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ۡ�40��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 1 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("�����30��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 2 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("�µϡ�20��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 3 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ڡ�10��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 4 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ۡ�5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 5 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("�����5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 6 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("�µϡ�5��") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 7 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ڡ�5��") );
		lstrcat( pText,szDesc );
	}
	_snprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"), cbTimes);
	lstrcat( pText,szDesc );
}
