#ifndef GAME_VIEW_HEAD_FILE
#define GAME_VIEW_HEAD_FILE

#pragma once

#include "StdAfx.h"
#include "Ball.h"
#include "Station.h"
#include "GameThreadDraw.h"
#include "PeaceDlg.h"
#include "ScoreWnd.h"
#include "GameRuleSet.h"


#define BACK_COLOR				RGB(15,64,85)			//��ɫ

//״̬����
#define NO_STATE				0						//û��״̬
#define BALL_RUN_ING			1						//������
#define ME_HIT_BALL				2						//��Ҵ���
#define OTHER_HIT_BALL			3						//�Է�����
#define WATI_SET_BALL			4						//������λ��
#define  WIDTH_SCORE                275
#define  HEIGHT_SCORE               172


#define STATION_WND_HIGHT			80			//״̬���ڸ߶� 
#define PIC_BALL_RADII				21		//ͼ��뾶
#define ID_TIMER					10			//��ʱ�� 

class CGameClientView :public CGameFrameView
{
	friend class CGameClientDlg;
	friend class CStation;
	friend class CGameThreadDraw;

	//��������
public:
	bool					m_bMouseDown;				//����Ƿ���
	HCURSOR					m_hMouse;					//�����
	CScoreWnd					m_ScoreWnd;				//��ʾ�÷ֵĴ���
	//HANDLE					m_hGDIThreadHandle;			//�߳̾��

public:
	//
	//��������
	//
	CGameClientView(void);
	virtual ~CGameClientView(void);
	private:
	virtual void ResetGameView();
											//���ý���
	virtual void RectifyGameView(int nWidth, 
									int nHeight);
											//�����ؼ�
	virtual void DrawGameView(CDC * pDC, 
								int nWidth, 
								int nHeight);
											//�滭����
private:
	CSkinButton		m_GiveUp;
	CSkinButton		m_Replace;
	CSkinButton		m_Concession;
	CSkinButton		m_Begin;
	CPeaceDlg		m_PeaceDlg;				//��ͶԻ���
	//CGameRuleSet				m_RuleInfo;					//��������
	CGameThreadDraw     m_ThreadDraw;
	
	//CRect			m_Rct_Play;
	CRect           m_ClientRect;
	//�ؼ�����
public:
	CStation				m_Station;					//״̬����
		//��������
public:
	//��ȡ X ���ƫ��
	int GetXBase(); 
	//��ȡ Y ���ƫ��
	int GetYBase();

	//���غ���
public:

	//��Ϣ����
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//���ù��
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//�������
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//��갴��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void		OnBegin(void);
	void		OnGiveUp(void);
	void		OnReplace(void);
	void		OnConcession(void);
DECLARE_MESSAGE_MAP()

};


#endif
