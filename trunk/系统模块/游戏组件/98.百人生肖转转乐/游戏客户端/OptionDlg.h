#pragma once
#include "Resource.h"

// COptionDlg dialog

class COptionDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionDlg)

protected:
	CSkinButton    m_btOK;
	CSkinButton    m_btCancel;

public:
	bool           m_bAllowGameSound;

public:
	COptionDlg(CWnd* pParent = NULL);
	virtual ~COptionDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnBnClickedAllowSound();
	DECLARE_MESSAGE_MAP()
};
