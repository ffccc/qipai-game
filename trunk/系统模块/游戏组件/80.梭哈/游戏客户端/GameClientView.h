#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_START						WM_USER+100						//��ʼ��Ϣ
#define IDM_ADD_GOLD					WM_USER+101						//��ע��Ϣ
#define IDM_FOLLOW						WM_USER+102						//��ע��Ϣ
#define IDM_GIVE_UP						WM_USER+103						//������Ϣ
#define IDM_SHOWHAND					WM_USER+104						//�����Ϣ
#define IDM_SEND_CARD_FINISH			WM_USER+106						//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

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
	LONG							m_lMaxGold;							//�����ע
	LONG							m_lBasicGold;						//���ٷ���
	CPoint							m_GoldStation[8];					//����λ��
	bool							m_bShowScoreControl;				//

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
	CSkinImage						m_ImageCardBack;					//�˿���Դ
	CSkinImage						m_ImageTitle;						//����λͼ
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewCenter;					//����λͼ
	CSkinImage						m_ImageScoreControlBack;			//
	CSkinImage						m_ImageScoreTable;					//
	CPngImage						m_PngBkTopLeft;						//
	CPngImage						m_PngBkTopRight;					//
	CPngImage						m_PngBkBottomLeft;					//
	CPngImage						m_PngBkBottomRight;					//

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btFollow;							//��ע��ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btShowHand;						//�����ť
	CSkinButton						m_btAddTimes3;						//
	CSkinButton						m_btAddTimes2;						//
	CSkinButton						m_btAddTimes1;						//
	CGoldView						m_GoldView[8];						//�û�����
	CGoldView						m_TotalGoldView;					//
	CScoreView						m_ScoreView;						//������ͼ
	CCardControl					m_CardControl[4];					//�˿˿ؼ�

	//�ؼ�λ��
protected:
	CPoint							m_ptScoreControl;					//

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
	VOID ShowScoreControl( bool bShow );

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//��ע��ť
	afx_msg void OnFollow();
	//������ť
	afx_msg void OnGiveUp();
	//��ע��ť
	afx_msg void OnAddTimes1();
	//��ע��ť
	afx_msg void OnAddTimes2();
	//��ע��ť
	afx_msg void OnAddTimes3();
	//�����ť
	afx_msg void OnShowHand();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
