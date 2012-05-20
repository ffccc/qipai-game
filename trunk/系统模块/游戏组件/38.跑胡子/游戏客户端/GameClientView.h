#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "ChooseWnd.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ա����
	friend class CGameClientDlg;

	//��Ϸ����
protected:
	LONG							m_lCellScore;						//��������
	WORD							m_wBankerUser;						//ׯ���û�
	BYTE							m_bLeftCardCount;					//ʣ���˿�

	//���״̬
protected:
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	bool							m_bHuangZhuang;						//��ׯ��־

	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����˿�
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_bUserAction[3];					//�û�����
	BYTE							m_bUserHuXiCount[3];				//��Ϣ��Ŀ

	//��������
protected:
	bool							m_bMoveCard;						//�ƶ���־
	bool							m_bSwitchCard;						//������־
	WORD							m_wMouseDownItem;					//�������

	//λ����Ϣ
protected:
	CSize							m_BankerSize;						//ׯ�Ҵ�С
	CPoint							m_BankerPoint[GAME_PLAYER];			//ׯ��λ��

	//�˿˿ؼ�
public:
	CUserCard						m_UserCard[2];						//�û��˿�
	CWeaveCard						m_WeaveCard[3][7];					//����˿�
	CDiscardCard					m_DiscardCard[3];					//�����˿�
	CCardControl					m_HandCardControl;					//�����˿�

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CScoreView						m_ScoreView;						//������ͼ
	CChooseWnd						m_ChooseWnd;						//���ƴ���
	CControlWnd						m_ControlWnd;						//���ƴ���

	//�������
protected:
	CSkinImage						m_ImageWait;						//�ȴ���ʾ
	CSkinImage						m_ImageTitle;						//����λͼ
	CSkinImage						m_ImageBanker;						//ׯ��λͼ
	CSkinImage						m_ImageOutCard;						//������ʾ
	CSkinImage						m_ImageViewBack;					//����λͼ
	CSkinImage						m_ImageCardFrame;					//����λͼ
	CSkinImage						m_ImageUserAction;					//�û�����
	CSkinImage						m_ImageActionBack;					//��������
	CSkinImage						m_ImageHuangZhuang;					//��ׯ��־

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
	//��������
	void SetCellScore(LONG lCellScore);
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//���û�ׯ
	void SetHuangZhuang(bool bHuangZhuang);
	//ʣ���˿�
	void SetLeftCardCount(BYTE bLeftCardCount);
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//��Ϣ��Ŀ
	void SetUserHuXiCount(WORD wChairID, BYTE bHuXiCount);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
