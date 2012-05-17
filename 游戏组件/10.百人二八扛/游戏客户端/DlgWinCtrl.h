#pragma once
#include "Resource.h"
// CDlgWinCtrl 对话框

class CDlgWinCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgWinCtrl)

public:
	CDlgWinCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWinCtrl();

// 对话框数据
	enum { IDD = IDD_DIALOG_WIN_CTRL };
	bool SetWinRate(const void *pInfo, WORD size);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CMD_S_Win_Rate_Info		m_Info;
};
