#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CallCardWnd.h"
#include "CardControl.h"
#include "ConcealCardView.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��ť��Ϣ
#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_OUT_PROMPT				(WM_USER+102)						//��ʾ��Ϣ
#define IDM_SEND_CONCEAL			(WM_USER+103)						//������Ϣ

//���ܰ�ť
#define IDM_CONCEAL_CARD			(WM_USER+110)						//�鿴����
#define IDM_REQUEST_LEAVE			(WM_USER+111)						//�����뿪
#define IDM_LAST_TURN_CARD			(WM_USER+112)						//�����˿�
#define IDM_TRUSTEE_CONTROL			(WM_USER+113)						//�йܿ���
#define IDM_RESPONSE_LEAVE			(WM_USER+114)						//�����뿪��Ӧ

//test
#define IDM_TEST					(WM_USER+15)	

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��ʾ����
protected:
	bool							m_bWaitConceal;						//�ȴ���־
	bool							m_bLastTurnCard;					//�����˿�

	//��Ϸ����
protected:
	WORD							m_wCardScore;						//��ҵ÷�
	WORD							m_wBankerUser;						//ׯ���û�
	bool							m_bStrustee[GAME_PLAYER];			//�йܱ�־
	//������Ϣ
protected:
	BYTE							m_cbCallColor;						//���ƻ�ɫ
	BYTE							m_cbCallCount;						//������Ŀ
	BYTE							m_cbMainValue;						//������ֵ
	WORD							m_wCallCardUser;					//�����û�
	BYTE							m_cbValueOrder[2];					//������ֵ

	//�÷���Ϣ
protected:
	WORD							m_cbScoreCardCount;					//�˿���Ŀ
	BYTE							m_cbScoreCardData[2*12];			//�˿�����

	//λ����Ϣ
protected:
	CPoint							m_PointBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptStrustee[GAME_PLAYER];			//�йܱ�־

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageCenter;						//���б���
	CSkinImage						m_ImageBanker;						//ׯ����Դ
	CSkinImage						m_ImageLastTurn;					//���ֱ�־
	CSkinImage						m_ImageColorFlag;					//��ɫ��־
	CSkinImage						m_ImageWaitConceal;					//�ȴ���Դ
	CSkinImage						m_ImageStatusInfo;					//�÷�״̬
	CSkinImage						m_ImageScoreCard;					//�÷���
	CPngImage						m_PngStrustee;						//�йܱ�־

	//��Ƶ�ؼ�
public:
#ifdef VIDEO_GAME
	CVideoServiceControl			m_DlgVedioService[4];				//��Ƶ����
#endif

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btOutPrompt;						//��ʾ��ť
	CSkinButton						m_btSendConceal;					//���װ�ť

	//���ܰ�ť
public:
	CSkinButton						m_btConcealCard;					//�鿴����
	CSkinButton						m_btRuquestLeave;					//�����뿪
	CSkinButton						m_btLastTurnCard;					//�����˿�
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_UserCardControl[4];				//�˿���ͼ

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CCallCardWnd					m_CallCardWnd;						//���ƴ���
	CConcealCardView				m_ConcealCardView;					//���ƴ���

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

	//��Ϸ��Ϣ
public:
	//���õ÷�
	void SetCardScore(WORD wCardScore);
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//���õȴ�
	void SetWaitConceal(bool bWaitConceal);
	//��������
	void SetLastTurnCard(bool bLastTurnCard);
	//��������
	void SetValueOrder(BYTE cbMainValue, BYTE cbValueMySelf, BYTE cbValueOther);
	//��������
	void SetCallCardInfo(WORD wCallCardUser, BYTE cbCallColor, BYTE cbCallCount);
	//���õ÷���
	void SetScoreData( const BYTE cbScoreCard[],BYTE cbCardCount );
	//�����й�״̬
	void SetStrustee( WORD wChairId,bool bStrustee );

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);


	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//��ʾ��ť
	afx_msg void OnOutPrompt();
	//���װ�ť
	afx_msg void OnSendConceal();
	//�鿴����
	afx_msg void OnConcealCard();
	//�����뿪
	afx_msg void OnRuquestLeave();
	//�����˿�
	afx_msg void OnLastTurnCard();
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
