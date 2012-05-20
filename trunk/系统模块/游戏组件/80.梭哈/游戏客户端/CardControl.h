#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft, 
	enXCenter,
	enXRight,
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,
	enYCenter,
	enYBottom,
};

//////////////////////////////////////////////////////////////////////////

//�˿˿ؼ�
class CCardControl : public CWnd
{
	//״̬����
protected:
	bool								m_bShowFirst;					//��ʾ�˿�
	bool								m_bPositively;					//��Ӧ��־
	bool								m_bCaptureMouse;				//�����־
	CArrayTemplate<BYTE>				m_CardDataArray;				//�˿�����

	//λ����Ϣ
protected:
	CRgn								m_CardRgn;						//�˿�����
	CPoint								m_BasicStation;					//��׼λ��
	enXCollocateMode					m_XCollocateMode;				//��ʾģʽ
	enYCollocateMode					m_YCollocateMode;				//��ʾģʽ

	//��̬����
protected:
	static bool							m_bInit;						//��ʼ��־
	static CSkinImage					m_ImageCard;					//�˿�ͼƬ
	static CSkinImage					m_ImageBack;					//�˿�ͼƬ

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//���ܺ���
public:
	//��ȡ��Ŀ
	WORD GetCardCount();
	//��ʾ�˿�
	void ShowFirstCard(bool bShowFirst);
	//������Ӧ
	void AllowPositively(bool bPositively);
	//�����˿�
	WORD GetCardData(BYTE bCardBuffer[], WORD wBufferSize);
	//�����˿�
	bool SetCardData(const BYTE bCardData[], WORD wCardCount);
	//���û�׼�㺯��
	void SetBasicStation(CPoint & BasicStation, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��ȡ��βλ��
	CPoint GetTailPos();

	//�ڲ�����
private:
	//�˿���ֵ
	BYTE GetCardValue(BYTE bCardData);
	//�˿˻�ɫ
	BYTE GetHuaKind(BYTE bCardData) { return (bCardData-1)/13; };
	//����λ��
	void RectifyControl();	
	//�˿�λ��
	WORD GetCardXPos(BYTE bCardData);
	//�˿�λ��
	WORD GetCardYPos(BYTE bCardData);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif