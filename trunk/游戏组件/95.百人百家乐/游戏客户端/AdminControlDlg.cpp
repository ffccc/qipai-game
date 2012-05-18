// AdminControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AdminControlDlg.h"
#include ".\admincontroldlg.h"


// CAdminControlDlg 对话框

IMPLEMENT_DYNAMIC(CAdminControlDlg, CDialog)
CAdminControlDlg::CAdminControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminControlDlg::IDD, pParent)
{
	m_cbWinArea=0;				//赢牌区域
	m_cbExcuteTimes=0;			//执行次数
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


// CAdminControlDlg 消息处理程序

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


	//m_btAdminControl[0].SetWindowText("重置");
	//m_btAdminControl[1].SetWindowText("重置");
	//m_btAdminControl[2].SetWindowText("重置");
	//m_btAdminControl[3].SetWindowText("重置");

	SetDlgItemText(IDC_STATIC_TIMES,"控制局数:");
	SetDlgItemText(IDC_STATIC_AREA, "区域控制:");
	SetDlgItemText(IDC_STATIC_NOTIC,"控制说明:");
	SetDlgItemText(IDC_STATIC_TEXT,"区域输赢控制比游戏库存控制策略优先!");
	SetDlgItemText(IDC_RADIO_LT,"闲对子");
	SetDlgItemText(IDC_RADIO_LC,"  闲  ");
	SetDlgItemText(IDC_RADIO_LB,"闲天王");
	SetDlgItemText(IDC_RADIO_CC,"  平  ");
	SetDlgItemText(IDC_RADIO_CB,"同点平");
	SetDlgItemText(IDC_RADIO_RT,"庄对子");
	SetDlgItemText(IDC_RADIO_RC,"  庄  ");
	SetDlgItemText(IDC_RADIO_RB,"庄天王");

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("1");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("2");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("3");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("4");
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString("5");


	SetDlgItemText(IDC_BUTTON_RESET,"取消控制");
	SetDlgItemText(IDC_BUTTON_SYN,"本局");
	SetDlgItemText(IDC_BUTTON_OK,"执行");
	SetDlgItemText(IDC_BUTTON_CANCEL,"取消");

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
		AfxMessageBox("请选择受控次数以及受控区域!");
	}
	// TODO: 在此添加控件通知处理程序代码
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
						case 0:str.Format("无受控区域");break;
						case 1:str.Format("请求已接受,胜利区域:闲对子,执行次数:%d",m_cbExcuteTimes);break;
						case 2:str.Format("请求已接受,胜利区域:  闲  ,执行次数:%d",m_cbExcuteTimes);break;
						case 3:str.Format("请求已接受,胜利区域:闲天王,执行次数:%d",m_cbExcuteTimes);break;
						case 4:str.Format("请求已接受,胜利区域:  平  ,执行次数:%d",m_cbExcuteTimes);break;
						case 5:str.Format("请求已接受,胜利区域:同点平,执行次数:%d",m_cbExcuteTimes);break;
						case 6:str.Format("请求已接受,胜利区域:庄对子,执行次数:%d",m_cbExcuteTimes);break;
						case 7:str.Format("请求已接受,胜利区域:  庄  ,执行次数:%d",m_cbExcuteTimes);break;
						case 8:str.Format("请求已接受,胜利区域:庄天王,执行次数:%d",m_cbExcuteTimes);break;
						default:break;
					}
			}
			else
			{
					str.Format("请求失败!");
					m_cbExcuteTimes=0;
					m_cbWinArea=0;
			}
			break;
		}
	case ACK_RESET_CONTROL:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="重置请求已接受!";
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
				str="服务器同步请求已接受!";
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
				str.Format("请求失败!");
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
	// TODO: 在此添加专用代码和/或调用基类
	ShowWindow(SW_HIDE);
	//	CDialog::OnCancel();
}

void CAdminControlDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(10);
	CMD_S_CommandResult Result;
	Result.cbAckType=ACK_SET_WIN_AREA;
	Result.cbResult=CR_ACCEPT;
	ReqResult(&Result);
	CDialog::OnTimer(nIDEvent);
}
