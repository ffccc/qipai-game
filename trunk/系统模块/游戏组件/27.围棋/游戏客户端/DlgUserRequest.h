#ifndef GLG_USER_REQUEST_HEAD_FILE
#define GLG_USER_REQUEST_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//����Ի���
class CDlgUserRequest : public CSkinDialogEx
{
	//���ö���
protected:
	UINT							m_nElapse;							//����ʱ��
	UINT							m_uMessage;							//֪ͨ��Ϣ
	CString							m_strMessage;						//��ʾ��Ϣ

	//��������
protected:
	UINT							m_nResidualTime;					//ʣ��ʱ��

	//�ؼ�����
protected:
	CSkinButton						m_btApprove;						//ͬ�ⰴť
	CSkinButton						m_btCancel;							//ȡ����ť

	//��������
public:
	//���캯��
	CDlgUserRequest();
	//��������
	virtual ~CDlgUserRequest();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//���ܺ���
public:
	//��������
	void InitRequest(UINT uMessage, UINT nElapse, LPCTSTR pszMessage);

	//��Ϣӳ��
protected:
	//ͬ�ⰴť
	afx_msg void OnBnClickedApprove();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif