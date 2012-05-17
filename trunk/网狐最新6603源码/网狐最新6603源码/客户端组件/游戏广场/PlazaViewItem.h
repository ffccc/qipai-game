#ifndef PLAZA_VIEW_ITEM_HEAD_FILE
#define PLAZA_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
class CPlazaBrowser : public CHtmlView
{
	//��������
public:
	//���캯��
	CPlazaBrowser();
	//��������
	virtual ~CPlazaBrowser();

	//���غ���
protected:
	//�󶨺���
	virtual VOID PreSubclassWindow();
	//���Ĵ���
	virtual VOID PostNcDestroy();

	//���ܺ���
public:
	//��������
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext=NULL);

	//��Ϣ����
public:
	//�ر��¼�
	VOID OnWindowClosing(BOOL IsChildWindow, BOOL * bCancel);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ�㳡
class CPlazaViewItem : public CDialog, public IUnknownEx
{
	//״̬����
protected:
	bool							m_bCreateFlag;						//������־

	//��ť�ؼ�
protected:
	CSkinButton						m_btWebLast;						//��ҳ��ť
	CSkinButton						m_btWebNext;						//��ҳ��ť
	CSkinButton						m_btWebStop;						//��ҳ��ť
	CSkinButton						m_btWebHome;						//��ҳ��ť
	CSkinButton						m_btWebReload;						//��ҳ��ť

	//�ؼ�����
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CPlazaBrowser					m_PlazaBrowser;						//����ؼ�

	//��Դ����
protected:
	CEncircleBMP					m_BrowserEncircle;					//�����Դ

	//�ؼ�ָ��
protected:
	static CPlazaViewItem *			m_pPlazaViewItem;					//�㳡ָ��

	//��������
public:
	//���캯��
	CPlazaViewItem();
	//��������
	virtual ~CPlazaViewItem();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//ȷ������
	virtual VOID OnOK() { return; };
	//ȡ������
	virtual VOID OnCancel() { return; };

	//���ܺ���
public:
	//���ҳ��
	bool WebBrowse(LPCTSTR pszURL);
	//��ȡʵ��
	static CPlazaViewItem * GetInstance() { return m_pPlazaViewItem; }

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif