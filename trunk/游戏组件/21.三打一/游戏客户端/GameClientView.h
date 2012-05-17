#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"


//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_LAND_SCORE				(WM_USER+103)						//�з���Ϣ
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ

#define IDM_SORT_BY_NUM				(WM_USER+106)						//����������
#define IDM_SORT_BY_SIZE			(WM_USER+107)						//����С����

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�����û�
	WORD							m_wBombTime;						//ը������
	LONG							m_lBaseScore;						//��������
	BYTE							m_cbLandScore;						//��������

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��

	//״̬����
public:
	bool							m_bLandTitle;						//������־
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bScore[GAME_PLAYER];				//�û��з�
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bMaxBombCount[GAME_PLAYER];		//��������
	BYTE							m_bOutBombCount[4];					//����ը����
	bool							m_bMaxBombCountShow;				//����ʾ�ĳ�����

	//λ����Ϣ
public:
	CSize							m_LandSize;							//�ڿӱ�־
	CPoint							m_ptScore[GAME_PLAYER];				//�з�λ��
	CPoint							m_ptLand[GAME_PLAYER];				//����λ��
	CPoint							m_ptBomb[GAME_PLAYER];				//ը��λ��

	//λͼ����
protected:
	CSkinImage						m_ImageBomb;						//ը��ͼƬ
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageFill;
	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btOneScore;						//1 �ְ�ť
	CSkinButton						m_btTwoScore;						//2 �ְ�ť
	CSkinButton						m_btThreeScore;						//3 �ְ�ť
	CSkinButton						m_btShowCard;						//���ư�ť
	CSkinButton						m_btGiveUpScore;					//������ť

	CSkinButton						m_btSortByNum;						//���������а�ť
	CSkinButton						m_btSortBySize;						//����С���а�ť

	//�˿˿ؼ�
public:
	CCardControl					m_BackCardControl;					//�����˿�
	CCardControl					m_UserCardControl[4];				//�˿���ͼ
	CCardControl					m_HandCardControl[4];				//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ

#ifdef VIDEO_GAME
	CVideoServiceControl			m_DlgVideoService[4];				//��Ƶ����
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

#ifdef VIDEO_GAME
	void RectifyVideoView(int nWidth, int nHeight);
#endif

	//���ú���
public:
	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//���ܺ���
public:
	//ը������
	void SetBombTime(WORD wBombTime);
	//��������
	void SetBaseScore(LONG lBaseScore);
	//��ʾ��ʾ
	void ShowLandTitle(bool bLandTitle);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//�ڿӷ���
	void SetLandUser(WORD wChairID, BYTE bLandScore);
	//�ڿӷ���
	void SetLandScore(WORD wChairID, BYTE bLandScore);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//�зְ�ť
	afx_msg void OnOneScore();
	//�зְ�ť
	afx_msg void OnTwoScore();
	//�зְ�ť
	afx_msg void OnThreeScore();
	//�зְ�ť
	afx_msg void OnShowCard();
	//������ť
	afx_msg void OnGiveUpScore();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//����������ť
	afx_msg void OnSortByNum();
	//����С����ť
	afx_msg void OnSortBySize();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
