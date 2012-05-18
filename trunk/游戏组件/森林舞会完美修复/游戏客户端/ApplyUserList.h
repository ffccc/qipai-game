#ifndef APPLY_USER_LIST_HEAD_FILE
#define APPLY_USER_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//
struct tagApplyUser
{
	//�����Ϣ
	CString							strUserName;					//����ʺ�
	CString							stAname;					    //����ʺ�
	DWORD							dwUserID;						//����ʺ�
	INT64							lUserScore;						//��ҽ��
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��¼
class CApplyUser : public CDialog
{
	//�ؼ�����
public:
	CListCtrl					m_AppyUserList;					//��Ϸ�б�

	//��������
public:
	//���캯��
	CApplyUser();
	//��������
	virtual ~CApplyUser();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }
	char* DrawNUM(INT64 num);
	string s1;
	//���ܺ���
public:
	//�����б�
	void InserUser(tagApplyUser & ApplyUser);
	//ɾ���б�
	void DeleteUser(tagApplyUser & ApplyUser);
	//�����б�
	void UpdateUser( tagApplyUser & ApplyUser );
	//����б�
	void ClearAll();
	//ȡ��������
	WORD GetCount(){return m_AppyUserList.GetItemCount();}

	int FindUser(DWORD dwUserID);

	//��Ϣӳ��
protected:
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif