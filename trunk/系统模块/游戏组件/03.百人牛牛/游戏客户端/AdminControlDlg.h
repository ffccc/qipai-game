#pragma once
#include "Resource.h"
#define CONTROL_AREA 4

// CAdminControlDlg �Ի���
#define IDM_ADMIN_COMMDN WM_USER+1000

class CAdminControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminControlDlg)
protected:
	bool m_bWinArea[CONTROL_AREA];
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;
public:
	//��׼���캯��
	CAdminControlDlg(CWnd* pParent = NULL); 
	//��������
	virtual ~CAdminControlDlg();
	//���ý���
	void ReSetAdminWnd();
	//��ʾ״̬
	void PrintCurStatus();
	//�������
	VOID OnAllowControl(bool bEnable);
	//������
	bool ReqResult(const void * pBuffer);
	// �Ի�������
	enum { IDD = IDD_DIALOG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
	afx_msg void  OnRadioClick();
protected:
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
};
