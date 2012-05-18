#pragma once

#include "Stdafx.h"
#include "GameCard.h"
#include "NtList.h"
#include "SmallGameCard.h"
#include "HintInfo.h"
#include "ScoreView.h"
#include "GameCardEx.h"


//////////////////////////////////////////////////////////////////////////
#define IDM_BEGIN			WM_USER+120					//��ʼ��ť��Ϣ
#define	IDM_HINT			WM_USER+121					//��ʾ��Ϣ
#define IDM_GIVE_BACK		WM_USER+123					//�����ư�ť
#define	IDM_CHANGE_BACK		WM_USER+131					//����
#define	IDM_GIVE_CARD		WM_USER+119					//����
#define IDM_OUT_CARD		WM_USER+124					//�û�����
#define IDM_LOOK_LAST		WM_USER+125					//�������˿�
#define IDM_BACK_CARD		WM_USER+126					//������Ϣ
#define	IDM_GIVE_UP			WM_USER+127					//�����ȴ�����

#define IDM_AI  			WM_USER+130					//��ǰ������ť
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;
protected:
	//////////////////////////
	//�ؼ�
	CSkinButton		m_btBegin;						//��ʼ
	CSkinButton		m_btOutCard;					//����
	CSkinButton		m_btHint;						//��ʾ


	CSkinButton		m_btAI;							//�й�
	CSkinButton		m_btLastTurn;					//��һ��

	//��ͼ
	CSkinImage		m_ImageBack;					//��Ϸ����
	CSkinImage		m_ImageBackCenter;				//���ı���

	CSkinImage		m_ImageNT;						//ׯ�ұ�ʶ
	CSkinImage		m_ImageNTLogo;					//������ʶ
	CSkinImage		m_ImageScore;					//����
	CSkinImage		m_ImagePublicCard;				//�����Ʊ���

	CSkinImage		m_ImageFlower;					//����

	CSkinImage		m_ImageFullTimer;				//��ʱ����
	CSkinImage		m_ImageEmptyTimer;				//��
	float			m_Timer_Scale;					//ʱ��

	//��Ϣλ��
	POINT			m_PtNt[GAME_PLAYER];			//ׯ�ұ�ʶλ��
	POINT			m_PtNtLogo[GAME_PLAYER];		//����

	POINT			m_PtScoreCard[GAME_PLAYER];		//����λ��

	POINT			m_PtPublicCard;					//����λ��

public:
	CRect			m_PlayRect;						//��һ����
	CRect			m_Rct_DymTimer;					//ʱ��������
	CRect			m_Rct_Info;						//��ʾ��Ϣ����

public:
	int				m_nCurScore[GAME_PLAYER];		//��������ķ���

	bool			m_bSunShi;						//�Ƿ�˳ʱ��

	WORD			m_wBankerChair;					//ׯ����λ������������λ��
	WORD			m_wViewBankerChair;				//��ͼλ��

	WORD			m_wPublicChair;					//���Ƶ���λ
	WORD			m_wViewPublicChair;				//��ͼλ��
	BYTE			m_bPublicCard;					//���ƻ�ɫ

	CGameCardEx		m_MeCard;						//�ҵ��˿�
	CGameCard		m_OutCard[GAME_PLAYER];			//�����˿�
	CSmallGameCard	m_PublicCard;					//������


	CSmallGameCard	m_Score_Card[GAME_PLAYER];		//����				

	CNtList			m_NtList;
	CHintInfo		m_dlgHintInfo;					//��ʾ��Ϣ
	CScoreWnd       m_ScoreWnd;						//������


	//�������
protected:

	//�ؼ�����
public:

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int cx, int cy);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	
	//������Դ
	void	LoadResource();
public:
	//�������а�ť
	void	HideAllButton();
	//��ʾ������Ϣ
	void	SetShowInfo(const CString &str,bool bIsShow);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnHitBegin();
	afx_msg void OnHitBackCard();

	afx_msg void OnHitOutCard();
	afx_msg void OnHint();
	afx_msg void OnHitLastTurn();
	afx_msg void OnHitLookBack();
	afx_msg void OnQuit_();
	afx_msg void OnAI();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
