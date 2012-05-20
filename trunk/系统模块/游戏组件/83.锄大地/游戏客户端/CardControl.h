#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�˿˶���
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
//	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
//	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

//ö�ٶ���

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//��Ϣ����
#define	IDM_LEFT_HIT_CARD			(WM_USER+500)						//����˿�
#define	IDM_RIGHT_HIT_CARD			(WM_USER+501)						//�һ��˿�

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

//�˿˽ṹ
struct tagCardItem
{
	bool							bShoot;								//�����־
	BYTE							bCardData;							//�˿�����
};

//////////////////////////////////////////////////////////////////////////

//�˿˿ؼ�
class CCardControl : public CWnd
{
	//���ñ���
protected:
	bool							m_bDisplay;							//��ʾ��־
	bool							m_bHorizontal;						//��ű�־
	bool							m_bPositively;						//��Ӧ��־

	//�������
protected:
	DWORD							m_dwCardHSpace;						//������
	DWORD							m_dwCardVSpace;						//������
	DWORD							m_dwShootAltitude;					//����߶�

	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//���б���
protected:
	DWORD							m_dwCurrentIndex;					//�������

	//�ڲ�����
protected:
	CRgn							m_CardRegion;						//�ؼ�����
	CWnd *							m_pSinkWindow;						//�ص�����
	CArrayTemplate<tagCardItem>		m_CardDataItem;						//�˿�����

	//��Դ����
protected:
	static bool						m_bLoad;							//���ر�־
	static CSize					m_CardSize;							//�˿˴�С
	static CSkinImage				m_ImageCard;						//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//���ú���
public:
	//���ô���
	void SetSinkWindow(CWnd * pSinkWindow);

	//�ؼ�����
public:
	//�����˿�
	void ShootAllCard(bool bShoot);
	//������ʾ
	void SetDisplayFlag(bool bDisplay);
	//���÷���
	void SetDirection(bool bHorizontal);
	//������Ӧ
	void SetPositively(bool bPositively);
	//���ü��
	void SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace, DWORD dwShootAltitude);
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�˿˿���
public:
	//��ȡ��Ŀ
	DWORD GetCardCount();
	//��ȡ�˿�
	DWORD GetShootCard(BYTE bCardData[], DWORD dwMaxCount);
	//�����˿�
	DWORD SetCardData(const BYTE bCardData[], DWORD dwCardCount);
	//��ȡ�˿�
	DWORD GetCardData(tagCardItem CardItemArray[], DWORD dwMaxCount);
	//�����˿�
	DWORD SetCardItem(const tagCardItem CardItemArray[], DWORD dwCardCount);
	//���õ����˿�
	DWORD SetShootCard(const BYTE bCardData[], DWORD dwCardCount);

	//�ڲ�����
private:
	//����λ��
	void RectifyControl();
	//�����л�
	DWORD SwitchCardPoint(const CPoint & MousePoint);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
