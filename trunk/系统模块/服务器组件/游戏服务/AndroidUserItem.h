#ifndef ANDROID_USER_TIEM_HEAD_FILE
#define ANDROID_USER_TIEM_HEAD_FILE

#pragma once

#include "GameServiceExport.h"

//////////////////////////////////////////////////////////////////////////

//ʱ������
struct tagTimerItem
{
	UINT							nTimerID;							//ʱ���ʶ
	UINT							nTimeLeave;							//ʣ��ʱ��
};

//��˵��
typedef CArrayTemplate<tagTimerItem *> CTimerItemArray;					//ʱ������

//////////////////////////////////////////////////////////////////////////

//��������Ϣ
class CAndroidUserItem : public IAndroidUserItem
{
	//��Ԫ����
	friend class CAndroidUserManager;

	//״̬����
protected:
	bool							m_bStartClient;						//��Ϸ��־
	BYTE							m_cbGameStatus;						//��Ϸ״̬
	tagUserStatus					m_CurrentUserStatus;				//�û�״̬

	//�󶨱���
protected:
	DWORD							m_dwUserID;							//�û���ʶ
	IServerUserItem *				m_pIServerUserItem;					//�û��ӿ�

	//��������
protected:
	WORD							m_wRountID;							//ѭ������
	WORD							m_wAndroidIndex;					//��������

	//ʱ�����
protected:
	CTimerItemArray					m_TimerItemActive;					//�����
	static CTimerItemArray			m_TimerItemStorage;					//�������

	//�ӿڱ���
protected:
	IServerUserManager *			m_pIServerUserManager;				//�û�����
	IAndroidUserManager *			m_pIAndroidUserManager;				//����ӿ�
	IAndroidUserItemSink *			m_pIAndroidUserItemSink;			//�ص��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItem();
	//��������
	virtual ~CAndroidUserItem();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE __cdecl GetGameStatus() { return m_cbGameStatus; }
	//����״̬
	virtual VOID __cdecl SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; }

	//�û���Ϣ
public:
	//��ȡ I D
	virtual DWORD __cdecl GetUserID();
	//���Ӻ���
	virtual WORD __cdecl GetTableID();
	//���Ӻ���
	virtual WORD __cdecl GetChairID();

	//�û��ӿ�
public:
	//��ȡ�Լ�
	virtual IServerUserItem * __cdecl GetMeUserItem();
	//��Ϸ�û�
	virtual IServerUserItem * __cdecl GetTableUserItem(WORD wChariID);

	//����ӿ�
public:
	//���ͺ���
	virtual bool __cdecl SendSocketData(WORD wSubCmdID);
	//���ͺ���
	virtual bool __cdecl SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//���ܽӿ�
public:
	//ɾ��ʱ��
	virtual bool __cdecl KillGameTimer(UINT nTimerID);
	//����ʱ��
	virtual bool __cdecl SetGameTimer(UINT nTimerID, UINT nElapse);
	//����׼��
	virtual bool __cdecl SendUserReady(VOID * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor);

	//�¼�֪ͨ
public:
	//ʱ���¼�
	bool OnTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter);
	//������Ϣ
	bool OnSocketRead(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//��¼ʧ��
	bool OnSocketSubLogonFailed(VOID * pData, WORD wDataSize);
	//��¼���
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//�û�״̬
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	bool OnSocketGameMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//���ƺ���
protected:
	//������Ϸ
	VOID StartGameClient();
	//�ر���Ϸ
	VOID CloseGameClient();

	//��������
public:
	//��λ����
	VOID RepositUserItem();
};

//////////////////////////////////////////////////////////////////////////

#endif