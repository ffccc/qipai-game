#pragma once
#define  WM_LBTUP   WM_USER+30

#include "afxcmn.h"

// CHeaderCtrlCl

class CHeaderCtrlCl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CHeaderCtrlCl)

public:
	CHeaderCtrlCl();
	virtual ~CHeaderCtrlCl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CStringArray m_HChar;
	CString m_Format; //��ʾ�������͵���������,0��ʾ����룬1��ʾ�м���룬2��ʾ�Ҷ���
public:
	int m_R;
	int m_G;
	int m_B;
	int m_Gradient;	// �����屳��������ϵ��
	float m_Height;  //��ͷ�߶ȣ����Ǳ���,
	int m_fontHeight; //����߶�
	int m_fontWith;   //������
	COLORREF m_color;
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam );
};



// HeaderCtrlCl.cpp : ʵ���ļ�
//





// CHeaderCtrlCl


class CListCtrlCl : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlCl)

public:
	CHeaderCtrlCl m_Header;
	CListCtrlCl();
	virtual ~CListCtrlCl();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
public:
	// �и�
	int m_nRowHeight;
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
public:
	// Gradient - ����ϵ�������屳����,���ý�����Ϊ0
	void SetHeaderBKColor(int R, int G, int B, int Gradient);
public:
	// ���ñ�ͷ�߶�
	void SetHeaderHeight(float Height);
	CPtrList m_ptrListCol;  //��������ɫ
	CPtrList m_ptrListItem; //����Item��ɫ��
	CPtrList m_colTextColor; //������������ɫ
	CPtrList m_ItemTextColor; //���浥Ԫ��������ɫ
	bool FindColColor(int col ,COLORREF &color); //��������ɫ
	bool FindItemColor(int col,int row,COLORREF &color);
	bool FindColTextColor(int col,COLORREF &color); //������������ɫ
	bool FindItemTextColor(int col,int row,COLORREF &color);
	void SetColColor(int col,COLORREF color);  //��������ɫ
	void SetItemColor(int col,int row,COLORREF color); //����Item��ɫ
	void SetColTextColor(int col,COLORREF color);   //�������ı���ɫ
	void SetItemTextColor(int col,int row,COLORREF color);
	void SetRowHeigt(int nHeight); //�����и�
	void SetHeaderFontHW(int nHeight,int nWith); //���ñ�ͷ�����С
	void SetHeaderTextColor(COLORREF color);
	COLORREF m_color;
	BOOL SetTextColor(COLORREF cr);
	void SetFontHW(int nHeight,int nWith);  //��������ĸߺͿ�

	void CListCtrlCl::Init(CRect windowRect,CWnd* pParent);

	BOOL CListCtrlCl::OnEraseBkgnd(CDC* pDC);

	void CListCtrlCl::OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg LRESULT  OnDraw(WPARAM wParam,LPARAM lParam);

	void CListCtrlCl::OnPaint();
public:
	// ����߶�
	int m_fontHeight;
public:
	// ������
	int m_fontWith;
	HWND  m_viewHandle;
};


