#ifndef PLAZA_USER_INFO_VIEW_HEAD_FILE
#define PLAZA_USER_INFO_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class CUserInfoView : public CDialog
{
	//״̬����
protected:
	bool							m_bShowInfo;						//��ʾ��־
	bool							m_bCreateFlag;						//������־
	UINT							m_nTitleHeight;						//����߶�

	//��Դ����
protected:
	CSkinImage						m_ImageTitleL;						//λͼ����
	CSkinImage						m_ImageTitleM;						//λͼ����
	CSkinImage						m_ImageTitleR;						//λͼ����

	//��������
public:
	//���캯��
	CUserInfoView();
	//��������
	virtual ~CUserInfoView();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK() { return; };
	//ȡ������
	virtual VOID OnCancel() { return; };

	//���ܺ���
public:
	//����߶�
	UINT GetTitleHeight();
	//������Ϣ
	VOID UpdateUserInfo();
	//��ʾ��Ϣ
	VOID ShowUserInfo(bool bShowInfo);
	//������Դ
	bool UpdateSkinResource();

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif