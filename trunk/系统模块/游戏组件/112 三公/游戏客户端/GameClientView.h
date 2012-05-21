#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_SCORE					(WM_USER+101)						//��ע��Ϣ
#define IDM_CLEAN					(WM_USER+102)						//��ע��Ϣ
#define IDM_BANKER					(WM_USER+103)						//��ׯ��Ϣ
#define IDM_CHIP_IN					(WM_USER+104)						//��ע��Ϣ
#define IDM_SEND_CARD_FINISH		(WM_USER+105)						//�������
#define IDM_RECLAIM_CARD_FINISH		(WM_USER+106)						//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CArrayTemplate<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//�ȴ�����
protected:
	BYTE							m_bDrawTimes;						//�滭����
	WORD							m_wUserPost[GAME_PLAYER];			//����λ��

	//�������
public:
	WORD							m_wMeChairID;						//�ҵ�λ��
	WORD							m_wBankerUser;						//ׯ��λ��
	WORD							m_wChairIndex[GAME_PLAYER];			//λ������

	//��ʷ��¼
protected:
	WORD							m_wDrawCount;						//��Ϸ����
	LONG							m_lGameScore;						//��Ϸ�ɼ�

	//״̬����
protected:
	bool							m_bShowType;						//��ʾ����
	bool							m_bWaitBanker;						//�ȴ�ׯ��
	bool							m_bWaitPlayer;						//�ȴ��м�
	TCHAR							m_szCardType[GAME_PLAYER][16];		//������Ϣ

	//��ע��Ϣ
protected:
	LONG							m_lMaxScore;						//�����ע
	LONG							m_lLessScore;						//������ע

	//���Ʊ���
protected:
	WORD							m_wSendCount;						//���ƴ���
	WORD							m_wSendIndex;						//��������
	CPoint							m_SendCardPos;						//����λ��
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//���Ʊ���
protected:
	bool							m_bReclaimIng;						//���ձ�־
	WORD							m_wReclaimCount;					//���ƴ���
	WORD							m_wReclaimIndex;					//��������
	CPoint							m_ReclaimCardPos;					//��ǰλ��
	CPoint							m_ReclaimStartPos;					//����λ��
	CPoint							m_ReclaimEndingPos;					//����λ��

	//λ�ñ���
protected:
	CPoint							m_SendStartPos;						//��ʼλ��
	CPoint							m_SendEndingPos[GAME_PLAYER];		//����λ��

	//λ�ñ���
public:
	bool							m_bLongBanker;						//��ׯ��־
	bool							m_bTakeScore;						//���ձ�־
	WORD							m_wScoreIndex;
	bool							m_bBankerScore[GAME_PLAYER];		//ׯ�ұ�־
	CPoint							m_GoldTarget[GAME_PLAYER];			//����λ��
	CPoint							m_GoldStation[GAME_PLAYER];			//����λ��

	//���밴ť
public:
	CSkinButton						m_btScore1;							//���밴ť
	CSkinButton						m_btScore2;							//���밴ť
	CSkinButton						m_btScore3;							//���밴ť
	CSkinButton						m_btScore4;							//���밴ť
	CSkinButton						m_btScore5;							//���밴ť

	//���ܰ�ť
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btClean;							//�����ť
	CSkinButton						m_btChipIn;							//��ע��ť
	CSkinButton						m_btBanker;							//��ׯ��ť
	CSkinButton						m_btNoBanker;						//������ť
	CSkinButton						m_btControlMax;

	//�û��ؼ�
public:
	CGoldView						m_GoldView[GAME_PLAYER];			//����ؼ�
	CCardControl					m_CardControl[GAME_PLAYER];			//�˿˿ؼ�

	//�ؼ�����
public:
	CScoreView						m_ScoreView;						//�ɼ���ͼ

	//��Դ����
protected:
	CSkinImage						m_ImageCard;						//�˿���Դ
	CSkinImage						m_ImageCards;						//�˿���Դ
	CSkinImage						m_ImageBanker;						//ׯ�ұ�־
	CSkinImage						m_ImageLongBanker;					//��ׯ��־
	CSkinImage						m_ImageDrawInfo;					//��Ϸ��Ϣ
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewBanker;					//����λͼ
	CSkinImage						m_ImageViewPlayer;					//����λͼ
	CSkinImage						m_ImageWaitBanker;					//�ȴ�ׯ��
	CSkinImage						m_ImageWaitPlayer;					//�ȴ����
	CSkinImage						m_ImageWaitEnter;					//�ȴ�����
	CSkinImage						m_ImageWaitContinue;				//�ȴ�����
	CSkinImage						m_ImageWait;						//�ȴ���Դ
	CSkinImage						m_ImageUserFlag;					//������Դ

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

	//��Ϣ����
public:
	//��ȡ����
	LONG GetUserClipInInfo(WORD wChairID);

	//���ܺ���
public:
	//��ʾ����
	void SetCardType(bool bShowType);
	//�ȴ�ׯ��
	void SetWaitBanker(bool bWaitBanker);
	//�ȴ��м�
	void SetWaitPlayer(bool bWaitPlayer);
	//��ע��Ϣ
	void SetChipInInfo(LONG lMaxScore, LONG lLessScore);
	//������Ϣ
	void SetUserDrawInfo(WORD wDrawCount, LONG lGameScore);
	//��������
	void SetCardTypeInfo(WORD wChairID, LPCTSTR pszCardType);
	//����λ��
	void SetUserChairInfo(WORD wBankerUser, WORD wMeChairID);
	//���ó���
	void SetUserClipInInfo(WORD wChairID, LONG lChipInCount);
	//���ó�ׯ
	void SetLongBanker(bool bLongBanker);
	//����λ��
	void SetUserPost(WORD wPhPost,WORD wViewPost);
	//��ȡλ��
	WORD GetUserPost(WORD wViewPost){return m_wUserPost[wViewPost];}
	//��ȡλ��
	WORD GetBankerViewID(){return m_wChairIndex[m_wBankerUser];}
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//���ƺ���
public:
	//�����ж�
	bool IsDispatchCard();
	//�����˿�
	void ReclaimUserCard();
	//�������
	void ReclaimUserScore();
	//��ɷ���
	void FinishDispatchCard();
	//�����˿�
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//���밴ť
	afx_msg void OnScore1();
	//���밴ť
	afx_msg void OnScore2();
	//���밴ť
	afx_msg void OnScore3();
	//���밴ť
	afx_msg void OnScore4();
	//���밴ť
	afx_msg void OnScore5();
	//�����ť
	afx_msg void OnClean();
	//��ע��ť
	afx_msg void OnChipin();
	//��ׯ��ť
	afx_msg void OnBanker();
	//������ť
	afx_msg void OnNoBanker();
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void ControlMax();

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
