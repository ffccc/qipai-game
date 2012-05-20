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
#define IDM_SORT_BY_COLOR			(WM_USER+104)						//��ɫ����
#define IDM_SORT_BY_VALUE			(WM_USER+105)						//��С����
#define IDM_STRUSTEE				(WM_USER+106)						//�й�

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//״̬����
public:
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	CPoint							m_PointCardHeap[GAME_PLAYER];		//�˿�λ��
	CPoint							m_PointUserPass[GAME_PLAYER];		//����λ��
	CPoint							m_ptStrustee[GAME_PLAYER];			//�йܱ�־

protected:
	bool							m_bStrustee[GAME_PLAYER];			//�йܱ�־

	//��ʷ����
public:
	DOUBLE                          m_lAllTurnScore[GAME_PLAYER];		//�ֵܾ÷�
	DOUBLE                          m_lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�

	//λͼ����
protected:
	CSkinImage						m_ImageCardHeap;					//�˿�λͼ
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewBack;					//����λͼ
	CSkinImage						m_ImageUserPass;					//������־
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
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btOutPrompt;						//��ʾ��ť
	CSkinButton						m_btSortByColor;					//��ɫ����
	CSkinButton						m_btSortByValue;					//��С����
	CSkinButton						m_btStrustee;						//�й�

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl[4];				//�����˿�
	CCardControl					m_UserCardControl[4];				//�˿���ͼ

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ

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
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//�����й�״̬
	void SetStrustee( WORD wChairId,bool bStrustee );

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
	//��ɫ����
	afx_msg void OnSortByColor();
	//��С����
	afx_msg void OnSortByValue();
	//�й�
	afx_msg void OnStrustee();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
