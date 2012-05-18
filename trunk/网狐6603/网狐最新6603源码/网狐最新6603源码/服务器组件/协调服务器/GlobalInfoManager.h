#ifndef GLOBAL_USER_MANAGER_HEAD_FILE
#define GLOBAL_USER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CGlobalUserItem;
class CGlobalPlazaItem;
class CGlobalServerItem;

//���鶨��
typedef CWHArray<CGlobalUserItem *> CGlobalUserItemArray;
typedef CWHArray<CGlobalPlazaItem *> CGlobalPlazaItemArray;
typedef CWHArray<CGlobalServerItem *> CGlobalServerItemArray;

//��������
typedef CMap<DWORD,DWORD,CGlobalUserItem *,CGlobalUserItem *> CMapUserID;
typedef CMap<DWORD,DWORD,CGlobalUserItem *,CGlobalUserItem *> CMapGameID;
typedef CMap<WORD,WORD,CGlobalPlazaItem *,CGlobalPlazaItem *> CMapPlazaID;
typedef CMap<WORD,WORD,CGlobalServerItem *,CGlobalServerItem *> CMapServerID;

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//�û���Ϣ
struct tagGlobalUserInfo
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�
};

//////////////////////////////////////////////////////////////////////////////////

//�û�����
class CGlobalUserItem
{
	//��Ԫ����
	friend class CGlobalInfoManager;

	//�û�����
public:
	DWORD							m_dwUserID;							//�û���ʶ
	DWORD							m_dwGameID;							//��Ϸ��ʶ
	TCHAR							m_szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
public:
	BYTE							m_cbGender;							//�û��Ա�
	BYTE							m_cbMemberOrder;					//��Ա�ȼ�
	BYTE							m_cbMasterOrder;					//����ȼ�

	//������Ϣ
public:
	CGlobalServerItemArray			m_GlobalServerItemArray;			//��������

	//��������
protected:
	CGlobalUserItem *				m_pNextUserItemPtr;					//����ָ��

	//��������
protected:
	//���캯��
	CGlobalUserItem();
	//��������
	virtual ~CGlobalUserItem();

	//���ܺ���
public:
	//�û���ʶ
	DWORD GetUserID() { return m_dwUserID; }
	//��Ϸ��ʶ
	DWORD GetGameID() { return m_dwGameID; }
	//�û��ǳ�
	LPCTSTR GetNickName() { return m_szNickName; }

	//�û���Ϣ
public:
	//�û��Ա�
	BYTE GetGender() { return m_cbGender; }
	//��Ա�ȼ�
	BYTE GetMemberOrder() { return m_cbMemberOrder; }
	//��Ա�ȼ�
	BYTE GetMasterOrder() { return m_cbMasterOrder; }

	//������Ϣ
public:
	//ö�ٷ���
	CGlobalServerItem * EnumServerItem(WORD wIndex);
};

//////////////////////////////////////////////////////////////////////////////////

//�㳡����
class CGlobalPlazaItem
{
	//��Ԫ����
	friend class CGlobalInfoManager;

	//��������
public:
	WORD							m_wIndex;							//������
	tagGamePlaza					m_GamePlaza;						//��Ϸ�㳡

	//��������
public:
	CMapUserID						m_MapUserID;						//�û�����

	//��������
protected:
	CGlobalPlazaItem *				m_pNextPlazaItemPtr;				//����ָ��

	//��������
protected:
	//���캯��
	CGlobalPlazaItem();
	//��������
	virtual ~CGlobalPlazaItem();
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class CGlobalServerItem
{
	//��Ԫ����
	friend class CGlobalInfoManager;

	//��������
public:
	WORD							m_wIndex;							//������
	tagGameServer					m_GameServer;						//��Ϸ����

	//��������
public:
	CMapUserID						m_MapUserID;						//�û�����

	//��������
protected:
	CGlobalServerItem *				m_pNextServerItemPtr;				//����ָ��

	//��������
protected:
	//���캯��
	CGlobalServerItem();
	//��������
	virtual ~CGlobalServerItem();

	//���ܺ���
public:
	//������
	WORD GetIndex() { return m_wIndex; }
	//��ȡ����
	WORD GetKindID() { return m_GameServer.wKindID; }
	//��ȡ����
	WORD GetServerID() { return m_GameServer.wServerID; }
	//�û���Ŀ
	DWORD GetUserItemCount() { return (DWORD)m_MapUserID.GetCount(); }

	//���Һ���
public:
	//Ѱ���û�
	CGlobalUserItem * SearchUserItem(DWORD dwUserID);
};

//////////////////////////////////////////////////////////////////////////////////

//ȫ����Ϣ
class CGlobalInfoManager
{
	//��������
protected:
	CMapUserID						m_MapUserID;						//�û���ʶ
	CMapPlazaID						m_MapPlazaID;						//�㳡��ʶ
	CMapServerID					m_MapServerID;						//�����ʶ

	//��������
protected:
	CMapGameID						m_MapGameID;						//��ʶ����
	CMapStringToPtr					m_MapNickName;						//�ǳ�����

	//�洢����
protected:
	CGlobalUserItem *				m_pGlobalUserItem;					//�û��洢
	CGlobalPlazaItem *				m_pGlobalPlazaItem;					//�㳡�洢
	CGlobalServerItem *				m_pGlobalServerItem;				//����洢

	//��������
public:
	//���캯��
	CGlobalInfoManager();
	//��������
	virtual ~CGlobalInfoManager();

	//������
public:
	//��������
	VOID ResetData();
	//�û���Ŀ
	DWORD GetUserItemCount() { return (DWORD)m_MapUserID.GetCount(); }
	//������Ŀ
	DWORD GetPlazaItemCount() { return (DWORD)m_MapPlazaID.GetCount(); }
	//������Ŀ
	DWORD GetServerItemCount() { return (DWORD)m_MapServerID.GetCount(); }

	//�û�����
public:
	//ɾ���û�
	bool DeleteUserItem(DWORD dwUserID, WORD wServerID);
	//�����û�
	bool ActiveUserItem(tagGlobalUserInfo & GlobalUserInfo, WORD wServerID);

	//�㳡����
public:
	//ɾ���㳡
	bool DeletePlazaItem(WORD wPlazaID);
	//����㳡
	bool ActivePlazaItem(WORD wBindIndex, tagGamePlaza & GamePlaza);

	//�������
public:
	//ɾ������
	bool DeleteServerItem(WORD wServerID);
	//�����
	bool ActiveServerItem(WORD wBindIndex, tagGameServer & GameServer);

	//�������
public:
	//Ѱ�ҹ㳡
	CGlobalPlazaItem * SearchPlazaItem(WORD wPlazaID);
	//Ѱ�ҷ���
	CGlobalServerItem * SearchServerItem(WORD wServerID);

	//�û�����
public:
	//Ѱ���û�
	CGlobalUserItem * SearchUserItemByUserID(DWORD dwUserID);
	//Ѱ���û�
	CGlobalUserItem * SearchUserItemByGameID(DWORD dwGameID);
	//Ѱ���û�
	CGlobalUserItem * SearchUserItemByNickName(LPCTSTR pszNickName);

	//ö�ٺ���
public:
	//ö���û�
	CGlobalUserItem * EnumUserItem(POSITION & Position);
	//ö�ٹ㳡
	CGlobalPlazaItem * EnumPlazaItem(POSITION & Position);
	//ö�ٷ���
	CGlobalServerItem * EnumServerItem(POSITION & Position);

	//��������
private:
	//�����û�
	CGlobalUserItem * CreateGlobalUserItem();
	//�����㳡
	CGlobalPlazaItem * CreateGlobalPlazaItem();
	//��������
	CGlobalServerItem * CreateGlobalServerItem();

	//�ͷź���
private:
	//�ͷ��û�
	bool FreeGlobalUserItem(CGlobalUserItem * pGlobalUserItem);
	//�ͷŹ㳡
	bool FreeGlobalPlazaItem(CGlobalPlazaItem * pGlobalPlazaItem);
	//�ͷŷ���
	bool FreeGlobalServerItem(CGlobalServerItem * pGlobalServerItem);
};

//////////////////////////////////////////////////////////////////////////////////

#endif