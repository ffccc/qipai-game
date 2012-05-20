#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////

//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};

//////////////////////////////////////////////////////////////////////////

//�˿˿ؼ�
class CCardControl : public CWnd
{
	//���ñ���
protected:
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayHead;						//��ʾ�˿�
	DWORD							m_dwCardHSpace;						//������

	//״̬����
protected:
	bool							m_bCaptureMouse;					//�����־
	CArrayTemplate<BYTE>			m_CardDataArray;					//�˿�����

	//λ����Ϣ
protected:
	CRgn							m_CardRegion;						//�˿�����
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//��̬����
protected:
	static bool						m_bLoad;							//��ʼ��־
	static CSize					m_CardSize;							//�˿˴�С
	static CSkinImage				m_ImageCard;						//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//���ƺ���
public:
	//���ü��
	void SetCardSpace(DWORD dwCardSpace);
	//������Ӧ
	void SetPositively(bool bPositively);
	//��ʾ�˿�
	void SetDisplayHead(bool bDisplayHead);
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��ȡ��βλ��
	CPoint GetTailPos();

	//���ܺ���
public:
	//��ȡ��Ŀ
	DWORD GetCardCount();
	//�����˿�
	DWORD GetCardData(BYTE cbCardData[], DWORD dwMaxCount);
	//�����˿�
	DWORD SetCardData(const BYTE bCardData[], DWORD dwCardCount);

	//�ڲ�����
private:
	//����λ��
	void RectifyControl();

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
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif