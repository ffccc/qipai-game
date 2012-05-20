#pragma once
#include "Resource.h"

// CSaveChessDlg �Ի���

class CSaveChessDlg : public CDialog
{

public:
	CSaveChessDlg(CWnd * pParent=NULL);   // ��׼���캯��
	virtual ~CSaveChessDlg();

	// �Ի�������
	enum { IDD = IDD_SAVE_CHESS_DLG };
	CButton				m_Ok;
	CButton				m_Cancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	short m_iClose;
};
