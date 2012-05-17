#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "..\��Ϸ������\GameLogic.h"
#include "SiceControl.h"
#include "DlgWinCtrl.h"

//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_CONTINUE_CARD			WM_USER+202							//��������
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//��ť��ʶ
#define IDM_OPEN_CARD				WM_USER+204
#define IDM_MANAGE_CONTROL			WM_USER+205							//������Ϣ

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

#define	IDM_BANK					WM_USER+206

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��ʷ����
#define MAX_FALG_COUNT				14									//��ʶ����

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//��¼��Ϣ
struct tagClientGameRecord
{
	enOperateResult					enOperateTianMen;					//������ʶ
	enOperateResult					enOperateDiMen;						//������ʶ
	enOperateResult					enOperateXuanMen;					//������ʶ
	bool							bWinTianMen;						//˳��ʤ��
	bool							bWinDiMen;							//����ʤ��
	bool							bWinXuanMen;						//����ʤ��
};

//������ʾ
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//��������
	enDispatchCardTip_Dispatch											//����ϴ��
};

//��������
typedef CArrayTemplate<tagJettonInfo, tagJettonInfo&> CJettonInfoArray;

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
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CPoint							m_rcTianMen;						//�м�����
	CRgn							m_rgnTianMen;						//����
	CPoint							m_rcDimen;							//����������
	CRgn							m_rgnDimen;							//����
//	CRect							m_rcQiao;							//��������
//	CRect							m_rcHuangMen;						//ƽ����
	CPoint							m_rcXuanMen;						//ͬ��ƽ����
	CRgn							m_rgnXuanmen;						//����
//	CRect							m_rcHuang;							//ͬ��ƽ����
//	CRect							m_rcJiaoR;							//ׯ������
	int								m_OpenCardIndex;					//�_�����
	int								m_PostCardIndex;					//�l�����
	int								m_PostStartIndex;					//������ʼλ��

	//�˿���Ϣ
public:
	BYTE							m_cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE];			//�����˿�
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard[2];
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//���Զ�
	LONG							m_lUserCardType[CARD_LOCATION_COUNT];		//����

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼
	BYTE							m_CardHistoryArray[5][8];			//��Ϸ��¼���齫��

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������
	bool							m_bWinTianMen;						//ʤ����ʶ
	bool                            m_bWinDiMen;
	bool							m_bWinXuanMen;						//ʤ����ʶ
	bool							m_bFlashResult;						//��ʾ���
	bool							m_bShowGameResult;					//��ʾ���
	enDispatchCardTip				m_enDispatchCardTip;				//������ʾ

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
	CPoint							m_PointJetton[AREA_COUNT];				//����λ��
	CPoint							m_PointJettonNumber[AREA_COUNT];		//����λ��
	CJettonInfoArray				m_JettonInfoArray[JETTON_COUNT];		//��������

	//�ؼ�����
public:
//	CSkinButton						m_btJetton100;						//���밴ť
	CSkinButton						m_btJetton1000;						//���밴ť
	CSkinButton						m_btJetton10000;					//���밴ť
	CSkinButton						m_btJetton50000;					//���밴ť
	CSkinButton						m_btJetton100000;					//���밴ť
	CSkinButton						m_btJetton500000;					//���밴ť
	CSkinButton						m_btJetton1000000;					//���밴ť
//	CSkinButton						m_btJetton5000000;					//���밴ť

	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��
	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btAutoOpenCard;					//�Զ�����
	CSkinButton						m_btOpenCard;						//�ֶ�����
	CSkinButton						m_btBank;							//���а�ť
	CSkinButton						m_btContinueCard;					//��������
	CSkinButton						m_btBankerStorage;					//��ť
	CSkinButton						m_btBankerDraw;						//ȡ�ť
	CSkinButton						m_btUp;								//�����б�����
	CSkinButton						m_btDown;							//�����б�����
	CSkinButton						m_btWinRate;						//���Ӯ��

	//�ؼ�����
public:
	CDlgWinCtrl						m_dlgWinCtrl;						//����Ӯ��
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�
	CCardControl					m_CardControl[CARD_LOCATION_COUNT];					//�˿˿ؼ�
	CSiceControl					m_Sice[2];							//��������
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
//	CGameBank						m_DlgBank;							//���пؼ�
	CRect							m_MeInfoRect;
	bool							m_blCanStore;

public:
	bool							m_bShowLeft;						//��ʾ��ҽ�
	bool							m_bShowRight;						//��ʾ�ҹҽ�

	//�������
protected:
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewBack;					//����λͼ
	CSkinImage						m_ImageWinFlags;					//��־λͼ
	CSkinImage						m_ImageJettonView;					//������ͼ
	CSkinImage						m_ImageScoreNumber;					//������ͼ
	CSkinImage						m_ImageMeScoreNumber;				//������ͼ
	CSkinImage						m_ImageTimeFlag;					//ʱ���ʶ
	CSkinImage						m_ImageDispatchCardTip;				//������ʾ


	CSkinImage						m_ImageBackLeft01;					//��ҽ�01
	CSkinImage						m_ImageBackLeft02;					//��ҽ�02
	CSkinImage						m_ImageBackRight01;					//�ҹҽ�01
	CSkinImage						m_ImageBackRight02;					//�ҹҽ�02

	//�߿���Դ
protected:
	CSkinImage						m_ImageFrameTianMen;				//�߿�ͼƬ
	CSkinImage						m_ImageFrameDiMen;					//�߿�ͼƬ
	CSkinImage						m_ImageFrameXuanMen;				//�߿�ͼƬ
	CSkinImage						m_ImageFrameQiao;					//�߿�ͼƬ
	CSkinImage						m_ImageFrameJiaoR;					//�߿�ͼƬ

	CSkinImage						m_ImageMeBanker;					//�л�ׯ��
	CSkinImage						m_ImageChangeBanker;				//�л�ׯ��
	CSkinImage						m_ImageNoBanker;					//�л�ׯ��
	CSkinImage						m_ImageWinRate;						//Ӯ����Դ

	//������Դ
protected:
	CSkinImage						m_ImageGameEnd;						//�ɼ�ͼƬ

	CPngImage						m_ImageTP;				//ͼƬ��Դ
	CPngImage						m_ImageTS;				//ͼƬ��Դ

	//zhangchen
protected:
	CSkinImage						m_ImageCodeBack;						//�齫����
	CSkinImage						m_ImageCurrentJoinCount;				//��ǰ����ע
	CPngImage						m_ImageGameBegin;						//�齫����
	CSkinImage						m_ImageCardSmall;						//�齫��ʷ


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
	void SetMeChairID(DWORD dwMeUserID);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore)
	{
		m_wBankerTime = wBankerTime;
		m_lTmpBankerWinScore = lWinScore;
	}
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);
	//�����˿�
	void SetCardInfo(BYTE cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(bool bWinTian, bool bWinDi, bool bWinXuan);
	//�ֻ�ׯ��
	void ShowChangeBanker(bool bChangeBanker);
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker)
	{
		m_bEnableSysBanker = bEnableSysBanker;
	}
	//������ʾ
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);

	void SetFirstShowCard(BYTE *bcCard);
	//��գ���
	void ClearBank();
	//������ʷ�齫
	void SetCodeHistory(BYTE bCode[8]);

	//��������
public:
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard();
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton();

	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool bWinTian, bool bWinDi, bool bWinXuan, bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//�滭ׯ��
	void DrawBankerInfo(CDC *pDC, int nWidth, int nHeight);
	//�滭���
	void DrawMeInfo(CDC *pDC, int nWidth, int nHeight);

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton()
	{
		return m_lCurrentJetton;
	}
	void SetMoveCardTimer();

	void KillCardTime();

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat = -1);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);

	//
	void DrawType(CDC* pDC, WORD wChairID);
	//�ƶ�Ӯ��
	void DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan, BYTE &TianMultiple, BYTE &diMultiple, BYTE &TianXuanltiple);


	//��ť��Ϣ
protected:
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	//�ֹ�����
	afx_msg void OnOpenCard();
	//�Զ�����
	afx_msg void OnAutoOpenCard();
	//���д��
	afx_msg void OnBankStorage();
	//����ȡ��
	afx_msg void OnBankDraw();

	afx_msg void OnUp();

	afx_msg void OnDown();

public:
	//��Ӯ����
	afx_msg void OnControl();
	void ShowWinCtrl();

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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�ؼ�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�����ؼ����UP��Ϣ
	LRESULT OnViLBtUp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
