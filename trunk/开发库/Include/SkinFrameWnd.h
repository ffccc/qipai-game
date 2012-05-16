#ifndef SKIN_FRAMEWND_HEAD_FILE
#define SKIN_FRAMEWND_HEAD_FILE

#pragma once

#include "SkinControls.h"
#include "SkinWndObject.h"

//////////////////////////////////////////////////////////////////////////

//�����
class SKIN_CONTROL_CLASS CSkinFrameWnd : public CFrameWnd, public CSkinWndObject
{
	//��������
public:
	//���캯��
	CSkinFrameWnd();
	//��������
	virtual ~CSkinFrameWnd();

	//��Ϣӳ��
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CSkinFrameWnd)
};

//////////////////////////////////////////////////////////////////////////

#endif