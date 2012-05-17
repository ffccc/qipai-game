#pragma once

#include "Stdafx.h"
#include "GameScoreWnd.h"
#include "ScoreView.h"
#include "ApplyUserList.h"
#include "SkinButtonEx.h"
//////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_SCORE_L					(WM_USER+101)						//������Ϣ
#define IDM_SCORE_R					(WM_USER+102)						//������Ϣ
#define IDM_BANKER_T				(WM_USER+103)						//������Ϣ
#define IDM_BANKER_B				(WM_USER+104)						//������Ϣ
#define IDM_PLACE_JETTON			(WM_USER+105)						//��ס��Ϣ
#define IDM_APPLY_BANKER			(WM_USER+106)						//������Ϣ
#define	IDM_BANK					(WM_USER+202)						//������Ϣ
//////////////////////////////////////////////////////////////////////////
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
//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;
	//������Ϣ
protected:
	LONG							m_lMeMaxScore;						//�����ע
	LONG							m_lAreaLimitScore;					//��������
	bool							m_bIsMove;							//���ֶ���
	BYTE							m_cbMoveIndex;						//�ƶ�����
	BYTE							m_cbGameState;						//��Ϸ״̬
	BYTE							m_cbGameTimer;						//��Ϸʱ��
	LONG							m_lMeCanUseScore;					//���ý��
	BYTE							m_cbSlectPos;						//ѡ�з���
	//��Ϣ����
protected:
	WORD							m_wMeChairID;						//�Լ�����λ��	
	LONG							m_lCurrentJetton;					//��ǰ����
	BYTE							m_cbJettonIndex;					//������������
	BYTE							m_cbSelectIndex;					//ѡ������
	//������ע��Ϣ
protected:
	LONG							m_lMeBigSharkScore;					//�Լ�����������
	LONG							m_lMeFeiQinScore  ;					//����
	LONG							m_lMeZouShouScore ;					//����
	LONG							m_lMeYanZiScore;					//�Լ�����������
	LONG							m_lMeTuZiScore;						//����
	LONG							m_lMeGeZiScore;						//����
	LONG							m_lMeXiongMaoScore;					//��è
	LONG							m_lMeKongQueScore;					//��ȸ
	LONG							m_lMeHouZiScore;					//����
	LONG							m_lMeLaoYingScore;					//��ӥ
	LONG							m_lMeShiZiScore;					//ʨ��

	//ȫ����ע
protected:
	LONG							m_lAllBigSharkScore;
	LONG							m_lAllFeiQinScore;
	LONG							m_lAllZouShouScore;
	LONG							m_lALLYanZiScore;					//��������
	LONG							m_lALLTuZiScore;					//����
	LONG							m_lALLGeZiScore;					//����
	LONG							m_lALLXiongMaoScore;				//��è
	LONG							m_lALLKongQueScore;					//��ȸ
	LONG							m_lALLHouZiScore;					//����
	LONG							m_lALLLaoYingScore;					//��ӥ
	LONG							m_lALLShiZiScore;					//ʨ��
	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	LONG							m_lBankerScore;						//ׯ�һ���
	LONG							m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	BOOL							m_bIsShowJetton;					//�Ƿ���ʾ����
	//��ʷ��Ϣ
protected:
	LONG							m_lMeStatisticScore;				//��Ϸ�ɼ�
	CArrayTemplate<BYTE>			m_cbScoreHistroy;					//��ʷ�ɼ�
	BYTE							m_cbScoreHistroyIndex;				//��ʷ�ɼ�����
	//���ݱ���
protected:
	CPoint							m_PointJetton[11];					//����λ��
	CPoint							m_PointJettonNumber[11];				//����λ��
	CJettonInfoArray				m_JettonInfoArray[11];				//��������
	//�������
protected:
	CSkinImage						m_ImageViewBack;					//����λͼ
	CPngImage						m_ImageJettonView;					//������ͼ
	CPngImage						m_ImageScoreNumber;					//������ͼ
	CPngImage						m_ImageMeScoreNumber;				//������ͼ
	CPngImage						m_ImageSelect;						//ѡ��״̬
	CPngImage						m_ImageGameState;					//��Ϸ״̬
	CPngImage						m_ImageGameTimer;					//��Ϸʱ��
	CPngImage						m_ImageScoreHistory;				//��ʷ�ɼ�
	CPngImage						m_ImageStar;						//����Ч��
	CPngImage						m_ImageJettonBK;					//���뱳��

	//����ͼƬ
	CPngImage						m_ImageYanZi;						//����
	CPngImage						m_ImageYanZi1;
	CPngImage						m_ImageTuZi;						//����
	CPngImage						m_ImageTuZi1;
	CPngImage						m_ImageGeZi;						//����
	CPngImage						m_ImageGeZi1;
	CPngImage						m_ImageXiongMao;					//��è
	CPngImage						m_ImageXiongMao1;
	CPngImage						m_ImageKongQue;						//��ȸ
	CPngImage						m_ImageKongQue1;
	CPngImage						m_ImageHouZi;						//����
	CPngImage						m_ImageHouZi1;
	CPngImage						m_ImageLaoYing;						//��ӥ
	CPngImage						m_ImageLaoYing1;
	CPngImage						m_ImageShiZi;						//ʨ��
	CPngImage						m_ImageShiZi1;
	CPngImage						m_ImageDaBaiSha;					//�����
	CPngImage						m_ImageDaBaiSha1;
	CPngImage						m_ImageFeiQin;						//����
	CPngImage						m_ImageZouShou;						//����

	//λ�ñ���
	CPoint							m_ptUserInfo;						//�û���Ϣλ��
	CRect							m_rcBigShark;						//�����
	CRect							m_rcFeiQin;							//����
	CRect							m_rcZouShou;						//����
	CRect							m_rcYanZi;							//����
	CRect							m_rcTuZi;							//����
	CRect							m_rcGeZi;							//����
	CRect							m_rcXiongMao;						//��è
	CRect							m_rcKongQue;						//��ȸ
	CRect							m_rcHouZi;							//����
	CRect							m_rcLaoYing;						//��ӥ
	CRect							m_rcShiZi;							//ʨ��
	CPoint							m_ptBaseTop;						//�ϴ󱼵�
	CPoint							m_ptBaseBottom;						//�´󱼵�
	CPoint							m_ptBaseLeft;						//��󱼵�
	CPoint							m_ptBaseRight;						//�Ҵ󱾵�
	CPoint							m_ptGameState;						//��Ϸ״̬
	CPoint							m_ptGameTimer;						//ʱ��
	CPoint							m_ptScoreHistory;					//��ʷ�ɼ�
	//��ť
	//CSkinButton						m_btBanker_T;						//������ׯ��ť��
	//CSkinButton						m_btBanker_B;						//������ׯ��ť��
	CSkinButton						m_btScore_L;						//������ׯ��ť��
	CSkinButton						m_btScore_R;						//������ׯ��ť��
	CSkinButtonEx					m_btJetton100;						//���밴ť
	CSkinButtonEx					m_btJetton1000;						//���밴ť
	CSkinButtonEx					m_btJetton10000;					//���밴ť
	CSkinButtonEx					m_btJetton100000;					//���밴ť
	CSkinButtonEx					m_btJetton1000000;					//���밴ť
	CSkinButtonEx					m_btJetton10000000;					//���밴ť
	CSkinButtonEx					m_btJetton;							//�������


	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	//CApplyUser						m_ApplyUser;						//�����б�
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
	//�����Լ�λ��
	void SetMeChairID(WORD wMeChair);
	//������Ϣ
	void SetMeMaxScore(LONG lMeMaxScore);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, LONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//��������
	void SetAreaLimitScore(LONG lAreaLimitScore);	
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONG lScoreCount);
	//�������
	void CleanUserJetton();
	//�滭����
	void DrawNumberString(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false, bool bTimer=false);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount);
	//����ѡ������
	void SetSelectIndex(BYTE cbSelectIndex);
	//��ȡѡ����������ʼ��
	void GetSelectPt(CPoint &ptSelect, BYTE cbMoveIndex=0xff);
	//���ֶ���
	void SetCanMove(bool bMove);
	//���ö�������
	void SetMoveIndex(BYTE cbMoveIndex);
	//���ó���
	void SetCurrentJetton(LONG lCurrentJetton);
	//������Ϸ״̬
	void SetGameState(BYTE cbState);
	//�滭ʱ��
	void SetGameTimerNum(BYTE cbTimerNum);
	//���ÿ��ý��
	void SetMeCanUseSocre(LONG lCanUseScore);
	//���óɼ�
	void SetMeScore(LONG lMeScore);
	//����ѡ����������
	void SetSelectJettonIndex(BYTE cbSelectIndex);
	//���ø��˳ɼ�
	void SetMeHisScore(LONG lHisScore);
	//������ʷ��Ϣ
	void SetScoreHistory(BYTE cbScoreHistory[MAX_SCORE_HISTORY]);
	//�����ע
	LONG GetMaxScore(BYTE cbArea);
	//��ȡ�Ƕ�
	double GetAngle(BYTE cbMoveIndex);
	//�������
	POINT GetErrorPoint(BYTE cbMoveIndex);

	//��������
public:
	//��ǰ����
	inline LONG GetCurrentJetton() { return m_lCurrentJetton; }	
	//��ť��Ϣ
protected:
	//������ť
	afx_msg void OnScoreLeft(); 
	//������ť
	afx_msg void OnScoreRight();
	//������ť
	afx_msg void OnBankerTop();
	//������ť
	afx_msg void OnBankerBottom();
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
	afx_msg void OnJettonButton10000000();
	//���밴ť
	afx_msg void OnJettonButton();
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//��ȡ��ť
	afx_msg void OnSaveBank();
	//ȡǮ��ť
	afx_msg void OnGetBank();

	//��Ϣӳ��
protected:
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
