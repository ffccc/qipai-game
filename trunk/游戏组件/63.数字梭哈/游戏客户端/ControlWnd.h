#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

#define IDM_ADD_SCORE_CONTROL		WM_USER+201

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
	//��������
protected:
	LONG							m_lScoreMax;						//�����Ŀ
	LONG							m_lScoreLess;						//������Ŀ

	//״̬����
protected:
	WORD							m_wItemCount;						//������Ŀ
	LONG							m_lScoreCurrent;					//��ǰ��Ŀ
	WORD							m_wCurrentItem;						//��ǰ����

	//�����ߴ�
protected:
	INT								m_ControlHead;						//ͷ�����
	INT								m_ControlTail;						//β�����

	//�ؼ�λ��
protected:
	CSize							m_SizeItem;							//����ߴ�
	CSize							m_SizeControl;						//�ؼ���С
	CPoint							m_BenchmarkPos;						//��׼λ��

	//�ؼ�����
protected:
	CSkinButton						m_btMaxScore;						//������
	CSkinButton						m_btLessScore;						//��С����

	//��Դ����
protected:
	CSkinImage						m_ImageControlL;					//������Դ
	CSkinImage						m_ImageControlM;					//������Դ
	CSkinImage						m_ImageControlR;					//������Դ
	CSkinImage						m_ImageArrow;						//�����ͷ
	CSkinImage						m_ImageNumber;						//����
	CSkinImage						m_ImageNumberDot;					//����

	//��������
public:
	//���캯��
	CControlWnd();
	//��������
	virtual ~CControlWnd();

	//���ܺ���
public:
	//����λ��
	VOID SetBenchmarkPos(INT nXPos, INT nYPos);
	//��ȡ��Ŀ
	LONGLONG GetScoreCurrent() { return m_lScoreCurrent; }
	//������Ϣ
	VOID SetScoreControl(LONG lScoreMax, LONG lScoreLess, LONG lScoreCurrent);

	//�ڲ�����
protected:
	//��������
	VOID PlayGoldSound();
	//ת������
	WORD SwitchToIndex(CPoint MousePoint);

	//��ť��Ϣ
protected:
	//���ť
	VOID OnBnClickedScoreMax();
	//��С��ť
	VOID OnBnClickedScoreLess();
	//�ų���ť
	VOID OnBnClickedSendScore();

	//��Ϣӳ��
protected:
	//�滭��Ϣ
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnRButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
