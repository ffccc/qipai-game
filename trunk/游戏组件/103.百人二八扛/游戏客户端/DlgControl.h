#ifndef CONTROL_HEAD_FILE
#define CONTROL_HEAD_FILE

#include"resource.h"
#pragma once



class CDlgControl : public CDialog
{
	//��������
public:
	int							m_nRadioControlArea;							//�������
	int							m_nRadioControlBanker;							//�������
	BOOL						m_bControlBanker;
	BOOL						m_bControlArea;
	BOOL						m_bUserWin;
	BOOL						m_bUserLost;
	CString                     m_UserName;
public:
	CDlgControl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgControl();

// Dialog Data
	enum { IDD = IDD_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgControl)
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
};
#endif