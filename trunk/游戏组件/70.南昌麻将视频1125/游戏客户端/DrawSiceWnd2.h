#ifndef DRAW_WND2_HEAD_FILE
#define DRAW_WND2_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
#define IDM_GAME_START2			(WM_USER+404)						//�˿˲���
//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CDrawSiceWnd2 : public CWnd
{
	//���ñ���
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageDrawSiceFirst;				//��������
	CSkinImage						m_ImageDrawSiceSecond;				//
	BYTE                            m_bSicePos;
	int m_nXPos;
	int m_nYPos;
	CDC      *ParentDC;
	//��������
public:
	//���캯��
	CDrawSiceWnd2();
	//��������
	virtual ~CDrawSiceWnd2();

	//�ڲ�����
public:
	void SetSiceInfo(CDC *dc,BYTE bMoveTime,BYTE SiceFirst,BYTE SiceSecond);
	void SetBenchmarkPos(int nXPos, int nYPos);
	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif