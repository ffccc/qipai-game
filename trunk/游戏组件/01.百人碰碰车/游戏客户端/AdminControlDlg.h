#pragma once


// CAdminControlDlg �Ի���
#define IDM_ADMIN_COMMDN WM_USER+1000

class CAdminControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminControlDlg)

public:
	CAdminControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminControlDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//���¿���
	void UpdateControl(CMD_S_ControlReturns* pControlReturns);

protected:
	//��Ϣ
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);

public:
	//��ʼ��
	virtual BOOL OnInitDialog();
	//������ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ȡ������
	afx_msg void OnBnClickedButtonReset();
	//���ֿ���
	afx_msg void OnBnClickedButtonSyn();
	//��������
	afx_msg void OnBnClickedButtonOk();
	//ȡ���ر�
	afx_msg void OnBnClickedButtonCancel();

};
