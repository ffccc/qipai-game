#ifndef DLG_COMPAION_MANAGER_HEAD_FILE
#define DLG_COMPAION_MANAGER_HEAD_FILE

#pragma once

#include "Companion.h"
#include "CompanionTreeCtrl.h"
#include "CompanionListCtrl.h"

//////////////////////////////////////////////////////////////////////////

//���ѹ���Ի�����
class CDlgCompanionManager : public CSkinDialogEx
{
	//��������
protected:
	ICompanionManager				* m_pICompanionManager;				//����ӿ�

	//�ؼ�����
public:
	CSkinButton						m_btClose;							//�رհ�ť
	CCompanionListCtrl				m_FriendListCtrl;					//�����б�
	CCompanionListCtrl				m_DetestListCtrl;					//����б�
	CCompanionTreeCtrl				m_CompanionTreeCtrl;				//���Կؼ�

	//��������
public:
	//���캯��
	CDlgCompanionManager();
	//��������
	virtual ~CDlgCompanionManager();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel();

	//���ܺ���
public:
	//���ýӿ�
	bool SetCompanionManager(IUnknownEx * pIUnknownEx);
	//�����ϵ
	bool InsertCompanionItem(const tagCompanionItem * pCompanionItem);
	//���¹�ϵ
	bool UpdateCompanionItem(const tagCompanionItem * pCompanionItem, bool bChangeCompanion);
	//ɾ����ϵ
	bool DeleteCompanionItem(const tagCompanionItem * pCompanionItem);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif