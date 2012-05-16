#pragma once

#include "Stdafx.h"
#include "DlgLogon.h"
#include "GameFrame.h"
#include "UserInfoView.h"
#include "ServerItemView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define		WM_DOWN_LOAD_FACE		WM_USER+100				//������Ϣ

//�Զ���ͷ��
struct tagCustomFace
{
	DWORD						dwUserID;				//��� ID
	BYTE						*pFaceData;				//ͷ������
	DWORD						dwFaceSize;				//ͷ���С
	DWORD						dwCurrentSize;			//��ǰ��С

	//���캯��
	tagCustomFace():dwUserID(0),pFaceData(NULL),dwFaceSize(0),dwCurrentSize(0){}

	//����ڴ�
	void Clear(void)
	{
		delete[]pFaceData;
		pFaceData=NULL;
		dwUserID=0;
		dwFaceSize=0;
		dwCurrentSize=0;
	}

	//��������
	~tagCustomFace()
	{
		Clear();
	}

};

//������Ϣ
struct tagDownloadInfo
{
	DWORD						dwUserID;				//��� ID
	BYTE						bFaceVer;				//ͷ��汾
	BYTE						bReference;				//���ü���

	//��ʼ��
	tagDownloadInfo():dwUserID(0),bFaceVer(0),bReference(1){}
};

//���Ͷ���
typedef CArrayTemplate<tagDownloadInfo>			CDownloadInfoArrary;
//////////////////////////////////////////////////////////////////////////

//��Ϸ�㳡
class CPlazaViewItem : public CDialog, public IPlazaViewItem,public ITCPSocketSink
{
	//״̬����
public:
	bool						m_bInitDialog;						//��ʼ��־
	bool						m_bLogonPlaza;						//��¼��־

	//�ؼ�����
public:
	CDlgLogon					m_DlgLogon;							//��¼�Ի���
	CTCPSocketHelper			m_ClientSocket;						//��������

	//ͷ�����
protected:
	bool						m_bStartDownloadFace			;	//���ر�ʶ
	bool						m_bDownloadConnect;					//���ر�ʶ
	tagCustomFace				m_CustomFace;						//�Զ���ͷ��
	CDownloadInfoArrary			m_DownloadInfoArrary			;	//���ض���

	//������ť
public:
	CSkinButton					m_WEB_BEFORE;
	CSkinButton					m_WEB_BRUSH;
	CSkinButton					m_WEB_HOME;
	CSkinButton					m_WEB_NEXT;
	CSkinButton					m_WEB_STOP;

	//�ؼ�����
public:
	CHtmlBrower *				m_pHtmlBrower;						//�������
	CToolTipCtrl				m_ToolTipCtrl;						//��ʾ�ؼ�

	//������Ϣ
protected:
	tagEncircleInfo				m_ImageInfoWeb;						//��ҳ���

	//��Դ����
protected:
	tagEncircleImage			m_EncircleWeb;						//��ҳ���

	//��������
public:
	CPlazaViewItem();
	virtual ~CPlazaViewItem();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; };
	//ȡ������
	virtual void OnCancel() { return; };

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//��ͼ�ӿ�
public:
	//��ȡ���
	virtual HWND __cdecl GetWindowHandle() { return GetSafeHwnd(); }

	//�����¼�
public:
	//�����¼�
	virtual bool __cdecl OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize);

	//��Ϣ����
protected:
	//��¼��Ϣ
	bool OnSocketMainLogon(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�б���Ϣ
	bool OnSocketMainServerList(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketMainSystem(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û���Ϣ
	bool OnSocketMainUser(CMD_Command Command, void * pBuffer, WORD wDataSize);

	//�㳡�ӿ�
public:
	//��¼��Ϣ
	virtual bool __cdecl SendLogonMessage();
	//������Ϣ
	virtual bool __cdecl SendConnectMessage();

	//���ܺ���
public:
	//��ʾ��ҳ
	void WebBrowse(LPCTSTR lpszUrl, bool bForceBrowse);
	//��ʾ��Ϣ
    int ShowMessageBox(LPCTSTR pszMessage, UINT nType);
	//��������
	bool ConnectServer();
	//������Դ
	bool UpdateSkinResource();

	//�ڲ�����
private:
	//���±���
	void UpdateTitleText();
	//�滭��ҳ
	void DrawWebFrame(CDC * pDC);

	//��Ϣ����
protected:
	//�滭��Ϣ
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//��¼�㳡
	afx_msg void OnCommandLogon();
	//���ӷ�����
	afx_msg void OnCommandConnect();
	//ȡ������
	afx_msg void OnCommandCancelConnect();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��ʾ��Ϣ
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//������Ϣ
	LRESULT OnDownloadFace(WPARAM wParam, LPARAM lParam);

	//������ť
	afx_msg void OnBnClickedBEFORE();
	afx_msg void OnBnClickedNEXT();
	afx_msg void OnBnClickedMBRUSH();
	afx_msg void OnBnClickedSTOP();
	afx_msg void OnBnClickedHOME();
	afx_msg void OnBnClickedBBS();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
