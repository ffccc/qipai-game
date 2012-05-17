// AdminControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AdminControlDlg.h"
#include ".\admincontroldlg.h"


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

void CAdminControlDlg::ReSetAdminWnd()
{

	if(m_cbControlStyle!=CS_BANKER_WIN&&m_cbControlStyle!=CS_BANKER_LOSE)
	{
		((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(m_cbControlStyle==CS_BANKER_WIN?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(m_cbControlStyle==CS_BANKER_LOSE?1:0);
	}

	if(m_cbControlStyle==CS_BET_AREA)
	{
		((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(1);

		BYTE cbIndex=0;
		for(;cbIndex<CONTROL_AREA;cbIndex++)
			((CButton*)GetDlgItem(IDC_CHECK_TIAN+cbIndex))->SetCheck(m_bWinArea[cbIndex]?1:0);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes>0?(m_cbExcuteTimes-1):-1);
	OnRadioClick();
}

//������
bool CAdminControlDlg::ReqResult(const void * pBuffer)
{
	const CMD_S_CommandResult*pResult=(CMD_S_CommandResult*)pBuffer;
	CString str;
	switch(pResult->cbAckType)
	{
	case ACK_SET_WIN_AREA:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="��Ӯ���������Ѿ����ܣ�";
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format("����ʧ��!");
				m_cbExcuteTimes=0;
				m_cbControlStyle=0;
				ZeroMemory(m_bWinArea,sizeof(m_bWinArea));
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_RESET_CONTROL:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				m_cbControlStyle=0;
				m_cbExcuteTimes=0;
				ZeroMemory(m_bWinArea,sizeof(m_bWinArea));
				ReSetAdminWnd();
				str="���������ѽ���!";
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_PRINT_SYN:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="������ͬ�������ѽ���!";
				
				tagAdminReq*pAdminReq=(tagAdminReq*)pResult->cbExtendData;
				m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
				m_cbControlStyle=pAdminReq->m_cbControlStyle;
				memcpy(m_bWinArea,pAdminReq->m_bWinArea,sizeof(m_bWinArea));
				ReSetAdminWnd();
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format("����ʧ��!");
			}
			break;
		}

	default: break;
	}

	SetWindowText(str);
	return true;
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BT_CANCEL,OnCancel)
	ON_BN_CLICKED(IDC_BT_EXCUTE,OnExcute)
	ON_BN_CLICKED(IDC_BT_CURSET,OnRefresh)
	ON_BN_CLICKED(IDC_RADIO_CT_AREA,OnRadioClick)
	ON_BN_CLICKED(IDC_RADIO_CT_BANKER,OnRadioClick)
	ON_WM_TIMER()
END_MESSAGE_MAP()



void CAdminControlDlg::OnRadioClick()
{
	if(((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->GetCheck()==1)
	{
		GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_WIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TIAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DI)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_XUAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HUANG)->EnableWindow(FALSE);
	}
	else
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->GetCheck())
		{
			GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_WIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_TIAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_DI)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_XUAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_HUANG)->EnableWindow(TRUE);
		}
	}
}

// CAdminControlDlg ��Ϣ�������
void CAdminControlDlg::OnReSet()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_RESET_CONTROL;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}
void CAdminControlDlg::OnRefresh()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_PRINT_SYN;
	AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}
void CAdminControlDlg::OnExcute()
{
	BYTE cbIndex=0;
	bool bFlags=false;
	m_cbControlStyle=0;
	m_cbExcuteTimes=0;
	ZeroMemory(m_bWinArea,sizeof(m_bWinArea));

	//����ׯ��
	if(((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->GetCheck())
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_WIN))->GetCheck())
		{
			m_cbControlStyle=CS_BANKER_WIN;
			bFlags=true;
		}
		else
		{
			if(((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck())
			{
				m_cbControlStyle=CS_BANKER_LOSE;
				bFlags=true;
			}
		}
	}
	else //��������
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->GetCheck())
		{
			m_cbControlStyle=CS_BET_AREA;
			for(cbIndex=0;cbIndex<CONTROL_AREA;cbIndex++)
				m_bWinArea[cbIndex]=((CButton*)GetDlgItem(IDC_CHECK_TIAN+cbIndex))->GetCheck()?true:false;
			bFlags=true;
		}
	}

	m_cbExcuteTimes=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel()+1;
	//��ȡִ�д���
	if(m_cbExcuteTimes<=0)
	{
		bFlags=false;
	}
	

	if(bFlags) //������Ч
	{
		CMD_C_AdminReq adminReq;
		adminReq.cbReqType=RQ_SET_WIN_AREA;
		tagAdminReq*pAdminReq=(tagAdminReq*)adminReq.cbExtendData;
		pAdminReq->m_cbExcuteTimes=m_cbExcuteTimes;
		pAdminReq->m_cbControlStyle=m_cbControlStyle;
		memcpy(pAdminReq->m_bWinArea,m_bWinArea,sizeof(m_bWinArea));
		AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox("��ѡ���ܿش����Լ��ܿط�ʽ!");
		OnRefresh();
	}
}
void CAdminControlDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnCancel();
}

BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_BT_RESET,"ȡ������");	
	SetDlgItemText(IDC_BT_CURSET,"��ǰ����");
	SetDlgItemText(IDC_BT_EXCUTE,"ִ��");	
	SetDlgItemText(IDC_BT_CANCEL,"ȡ��");	
	SetDlgItemText(IDC_RADIO_WIN,"ׯ��Ӯ");	
	SetDlgItemText(IDC_RADIO_LOSE,"ׯ����");			 
	SetDlgItemText(IDC_RADIO_CT_BANKER,"ׯ�ҿ���");	
	SetDlgItemText(IDC_RADIO_CT_AREA,"�������");	
	SetDlgItemText(IDC_CHECK_TIAN,"��");	
	SetDlgItemText(IDC_CHECK_DI,"��");	
	SetDlgItemText(IDC_CHECK_XUAN,"��");
	SetDlgItemText(IDC_CHECK_HUANG,"��");
	SetDlgItemText(IDC_STATIC_TIMES	,"���ƴ���");	
	SetDlgItemText(IDC_STATIC_CHOICE,"����ѡ��");	
	SetDlgItemText(IDC_STATIC_NOTICE,"˵��");
	SetDlgItemText(IDC_STATIC_NOTICE1,"1.������Ӯ���Ʊ���Ϸ�����Ʋ������ȡ�");	
	SetDlgItemText(IDC_STATIC_NOTICE2,"2.������ƣ�ѡ��Ϊʤ��δѡ��Ϊ�䡣");	

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("1");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("2");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("3");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("4");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("5");

	((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(1);
	OnRadioClick();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAdminControlDlg::PrintCurStatus()
{
	CString str;
	switch(m_cbControlStyle)
	{
	case CS_BET_AREA:
		{
			str="ʤ������:";
			BYTE cbIndex=0;
			bool bFlags=false;
			for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
			{
				if(m_bWinArea[cbIndex])
				{
					bFlags=true;
					switch(cbIndex)
					{
						case 0:str+="�� ";break;
						case 1:str+="�� ";break;
						case 2:str+="�� ";break;
						case 3:str+="�� ";break;
						default: break;
					}
				}
			}
			if(!bFlags)
			{
				str+="ׯ��ͨɱ";
			}
			str.AppendFormat(",ִ�д���:%d",m_cbExcuteTimes);
			break;
		}
	case CS_BANKER_WIN:
		{
			str.Format("ׯ���ܿ�,�ܿط�ʽ:Ӯ,ִ�д���:%d",m_cbExcuteTimes);break;
		}
	case CS_BANKER_LOSE:
		{
			str.Format("ׯ���ܿ�,�ܿط�ʽ:��,ִ�д���:%d",m_cbExcuteTimes);break;
		}
	default: str="�ǿ���״̬";break;
	}	

	SetWindowText(str);
}

VOID CAdminControlDlg::OnAllowControl(bool bEnable)
{
	GetDlgItem(IDC_BT_EXCUTE)->EnableWindow(bEnable);
}
void CAdminControlDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	PrintCurStatus();
	CDialog::OnTimer(nIDEvent);
}
