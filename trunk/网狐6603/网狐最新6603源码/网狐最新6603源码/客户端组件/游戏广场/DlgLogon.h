#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerControl.h"
#include "AccountsControl.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//��¼��ʽ
#define LOGON_BY_GAME_ID			1									//I D ��¼
#define LOGON_BY_ACCOUNTS			0									//�ʺŵ�¼

//////////////////////////////////////////////////////////////////////////////////

//�˺���Ϣ
struct tagAccountsInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szGameID[16];						//��Ϸ��ʶ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�˺�
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//���鶨��
typedef CWHArray<tagAccountsInfo *>	CAccountsInfoArray;					//�˺�����

//////////////////////////////////////////////////////////////////////////////////

//��¼����
class CDlgLogon : public CDialog
{
	//���ñ���
public:
	BYTE							m_cbLogonMode;						//��¼ģʽ
	BYTE							m_cbRemPassword;					//��ס����

	//��¼��Ϣ
public:
	DWORD							m_dwGameID;							//��Ϸ I D
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//��Ϸ�ʺ�
	TCHAR							m_szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							m_szLogonServer[LEN_SERVER];		//��¼��ַ

	//�ؼ�����
protected:
	CSkinButton						m_btQuit;							//�˳���ť
	CSkinButton						m_btEnter;							//��¼��ť
	CSkinButton						m_btOption;							//���ð�ť
	CSkinButton						m_btRegister;						//ע�ᰴť
	CSkinButton						m_btDeleteRecord;					//ɾ����ť

	//�ʺſؼ�
protected:
	CAccountsCtrl					m_GameIDControl;					//��¼ I D
	CAccountsCtrl					m_AccountsControl;					//��¼�ʺ�

	//��ť�ؼ�
protected:
	CSkinButton						m_btLogonLink1;						//���Ӱ�ť
	CSkinButton						m_btLogonLink2;						//���Ӱ�ť
	CSkinButton						m_btLogonLink3;						//���Ӱ�ť
	CSkinButton						m_btLogonLink4;						//���Ӱ�ť

	//����ؼ�
protected:
	CSkinTabCtrl					m_TabControl;						//��¼ģʽ
	CServerControl					m_ServerControl;					//��¼��ַ
	CPasswordControl				m_PasswordControl;					//�û�����

	//����ؼ�
protected:
	CBrush							m_brBrush;							//������ˢ
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��
	CPlatformPublicize				m_PlatformPublicize;				//����ؼ�

	//��������
protected:
	CAccountsInfoArray				m_AccountsInfoArray;				//�˺���Ϣ

	//��������
public:
	//���캯��
	CDlgLogon();
	//��������
	virtual ~CDlgLogon();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���ܺ���
public:
	//��������
	WORD ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize);

	//��������
private:
	//��ȡ��Ϣ
	bool GetInformation();
	//�����˺�
	VOID LoadAccountsInfo();
	//��ַ��Ϣ
	VOID LoadLogonServerInfo();

	//���Ӱ�ť
protected:
	//���Ӱ�ť
	VOID OnBnClickedLogonLink1();
	//���Ӱ�ť
	VOID OnBnClickedLogonLink2();
	//���Ӱ�ť
	VOID OnBnClickedLogonLink3();
	//���Ӱ�ť
	VOID OnBnClickedLogonLink4();

	//��ť��Ϣ
protected:
	//ע���ʺ�
	VOID OnBnClickedRegister();
	//��������
	VOID OnBnClickedProxyConfig();
	//ɾ����¼
	VOID OnBnClickedDeleteRecord();

	//�ؼ�����
protected:
	//ѡ��ı�
	VOID OnSelchangeGameID();
	//ѡ��ı�
	VOID OnSelchangeAccounts();
	//�༭�ı�
	VOID OnCbnEditchangeAccounts();
	//���͸ı�
	VOID OnTcnSelchangeLogonMode(NMHDR * pNMHDR, LRESULT * pResult);

	//ϵͳ��Ϣ
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//�ؼ���ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif