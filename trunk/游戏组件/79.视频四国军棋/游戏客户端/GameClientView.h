#pragma once

#include "Stdafx.h"
#include "ChessBorad.h"
#include "DlgUserRequest.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_PLACEMENT				(WM_APP+100)						//��ɲ���
#define WM_HIT_CHESS_BORAD			(WM_APP+101)						//�������
#define WM_CHANGE_CURSOR			(WM_APP+102)						//�ı����
#define WM_RESET_GAME				(WM_APP+103)						//������Ϸ
#define WM_SAVE_PLACEMENT			(WM_APP+104)						//���沼��
#define WM_LOAD_PLACEMENT			(WM_APP+105)						//װ�ز���
#define	IDM_LOAD_HISTORY			(WM_APP+106)						//�������
#define	IDM_PREV_STEP				(WM_APP+107)						//��һ����
#define	IDM_NEXT_STEP				(WM_APP+108)						//��һ����
#define	IDM_TURN_CHESS				(WM_APP+109)						//��ת����
#define IDM_LOSE					(WM_APP+110)						//������Ϣ
#define IDM_GIVEUP					(WM_APP+111)						//������Ϣ
#define IDM_PEACE					(WM_APP+112)						//������Ϣ


//////////////////////////////////////////////////////////////////////////////////


//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��������
protected:
	BYTE							m_bFrameX ;							//�߿�λ��
	BYTE							m_bFrameY ;							//�߿�λ��
	bool							m_bDeasilOrder;						//����˳��
	LONG							m_lCurrentStepCount;				//��ǰ����

	//�������
protected:
	CSkinImage						m_ImageBack;						//����ͼ��


	//�ؼ�����
public:
	CChessBorad						m_ChessBoard;						//��Ϸ����
	CDlgUserRequest					m_PeaceRequest;						//��������

	CSkinButton						m_btPlacement;						//���ְ�ť
	CSkinButton						m_btSavePlacement;					//���沼��
	CSkinButton						m_btLoadPlacement;					//���밴ť
	CSkinButton						m_btResetGame;						//������Ϸ
	CSkinButton						m_btLoadHistory;					//�������
	CSkinButton						m_btPrevStep;						//��һ����
	CSkinButton				    	m_btNextStep;						//��һ����
	CSkinButton						m_btTurn;							//��ת����
	CSkinButton						m_btLose;							//���䰴ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btPeace;							//���尴ť

	CScoreView						m_ScoreView;						//������ͼ

		#ifdef VIDEO_GAME
	//��Ƶ���
private:
	CVideoServiceManager			m_VedioServiceManager;				//��Ƶ����
	CVideoServiceControl			m_DlgVedioService[GAME_PLAYER];		//��Ƶ���
#endif

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
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ܺ���
public:
	//��������
	void ShowPeaceRequest();
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos) ;
	//�滭�߿�
	void DrawChessFrame(BYTE bXPos , BYTE bYPos) ;
	//����˳��
	void SetUserOrder(bool bDeasilOrder) ;
	//����˳��
	bool IsDeasilOrder(bool bDeasilOrder) ;
	//�õ��߿�
	void GetChessFrame(BYTE &bXPos , BYTE &bYPos) ;
	//���ò���
	void SetStepCount(LONG lStepCount) ;

	//�ڲ����� 
private:
	//����״̬
	void UpdateUserInfoArea();

	//�ؼ���Ϣ
protected:
	//���ְ�ť
	void OnHitPlacement();
	//������Ϸ
	void OnHitResetGame() ;
	//���沼��
	void OnSavePlacement() ;
	//װ�ز���
	void OnHitLoadPlacement() ;
	//�������
	void OnHitLoadHistory();
	//��һ����			
	void OnHitPrevStep();
	//��һ����				
	void OnHitNextStep();
	//��ת����				
	void OnHitTurn();	
	//���䰴ť
	void OnHitLose();
	//������ť
	void OnHitGiveUp();
	//�������
	void OnHitPeace();

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
