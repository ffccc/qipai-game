#ifndef BANKER_STORAGE_HEAD_FILE
#define BANKER_STORAGE_HEAD_FILE

#include "PropertyModule.h"

#pragma once

//////////////////////////////////////////////////////////////////////////

//�һ���Ϸ�� 
class PROPERTY_MODULE_CLASS CBankStorage : public CDialog
{
	//��������
public:
	tagUserData						*m_pMeUserData;					//�û���Ϣ
	LONG						m_lGameGold;					//������Ŀ
	LONG						m_lStorageGold;					//�洢��Ŀ
	LONG						m_lInCount;						//������Ϸ��
	CBrush							m_brBakcground;

	CString							m_strGameGold;					//������Ŀ
	CString							m_strStorageGold;				//�洢��Ŀ

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
	CBankStorage(CWnd* pParent = NULL); 
	//��������
	virtual ~CBankStorage();

	//��������
public:
	//���½���
	void UpdateView();

	//���غ���
public:
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
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserDatas);
	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
//////////////////////////////////////////////////////////////////////////

//�һ���Ϸ�� 
class PROPERTY_MODULE_CLASS CBankDrawing : public CDialog
{
	//��������
public:
	tagUserData						*m_pMeUserData;					//�û���Ϣ
	LONG						m_lGameGold;					//������Ŀ
	LONG						m_lStorageGold;					//�洢��Ŀ
	LONG						m_lInCount;						//������Ϸ��
	CBrush							m_brBakcground;

	CString							m_strGameGold;					//������Ŀ
	CString							m_strStorageGold;				//�洢��Ŀ

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
	CBankDrawing(CWnd* pParent = NULL); 
	//��������
	virtual ~CBankDrawing();

	//��������
public:
	//���½���
	void UpdateView();
	void Process();

	//���غ���
public:
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
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserDatas);
	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

void MakeString(CString &strNum,LONG lNumber);
void MakeStringToNum(CString str,LONG &Num);
//////////////////////////////////////////////////////////////////////////
#endif