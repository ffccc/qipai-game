#pragma once
#include "Resource.h"
// CDlgWinCtrl �Ի���

class CDlgWinCtrl : public CDialog
{
	DECLARE_DYNAMIC(CDlgWinCtrl)

public:
	CDlgWinCtrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWinCtrl();

// �Ի�������
	enum { IDD = IDD_DIALOG_WIN_CTRL };
	bool SetWinRate(const void *pInfo, WORD size);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CMD_S_Win_Rate_Info		m_Info;
};
