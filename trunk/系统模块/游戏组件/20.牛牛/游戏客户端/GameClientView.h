#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "GoldControl.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_HINT_OX					WM_USER+99							//��ʾ��Ϣ
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_READY					WM_USER+101							//̯����Ϣ
#define IDM_OX						WM_USER+102							//ţţ��Ϣ
#define IDM_MAX_SCORE				WM_USER+103							//����ע
#define IDM_SEND_CARD_FINISH		WM_USER+104							//�������
#define IDM_SHOW_VALUE				WM_USER+105							//������Ϣ
#define IDM_SHOW_INFO				WM_USER+106							//��ʾ��Ϣ
#define IDM_OX_ENABLE				WM_USER+107							//ţţ��ť
#define IDM_SORT					WM_USER+108							//�˿˷���
#define IDM_RESORT					WM_USER+109							//������Ϣ
#define IDM_ADD_SCORE				WM_USER+110							//��ע��Ϣ
#define IDM_SHORTCUT_KEY			WM_USER+111							//��ݼ�
#define IDM_BANKER					WM_USER+112							//��ׯ��Ϣ


#define MY_VIEW_CHAIRID				2									//����λ��

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
	//���ݱ���
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	LONG							m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	BYTE							m_bOxValue[GAME_PLAYER];			//ţţ����
	BYTE							m_bOpenCardUser[GAME_PLAYER];		//̯���û�
	BYTE							m_bCallUser;						//�ȴ���ׯ
	bool							m_bInvest;							//�ȴ���ע
	bool							m_bShowSocre[GAME_PLAYER];			//��ʾ����

	//��������
public:
	bool							m_bShow;							//��ʾ����
	bool							m_bLookOnUser;						//�Թ۱�־
	bool							m_bOpenCard;						//�ȴ�̯��

	//��������
protected:
	CPoint							m_SendCardPos;						//����λ��
	WORD							m_wSendCount;						//�ƶ�����
	WORD							m_wSendIndex;						//�ƶ�����
	CPoint							m_SendEndingPos[GAME_PLAYER];		//����λ��
	CPoint							m_SendStartPos;						//ʼ��λ��
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//��ť�ؼ�
public:
	CSkinButton						m_btHintOx;							//��ʾ��ť
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btReSort;							//���Ű�ť
	CSkinButton						m_btOpenCard;						//̯�ư�ť
	CSkinButton						m_btOx;								//ţţ��ť
	CSkinButton						m_btShortcut;						//��ݼ���ť
	CSkinButton						m_btOneScore;						//һ�ְ�ť
	CSkinButton						m_btTwoScore;						//���ְ�ť
	CSkinButton						m_btThreeScore;						//���ְ�ť
	CSkinButton						m_btFourScore;						//�ķְ�ť
	CSkinButton						m_btBanker;							//ׯ�Ұ�ť
	CSkinButton						m_btIdler;							//�мҰ�ť

	//�ؼ�����
public:
	CScoreView						m_ScoreView;						//�ɼ�����
	CCardControl					m_CardControl[GAME_PLAYER];			//�û��˿�
	CCardControl					m_CardControlOx[GAME_PLAYER];		//�û��˿�

#ifdef VIDEO_GAME
	//�ؼ�����
protected:
	CVideoServiceControl			m_DlgVideoService[GAME_PLAYER];		//��Ƶ����
#endif

	//λ����Ϣ
protected:
	CPoint							m_PointBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptJeton[GAME_PLAYER];				//���λ��
	CPoint							m_ptValue[GAME_PLAYER];				//����λ��

	//�������
protected:
	CSkinImage						m_ImageBanker;						//ׯ����Դ
	CSkinImage						m_ImageCard;						//�˿���Դ
	CSkinImage						m_ImageViewBack;					//������Դ
	CSkinImage						m_ImageViewCenter;					//������Դ
	CSkinImage						m_ImageJeton;						//������Դ
	CSkinImage						m_ImageOxValue;						//ţֵ��Դ
	CSkinImage						m_ImageOxValueZero;					//ţֵ��Դ
	CSkinImage						m_ImageDoulbeOx;					//ţţ��Դ
	CSkinImage						m_ImageWaitCall;					//�ȴ���Դ
	CSkinImage						m_ImageNumber;						//������Դ
	CSkinImage						m_ImageOpenCard;					//̯����Դ
	CSkinImage						m_ImageScore;						//������Դ

	//��ʾ����
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

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
	//�����ж�
	bool IsDispatchCard();
	//��ɷ���
	void FinishDispatchCard();
	//�����˿�
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//������ע
	void SetUserTableScore(WORD wChairID, LONG lTableScore);
	//ׯ�ұ�־
	void SetBankerUser(WORD wBankerUser);
	//�ȴ���ׯ
	void SetWaitCall(BYTE bCallUser);
	//�ȴ���־
	void SetWaitInvest(bool Invest);
	//������Ϣ
	//void SetScoreInfo(LONG lTurnMaxScore,LONG lTurnLessScore);
	//��ʾ����
	void DisplayType(bool bShow);
	//���õ���
	void SetUserOxValue(WORD wChiarID,BYTE bValue);
	//��ȡ����
	BYTE GetValue(WORD wChiarID){return m_bOxValue[wChiarID];}
	//��ʾ̯��
	void ShowOpenCard(WORD wChiarID,BOOL bShow=TRUE);
	//��ʾ����
	void ShowScore(WORD wChiarID,bool bShow);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
