#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "GoldControl.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_START						WM_USER+100						//��ʼ��Ϣ
#define IDM_ADD_GOLD					WM_USER+101						//��ע��Ϣ
#define IDM_FOLLOW						WM_USER+102						//��ע��Ϣ
#define IDM_GIVE_UP						WM_USER+103						//������Ϣ
#define IDM_SHOWHAND					WM_USER+104						//�����Ϣ

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

	//�������
protected:
	CSkinImage						m_ImageTitle;						//����λͼ
	CSkinImage						m_ImageBarMid;						//����λͼ
	CSkinImage						m_ImageBarLeft;						//����λͼ
	CSkinImage						m_ImageBarRight;					//����λͼ
	CSkinImage						m_ImageGlodTable;					//���λͼ
	CSkinImage						m_ImageBackGround;					//����λͼ
	CSkinImage						m_ImageTotal;						//�ܳ���
	CSkinImage						m_ImageHeadBg;						//ͷ�񱳾�
	CPoint							m_GoldStation[8];					//����λ��

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btFollow;							//��ע��ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btAddGold;						//��ע��ť
	CSkinButton						m_btShowHand;						//�����ť
	CGoldView						m_GoldView[8];						//�û�����
	CScoreView						m_ScoreView;						//������ͼ
	CGoldControl					m_GoldControl;						//��ע��ͼ
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

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
