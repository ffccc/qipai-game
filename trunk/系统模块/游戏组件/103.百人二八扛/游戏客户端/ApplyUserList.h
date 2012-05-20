#ifndef APPLY_USER_LIST_HEAD_FILE
#define APPLY_USER_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"

#include "CMyListCtrl.h"

#define  WM_VIEWLBTUP WM_USER+1


//////////////////////////////////////////////////////////////////////////

//
struct tagApplyUser
{
	//�����Ϣ
	CString							strUserName;						//����ʺ�
	LONGLONG							lUserScore;							//��ҽ��
};


//////////////////////////////////////////////////////////////////////////

//��Ϸ��¼
class CApplyUser : public CDialog
{
	//�ؼ�����
public:
	CListCtrlCl					m_AppyUserList;					//��Ϸ�б�
	HWND						m_viewHandle;

	CRect						m_OrtRect;


	//��������
public:
	//���캯��
	CApplyUser();

	void MySetRect(CRect aRect);
	//��������
	virtual ~CApplyUser();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK()
	{
		return;
	}
	//ȡ����Ϣ
	virtual void OnCancel()
	{
		return;
	}

	//���ܺ���
public:
	//�����б�
	void InserUser(tagApplyUser & ApplyUser);
	//ɾ���б�
	void DeleteUser(tagApplyUser & ApplyUser);
	//�����б�
	void UpdateUser(tagApplyUser & ApplyUser);

	int  GetItemCount();

	//�������
	bool CApplyUser::FindUser(LPCTSTR lpszUserName);
	//����б�
	void ClearAll();
	//listctrlUp��Ϣ
	LRESULT OnLBtUp(WPARAM wParam, LPARAM lParam);

	//��Ϣӳ��
protected:
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
