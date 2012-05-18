#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\�����ļ�\UpGradeLogic.h"
#include <deque>
using namespace std;

//////////////////////////////////////////////////////////////////////////
#define TEST

#define		TIME_ID_SEND_CARD		30

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//�������
protected:
	CUpGradeGameLogic				m_Logic;									//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;							//��ܽӿ�
	const tagGameServiceOption		* pGameServiceOption;						//���ò���
//////////////////
	BOOL							m_bIsHaveScore;								//�Ƿ��з�
	BYTE							m_bMaxCardCnt;
	BYTE							m_bMaxCard[PLAYER_CARD_CNT];
////////////////
	BOOL							m_bIsCanLook[GAME_PLAYER];					//�Ƿ������Թ�
	BYTE							m_bFirstCard[PLAYER_CARD_CNT];				//���ȳ�����
	BYTE							m_bFirstCardCnt;							
	BYTE							m_bCurPublicFlower;							//��ǰ���Ļ�ɫ
	BYTE							m_bTempCnt;									//���������
	BYTE							m_bPublicCard[GAME_PLAYER][2];				//��������
	BYTE							m_bPublicCardCnt[GAME_PLAYER];				//���Ƹ���

	WORD							m_wFirstOut;								//���ȳ�����
	BYTE							m_bScoreCardCnt[GAME_PLAYER];				//���Ƹ���
	BYTE							m_bLastEndReason;							//��һ����Ϸ������ԭ��
	BYTE							m_bLastOutCard[PLAYER_CARD_CNT][GAME_PLAYER][PLAYER_CARD_CNT];//��������
	BYTE							m_bLastOutCardCnt[PLAYER_CARD_CNT][GAME_PLAYER];				//����

	BYTE							m_bOutHandCnt;								//���˼�����
	DWORD							m_dwUserID[GAME_PLAYER];					//��¼�տ�ʼʱ���û���ID
////////////////


	BYTE							m_bUserCard[GAME_PLAYER][PLAYER_CARD_CNT];	//������ϵ���
	BYTE							m_bUserCardCnt[GAME_PLAYER];				//�Ƹ���
	
	BYTE							m_bOutCard[GAME_PLAYER][PLAYER_CARD_CNT];	//��������
	BYTE							m_bOutCardCnt[GAME_PLAYER];					//����

	BYTE							m_bScoreCard[GAME_PLAYER][25];	//����

	BYTE							m_bCurPublicCard[2];						//��ǰ����
	BYTE							m_bCurPublicCardCnt;						//����

	
	WORD							m_wBanker;									//ׯ��
	WORD							m_wPublic;									//��ǰ��������ճ������

	WORD							m_wCurOutCard;								//��ǰ������

	int								m_nScoreValue[GAME_PLAYER];					//��ֵ

//////////////////////////////////////////////////
private:
	//�Զ��庯��
	void			InitVariable();											//��ʼ��	
	void			ResendCard();											//���·���
	void			SendCardNoPublic();										//��������
	void			BeginOutCard();
	void			SendScoreCard(bool bIsLastRound);

	WORD			GetNextSeat(WORD wSeat);
	WORD			GetNextBanker(WORD wBanker);
	WORD			GetWiner(WORD wFirstOut);

	//������Ϣ
	bool			SubOutCard(WORD wOutSeat,BYTE bOutCards[],BYTE bOutCardsCnt);
	bool			SubNTCard(ClientNtStruct *pNT,WORD wSeat);
	bool			GameEnd(WORD wSeat,BYTE bEndReason);

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void __cdecl RepositTableFrameSink();

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode();
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
};

//////////////////////////////////////////////////////////////////////////

#endif