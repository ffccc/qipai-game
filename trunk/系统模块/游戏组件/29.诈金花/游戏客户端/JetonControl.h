#ifndef JETON_CONTROL_HEAD_FILE
#define JETON_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//λ�ö���
#define JETOM_IMAGE_USER				0								//����λ��
#define JETOM_IMAGE_SECOND				1								//����λ��
#define JETOM_IMAGE_THIRDLY				2								//����λ��
#define JETOM_IMAGE_FOURTHLY			3								//����λ��
#define JETOM_IMAGE_FIFTHLY				4								//����λ��

//////////////////////////////////////////////////////////////////////////

//����ؼ�
class CJetonControl
{
	//״̬����
protected:
	LONG							m_lJeton;							//������Ŀ

	//�滭����
protected:
	int								m_nImagePos;						//����λ��
	CPoint							m_BenchmarkPos;						//��׼λ��

	//��������
protected:
	static bool						m_bLoad;							//��ʼ��־
	static CSkinImage				m_ImageJeton;						//����ͼƬ

	//��������
public:
	//���캯��
	CJetonControl();
	//��������
	virtual ~CJetonControl();

	//���ܺ���
public:
	//��ȡ����
	LONG GetJeton();
	//���ó���
	void SetJeton(LONG lJeton);
	//��������
	void SetImagePos(int nImagePos);

	//�滭����
public:
	//�滭����
	void DrawJetonControl(CDC * pDC);
	//�滭λ��
	bool SetBenchmarkPos(int nXPos, int nYPos);
};

//////////////////////////////////////////////////////////////////////////

#endif