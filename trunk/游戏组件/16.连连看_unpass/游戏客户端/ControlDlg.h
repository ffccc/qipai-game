#pragma once


// CControlDlg �Ի���

class CControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CControlDlg)

	CSkinImage			m_ImageBg;

public:
	CControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CControlDlg();

// �Ի�������
	enum { IDD = IDD_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};
