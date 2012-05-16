#ifndef SERVER_LIST_HEAD_FILE
#define SERVER_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "ListService.h"

//////////////////////////////////////////////////////////////////////////

//��˵��
typedef CArrayTemplate<tagGameTypeItem *>								CGameTypeItemArray;
typedef CArrayTemplate<tagGameKindItem *>								CGameKindItemArray;
typedef CArrayTemplate<tagGameServerItem *>								CGameServerItemArray;
typedef CArrayTemplate<tagGameStationItem *>							CGameStationItemArray;
typedef CMap<WORD,WORD,tagGameTypeItem *,tagGameTypeItem * &>			CTypeItemMap;
typedef CMap<WORD,WORD,tagGameKindItem *,tagGameKindItem * &>			CKindItemMap;
typedef CMap<DWORD,DWORD,tagGameServerItem *,tagGameServerItem * &>		CServerItemMap;
typedef CMap<DWORD,DWORD,tagGameStationItem *,tagGameStationItem * &>	CStationItemMap;

//////////////////////////////////////////////////////////////////////////

//�������б���
class CServerListCenter : public IServerListCenter
{
	//��������
protected:
	CTypeItemMap					m_TypeItemMap;						//��������
	CKindItemMap					m_KindItemMap;						//��������
	CServerItemMap					m_ServerItemMap;					//��������
	CStationItemMap					m_StationItemMap;					//��������

	//�洢����
protected:
	CGameTypeItemArray				m_TypeItemStore;					//�洢����
	CGameKindItemArray				m_KindItemStore;					//�洢����
	CGameServerItemArray			m_ServerItemStore;					//�洢����
	CGameStationItemArray			m_StationItemStore;					//�洢����

	//��������
public:
	//���캯��
	CServerListCenter(void);
	//��������
	virtual ~CServerListCenter(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl InsertGameType(tagGameType * pGameType);
	//��������
	virtual bool __cdecl InsertGameKind(tagGameKind * pGameKind);
	//���뷿��
	virtual bool __cdecl InsertGameServer(tagGameServer * pGameServer);
	//����վ��
	virtual bool __cdecl InsertGameStation(tagGameStation * pGameStation);

	//ɾ���ӿ�
public:
	//ɾ������
	virtual bool __cdecl DeleteGameType(WORD wTypeID);
	//ɾ������
	virtual bool __cdecl DeleteGameKind(WORD wKindID);
	//ɾ������
	virtual bool __cdecl DeleteGameServer(WORD wKindID, WORD wServerID);
	//ɾ��վ��
	virtual bool __cdecl DeleteGameStation(WORD wKindID, WORD wStationID);

	//ö�ٽӿ�
public:
	//ö������
	virtual tagGameTypeItem * const __cdecl EmunGameTypeItem(POSITION & Pos);
	//ö������
	virtual tagGameKindItem * const __cdecl EmunGameKindItem(POSITION & Pos);
	//ö�ٷ���
	virtual tagGameServerItem * const __cdecl EmunGameServerItem(POSITION & Pos);
	//ö��վ��
	virtual tagGameStationItem * const __cdecl EmunGameStationItem(POSITION & Pos);

	//���ҽӿ�
public:
	//��������
	virtual tagGameTypeItem * const __cdecl SearchGameType(WORD wTypeID);
	//��������
	virtual tagGameKindItem * const __cdecl SearchGameKind(WORD wKindID);
	//���ҷ���
	virtual tagGameServerItem * const __cdecl SearchGameServer(WORD wKindID, WORD wServerID);
	//����վ��
	virtual tagGameStationItem * const __cdecl SearchGameStation(WORD wKindID, WORD wStationID);

	//��Ŀ�ӿ�
public:
	//������Ŀ
	virtual DWORD __cdecl GetGameTypeCount() { return (DWORD)m_TypeItemMap.GetCount(); }
	//������Ŀ
	virtual DWORD __cdecl GetGameKindCount() { return (DWORD)m_KindItemMap.GetCount(); }
	//������Ŀ
	virtual DWORD __cdecl GetGameServerCount() { return (DWORD)m_ServerItemMap.GetCount(); }
	//վ����Ŀ
	virtual DWORD __cdecl GetGameStationCount() { return (DWORD)m_StationItemMap.GetCount(); }

	//ά���ӿ�
public:
	//�����б�
	virtual void __cdecl ResetServerList();
	//�����б�
	virtual bool __cdecl LoadServerList(tagDataBaseInfo & DataBaseInfo);
	//��������
	virtual bool __cdecl UpdateServerOnLineCount(WORD wKindID, WORD wServerID, DWORD dwOnLineCount);

	//���ؽӿ�
private:
	//��������
	void LoadGameType(CDataBaseAide & DataBaseAide);
	//��������
	void LoadGameKind(CDataBaseAide & DataBaseAide);
	//����վ��
	void LoadGameStation(CDataBaseAide & DataBaseAide);

	//�ڲ�����
private:
	//��ȡ����
	UINT GetMaxPrime(UINT uLessNum);
	//��������
	void UpdateKindOnLineCount(WORD wKindID);
};

//////////////////////////////////////////////////////////////////////////

#endif