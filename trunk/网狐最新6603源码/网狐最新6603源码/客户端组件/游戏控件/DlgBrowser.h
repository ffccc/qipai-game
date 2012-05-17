#ifndef DLG_BROWSER_HEAD_FILE
#define DLG_BROWSER_HEAD_FILE

#pragma once

#include "WebPublicize.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
class SHARE_CONTROL_CLASS CDlgBrowser : public CSkinDialog
{
	//��������
protected:
	CSize							m_SizeWindow;						//���ڴ�С
	CString							m_strBrowserURL;					//���ӵ�ַ
	CString							m_strWindowTitle;					//������Ϣ

	//�������
protected:
	CWebPublicize					m_WebPublicize;						//�������

	//��������
public:
	//���캯��
	CDlgBrowser();
	//��������
	virtual ~CDlgBrowser();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//���ܺ���
public:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//��������
	VOID CreateBrowserControl(LPCTSTR pszTitle, LPCTSTR pszBrowserURL, CSize SizeWindow);

	//��Ϣ����
protected:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif