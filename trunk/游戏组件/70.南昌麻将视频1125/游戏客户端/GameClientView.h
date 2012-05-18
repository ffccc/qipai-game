#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "ScoreControl.h"
#include "DrawSiceWnd.h"
#include "DrawSiceWnd2.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_BATCH					(WM_USER+102)						//�����Ϣ	
#define IDM_TRUSTEE_CONTROL			(WM_USER+103)						//�йܿ���
#define IDM_BATCH_END				(WM_USER+104)						//ϴ�����
#define IDI_DISC_EFFECT					103								//����Ч��

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��־����
protected:
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	bool							m_bHuangZhuang;						//��ׯ��־
	bool							m_bTrustee[GAME_PLAYER];			//�Ƿ��й�
	bool							m_bShowScore;						//����״̬

	//��Ϸ����
protected:
	LONG							m_lCellScore;						//��������
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbBatch[GAME_PLAYER];				//ϴ����

	//��ʷ����
public:
	LONG                            m_lAllTurnScore[GAME_PLAYER];		//�ֵܾ÷�
	LONG                            m_lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�

	//���·���Ϣ
	BYTE							m_cbUpDownKingCardData;				//���·�
	LONG							m_lUpDownScore[GAME_PLAYER];		//���·�
	BYTE							m_cbFloor;							//¥��		

	//��������
protected:
	bool							m_bBombEffect;						//����Ч��
	BYTE							m_cbBombFrameIndex;					//֡������

	//ϴ��Ч��
	bool							m_bBatchEffect;						//����Ч��
	BYTE							m_cbBatchFrameIndex;				//֡������

	//����Ч��
	WORD							m_wDiscUser;						//�����û�
	BYTE							m_cbDiscFrameIndex;					//֡������	

	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����˿�
	WORD							m_wOutCardUser;						//�����û�
	WORD							m_wUserAction[4];					//�û�����
	BYTE							m_cbKingCardIndex[2];				//��������

	//λ�ñ���
protected:
	CPoint							m_UserFlagPos[4];					//��־λ��
	CPoint							m_PointTrustee[GAME_PLAYER];		//�й�λ��

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageCenter;						//LOGOͼ
	CSkinImage						m_ImageLeft;						//����ͼƬ
	CSkinImage						m_ImageWait;						//�ȴ���ʾ
	CSkinImage						m_ImageOutCard;						//������ʾ
	CSkinImage						m_ImageUserFlag;					//�û���־
	CSkinImage						m_ImageUserAction;					//�û�����
	CSkinImage						m_ImageActionBack;					//��������
	CSkinImage						m_ImageHuangZhuang;					//��ׯ��־
	CPngImage						m_ImageTrustee;						//�йܱ�־	
	CPngImage						m_PngBatch[4];						//�Լ�ϴ��

	CPngImage						m_ImageActionAni;					//���ƶ�����Դ
	CPngImage						m_ImageDisc;						//����Ч��
	CPngImage						m_ImageArrow;						//��ʱ����ͷ	
	CSkinImage						m_ImageHistoryScore;				//����λͼ

	//ͼƬ��С
private:
	CSize							m_sizeHistory;						//���ִ�С

	//��Ƶ���
private:
	//CVideoServiceControl 			m_DlgVedioService[4];				//��Ƶ����

	//�˿˿ؼ�
public:
	CHeapCard						m_HeapCard[4];						//�����˿�
	CUserCard						m_UserCard[3];						//�û��˿�
	CTableCard						m_TableCard[4];						//�����˿�
	CWeaveCard						m_WeaveCard[4][4];					//����˿�
	CDiscardCard					m_DiscardCard[4];					//�����˿�
	CCardControl					m_HandCardControl;					//�����˿�

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CControlWnd						m_ControlWnd;						//���ƴ���
	CScoreControl					m_GameScoreWnd;						//��������
	CDrawSiceWnd                    m_DrawSiceWnd;                      //ɸ�Ӵ���
	CDrawSiceWnd2					m_DrawSiceWnd2;						//ɸ�Ӵ���
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���
	CSkinButton						m_btScore;							//���ְ�ť
	CSkinButton						m_btScoreClose;						//�رճɼ�
	CSkinButton						m_btBatch;							//ϴ�谴ť


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
	//��ׯ����
	void SetHuangZhuang(bool bHuangZhuang);
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, WORD wUserAction);
	//��������
	void SetKingCardIndex(BYTE cbKingCardIndex[2]);
	//���ö���
	bool SetBombEffect(bool bBombEffect);
	//����ϴ��
	bool SetBatchEffect(bool bBatchEffect);
	//�����û�
	void SetDiscUser(WORD wDiscUser);
	//��ʱ���
	void SetCurrentUser(WORD wCurrentUser);
	//�����й�
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);
	//ϴ����
	void SetBatchResult(BYTE cbBatchResult[4]);

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//ϴ�谴ť
	afx_msg void OnBatch();
	 //�Ϲܿ���
	 afx_msg void OnStusteeControl();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//���ְ�ť
	afx_msg void OnBnClickedScore();
	//���ְ�ť
	afx_msg void OnBnClickedCloseScore();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
