

#pragma once


//��Ϣ����
#define		IDM_LEFT_HITCARD		(WM_USER+70)		//����˿�
#define		IDM_RIGHT_HITCARD		(WM_USER+75)		//�һ��˿�

#define IDM_REVERT_CARD		WM_USER+136					//����һ�ֵ�ʱ��ԭ�˿�

//��������
#define		UG_HUA_MASK				0xF0			//��ɫ����
#define		UG_VALUE_MASK			0x0F			//��������

#define		HEIGHT_CARD				95
#define		WIDTH_CARD				70


//
//�˿���
//
class  CGameCard : public CWnd
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
	CGameCard();
	//��������
	virtual ~CGameCard();

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


