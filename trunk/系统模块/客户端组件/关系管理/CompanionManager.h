#ifndef COMPANION_MANAGER_HEAD_FILE
#define COMPANION_MANAGER_HEAD_FILE

#pragma once

#include "Companion.h"
#include "DlgCompanionManager.h"

//////////////////////////////////////////////////////////////////////////

//��˵��
typedef CArrayTemplate<tagCompanionItem *> CCompanionItemArray;			//�û���ϵ

//////////////////////////////////////////////////////////////////////////

//��ϵ����
class CCompanionManager : public ICompanionManager
{
	//��������
protected:
	bool							m_bNeedUpdate;						//���±�־

	//�������
protected:
	CCompanionItemArray				m_CompanionItemActive;				//�����û�
	CCompanionItemArray				m_CompanionItemRelease;				//�ͷ��û�
	CDlgCompanionManager			m_DlgCompanionManager;				//������

	//��������
public:
	//���캯��
	CCompanionManager(void);
	//��������
	virtual ~CCompanionManager(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//������Ϣ
	virtual bool __cdecl LoadCompanion();
	//�����ϵ
	virtual bool __cdecl SaveCompanion();
	//��ʾ����
	virtual bool __cdecl ShowCompanionManager();
	//��ϵ��Ŀ
	virtual INT_PTR __cdecl GetCompanionItemCount() { return m_CompanionItemActive.GetCount(); }

	//���ܽӿ�
public:
	//�����ϵ
	virtual const tagCompanionItem * __cdecl InsertCompanionItem(tagCompanionItem & CompanionItem);
	//���¹�ϵ
	virtual const tagCompanionItem * __cdecl UpdateCompanionItem(tagCompanionItem & CompanionItem);
	//���¹�ϵ
	virtual const tagCompanionItem * __cdecl UpdateCompanionItem(DWORD dwUserID, LPCTSTR pszNote);
	//���¹�ϵ
	virtual const tagCompanionItem * __cdecl UpdateCompanionItem(DWORD dwUserID, LPCTSTR pszAccounts, WORD wFaceID);
	//ɾ����ϵ
	virtual bool __cdecl DeleteCompanionItem(DWORD dwUserID);

	//���ҽӿ�
public:
	//ö�ٹ�ϵ
	virtual const tagCompanionItem * __cdecl EmunCompanionItem(WORD wIndex);
	//Ѱ�ҹ�ϵ
	virtual const tagCompanionItem * __cdecl SearchCompanionItem(DWORD dwUserID);
};

//////////////////////////////////////////////////////////////////////////

#endif