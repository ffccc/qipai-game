#ifndef CHOOSE_WND_HEAD_FILE
#define CHOOSE_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_CHOOSE_CARD				(WM_USER+500)						//ѡ���˿�

//////////////////////////////////////////////////////////////////////////

//ѡ�񴰿�
class CChooseWnd : public CWnd
{
	//���ݱ���
protected:
	BYTE							m_bWeaveCount;						//�����Ŀ
	tagChiCardInfo					m_ChiCardInfo[6];					//������Ϣ

	//״̬����
protected:
	BYTE							m_cbCurrentIndex;					//��ǰ����

	//λ�ñ���
protected:
	CSize							m_SizeCell;							//��Ԫ��С
	CSize							m_SizeLeft;							//��ߴ�С
	CSize							m_SizeRight;						//�ұߴ�С
	CPoint							m_ControlPoint;						//�ؼ�λ��

	//��Դ����
protected:
	CSkinImage						m_ImageBackL;						//��ԴͼƬ
	CSkinImage						m_ImageBackM;						//��ԴͼƬ
	CSkinImage						m_ImageBackR;						//��ԴͼƬ
	CSkinImage						m_ImageBackH;						//��ԴͼƬ

	//��������
public:
	//���캯��
	CChooseWnd();
	//��������
	virtual ~CChooseWnd();

	//���ܺ���
public:
	//���û�׼
	void SetBenchmarkPos(int nXPos, int nYPos);
	//��������
	bool SetChooseWeave(tagChiCardInfo ChiCardInfo[6], BYTE cbWeaveCount);

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif