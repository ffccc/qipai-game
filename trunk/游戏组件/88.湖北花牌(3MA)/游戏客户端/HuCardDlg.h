#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"


//��ʾ���ƶԻ���
class CHuCardDlg :public CWnd
{
protected:
	CSkinImage		    m_ImageBack;				//������Դ
	tagHuCardWeave      m_HuCardWeave;              //��Ѻ������ 
	BYTE                m_bHuScore;                 //���Ʒ���
	BYTE                m_bGoldCard;                //�������
	CPoint	     		m_PointBenchmark;			//��׼λ��

	CSkinButton			m_btChiHu;					//�Ժ���ť
	CSkinButton			m_btGiveUp;					//������ť

	CSkinImage			m_ImageControlLeft;			//��ԴͼƬ
	CSkinImage			m_ImageControlMid;			//��ԴͼƬ
	CSkinImage			m_ImageControlRight;		//��ԴͼƬ

	//��������
public:
	//���캯��
	CHuCardDlg(void);

	//��������
	virtual ~CHuCardDlg(void);


	//�ڲ�����
protected:
	//�����ؼ�
	void RectifyControl();

	//���ܺ���
public:
	//���ú�����Ϣ
	void SetHuCardWeave(tagHuCardWeave HuCardWeave,BYTE HuScore,BYTE GoldCard);
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos);

	//�ػ�����
	afx_msg void OnPaint();

	//�Ժ���ť
	afx_msg void OnChiHu();
	//������ť
	afx_msg void OnGiveUp();

	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
