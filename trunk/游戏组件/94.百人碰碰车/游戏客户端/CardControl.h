#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��������
#define INVALID_ITEM				0xFFFF								//��Ч����

//���Զ���
#define MAX_CARD_COUNT				41									//�˿���Ŀ
#define SPACE_CARD_DATA				255									//����˿�

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//��ඨ��
#define DEF_X_DISTANCE				17									//Ĭ�ϼ��
#define DEF_Y_DISTANCE				17									//Ĭ�ϼ��
#define DEF_SHOOT_DISTANCE			20									//Ĭ�ϼ��

//��ඨ��
#define DEF_X_DISTANCE_SMALL		17									//Ĭ�ϼ��
#define DEF_Y_DISTANCE_SMALL		17									//Ĭ�ϼ��
#define DEF_SHOOT_DISTANCE_SMALL	20									//Ĭ�ϼ��

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

//////////////////////////////////////////////////////////////////////////

//�˿˽ṹ
struct tagCardItem
{
	bool							bShoot;								//�����־
	BYTE							cbCardData;							//�˿�����
};

//////////////////////////////////////////////////////////////////////////
struct TMoveCard
{
	bool							m_blMoveCard;
	bool							m_blMoveFinish;
	TMoveCard()
	{
		memset(this,0,sizeof(this));
	}

};
//�˿˿ؼ�
class CCardControl
{
public:
		CPoint							m_MovePoint;
		CPoint							m_DownPoint;
		TMoveCard						m_tMoveCard;
		bool							m_blAuto;
		bool							m_blGameEnd;
		bool							m_blhideOneCard;
		bool							m_blAdd;
		bool							m_blShowResult;
		bool							m_blShowLineResult;

	//״̬����
protected:
	bool							m_bSmallMode;						//С��ģʽ
	bool							m_bHorizontal;						//��ʾ����
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayItem;						//��ʾ��־
	bool							m_bShowCardControl;					//��ʾ�˿�



	//�˿�����
public:
	WORD							m_wCardCount;	

	CArrayTemplate<tagCardItem> m_CardItemArray;
	CArrayTemplate<tagCardItem> m_CardTempItemArray;
	//�˿���Ŀ
	//tagCardItem						m_CardItemArray[MAX_CARD_COUNT];	//�˿�����
	//tagCardItem						m_CardTempItemArray[MAX_CARD_COUNT];	//�˿�����

	//ѡ�����
protected:
	WORD							m_wEndIndex;						//��������
	WORD							m_wStartIndex;						//��ʼ����

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
	CSkinImage	static				m_ImageCard;						//ͼƬ��Դ
	CSkinImage				m_ImageOpenCardHandle;				//ͼƬ��Դ  IDB_HAND
	DWORD							m_dwDragMax;						//������Զ����
	DWORD							m_dwCurrentIndex;

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//�˿˿���
public:
	//�����˿�
	bool SetCardData(WORD wCardCount);
	//����ģʽ
	bool SetCardMode(bool bSmallMode);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount,bool blTmp = true);
	//�����˿�
	bool SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount);

	//�����˿�
public:
	//�����˿�
	void ShootAllCard(bool bShoot);
	//�����˿�
	bool SetShootIndex(WORD wStartIndex, WORD wEndIndex);
	//�����˿�
	bool SetSelectIndex(WORD wStartIndex, WORD wEndIndex);
	//�����˿�
	bool SetShootCard(const BYTE cbCardData[], WORD wCardCount);

	//�˿˿���
public:
	//ɾ���˿�
	bool RemoveShootItem();
	//�ƶ��˿�
	bool MoveCardItem(WORD wTargerItem);
	//�����˿�
	bool SwitchCardItem(WORD wSourceItem, WORD wTargerItem);

	//��ȡ�˿�
public:
	//��ȡ�˿�
	tagCardItem * GetCardFromIndex(WORD wIndex);
	//��ȡ�˿�
	tagCardItem * GetCardFromPoint(CPoint & MousePoint);

	//��ȡ�˿�
public:
	//�˿���Ŀ
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
	VOID SetDisplayFlag(bool bDisplayItem) { m_bDisplayItem=bDisplayItem; }
	//���þ���
	VOID SetCardSpace(UINT nXDistance, UINT nYDistance, UINT nShootDistance);
	//��ʾ�˿�
	void ShowCardControl(bool bShow){m_bShowCardControl=bShow;}

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

	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint wndPoint);

	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint wndPoint);
	//���
	afx_msg void OnMouseMove(UINT nFlags, CPoint wndPoint);



	//�ڲ�����
public:
	//��ȡ��С
	VOID GetControlSize(CSize & ControlSize,WORD dwCardCount = 0);
	//��ȡԭ��
	VOID GetOriginPoint(CPoint & OriginPoint);
	//�����л�
	WORD SwitchCardPoint(CPoint & MousePoint);

	void OnCopyCard();
};

//////////////////////////////////////////////////////////////////////////

#endif