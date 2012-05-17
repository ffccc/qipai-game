#ifndef SMALL_CARD_CONTROL_HEAD_FILE
#define SMALL_CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#define INVALID_ITEM				0xFFFF								//��Ч����


//�˿˿ؼ�
class CSmallCardControl
{
	//״̬����
protected:
	bool							m_bHorizontal;						//��ʾ����
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayItem;						//��ʾ��־

	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	tagCardItem						m_CardItemArray[MAX_CARD_COUNT];	//�˿�����

	//�������
protected:
	UINT							m_nXDistance;						//������
	UINT							m_nYDistance;						//������
	UINT							m_nShootDistance;					//����߶�

	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//��Դ����
protected:
	CSize							m_CardSize;							//�˿˴�С
	static CSkinImage						m_ImageCard;						//ͼƬ��Դ

	//��������
public:
	//���캯��
	CSmallCardControl();
	//��������
	virtual ~CSmallCardControl();

	//�˿˿���
public:
	//�����˿�
	bool SetCardData(WORD wCardCount);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);
	//�����˿�
	bool SetShootCard(const BYTE cbCardData[], WORD wCardCount);
	//�����˿�
	bool SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount);

	//��ȡ�˿�
public:
	//��ȡ�˿�
	tagCardItem * GetCardFromIndex(WORD wIndex);
	//��ȡ�˿�
	tagCardItem * GetCardFromPoint(CPoint & MousePoint);

	//��ȡ�˿�
public:
	//�˿�����
	WORD GetCardCount() { return m_wCardCount; }
	//��ȡ�˿�
	WORD GetCardData(BYTE cbCardData[], WORD wBufferCount);
	//��ȡ�˿�
	WORD GetShootCard(BYTE cbCardData[], WORD wBufferCount);
	//��ȡ�˿�
	WORD GetCardData(tagCardItem CardItemArray[], WORD wBufferCount);

	//״̬��ѯ
public:
	//��ȡ��С
	CSize GetCardSize() { return m_CardSize; }
	//��ѯ����
	bool GetDirection() { return m_bHorizontal; }
	//��ѯ��Ӧ
	bool GetPositively() { return m_bPositively; }
	//��ѯ��ʾ
	bool GetDisplayItem() { return m_bDisplayItem; }

	//״̬����
public:
	//���÷���
	VOID SetDirection(bool bHorizontal) { m_bHorizontal=bHorizontal; }
	//������Ӧ
	VOID SetPositively(bool bPositively) { m_bPositively=bPositively; }
	//������ʾ
	VOID SetDisplayItem(bool bDisplayItem) { m_bDisplayItem=bDisplayItem; }
	//���þ���
	VOID SetCardDistance(UINT nXDistance, UINT nYDistance, UINT nShootDistance);

	//�ؼ�����
public:
	//��ȡ����
	VOID GetCenterPoint(CPoint & CenterPoint);
	//��׼λ��
	VOID SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	VOID SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�¼�����
public:
	//�滭�˿�
	VOID DrawCardControl(CDC * pDC);
	//�����Ϣ
	bool OnEventSetCursor(CPoint Point);
	/*static*/ void DrawOneCard(CDC* pDC, BYTE bCardData, int nX = 0, int nY = 0 );

	//�ڲ�����
public:
	//��ȡ��С
	VOID GetControlSize(CSize & ControlSize);
	//��ȡԭ��
	VOID GetOriginPoint(CPoint & OriginPoint);
	//�����л�
	WORD SwitchCardPoint(CPoint & MousePoint);
};

//////////////////////////////////////////////////////////////////////////

#endif