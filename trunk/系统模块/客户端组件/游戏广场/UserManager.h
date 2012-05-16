#ifndef USER_MANAGER_HEAD_FILE
#define USER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "IGamePlaza.h"

//////////////////////////////////////////////////////////////////////////

//�û���Ϣ��
class CUserItem : public IUserItem
{
	friend class CClientUserManager;

	//��������
public:
	tagUserData						m_UserData;						//�û���Ϣ

	//�ڲ�����
private:
	bool							m_bActive;						//������Ч

	//��������
public:
	//���캯��
	CUserItem();
	//��������
	virtual ~CUserItem();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//�����ж�
	virtual bool __cdecl IsActive() { return m_bActive; }

	//���Խӿ�
public:
	//��Ϸ����
	virtual LONG __cdecl GetUserPlayCount();
	//�û� I D
	virtual DWORD __cdecl GetUserID() { return m_UserData.dwUserID; }
	//�û�����
	virtual LPCTSTR __cdecl GetUserName() { return m_UserData.szName; }
	//��ȡ�û�
	virtual tagUserData * __cdecl GetUserData() { return &m_UserData; }

	//���ýӿ�
public:
	//���û���
	virtual void __cdecl SetUserScore(const tagUserScore * pUserScore);
	//����״̬
	virtual void __cdecl SetUserStatus(const tagUserStatus * pUserStatus);

	//���ܺ���
public:
	//��������
	void CleanData();
};

//////////////////////////////////////////////////////////////////////////

//��˵��
typedef CArrayTemplate<CUserItem *> CUserItemPtrArray;

//�û�������
class CClientUserManager : public IClientUserManager
{
	//��������
protected:
	CUserItemPtrArray				m_UserItemActive;					//�����
	static CUserItemPtrArray		m_UserItemStorage;					//�洢����

	//�ⲿ�ӿ�
protected:
	IUserManagerSink				* m_pIUserManagerSink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CClientUserManager(void);
	//��������
	virtual ~CClientUserManager(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//���ûص��ӿ�
	virtual bool __cdecl SetUserManagerSink(IUnknownEx * pIUnknownEx);
	//�����û�
	virtual IUserItem * __cdecl ActiveUserItem(tagUserData & UserData);
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IUserItem * pIUserItem);
	//���»���
	virtual bool __cdecl UpdateUserItemScore(IUserItem * pIUserItem, const tagUserScore * pUserScore);
	//����״̬
	virtual bool __cdecl UpdateUserItemStatus(IUserItem * pIUserItem, const tagUserStatus * pUserStatus);

	//��Ϣ�ӿ�
public:
	//��ȡ����
	virtual DWORD __cdecl GetOnLineCount() { return (DWORD)m_UserItemActive.GetCount(); }

	//���ҽӿ�
public:
	//ö���û�
	virtual IUserItem * __cdecl EnumUserItem(WORD wEnumIndex);
	//�����û�
	virtual IUserItem * __cdecl SearchUserByUserID(DWORD dwUserID);
	//�����û�
	virtual IUserItem * __cdecl SearchUserByGameID(DWORD dwGameID);
};

//////////////////////////////////////////////////////////////////////////

#endif