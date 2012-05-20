#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ApplyUserList.h"
#include "GameLogic.h"
//#include "DlgViewChart.h"

//////////////////////////////////////////////////////////////////////////



//���붨��
#define JETTON_COUNT				7									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define	IDM_BANK					WM_USER+202
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

//��
#define HORSE_COUNT					8									//������Ŀ
struct tagHorseInfo
{
	bool							bStatic;							//���״̬
	bool							bBlow;								//�Ƿ���
	int								nXPos;								//���λ��
	int								nYPos;								//���λ��
	int								nSpeed;								//����ٶ�
	BYTE							cbHorseIndex;						//�������
	BYTE							cbHorseIndex1;						//�������

};

struct tagSignInfo
{
	float							fXPos;								//xλ��
	float							fYPos;								//yλ��
	float							fSpeed;								//�ٶ�
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
	LONGLONG							m_lMeMaxScore;						//�����ע
	LONGLONG							m_lAreaLimitScore;					//��������

//��ע��Ϣ
protected:

	LONGLONG							m_lScore2;							//��ƽ��ע
	LONGLONG							m_lScore3;							//��ׯ��ע
	LONGLONG							m_lScore1;							//������ע
	LONGLONG							m_lScore6;							//ƽ����ע
	LONGLONG							m_lScore5;							//ׯ����ע
	LONGLONG							m_lScore4;							//������ע
	LONGLONG							m_lScore7;							//�мҶ���
	LONGLONG							m_lScore8;							//ׯ�Ҷ���

	LONGLONG							m_lMeScore2;						//��ƽ��ע
	LONGLONG							m_lMeScore3;						//��ׯ��ע
	LONGLONG							m_lMeScore1;						//������ע
	LONGLONG							m_lMeScore6;						//ͬ��ƽע
	LONGLONG							m_lMeScore5;						//������ע
	LONGLONG							m_lMeScore4;						//ׯ����ע
	LONGLONG							m_lMeScore7;						//������ע
	LONGLONG							m_lMeScore8;						//������ע

	//ȫ����ע
	protected:
	LONGLONG							m_lAllScore2;						//��ƽ��ע
	LONGLONG							m_lAllScore3;						//��ׯ��ע
	LONGLONG							m_lAllScore1;						//������ע
	LONGLONG							m_lAllScore6;						//ƽ����ע
	LONGLONG							m_lAllScore5;						//ׯ����ע
	LONGLONG							m_lAllScore4;						//������ע
	LONGLONG							m_lAllScore7;						//�мҶ���
	LONGLONG							m_lAllScore8;						//ׯ�Ҷ���

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nWinPointsExcursionX;				//ƫ��λ��
	int								m_nWinPointsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcFrame1;							//�м�����
	CRect							m_rcFrame4;							//����������
	CRect							m_rcFrame7;							//��������
	CRect							m_rcFrame2;							//ƽ����
	CRect							m_rcFrame6;							//ͬ��ƽ����
	CRect							m_rcFrame3;							//ׯ������
	CRect							m_rcFrame5;							//ׯ��������
	CRect							m_rcFrame8;							//��������

	//��������
protected:
	bool							m_bBombEffect[9];					//��ըЧ��
	BYTE							m_cbBombFrameIndex[9];				//֡������
	BYTE							m_cbBlowIndex;						//����֡����

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_C_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	int								m_Num;								//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��

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
	LONGLONG						m_lMeCurGameReturnScore;			//���سɼ�
	LONGLONG						m_lBankerCurGameScore;				//ׯ�ҳɼ�
	LONGLONG						m_lGameRevenue;						//��Ϸ˰��
	LONGLONG						m_lMeCurJettonScore;				//�ҵĳɼ�

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
	CSkinButton						m_btJetton10000000;					//���밴ť
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btBank;							//����

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�

	//�ؼ�����
public:
	CApplyUser						m_ApplyUser;						//�����б�
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�

	//�������
protected:
	CSkinImage						m_ImageViewBack;					//����λͼ	
	
	CPngImage						m_ImageJettonView;					//������ͼ
	CPngImage						m_ImageMeScoreNumber;				//������ͼ

	CPngImage						m_ImageWinFlags;					//��־λͼ

	CPngImage						m_ImageBombEffect;					//����ͼƬ

	CPngImage						m_ImageSaticHorse[HORSE_COUNT];		//��̬ͼƬ
	CPngImage						m_ImageDynamicHorse[HORSE_COUNT];	//����ͼƬ
	CPngImage						m_ImageGround;						//�ܵ�ͼƬ
	CPngImage						m_ImageBgJetton;					//��ע����
	CSkinImage						m_ImageCut;							//����Ч��
	CPngImage						m_ImageSign;						//С��ͼ
	CPngImage						m_ImageStartLine;					//������
	CPngImage						m_ImageEndLine;						//������
	CPngImage						m_ImageMeScore;						//�ҵ���ע
	CPngImage						m_ImageFirst;						//NO.1
	CPngImage						m_ImageBlow;						//��Ϸ����
	CPngImage						m_ImageResult;						//��Ϸ����
	CSkinImage						m_ImageWin;							//ʤ�����

	CPngImage						m_ImageFrame;						//��Ϸ����

	int								m_nPosX;
	int								m_nStartPosX;
	int								m_nEndPosX;
	tagHorseInfo					m_Horse[HORSE_COUNT];
	tagSignInfo						m_Sign[HORSE_COUNT];


	//�߿���Դ
protected:
	//CSkinImage						m_ImageFrame;					//�߿�ͼƬ

	CPngImage						m_ImageMeBanker;					//�л�ׯ��
	CPngImage						m_ImageChangeBanker;				//�л�ׯ��
	CPngImage						m_ImageNoBanker;					//�л�ׯ��
	

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
	void SetHorseSpeed(BYTE cbSpeedArray[8][8]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(enOperateResult OperateResult, BYTE cbWinner);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	//�����Ϣ
public:
	BYTE							m_cbHorseSpeed[HORSE_COUNT][HORSE_COUNT];
	BYTE							m_nSprint;
	bool                            m_bFinish;
	bool                            m_bCutPhoto;

	//�����Ϣ��ʼ��
	void InitHorseInfo();
	//��������
	void FinishRun();
	//���������ٶ�
	void SetEveryHorseSpeed(BYTE cbSpeed);

	void SetFreeTime(); 

	BYTE GetFastOrLater(bool bFast = true);

	bool GetIsFinishRace(){return m_bCutPhoto;}

	void InitSignInfo();

	//�滭ʱ��
	void DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//��������
public:	
	////��ʼ����
	//void DispatchCard();
	////��������
	//void FinishDispatchCard();
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//�滭��ը
	void DrawBombEffect(CDC *pDC);

	//��������
	void DrawHorse(CDC *pDC);
	//�����ҵ���ע
	void DrawMeScore(CDC * pDC, int nWidth, int nHeight);

	//��������
protected:
	//�����ע �м�
	LONGLONG GetMaxHorse1();
	//�����ע ������
	LONGLONG GetMaxHorse4();
	//�����ע ׯ��
	LONGLONG GetMaxHorse3();
	//�����ע ׯ����
	LONGLONG GetMaxHorse5();
	//�����ע ƽ
	LONGLONG GetMaxHorse2();
	//�����ע ƽ����
	LONGLONG GetMaxHorse6();
	//�����ע �мҶ���
	LONGLONG GetMaxHorse7();
	//�����ע ׯ�Ҷ���
	LONGLONG GetMaxHorse8();


	//�滭����
protected:
	////��ע��˸�߿�
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

	//�����ͼ
	void CutPhoto(int nWidth, int nHeight, CDC *pDC);

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
	//void DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair);

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
	//���밴ť
	afx_msg void OnJettonButton10000000();
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
	//��Ǯ��ť
	afx_msg void OnBank();
	////ȡǮ��ť
	//afx_msg void OnBankGet();
	//
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
