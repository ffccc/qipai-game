#ifndef DLG_INDIVIDUAL_INFO_HEAD_FILE
#define DLG_INDIVIDUAL_INFO_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//��¼�Ի���
class CDlgIndividualInfo : public CSkinDialogEx
{
	//�ؼ�����
public:
	CWebBrowser							m_BrowerAD;						//�������

	//��������
public:
	//���캯��
	CDlgIndividualInfo();
	//��������
	virtual ~CDlgIndividualInfo();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif