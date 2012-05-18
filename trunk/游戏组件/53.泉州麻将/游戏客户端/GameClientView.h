#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_AUTO					(WM_USER+102)						//����ť
#define IDM_OPEN_DOOR				(WM_USER+103)						//������Ϣ
#define IDM_OPEN_GOLD				(WM_USER+104)						//������Ϣ


//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��־����
protected:
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	bool							m_bHuangZhuang;						//��ׯ��־
	POINT							m_ptGold;
	CSize							m_csGlod;							//���ͼ��С	
	bool							m_bAutoEnable;						//�����־
	bool							m_bGoldCardShow;					//������ʾ	
	bool							m_bGoldOutEnable;					//����ʹ��

	//��Ϸ����
protected:
	LONG							m_lCellScore;						//��������
	WORD							m_wBankerUser;						//ׯ���û�

	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����˿�
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbUserAction[5];					//�û�����

	//���ӱ���
protected:
	WORD							m_wDoorDice;						//���ŵ���
	WORD							m_wGoldDice;						//�������


	//λ�ñ���
protected:
	CPoint							m_UserFlagPos[4];					//��־λ��

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageWait;						//�ȴ���ʾ
	CSkinImage						m_ImageOutCard;						//������ʾ
	CSkinImage						m_ImageUserFlag;					//�û���־
	CSkinImage						m_ImageUserAction;					//�û�����
	CSkinImage						m_ImageActionBack;					//��������
	CSkinImage						m_ImageBirdBack;					//��ͼ����
	CSkinImage						m_ImageHuangZhuang;					//��ׯ��־
	CSkinImage						m_ImageGold;						//��Bλ��
	CSkinImage						m_BackGround;						//��ɫ
	CSkinImage						m_ImageChuiFlag;					//����ͼ��
	CSkinImage						m_ImageGoldCard;					//���Ʊ���

	//�˿˿ؼ�
public:
	CHeapCard						m_InsideHeapCard[4];				//�����˿�
	CHeapCard						m_OutSideHeapCard[4];				//�����˿�
	CUserCard						m_UserCard[3];						//�û��˿�
	CTableCard						m_TableCard[4];						//�����˿�
	CWeaveCard						m_WeaveCard[4][5];					//����˿�
	CDiscardCard					m_DiscardCard[4];					//�����˿�
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_GoldCardControl;					//�����˿�
	

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	//CSkinButton						m_btAutoOn;							//����ON��ť
	//CSkinButton						m_btAutoOff;						//����OFF��ť
	CControlWnd						m_ControlWnd;						//���ƴ���
	CGameScoreWnd					m_GameScoreWnd;						//��������
	CSkinButton						m_btOpenDoor;						//���Ű�ť
	CSkinButton						m_btOpenGold;						//����ť

	//��ʾ�ؼ�
public:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�


	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);
	void  SetImageGold( CDC *pDC, BYTE byPlayer, const tagUserData* pUserData );
	void SetArtText( CDC *pDC, int x, int y, COLORREF nColor, COLORREF nBkColor, LPCTSTR lpszString );

	//���ܺ���
public:
	//��������
	void SetCellScore(LONG lCellScore);
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//��ׯ����
	void SetHuangZhuang(bool bHuangZhuang);
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//����ON��ť
	afx_msg void OnAutoOn();
	//����OFF��ť
	afx_msg void OnAutoOff();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//������Ϣ
	afx_msg void OnOpenDoor();
	//������Ϣ
	afx_msg void OnOpenGold();

	DECLARE_MESSAGE_MAP()

public:
	
	// ������ʾ
	void SetImageTing( CDC *pDC, BYTE byPlayer);
	//���ý�����ʾ
	void SetGoldCardShow(bool bGoldCardShow);
	//����ʹ��
	void SetGoldOutEnable(bool bGoldOutEnable){m_bGoldOutEnable=bGoldOutEnable;}
	//���ŵ���
	void SetDoorDice(WORD wDoorDice){m_wDoorDice=wDoorDice;}
	//�������
	void SetGoldDice(WORD wGoldDice){m_wGoldDice=wGoldDice;}
	
	
};

//////////////////////////////////////////////////////////////////////////
