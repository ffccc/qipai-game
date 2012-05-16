#ifndef SINGLE_PRO_ITEM_CONTROL_HEAD
#define SINGLE_PRO_ITEM_CONTROL_HEAD

#pragma once

#include "GameFrame.h"
#include "PropertyModule.h"

//////////////////////////////////////////////////////////////////////////
//���˵���
class PROPERTY_MODULE_CLASS CBugleDlg : public CSkinDialogEx
{
	//��������
protected:
	tagUserData						*m_pMeUserData;					//�û���Ϣ
	COLORREF						m_crChatTX;						//������ɫ
	CString							m_strInuptChat;					//������Ϣ
	int								m_nMaxChar;						//������
	CString							m_strPropertyInfo;				//������Ϣ

	//�ؼ�����
protected:
	CExpression						m_Expression;					//���鴰��
	CSkinButton						m_btOK;							//ȷ����ť
	CSkinButton						m_btCancel;						//�رհ�ť
	CSkinButton						m_btExpression;					//���鰴ť
	CSkinButton						m_btColorSet;					//��ɫ����
	CEdit							m_InputChat;					//�����

	//�������
protected:
	IClientKernel					* m_pIClientKernel;				//�ں˽ӿ�
	ITCPSocket						* m_pClientSocket;				//����ӿ�

public:
	//���캯��
	CBugleDlg(CWnd* pParent = NULL); 
	//��������
	virtual ~CBugleDlg();

protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);    

	//���ܺ���
public:
	//������Դ
	bool UpdateSkinResource();
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData);
	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	//��Ϣ����
protected:
	//��ɫ��ť
	afx_msg void OnBnClickedColorSet();
	//������Ϣ
	LRESULT OnHitExpression(WPARAM wParam, LPARAM lParam);
	//���鰴ť
	afx_msg void OnBnClickedExpression();
	//��ʼ���
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	afx_msg void OnBnClickedOk();
	//�ؼ���ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�滭����
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//���˵���
extern "C" PROPERTY_MODULE_CLASS VOID __cdecl ShowBugleDlg(IClientKernel *pIClientKernel,ITCPSocket *pClientSocket,tagUserData *pMeUserData);

//////////////////////////////////////////////////////////////////////////

#endif
