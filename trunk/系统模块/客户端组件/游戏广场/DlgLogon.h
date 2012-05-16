#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//��¼ģʽ
enum enLogonMode
{
	LogonMode_Accounts,			//�ʺŵ�¼
	LogonMode_UserID,			//I D ��¼
};

//ע�������
#define REG_OPTION_LOGON		TEXT("OptionLogon")
#define REG_USER_INFO			TEXT("Software\\WHTZRWQPGame\\GamePlaza\\UserInfo")
#define REG_LOGON_SERVER		TEXT("Software\\WHTZRWQPGame\\GamePlaza\\LogonServer")

//////////////////////////////////////////////////////////////////////////

//��¼ TAB �ؼ�
class CTabCtrlLogon : public CTabCtrl
{
	//��������
public:
	//���캯��
	CTabCtrlLogon();
	//��������
	virtual ~CTabCtrlLogon();

	//��Ϣ����
protected:
	//�ػ�����	
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//�û�ע��
class CDlgRegister : public CSkinDialogEx
{
	friend class CDlgLogon;

	//��¼��Ϣ
public:
	WORD								m_wFaceID;						//ͷ���ʶ
	BYTE								m_cbGender;						//�û��Ա�
	TCHAR								m_szSpreader[NAME_LEN];			//��������
	TCHAR								m_szAccounts[NAME_LEN];			//��Ϸ�ʺ�
	TCHAR								m_szPassword[PASS_LEN];			//��Ϸ����
	TCHAR								m_szPasswordBank[PASS_LEN];		//��Ϸ����

	//�ؼ�����
public:
	CImageList							m_ImageList;
	CComboBoxEx							m_FaceSelect;
	CSkinButton							m_btLogon;						//��¼��ť
	CSkinButton							m_btCancel;						//ȡ����ť
	CSkinHyperLink						m_LineRegWeb;					//��վע��
	CSkinHyperLink						m_LineMainPage;					//��Ϸ��ҳ
	CSkinHyperLink						m_LinePassWord;					//���뱣��

	//��������
public:
	//���캯��
	CDlgRegister();
	//��������
	virtual ~CDlgRegister();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//��¼�Ի���
class CDlgLogon : public CSkinDialogEx
{
	friend class CRoomViewItem;
	friend class CPlazaViewItem;

	//��¼��Ϣ
protected:
	WORD								m_wFaceID;						//ͷ���ʶ
	BYTE								m_cbGender;						//�û��Ա�
	DWORD								m_dwUserID;						//�û� I D
	TCHAR								m_szSpreader[NAME_LEN];			//�ƹ�����
	TCHAR								m_szAccounts[NAME_LEN];			//��Ϸ�ʺ�
	TCHAR								m_szPassword[PASS_LEN];			//��Ϸ����
	TCHAR								m_szPasswordBank[PASS_LEN];		//��Ϸ����

	//λ�ñ���
protected:
	INT									m_nFullWidth;					//ȫ�����
	INT									m_nFullHeight;					//ȫ���߶�
	bool								m_bNetOption;					//��������
	CRect								m_rcNormalFrame;				//���λ��

	//��¼ģʽ
protected:
	enLogonMode							m_LogonMode;					//��¼ģʽ

	//������Ϣ
protected:
	bool								m_bRegister;					//ע���־
	bool								m_bChangePassWord;				//�ı��־

	//������Ϣ
protected:
	CString								m_strLogonServer;				//��������ַ

	//��ť����
public:
	CSkinButton							m_btLogon;						//��¼��ť
	CSkinButton							m_btCancel;						//ȡ����ť
	CSkinButton							m_btDelete;						//ɾ����ť
	CSkinButton							m_btRegister;					//ע�ᰴť
	CSkinButton							m_btNetOption;					//���簴ť
	CSkinButton							m_btProxyTest;					//���԰�ť

	//�ؼ�����
public:
	CWebBrowser							m_BrowerAD;						//�������
	CTabCtrlLogon						m_TabLogonMode;					//��¼ѡ��
	CSkinHyperLink						m_LineMainPage;					//��Ϸ��ҳ
	CSkinHyperLink						m_LinePassWord;					//���뱣��
	CSkinHyperLink						m_LineGetPassWord;				//ȡ������

	//�༭�ؼ�
protected:
	CSkinEdit							m_edProxyPort;					//����˿�
	CSkinEdit							m_edProxyServer;				//�����ַ
	CSkinEdit							m_edProxyUserName;				//�����û�
	CSkinEdit							m_edProxyPassword;				//��������
	CComboBox							m_cmProxyServerType;			//��������

	//��������
public:
	//���캯��
	CDlgLogon();
	//��������
	virtual ~CDlgLogon();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//���ܺ���
public:
	//��ȡ������
	LPCTSTR GetLogonServer() { return m_strLogonServer; }
	//���͵�¼��
	bool SendLogonPacket(ITCPSocket * pIClientSocke);
	//��¼�ɹ�����
	bool OnLogonSuccess();

	//�ڲ�����
private:
	//���ط�����
	void LoadLogonServer();
	//��ȡ�ʺ�
	void LoadAccountsInfo();
	//������Ϣ
	void LoadProxyServerInfo();
	//Ч������
	bool CheckLogonInput(bool bShowError);
	//����ģʽ
	void SetLogonMode(enLogonMode LogonMode);

	//��������
private:
	//��������
	void UpdateUserPassWord(DWORD dwUserDBID);
	//����ģʽ
	void SwitchNetOption(bool bNetOption);
	//����ѡ��
	void UpdateUserComboBox(UINT uComboBoxID);
	//�����ַ�
	int ComboBoxFindString(CComboBox * pComboBox, LPCTSTR pszFindString);
	//��ȡ����
	void GetProxyInfo(enProxyServerType &ProxyServerType, tagProxyServerInfo &ProxyServerInfo);
	//�����ж�
	bool EnableProxy() { return m_bNetOption;}

	//��Ϣ����
public:
	//ע���ʺ�
	afx_msg void OnRegisterAccounts();
	//ɾ���û�
	afx_msg void OnDeleteAccounts();
	//��������
	afx_msg void OnBnClickedNetOption();
	//�������
	afx_msg void OnBnClickedProxyTest();
	//����ı�
	afx_msg void OnEnChangePassword();
	//ѡ��ı�
	afx_msg void OnSelchangeAccounts();
	//ѡ��ı�
	afx_msg void OnSelchangeUserID();
	//���͸ı�
	afx_msg void OnTcnSelchangeLogonType(NMHDR * pNMHDR, LRESULT * pResult);
	//�ػ���Ϣ
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
