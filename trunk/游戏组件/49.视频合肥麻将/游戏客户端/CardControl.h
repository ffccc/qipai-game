#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"
#include "cassert"
//////////////////////////////////////////////////////////////////////////
//	�˿˶���

//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
//	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
//	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//����
//	0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,							//����

//////////////////////////////////////////////////////////////////////////

//�˿˶���
#define HEAP_FULL_COUNT				34									//����ȫ��

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

//����ö��
enum enDirection
{
	Direction_East,					//����
	Direction_South,				//����
	Direction_West,					//����
	Direction_North,				//����
};

//////////////////////////////////////////////////////////////////////////

//�˿�ͼƬ
class CCardListImage
{
	//λ�ñ���
protected:
	int								m_nItemWidth;						//����߶�
	int								m_nItemHeight;						//������
	int								m_nViewWidth;						//����߶�
	int								m_nViewHeight;						//������

	//��Դ����
protected:
	CSkinImage						m_CardListImage;					//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardListImage();
	//��������
	virtual ~CCardListImage();

	//��Ϣ����
public:
	//��ȡ���
	int GetViewWidth() { return m_nViewWidth; }
	//��ȡ�߶�
	int GetViewHeight() { return m_nViewHeight; }

	//��Դ����
public:
	//������Դ
	bool LoadResource(UINT uResourceID, int nViewWidth, int nViewHeight);
	//�ͷ���Դ
	bool DestroyResource();

	//���ܺ���
public:
	//��ȡλ��
	inline int GetImageIndex(BYTE cbCardData);
	//�滭�˿�
	inline bool DrawCardItem(CDC * pDestDC, BYTE cbCardData, int xDest, int yDest);
};

//////////////////////////////////////////////////////////////////////////

//�˿���Դ
class CCardResource
{
	//�û��˿�
public:
	CSkinImage						m_ImageUserTop;						//ͼƬ��Դ
	CSkinImage						m_ImageUserLeft;					//ͼƬ��Դ
	CSkinImage						m_ImageUserRight;					//ͼƬ��Դ
	CCardListImage					m_ImageUserBottom;					//ͼƬ��Դ

	//�����˿�
public:
	CCardListImage					m_ImageTableTop;					//ͼƬ��Դ
	CCardListImage					m_ImageTableLeft;					//ͼƬ��Դ
	CCardListImage					m_ImageTableRight;					//ͼƬ��Դ
	CCardListImage					m_ImageTableBottom;					//ͼƬ��Դ

	//�ƶ��˿�
public:
	CSkinImage						m_ImageBackH;						//ͼƬ��Դ
	CSkinImage						m_ImageBackV;						//ͼƬ��Դ
	CSkinImage						m_ImageHeapSingleV;					//ͼƬ��Դ
	CSkinImage						m_ImageHeapSingleH;					//ͼƬ��Դ
	CSkinImage						m_ImageHeapDoubleV;					//ͼƬ��Դ
	CSkinImage						m_ImageHeapDoubleH;					//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardResource();
	//��������
	virtual ~CCardResource();

	//���ܺ���
public:
	//������Դ
	bool LoadResource();
	//������Դ
	bool DestroyResource();
};

//////////////////////////////////////////////////////////////////////////

//�����˿�
class CHeapCard
{
	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	//�˿˱���
protected:
	WORD							m_wFullCount;						//ȫ����Ŀ
	WORD							m_wMinusHeadCount;					//ͷ����ȱ
	WORD							m_wMinusLastCount;					//β����ȱ

	//��������
public:
	//���캯��
	CHeapCard();
	//��������
	virtual ~CHeapCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount);

	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//����˿�
class CWeaveCard
{
	//���Ʊ���
protected:
	bool							m_bDisplayItem;						//��ʾ��־
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���
	BYTE							m_cbDirectionCardPos;				//�����˿�				

	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	BYTE							m_cbCardData[4];					//�˿�����

	//��������
public:
	//���캯��
	CWeaveCard();
	//��������
	virtual ~CWeaveCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�滭�˿�
	void DrawCardControl(CDC * pDC, int nXPos, int nYPos);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);

	//�ؼ�����
public:
	//������ʾ
	void SetDisplayItem(bool bDisplayItem) { m_bDisplayItem=bDisplayItem; }
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
	//������
	void SetDirectionCardPos(BYTE cbPos) { assert(cbPos <= 3); m_cbDirectionCardPos = cbPos;}


	//�ڲ�����
private:
	//��ȡ�˿�
	inline BYTE GetCardData(WORD wIndex);
};

//////////////////////////////////////////////////////////////////////////

//�û��˿�
class CUserCard
{
	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	bool							m_bCurrentCard;						//��ǰ��־

	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	//��������
public:
	//���캯��
	CUserCard();
	//��������
	virtual ~CUserCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool SetCurrentCard(bool bCurrentCard);
	//�����˿�
	bool SetCardData(WORD wCardCount, bool bCurrentCard);

	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//�����˿�
class CDiscardCard
{
	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	BYTE							m_cbCardData[22];					//�˿�����

	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	//��������
public:
	//���캯��
	CDiscardCard();
	//��������
	virtual ~CDiscardCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool AddCardItem(BYTE cbCardData);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);
	//��ȡλ��
	CPoint GetLastCardPosition();

	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//�����˿�
class CTableCard
{
	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	BYTE							m_cbCardData[14];					//�˿�����

	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	//��������
public:
	//���캯��
	CTableCard();
	//��������
	virtual ~CTableCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);

	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//�˿˽ṹ
struct tagCardItem
{
	bool							bShoot;								//�����־
	BYTE							cbCardData;							//�˿�����
};

//�˿˿ؼ�
class CCardControl
{
	//״̬����
protected:
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayItem;						//��ʾ��־

	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	WORD							m_wHoverItem;						//��������
	tagCardItem						m_CurrentCard;						//��ǰ�˿�
	tagCardItem						m_CardItemArray[13];				//�˿�����

	//��Դ����
protected:
	CSize							m_ControlSize;						//�ؼ���С
	CPoint							m_ControlPoint;						//�ؼ�λ��

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//��ѯ����
public:
	//��ȡ�˿�
	BYTE GetHoverCard();
	//��ȡ�˿�
	BYTE GetCurrentCard() { return m_CurrentCard.cbCardData; }

	//�˿˿���
public:
	//�����˿�
	bool SetCurrentCard(BYTE cbCardData);
	//�����˿�
	bool SetCurrentCard(tagCardItem CardItem);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard);
	//�����˿�
	bool SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount);

	//�ؼ�����
public:
	//������Ӧ
	void SetPositively(bool bPositively) { m_bPositively=bPositively; }
	//������ʾ
	void SetDisplayItem(bool bDisplayItem) { m_bDisplayItem=bDisplayItem; }
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�¼�����
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����Ϣ
	bool OnEventSetCursor(CPoint Point, bool & bRePaint);

	//�ڲ�����
private:
	//�����л�
	WORD SwitchCardPoint(CPoint & MousePoint);
};

//////////////////////////////////////////////////////////////////////////

//��������
//��������
extern CCardResource				g_CardResource;						//�˿���Դ

//////////////////////////////////////////////////////////////////////////

#endif