#ifndef DRAW_WND_HEAD_FILE
#define DRAW_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
#define IDM_SICE_FINISH			(WM_USER+401)						//�˿˲���
#define IDM_CARD_MOVE			(WM_USER+402)						//�ƶ�����
#define IDM_SICE_TWO			(WM_USER+403)						//�ƶ�����

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CDrawSiceWnd : public CWnd
{
	//���ñ���
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageDrawSiceFirst;				//��������
	CSkinImage						m_ImageDrawSiceSecond;				//
	CSkinImage						m_ImageDrawSiceUserFirst;				//��������
	CSkinImage						m_ImageDrawSiceUserSecond;				//��������
	BYTE                            m_bSicePos;
	int m_nXPos;
	int m_nYPos;
	CDC      *ParentDC;
	CWnd							*m_pSinkWnd;						//֪ͨ����
	//��������
public:
	//���캯��
	CDrawSiceWnd();
	//��������
	virtual ~CDrawSiceWnd();

	//�ڲ�����
public:
	//����ɫ����Ϣ
	void SetSiceInfo(CDC *dc,BYTE bMoveTime,WORD wSice1,WORD wSice2);
	//���û�׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos);
	//����֪ͨ����
	void SetWindowSink(CWnd *pSinkWnd);
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
