#include "../../../������/Include/SkinButton.h"

#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
//#include "CallCardWnd.h"
#include "CardControl.h"
#include "ConcealCardView.h"
#include "CallScore.h"
#include "CallScoreInfo.h"


//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��ť��Ϣ
#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_OUT_PROMPT				(WM_USER+102)						//��ʾ��Ϣ
#define IDM_SEND_CONCEAL			(WM_USER+103)						//������Ϣ

//���ܰ�ť
#define IDM_CONCEAL_CARD			(WM_USER+110)						//�鿴����
#define IDM_REQUEST_LEAVE			(WM_USER+111)						//�����뿪
#define IDM_LAST_TURN_CARD			(WM_USER+112)						//�����˿�
#define IDM_TRUSTEE_CONTROL			(WM_USER+113)						//�йܿ���

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��ʾ����
protected:
	bool							m_bDeasilOrder;						//����˳��
	bool							m_bLastTurnCard;					//�����˿�

	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	int								m_cbJuKuang;						//�ֿ�
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	WORD							m_wCallCardUser;					//�����û�
	BYTE							m_cbValueOrder[2];					//�÷�
	BYTE							m_cbMainValue;						//������ֵ

	//״̬����
public:
	bool							m_bLandTitle;						//������־
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bScore[GAME_PLAYER];				//�û��з�
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	WORD							m_wLandUser;						//�����û�
	BYTE							m_cbLandScore;						//��������
	WORD							m_bMeChair;							//�Լ�λ��

	//λ����Ϣ
protected:
	CSize							m_SizeBanker;						//ׯ�Ҵ�С
	CSize							m_SizeStation;						//״̬��С
	CPoint							m_PointBanker[GAME_PLAYER];			//ׯ��λ��

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageBackGround;					//������Դ
	CSkinImage						m_ImageLastTurn;					//���ֱ�־
	CSkinImage						m_ImageColorFlag;					//��ɫ��־
	//CSkinImage						m_ImageStationBack;					//״̬��Դ
	CSkinImage						m_ImageStationTitle;				//״̬��Դ
	CSkinImage						m_ImageUserFlag;					//�û���־

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btOutPrompt;						//��ʾ��ť
	
	//���ܰ�ť
public:
	CSkinButton						m_btLastTurnCard;					//�����˿�
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���

	//�˿˿ؼ�
public:
	CCardControl					m_CardScore;						//�÷��˿�
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_UserCardControl[4];				//�˿���ͼ
	CCardControl					m_HandOutCardControl[4][4];			//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	CCallScoreInfo					m_CallScoreInfo;					//�з�״̬����
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CCallScore						m_CallScore;						//�зִ���

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

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

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
	//�Լ�λ��
	void SetMeChair(WORD wMeChair);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }

	//��Ϸ��Ϣ
public:
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//���þֿ�
	void SetJuKuang(int nJuKuang);
	//���ö�Լ��Ϣ
	void SetDingYue(BYTE cbMainValue,BYTE cbMainColor);
	//��������
	void SetLastTurnCard(bool bLastTurnCard);
	//���õ÷�
	void SetValueOrder(BYTE cbValueBanker, BYTE cbValueOther);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//��ʾ��ť
	afx_msg void OnOutPrompt();
	//���װ�ť
	afx_msg void OnSendConceal();
	//�鿴����
	afx_msg void OnConcealCard();
	//�����뿪
	afx_msg void OnRuquestLeave();
	//�����˿�
	afx_msg void OnLastTurnCard();
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	
	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
