#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�˿˿ؼ�
class CCardControl
{
	//״̬����
protected:
	bool							m_bHorizontal;						//��ʾ����
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayItem;						//��ʾ��־

	//�˿�����
protected:
	BYTE							m_cbCardCount;						//�˿���Ŀ
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
	CSkinImage						m_ImageCard;						//ͼƬ��Դ
	CSkinImage						m_ImageCardMask;					//��Ч��ͼ
	CSkinImage						m_ImageWin;							//ʤ����־

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//�˿˿���
public:
	//�����˿�
	bool SetCardData(BYTE cbCardCount);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	bool SetShootCard(const BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	bool SetCardItem(const tagCardItem CardItemArray[], BYTE cbCardCount);
	//������Ч
	bool SetCardEffect(const BYTE cbCardData[],BYTE cbCardCount);
	//���ñ�־
	bool SetMyCard(const BYTE cbCardData[],BYTE cbCardCount);

	//��ȡ�˿�
public:
	//��ȡ�˿�
	tagCardItem * GetCardFromIndex(BYTE cbIndex);
	//��ȡ�˿�
	tagCardItem * GetCardFromPoint(CPoint & MousePoint);

	//��ȡ�˿�
public:
	//�˿�����
	WORD GetCardCount() { return m_cbCardCount; }
	//��ȡ�˿�
	WORD GetCardData(BYTE cbCardData[], BYTE cbBufferCount);
	//��ȡ�˿�
	WORD GetShootCard(BYTE cbCardData[], BYTE cbBufferCount);
	//��ȡ�˿�
	WORD GetCardData(tagCardItem CardItemArray[], BYTE wBufferCount);

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
	//��ȡ��С
	VOID GetControlSize(CSize & ControlSize);
	//��ȡ����
	VOID GetCenterPoint(CPoint & CenterPoint);
	//��׼λ��
	VOID SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	VOID SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�¼�����
public:
	//�滭�˿�
	VOID DrawCardControl(CDC * pDC,bool bEffect);
	//�����Ϣ
	bool OnEventSetCursor(CPoint Point);

	//�ڲ�����

	//��ȡԭ��
	VOID GetOriginPoint(CPoint & OriginPoint);
	//�����л�
	WORD SwitchCardPoint(CPoint & MousePoint);
};

//////////////////////////////////////////////////////////////////////////

#endif