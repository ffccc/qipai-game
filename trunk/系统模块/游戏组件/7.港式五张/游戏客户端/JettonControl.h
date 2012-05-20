#ifndef JETTON_CONTROL_HEAD_FILE
#define JETTON_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���Զ���
#define JETTON_COUNT				14									//������Ŀ

//�滭ģʽ
enum enDrawMode
{
	enDrawMode_Layer,			//���ģʽ
	enDrawMode_Rand				//���ģʽ
};

//�滭������Ϣ
struct tagJettonDraw
{
	WORD	wDrawCount;
	WORD	wDrawIndex;
};

//////////////////////////////////////////////////////////////////////////

//����ؼ�
class CJettonControl
{
	//��������
protected:
	LONG							m_lScore;							//������Ŀ
	LONG							m_lDrawScore;						//
	CPoint							m_BenchmarkPos;						//��׼λ��
	enDrawMode						m_DrawMode;							//�滭ģʽ

	//λͼ����
protected:
	CPngImage						m_PngJetton;						//����λͼ

	//λ�ñ���
protected:
	CArrayTemplate<tagJettonDraw>	m_JetonDraw;						//�滭����
	CArrayTemplate<CPoint>			m_DrawExcusions;					//�滭λ��

	//��������
protected:
	//�����ؼ�
	void RectifyControl();
public:
	//���캯��
	CJettonControl();
	//��������
	virtual ~CJettonControl();

	//���ܺ���
public:
	//��ȡ����
	LONG GetScore() { return m_lScore; }
	//���ó���
	VOID SetScore(LONG lScore);
	//��ע
	void AddScore(LONG lScore);
	//���û滭ģʽ
	void SetDrawMode( enDrawMode DrawMode );

	//λ�ú���
public:
	//��׼λ��
	VOID SetBenchmarkPos(INT nXPos, INT nYPos);
	//��ȡλ��
	CPoint GetBenchmarkPos() { return m_BenchmarkPos; }

	//���ƺ���
public:
	//�滭�ؼ�
	VOID DrawJettonControl(CDC * pDC);
};

//////////////////////////////////////////////////////////////////////////

#endif