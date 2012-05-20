#pragma once
#include "Resource.h"

// CAdminControlDlg �Ի���
#define IDM_ADMIN_COMMDN WM_USER+1000

class CAdminControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminControlDlg)
protected:
	BYTE m_cbWinArea;				//Ӯ������
	BYTE m_cbExcuteTimes;			//ִ�д���
	//����ؼ�
protected:
	CFont		m_btFont;
	CButton		m_btAdminControl[4];
public:
	CAdminControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminControlDlg();
	//������
	bool ReqResult(const void * pBuffer);
// �Ի�������
	enum { IDD = IDD_DIALOG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnTimer(UINT nIDEvent);
};
