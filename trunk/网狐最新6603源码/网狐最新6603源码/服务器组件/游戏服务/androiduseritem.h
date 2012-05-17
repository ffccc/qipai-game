#ifndef ANDROID_USER_TIEM_HEAD_FILE
#define ANDROID_USER_TIEM_HEAD_FILE

#pragma once

#include "GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ������
struct tagTimerItem
{
	UINT							nTimerID;							//ʱ���ʶ
	UINT							nTimeLeave;							//ʣ��ʱ��
};

//��˵��
typedef CWHArray<tagTimerItem *>	CTimerItemArray;					//ʱ������

//////////////////////////////////////////////////////////////////////////////////

//��������Ϣ
class GAME_SERVICE_CLASS CAndroidUserItem : public IAndroidUserItem
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
	DWORD							m_dwReposeTime;						//����ʱ��
	IServerUserItem *				m_pIServerUserItem;					//�û��ӿ�
	tagAndroidParameter				m_AndroidParameter;					//������Ϣ

	//��������
protected:
	WORD							m_wRoundID;							//ѭ������
	WORD							m_wAndroidIndex;					//��������

	//ʱ�����
protected:
	CTimerItemArray					m_TimerItemActive;					//�����
	static CTimerItemArray			m_TimerItemBuffer;					//�������

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
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�û���Ϣ
public:
	//��ȡ I D
	virtual DWORD GetUserID();
	//���Ӻ���
	virtual WORD GetTableID();
	//���Ӻ���
	virtual WORD GetChairID();

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE GetGameStatus() { return m_cbGameStatus; }
	//����״̬
	virtual VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; }

	//������Ϣ
public:
	//��ȡ����
	virtual tagAndroidParameter * GetAndroidParameter() { return &m_AndroidParameter; }

	//�û��ӿ�
public:
	//��ȡ�Լ�
	virtual IServerUserItem * GetMeUserItem();
	//��Ϸ�û�
	virtual IServerUserItem * GetTableUserItem(WORD wChariID);

	//���нӿ�
public:
	//������
	virtual bool PerformSaveScore(SCORE lScore);
	//��ȡ���
	virtual bool PerformTakeScore(SCORE lScore);

	//����ӿ�
public:
	//���ͺ���
	virtual bool SendSocketData(WORD wSubCmdID);
	//���ͺ���
	virtual bool SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//���ܽӿ�
public:
	//ɾ��ʱ��
	virtual bool KillGameTimer(UINT nTimerID);
	//����ʱ��
	virtual bool SetGameTimer(UINT nTimerID, UINT nElapse);
	//����׼��
	virtual bool SendUserReady(VOID * pData, WORD wDataSize);
	//��������
	virtual bool SendChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);

	//�¼�֪ͨ
public:
	//ʱ���¼�
	bool OnTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter);
	//������Ϣ
	bool OnSocketRead(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//��¼ʧ��
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//��¼���
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//�û�״̬
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
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

//////////////////////////////////////////////////////////////////////////////////

#endif