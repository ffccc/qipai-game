#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_OUT_PROMPT				(WM_USER+103)						//��ʾ����
#define IDM_SORT_BY_COLOR			(WM_USER+104)						//��ɫ����
#define IDM_SORT_BY_VALUE			(WM_USER+105)						//��С����
#define IDM_TRUSTEE_CONTROL			(WM_USER+106)						//�йܿ���
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��

	//״̬����
public:
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	CPoint							m_PointUserPass[GAME_PLAYER];		//����λ��
	CPoint							m_PointTrustee[GAME_PLAYER];		//�й�λ��
	WORD							m_wWinOrder[GAME_PLAYER];			//ʤ���б�
	bool							m_bTrustee[GAME_PLAYER];			//�Ƿ��й�

	//λͼ����
protected:
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewBack;					//����λͼ
	CSkinImage						m_ImageUserPass;					//������־
	CSkinImage						ImageWinOrder;						//����λͼ
	CPngImage						m_ImageTrustee;						//�йܱ�־	


	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	//CSkinButton						m_btOutPrompt;						//��ʾ��ť
	CSkinButton						m_btSortByColor;					//��ɫ����
	CSkinButton						m_btSortByValue;					//��С����
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���

	//CSkinButton						m_btShowVideo;						//��ʾ��Ƶ

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl[4];				//�����˿�
	CCardControl					m_UserCardControl[4];				//�˿���ͼ

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ

#ifdef VIDEO_GAME
	//��Ƶ���
private:
	CVideoServiceControl 			m_DlgVedioService[4];				//��Ƶ����
#endif

	//����λ��
private:
	CPoint							m_PointWinOrder[GAME_PLAYER];		//��Ӯ����

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

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
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//��������
	bool SetUserWinOrder(WORD wChairID, WORD wWinOrder);
	//�����й�
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);


	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//��ʾ��ť
	afx_msg void OnOutPrompt();
	//��ɫ����
	afx_msg void OnSortByColor();
	//��С����
	afx_msg void OnSortByValue();
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
	//��ʾ��Ƶ
	//afx_msg void OnShowVideo();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
