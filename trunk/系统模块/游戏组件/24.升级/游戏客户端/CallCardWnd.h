#ifndef CALL_CARD_WND_HEAD_FILE
#define CALL_CARD_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ɫ����
#define CALL_NONE					0x00								//���ƶ���
#define CALL_NT						0x10								//���ƶ���
#define CALL_HEI_TAO				0x01								//���ƶ���
#define CALL_HONG_TAO				0x02								//���ƶ���
#define CALL_MEI_HUA				0x04								//���ƶ���
#define CALL_FANG_KUAI				0x08								//���ƶ���

//��Ϣ����
#define IDM_CALL_CARD				(WM_USER+200)						//������Ϣ

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CCallCardWnd : public CWnd
{
	//��ť�ؼ�
protected:
	CSkinButton						m_btCallNT;							//���ư�ť
	CSkinButton						m_btCallHeiTao;						//���ư�ť
	CSkinButton						m_btCallHongTao;					//���ư�ť
	CSkinButton						m_btCallMeiHua;						//���ư�ť
	CSkinButton						m_btCallFangKuai;					//���ư�ť

	//��������
public:
	//���캯��
	CCallCardWnd();
	//��������
	virtual ~CCallCardWnd();

	//���غ���
public:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//���¿���
	void UpdateCallControl(BYTE cbCallColor);
	//���¿���
	void UpdateCallControl(bool bCallNT, bool bCallHeiTao, bool bCallHongTao, bool CallMeiHua, bool bCallFangKuai);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif