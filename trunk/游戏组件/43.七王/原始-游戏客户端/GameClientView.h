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
#define IDM_BAIPAI                  (WM_USER+104)                       //������Ϣ
#define IDM_TOUXIANG                (WM_USER+105)                        //Ͷ����Ϣ
#define IDM_ABNEGATE                (WM_USER+106)                       //������Ϣ
#define IDM_JIU                     (WM_USER+107)						//������Ϣ
#define IDM_DISPLAYTEN				(WM_USER+108)						//��ʮ

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
	bool							m_bMost[GAME_PLAYER];				//�������
	bool							m_wBanker[GAME_PLAYER];				//ׯ������
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	CPoint							m_PointUserPass[GAME_PLAYER];		//����λ��

	//��ʷ����
public:
	LONG                            m_lAllTurnScore[GAME_PLAYER];		//�ֵܾ÷�
	LONG                            m_lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageBackGround;
	CSkinImage						m_ImageLc;
	CSkinImage						m_ImageRc;

	CSkinImage						m_ImageHeadBg;
	CSkinImage						m_ImageUserPass;					//������־
	CSkinImage                      m_RedT_Ten;                           //����ʮ
	CSkinImage                      m_Fang_Ten;                          //��ʮ

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	//CSkinButton					m_btOutPrompt;						//��ʾ��ť
	CSkinButton                     m_btTouXiang ;                       //Ͷ����ť
    CSkinButton                     m_btBaiPai  ;                        //���ư�ť
	CSkinButton                     m_btAbnegate ;                      //������ť
	CSkinButton						m_btJiu;							//���˰�ť
	CSkinButton						m_btDisplayTen;						//��ʮ

	//�˿˿ؼ�
public:
	CCardControl					m_HandCardControl;					//�����˿�
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�˿���ͼ
	CCardControl					m_LeaveCardControl[GAME_PLAYER-1];	//�����˿�
	CCardControl					m_ScoreCardControl[GAME_PLAYER];	//�����˿�

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
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//��������־
	void SetMostFlag(WORD wChairID,bool bMost);
	//����ׯ�ұ�־
	void SetBankerFlag(WORD wChairID,bool wBanker);
	//һ��һ����
	void ClearOneTime();

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
	//���ư�ť
	afx_msg void OnBaiPai();
	//Ͷ����ť
	afx_msg void OnTouXiang();
	//������ť
	afx_msg void OnAbnegate();
	//���˰�ť
	afx_msg void OnJiu();
	afx_msg void OnDisplayTen();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);


	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
