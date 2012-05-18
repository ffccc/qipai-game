
#pragma once

#include "GameCard.h"


//
//�˿���
//
class  CGameCardEx : public CWnd
{
	//ö�ٶ���
public:
	enum XShowMode { SWX_MODE_LEFT, SWX_MODE_CENTER, SWX_MODE_RIGHT };
	enum YShowMode { SWY_MODE_TOP,  SWY_MODE_CENTER, SWY_MODE_BOTTOM }; 

	//��������
protected:
	int								m_iCardHight;					//�˿˸߶�
	int								m_iCardWidth;					//�˿˿��
	int								m_iCardHSpace;					//�˿˼��
	int								m_iCardVSpace;					//�˿˼��
	int								m_iCardUpSpace;					//����߶�
	CRgn							m_CardViewRgn;					//�˿�����
	CWnd							* m_pHandleWnd;					//������

	int								m_iCardHSpaceAdd;				//�������
	BYTE							m_bAddMaxCnt;					//���Ӹ���
	BYTE							m_bAddPosArray[5];				//�Ӿ�λ��


	//��Ϣ����
protected:
	bool							m_bShowCard;					//��ʾ�˿�
	bool							m_bHorzLook;					//�Ƿ���
	bool							m_bUseMouse;					//��Ӧ���
	BYTE							m_bHotHitCard;					//�����˿�
	BYTE							m_bCardCount;					//�˿���Ŀ
	CPoint							m_PtBasePoint;					//��׼����
	XShowMode						m_XShowMode;					//��ʾģʽ
	YShowMode						m_YShowMode;					//��ʾģʽ
	CByteArray						m_CardArray;					//�˿�����
	CByteArray						m_CardUpArray;					//�˿�����

	//ͼƬ����
protected:
	CSkinImage						m_CardPic[2];					//�˿˿ؼ�

	//��������
public:
	//���캯��
	CGameCardEx();
	//��������
	virtual ~CGameCardEx();

	//���ú���
public:
	//��ʼ������
	bool InitGameCard(CWnd * pHandleWnd);
	//�����˿˲���
	void SetCardParameter(int iHSpace, int iVSpace, int iUpSpace);
	//���û�׼�㺯��
	void SetBasePoint(CPoint & BasePoint, XShowMode XShowMode, YShowMode YShowMode);
	//������ʾ�˿�
	void SetShowCard(bool bShow);
	//����ʹ�����
	void SetUseMouse(bool bUserMouse);
	//������ʾ��ʽ
	void SetLookMode(bool bHorzLook);
	//�����������
	void SetAddParameter(BYTE bAddPos[],BYTE bAddCnt);


	//���ܺ���
public:
	//�����˿�
	bool SetCard(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount);
	//ɾ��������˿�
	BYTE RemoveUpCard();
	//��ȡ������˿�
	BYTE GetUpCard(BYTE bCardList[]);
	//��ȡ�˿���Ϣ
	BYTE GetCard(BYTE bCardList[], BYTE bUpList[]);

	//��������
public:
	//��ȡ�˿���Ŀ
	inline BYTE GetCardCount() { return m_bCardCount; }
	//��ȡ�˿���ֵ
	inline BYTE GetCardValoe(BYTE bCard) { 
		BYTE bkk=(bCard&UG_VALUE_MASK)+1;
		if (bkk==14) bkk=1;
		return bkk;  }
	//��ȡ�˿˻�ɫ
	inline BYTE GetHuaKind(BYTE bCard) { return bCard&UG_HUA_MASK; }
	//��ȡ�˿�λ��
	inline BYTE GetCardXPos(BYTE bCard) { return GetCardValoe(bCard)-1; };
	//��ȡ�˿�λ��
	inline BYTE GetCardYPos(BYTE bCard) { return GetHuaKind(bCard)>>4; };

	//�ڲ�����
protected:
	//��������
	BYTE HitCardTest(CPoint & Point);
	//�õ��Ƶ�����λ��
	BYTE GetCardIndexPos(int pos);
	//��������λ��
	void FixCardControl();
	//��ȡ��С����
	bool GetCardSize(CSize & Size);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


