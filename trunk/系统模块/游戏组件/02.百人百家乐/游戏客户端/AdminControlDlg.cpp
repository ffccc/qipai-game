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
	m_cbWinArea=0;				//Ӯ������
	m_cbExcuteTimes=0;			//ִ�д���
}

CAdminControlDlg::~CAdminControlDlg()
{
}

void CAdminControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnExcute)
	ON_BN_CLICKED(IDC_BUTTON_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BUTTON_SYN,OnRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL,OnCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAdminControlDlg ��Ϣ�������

BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	//m_btAdminControl[0].Create("",WS_CHILD|WS_VISIBLE,CRect(0,0,48,20),this,IDC_BUTTON_RESET+233);
	//m_btAdminControl[1].Create("",WS_CHILD|WS_VISIBLE,CRect(50,0,98,20),this,IDC_BUTTON_SYN+233);
	//m_btAdminControl[2].Create("",WS_CHILD|WS_VISIBLE,CRect(100,0,148,20),this,IDC_BUTTON_OK+233);
	//m_btAdminControl[3].Create("",WS_CHILD|WS_VISIBLE,CRect(150,0,198,20),this,IDC_BUTTON_CANCEL+233);




	//LOGFONT   lf;  
	//CFont*   pFont=m_btAdminControl[0].GetFont(); 
	//pFont->GetLogFont(&lf);   
	//lf.lfWeight   =   700;   
	//m_btFont.CreateFontIndirect(&lf);   
	//m_btAdminControl[0].SetFont(&m_btFont);


	//m_btAdminControl[0].SetWindowText("����");
	//m_btAdminControl[1].SetWindowText("����");
	//m_btAdminControl[2].SetWindowText("����");
	//m_btAdminControl[3].SetWindowText("����");

	SetDlgItemText(IDC_STATIC_TIMES,"���ƾ���:");
	SetDlgItemText(IDC_STATIC_AREA, "�������:");
	SetDlgItemText(IDC_STATIC_NOTIC,"����˵��:");
	SetDlgItemText(IDC_STATIC_TEXT,"������Ӯ���Ʊ���Ϸ�����Ʋ�������!");
	SetDlgItemText(IDC_RADIO_LT,"�ж���");
	SetDlgItemText(IDC_RADIO_LC,"  ��  ");
	SetDlgItemText(IDC_RADIO_LB,"������");
	SetDlgItemText(IDC_RADIO_CC,"  ƽ  ");
	SetDlgItemText(IDC_RADIO_CB,"ͬ��ƽ");
	SetDlgItemText(IDC_RADIO_RT,"ׯ����");
	SetDlgItemText(IDC_RADIO_RC,"  ׯ  ");
	SetDlgItemText(IDC_RADIO_RB,"ׯ����");

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("1");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("2");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("3");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("4");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("5");


	SetDlgItemText(IDC_BUTTON_RESET,"ȡ������");
	SetDlgItemText(IDC_BUTTON_SYN,"����");
	SetDlgItemText(IDC_BUTTON_OK,"ִ��");
	SetDlgItemText(IDC_BUTTON_CANCEL,"ȡ��");

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}




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
	CMD_C_AdminReq adminReq;
	BYTE cbIndex=0;
	int nCheckID;
	for(cbIndex=0;cbIndex<8;cbIndex++)
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_LT+cbIndex))->GetCheck())
		{
			nCheckID=cbIndex+1;
		}
	}

	int nSelect=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

	if(nCheckID>0&&nSelect>=0)
	{
		adminReq.cbReqType=RQ_SET_WIN_AREA;
	    adminReq.cbExtendData[0]=(BYTE)nCheckID;
		adminReq.cbExtendData[1]=(BYTE)nSelect+1;
		m_cbWinArea=adminReq.cbExtendData[0];
		m_cbExcuteTimes=adminReq.cbExtendData[1];

		AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox("��ѡ���ܿش����Լ��ܿ�����!");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


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
					switch(m_cbWinArea)
					{
						case 0:str.Format("���ܿ�����");break;
						case 1:str.Format("�����ѽ���,ʤ������:�ж���,ִ�д���:%d",m_cbExcuteTimes);break;
						case 2:str.Format("�����ѽ���,ʤ������:  ��  ,ִ�д���:%d",m_cbExcuteTimes);break;
						case 3:str.Format("�����ѽ���,ʤ������:������,ִ�д���:%d",m_cbExcuteTimes);break;
						case 4:str.Format("�����ѽ���,ʤ������:  ƽ  ,ִ�д���:%d",m_cbExcuteTimes);break;
						case 5:str.Format("�����ѽ���,ʤ������:ͬ��ƽ,ִ�д���:%d",m_cbExcuteTimes);break;
						case 6:str.Format("�����ѽ���,ʤ������:ׯ����,ִ�д���:%d",m_cbExcuteTimes);break;
						case 7:str.Format("�����ѽ���,ʤ������:  ׯ  ,ִ�д���:%d",m_cbExcuteTimes);break;
						case 8:str.Format("�����ѽ���,ʤ������:ׯ����,ִ�д���:%d",m_cbExcuteTimes);break;
						default:break;
					}
			}
			else
			{
					str.Format("����ʧ��!");
					m_cbExcuteTimes=0;
					m_cbWinArea=0;
			}
			break;
		}
	case ACK_RESET_CONTROL:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="���������ѽ���!";
				for(int nIndex=0;nIndex<8;nIndex++)
					((CButton*)GetDlgItem(IDC_RADIO_LT+nIndex))->SetCheck(0);
				if(m_cbExcuteTimes>0)
					((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(-1);
				m_cbWinArea=0;
				m_cbExcuteTimes=0;
			}
			break;
		}
	case ACK_PRINT_SYN:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="������ͬ�������ѽ���!";
				for(int nIndex=0;nIndex<8;nIndex++)
					((CButton*)GetDlgItem(IDC_RADIO_LT+nIndex))->SetCheck(0);

				m_cbWinArea=pResult->cbExtendData[0];
				m_cbExcuteTimes=pResult->cbExtendData[1];
				if(m_cbWinArea>0&&m_cbWinArea<=8)
					((CButton*)GetDlgItem(IDC_RADIO_LT+m_cbWinArea-1))->SetCheck(1);
				if(m_cbExcuteTimes>0&&m_cbExcuteTimes<=5)
					((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes-1);

				SetTimer(10,2000,0);
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


void CAdminControlDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	ShowWindow(SW_HIDE);
	//	CDialog::OnCancel();
}

void CAdminControlDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(10);
	CMD_S_CommandResult Result;
	Result.cbAckType=ACK_SET_WIN_AREA;
	Result.cbResult=CR_ACCEPT;
	ReqResult(&Result);
	CDialog::OnTimer(nIDEvent);
}
