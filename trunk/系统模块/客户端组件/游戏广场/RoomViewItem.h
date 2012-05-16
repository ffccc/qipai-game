#pragma once

#include "Stdafx.h"
#include "IGamePlaza.h"
#include "TableFrame.h"
#include "UserManager.h"
#include "FindUserDlg.h"
#include "UserListView.h"
#include "SystemOption.h"
#include "ShortMessage.h"
#include "ManagerClass.h"
#include "ServerItemView.h"
#include "GameFrame.h"

//////////////////////////////////////////////////////////////////////////

//�������״̬
enum enServiceStatus
{
	ServiceStatus_Null,					//û��״̬
	ServiceStatus_Connecting,			//����״̬
	ServiceStatus_EfficacyUser,			//Ч���û�
	ServiceStatus_RecvConfigInfo,		//��������
	ServiceStatus_RecvRoomInfo,			//������Ϣ
	ServiceStatus_Serviceing,			//����״̬
	ServiceStatus_NetShutDown,			//�����ж�
};

//���ҽṹ
struct tagFindTable
{
	bool								bOneNull;							//һ����λ
	bool								bTwoNull;							//������λ
	bool								bAllNull;							//ȫ��λ��
	bool								bNotFull;							//��ȫ��λ
	bool								bFilterPass;						//��������
	WORD								wBeginTableID;						//��ʼ����
	WORD								wResultTableID;						//�������
	WORD								wResultChairID;						//�������
};

//���Ͷ���
typedef CArrayTemplate<tagPropertyInfo> CPropertyAttributeArrary;
typedef CArrayTemplate<tagFlowerInfo> CFlowerAttributeArrary;
//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define	WM_UPDATE_FACE					WM_USER+200							//����ͷ��

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CRoomViewItem : public CDialog, public IRoomViewItem, public ITCPSocketSink, public IPurchaseInfo,
	public ITableFrameSink, public IUserManagerSink, public ISplitterSink, public IChannelMessageSink
{
	friend class CFindUserDlg;
	friend class CShortMessage;

	//Ӧ�ñ���
protected:
	CListServer	*					m_pListServer;						//������Ϣ
	tagOptionBuffer					m_OptionBuffer;						//������Ϣ
	CMD_GR_ColumnInfo				m_ListColumnInfo;					//�б���Ϣ

	//��������
protected:
	WORD							m_wGameGenre;						//��Ϸ����
	DWORD							m_dwVideoAddr;						//��Ƶ��ַ
	BYTE							m_cbHideUserInfo;					//������Ϣ

	//��������
protected:
	bool							m_bRectifyHor;						//������־
	bool							m_bRectifyVor;						//������־
	bool							m_bInitDialog;						//��ʼ��־
	bool							m_bHideUserList;					//���ر�־
	DWORD							m_dwLimitChatTimes;					//������ʱ
	WORD							m_wReqTableID;						//��������
	WORD							m_wReqChairID;						//����λ��
	WORD							m_wFindTableID;						//����λ��
	DWORD							m_dwMenuUserID;						//�˵��û�

	//���ñ���
protected:
	CGameOption *					m_pGameOption;						//��Ϸ����
	CServerOption *					m_pServerOption;					//��������

	//�������
protected:
	IUserItem *						m_pMeUserItem;						//�Լ���Ϣ
	enServiceStatus					m_ServiceStatus;					//����״̬
	CClientUserManager				m_ClientUserManager;				//�û�����

	//������Ϣ
protected:
	HWND							m_hWndChannel;						//�ŵ�����
	HANDLE							m_hShareMemory;						//�����ڴ�
	TCHAR							m_szShareName[64];					//��������
	tagShareMemory *				m_pShareMemory;						//�����ڴ�
	PROCESS_INFORMATION				m_GameProcessInfo;					//������Ϣ

	//�������
protected:
	DWORD							m_dwChatUserID;						//�����û�
	TCHAR							m_szChatName[NAME_LEN];				//�����û�

	//����ؼ�
public:
	CComboBox						m_ChatInput;						//��������
	CComboBox						m_ChatObject;						//�������
	//CSkinButton						m_btPhrase;							//���ﰴť
	CSkinButton						m_btColorSet;						//��ɫ����
	CSkinButton						m_btCleanScreen;					//������ť
	CSkinButton						m_btSendChat;						//���Ͱ�ť
	CSkinButton						m_btExpression;						//���鰴ť
	CSkinButton						m_btBugle;							//���Ȱ�ť
	CExpression						m_Expression;						//���鴰��
	CSkinRichEdit					m_ChatMessage;						//�������

	//���ܿؼ�
public:
	CSkinButton						m_btBank;							//���а�ť
	CSkinButton						m_btAutoSit;						//���밴ť
	CSkinButton						m_btFindUser;						//���Ұ�ť
	CSkinButton						m_btQuitRoom;						//�˳���ť

	//�û��ؼ�
public:
	CWebBrowser						m_BrowerAD;							//�������
	CTableFrame						m_TableFrame;						//���ӿ��
	CUserListView					m_UserListView;						//�û��б�

	//����ؼ�
public:
	CManagerServer					m_ManagerServer;					//�������
	CManagerMessage					m_ManagerMessage;					//ϵͳ��Ϣ
	CManagerUserRight				m_ManagerUserRight;					//Ȩ�޹���
	CManagerSendWarning				m_ManagerSendWarning;				//�������

	//������Ϣ
protected:
	tagEncircleInfo					m_ImageInfoTable;					//������Ϣ
	tagEncircleInfo					m_ImageInfoList;					//�б���Ϣ
	tagEncircleInfo					m_ImageInfoChat;					//�Ŀ���Ϣ

	//��Դ����
protected:
	tagEncircleImage				m_EncircleTable;					//������Դ
	tagEncircleImage				m_EncircleList;						//�б���Դ
	tagEncircleImage				m_EncircleChat;						//�Ŀ���Դ

	//���ݻ���
protected:
	WORD							m_wPacketTableID;					//����λ��
	WORD							m_wPacketChairID;					//����λ��
	CDataQueue						m_PacketDataStorage;				//���ݻ���

	//�����ؼ�
public:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CSkinSplitter					m_HorSplitter;						//��ִ���
	CSkinSplitter					m_VorSplitter;						//��ִ���
	CIPCRecvCopyData				m_IPCRecvCopyData;					//�ŵ����
	CTCPSocketHelper				m_ClientSocket;						//��������
	CMessageProxyHelper				m_MessageProxyHelper;				//��Ϣ����
	CPropertyBar					m_PropertyBar;						//���߿ؼ�

	//��չ����
public:
	CPropertyAttributeArrary		m_PropertyAttributeArrary;			//������Ϣ
	CFlowerAttributeArrary			m_FlowerAttributeArrary;			//�ʻ���Ϣ

	//��������
public:
	//���캯��
	CRoomViewItem();
	//��������
	virtual ~CRoomViewItem();

	//���غ��� 
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; };
	//ȡ������
	virtual void OnCancel() { return; };
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���߽ӿ� 
public:
	//������
	virtual BYTE __cdecl GetLocation(){return LOCATION_PLAZA_ROOM;}
	//��������
	virtual WORD __cdecl GetGameGenre();
	//ö�����
	virtual tagUserData * __cdecl EnumLookonUser(WORD wIndex);
	//�����û�
	virtual tagUserData * __cdecl SearchUserItem(DWORD dwUserID);
	//�����û�
	virtual tagUserData * __cdecl SearchUserItem(LPCTSTR pszUserName);
	//��ȡ���
	virtual const tagUserData * __cdecl GetUserInfo(WORD wChairID);
	//��ȡ�Լ�
	virtual const tagUserData * __cdecl GetMeUserInfo();
	//������Ϣ
	virtual void __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);

	//��ͼ�ӿ�
public:
	//�ϰ��¼�
	void NotifyBossCome(bool bBossCome);
	//��ȡ���
	virtual HWND __cdecl GetWindowHandle() { return GetSafeHwnd(); }

	//����ӿ�
public:
	//�ر�ѯ��
	virtual bool __cdecl QueryCloseRoom();
	//�رշ���
	virtual void __cdecl CloseRoomViewItem();
	//������Ϣ
	virtual tagGameKind * __cdecl GetKindInfo();
	//������Ϣ
	virtual tagGameServer * __cdecl GetServerInfo();

	//������ӿ�
public:
	//��ť��Ϣ
	virtual bool __cdecl OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton);
	//�������Ϣ
	virtual bool __cdecl OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos);

	//�����¼�
public:
	//�����¼�
	virtual bool __cdecl OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize);

	//�ŵ��ӿ�
public:
	//�ŵ����ݴ���
	virtual bool __cdecl OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

	//���ӽӿ�
public:
	//����������
	virtual void __cdecl OnLButtonHitTable(WORD wTableID, WORD wChairID, bool bMirror);
	//����Ҽ�����
	virtual void __cdecl OnRButtonHitTable(WORD wTableID, WORD wChairID, bool bMirror);
	//���˫��
	virtual void __cdecl OnDoubleHitTable(WORD wTableID, bool bMirror);

	//�û��ӿ�
public:
	//�û�����
	virtual void __cdecl OnUserItemAcitve(IUserItem * pIUserItem);
	//�û�����
	virtual void __cdecl OnUserItemUpdate(IUserItem * pIUserItem);
	//�û�ɾ��
	virtual void __cdecl OnUserItemDelete(IUserItem * pIUserItem);

	//����������
protected:
	//��¼��Ϣ
	bool OnSocketMainLogon(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û���Ϣ
	bool OnSocketMainUser(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSocketMainInfo(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//״̬��Ϣ
	bool OnSocketMainStatus(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketMainSystem(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSocketMainServerInfo(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//��Ϸ��Ϣ
	bool OnSocketMainGameFrame(CMD_Command Command, void * pBuffer, WORD wDataSize);

	//����������
protected:
	//�û�����
	bool OnSocketSubUserCome(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û�״̬
	bool OnSocketSubStatus(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSocketSubScore(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û�Ȩ��
	bool OnSocketSubRight(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//��Ա�ȼ�
	bool OnSocketSubMemberOrder(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//����ʧ��
	bool OnSocketSubSitFailed(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSocketSubChat(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û�˽��
	bool OnSocketSubWisper(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�������
	bool OnSocketSubUserInvite(CMD_Command Command, void * pBuffer, WORD wDataSize);

	//IPC �¼�����
protected:
	//IPC �ں�����
	bool OnIPCKernel(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//IPC ��������
	bool OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

	//���ƺ���
public:
	//��ʼ������
	bool InitRoomViewItem(CListServer * pListServer);
	//���ӷ�����
	bool ConnectGameServer();
	//���͹���
	bool SendUserRule();
	//˽����Ϣ
	bool SendUserWhisper(IUserItem * pIUserItem);
	//�û��˵�
	bool ShowUserInfoMenu(IUserItem * pIUserItem, CPoint Point);

	//���ܺ���
public:
	//��Դ���
	HINSTANCE GetResInstanceHandle();
	//��ȡ��Դ
	tagServerViewImage GetServerViewRes();
	//������Դ
	bool UpdateSkinResource();
	//��Ϸ����
	CGameOption * GetGameOption() { return m_pGameOption; }
	//��������
	CServerOption * GetServerOption() { return m_pServerOption; }

	//��������
protected:
	//���͵�¼����
	bool SendLogonPacket();
	//������������
	bool SendOptionsPacket();
	//������������
	bool SendStandUpPacket();
	//����ǿ������
	bool SendLeftGamePacket();
	//���ͷ������
	bool SendUserRulePacket();
	//�����Թ�����
	bool SendLookonPacket(WORD wTableID, WORD wChairID, LPCTSTR pszTablePass);
	//������������
	bool SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR pszTablePass);
	//������������
	bool SendChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor);

	//��������
protected:
	//�Թ۶���
	bool PerformLookonAction(WORD wTableID, WORD wChairID);
	//���¶���
	bool PerformSitDownAction(WORD wTableID, WORD wChairID);

	//IPC ����
protected:
	//������Ϸ��Ϣ
	bool IPCSendGameInfo(CIPCSendCopyData * pSendCopyData);
	//�����û���Ϣ
	bool IPCSendTableUsers(CIPCSendCopyData * pSendCopyData);

	//�ڲ�����
protected:
	//������Ϸ
	int StartGameClient();
	//�ر���Ϸ
	void CloseGameClient();
	//�����û�
	bool SendTableUser(IUserItem * pIUserItem, CIPCSendCopyData * pSendCopyData);
	//������Ϣ
	bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	//��������
protected:
	//���±���
	void UpdateTitleText();
	//�滭�б�
	void DrawUserFrame(CDC * pDC);
	//�滭����
	void DrawChatFrame(CDC * pDC);
	//�滭����
	void DrawTableFrame(CDC * pDC);
	//����ͼ��
	void DrawLeftViewFrame(CDC * pDC);
	//�������
	bool SetChatObject(IUserItem * pIUserItem);
	//��������
	bool FindGameTable(tagFindTable & FindInfo);
	//�����ؼ�
	void RectifyControl(int nWidth, int nHeight);
	//��ʾ��Ϣ
    int ShowMessageBox(LPCTSTR pszMessage, UINT nType);
	//�ܷ�����
	bool CanSitDownTable(WORD wTableID, WORD wChairID, bool bLookon, bool bShowError);

	//��Ϣӳ��
protected:
	//�滭��Ϣ
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//��ť��Ϣ
protected:
	//���Ͱ�ť
	afx_msg void OnBnClickedSendChat();
	//���鰴ť
	afx_msg void OnBnClickedExpression();
	//��ɫ��ť
	afx_msg void OnBnClickedColorSet();
	//������ť
	afx_msg void OnBnClickedCleanScreen();
	//���Ȱ�ť
	afx_msg void OnBnClickedBugle();
	//��ʾ��Ϣ
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//˫������
	afx_msg void OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//�Ҽ��б�
	afx_msg void OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//����б�
	afx_msg void OnNMclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//����ͷ��
	LRESULT OnUpdateFace(WPARAM wParam, LPARAM lParam);
	//IPC ��Ϣ
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT * pCopyDataStruct);
	//������Ϣ
	LRESULT OnHitExpression(WPARAM wParam, LPARAM lParam);

	//���ܰ�ť
protected:
	//���а�ť
	afx_msg void OnBnClickedBank();
	//���밴ť
	afx_msg void OnBnClickedAutoSit();
	//���Ұ�ť
	afx_msg void OnBnClickedFindUser();
	//�˳���ť
	afx_msg void OnBnClickedQuitRoom();

	CPtrArray						m_ShortMessage;						//��Ϣ����
	CFindUserDlg					* m_pFindUserDlg;					//��

	//������Ϣ����
	CShortMessage * ActiveShortWnd(long int dwUserID, IUserItem * pUserItem, bool bCreate);


	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
