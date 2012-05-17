#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//ö�ٶ���

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//��Ϣ����
#define	IDM_LEFT_HIT_CARD			(WM_USER+500)						//����˿�
#define	IDM_RIGHT_HIT_CARD			(WM_USER+501)						//�һ��˿�
#define IDM_SHOW_VALUE				WM_USER+105							//������Ϣ
#define IDM_SHOW_INFO				WM_USER+106							//��ʾ��Ϣ
#define IDM_OX_ENABLE				WM_USER+107							//ţţ��ť
#define IDM_SORT					WM_USER+108							//�˿˷���

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
	bool							m_bKey;								//��ݼ�
	bool							m_bOX;								//ţţ��־
	//bool							m_bScot;							//�����־

	//�������
protected:
	DWORD							m_dwCardHSpace;						//������
	DWORD							m_dwCardVSpace;						//������
	DWORD							m_dwShootAltitude;					//����߶�
	DWORD							m_dwDrawHeight;						//�滭�߶�	

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
	CGameLogic						m_GameLogic;						//�߼�����

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
	//�滭����
	void SetDrawHeight(DWORD dwDrawHeight);
	//����ţţ
	void SetOX(bool bOX);
	//���ÿ�ݼ�
	void SetKey(bool Key);
	//�����˿�
	void ShootAllCard(bool bShoot);
	//������ʾ
	void SetDisplayFlag(bool bDisplay);
	//���÷���
	void SetDirection(bool bHorizontal);
	//������Ӧ
	void SetPositively(bool bPositively);
	//���ü��
	void SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace=18, DWORD dwShootAltitude=18);
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�˿˿���
public:
	//��ȡ��ݼ�
	bool GetKey(){return m_bKey;}
	//��ȡţţ
	bool GetOX(){return m_bOX;}
	//��ȡ��Ŀ
	DWORD GetCardCount();
	//��ȡ�����˿���Ŀ
	DWORD GetShootCount();
	//��ȡ�˿�
	DWORD GetShootCard(BYTE bCardData[], bool bShoot=true);
	//�����˿�
	DWORD SetCardData(const BYTE bCardData[], DWORD dwCardCount);
	//��ȡ�˿�
	DWORD GetCardData(BYTE cbCardData[], DWORD dwMaxCount);
	//��ȡλ��
	CPoint GetCardPos();
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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
