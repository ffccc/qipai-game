#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_OUT_PROMPT				(WM_USER+103)						//��ʾ����
#define IDM_LAST_TURN_CARD			(WM_USER+104)						//���ְ�ť
#define IDM_TRUSTEE_CONTROL			(WM_USER+105)						//�йܿ���
#define IDM_SORT_CARD				(WM_USER+106)						//�����˿�

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
	bool							m_bLastTurnCard;					//���ֱ�־

	//״̬����
protected:
	LONG							m_lGameScore[3];					//��Ϸ�÷�
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_cbWinTaxis[GAME_PLAYER];			//��Ϸ����
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ

	//λ����Ϣ
protected:
	CPoint							m_PointWin[GAME_PLAYER];			//����λ��
	CPoint							m_PointUserPass[GAME_PLAYER];		//����λ��

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageCenter;						//������Դ
	CSkinImage						m_ImageBanker;						//ׯ����Դ
	CSkinImage						m_ImageUserPass;					//������־
	CSkinImage						m_ImageScoreInfo;					//�÷���Ϣ
	CSkinImage						m_ImageWinTaxis;					//������Ϣ
	CSkinImage						m_ImageLastTurnTip;					//������ʾ
	CSkinImage						m_ImageReady;						//׼��ͼ��
	
	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btOutPrompt;						//��ʾ��ť
	CSkinButton						m_btSortCard;						//�����˿�
	CSkinButton						m_btLastTurn;						//�����˿�
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl[2];				//�����˿�
	CCardControl					m_UserCardControl[4];				//�˿���ͼ
	CCardControl					m_OtherCardControl[2];				//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
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

	//���ú���
public:
	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//���ܺ���
public:
	//��������
	void SetLastTurnCard(bool bLastTurnCard);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//���õ÷�
	void SetUserScore(WORD wIndex, LONG lScore);
	//��������
	void SetUserTaxis(WORD wChairID, BYTE cbTaxis);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//��ʾ��ť
	afx_msg void OnOutPrompt();
	//���а�ť
	afx_msg void OnSortCard();
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
	//���ְ�ť
	afx_msg void OnBnClickedLastTurn();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
