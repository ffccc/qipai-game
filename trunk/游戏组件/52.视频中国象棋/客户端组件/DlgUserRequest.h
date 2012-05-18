#ifndef DLG_USER_REQUEST_HEAD_FILE
#define DLG_USER_REQUEST_HEAD_FILE

#pragma once

#include "Stdafx.h"
//#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////

//���ݶ���
#define REQUEST_PEACE				1									//��������
#define REQUEST_REGRET				2									//��������

//��Ϣ����
#define IDM_USER_REQUEST_RESPOND	(WM_USER+400)						//�����Ӧ

//////////////////////////////////////////////////////////////////////////

//�û�����
class CDlgUserRequest : public CDialog
{
	//��������
public:
	BYTE							m_cbRequestType;					//��������
	LONG							m_nResidualTime;					//ʣ��ʱ��
	CSkinImage						m_ImageViewBack;					//������Դ

	//�ؼ�����
protected:
	CSkinButton						m_btOk;								//��ť�ؼ�
	CSkinButton						m_btCancel;							//��ť�ؼ�

	//��������
public:
	//���캯��
	CDlgUserRequest();
	//��������
	virtual ~CDlgUserRequest();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//��Ϣӳ��
protected:
	//�ػ�����
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
