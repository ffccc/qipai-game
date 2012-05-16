#ifndef CLIENT_KERNEL_HEAD_FILE
#define CLIENT_KERNEL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameFrame.h"

//////////////////////////////////////////////////////////////////////////

//��˵��
typedef CFactoryTemplate<tagUserData>	CUserItemFactory;
typedef CArrayTemplate<tagUserData *>	CUserItemPtrArray;

//////////////////////////////////////////////////////////////////////////

//�ͻ����ں�
class CClientKernel : public CWnd, public IClientKernel, public IChannelMessageSink
{
	//������Ϣ
protected:
	WORD								m_wTableID;						//���Ӻ���
	WORD								m_wChairID;						//���Ӻ���

	//������Ϣ
protected:
	DWORD								m_dwUserID;						//�û� I D
	tagServerAttribute					m_ServerAttribute;				//��������

	//��Ϸ��Ϣ
protected:
	bool								m_bInquire;						//�ر�ѯ��
	bool								m_bLookonMode;					//�Թ�ģʽ
	BYTE								m_bGameStatus;					//��Ϸ״̬

	//������Ϣ
protected:
	bool								m_bAllowLookon;					//�����Թ�
	bool								m_bAllowUserLookon;				//�����Թ�

	//��ʱ����Ϣ
private:
	UINT								m_nTimerID;						//��ʱ�� ID
	UINT								m_nElapseCount;					//��ʱ������
	WORD								m_wTimerChairID;				//������λ��
	HWND								m_hWndGameFrame;				//��ܾ��

	//�û���Ϣ
protected:
	tagUserData							* m_pMeUserItem;				//�Լ���Ϣ
	tagUserData							* m_pUserItem[MAX_CHAIR];		//�����Ϣ
	CUserItemPtrArray					m_UserItemLookon;				//�Թ�����
	CUserItemFactory					m_UserItemFactory;				//�û�����

	//ͨѶ����
protected:
	HANDLE								m_hShareMemory;					//������
	tagShareMemory						* m_pShareMemory;				//�����ڴ�
	CChannelServiceHelper				m_ChannelServiceHelper;			//�ŵ�����

	//�ӿ�ָ��
protected:
	IMessageProxy						* m_pIMessageProxy;				//��Ϣ����
	IClientKernelSink					* m_pIClientKernelSink;			//���ӽӿ�

	//��������
public:
	//���캯��
	CClientKernel();
	//��������
	virtual ~CClientKernel();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�����ӿ�
public:
	//���ýӿ�
	virtual bool __cdecl SetMessageProxy(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool __cdecl DestroyClientKernel();
	//�ں�����
	virtual bool __cdecl InitClientKernel(LPCTSTR lpszComLine, IUnknownEx * pIUnknownEx);

	//��Ϣ�ӿ�
public:
	//�뿪״̬
	virtual bool __cdecl GetExitFlags();
	//�Թ�״̬
	virtual bool __cdecl IsLookonMode() { return m_bLookonMode; }
	//�����Թ�
	virtual bool __cdecl IsAllowLookon() { return m_bAllowLookon; }
	//��������
	virtual const tagServerAttribute * __cdecl GetServerAttribute() { return &m_ServerAttribute; }

	//�Թۿ���
public:
	//�Թ�����
	virtual bool __cdecl IsAllowUserLookon()  { return m_bAllowUserLookon; }
	//�Թۿ���
	virtual bool __cdecl AllowUserLookon(DWORD dwUserID, bool bAllowLookon);

	//״̬�ӿ�
public:
	//��Ϸ״̬
	virtual BYTE __cdecl GetGameStatus() { return m_bGameStatus; }
	//��Ϸ״̬
	virtual void __cdecl SetGameStatus(BYTE bGameStatus) { m_bGameStatus=bGameStatus; }

	//�û��ӿ�
public:
	//�Լ�λ��
	virtual WORD __cdecl GetMeChairID() { return m_wChairID; }
	//��ȡ�Լ�
	virtual const tagUserData * __cdecl GetMeUserInfo() { return m_pMeUserItem; }
	//��ȡ���
	virtual const tagUserData * __cdecl GetUserInfo(WORD wChairID);

	//���ͽӿ�
public:
	//��ʾ��Ϣ
	virtual int __cdecl ShowMessageBox(LPCTSTR pszMessage, UINT nType);
	//���ͺ���
	virtual bool __cdecl SendSocketData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	virtual bool __cdecl SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendProcessData(WORD wMainCmdID, WORD wSubCmdID);
	//��������
	virtual bool __cdecl SendProcessData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	//���ܽӿ�
public:
	//����׼��
	virtual bool __cdecl SendUserReady(void * pBuffer, WORD wDataSize);
	//��������
	virtual bool __cdecl SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor);

	//��ʱ���ӿ�
public:
	//��ʱ��λ��
	virtual WORD __cdecl GetTimeChairID() { return m_wTimerChairID; }
	//���ö�ʱ��
	virtual bool __cdecl SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse);
	//ɾ����ʱ��
	virtual bool __cdecl KillGameTimer(UINT nTimerID);

	//�ŵ��ӿ�
public:
	//�ŵ����ݴ���
	virtual bool __cdecl OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

	//IPC ������
private:
	//������Ϣ
	bool OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//������Ϣ
	bool OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//�û�֪ͨ
	bool OnIPCUser(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//����֪ͨ
	bool OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

	//�û�����
private:
	//�����û�
	virtual tagUserData * __cdecl SearchUserItem(DWORD dwUserID);
	//ö�����
	virtual tagUserData * __cdecl EnumLookonUser(WORD wIndex);
	//�����û�
	bool ActiveUserItem(const tagUserData * pUserData);
	//ɾ���û�
	bool DeleteUserItem(DWORD dwUserID);
	//�����û�
	bool UpdateUserItem(DWORD dwUerID, const tagUserScore * pUserScore);
	//�����û�
	bool UpdateUserItem(DWORD dwUserID, BYTE cbUserStatus, WORD wNetDelay);

	//�ڲ�����
private:
	//���±���
	void UpdateGameTitle();
	//�ں�����
	void ResetClientKernel();

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif