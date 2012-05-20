#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "../../../������/Include/SkinButton.h"


//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_WAIT_START			    (WM_USER+102)                       //������Ϣ
#define IDM_SHOW_CARD               (WM_USER+103)                       //������Ϣ
#define IDM_NOT_SHOW_CARD           (WM_USER+104)                       //������Ϣ
#define IDM_AUTO_OUT_CARD           (WM_USER+105)                       //�Զ�����
#define IDM_CANCEL_AUTO_OUT_CARD    (WM_USER+106)                       //ȡ������
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
public:
	BYTE							m_bScoreCard[GAME_PLAYER][16];      //��ֵ�˿�
	BYTE							m_bScoreCardCount[GAME_PLAYER];     //�˿���Ŀ
	BYTE							m_bShowCard[GAME_PLAYER][4];        //������� 
	BYTE							m_bShowCardCount[GAME_PLAYER];      //������Ŀ

	//�������
protected:
	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
	//״̬����
public:
	bool							m_bSendWaiMsg;                      //���ּ�ʱ
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	bool							m_bShowCardHint;                    //������ʾ

	//�ؼ�����
	//��ť�ؼ�
public:
	CSkinButton					    m_btStart;							//��ʼ��ť
	CSkinButton					    m_btOutCard;						//���ư�ť
	CSkinButton						m_btShowCard;                       //���ư�ť
	CSkinButton						m_btNotShowCard;                    //������ť
	CSkinButton						m_btAutoOutCard;                    //�Զ�����
	CSkinButton						m_btCancelAutoOutCard;              //ȡ������

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�˿���ͼ

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageBackGround;					//������Դ

	CSkinImage						m_ImageScoreLeft;					//�������
	CSkinImage						m_ImageScoreMid;					//�����м�
	CSkinImage						m_ImageScoreRight;					//�����ұ�
	CSkinImage						m_ImageScoreSeparator;				//���ַָ�
	
	CSkinImage						m_ImageScore[16];                   //����λͼ

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//���ú���
public:
	//�û�˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//���ܺ���
public:	
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//���ӷ���
	void AddScoreCard(WORD wChairID , BYTE bCardData[] , BYTE bCardCount) ;
	//��������
	void SetShowCard(WORD wChairID , BYTE bCardData[] , BYTE bCardCount) ;

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//���ư�ť
	afx_msg void OnShowCard() ;
	//������ť
	afx_msg void OnNotShowCard() ;
	//�Զ�����
	afx_msg void OnAutoOutCard() ;
	//ȡ������
	afx_msg void OnCancelAutoOutCard() ;
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
