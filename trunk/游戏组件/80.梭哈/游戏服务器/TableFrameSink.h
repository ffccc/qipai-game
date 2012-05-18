#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//����ԭ��
#define GER_NO_PLAYER						0x10						//û�����

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
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	BYTE							m_bSendCardCount;						//������Ŀ
	BYTE							m_cbCardCount[GAME_PLAYER];				//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][5];		//�����˿�
	BYTE							m_cbGameEndReason;						//��Ϸ��������

	//�����Ϣ
protected:
	LONG							m_lMaxGold;								//�����ע
	LONG							m_lBasicGold;							//��Ԫ��ע
	LONG							m_lTurnBasicGold;						//������ע
	LONG							m_lTableGold[2*GAME_PLAYER];			//��ע���

	//��������
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬

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

	//��Ϸ�¼�
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//��ע�¼�
	bool OnUserAddGold(WORD wChairID, LONG lGold, bool bCancel);
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