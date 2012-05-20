#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#ifdef  __BANKER___
#include "DlgBank.h"
#endif
#include "..\��Ϸ������\GameLogic.h"
#include "AdminControlDlg.h"


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
#define IDM_SHOW_RESULT				WM_USER+205		
#define IDM_SOUND					WM_USER+206		


//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��ʷ����
#define MAX_FALG_COUNT				9									//��ʶ����
#define COUNT_DRAWINFO              18

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
	enOperateResult					enOperateMen[AREA_COUNT+1];					//������ʶ
	bool							bWinMen[AREA_COUNT+1];						//˳��ʤ��

};

//������ʾ
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//��������
	enDispatchCardTip_Dispatch											//����ϴ��
};

struct  sT_ShowInfo
{
	CRect RcRect[AREA_COUNT+1];
	CPoint point[AREA_COUNT+1];
	bool  blShow[AREA_COUNT+1];
};
struct  sT_ShowJetton
{
	bool  blShow[6];
	sT_ShowJetton()
	{
		for (int i = 0;i<6;i++){
			blShow[i]=true;
		}
	}
};
//��������
typedef CArrayTemplate<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

struct tagAndroidBet
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//���λ��
	int								nLeftTime;							//ʣ��ʱ�� (100msΪ��λ)
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lRobotMaxJetton;					//������������

	//��ע��Ϣ
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע
	LONGLONG						m_JettonQue[COUNT_DRAWINFO];
	BYTE							m_JettonQueArea[COUNT_DRAWINFO];
	BYTE							m_JettonQueIndex;

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcTianMen;						//�м�����
	CRect							m_rcDimen;							//����������
	CRect							m_rcQiao;							//��������
	CRect							m_rcHuangMen;							//ƽ����
	CRect							m_rcXuanMen;							//ͬ��ƽ����
	CRect							m_rcHuang;							//ͬ��ƽ����
	CRect							m_rcJiaoR;							//ׯ������
	int								m_OpenCardIndex;					//�_�����
	int								m_PostCardIndex;					//�l�����
	int								m_PostStartIndex;					//������ʼλ��
	CPoint							m_CardTypePoint[AREA_COUNT+1];
   

	//�˿���Ϣ
public:	
    BYTE							m_cbTableCardArray[1][1];			//�����˿�
	BYTE							m_cbTableSortCardArray[1][1];			//�����˿�
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//���Զ�
	LONGLONG						m_lUserCardType[AREA_COUNT+1];	//������ע
	CRect							m_JettonAreaRect[AREA_COUNT];
	CRect m_cTmpRect[64];
	sT_ShowJetton                   m_T_ShowJetton;

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������
	bool							m_bWinTianMen;						//ʤ����ʶ
	bool							m_bWinHuangMen;						//ʤ����ʶ
	bool                            m_bWinDiMen;
	bool							m_bWinXuanMen;						//ʤ����ʶ
	bool							m_bFlashResult;						//��ʾ���
	bool							m_bShowGameResult;					//��ʾ���
	enDispatchCardTip				m_enDispatchCardTip;				//������ʾ
	CPngImage						m_ViewBackPng;
	

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
	CPoint							m_PointJetton[AREA_COUNT];			//����λ��
	CPoint							m_PointJettonNumber[AREA_COUNT];	//����λ��
	CJettonInfoArray				m_JettonInfoArray[AREA_COUNT];		//��������
	bool							m_bWinFlag[AREA_COUNT];						//ʤ����ʶ
	int								m_TopHeight;
	int								m_LifeWidth;
	int								m_CurArea;

	//��������ע
	CArrayTemplate<tagAndroidBet>	m_ArrayAndroid;

	//�ؼ�����
public:

	
	CRgnButton						m_btJetton100;						//���밴ť
	CRgnButton						m_btJetton1000;						//���밴ť
	CRgnButton						m_btJetton10000;					//���밴ť
	CRgnButton						m_btJetton50000;					//���밴ť
	CRgnButton						m_btJetton100000;					//���밴ť
	CRgnButton						m_btJetton500000;					//���밴ť
	CRgnButton						m_btJetton1000000;					//���밴ť
	CRgnButton						m_btJetton5000000;					//���밴ť
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��
	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btAutoOpenCard;					//�Զ�����
	CSkinButton						m_btOpenCard;						//�ֶ�����
	CSkinButton						m_btBank;							//���а�ť

	CSkinButton						m_btBankerStorage;					//��ť
	CSkinButton						m_btBankerDraw;						//ȡ�ť
	CSkinButton						m_btUp;								//��ť
	CSkinButton						m_btDown;							//ȡ�ť
	CButton							m_btOpenAdmin;						//ϵͳ����

	int								m_ShowImageIndex;
	int								m_CheckImagIndex;
	//�ؼ�����
public:
	CAdminControlDlg				m_AdminControl;						//����
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�
	
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
#ifdef  __BANKER___
	CDlgBank						m_DlgBank;							//���пؼ�
#endif
	CRect							m_MeInfoRect;						//
	int								m_Out_Bao_y;
	bool						    m_bShowBao;
	CRect							m_CarRect[32];		
	CPngImage						m_idb_selPng;
	int								m_CarIndex ;

	//����
public:
	bool							m_blCanStore;                       //�Ƿ��ܱ���

	//�������
protected:
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewcentre;					//����λͼ
	CPngImage						m_ImageWinFlags;					//��־λͼ
	CSkinImage						m_ImageJettonView;					//������ͼ
	CSkinImage						m_ImageScoreNumber;					//������ͼ
	CSkinImage						m_ImageMeScoreNumber;				//������ͼ

	CPngImage						m_ImageTimeFlagPng;					//ʱ���ʶ
	CSkinImage						m_ImageDispatchCardTip;				//������ʾ
	CSkinImage						m_ImageCardType[AREA_COUNT+1];		//������ͼ
	CSkinImage						m_ImageBao;							//������ʾ
	CRect							m_BaoPosion;

	//�߿���Դ
protected:

	CSkinImage						m_ImageFrameTianMen;				//�߿�ͼƬ
	CSkinImage						m_ImageFrameDiMen;					//�߿�ͼƬ
	CSkinImage						m_ImageFrameHuangMen;				//�߿�ͼƬ
	CSkinImage						m_ImageFrameXuanMen;				//�߿�ͼƬ
	CSkinImage						m_ImageFrameQiao;					//�߿�ͼƬ
	CSkinImage						m_ImageFrameJiaoR;					//�߿�ͼƬ
	CSkinImage						m_ImageMeBanker;					//�л�ׯ��
	CSkinImage						m_ImageChangeBanker;				//�л�ׯ��
	CSkinImage						m_ImageNoBanker;					//�л�ׯ��
	bool							m_blMoveShowInfo;

protected:

	CPngImage                          m_PngPushBox[7];
	CPngImage                          m_PngShowJetton[4];
	bool							   m_bShowJettonAn;
	int                                m_bShowJettonIndex;
	CPngImage                          m_PngShowLeaveHandle[4];
	bool							   m_bShowLeaveHandleAn;
	int                                m_bShowLeaveHandleIndex;
	bool							   m_bOPenBoxAn;
	int                                m_bOPenBoxIndex;
	bool                               m_blShowLastResult;

	CPngImage                          m_PngResult[4];
	CPngImage                          m_TimerCount_png;
	CPngImage						   m_ImageMeScoreNumberPng;				//������ͼ
	CPngImage						   m_ImageScoreNumberPng;

	bool							   m_bShowResult;
	bool                               m_blShowResultIndex;
	LONGLONG						   m_lLastJetton;


	//������Դ
protected:
	CSkinImage						m_ImageGameEnd;						//�ɼ�ͼƬ
	CPngImage						m_pngGameEnd;
	bool							m_DrawBack;
	bool							m_bEnablePlaceJetton;

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
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������ע
	void AndroidBet(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbTableCardArray[1][1]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(BYTE *bcResulte);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//������ʾ
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);
	//ִ��ʣ�����еĻ��嶯��
	void PerformAllBetAnimation();
	//���û�����������
	void SetRobotMaxJetton(LONGLONG lRobotMaxJetton){ m_lRobotMaxJetton = lRobotMaxJetton; };

	void SetFirstShowCard(BYTE bcCard);
	//ȡ������
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }

	void SetEnablePlaceJetton(bool bEnablePlaceJetton);

	void UpdataJettonButton();

	void StartRunCar(int iTimerStep);

	void RuningCar(int iTimerStep);

	void FlashWinOpenSide();

	void StartRandShowSide();

	CString NumberStringWithSpace(LONGLONG lNumber);

	int ifirstTimer;

	bool m_bFlashrandShow;

	bool m_blRungingCar;

	int iTimerStep;

	int iOpenSide;

	int iRunIndex;

	int iTotoalRun;

	CRect  m_RectArea[8];

	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard( bool bRecord = true );
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton(BYTE cbJettonArea = 0);

	void  GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);

	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool blWin[],bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//�滭ׯ��
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭���
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	

	void KillCardTime();

	void SwitchToCheck();

	void SwithToNormalView();

	void SetMoveCardTimer();

	void StartMove();

	void StartJetton_AniMationN();

	void StartOPenBox();

	void StartHandle_Leave();

	void StarShowResult();

	void MyDrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea);

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool blTimer= false,bool bMeScore = false);
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
	void DeduceWinner(bool bWinMen[]);

	void DrawMoveInfo(CDC* pDC, CRect rcRect);

	void SetJettonHide(int ID);


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
	//����Ա����
	afx_msg void OpenAdminWnd();

	DECLARE_MESSAGE_MAP()	

};

//////////////////////////////////////////////////////////////////////////

#endif
