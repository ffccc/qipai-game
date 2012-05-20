#ifndef BANK_HEAD_FILE
#define BANK_HEAD_FILE

#pragma once
#include "Stdafx.h"
#include "Resource.h"


class CGameBank : public CSkinDialogEx
{
	//��������
public:
	tagUserData	const				*m_pMeUserData;					//�û���Ϣ
	DWORD							m_lGameGold;					//������Ŀ
	DWORD							m_lStorageGold;					//�洢��Ŀ
	DWORD							m_lInCount;						//������Ϸ��

	//�ؼ�����
protected:
	CSkinButton						m_btOK;							//ȷ����ť
	CSkinButton						m_btCancel;						//ȡ����ť
	CSkinHyperLink					m_textTip;
	//�ӿڱ���
protected:
	IClientKernel					* m_pIClientKernel;				//�ں˽ӿ�

	//��������
public:
	//���캯��
	CGameBank();
	//��������
	virtual ~CGameBank();
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
	//ȷ����ť
	afx_msg void OnBnClickedOk();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	afx_msg void OnEnChangeInCount();
	//ѡ��ȡ��
	afx_msg void OnDrawOutSelected();
	//ѡ����
	afx_msg void OnSaveSelected();


	//���ܺ���
public:
	//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//�������
	void SetSendInfo(IClientKernel *pClientKernel, const tagUserData *pMeUserDatas);
	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};

#endif
