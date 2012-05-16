#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CenterService.h"

//////////////////////////////////////////////////////////////////////////

//���öԻ���
class CSystemOptionDlg : public CDialog
{
public:
	CSystemOptionDlg();
	virtual ~CSystemOptionDlg();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK();

	//���ܺ���
public:
	//���ò���
	void SetInitParamter(const CInitParamter & InitParamter);

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//���Ի���
class CCenterServerDlg : public CDialog
{
	//�������
public:
	CCenterService					m_CenterService;					//��¼����
	CRichEditTrace					m_RichEditTrace;					//�¼���¼

	//���캯��
public:
	//�������
	CCenterServerDlg();
	//��������
	virtual ~CCenterServerDlg();

	//���غ���
protected:
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ������
	virtual void OnCancel();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//��Ϣ����
protected:
	//��������
	afx_msg void OnBnClickedStartService();
	//ֹͣ����
	afx_msg void OnBnClickedStopService();
	//���÷���
	afx_msg void OnBnClickedInitService();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
