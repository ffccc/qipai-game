#ifndef DRAW_WND_HEAD_FILE
#define DRAW_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
#define IDM_GAME_START			(WM_USER+401)						//�˿˲���
#define IDM_CARD_MOVE			(WM_USER+402)						//�ƶ�����

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CDrawSiceWnd : public CWnd
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
	CDrawSiceWnd();
	//��������
	virtual ~CDrawSiceWnd();

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