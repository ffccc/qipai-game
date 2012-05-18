#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "ControlWnd.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_START						WM_USER+100						//��ʼ��Ϣ
#define IDM_ADD_GOLD					WM_USER+101						//��ע��Ϣ
#define IDM_FOLLOW						WM_USER+102						//��ע��Ϣ
#define IDM_GIVE_UP						WM_USER+103						//������Ϣ
#define IDM_SHOWHAND					WM_USER+104						//�����Ϣ
#define IDM_SEND_CARD_FINISH			WM_USER+106						//�������

//////////////////////////////////////////////////////////////////////////

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CArrayTemplate<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;

	//��ͼ����
protected:
	LONG							m_lUserGold[GAME_PLAYER];			//
	LONG							m_lMaxGold;							//�����ע
	LONG							m_lBasicGold;						//���ٷ���
	CPoint							m_GoldStation[8];					//����λ��
	bool							m_bUserShowHand;					//

	//��������
protected:
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_SendCardCurPos;					//���Ƶ�ǰλ��
	INT								m_nStepCount;						//����
	INT								m_nXStep;							//����
	INT								m_nYStep;							//����
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//�������
protected:
	CSkinImage						m_ImageTitle;						//����λͼ
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewCenter;					//����λͼ
	CSkinImage						m_ImageCardBack;					//�˿���Դ
	CPngImage						m_PngShowHand;						//

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btFollow;							//��ע��ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btAddGold;						//��ע��ť
	CSkinButton						m_btShowHand;						//�����ť
	CGoldView						m_GoldView[8];						//�û�����
	CScoreView						m_ScoreView;						//������ͼ
	CControlWnd						m_GoldControl;						//��ע��ͼ
	CCardControl					m_CardControl[4];					//�˿˿ؼ�

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
	//��ұ���
	void SetGoldTitleInfo(DWORD lMaxGold, DWORD lBasicGold);
	//�û�����
	void SetUserGoldInfo(WORD wViewChairID, bool bTableGold, DWORD dwGold);
	//�����ж�
	bool IsDispatchCard();
	//��ɷ���
	void FinishDispatchCard();
	//�����˿�
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//
	void SetUserShowHand( bool bShowHand );
	//
	void SetUserGold( WORD wChairId, LONG lGold );

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//��ע��ť
	afx_msg void OnFollow();
	//������ť
	afx_msg void OnGiveUp();
	//��ע��ť
	afx_msg void OnAddGold();
	//�����ť
	afx_msg void OnShowHand();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//
	VOID OnLButtonUp(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
