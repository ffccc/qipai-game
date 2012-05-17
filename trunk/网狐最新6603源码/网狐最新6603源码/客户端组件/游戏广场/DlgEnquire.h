#ifndef DLG_ENQUIRE_HEAD_FILE
#define DLG_ENQUIRE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//�رմ���
class CDlgEnquire : public CDialog
{
	//�ؼ�����
protected:
	CSkinButton						m_btCancel;							//ȡ������
	CSkinButton						m_btClosePlaza;						//�رմ���
	CSkinButton						m_btCloseServer;					//�رշ���
	CSkinButton						m_btSwitchAccounts;					//�л��ʺ�

	//������Դ
protected:
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��
	CPlatformPublicize				m_PlatformPublicize;				//����ؼ�

	//��������
public:
	//���캯��
	CDlgEnquire();
	//��������
	virtual ~CDlgEnquire();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//��ť��Ϣ
protected:
	//�رմ���
	VOID OnBnClickedClosePlaza();
	//�رշ���
	VOID OnBnClickedCloseServer();
	//�л��ʺ�
	VOID OnBnClickedSwitchAccounts();

	//��Ϣ����
public:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ʾ��Ϣ
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//λ�øı�
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif