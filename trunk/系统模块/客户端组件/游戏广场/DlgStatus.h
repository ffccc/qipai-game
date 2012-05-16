#ifndef DLG_STATUS_HEAD_FILE
#define DLG_STATUS_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ״̬��
class CDlgStatus : public CDialog
{
	//��������
protected:
	WORD								m_wImagePos;					//λͼλ��					
	CWnd								* m_pNotifyWnd;					//֪ͨ����
	CString								m_strMessage;					//��Ϣ�ַ�

	//�ؼ�����
protected:
	CSkinButton							m_btCancel;						//ȡ����ť

	//��������
public:
	//���캯��
	CDlgStatus();
	//��������
	virtual ~CDlgStatus();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK() { return; };
	//ȡ������
	virtual void OnCancel() { return; };

	//���ܺ���
public:
	//������Ϣ
	bool ShowStatusMessage(LPCTSTR pszMessage, CWnd * pNotifyWnd);
	//���ٴ���
	bool DestroyStatusWnd(CWnd * pNotifyWnd);

	//��Ϣӳ��
protected:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//ȡ����ť
	afx_msg void OnBnClickedClose();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif