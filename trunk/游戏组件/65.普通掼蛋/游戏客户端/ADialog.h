#pragma once
#include "Resource.h"

// CADialog �Ի���

class CADialog : public CSkinDialogEx
{
	DECLARE_DYNAMIC(CADialog)

public:
	CADialog();   // ��׼���캯��
	virtual ~CADialog();

// �Ի�������
	enum { IDD = IDD_ANSWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_bCause;
	CString m_title;
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť
	virtual BOOL OnInitDialog();
};
