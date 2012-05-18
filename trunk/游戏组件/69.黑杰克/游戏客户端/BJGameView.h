#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "GoldControl.h"
#include "..\��Ϣ����\CMD_BlackJack.h"

//#define DEBUG_GAME
//��Ϸ��ͼ
class CBJGameView : public CGameFrameView
{
	//��Ԫ����
	friend class CBJGameDlg;

	//��ͼ����
protected:
	LONG							m_lMaxGold;							//�����ע
	LONG							m_lBasicGold;						//���ٷ���

	//��������
protected:
	CSize							m_ImageSize;						//������С
	CSkinImage						m_ImageBack;						//��Ϸ����
	CSkinImage						m_ImageTitle;						//���ⱳ��
	CSkinImage						m_ImageBanker;	    				//ׯ�ұ�־
	CSkinImage						m_ImageArrowHead;    				//���Ʊ�ʶ
	CSkinImage						m_ImageCenter;
	CRect							m_Rct_Play;							//��Ϸ����

	//��ʶ״̬
protected:
	CSkinImage						m_ImageAddGoldMark;    				//��ע״̬
	CSkinImage						m_ImageGetCardMark;    				//Ҫ��״̬
	CSkinImage						m_ImageStopMark;    				//ͣ��״̬
	CSkinImage						m_ImageBrustMark;    				//����״̬

    //λ�ñ���
protected:
	POINT							m_ptMark[GAME_PLAYER];              //��Ϸׯ��
	POINT							m_ptCardSum[GAME_PLAYER];		    //�˿���ֵ
	CPoint							m_GoldStation[GAME_PLAYER*2];		//����λ��
	WORD 						    m_wBankerStation;					//ׯ��λ��

	//��Ϸ����
protected:
	BYTE 						    m_cbCutCardMark;					//���Ʊ�ʶ
	BYTE                            m_cbTopCardSum[GAME_PLAYER];        //�˿���ֵ
	BYTE                            m_cbBottomCardSum[GAME_PLAYER];     //�˿���ֵ
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬

	//�ؼ�����
public:
	CGoldView						m_GoldView[GAME_PLAYER*2];			//�û�����
	CCardControl					m_CardControl[GAME_PLAYER*2];		//�˿˿ؼ�
	CScoreView						m_ScoreView;						//������ͼ
	CGoldControl					m_GoldControl;						//��ע��ͼ

	//�ؼ�����
protected:
	CSkinButton						m_btAddGold;						//��ע��ť
	CSkinButton						m_btBegin;							//��ʼ��ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btGetCard;						//Ҫ�ư�ť
	CSkinButton						m_btStop;							//ͣ�ư�ť
	CSkinButton						m_btDouble;							//˫����ť
	CSkinButton						m_btCutCard;						//���ư�ť

	//��������
public:
	//���캯��
	CBJGameView();
	//��������
	virtual ~CBJGameView();

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
	//��ұ���
	void SetGoldTitleInfo(DWORD lMaxGold, DWORD lBasicGold);
	//�û�����
	void SetUserGoldInfo(WORD wViewChairID, bool bTableGold, DWORD dwGold);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//������ť
	afx_msg void OnGiveUp();
	//��ע��ť
	afx_msg void OnAddGold();
	//Ҫ�ư�ť
	afx_msg void OnGetCard();
	//ͣ�ư�ť
	afx_msg void OnStopCard();
	//˫����ť
	afx_msg void OnDoubleCard();
	//˫����ť
	afx_msg void OnCutCard();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
