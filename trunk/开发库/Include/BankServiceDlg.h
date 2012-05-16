#ifndef IDC_BANK_SERVICE_DLG
#define IDC_BANK_SERVICE_DLG

#pragma once

#include "Stdafx.h"

#include "BankStorage.h"
//////////////////////////////////////////////////////////////////////////

//��½ TAB �ؼ�
class CTabCtrlBank : public CTabCtrl
{
	//��������
public:
	//���캯��
	CTabCtrlBank();
	//��������
	virtual ~CTabCtrlBank();

	//��Ϣ����
protected:
	//�ػ�����	
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//����
class CBankItemDlg : public CDialog
{
	//�ؼ�����
public:
	LONG							m_lInputCount;
	CString								m_strNickname;
	CString								m_strPassword;
	int									m_nSelectType;
	CBrush								m_brBakcground;
	DWORD								m_dwUserID;

	//�ӿڱ���
protected:	
	IClientKernel					* m_pIClientKernel;				//�ں˽ӿ�
	ITCPSocket						* m_pClientSocket;				//����ӿ�
	tagUserData						*m_pMeUserData;					//�û���Ϣ

	//��������
public:
	//���캯��
	CBankItemDlg();
	//��������
	virtual ~CBankItemDlg();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; };
	//ȡ����Ϣ
	virtual void OnCancel() { return; };

	//���ܺ���
public:
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserDatas);
	bool CheckInput();

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//��С��Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedTargetUser();
	afx_msg void OnBnClickedTargetUser2();

	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
};

//���з���
class CBankFillDlg : public CDialog
{
public:
	CString							m_strOriginPassword;
	CString							m_strNewPassword;
	CString							m_strConfirmPassword;

	//�ؼ�����
public:
	CBrush							m_brBakcground;

	//��������
public:
	//���캯��
	CBankFillDlg();
	//��������
	virtual ~CBankFillDlg();
	bool CheckInput();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
};
//////////////////////////////////////////////////////////////////////////

//���з���
class CBankServiceDlg : public CSkinDialogEx
{
	//��������
protected:
	CDialog	 *							m_pActiveItem;					//�������

	//�ؼ�����
public:
	CBankStorage						m_BankSavingDlg;				//������Ϸ��
	CBankDrawing						m_BankDrawingDlg;				//������Ϸ��
	CBankItemDlg						m_BankTransferDlg;				//ת�ʷ���
	CBankFillDlg						m_BankChangePassDlg;			//�޸�����

	CTabCtrlBank						m_TabServiceMode;				//����ѡ��

	//�ؼ�����
public:
	CSkinButton							m_btOK;							//ȷ����ť
	CSkinButton							m_btCancel;						//ȡ����ť

	tagUserData						*m_pMeUserData;					//�û���Ϣ

		//�ӿڱ���
public:	
	IClientKernel					* m_pIClientKernel;				//�ں˽ӿ�
	ITCPSocket						* m_pClientSocket;				//����ӿ�

	//��������
public:
	//���캯��
	CBankServiceDlg();
	//��������
	virtual ~CBankServiceDlg();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();

	//��Ϣ����
public:
	//���͸ı�
	afx_msg void OnTcnSelchangeServiceType(NMHDR * pNMHDR, LRESULT * pResult);

	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////



//�һ�����
extern "C" PROPERTY_MODULE_CLASS VOID __cdecl ShowBankStorageDlg(IClientKernel *pIClientKernel,ITCPSocket *pClientSocket,tagUserData *pUserData);

//////////////////////////////////////////////////////////////////////////

#endif