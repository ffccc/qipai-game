#ifndef CONCEAL_CARD_VIEW_HEAD_FILE
#define CONCEAL_CARD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CConcealCardView : public CWnd
{
	//������Ϣ
protected:
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];					//�����˿�

	//��Դ����
protected:
	CSize							m_CardSize;							//�˿˴�С
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageCard;						//�˿�λͼ

	//��������
public:
	//���캯��
	CConcealCardView();
	//��������
	virtual ~CConcealCardView();
	
	//���ܺ���
public:
	//������Ϣ
	void SetConcealCard(BYTE cbConcealCard[], BYTE cbConcealCount);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif