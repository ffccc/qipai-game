#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "GameLogic.h"
#include "AdminControlDlg.h"
#include "DlgViewChart.h"

#ifdef __SPECIAL___
#include "DlgBank.h"
#endif
//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};

//��������
typedef CArrayTemplate<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������

	//��ע��Ϣ
protected:
	LONGLONG						m_lMeTieScore;						//��ƽ��ע
	LONGLONG						m_lMeBankerScore;					//��ׯ��ע
	LONGLONG						m_lMePlayerScore;					//������ע
	LONGLONG						m_lMeTieSamePointScore;				//ͬ��ƽע
	LONGLONG						m_lMePlayerKingScore;				//������ע
	LONGLONG						m_lMeBankerKingScore;				//ׯ����ע
	LONGLONG						m_lMePlayerTwoPair;					//������ע
	LONGLONG						m_lMeBankerTwoPair;					//������ע

	//ȫ����ע
protected:
	LONGLONG						m_lAllTieScore;						//��ƽ��ע
	LONGLONG						m_lAllBankerScore;					//��ׯ��ע
	LONGLONG						m_lAllPlayerScore;					//������ע
	LONGLONG						m_lAllTieSamePointScore;			//ƽ����ע
	LONGLONG						m_lAllBankerKingScore;				//ׯ����ע
	LONGLONG						m_lAllPlayerKingScore;				//������ע
	LONGLONG						m_lAllPlayerTwoPair;				//�мҶ���
	LONGLONG						m_lAllBankerTwoPair;				//ׯ�Ҷ���

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nWinPointsExcursionX;				//ƫ��λ��
	int								m_nWinPointsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcPlayer;							//�м�����
	CRect							m_rcPlayerKing;						//����������
	CRect							m_rcPlayerTwoPair;					//��������
	CRect							m_rcTie;							//ƽ����
	CRect							m_rcTieKing;						//ͬ��ƽ����
	CRect							m_rcBanker;							//ׯ������
	CRect							m_rcBankerKing;						//ׯ��������
	CRect							m_rcBankerTwoPair;					//��������
	CRect							m_rcTrigger;						//��������
	//�˿���Ϣ
protected:	
	BYTE							m_cbCardCount[2];					//�˿���Ŀ
    BYTE							m_cbTableCardArray[2][3];			//�����˿�

	//��������
protected:
	POINT							m_ptDispatchCard;					//����λ��
	BYTE							m_cbSendCount[2];					//�˿���Ŀ
	CCardControl					m_DispatchCard;						//�����˿�
	bool							m_bBombEffect[9];					//��ըЧ��
	BYTE							m_cbBombFrameIndex[9];				//֡������

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	CString							m_strDispatchCardTips;				//������ʾ
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	LONGLONG						m_lBankerWinScore;					//ׯ�ҳɼ�	
	LONGLONG						m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//���ֳɼ�
public:
	LONGLONG						m_lMeCurGameScore;					//�ҵĳɼ�
	LONGLONG						m_lMeCurGameReturnScore;			//�ҵĳɼ�
	LONGLONG						m_lBankerCurGameScore;				//ׯ�ҳɼ�
	LONGLONG						m_lGameRevenue;						//��Ϸ˰��

	//���ݱ���
protected:
	CPoint							m_PointJetton[8];					//����λ��
	CPoint							m_PointJettonNumber[8];				//����λ��
	CJettonInfoArray				m_JettonInfoArray[8];				//��������

	//�ؼ�����
public:
	CSkinButton						m_btJetton100;						//���밴ť
	CSkinButton						m_btJetton1000;						//���밴ť
	CSkinButton						m_btJetton10000;					//���밴ť
	CSkinButton						m_btJetton100000;					//���밴ť
	CSkinButton						m_btJetton1000000;					//���밴ť
	CSkinButton						m_btJetton5000000;					//���밴ť
	CSkinButton						m_btViewChart;						//�鿴·��
	
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�

	CSkinButton						m_btUp;								//��ť
	CSkinButton						m_btDown;							//��ť

	CButton							m_btOpenAdmin;
	//�ؼ�����
public:
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�
	CCardControl					m_CardControl[2];					//�˿˿ؼ�	
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CDlgViewChart					m_DlgViewChart;						//�鿴·��

	//
#ifdef __SPECIAL___
	CDlgBank						m_DlgBank;							//���пؼ�	
	CSkinButton						m_btBankerStorage;					//��ť
	CSkinButton						m_btBankerDraw;						//ȡ�ť
#endif

	//�������
protected:
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewBack;					//����λͼ
	CSkinImage						m_ImageWinFlags;					//��־λͼ
	CSkinImage						m_ImageJettonView;					//������ͼ
	CSkinImage						m_ImageScoreNumber;					//������ͼ
	CSkinImage						m_ImageMeScoreNumber;				//������ͼ
	CSkinImage						m_ImageTimeFlag;					//ʱ���ʶ
	CPngImage						m_ImageBombEffect;					//����ͼƬ

	//�߿���Դ
protected:
	CSkinImage						m_ImageFrameXianJia;				//�߿�ͼƬ
	CSkinImage						m_ImageFrameZhuangJia;				//�߿�ͼƬ
	CSkinImage						m_ImageFrameXianTianWang;			//�߿�ͼƬ
	CSkinImage						m_ImageFrameZhuangTianWang;			//�߿�ͼƬ
	CSkinImage						m_ImageFramePingJia;				//�߿�ͼƬ
	CSkinImage						m_ImageFrameTongDianPing;			//�߿�ͼƬ
	CSkinImage						m_ImageFramePlayerTwoPair;			//�߿�ͼƬ
	CSkinImage						m_ImageFrameBankerTwoPair;			//�߿�ͼƬ

	CSkinImage						m_ImageMeBanker;					//�л�ׯ��
	CSkinImage						m_ImageChangeBanker;				//�л�ׯ��
	CSkinImage						m_ImageNoBanker;					//�л�ׯ��

	//������Դ
protected:
	CSkinImage						m_ImageGameEnd;						//�ɼ�ͼƬ
	CSkinImage						m_ImageGameEndFrame;				//�߿�ͼƬ
	CSkinImage						m_ImageGamePoint;					//����ͼƬ
	bool                            m_bFlashResult;
public:
    CAdminControlDlg				m_AdminDlg;							//����Ա����
	//����
public:
	bool							m_blCanStore;                       //�Ƿ��ܱ���

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
	//������Ϣ
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//�ҵ�λ��
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID; }
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//ȡ������
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }

	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard(bool bScene = false);
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//�滭��ը
	void DrawBombEffect(CDC *pDC);

	//��������
public:
	//�����ע
	LONGLONG GetMaxPlayerScore();
	//�����ע
	LONGLONG GetMaxPlayerKingScore();
	//�����ע
	LONGLONG GetMaxBankerScore();
	//�����ע
	LONGLONG GetMaxBankerKingScore();
	//�����ע
	LONGLONG GetMaxTieScore();
	//�����ע
	LONGLONG GetMaxTieKingScore();
	//�����ע
	LONGLONG GetMaxPlayerTwoPair();
	//�����ע
	LONGLONG GetMaxBankerTwoPair();

	//��������
public:
	//��ȡ������ע
	LONGLONG GetTotalChip();


	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);


	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(BYTE cbWinnerSide,bool blSet = false);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//������ʾ
	void SetDispatchCardTips();

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat=-1);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair);

	//��ť��Ϣ
protected:
	//���밴ť
	afx_msg void OnJettonButton100();
	//���밴ť
	afx_msg void OnJettonButton1000();
	//���밴ť
	afx_msg void OnJettonButton10000();
	//���밴ť
	afx_msg void OnJettonButton100000();
	//���밴ť
	afx_msg void OnJettonButton1000000();
	//���밴ť
	afx_msg void OnJettonButton5000000();
	//�鿴·��
	afx_msg void OnViewChart();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	//���д��
	afx_msg void OnBankStorage();
	//����ȡ��
	afx_msg void OnBankDraw();
	//����Ա����
	afx_msg void OpenAdminWnd();
	//�Ϸ�ҳ
	afx_msg void OnUp();
	//�·�ҳ
	afx_msg void OnDown();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
