#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "GameScoreWnd.h"

#include "TimerControl.h"
//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_SHOW_SZ				(WM_USER+102)						//show
#define IDM_TING_CARD	(WM_USER+103)						//������Ϣ

#define IDM_RESET_UI			(WM_USER+113)                       //ˢ��UI
#define MYSLEF_VIDEO_W    80
#define MYSLEF_VIDEO_H    60
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;
public:
	CPoint m_ptNameNew[GAME_PLAYER];
	int xOffInt;
	int yOffInt;


public:
	int									m_tDice;						//���Ӷ���ʱ��
	CTimerControl						m_Timer;						//ʱ�ӿؼ�
	//�������
void ShowTingPic( CDC *pDC,int x, int y );
bool							m_bTingFlag[GAME_PLAYER];				//���Ʊ�־
int		m_IntFirstTingFlag[GAME_PLAYER];//����ĵ�һ���� 
	//��־����
protected:
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	bool							m_bHuangZhuang;						//��ׯ��־

	//��Ϸ����
protected:
	LONG							m_lCellScore;						//��������
	WORD							m_wBankerUser;						//ׯ���û�

	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����齫
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbUserAction[GAME_PLAYER];					//�û�����

	int									m_nDiceCount1;					///������1
	int									m_nDiceCount2;					//������2
	//λ�ñ���
protected:
	CPoint							m_UserFlagPos[GAME_PLAYER];					//��־λ��
	CSize								m_csActionDice1;				//���Ӷ�����С
	CSize								m_csActionDice2;				//���Ӵ�С

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageWait;						//�ȴ���ʾ
	CSkinImage						m_ImageOutCard;						//������ʾ
	CSkinImage						m_ImageUserFlag;					//�û���־
	CSkinImage						m_ImageUserAction;					//�û�����
	CSkinImage						m_ImageActionBack;					//��������
	CSkinImage						m_ImageHuangZhuang;					//��ׯ��־

	CSkinImage						m_ImageTingBack;
	CSkinImage							m_ActionDice1;					//���Ӷ���
	CSkinImage							m_ActionDice2;					//���Ӷ�������
	//�齫�ؼ�
public:
	CHeapCard						m_HeapCard[GAME_PLAYER];						//�����齫
	CUserCard						m_UserCard[3];						//�û��齫
	CTableCard						m_TableCard[GAME_PLAYER];						//�����齫
	CWeaveCard						m_WeaveCard[GAME_PLAYER][GAME_PLAYER];					//����齫
	CDiscardCard					m_DiscardCard[GAME_PLAYER];					//�����齫
	CCardControl					m_HandCardControl;					//�����齫

	//�ؼ�����
public:
	CSkinButton						m_btTingCard;						//���ư�ť
	CSkinButton						m_btStart;							//��ʼ��ť
	CControlWnd						m_ControlWnd;						//���ƴ���
	CGameScoreWnd					m_GameScoreWnd;						//��������

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���Ӷ���
	void  ActionDice1( CDC *pDC,int nFrame);
	//���Ӷ���2
	void ActionDice2( CDC *pDC,int pos1, int pos2 );
	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

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
	//���ư�ť
	afx_msg void OnTingCard();
	//��ʼ��ť
	afx_msg void OnStart();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);//����ƶ�
	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
