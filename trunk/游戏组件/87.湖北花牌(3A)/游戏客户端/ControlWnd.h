#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_CARD_OPERATE			(WM_USER+300)						//�˿˲���

//////////////////////////////////////////////////////////////////////////

//������ʾ����--��ʾ�û��ɽ��еĲ��������ơ����С����С��Ĳء���غͷ���
class CControlWnd : public CWnd
{
	//���ñ���
protected:
	BYTE				m_cbActionMask;						//��������
	BYTE				m_cbCenterCard;						//�����˿�
	CPoint				m_PointBenchmark;					//��׼λ��

	//״̬����
protected:
	BYTE				m_cbItemCount;						//������Ŀ
	BYTE				m_cbCurrentItem;					//��ǰ����

	//�ؼ�����
protected:
	CSkinButton			m_btChiHu;							//�Ժ���ť
	CSkinButton			m_btGiveUp;							//������ť

	//��Դ����
protected:
	CSkinImage			m_ImageControlTop;					//��ԴͼƬ
	CSkinImage			m_ImageControlMid;					//��ԴͼƬ
	CSkinImage			m_ImageControlButtom;				//��ԴͼƬ
	CSkinImage			m_ImageActionExplain;				//��������

	BYTE                m_cbSiTongCard[9];                 //�Ĳص�������
	BYTE                m_cbsiTongHuaNm[9];                //�Ĳ����л��Ƶĸ���

	BYTE                m_cbWuTongCard[9];                 //��ص������� 
	BYTE                m_cbWuTongHuaNm[9];                //����л��Ƶĸ���
	
	BYTE                m_cbDuiHuaNm;                      //�����л��Ƶĸ���
	BYTE                m_cbSanZhaoNm;                     //�����л��Ƶĸ���
	BYTE                m_cbSiZhaoNm;                      //�����л��Ƶĸ���

//////////////////////////////////////////////////////////////////////////////////////
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

	//���ö��в�����������
	void SetDuiZhaoOperateCardData(BYTE cbCenterCard, BYTE cbActionMask,BYTE HuaCardIndex[]);

	//���ò��Ʋ�����������
	void SetTongOperateCardData(BYTE cbCenterCard, BYTE cbActionMask, tagSiTongResult &SiTongCardResult,tagWuTongResult &WuTongCardResult,BYTE HuaCardIndex[]);

	//�ڲ�����
protected:
	//�����ؼ�
	void RectifyControl();
	//��������ת����������
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�Ժ���ť
	afx_msg void OnChiHu();
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