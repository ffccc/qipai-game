#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;
	//��Ϸ����
protected:
	LONG							m_lBaseScore;						//��������
	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��


	//״̬����
public:
	bool							m_bEightOne[3];						//�Ƿ��һ
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ

	//λ����Ϣ
public:
//	CSize							m_LandSize;							//�ڿӱ�־
//	CPoint							m_ptScore[GAME_PLAYER];				//�з�λ��
//	CPoint							m_ptLand[GAME_PLAYER];				//����λ��
	CPoint							m_PointUserPass[GAME_PLAYER];		//Passλ��

	//λͼ����
protected:
	CSkinImage						m_ImageBomb;						//ը��ͼƬ
	CSkinImage						m_ImageLand;						//ׯ��ͼƬ
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageCenter;						//������Դ
	CSkinImage						m_ImageUserPass;					//����ͼƬ

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btAutoOutCard;					//��ʾ��ť

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_UserCardControl[3];				//�˿���ͼ
	CCardControl					m_LeaveCardControl[2];				//�����˿�

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

	//���ú���
public:
	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//���ܺ���
public:
	//��������
	void SetBaseScore(LONG lBaseScore);
	//��ʾ��ʾ
	//void ShowLandTitle(bool bLandTitle);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
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
