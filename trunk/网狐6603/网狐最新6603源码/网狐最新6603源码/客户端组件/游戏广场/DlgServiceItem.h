#ifndef DLG_SERVICE_ITEM_HEAD_FILE
#define DLG_SERVICE_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgServiceItem : public CDialog
{
	//��Ԫ����
	friend class CDlgService;
	friend class CDlgServiceBar;

	//��������
protected:
	BYTE							m_cbImageIndex;						//ͼ����Ҫ

	//��������
public:
	//���캯��
	CDlgServiceItem(UINT uIDTemplate);
	//��������
	virtual ~CDlgServiceItem();

	//���غ���
protected:
	//ȷ����Ϣ
	virtual VOID OnOK() { return; }
	//ȡ����Ϣ
	virtual VOID OnCancel() { return; }

	//���غ���
public:
	//��ȡ����
	virtual LPCTSTR GetServiceItemName()=NULL;
	//�滭����
	virtual VOID DrawItemView(CDC * pDC, INT nWidth, INT nHeight);

	//���ܺ���
public:
	//��������
	VOID CreateServiceItem(CWnd * pParentWnd, CRect rcCreate);
	//��ȡ�ַ�
	VOID GetControlItemText(UINT uID, TCHAR szString[], WORD wMaxCount);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ɫ��Ϣ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��ҳ����
class CDlgServiceBrowser : public CDlgServiceItem
{
	//��������
protected:
	TCHAR							m_szItemName[32];					//��������
	TCHAR							m_szServiceUrl[32];					//���ӵ�ַ

	//�ؼ�����
protected:
	CWebPublicize					m_ServicePublicize;					//����ؼ�

	//��������
public:
	//���캯��
	CDlgServiceBrowser();
	//��������
	virtual ~CDlgServiceBrowser();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//��ȡ����
	virtual LPCTSTR GetServiceItemName() { return m_szItemName; }

	//���ܺ���
public:
	//���÷���
	VOID InitService(BYTE cbImageIndex, LPCTSTR pszItemName, LPCTSTR pszServiceUrl);

	//��Ϣ����
protected:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif