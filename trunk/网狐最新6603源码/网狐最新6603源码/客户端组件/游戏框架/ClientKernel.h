#ifndef CLIENT_KERNEL_HEAD_FILE
#define CLIENT_KERNEL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "TimerPulseEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CWHArray<IUserEventSink *,IUserEventSink *> CUserEventSinkArray;

//////////////////////////////////////////////////////////////////////////////////

//�ں˿��
class CClientKernel : public CWnd, public IClientKernel, public IProcessControlSink, public IUserManagerSink
{
	//״̬����
protected:
	bool							m_bService;							//����״̬
	tagUserAttribute				m_UserAttribute;					//�û�����

	//ʱ����Ϣ
protected:
	WORD							m_wClockID;							//ʱ�ӱ�ʶ
	UINT							m_nElapseCount;						//ʱ�Ӽ���
	WORD							m_wClockChairID;					//ʱ��λ��

	//��Ϸ״̬
protected:
	bool							m_bAllowLookon;						//�����Թ�
	BYTE							m_cbGameStatus;						//��Ϸ״̬
	IClientUserItem	*				m_pIMySelfUserItem;					//�Լ���Ϣ

	//��Ϸ����
protected:
	tagGameAttribute				m_GameAttribute;					//��Ϸ����
	tagServerAttribute				m_ServerAttribute;					//��������

	//�ӿڱ���
protected:
	IStringMessage *				m_pIStringMessage;					//��Ϣ�ӿ�
	IClientKernelSink *				m_pIClientKernelSink;				//�ں˽ӿ�

	//�ں˱���
protected:
	HWND							m_hProcessWnd;						//ͨѶ����
	CUserEventSinkArray				m_UserEventSinkArray;				//�û��¼�

	//�������
protected:
	CTimerPulseEngine				m_TimerPulseEngine;					//ʱ������
	CProcessControlHelper			m_ProcessControlModule;				//���̴���
	CGameUserManagerHelper			m_GameUserManagerModule;			//�û�����

	//��������
public:
	//���캯��
	CClientKernel();
	//��������
	virtual ~CClientKernel();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//�ں�����
	virtual bool InitClientKernel(LPCTSTR lpszComLine);
	//���ýӿ�
	virtual bool SetUserEventSink(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool SetStringMessage(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool SetClientKernelSink(IUnknownEx * pIUnknownEx);
	//��Ϸ����
	virtual bool SetGameAttribute(WORD wKindID, WORD wPlayerCount, DWORD dwClientVersion, HICON hGameIcon, LPCTSTR pszGameName);

	//���ӽӿ�
public:
	//���Ӵ���
	virtual bool CreateConnect();
	//�ж�����
	virtual bool IntermitConnect();

	//���Խӿ�
public:
	//�û�����
	virtual tagUserAttribute * GetUserAttribute() { return &m_UserAttribute; }
	//��Ϸ����
	virtual tagGameAttribute * GetGameAttribute() { return &m_GameAttribute; }
	//��������
	virtual tagServerAttribute * GetServerAttribute() { return &m_ServerAttribute; }

	//ģʽ�ӿ�
public:
	//����ģʽ
	virtual bool IsSingleMode();
	//�Թ��û�
	virtual bool IsLookonMode();
	//�����Թ�
	virtual bool IsAllowLookon();
	//����״̬
	virtual bool IsServiceStatus();

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE GetGameStatus() { return m_cbGameStatus; };
	//����״̬
	virtual VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; };

	//�û��ӿ�
public:
	//�Լ�λ��
	virtual WORD GetMeChairID();
	//�Լ�λ��
	virtual IClientUserItem * GetMeUserItem();
	//��Ϸ�û�
	virtual IClientUserItem * GetTableUserItem(WORD wChariID);
	//�����û�
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID);
	//�����û�
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID);
	//�Թ��û�
	virtual IClientUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//����ӿ�
public:
	//���ͺ���
	virtual bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	virtual bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//���̽ӿ�
public:
	//��������
	virtual bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID);
	//��������
	virtual bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//ִ������
	virtual bool SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//���ܽӿ�
public:
	//����׼��
	virtual bool SendUserReady(VOID * pData, WORD wDataSize);
	//�Թۿ���
	virtual bool SendUserLookon(DWORD dwUserID, bool bAllowLookon);
	//���ͱ���
	virtual bool SendUserExpression(DWORD dwTargetUserID, WORD wItemIndex);
	//��������
	virtual bool SendUserChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);

	//�����ӿ�
public:
	//������
	virtual VOID ActiveGameFrame();
	//�л�����
	virtual WORD SwitchViewChairID(WORD wChairID);

	//ʱ�ӽӿ�
public:
	//ʱ�ӱ�ʶ
	virtual UINT GetClockID();
	//ʱ��λ��
	virtual WORD GetClockChairID();
	//ɾ��ʱ��
	virtual VOID KillGameClock(WORD wClockID);
	//����ʱ��
	virtual VOID SetGameClock(WORD wChairID, WORD wClockID, UINT nElapse);

	//�û�����
public:
	//�û�����
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//�û�ɾ��
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//�û�����
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);

	//���ݽӿ�
public:
	//��������
	virtual bool OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend);

	//���ݴ���
protected:
	//������Ϣ
	bool OnProcessMainSocket(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnProcessMainControl(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnProcessMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnProcessMainUserInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//�û�����
	bool OnSocketSubUserChat(VOID * pData, WORD wDataSize);
	//�û�����
	bool OnSocketSubExpression(VOID * pData, WORD wDataSize);
	//��Ϸ״̬
	bool OnSocketSubGameStatus(VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSocketSubGameScene(VOID * pData, WORD wDataSize);
	//�Թ�״̬
	bool OnSocketSubLookonStatus(VOID * pData, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);

	//�ڲ�����
protected:
	//���ñ���
	VOID SetGameFrameTitle();

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif