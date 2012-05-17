#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../��Ϣ����/GameLogic.h"
#include "TransparentCtrl.h"
#include "SkinButtonEx.h"
#include "afxtempl.h"
//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				9									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			(WM_USER + 200)							// ��ס��Ϣ
#define IDM_APPLY_BANKER			(WM_USER + 201)							// ������Ϣ
#define IDM_CHANGE_SICBO_TYPE       (WM_USER + 202)                         // ҡ���ӷ�ʽ
#define IDM_OPEN_BANK               (WM_USER + 203)                         // ���в���
#define IDM_USE_WINNER              (WM_USER + 204)                    // ��ҿ���

#define SICBO_ANIM_FRAME_COUNT      18
//////////////////////////////////////////////////////////////////////////

//�ṹ����

//��¼��Ϣ
struct tagClientGameRecord
{
	bool					bFlags;						// �ǿ����ǿ�С
	int                     iCards[MAX_COUNT_SICBO];
};

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};

// ����ʱ�����ӵĶ����켣
struct tagSicboAnim
{
	BYTE byResultX;  // ���ӵĵ���(����ȷ���˾�̬ʱȡX��)
	BYTE byResultY;  // ��ֹ״̬ʱȡY��
	BYTE byAnimX;   // �˶����������һ������ȡx��
	BYTE byAnimY;   // �˶����������һ������ȡy��
	CPoint ptEnd;   // ���ֹͣ������
};

struct tagApplyUser
{
	//�����Ϣ
	CString							strUserName;						//����ʺ�
	__int64							lUserScore;							//��ҽ��
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
	__int64							m_lMeMaxScore;						// �����ע
	__int64                         m_lAllMaxScore;                     // ���ֿ�����ע����ע
    __int64							m_lAreaLimitScore;					//��������

	//��ע��Ϣ
protected:
	__int64							m_lMeScore[COUNT_AZIMUTH];	        //ÿ����λ��ע

	//ȫ����ע
protected:
	__int64							m_lAllScore[COUNT_AZIMUTH];			//ÿ����λ��ע

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	CPoint                          m_ptApplyListPos;
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcUser[COUNT_AZIMUTH];			//�����ע����

	//�˿���Ϣ
protected:	
	E_SICBO_NUMBER					m_enCards[MAX_COUNT_SICBO];         // ���ӵ���
	bool					        m_bWinner[COUNT_AZIMUTH];           // ��Ӯ���

	//��������
protected:
	CPoint                          m_ptSicboAnim[SICBOTYPE_COUNT];         // ���Ӷ�������(ÿ��ҡ���ӵķ�ʽ���겻ͬ)
	tagSicboAnim                    m_SicboAnimInfo[MAX_COUNT_SICBO];       // �����������ҡ���ӵĶ�����Ϣ
	BYTE                            m_bySicboDraw[MAX_COUNT_SICBO];         // �����ӵ�˳��
	bool                            m_bSicboShow;                           // ������ɺ���ʾ���
	bool							m_bSicboEffect;		                    // ҡ����Ч��
	BYTE                            m_bySicboFrameIndex;                    // ��ǰ֡
	bool							m_bBombEffect[COUNT_AZIMUTH];			// ��ըЧ��
	BYTE							m_cbBombFrameIndex[COUNT_AZIMUTH];		// ֡������

	//��ʷ��Ϣ
protected:
	__int64                         m_lMeStatisticScore;				  // ��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];// ��Ϸ��¼
	int								m_nRecordFirst;						  // ��ʼ��¼
	int								m_nRecordLast;						  // ����¼
	CArray<tagApplyUser>            m_ApplyUserArray;                     // ����ׯ�Ҷ���
	int                             m_iApplyFirst;                        // �صڼ�����ʼ��ʾ

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbAreaFlash;						//ʤ�����
	__int64							m_lCurrentJetton;					//��ǰ����
	CString							m_strDispatchCardTips;				//������ʾ
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bShowChangeSicbo;				    // �ı�ҡ���ӷ�ʽ

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	__int64							m_lBankerScore;						//ׯ�һ���
	__int64							m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	E_CARD_TYPE                     m_enSelArea;                        // ѡ������
	__int64                         m_lSelAreaMax;                      // ѡ��������·�

	//���ֳɼ�
public:
	__int64							m_lMeCurGameScore;					//�ҵĳɼ�
	__int64							m_lMeCurGameReturnScore;			//�ҵĳɼ�
	__int64							m_lBankerCurGameScore;				//ׯ�ҳɼ�
	__int64							m_lGameRevenue;						//��Ϸ˰��
	__int64							m_lBankerWinScore;					//ׯ�ҳɼ�

	//���ݱ���
protected:
	CPoint							m_PointJetton[COUNT_AZIMUTH];					//����λ��
	CPoint							m_PointJettonNumber[COUNT_AZIMUTH];				//����λ��
	CJettonInfoArray				m_JettonInfoArray[COUNT_AZIMUTH];				//��������

	//�ؼ�����
public:
	E_SICBO_TYPE                    m_enSicboType;                       // ҡ��������
	CSkinButtonEx						m_btJetton100;						//���밴ť
	CSkinButtonEx						m_btJetton1000;						//���밴ť
	CSkinButtonEx						m_btJetton10000;					//���밴ť
	CSkinButtonEx						m_btJetton100000;					//���밴ť
	CSkinButtonEx						m_btJetton500000;					//���밴ť
	CSkinButtonEx						m_btJetton1000000;					//���밴ť
	CSkinButtonEx						m_btJetton5000000;					//���밴ť
	
	CSkinButtonEx						m_btApplyBanker;					//����ׯ��
	CSkinButtonEx						m_btCancelBanker;					//ȡ��ׯ��
	CSkinButtonEx						m_btBankerMoveUp;					//�ƶ�ׯ��
	CSkinButtonEx						m_btBankerMoveDown;					//�ƶ�ׯ��

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	
	CSkinButton                     m_btSicboDesktop;                    // ����ҡ����
	CSkinButton                     m_btSicboInMidair;                   // ���ҡ����
	CSkinButton                     m_btSicboInTheAir;                   // ����ҡ����

	CSkinButton                     m_btOpenBank;                       // ����

	//�ؼ�����
public:
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�

	//�������
protected:
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewBack;					//����λͼ
	CSkinImage						m_ImageWinFlags;					//��־λͼ
	CSkinImage						m_ImageCardDot;					    //���ӵ���
	CPngImage						m_ImageJettonView;					//������ͼ
	CSkinImage						m_ImageScoreNumber;					//������ͼ
	CSkinImage						m_ImageMeScoreNumber;				//������ͼ
	CSkinImage						m_ImageTimeFlag;					//ʱ���ʶ
	CPngImage						m_ImageBombEffect;					//����ͼƬ
	CPngImage						m_ImgSicboEffectDesktop;			//����ͼƬ
	CPngImage						m_ImgSicboEffectInMidair;			//����ͼƬ
	CPngImage						m_ImgSicboEffectInTheAir;			//����ͼƬ
	CPngImage						m_ImgSicboEffectNormal;			    //����ͼƬ
	CPngImage						m_ImgSicboEffectResult;			    //����ͼƬ
	CPngImage						m_ImgSicboEffectBack;			    //����ͼƬ
	CPngImage						m_ImgSicboGiftInTheAir;			    //����ͼƬ
	bool                            m_bShowSicboEffectBack;             // �Ƿ���ʾ��������
	CPngImage						m_PngSicboArea[COUNT_AZIMUTH];		// ÿ����������ƶ�ʱ�ı���ͼ

	CPngImage						m_ImgSicboEPlan;			        //����ͼƬ
	CPngImage						m_ImgSicboPlan;			            //����ͼƬ
	CPngImage						m_ImgSicboNum;			            //����ͼƬ
	CPngImage						m_ImgSicboAnim[SICBO_ANIM_FRAME_COUNT]; //����ͼƬ

	//�߿�
protected:
	CPngImage						m_ImageFrameWin;		//�߿�ͼƬ
	CSkinImage						m_ImageMeBanker;					//�л�ׯ��
	CSkinImage						m_ImageChangeBanker;				//�л�ׯ��
	CSkinImage						m_ImageNoBanker;					//�л�ׯ��

	CSkinImage                      m_ImageSicboDesktop;                 // ����ҡ����
	CSkinImage                      m_ImageSicboInMidair;                // ���ҡ����
	CSkinImage                      m_ImageSicboInTheAir;                // ����ҡ����

	//������Դ
protected:
	CPngImage						m_PngGameEnd;						//�ɼ�ͼƬ
	CSkinImage						m_ImageCardType;					//����ͼƬ
	CSkinImage						m_ImageSicboPoint;					//�Ƶ���ͼƬ
	CMutex                          m_ApplyBankerLock;
	CTransparentCtrl                m_TCPrompt;

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

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	//���ú���
public:
	//������Ϣ
	void SetGameMaxScore(__int64 lMeMaxScore, __int64 lAllMaxScore);

	//�ҵ�λ��
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID;}
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, __int64 lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, __int64 lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(__int64 lMeCurGameScore, __int64 lMeCurGameReturnScore, __int64 lBankerCurGameScore, __int64 lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, __int64 lScoreCount);
	//��������
	void SetAreaLimitScore(__int64 lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(E_SICBO_NUMBER enCards[MAX_COUNT_SICBO], bool bWinner[COUNT_AZIMUTH]);
	//���ó���
	void SetCurrentJetton(__int64 lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(tagClientGameRecord &gameRecord);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(__int64 lMeCurGameScore, __int64 lMeCurGameReturnScore, __int64 lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	//�ı�ҡ���ӷ�ʽ
	void SetSicboType(E_SICBO_TYPE enSicboType);

	void AddApplyBanker(tagApplyUser &applyUser);
	void CancelApplyBanker(tagApplyUser &applyUser);
	void UpdateApplyBanker(tagApplyUser &applyUser);

	//��������
public:	
	// ��ʼ����
	void DispatchCard();
	void DispatchCardSicbo();
	void DispatchCardNormal();

	//��������
	void FinishDispatchCard();
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, int iAreaIndex);
	//�滭��ը
	void DrawBombEffect(CDC *pDC);

	// ҡ���Ӷ���
	void DrawSicboAnim(CDC *pDC);
	void DrawSicboAnimNormal(CDC *pDC);
	void DrawSicboAnimDesktop(CDC *pDC);
	void DrawSicboAnimInMidair(CDC *pDC);
	void DrawSicboAnimInTheAir(CDC *pDC);
	void DrawSicboAnimSicbo(CDC *pDC);

	//�����ע
	__int64 GetMaxPlayerScore(int iArea);

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

	//��������
public:
	//��ǰ����
	inline __int64 GetCurrentJetton() { return m_lCurrentJetton; }	

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, __int64 lNumber, CRect rcRect, INT nFormat=-1);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);

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
	afx_msg void OnJettonButton500000();
	//���밴ť
	afx_msg void OnJettonButton1000000();
	//���밴ť
	afx_msg void OnJettonButton5000000();
	//�ƶ�ׯ��
	afx_msg void OnBtnBankerMoveUp();
	afx_msg void OnBtnBankerMoveDown();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	// ����ҡ����
	afx_msg void OnBtnSicboDesktop();
	// ���ҡ����
	afx_msg void OnBtnSicboInMidair();
	// ����ҡ����
	afx_msg void OnBtnSicboInTheAir();
	// ���в���
	afx_msg void OnBtnOpenBank();
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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////

#endif
