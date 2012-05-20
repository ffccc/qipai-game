#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_LAND_SCORE				(WM_USER+103)						//�з���Ϣ
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ
#define IDM_LAND_AUTOPLAY			(WM_USER+105)                       //�й���Ϣ
#define IDM_SORT_HAND_CARD			(WM_USER+106)						//�����˿�

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�����û�
	WORD							m_wBombTime;						//ը������
	LONG							m_lBaseScore;						//��������
	BYTE							m_cbLandScore;						//��������

	//״̬����
public:
	bool							m_bLandTitle;						//������־
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bScore[GAME_PLAYER];				//�û��з�
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	bool							m_bDeasilOrder;						//˳ʱ���
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�
	bool							m_bShowScore;						//����״̬
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//������Ϣ

	//��ը����
protected:
	bool							m_bBombEffect;						//��ըЧ��
	BYTE							m_cbBombFrameIndex;					//֡������

	//λ����Ϣ
public:
	CSize							m_LandSize;							//�ڿӱ�־
	CPoint							m_ptScore[GAME_PLAYER];				//�з�λ��
	CPoint							m_ptLand[GAME_PLAYER];				//����λ��

	//λͼ����
protected:
	CSkinImage						m_ImageBomb;						//ը��ͼƬ
	CSkinImage						m_ImageBack;						//������Դ
	CSkinImage						m_ImageCenter;						//������Դ
	CSkinImage						m_ImageBombEffect;					//ը��Ч��
	CSkinImage						m_ImageHistoryScore;				//��ʷ����
	CPngImage						m_ImageScore;						//����ͼƬ
	CPngImage						m_ImageLand;						//ׯ��ͼƬ
	CPngImage						m_ImageUserTrustee;					//����й�
    CPngImage						m_Image002;		            		//����  peter
	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btOneScore;						//1 �ְ�ť
	CSkinButton						m_btTwoScore;						//2 �ְ�ť
	CSkinButton						m_btThreeScore;						//3 �ְ�ť
	CSkinButton						m_btGiveUpScore;					//������ť
	CSkinButton						m_btAutoOutCard;					//��ʾ��ť
	CSkinButton						m_btSortCard;						//����ť
	//CSkinButton						m_Image002;						// �����Զ��й�
	CSkinButton						m_btAutoPlayOff;					// �ر��Զ��й�
	CSkinButton						m_btScore;							//���ְ�ť
//	CSkinButton	                    m_Image002;
	//�˿˿ؼ�
public:
	CCardControl					m_BackCardControl;					//�����˿�
	CCardControl					m_UserCardControl[3];				//�˿���ͼ
	CCardControl					m_HandCardControl[3];				//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	CSize							m_sizeHistory;						//���ִ�С

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
	//ը������
	void SetBombTime(WORD wBombTime);
	//��������
	void SetBaseScore(LONG lBaseScore);
	//��ʾ��ʾ
	void ShowLandTitle(bool bLandTitle);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//�ڿӷ���
	void SetLandUser(WORD wChairID, BYTE bLandScore);
	//�ڿӷ���
	void SetLandScore(WORD wChairID, BYTE bLandScore);
	//����˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//����й�
	void SetUserTrustee( WORD wChairID, bool bTrustee );
	//���û���
	bool SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//�зְ�ť
	afx_msg void OnOneScore();
	//�зְ�ť
	afx_msg void OnTwoScore();
	//������ť
	afx_msg void OnGiveUpScore();
	//������ʾ
	afx_msg void OnAutoOutCard();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//�зְ�ť
	afx_msg void OnThreeScore();
	// �Զ��й�
    afx_msg void IDRPNG();
    // ȡ���й�
    afx_msg void OnAutoPlayerOff();
	//����ť
	VOID OnBnClickedSortCard();
	//���ְ�ť
	VOID OnBnClickedScore();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
