#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_CARD_OPERATE			(WM_USER+300)						//�齫����

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
	//���ñ���
protected:
	BYTE							m_cbActionMask;						//��������
	BYTE							m_cbCenterCard;						//�����齫
	BYTE							m_cbGangCard[5];					//��������
	CPoint							m_PointBenchmark;					//��׼λ��

	//״̬����
protected:
	BYTE							m_cbItemCount;						//������Ŀ
	BYTE							m_cbCurrentItem;					//��ǰ����

	//�ؼ�����
protected:
	CSkinButton						m_btChiHu;							//�Ժ���ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btTingCard;						//���ư�ť

	//��Դ����
protected:
	CSkinImage						m_ImageControlTop;					//��ԴͼƬ
	CSkinImage						m_ImageControlMid;					//��ԴͼƬ
	CSkinImage						m_ImageControlButtom;				//��ԴͼƬ
	CSkinImage						m_ImageActionExplain;				//��������

	//��������
public:
	//���캯��
	CControlWnd();
	//��������
	virtual ~CControlWnd();

	//�ؼ�����
public:
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos);
	//�����齫
	void SetControlInfo(BYTE cbCenterCard, BYTE cbActionMask, tagGangCardResult & GangCardResult);

	//�������ư�ť
	//0=����  , 1=��ʾ
	void ShowHideTingCardButton(int opTypeInt );

	//�ڲ�����
protected:
	//�����ؼ�
	void RectifyControl();

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�Ժ���ť
	afx_msg void OnChiHu();

	//���ư�ť
	afx_msg void OnTingCard();

	//������ť
	afx_msg void OnGiveUp();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif