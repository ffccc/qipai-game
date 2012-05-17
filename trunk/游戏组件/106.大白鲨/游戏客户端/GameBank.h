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
	WORD							m_wBankerUser;					//��ǰׯ��
	LONG							m_lApplyBankerCondition;			//��������
	LONG							m_lGameGold;					//����Ǯ��
	LONG							m_lStorageGold;					//����Ǯ��
	LONG							m_lInCount;						//������Ϸ��
	LONG							m_lMeJetton;					//�Լ���ע����
	bool							m_bMeApplyBanker;					//�����ʶ
	//�ؼ�����
protected:
	CSkinButton						m_btOK;							//ȷ����ť
	CSkinButton						m_btCancel;						//ȡ����ť

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

	//���ܺ���
public:
	//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,const tagUserData *pMeUserDatas,LONG lMeJetton, WORD wBankerUser, LONG lApplyBankerCondition, bool bMeApplyBanker);
	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};

#endif
