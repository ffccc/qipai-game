#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink, public IGameDataBaseEngine
{
	//��Ԫ����
	friend class CServiceUnits;

	//��������
protected:
	DBO_GR_LogonFailure				m_LogonFailure;						//��¼ʧ��
	DBO_GR_LogonSuccess				m_LogonSuccess;						//��¼�ɹ�

	//��Ϸ���ݿ�
protected:
	CDataBaseAide					m_GameDBAide;						//��Ϸ����
	CDataBaseHelper					m_GameDBModule;						//��Ϸ����

	//������ݿ�
protected:
	CDataBaseAide					m_TreasureDBAide;					//������ݿ�
	CDataBaseHelper					m_TreasureDBModule;					//������ݿ�

	//���ñ���
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagGameParameter *				m_pGameParameter;					//���ò���
	tagDataBaseParameter *			m_pDataBaseParameter;				//������Ϣ
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//�������
protected:
	IDataBaseEngine *				m_pIDataBaseEngine;					//����ӿ�
	IGameServiceManager *			m_pIGameServiceManager;				//�������
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//��ѯ�ӿ�
protected:
	IGameDataBaseEngineSink *		m_pIGameDataBaseEngineSink;			//���ݽӿ�

	//��������
public:
	//���캯��
	CDataBaseEngineSink();
	//��������
	virtual ~CDataBaseEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ò���
public:
	//�Զ�����
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//��������
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//��������
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }

	//��ȡ����
public:
	//��ȡ����
	virtual VOID * GetDataBase(REFGUID Guid, DWORD dwQueryVer);
	//��ȡ����
	virtual VOID * GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//Ͷ�ݽ��
	virtual bool PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����¼�
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��¼����
protected:
	//I D ��¼
	bool OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//I D ��¼
	bool OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺŵ�¼
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//ϵͳ����
protected:
	//��Ϸд��
	bool OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�뿪����
	bool OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ��¼
	bool OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ز���
	bool OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����б�
	bool OnRequestLoadGameColumn(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ػ���
	bool OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ص���
	bool OnRequestLoadGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���з���
protected:
	//������
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ����
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��������
private:
	//��¼���
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//���н��
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient);
};

//////////////////////////////////////////////////////////////////////////////////

#endif