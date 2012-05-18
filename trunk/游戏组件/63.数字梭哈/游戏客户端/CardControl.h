#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//���Զ���
#define MAX_CARD_COUNT				5									//�˿���Ŀ
#define DEF_X_DISTANCE				17									//Ĭ�ϼ��

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

#define MAX_MOVE_Y					40
#define MOVE_STEP_X					5
#define MOVE_STEP_Y					3

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

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

//
struct tagCardItem
{
	BYTE cbCardData;
	bool bDisplay;
};

//////////////////////////////////////////////////////////////////////////

//�˿˿ؼ�
class CCardControl
{
	//״̬����
protected:
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayHead;						//��ʾ��־

	//��������
protected:
	bool							m_bMoving;							//
	INT								m_nYStep;							//
	INT								m_nYMove;							//
	BYTE							m_cbShowIndex;						//

	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	tagCardItem						m_CardItem[MAX_CARD_COUNT];			//�˿�����

	//�������
protected:
	INT								m_nXDistance;						//������
	INT								m_nXOrgDistance;					//

	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//��Դ����
protected:
	CSize							m_CardSize;							//�˿˴�С
	CSkinImage						m_ImageCard;						//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//�˿˿���
public:
	//�˿���Ŀ
	WORD GetCardCount() { return m_wCardCount; }
	//��ȡ�˿�
	WORD GetCardData(BYTE cbCardData[], WORD wBufferCount);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);

	//״̬��ѯ
public:
	//��ȡ��С
	CSize GetCardSize() { return m_CardSize; }
	//��ѯ��Ӧ
	bool GetPositively() { return m_bPositively; }
	//��ѯ��ʾ
	bool GetDisplayHead() { return m_bDisplayHead; }

	//״̬����
public:
	//���þ���
	VOID SetXDistance(UINT nXDistance);
	//������Ӧ
	VOID AllowPositively(bool bPositively);
	//������ʾ
	VOID ShowFirstCard(bool bDisplayHead);
	//��ȡ��βλ��
	CPoint GetTailPos();
	//
	bool BeginMoveCard();
	//
	bool PlayMoveCard();
	//
	void SetDispalyCard( WORD wCardIndex,bool bDisplay );
	
	//�ؼ�����
public:
	//��׼λ��
	VOID SetBasicStation(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�¼�����
public:
	//�滭�˿�
	VOID DrawCardControl(CDC * pDC);
	//�����Ϣ
	bool OnEventSetCursor(CPoint Point);
	//�����Ϣ
	bool OnEventLeftMouseDown(CPoint Point);
	//
	bool OnEventLeftMouseUp(CPoint Point);

	//�ڲ�����
private:
	//��ȡ��С
	VOID GetControlSize(CSize & ControlSize);
	//�����л�
	WORD SwitchCardPoint(CPoint & MousePoint);
	//��ȡԭ��
	VOID GetOriginPoint(CPoint & OriginPoint);
};

//////////////////////////////////////////////////////////////////////////

#endif
