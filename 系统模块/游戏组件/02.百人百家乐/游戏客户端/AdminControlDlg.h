#pragma once
#include "Resource.h"

// CAdminControlDlg 对话框
#define IDM_ADMIN_COMMDN WM_USER+1000

class CAdminControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminControlDlg)
protected:
	BYTE m_cbWinArea;				//赢牌区域
	BYTE m_cbExcuteTimes;			//执行次数
	//界面控件
protected:
	CFont		m_btFont;
	CButton		m_btAdminControl[4];
public:
	CAdminControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdminControlDlg();
	//申请结果
	bool ReqResult(const void * pBuffer);
// 对话框数据
	enum { IDD = IDD_DIALOG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnTimer(UINT nIDEvent);
};
