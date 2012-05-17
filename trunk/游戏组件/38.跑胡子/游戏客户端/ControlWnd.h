#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_CARD_OPERATE			(WM_USER+300)						//�˿˲���

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
	//��������
protected:
	BYTE							m_cbCurrentCard;					//��ǰ�˿�

	//�ؼ�����
protected:
	CSkinButton						m_btChi;							//���ư�ť
	CSkinButton						m_btPeng;							//���ư�ť
	CSkinButton						m_btChiHu;							//�Ժ���ť
	CSkinButton						m_btGiveUp;							//������ť

	//��Դ����
protected:
	CSkinImage						m_ImageButtonBack;					//��ԴͼƬ

	//��������
public:
	//���캯��
	CControlWnd();
	//��������
	virtual ~CControlWnd();

	//���ܺ���
public:
	//�����˿�
	void SetControlInfo(BYTE cbCurrentCard, BYTE cbOperateCode);

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//���ư�ť
	afx_msg void OnChi();
	//���ư�ť
	afx_msg void OnPeng();
	//�Ժ���ť
	afx_msg void OnChiHu();
	//������ť
	afx_msg void OnGiveUp();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif