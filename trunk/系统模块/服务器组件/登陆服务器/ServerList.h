#ifndef SERVER_LIST_HEAD_FILE
#define SERVER_LIST_HEAD_FILE

#pragma once

#include "Commonality.h"

//////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CArrayTemplate<tagGameType *> CGameTypePtrArray;
typedef CArrayTemplate<tagGameKind *> CGameKindPtrArray;
typedef CArrayTemplate<tagGameServer *> CGameServerPtrArray;
typedef CArrayTemplate<tagGameStation *> CGameStationPtrArray;

//////////////////////////////////////////////////////////////////////////

//�б���Ϣ
class CServerListInfo
{
	//��������
public:
	CGameTypePtrArray				m_GameTypePtrArrayActive;			//��������
	CGameTypePtrArray				m_GameTypePtrArrayBuffer;			//��������
	CGameKindPtrArray				m_GameKindPtrArrayActive;			//��������
	CGameKindPtrArray				m_GameKindPtrArrayBuffer;			//��������
	CGameServerPtrArray				m_GameServerPtrArrayActive;			//��������
	CGameServerPtrArray				m_GameServerPtrArrayBuffer;			//��������
	CGameStationPtrArray			m_GameStationPtrArrayActive;		//վ������
	CGameStationPtrArray			m_GameStationPtrArrayBuffer;		//վ������

	//��������
public:
	//���캯��
	CServerListInfo();
	//��������
	virtual ~CServerListInfo();

	//ά������
public:
	//������Ϣ
	bool ResetInfo();
	//��������
	bool InsertGameType(tagGameType * pGameType);
	//��������
	bool InsertGameKind(tagGameKind * pGameKind);
	//����վ��
	bool InsertGameStation(tagGameStation * pGameStation);
	//���뷿��
	bool InsertGameServer(tagGameServer * pGameServer);
	//������Ϣ
	bool CopyServerListInfo(CServerListInfo & ServerListInfoScr);
};

//////////////////////////////////////////////////////////////////////////

//�б����
class CServerList
{
	//��������
protected:
	CServerListInfo					m_ServerListInfoBuffer;				//�б���Ϣ
	CServerListInfo					m_ServerListInfoActive;				//�б���Ϣ
	BYTE							m_cbSendBuffer[SOCKET_PACKET];		//���ͻ���

	//�ӿ�ָ��
protected:
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������

public:
	//���캯��
	CServerList();
	//��������
	virtual ~CServerList();

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool __cdecl SetSocketEngine(IUnknownEx * pIUnknownEx);

	//���ͽӿ�
public:
	//��������
	virtual bool __cdecl SendGameTypeList(DWORD dwSocketID);
	//��������
	virtual bool __cdecl SendGameKindList(DWORD dwSocketID);
	//����վ��
	virtual bool __cdecl SendGameStationList(DWORD dwSocketID);
	//����վ��
	virtual bool __cdecl SendGameStationList(DWORD dwSocketID, WORD wKindID);
	//���ͷ���
	virtual bool __cdecl SendGameServerList(DWORD dwSocketID);
	//���ͷ���
	virtual bool __cdecl SendGameServerList(DWORD dwSocketID, WORD wKindID);

	//���½ӿ�
public:
	//����׼��
	virtual bool __cdecl ResetServerListBuffer();
	//�����
	virtual bool __cdecl ActiveServerListBuffer();
	//׷������
	virtual bool __cdecl AppendGameTypeBuffer(tagGameType * pGameType, DWORD dwCount);
	//׷������
	virtual bool __cdecl AppendGameKindBuffer(tagGameKind * pGameKind, DWORD dwCount);
	//׷��վ��
	virtual bool __cdecl AppendGameStationBuffer(tagGameStation * pGameStation, DWORD dwCount);
	//׷�ӷ���
	virtual bool __cdecl AppendGameServerBuffer(tagGameServer * pGameServer, DWORD dwCount);
};

//////////////////////////////////////////////////////////////////////////

#endif