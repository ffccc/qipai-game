#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�û���Ϣ
struct tagScoreHistory
{
	__int64							lScore;									//�仯����
	__int64							lWinScore;								//ʤ������
	__int64							lLoseScore;								//�������
	SYSTEMTIME						RecordTime;								//��¼ʱ��
};

//��������
typedef CMap<DWORD,DWORD,DWORD,DWORD> CBlacklistIDMap;
typedef CMap<DWORD,DWORD,tagScoreHistory *,tagScoreHistory * &>	CScoreHistoryMap;

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	bool							m_bShowHand;							//�����־
	bool							m_bChaosCard;							//���ұ�־
	WORD							m_wOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wLastMostUser;						//�ϴη�������û�

	//�û�״̬
protected:
	LONG							m_lServiceScore;						//�������
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE							m_cbOperaScore[GAME_PLAYER];			//������־
	LONG							m_lTableScore[GAME_PLAYER*2];			//��ע��Ŀ
	LONG							m_lUserMaxScore[GAME_PLAYER];			//�����ע

	//�˿˱���
protected:
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbCardCount[GAME_PLAYER];				//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][5];		//�����˿�

	//��ע��Ϣ
protected:
	LONG							m_lMaxScore;							//�����ע
	LONG							m_lCellScore;							//��Ԫ��ע
	LONG							m_lTurnMaxScore;						//�����ע
	LONG							m_lTurnLessScore;						//��С��ע

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

	//��ʷ��¼
protected:
	static __int64					g_lMaxScore;							//���仯
	static __int64					g_lMaxWinScore;							//���Ӯ��
	static __int64					g_lMaxLoseScore;						//������
	static CBlacklistIDMap			g_BlacklistIDMap;						//�����û�
	static CScoreHistoryMap			g_ScoreHistoryMap;						//��ʷ��¼

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

	//�����¼�
public:
	//����ͬ��
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�������
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
	//��������
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }

	//�����¼�
public:
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser){ return true; }
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser){ return true; };
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//��Ϸ�¼�
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, LONG lScore, bool bGiveUp);
	//��ȡʤ��
	bool OnUserGetWinner(IServerUserItem * pIServerUserItem);

	//��Ϸ����
protected:
	//�ƶ�����
	WORD EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos);
	//�ƶ�ʤ��
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
};

//////////////////////////////////////////////////////////////////////////

#endif