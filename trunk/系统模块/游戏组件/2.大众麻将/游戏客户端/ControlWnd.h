#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_LISTEN_CARD				(WM_USER+300)						//�˿˲���
#define IDM_CARD_OPERATE			(WM_USER+301)						//�˿˲���

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
	//���ñ���
protected:
	BYTE							m_cbActionMask;						//��������
	BYTE							m_cbCenterCard;						//�����˿�
	BYTE							m_cbGangCard[5];					//��������
	CPoint							m_PointBenchmark;					//��׼λ��

	//״̬����
protected:
	BYTE							m_cbItemCount;						//������Ŀ
	BYTE							m_cbCurrentItem;					//��ǰ����

	//�ؼ�����
protected:
	CSkinButton						m_btChiHu;							//�Ժ���ť
	CSkinButton						m_btListen;							//���ư�ť
	CSkinButton						m_btGiveUp;							//������ť

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
	//�����˿�
	void SetControlInfo(BYTE cbCenterCard, BYTE cbActionMask, tagGangCardResult & GangCardResult);

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
	afx_msg void OnListen();
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