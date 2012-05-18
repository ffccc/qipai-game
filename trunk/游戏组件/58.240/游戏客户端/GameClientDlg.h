#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "..\�����ļ�\UpGradeLogic.h"


//#define		TEST_
//AI��Ӧʱ��

//���ʱ����
#define		MAX_OVER_TIME_CNT			3

#define		TIME_ID_WAIT_ADHIBIT		101
#define		TIME_ID_OUT_CARD			102
#define		TIME_ID_WAIT_START			103


#define		TIME_ID_WASH_DESK			1001
#define		TIME_CNT_WASH_DESK			2000
#define		TIME_ID_LAST_AROUND			1002
#define		TIME_CNT_LAST_AROUND		4000

#define		TIME_ID_LOOK_PUBLIC			1003
#define		TIME_CNT_LOOK_PUBLIC		2000

#define		TIME_ID_LOOK_BACK			1004
#define		TIME_CNT_LOOK_BACK			4000

//����Ƿ�ֻ��һ�����û׼����
#define		TIME_ID_CHECK_READY			1005
#define		TIME_CNT_CHECK_READY		1000

#define		TIME_ID_SEND_CARD			1006


//˦�ƶ�ʱ��
#define		TIME_ID_SHOW_CARD			1007
#define		TIME_CNT_SHOW_CARD			1000


#ifdef _DEBUG
#define		START_AI_CNT				1
#define		TIME_CNT_WAIT_GIVE			20
#define		TIME_CNT_WAIT_ADHIBIT		20
#define		TIME_CNT_OUT_CARD			55
#define		TIME_CNT_WAIT_START			55
#define		TIME_CNT_SEND_CARD			300

#else
#define		START_AI_CNT				1
#define		TIME_CNT_SEND_CARD			500
#define		TIME_CNT_WAIT_GIVE			60
#define		TIME_CNT_WAIT_ADHIBIT		10
#define		TIME_CNT_OUT_CARD			20
#define		TIME_CNT_WAIT_START			30
#endif


//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�ؼ�����
protected:
	CGameClientView					m_GameClientView;								//��Ϸ��ͼ
	CUpGradeGameLogic				m_Logic;


/////////////////////////
	BOOL							m_bIsHaveScore;									//�Ƿ����з�
	BYTE							m_bMaxCardCnt;									//���������
	BYTE							m_bMaxCard[PLAYER_CARD_CNT];					//
//////////////
	BOOL							m_bIsLookLast;									//�Ƿ��ڲ鿴��һ��
	WORD							m_wThrowSeat;									//˦����
	BOOL							m_bIsCanLook;									//�Ƿ�����


	WORD							m_wNoReadyer;									//δ׼�������
	BYTE							m_bOverTimeCnt;									//��ʱ����
	BYTE							m_bTimeCnt;										//AI�����ü���
	BOOL							m_bIsAI;										//�Ƿ�AI
	BYTE							m_bFirstOutCnt;									//���ȳ��Ƹ���
	BYTE							m_bFirstOutCards[PLAYER_CARD_CNT];				//���ȳ�����

	BYTE							m_bAllMeCardCnt;
	BYTE							m_bAllMeCard[PLAYER_CARD_CNT*2];				//���е���
	BYTE							m_bCurSendCardPos;								//��ǰ����λ��


	BYTE							m_bMeCardCnt;									//��ǰ�ҵ���	
	BYTE							m_bMeCard[PLAYER_CARD_CNT+BACK_CARD_CNT];						//��ֵ
	BYTE							m_bOutCardCnt[GAME_PLAYER];						//���Ƹ���(Ϊ��һ�ֹ���׼��)
	BYTE							m_bOutCard[GAME_PLAYER][PLAYER_CARD_CNT];		//��ֵ
	//��һ�ֳ�����
	BYTE							m_bLastOutCardCnt[PLAYER_CARD_CNT][GAME_PLAYER];	//���Ƹ���
	BYTE							m_bLastOutCard[PLAYER_CARD_CNT][GAME_PLAYER][PLAYER_CARD_CNT];	//��ֵ
	BYTE							m_bCurRound;									//��ǰ����(-1)

	BYTE							m_bCurLookRound;								//��ǰ�鿴������

	WORD							m_wCurSeat;										//��ǰ������
	BYTE							m_bPublicCardCnt;								//���Ƹ���
	BYTE							m_bPublicCard[2];								//����(���ֻ��2��)

private:
	void							InitVariable();									//��ʼ������
	void							UpdateNTList();
	void							KillAllTimer();	
	void							AutoOut();										//�Զ�����
	void							SetOutCardButton();								//���ó��ư�ť
	void							RevertDeskCard();								//��ԭ�˿�(�鿴��һ��ʱ����)

	void							SendOutCard(BYTE bCards[],BYTE bCardsCnt);

	void							SetMeUpCard(BYTE bUpCards[],BYTE bUpCardsCnt);

	void							SetSpaceCard();
	void							HideNTList();

	//���纯��
	bool							SubBeginSend(ServerBeginSend *pCard);			//��ʼ����
	bool							SubNTCard(ServerNtStruct *pCards);				//����
	bool							SubBeginOut(ServerBeginOut *pSeat);				//��ʼ����

	bool							SubOutCard(ServerOutCard *p);					//������Ϣ
	bool							SubBatchOut(ServerBatchOut *p);					//������Ϣ
	bool							SubGamePoint(ServerGamePoint *pCard);			//�Ʒ���
	bool							SubGameEnd(ServerGameEnd *pEnd);
	
	
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	//��ʼ����
	virtual bool InitGameFrame();
	//���ÿ��
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
public:
	//��ԭ�˿�
	LRESULT	OnRevertCard(WPARAM wparam, LPARAM lparam);
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	//NT ��ť����.
	LRESULT	OnHitPublicCard(WPARAM wparam, LPARAM lparam);
	//�����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);
	//��ʾ���ư�ť
	LRESULT	OnHint(WPARAM wparam, LPARAM lparam);
	//�������ư�ť����
	LRESULT	OnHitOutCard(WPARAM wparam, LPARAM lparam);
	//�������ְ�ť����
	LRESULT	OnHitLastTurn(WPARAM wparam, LPARAM lparam);
	//���������ư�ť����
	LRESULT	OnHitLookBack(WPARAM wparam, LPARAM lparam);
	//�Ҽ�����
	LRESULT	OnRightHitCard(WPARAM wparam, LPARAM lparam);
	//��ǰ������ť
	LRESULT	OnAI(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////
