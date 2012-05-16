#pragma once

#ifndef SKIN_DIALOG_HEAD_FILE
#define SKIN_DIALOG_HEAD_FILE

#include "SkinControls.h"
#include "SkinWndObject.h"

//////////////////////////////////////////////////////////////////////////

//�Ի�����
class SKIN_CONTROL_CLASS CSkinDialog : public CDialog, public CSkinWndObject
{
	//��������
public:
	//���캯��
	CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd=NULL);
	//��������
	virtual ~CSkinDialog();

	//���غ���
public:
	//��ʼ����Ϣ
	virtual BOOL OnInitDialog();

	//��Ϣӳ��
protected:
	//������Ϣ
	afx_msg void OnActivate(UINT nState, CWnd * pWndOther, BOOL bMinimized);
	//������Ϣ
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	//�����Ϣ
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�滭��Ϣ
	afx_msg void OnPaint();
	//�ı���Ϣ
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//λ�øı�
	afx_msg void OnWindowPosChanged(WINDOWPOS * lpwndpos);
	//�ؼ���ɫ
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	//������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinDialog)
};

//////////////////////////////////////////////////////////////////////////

//�Ի�����
class SKIN_CONTROL_CLASS CSkinDialogEx : public CSkinDialog
{
	//��������
public:
	//���캯��
	CSkinDialogEx(UINT nIDTemplate, CWnd * pParentWnd=NULL);
	//��������
	virtual ~CSkinDialogEx();

	//�滭����
protected:
	//�滭�߿�
	void DrawBorder(CDC * pDC);
	//�滭����
	void DrawCaption(CDC * pDC);
	//�滭����
	void DrawBackGround(CDC * pDC);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinDialogEx)
};

//////////////////////////////////////////////////////////////////////////

//����Ի�����
class SKIN_CONTROL_CLASS CRgnDialog : public CDialog
{
	//��������
protected:
	CRgn							m_DialogRgn;						//��������
	CSkinImage						m_ImageBack;						//����λͼ

	//��������
public:
	//���캯��
	CRgnDialog(UINT uTemplate, CWnd * pParent=NULL);
	//��������
	virtual ~CRgnDialog();
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���ܺ���
public:
	//����λͼ
	bool LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans);
	//����λͼ
	bool LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans);

	//�ڲ����� 
private:
	//��������
	bool CreateControlRgn(COLORREF crTrans);

	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
	//����Ҽ�������Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CRgnDialog)
};

//////////////////////////////////////////////////////////////////////////

#endif