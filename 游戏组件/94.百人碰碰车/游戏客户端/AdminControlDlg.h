#pragma once


// CAdminControlDlg 对话框
#define IDM_ADMIN_COMMDN WM_USER+1000

class CAdminControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminControlDlg)

public:
	CAdminControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdminControlDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//更新控制
	void UpdateControl(CMD_S_ControlReturns* pControlReturns);

protected:
	//信息
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);

public:
	//初始化
	virtual BOOL OnInitDialog();
	//设置颜色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//取消控制
	afx_msg void OnBnClickedButtonReset();
	//本局控制
	afx_msg void OnBnClickedButtonSyn();
	//开启控制
	afx_msg void OnBnClickedButtonOk();
	//取消关闭
	afx_msg void OnBnClickedButtonCancel();

};
