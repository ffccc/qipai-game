#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "GameLogic.h"

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Pleace,
	enOperateResult_Win,
	enOperateResult_Lost
	
};

//��¼��Ϣ
struct tagClientGameRecord
{
//	enOperateResult					enOperateFlags;						//������ʶ
	//BYTE							cbPlayerCount;						//�мҵ���
	//BYTE							cbBankerCount;						//ׯ�ҵ���
	//BYTE							cbKingWinner;						//����Ӯ��
	//bool							bPlayerTwoPair;						//���ӱ�ʶ
	//bool							bBankerTwoPair;						//���ӱ�ʶ
	BYTE							cbWinner;						
	BYTE							cbMePlaceRegion;
};

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��
//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				9									//������Ŀ
//#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ

//��������
#define INDEX_BANKER				0									//ׯ������
#define INDEX_PLAYER1				1									//�м�����
#define INDEX_PLAYER2				2									//�м�����
#define INDEX_PLAYER3				3									//�м�����
#define INDEX_PLAYER4				4									//�м�����


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
public:
	bool							m_bCanGo;
	bool							m_bSet;
	bool							m_bShowResult;

	bool							m_bAutoModel;
	BYTE							m_cbWaitPre;
protected:
	CSkinImage						m_ImageNiu;
	//CSkinButton						m_btnShouDong;
	//CSkinButton						m_btnZiDong;
	BYTE							m_cbNiuType[5];
	CPoint							m_ptDrawNiu[5];
	
	//������Ϣ
protected:
	LONG							m_lMeMaxScore;						//�����ע
    LONG							m_lAreaLimitScore;					//��������


	//��ע��Ϣ
protected:



	LONG							m_lMeTianScore;						//��
	LONG							m_lMeDiScore;						//��
	LONG							m_lMeXuanScore;						//��
	LONG							m_lMeHuangScore;					//��

	//ȫ����ע
protected:


	LONG							m_lAllTianScore;					//��
	LONG							m_lAllDiScore;						//��
	LONG							m_lAllXuanScore;					//��
	LONG							m_lAllHuangScore;					//��

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nWinPointsExcursionX;				//ƫ��λ��
	int								m_nWinPointsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcTian;							//�м�����
	CRect							m_rcDi;						//����������
	CRect							m_rcXuan;					//��������
	CRect							m_rcHuang;							//ƽ����
	//CRect							m_rcTieKing;						//ͬ��ƽ����
	//CRect							m_rcBanker;							//ׯ������
	//CRect							m_rcBankerKing;						//ׯ��������
	//CRect							m_rcBankerTwoPair;					//��������

	//�˿���Ϣ
protected:	
    BYTE							m_cbTableCardArray[6][3];			//�����˿�
	//��������
protected:
	POINT							m_ptDispatchCard;					//����λ��
	BYTE							m_cbSendCount[5];					//�˿���Ŀ
	CCardControl					m_DispatchCard;						//�����˿�
	bool							m_bBombEffect[9];					//��ըЧ��
	BYTE							m_cbBombFrameIndex[9];				//֡������

	//��ʷ��Ϣ
protected:
	LONG							m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONG							m_lCurrentJetton;					//��ǰ����
	CString							m_strDispatchCardTips;				//������ʾ
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	LONG							m_lBankerScore;						//ׯ�һ���
	LONG							m_lBankerWinScore;					//ׯ�ҳɼ�	
	LONG							m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//���ֳɼ�
public:
	LONG							m_lMeCurGameScore;					//�ҵĳɼ�
	LONG							m_lMeCurGameReturnScore;			//�ҵĳɼ�
	LONG							m_lBankerCurGameScore;				//ׯ�ҳɼ�
	LONG							m_lGameRevenue;						//��Ϸ˰��

	//���ݱ���
protected:
	CPoint							m_PointJetton[4];					//����λ��
	CPoint							m_PointJettonNumber[4];				//����λ��
	CJettonInfoArray				m_JettonInfoArray[4];				//��������

	//�ؼ�����
public:
	CSkinButton						m_btJetton100;						//���밴ť
	CSkinButton						m_btJetton1000;						//���밴ť
	CSkinButton						m_btJetton10000;					//���밴ť
	CSkinButton						m_btJetton50000;					//���밴ť
	CSkinButton						m_btJetton100000;					//���밴ť
	CSkinButton						m_btJetton500000;					//���밴ť
	CSkinButton						m_btJetton1000000;					//���밴ť
	CSkinButton						m_btJetton5000000;					//���밴ť
//	CSkinButton						m_btViewChart;						//�鿴·��
	
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�

	//�ؼ�����
public:
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�
	CCardControl					m_CardControl[6];					//�˿˿ؼ�	
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�

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
	//CSkinImage						m_ImageFrameXianTianWang;			//�߿�ͼƬ
	//CSkinImage						m_ImageFrameZhuangTianWang;			//�߿�ͼƬ
	//CSkinImage						m_ImageFramePingJia;				//�߿�ͼƬ
	//CSkinImage						m_ImageFrameTongDianPing;			//�߿�ͼƬ
	//CSkinImage						m_ImageFramePlayerTwoPair;			//�߿�ͼƬ
	//CSkinImage						m_ImageFrameBankerTwoPair;			//�߿�ͼƬ

	CSkinImage						m_ImageMeBanker;					//�л�ׯ��
	CSkinImage						m_ImageChangeBanker;				//�л�ׯ��
	CSkinImage						m_ImageNoBanker;					//�л�ׯ��


	//������Դ
protected:
	CSkinImage						m_ImageGameEnd;						//�ɼ�ͼƬ
	CSkinImage						m_ImageGameEndFrame;				//�߿�ͼƬ
	CSkinImage						m_ImageGamePoint;					//����ͼƬ

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
	void SetAreaFlash(BYTE cbAreaFlash){m_cbAreaFlash=cbAreaFlash;}
	//������Ϣ
	void SetMeMaxScore(LONG lMeMaxScore);
	//�ҵ�λ��
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID; }
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, LONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore, LONG lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(bool bHaveCard,BYTE cbTableCardArray[6][3]);
	//���ó���
	void SetCurrentJetton(LONG lCurrentJetton);
	//��ʷ��¼
//	void SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair);
	void SetGameHistory(enOperateResult OperateResult,BYTE cbWinner);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard();
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//�滭��ը
	void DrawBombEffect(CDC *pDC);

	//��ȡ����ʱ���ͱ�־ͼ��
	BYTE GetIconNum(BYTE cbBufferData[], BYTE cbBufferCount);

	//��������
protected:
	//�����ע
	LONG GetMaxTianScore();
	//�����ע
	LONG GetMaxDiScore();
	//�����ע
	LONG GetMaxXuanScore();
	//�����ע
	LONG GetMaxHuangScore();


	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);


	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(BYTE cbWinnerSide);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//������ʾ
	void SetDispatchCardTips();

	//��������
public:
	//��ǰ����
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }	

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, CRect rcRect, INT nFormat=-1);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE &cbWinner);

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
	afx_msg void OnJettonButton50000();
	afx_msg void OnJettonButton500000();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();

	afx_msg void OnShouDongModel();
	afx_msg void OnZiDongModel();

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
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////

#endif
