#ifndef DLG_RESPONSE_LEAVE_HEAD_FILE
#define DLG_RESPONSE_LEAVE_HEAD_FILE

#pragma once


#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//�����뿪��Ӧ
class CDlgResponseLeave : public CSkinDialogEx
{
	//�������
protected:
	WORD							m_wRequestUser;						//�����û�
	CWnd							*m_pGameFrameDlg;					//�ص�
	WORD							m_wTimer;							//��ʱʱ��
	CString							m_strCaption;						//�Ի������

	//�ؼ�����
public:
	CSkinButton						m_btOk;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	//��������
public:
	//���캯��
	CDlgResponseLeave(CWnd * pParentWnd=NULL);
	//��������
	virtual ~CDlgResponseLeave();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���ܺ���
public:
	//������Ϣ
	void ShowRequestMessage(WORD wChairId,LPCTSTR pszCaption,LPCTSTR pszMessage);
	//��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);
	//���ûص�
	void SetGameFrameDlg( CWnd *pWnd ) { m_pGameFrameDlg = pWnd; }
	//��ȡ�û�ID
	WORD GetRequestUser() { return m_wRequestUser; }

	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////

#endif