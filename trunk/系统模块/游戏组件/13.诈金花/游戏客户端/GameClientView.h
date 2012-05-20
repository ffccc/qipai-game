#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "GoldControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��Ϣ����
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_ADD_SCORE				WM_USER+101							//��ע��Ϣ
#define	IDM_CONFIRM					WM_USER+109							//ȷ����Ϣ	
#define	IDM_CANCEL_ADD				WM_USER+119							//ȡ����Ϣ	
#define IDM_MIN_SCORE				WM_USER+102							//���ټ�ע
#define IDM_MAX_SCORE				WM_USER+103							//����ע
#define IDM_LOOK_CARD				WM_USER+104							//������Ϣ
#define IDM_COMPARE_CARD			WM_USER+105							//������Ϣ
#define IDM_OPEN_CARD				WM_USER+106							//������Ϣ
#define IDM_GIVE_UP					WM_USER+107							//������Ϣ
#define IDM_SEND_CARD_FINISH		WM_USER+108							//�������
#define IDM_COMPARE_USER			WM_USER+110							//��ѡ�û�
#define IDM_FALSH_CARD_FINISH		WM_USER+114							//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//������Ϣ
struct tagGoldMes
{
	LONG							lGoldType;							//��������
	LONG							lDrawCount;							//�滭����
};

//����˵��
typedef CArrayTemplate<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//���ݱ���
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	LONG							m_lMaxCellScore;					//�����ע
	LONG							m_lCellScore;						//������ע
	LONG							m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	LONG							m_lCurrentScore[GAME_PLAYER];		//��ǰ��Ŀ
	LONG							m_lStopUpdataScore[GAME_PLAYER];	//������Ŀ
	bool							m_bInvest;							//�ȴ���ע
	bool							m_bCompareCard;						//ѡ�ȱ�־
	WORD							m_wWaitUserChoice;					//�ȴ���־

	//��������
public:
	bool							m_bShow;							//��ʾ����
	TCHAR							m_tcBuffer[GAME_PLAYER][64];		//�Ƶ�����

	//���Ʊ���
protected:
	WORD							m_wConmareIndex;					//��������
	WORD							m_wConmareCount;					//������Ŀ
	WORD							m_wCompareChairID[2];				//�����û�

	//���Ʊ���
protected:
	bool							m_bFalsh;							//���Ʋ���
	WORD							m_wFlashUser[GAME_PLAYER];			//�����û�
	WORD							m_wFalshCount;						//���ƴ���

	//������־
protected:
	bool							m_bCartoon;							//������־
	WORD							m_wLoserUser;						//���ưܻ�
	bool							m_bStopDraw;						//���Ʊ�־
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//λ�ñ���
protected:
	CPoint							m_ptUserCompare;					//����λ��
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_ptUserCard[GAME_PLAYER];			//����λ��
	CPoint							m_KeepPos;							//����λ��

	//�û�״̬
protected:
	BOOL							m_bCompareUser[GAME_PLAYER];		//��ѡ�����û�

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btMaxScore;						//���ť
	CSkinButton						m_btMinScore;						//����ť
	CSkinButton						m_btCancel;							//����ť
	CSkinButton						m_btConfirm;						//ȡ����ť
	CSkinButton						m_btAddScore;						//��ע��ť
	CSkinButton						m_btFollow;							//��ע��ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btLookCard;						//���ư�ť
	CSkinButton						m_btCompareCard;					//���ư�ť
	CSkinButton						m_btOpenCard;						//���ư�ť

	//�ؼ�����
public:
	CScoreView						m_ScoreView;						//�ɼ�����
	CGoldControl					m_GoldControl;						//��ע��ͼ
	CCardControl					m_CardControl[GAME_PLAYER];			//�û��˿�

	//λ����Ϣ
protected:
	CPoint							m_PointBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptJeton[GAME_PLAYER];				//���λ��
	CArrayTemplate<tagGoldMes>		m_lDrawGold[GAME_PLAYER];			//�滭��Ŀ
	CArrayTemplate<CPoint>			m_ptKeepJeton[GAME_PLAYER];			//���λ��

	//�������
protected:
	CSkinImage						m_ImageBanker;						//ׯ����Դ
	CSkinImage						m_ImageCard;						//�˿���Դ
	CSkinImage						m_ImageTitle;						//������Դ
	CSkinImage						m_ImageViewBack;					//������Դ
	CSkinImage						m_ImageViewCenter;					//������Դ
	CSkinImage						m_ImageJeton;						//������Դ
	CSkinImage						m_ImageArrowhead;					//��ͷ��Դ
	CSkinImage						m_ImageReady;						//׼����Դ
	CSkinImage						m_ImagePocket;						//�����Դ
	CSkinImage						m_ImageNumber;						//������Դ
	CSkinImage						m_ImageBalance;						//������Դ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

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
	//������ע
	void SetUserTableScore(WORD wChairID, LONG lTableScore,LONG lCurrentScore);
	//�ȴ�ѡ��
	void SetWaitUserChoice(WORD wChoice);
	//ׯ�ұ�־
	void SetBankerUser(WORD wBankerUse);
	//������Ϣ
	void SetScoreInfo(LONG lMaxCellScore,LONG lCellScore);
	//ѡ�ȱ�־
	void SetCompareCard(bool bCompareCard,BOOL bCompareUser[]);
	//ֹͣ����
	void StopUpdataScore(bool bStop);
	//�滭�˿�
	void DrawCompareCard(CDC * pDC, INT nXPos, INT nYPos, bool bChapped);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//�����ж�
	bool IsDispatchCard(){return m_bCartoon;}
	//��ȡ��Ϣ
	bool GetCompareInfo(){return m_bCompareCard;};

	//��������
public:
	//���ƶ���
	void PerformCompareCard(WORD wCompareUser[2], WORD wLoserUser);
	//���ƶ���
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//���ƶ���
	bool bFalshCard(WORD wFalshUser[]);
	//���ƴ���
	bool SendCard();
	//���ƴ���
	bool FlashCard();
	//���ƴ���
	bool CompareCard();

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
