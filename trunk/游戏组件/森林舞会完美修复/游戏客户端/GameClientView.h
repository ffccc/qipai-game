#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

#include "RecordGameList.h"
#include ".\appuserlist.h"
#include "RecordLZ.h"
#include <time.h>
#include "..\..\..\qplib\include\ui\TimeCtrlSprite.h"
///////////////////

#define JETTON_TYPE_COUNT			 3									//������Ŀ
  #define       HISTORY_RECORD_SHOW 10
//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����
#define INVALID_CARD				0xff								//��Ч�˿�

//�����Ӧ�Ľ���
const LONG JETTON_AMOUNT[JETTON_TYPE_COUNT] =
{
	 1,  5, 10
};


//////////��������
enum
{
	OUT_LEFT_CARD=1,
	OUT_RIGHT_CARD=2,        
};


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

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView: public CGameFrameView, public CGameWnd
{
	//��ע��Ϣ
public:
	LONG							m_lMyMaxStake;						//�����ע
	LONG							m_lAllMaxScore[INDEX_MAX];			//ÿ������������ע��
	LONG							m_lMyStakes[INDEX_MAX];				//�Լ��ĵ���ע���
	LONG							m_lAllStakes[INDEX_MAX];			//ֻ����עλ��ͳ�Ƶ���ע���
	LONG							m_Prizemoney  ;			            //�ʽ�
	LONG							m_lmaxStakes[INDEX_MAX];			//ÿ������Ŀ���ע��
	LONG							m_lBankerMaxLoss;					//ׯ������⸶
	LONG							m_lBankers;					        //ׯ�ҷ���
	LONG							m_lBankerwin;					    //ׯ�ҳɼ�
	LONG							m_lMywin;					        //��ҷ���
	WORD							m_wMyChairID;						//�ҵ�λ��
	bool                            m_endsrok;                          //�����־
    bool                            m_endsrok2;                         //�����־
	int                             m_timenum;
	bool                            m_timesound;
	bool                            m_move;
    CPoint                          m_lzPoint;
	bool                            m_bDrawWinArea;
	int								m_pZPIndex;							// ת����������λ��
	BOOL                            m_iSMyWin;                          //�Ƿ���ʾ���ѵķ���
	LONG                            m_wBankerScore;

	LONG                           m_MySlof;                           //��ҷ���

	long							m_wMyWinCount;						//Ӯ�ľ���


	//ׯ����Ϣ
protected:
	SURFACE*						m_pImageShowBanker;					//�ֻ�ׯ��
	WORD							m_wBankerCount;						//��ׯ����
	LONG							m_lBankerWinScore;					//ׯ�ҳɼ�


	//״̬����
protected:

	DWORD							m_dwWinnerFlash;					//ʤ�����
	INT								m_nWinnerFlashIndex;				//ʤ������֡
	LONG							m_lCurrentJetton;					//��ǰ����
	BYTE							m_cbPlayerValue;					//�м��Ƶ�
	BYTE							m_cbBankerValue;					//ׯ���Ƶ�
	CString							m_strGameHint;						//��Ϸ��ʾ��Ϣ
    CPoint                          m_showXy[24];
	int                             m_showPrize;
	bool                            m_showPrizeBZ;

   
	CPoint                          m_time;                          //ʱ������
	//���ݱ���
public:
	CRect							m_RectJetton[INDEX_MAX];			//���γ���λ��
	CJettonInfoArray				m_JettonInfoArray[INDEX_MAX];		//��������


    int                             m_pTipsX;


	//�ؼ�����
	BYTE                             m_showT[24];   
	int                              m_showTX[24];  
	BYTE                             m_showPL[INDEX_MAX]; 

	int                              m_showrobst[24];  
	bool                            m_showForT;
  	bool                            vBankerShow;
	CGameButton						m_btnJettons[JETTON_TYPE_COUNT];	//���밴ť����
	CGameButton						m_pImagefang;					    //ȡ��
	CAppUserList					m_AppList;
	CGameRecord						m_GameRecord;						//��¼�б�
	CGameButton						m_btnApplyBanker;					//����ׯ��
	CGameButton						m_btnCancelBanker;					//ȡ��ׯ��
	CGameButton						m_infoup;					        //��¼
	CGameButton						m_infodown;					        //��¼
	CGameButton						m_btnsupper[8];					    //ׯ���·�
	CGameButton						m_pGameB;					    //������Ϸ��
	CGameButton						m_pGame50;					    //������Ϸ��
	CGameButton						m_pGame100;					    //������Ϸ��
	CGameButton						m_pGame500;					    //������Ϸ��
	CGameButton						m_pGame1000;					    //������Ϸ��
	CGameButton						m_pGameClose;					    //������Ϸ��
	CGameButton						m_pSettlement;					    //����
	CGameButton                     m_BankerShow;

    CPoint                          m_pGameBZ;
	LONG                            m_MyGame;                           //���ѵ���Ϸ��
	bool                            m_supper;                           //�Ƿ񳬼�
	bool                            m_GameBi;
	BYTE                            m_pYP;
	WORD							m_wCurrentBanker;					//��ǰׯ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	BYTE							m_cbCardCount;			        	//�˿���Ŀ
	BYTE                            m_Card;
	CWnd*							m_pWinCallbak;						//��Ϣ�ص��ӿ�
	BYTE                            m_timezt;

	bool                            m_endsrok3;
	//������Դ���
protected:
	CString							m_strResPath;						//��Դ�ļ���·��

	SURFACE*                        m_Bankerbg;	                        //ׯ��.psd
	SURFACE*						m_pImageViewBack;					//����λͼ
 	SURFACE*						m_pImageStakeAmountTotal2;			//����ע��λͼ2
	SURFACE*						m_pImageStakeAmountMine2;			//�Լ���ע��λͼ
 	SURFACE*					    m_pImageHistoryWin;					//Ӯ������λͼ
	SURFACE*						m_pImageTimer;                      //����ʱ��
	SURFACE*					    m_pImageChangeBanker;				//�ֻ�ׯ��
	SURFACE*					    m_pImageHistory;					//��ʷ��¼ͼ
	HCURSOR							m_JettonCursors;	//�����״
	SURFACE*						m_pImageTimerbg;                      //����ʱ��
	SURFACE*                         	m_pImageplay;
	SURFACE*                         	m_pImagesend;
	SURFACE*					    m_pImageNoBanker;					//��ׯ��
	SURFACE*					    m_pImageMeBanker;					//�Լ���ׯ
	SURFACE*                        m_pImagekuan;
	SURFACE*						m_show[12];						// ת��ͼƬ
	SURFACE*						m_WinTips;   					// �вʽ���ʾ
	SURFACE*						m_WinTitle;						// �вʽ���
	SURFACE*						m_WinPrize;						// �ʵ�
	SURFACE*						m_Winbg;						// �вʱ���
	SURFACE*					    m_pImageBankbg;					//������Ϣ
	SURFACE*	                 	m_kuang;

	int								time;
public:

	BOOL							g_Tips;			
	BOOL							g_Title;		//�ʽ𶯻�
    BOOL                            ISDrawlight;
	LONG                            cdPrize;

private:
	UINT IDD;
	CWinThread *pShakeThread;
    //��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
    void StartHyMovie();
	void Create(RECT rc, CGameWnd *pWnd);
	void Animation(bool bClose);
	void Drawlight(SURFACE* psurfTarget,BYTE x);
    void TimerFLASH();
    void TipsFLASH();
    void StartWin();
    void CloseWin();
	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//����λ��
   char* DrawNUM(INT64 num);
   string s1;
	//virtual void OnSize();
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ܺ���
public:
	//���������ע��
	void SetMyMaxScore(LONG lMyMaxStake);

	void KillTimerFLASH();
	
	//����ÿ������������ע��
	void SetAllMaxScore(const INT64 lMaxScore[]);

	//���ó���
	void SetCurrentJetton(LONG lCurrentJetton);

	//�����Լ������������ע��Ϣ
	void SetMyStake(const INT64 plMyStakes[]);

	//�����Լ������������ע��Ϣ
	void SetMyStake(BYTE cbBetArea, LONG lBetAmount);

	//������Ϸ�������ۼ�����
	void SetHistoryScore(WORD wWinCount, WORD wLossCount, INT64 lMyWinAmountSum);
	
	//ׯ����Ϣ
	void SetBankerInfo( WORD wChairID, WORD wBankerCount, LONG lBankerWinScore ,LONG lBankerScore );

	//��ʾ�������
	void ShowWinnerFlash(SURFACE* purSource, int nWidth, int nHeight) ;

	void DrawNumber(SURFACE* psurfTarget,int nTime,int x,int y,bool i);

	void updateYazhumoney(SURFACE* psurfTarget,int nXPos,int nYPos);

    void Drawumoney(SURFACE* psurfTarget,int g_money,int nX,int nXP,int nYP);
	//���溯��
public:

	//�������
	void CleanUserJetton();
	
	//���ö������ĳ���
	void PlaceUserJetton(const INT64 plBetAmounts[]);

	//���õ�������ĳ���
	void PlaceUserJetton(BYTE cbBetArea, LONG lBetAmount,int cbchar);

	//����ʤ��
	void SetWinner(BYTE cbPlayerValue, BYTE cbBankerValue, DWORD dwWinner);

	void SetWinAmount(LONG lWinAmount);

	void SetGameHint(LPCTSTR pszFormat, ...);

	//��������
public:
	//��ǰ����
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }
	//�ҵ�λ��
	inline void SetMyChairID(WORD wMyChairID) { m_wMyChairID=wMyChairID; }

	virtual void SetUserLeft(BOOL bShow){};

	virtual void SetLoadFinish(){};

	void DrawUserTimer(SURFACE* psurfTarget, int xPos, int yPos, int Timer);

    void InsertSystemString(LPCTSTR pszString, ...);

	virtual void OnDraw(SURFACE* psurfTarget);

	bool OnCreate();

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);

	void LoadRes();

	//��ť��Ϣ
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//���밴ť����¼�
	LRESULT OnButtonClick(WPARAM wParam, LPARAM lParam);

    static UINT ShowAnimalFunction( LPVOID pParam); //�����߳�

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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//��ׯ��ť
	afx_msg void OnApplyBanker();

	//��ׯ��ť
	afx_msg void OnCancelBanker();

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////

#endif