#pragma once

#ifndef SKIN_BUTTON_EX_HEAD_FILE
#define SKIN_BUTTON_EX_HEAD_FILE

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��ť��
class CSkinButtonEx : public CButton
{
	//��������
protected:
	BYTE								m_byStatusCount;				//״̬��Ŀ
	bool								m_bHovering;					//������־
	CSkinImage							m_ImageBack;					//��ťλͼ
	CPngImage							m_PngBack;						//��ťλͼ

	//��������
public:
	//���캯��
	CSkinButtonEx();
	//��������
	virtual ~CSkinButtonEx();

	//���غ���
protected:
	//�ؼ����໯
	virtual void PreSubclassWindow();
	//����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//���ܺ���
public:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//���ú���
public:
	//����λ��
	bool FixButtonSize();
	//����λͼ
	bool SetButtonImage(LPCTSTR pszFileName, BYTE byStatusCount, bool bExpandImage);
	//����λͼ
	bool SetButtonImage(UINT uBitmapID, HINSTANCE hInstance, BYTE byStatusCount, bool bExpandImage);
	//Ĭ�ϰ�ť
	void SetDefaultButton(bool bResetIcon=true, bool bResetColor=true, bool bResetImage=true);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinButtonEx)
};

//////////////////////////////////////////////////////////////////////////

#endif