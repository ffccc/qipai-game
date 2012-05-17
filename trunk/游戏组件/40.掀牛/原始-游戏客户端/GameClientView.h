#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "..\��Ϸ������\GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_COUPAI					(WM_USER+103)						//������Ϣ
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ
#define IDM_XIANPAI					(WM_USER+105)						//������Ϣ
#define IDM_GIVEUP_QIANGCI			(WM_USER+106)						//��������
#define IDM_QIANGCI					(WM_USER+107)						//������Ϣ
#define IDM_FORETURN_CARD			(WM_USER+108)						//��һ����ʾ

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	WORD							m_wBanker;						//�����û�
	LONG							m_lBaseScore;						//��������
	BYTE							m_bZhanPaiCount[GAME_PLAYER];		//ռ������
protected:
	long							m_lForeScore[GAME_PLAYER];			//��һ�ֳɼ�
	long							m_lAllScore[GAME_PLAYER];			//�ܳɼ�
protected:
	static long							m_lTimeCount;					//��ʱ��

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��

	//״̬����
public:
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bScore[GAME_PLAYER];				//�û��з�
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	bool							m_bCouPaiTitle[GAME_PLAYER];		//���Ʊ�־
	bool							m_bXianPaiTitle[GAME_PLAYER];		//���Ʊ�־
	bool							m_bQiangCiTitle[GAME_PLAYER];		//���̱�־
protected:
	BYTE							m_bXianCount[GAME_PLAYER];			//��������
	BYTE							m_bBeiXianCount[GAME_PLAYER];		//��������

	//λ����Ϣ
public:
	CSize							m_LandSize;							//�ڿӱ�־
	CPoint							m_ptScore[GAME_PLAYER];				//�з�λ��
	CPoint							m_ptLand[GAME_PLAYER];				//����λ��
	CPoint							m_pPass[GAME_PLAYER];
	CPoint							m_pZhanPaiName[GAME_PLAYER];		//ռ���û���
	CPoint							m_pTimeFlag;						//ʱ���־

	//λͼ����
protected:
	CSkinImage						m_ImageBomb;						//ը��ͼƬ
	CSkinImage						m_ImageLand;						//ׯ��ͼƬ
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageScore;						//����ͼƬ
	CSkinImage						m_ImageCenter;						//������Դ
	CSkinImage						m_ImageLc;							//���ϽǱ�־
	CSkinImage						m_ImageHeadBg;						//ͷ�񱳾�
	CSkinImage						m_ImageUserPass;					//������־
	CSkinImage						m_ImageTime;						//ʱ���־
	CSkinImage						m_ImageTime2;						//ʱ���
	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btKouPai;
	CSkinButton						m_btXianPai;
	CSkinButton						m_btQiangCi;					
	CSkinButton						m_btGiveUpCiangCi;					//������ť
	CSkinButton						m_btAutoOutCard;					//��ʾ��ť
	CSkinButton						m_btForeTurn;						//������ʾ

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_ZhanCardControl[GAME_PLAYER];		//ռ���˿�
	CCardControl					m_BackCardControl;					//�����˿�
	CCardControl					m_UserCardControl[3];				//�˿���ͼ
	CCardControl					m_LeaveCardControl[2];				//�����˿�
	CCardControl					m_ForeTurn[GAME_PLAYER];			//��һ���˿�

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
	//�滭ʱ���־
	virtual void DrawTime(CDC * pDC,int nWidth,int nHeight);

	//���ú���
public:
	//�û���һ��Ҫ�����
	void ClearPlayerOneGame();
	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//���ܺ���
public:

	//��������
	void SetBaseScore(LONG lBaseScore);
	//��ʾ��ʾ
	void ShowKouPaiTitle(bool bCouPaiTitle,WORD wChairID);
	//��ʾ��ʾ
	void ShowXianPaiTitle(bool bXianPaiTitle,WORD wChairID);
	//��ʾ��ʾ
	void ShowQiangCiTitle(bool bQiangCiTitle,WORD wChairID);
	//������ʾ
	void HideAllTitle(bool bHide,WORD wChairID);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//����ռ����Ŀ
	void IncreaseZhanPaiCount(WORD wChairID,BYTE bCardCount);
	//������һ�ֳɼ�
	void SetForeScore(WORD wChairID,LONG lScore);
	//�����ܳɼ�
	void SetAllScore(WORD wChairID,LONG lScore);
	//����ʱ��
	void SetTimeFlag(LONG lTimeCount,DWORD dCurrentTime);
	//������������
	void SetIncreaseXian(WORD wChairID);
	//���ñ�������
	void SetIncreaseBeiXian(WORD wChairID);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//�зְ�ť
	afx_msg void OnKouPai();
	//�зְ�ť
	afx_msg void OnXianPai();
	//�зְ�ť
	afx_msg void OnQiangCi();
	//������ť
	afx_msg void OnGiveUpQingCi();
	//��һ����ʾ
	afx_msg void OnForTurn();
	//������ʾ
	afx_msg void OnAutoOutCard();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
