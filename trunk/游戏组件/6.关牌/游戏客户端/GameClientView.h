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
#define IDM_TRUSTEE_CONTROL			(WM_USER+104)						//�йܿ���

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	LONG							m_lCellScore;						//��������
	BYTE							m_cbBombCount;						//ը����Ŀ
	bool							m_bAutoPlayer[GAME_PLAYER];			//�й��û�
	//״̬����
public:
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	CPoint							m_PointUserPass[GAME_PLAYER];		//����λ��
	CPoint							m_PointUserBomb[GAME_PLAYER];		//ը��λ��
	CPoint							m_ptAuto[GAME_PLAYER];

	//λͼ����
protected:
	CSkinImage						m_ImageBomb;						//ը����Դ
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageFill;
	CSkinImage						m_ImageUserPass;					//������־

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btOutPrompt;						//��ʾ��ť
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl[2];				//�����˿�
	CCardControl					m_UserCardControl[2];				//�˿���ͼ

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
#ifdef VIDEO_GAME
	CVideoServiceControl			m_DlgVideoService[2];				//��Ƶ����
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
	//ը����Ŀ
	void SetBombCount(BYTE cbBombCount);
	//��������
	void SetCellScore(LONG lCellScore);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);

	void SetAutoUser(bool bAutoUser[2]);
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
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
