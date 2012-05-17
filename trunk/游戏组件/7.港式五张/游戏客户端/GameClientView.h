#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "JettonControl.h"
#include "GoldControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��Ϣ����
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_NO_ADD					WM_USER+101							//������Ϣ
#define IDM_FOLLOW					WM_USER+102							//��ע��Ϣ
#define IDM_GIVE_UP					WM_USER+103							//������Ϣ
#define IDM_ADD_SCORE				WM_USER+104							//��ע��Ϣ
#define IDM_SHOW_HAND				WM_USER+105							//�����Ϣ
#define IDM_SEND_CARD_FINISH		WM_USER+106							//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CArrayTemplate<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//���ݱ���
protected:
	LONG							m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	LONG							m_lCellScore;						//��Ԫע

	//��������
protected:
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_SendCardCurPos;					//���Ƶ�ǰλ��
	INT								m_nStepCount;						//����
	INT								m_nXStep;							//����
	INT								m_nYStep;							//����
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btNoAdd;							//���Ӱ�ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btFollow;							//��ע��ť
	CSkinButton						m_btShowHand;						//�����ť
	CSkinButton						m_btAddScore;						//��ע��ť

	//λ����Ϣ
protected:
	CPoint							m_ptTableScore[GAME_PLAYER];		//��ǰ��ע��Ϣ

	//�ؼ�����
public:
	CScoreView						m_ScoreView;						//�ɼ�����
	CCardControl					m_CardControl[GAME_PLAYER];			//�û��˿�
	CJettonControl					m_PlayerJeton[GAME_PLAYER+1];		//��ҳ���
	CGoldControl					m_GoldControl;						//��ע��

	//�������
protected:
	CSkinImage						m_ImageCard;						//�˿���Դ
	CSkinImage						m_ImageViewBack;					//������Դ
	CSkinImage						m_ImageViewFill;					//�����Դ
	CPngImage						m_PngNumber;						//����λͼ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

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
	//�����ж�
	bool IsDispatchCard();
	//��ɷ���
	void FinishDispatchCard();
	//�����˿�
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//������ע
	void SetUserTableScore(WORD wChairID, LONG lTableScore);
	//���õ�Ԫע
	void SetCellScore( LONG lCellScore );

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//test
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
