#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "ScoreControl.h"
#include "HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ͼλ��
#define MYSELF_VIEW_ID				2									//��ͼλ��

//������Ϣ
#define IDM_START					(WM_USER+200)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+201)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+202)						//PASS��Ϣ
#define IDM_OUT_PROMPT				(WM_USER+203)						//��ʾ��Ϣ
#define	IDM_LEFT_HIT_CARD			(WM_USER+206)						//����˿�
#define IDM_SORT_HAND_CARD			(WM_USER+207)						//�����˿�
#define IDM_LAST_TURN_CARD			(WM_USER+208)						//�����˿�
#define IDM_TRUSTEE_CONTROL			(WM_USER+209)						//�йܿ���

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	LONG							m_lCellScore;						//��Ԫ����
	BYTE							m_cbMainValue;						//������ֵ
	BYTE							m_cbValueOrder[GAME_PLAYER];		//�ȼ���ֵ
	bool							m_bAutoPlayer[GAME_PLAYER];			//�й��û�
	int								m_TurnScore;						//һ�ַ���
	int								m_PlayerScore[GAME_PLAYER];			//��ҷ���

	//�û�״̬
protected:
	bool							m_bUserPass[GAME_PLAYER];			//������־
	bool							m_bUserContinue[GAME_PLAYER];		//������־

	//��Ϸ״̬
protected:
	WORD							m_wWinOrder[GAME_PLAYER];			//ʤ���б�
	WORD							m_wPersistInfo[GAME_PLAYER][2];		//��Ϸ��Ϣ
	int								m_nLButtonUpCount;					//�������

	//��ը����
protected:
	bool							m_bBombEffect;						//��ըЧ��
	BYTE							m_cbBombFrameIndex;					//֡������

	//�ƶ�����
protected:
	WORD							m_wHoverCardItem;					//��������
	WORD							m_wMouseDownItem;					//�������
	bool							m_bMoveMouse;						//�ƶ���־
	bool							m_bSwitchCard;						//������־
	bool							m_bSelectCard;						//ѡ���־

	CPoint							m_ptAuto[GAME_PLAYER];

	//״̬����
public:
	bool							m_bShowScore;						//����״̬
	bool							m_bLastTurnCard;					//���ֱ�־

	//���ư�ť
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btStustee;						//�Ϲܿ���
	CSkinButton						m_btLastTurn;						//�����˿�
	CSkinButton						m_btSortCardCount;					//����ť
	CSkinButton						m_btSortCardColor;					//����ť
	CSkinButton						m_btSortCardOrder;					//����ť

	CSkinButton						m_btSort;							//���ư�ť

	//��Ϸ��ť
public:
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//PASS��ť
	//CSkinButton						m_btOutPrompt;						//��ʾ��ť

	//��Ϸ�ؼ�
public:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CScoreControl					m_ScoreControl;						//���ֿؼ�

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl[GAME_PLAYER];		//�û��˿�
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�˿���ͼ

	//λ�ñ���
protected:
	CPoint							m_ptHeap[GAME_PLAYER];				//�˿�λ��
	CPoint							m_ptPass[GAME_PLAYER];				//����λ��

	//��Դ����
protected:
	CSkinImage						m_ImageNumber;						//������Դ
	CSkinImage						m_ImageViewFill;					//������Դ
	CSkinImage						m_ImageViewBack;					//������Դ
	CSkinImage						m_ImageUserPass;					//������Դ
	CSkinImage						m_ImageBombEffect;					//ը��Ч��
	CSkinImage						m_ImageValueOrder;					//������Դ
	CSkinImage						m_ImageStatusInfo;					//״̬��Ϣ
	CSkinImage						m_ImageLastTurnTip;					//������ʾ
	CSkinImage						m_ImageTurnScore;

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

	//���ƽӿ�
public:
	//���ý���
	virtual void ResetGameView();

	//�̳к���
protected:
	//�����ؼ�
	virtual void RectifyGameView(INT nWidth, INT nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

	//���ܺ���
public:
	//��Ԫ����
	bool SetCellScore(LONG lCellScore);
	//������ֵ
	bool SetMainValue(BYTE cbMainValue);
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect);
	//��������
	VOID SetLastTurnCard(bool bLastTurnCard);
	//���÷���
	bool SetUserPass(WORD wChairID, bool bUserPass);
	//���ü���
	bool SetUserContinue(WORD wChairID, bool bContinue);
	//��������
	bool SetUserWinOrder(WORD wChairID, WORD wWinOrder);
	//������Ϣ
	bool SetUserPersist(WORD wChairID, WORD wPersistInfo[2]);
	//���õȼ�
	bool SetUserValueOrder(BYTE cbMySelfOrder, BYTE cbEnemyOrder);
	//������Ϸ����
	bool SetScoreInfo(int TurnScore,int PlayerScore[4]);
	//�й��û�
	void SetAutoUser(bool bAutoUser[4]);
	//�ڲ�����
private:
	//�滭����
	VOID DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos);

	//��ť��Ϣ
public:
	//��ʼ��ť
	VOID OnBnClickedStart();
	//������ť
	VOID OnBnClickedThrow();
	//�ع���ť
	VOID OnBnClickedIncep();
	//���ư�ť
	VOID OnBnClickedOutCard();
	//PASS��ť
	VOID OnBnClickedPassCard();
	//��ʾ��ť
	VOID OnBnClickedOutPrompt();
	//�Ϲܰ�ť
	VOID OnBnClickedStustee();
	//���ְ�ť
	VOID OnBnClickedLastTurn();
	//���ư�ť
	VOID OnBnClickedSort();
	//����ť
	VOID OnBnClickedSortCardColor();
	//����ť
	VOID OnBnClickedSortCardCount();
	//����ť
	VOID OnBnClickedSortCardOrder();

	//��Ϣ����
public:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnRButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif