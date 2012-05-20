#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ApplyUserList.h"


//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				7									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_UPDATE_CONTROL			WM_USER+202							//���¿ؼ�

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

#define INDEX_DICEBOX_NULL			0									//û�в���
#define INDEX_OPEN_DICEBOX			1									//������
#define INDEX_CLOSE_DICEBOX			2									//�ر�����
#define INDEX_SHAKE_DICEBOX			3									//ҡ������

//��ʷ��¼
#define MAX_SCORE_HISTORY			256									//��ʷ����

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
	enOperateResult					enOperateFlags;						//������ʶ
	BYTE							cbDiceCount;						//ɫ�ӵ���
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��ע��Ϣ
protected:
	LONG							m_lMeMaxScore;						//�����ע
	LONG							m_lMeOddScore;						//����ע
	LONG							m_lMeEvenScore;						//��˫��ע

	//ȫ����ע
protected:
	LONG							m_lAllOddScore;						//����ע
	LONG							m_lAllEvenScore;					//��˫��ע

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��

	//��ʷ��Ϣ
protected:
	WORD							m_wDrawCount;						//��Ϸ����
	LONG							m_lMeResultCount;					//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbWinnerFlash;					//ʤ�����
	LONG							m_lCurrentJetton;					//��ǰ����
	BYTE							m_cbPreJettonArea;					//֮ǰ����
	bool							m_bShowCloseDicebox;				//��ʾɫ��

	//ׯ����Ϣ
protected:
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	LONG							m_lAreaLimitScore;					//��������
	WORD							m_wCurrentBankerChairID;			//��ǰׯ��
	BYTE							m_cbBankerTime;						//��ׯ����
	LONG							m_lBankerScore;						//ׯ�ҳɼ�
	LONG							m_lBankerTreasure;					//ׯ�ҽ��
	BYTE							m_cbTimeIndex;						//ʱ������

	//���ֳɼ�
public:
	LONG							m_lMeCurGameScore;					//�ҵĳɼ�
	LONG							m_lMeCurGameReturnScore;			//�ҵĳɼ�
	LONG							m_lBankerCurGameScore;				//��������
	bool							m_bShowGameResult;					//��ʾ����

	//���ݱ���
protected:
	CPoint							m_PointJetton[2];					//����λ��
	CJettonInfoArray				m_JettonInfoArray[2];				//��������
	BYTE							m_cbDice[2];						//ɫ������

	//�ؼ�����
public:
	CSkinButton						m_btJetton100;						//���밴ť
	CSkinButton						m_btJetton1000;						//���밴ť
	CSkinButton						m_btJetton10000;					//���밴ť
	CSkinButton						m_btJetton100000;					//���밴ť
	CSkinButton						m_btJetton500;						//���밴ť
	CSkinButton						m_btJetton50000;					//���밴ť
	CSkinButton						m_btJetton500000;					//���밴ť	
	
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�

	//�ؼ�����
public:
	CApplyUser						m_ApplyUser;						//�����б�

	//�������
protected:
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewBack;					//����λͼ
	CSkinImage						m_ImageWinFlags;					//��־λͼ
	CSkinImage						m_ImageJettonView;					//������ͼ
	CSkinImage						m_ImageScoreNumber;					//������ͼ
	CSkinImage						m_ImageMeScoreNumber;				//������ͼ
	CSkinImage						m_ImageTimeFlag;					//ʱ���ʶ

	//�߿���Դ
protected:
	CSkinImage						m_ImageFrameOdd;					//�߿�ͼƬ
	CSkinImage						m_ImageFrameEven;					//�߿�ͼƬ
	CSkinImage						m_ImageMeBanker;					//�л�ׯ��
	CSkinImage						m_ImageChangeBanker;				//�л�ׯ��
	CSkinImage						m_ImageNoBanker;					//�л�ׯ��

	//������Դ
protected:
	CSkinImage						m_ImageGameEnd;						//�ɼ�ͼƬ
	CSkinImage						m_ImageDicebox;						//ɫ��ͼƬ
	CSkinImage						m_ImageDice;						//ɫ��ͼƬ
	CSkinImage						m_ImageCloseDicebox;				//ɫ��ͼƬ
	CSkinImage						m_ImageWinFlag;						//��Ӯ��ʶ
	CSkinImage						m_ImageEndSocre;					//����ͼƬ

	//��������
protected:
	BYTE							m_cbOpenDiceboxIndex;				//������
	BYTE							m_cbCloseDiceboxIndex;				//�ر�����
	BYTE							m_cbShakeDiceboxIndex;				//ҡ������

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
	//������Ϣ
	void SetMeMaxScore(LONG lMeMaxScore);
	//�����ע
	void SetAreaLimitScore(LONG lAreaLimitScore);

	//������Ϣ
	void SetMeOddScore(LONG lMeOddScore);
	//������Ϣ
	void SetMeEvenScore(LONG lMeEvenScore);

	//���ó���
	void SetCurrentJetton(LONG lCurrentJetton);
	//��ʷ�ɼ�
	void SetHistoryScore(WORD wDrawCount,LONG lMeResultCount);
	//��ʷ��¼
	void SetGameHistory(enOperateResult OperateResult, BYTE cbDiceCount);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//ׯ����Ϣ
	void SetBankerInfo( WORD wChairID, BYTE cbBankerTime, LONG lScore );
	//ׯ�ҽ��
	void SetBankerTreasure(LONG lBankerTreasure);

	//�ɼ�����
	void SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore);
	//���ñ�ʶ
	void SetShowGameFlag(bool bShowGameResult);
	//���ñ�ʶ
	void ShowCloseDicebox(bool bShowCloseDicebox){ m_bShowCloseDicebox = bShowCloseDicebox; UpdateGameView(NULL);};
	//���ö���
	void SetCartoon(BYTE cbCartoonIdex, bool bStart);
	//����ɫ��
	void SetDice(BYTE cbDice[2]);

	//״̬�ж�
	bool IsPlaceJettonStatus();
	//����ʱ��
	void SetTimeIndex(BYTE cbTimeIndex){m_cbTimeIndex = cbTimeIndex;UpdateGameView(NULL);}

	//��������
protected:
	//�����ע
	LONG GetMaxOddScore();
	//�����ע
	LONG GetMaxEvenScore();

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount);
	//����ʤ��
	void SetWinnerSide(BYTE cbWinnerSide);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);

	//��������
public:
	//��ǰ����
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }
	//�ҵ�λ��
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID; }

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawEndNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//�����ж�
	bool HitEvenAreaTest(CPoint &Point);
	//�����ж�
	bool HitOddAreaTest(CPoint &Point);
	//�滭����
	void DrawCartoon(CDC *pDC, int nWidth, int nHeight);

	//��ť��Ϣ
protected:
	//���밴ť
	afx_msg void OnJettonButton500();
	//���밴ť
	afx_msg void OnJettonButton50000();
	//���밴ť
	afx_msg void OnJettonButton500000();
	//���밴ť
	afx_msg void OnJettonButton100();
	//���밴ť
	afx_msg void OnJettonButton1000();
	//���밴ť
	afx_msg void OnJettonButton10000();
	//���밴ť
	afx_msg void OnJettonButton100000();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();

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