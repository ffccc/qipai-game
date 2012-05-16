#ifndef CHARMVALUE_EXCHANGE_HEAD_FILE
#define CHARMVALUE_EXCHANGE_HEAD_FILE

#include "PropertyModule.h"

#pragma once

//////////////////////////////////////////////////////////////////////////

//�һ���Ϸ�� 
class PROPERTY_MODULE_CLASS CCharmValueExchange : public CSkinDialogEx
{
	//��������
public:
	tagUserData						*m_pMeUserData;					//�û���Ϣ
	DWORD							m_lLoveliness;					//�û�����
	LONG						m_lBankGoldValue;				//������Ϸ��
	DWORD							m_lExchangeCharmValue;			//�Ի�����
	CString							m_strExChangeValue;				//�Ի�����

	//�ؼ�����
protected:
	CSkinButton						m_btOK;							//ȷ����ť
	CSkinButton						m_btCancel;						//ȡ����ť

	//�ӿڱ���
protected:	
	IClientKernel					* m_pIClientKernel;				//�ں˽ӿ�
	ITCPSocket						* m_pClientSocket;				//����ӿ�

	//��������
public:
	//���캯��
	CCharmValueExchange(CWnd* pParent = NULL); 
	//��������
	virtual ~CCharmValueExchange();

	//��������
public:
	//���½���
	void UpdateView();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	afx_msg void OnBnClickedBtExchange();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	afx_msg void OnEnChangeExchangeCharmValue();
	//������ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//�滭����
	afx_msg void OnPaint();

	//���ܺ���
public:
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pUserData);
	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////

//�һ�����
extern "C" PROPERTY_MODULE_CLASS VOID __cdecl ShowCharmValueExchangeDlg(IClientKernel *pIClientKernel,ITCPSocket *pClientSocket,tagUserData *pUserData);

//////////////////////////////////////////////////////////////////////////
#endif