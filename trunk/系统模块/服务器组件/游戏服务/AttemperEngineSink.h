#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerList.h"
#include "DataBaseSink.h"
#include "GameServiceExport.h"
#include "ServerUserManager.h"
#include "AndroidUserManager.h"

//////////////////////////////////////////////////////////////////////////
//�ṹ�嶨��

//���Ȳ���
struct tagAttemperSinkParameter
{
	//���ñ���
	DWORD							dwCenterServer;						//���ĵ�ַ
	tagGameServiceAttrib			* pGameServiceAttrib;				//��������
	tagGameServiceOption			* pGameServiceOption;				//������Ϣ

	//�������
	IAttemperEngine					* pIAttemperEngine;					//��������
	IGameServiceManager				* pIGameServiceManager;				//����ӿ�
};

//������Ϣ�ṹ
struct tagConnectItemInfo
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
	IServerUserItem					* pIServerUserItem;					//�û�ָ��
};

//����״��
struct tagTableUserStatus
{
	WORD							wLessUserCount;						//��������
	WORD							wTableUserCount;					//�û���Ŀ
	WORD							wTableAndroidCount;					//�û���Ŀ
};

//�û�״��
struct tagAndroidUserStatus
{
	WORD							wFreeUserCount;						//�û���Ŀ
	WORD							wPlayUserCount;						//�û���Ŀ
	WORD							wSitdownUserCount;					//�û���Ŀ
	IServerUserItem *				pServerUserFree[MAX_ANDROID];		//�û��ӿ�
	IServerUserItem *				pServerUserPlay[MAX_ANDROID];		//�û��ӿ�
	IServerUserItem *				pServerUserSitdown[MAX_ANDROID];	//�û��ӿ�
};

//���Ͷ���
typedef CArrayTemplate<tagPropertyInfo> CPropertyAttributeArrary;
typedef CArrayTemplate<tagFlowerInfo> CFlowerAttributeArrary;

//////////////////////////////////////////////////////////////////////////

//��˵��
class CTableFrame;

//�������湳��
class CAttemperEngineSink : public IAttemperEngineSink, public IGameServiceFrame
{
	//��Ԫ����
	friend class CGameService;

	//��������
protected:
	WORD							m_wMaxUserItem;						//����û�
	CTableFrame						* m_pTableFrame;					//����ָ��
	tagConnectItemInfo				* m_pNormalParameter;				//������Ϣ
	tagConnectItemInfo				* m_pAndroidParameter;				//������Ϣ

	//���ͳ���
protected:
	BYTE							m_cbSendLocation;					//���ͳ���

	//��Ϣ����
protected:
	BYTE							m_cbRoomMsgIndex;					//��Ϣ����			
	BYTE							m_cbGameMsgIndex;					//��Ϣ����			

	//���Ʊ���
protected:
	bool							m_bShallClose;						//�رձ�־
	bool							m_bAllowWisper;						//˽�ı�־
	bool							m_bAllowRoomChat;					//�����־
	bool							m_bAllowGameChat;					//�����־
	bool							m_bAllowEnterRoom;					//�����־
	bool							m_bAllowEnterGame;					//�����־

	//������Ϣ
protected:
	DWORD							m_dwCenterServer;					//���ĵ�ַ
	tagGameServiceAttrib			* m_pGameServiceAttrib;				//��������
	tagGameServiceOption			* m_pGameServiceOption;				//��������

	//��Ϸ���
protected:
	HINSTANCE						m_hDllInstance;						//��Ϸʵ��
	IGameServiceManager				* m_pIGameServiceManager;			//����ӿ�

	//�б����
protected:
	CMD_GR_ColumnInfo				m_ColumnInfo;						//�б����
	WORD							m_wColumnInfoSize;					//�б���

	//��������
protected:
	WORD							m_wOnLineInfoSize;					//��Ϣ����
	tagOnLineCountInfo				m_OnLineCountInfo[512];				//������Ϣ

	//�������
protected:
	CServerList						m_ServerList;						//�����б�
	CServerUserManager				m_ServerUserManager;				//�û�����
	CAndroidUserManager				m_AndroidUserManager;				//�û�����

	//�ӿڱ���
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	IAttemperEngine *				m_pIAttemperEngine;					//��������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	ITCPSocketService *				m_pITCPSocketCorrespond;			//Э������

	//��չ����
protected:
	CPropertyAttributeArrary		m_PropertyAttributeArrary;			//������Ϣ
	CFlowerAttributeArrary			m_FlowerAttributeArrary;			//�ʻ���Ϣ

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//״̬�ӿ�
public:
	//�رղ�ѯ
	virtual bool __cdecl IsShallClose() { return m_bShallClose; }
	//˽�Ĳ�ѯ
	virtual bool __cdecl IsAllowWisper() { return m_bAllowWisper; }
	//�����ѯ
	virtual bool __cdecl IsAllowRoomChat() { return m_bAllowRoomChat; }
	//�����ѯ
	virtual bool __cdecl IsAllowGameChat() { return m_bAllowGameChat; }
	//�����ѯ
	virtual bool __cdecl IsAllowEnterRoom() { return m_bAllowEnterRoom; }
	//�����ѯ
	virtual bool __cdecl IsAllowEnterGame() { return m_bAllowEnterGame; }

	//״̬֪ͨ
public:
	//����״̬
	virtual bool __cdecl SendTableStatus(WORD wTableID);
	//���ͷ���
	virtual bool __cdecl SendUserScore(IServerUserItem * pIServerUserItem);
	//����״̬
	virtual bool __cdecl SendUserStatus(IServerUserItem * pIServerUserItem);
	//���͵���
	virtual bool __cdecl SendResidualProperty(IServerUserItem * pIServerUserItem);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID);
	//��������
	virtual bool __cdecl SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);

	//��ʱ���ӿ�
public:
	//���ö�ʱ��
	virtual bool __cdecl SetTableTimer(WORD wTableID, DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam);
	//ɾ����ʱ��
	virtual bool __cdecl KillTableTimer(WORD wTableID, DWORD dwTimerID);

	//����ӿ�
public:
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual void __cdecl ExportInformation(LPCTSTR pszString, enTraceLevel TraceLevel);

	//��Ϣ����
public:
	//���͵�����Ϣ
	bool SendProMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType);
	//���ͷ�����Ϣ
	bool SendRoomMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType);
	//������Ϸ��Ϣ
	bool SendGameMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType);
	//���͵�����Ϣ
	bool SendProMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//���ͷ�����Ϣ
	bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//������Ϸ��Ϣ
	bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool __cdecl OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool __cdecl OnAttemperEngineStop(IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool __cdecl OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize) { return true; }
	//�Զ��¼�
	virtual bool __cdecl OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize) { return true; }

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool __cdecl OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool __cdecl OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool __cdecl OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPSocketRead(WORD wServiceID, CMD_Command Command, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool __cdecl OnEventTCPNetworkBind(DWORD dwClientIP, DWORD dwSocketID);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���ݿ⺯��
private:
	//�û���¼�ɹ�
	bool OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���¼ʧ��
	bool OnDBLogonError(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�Ȩ��
	bool OnDBUserRightResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ص���
	bool OnDBLoadProperty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ʻ�
	bool OnDBLoadFlower(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ҵ���
	bool OnDBUserProperty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	////�һ����
	//bool OnDBExchangeCharmResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ɼ���
	bool OnDBLadPropFinish(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���纯��
private:
	//��¼��Ϣ����
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�û���Ϣ����
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������Ϣ����
	bool OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�����Ϣ����
	bool OnSocketMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��Ϸ��Ϣ����
	bool OnSocketMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��Ƶ��Ϣ����
	bool OnSocketMainVideo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������Ϣ����
	bool OnSocketPresent(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������Ϣ����
	bool OnSocketBank(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��������
private:
	//����ʧ��
	bool SendLogonFailed(DWORD dwSocketID, LPCTSTR pszMessage, LONG lErrorCode);
	//�����û�
	bool SendUserItem(IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//������Ϣ
	bool SendGameServerInfo(IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//��������
	void SendPresentAttribute(IServerUserItem *pIServerUserItem);

	//��������
private:
	//�б���Ϣ����
	bool OnCenterMainServerList(CMD_Command Command, VOID * pData, WORD wDataSize);

	//��������
protected:
	//�󶨲���
	tagConnectItemInfo * GetBindParameter(WORD wBindIndex);
	//���û�
	IServerUserItem * GetServerUserItem(WORD wBindIndex);
	//�û�״��
	WORD GetAndroidUserStatus(tagAndroidUserStatus & AndroidUserStatus);
	//����״��
	WORD GetTableUserStatus(CTableFrame * pTableFrame, tagTableUserStatus & TableUserStatus);
	//���ʹ���
	bool OnEventFlower(const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//���ߴ���
	bool OnEventProperty(const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����һ�
	bool OnEventExchangeCharm(const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	bool OnEventBugle(const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//��ȡ�¼�
	bool OnEventBankDrawoutGold(const void * pData, WORD wDataSize, DWORD dwSocketID);
	//�洢�¼�
	bool OnEventBankStorage(const void * pData, WORD wDataSize, DWORD dwSocketID);

	//��������
protected:
	//ʹ��Ȩ��
	bool IsPropertyUseRight(INT nPropertyID,IServerUserItem *pIServerUserItem,IServerUserItem *pTargetUserItem);
	//�޸Ľ��
	void ModifyGameGold(IServerUserItem * pIServerUserItem,LONG lGameGold);
	//�޸��ʿ�
	void ModifyBankStorageGold(IServerUserItem * pIServerUserItem, LONG lStorageCount);
	//���ѽ��
	LONG GetBankStorageGold(IServerUserItem * pIServerUserItem);
};

//////////////////////////////////////////////////////////////////////////

#endif