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
	LONG								m_lCellScore;					//��Ԫע
	LONG								m_lTimes;						//����

	//λ�ñ���
protected:
	CPoint								m_BenchmarkPos;					//��׼λ��

	//��Դ����
protected:
	CSkinImage							m_ImageBack;					//������Դ
	CSkinImage							m_ImageNumber;					//������Դ

	//��ť�ؼ�
protected:
	CSkinButton							m_btAddTimes1;					//1��
	CSkinButton							m_btAddTimes2;					//2��
	CSkinButton							m_btAddTimes3;					//3��
	CSkinButton							m_btAddTimes4;					//4��
	CSkinButton							m_btCancel;						//ȡ��

	//��������
public:
	//���캯��
	CGoldControl();
	//��������
	virtual ~CGoldControl();

	//���ܺ���
public:
	//���ó���
	void SetCellScore(LONG lCellSocre);
	//����λ��
	void SetBenchmarkPos(int nXPos, int nYPos);
	//���ñ���
	void SetAddTimes( LONG lTimes );
	//��ȡ����
	LONG GetAddTimes() { return m_lTimes; }

	//�ڲ�����
private:
	//�����ؼ�
	void RectifyControl();

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�����ؼ�
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//1����ť
	afx_msg void OnBnAddTimes1();
	//2����ť
	afx_msg void OnBnAddTimes2();
	//3����ť
	afx_msg void OnBnAddTimes3();
	//4����ť
	afx_msg void OnBnAddTimes4();
	//ȡ��
	afx_msg void OnBnCancel();

	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////

#endif