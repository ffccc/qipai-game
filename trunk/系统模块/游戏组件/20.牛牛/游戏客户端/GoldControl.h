#ifndef GOLD_CONTROL_HEAD_FILE
#define GOLD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��ҿ���
class CGoldControl : public CWnd
{
	//��������
protected:
	LONG								m_lMaxGold;						//�����
	LONG								m_lGoldCell[7];					//��Ԫ���
	CPoint								m_BasicPoint;					//����λ��

	//λ�ñ���
protected:
	int									m_nWidth;						//�ؼ����
	int									m_nCellCount;					//��Ԫ��Ŀ
	int									m_AppendWidth;					//���ӿ��

	//��Դ����
protected:
	CSkinImage							m_ImageLeft;					//������Դ
	CSkinImage							m_ImageMid;						//������Դ
	CSkinImage							m_ImageRight;					//������Դ

	//��������
public:
	//���캯��
	CGoldControl();
	//��������
	virtual ~CGoldControl();

	//���ܺ���
public:
	//��ȡ���
	LONG GetGold();
	//���ý��
	void SetGold(LONG lGold);
	//������ע
	void SetMaxGold(LONG lMaxGold);
	//����λ��
	void SetBasicPoint(int nXPos, int nYPos);

	//�ڲ�����
private:
	//�����ؼ�
	void RectifyControl();
	//�滭���
	void DrawGoldCell(CDC * pDC, int nXPos, int nYPox, LONG lGold);

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//���������Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�Ҽ�������Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//���ù��
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////

#endif
