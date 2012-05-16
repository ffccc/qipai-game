#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "Afxcmn.h"
#include "UserListView.h"

//�û����ҶԻ��� 
class CFindUserDlg : public CSkinDialogEx
{
	//��������
protected:
	UINT							m_uGameType;						//��Ϸ����
	CWnd							* m_pParentWnd;						//������

	//�ؼ�����
public:
	CSkinEdit						m_UserName;							//�û�����
	CSkinEdit						m_UserAcc;							//�û� ACC
	CSkinButton						m_BtFind;							//���Ұ�ť
	CSkinButton						m_BtCancel;							//�رհ�ť
	CUserListView					m_UserList;							//�û��б�

	//��������
public:
	//���캯��
	CFindUserDlg(CWnd * pParentWnd, UINT uGameType);
	//��������
	virtual ~CFindUserDlg();
	//�����û���Ϣ
	void UpdateUserItem(IUserItem * pUserItem);
	//ɾ���û���Ϣ
	void DeleteUserItem(IUserItem * pUserItem);

	//���غ���
public:
	//���ݰ�
	virtual void DoDataExchange(CDataExchange * pDX);    
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ��
	virtual void OnOK();
	//ȡ��
	virtual void OnCancel() { DestroyWindow(); } 

	//��Ϣ����
protected:
	//��������Ҽ�
	afx_msg void OnNMRclickList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};
