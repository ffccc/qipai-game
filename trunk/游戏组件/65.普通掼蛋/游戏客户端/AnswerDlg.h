#pragma once
#include "Resource.h"

// CAnswerDlg �Ի���

class CAnswerDlg : public CSkinDialogEx
{
	DECLARE_DYNAMIC(CAnswerDlg)

public:
	CAnswerDlg();   // ��׼���캯��
	virtual ~CAnswerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_answer;
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť
	virtual BOOL OnInitDialog();
};
