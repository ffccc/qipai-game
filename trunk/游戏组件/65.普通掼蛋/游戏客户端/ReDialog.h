#pragma once

#include"Resource.h"

#define IDQUIT			100							//ǿ��
// CReDialog �Ի���

class CReDialog : public CSkinDialogEx
{
	DECLARE_DYNAMIC(CReDialog)

public:
	CReDialog();   // ��׼���캯��
	virtual ~CReDialog();

// �Ի�������
	enum { IDD = IDD_REQUEST };
public:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_bCause;
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
