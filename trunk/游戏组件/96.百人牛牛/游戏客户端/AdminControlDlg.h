#pragma once
#include "Resource.h"
#define CONTROL_AREA 4

// CAdminControlDlg 对话框
#define IDM_ADMIN_COMMDN WM_USER+1000

class CAdminControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminControlDlg)
protected:
	bool m_bWinArea[CONTROL_AREA];
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;
public:
	//标准构造函数
	CAdminControlDlg(CWnd* pParent = NULL); 
	//析构函数
	virtual ~CAdminControlDlg();
	//重置界面
	void ReSetAdminWnd();
	//显示状态
	void PrintCurStatus();
	//允许控制
	VOID OnAllowControl(bool bEnable);
	//申请结果
	bool ReqResult(const void * pBuffer);
	// 对话框数据
	enum { IDD = IDD_DIALOG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
	afx_msg void  OnRadioClick();
protected:
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
};
