#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			16									//��ʷ����
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
#ifndef __SPECIAL___
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
#else
class CTableFrameSink : public ITableFrameSink, public ITableUserAction,public ITableUserActionEX
#endif
{
	//����ע��
protected:
	LONGLONG						m_lTieScore;							//��ƽ��ע
	LONGLONG						m_lBankerScore;							//��ׯ��ע
	LONGLONG						m_lPlayerScore;							//������ע
	LONGLONG						m_lTieSamePointScore;					//ƽ����ע
	LONGLONG						m_lBankerKingScore;						//ׯ����ע
	LONGLONG						m_lPlayerKingScore;						//������ע
	LONGLONG						m_lPlayerTwoPair;						//�мҶ���
	LONGLONG						m_lBankerTwoPair;						//ׯ�Ҷ���

	//������ע
protected:
	LONGLONG						m_lUserTieScore[GAME_PLAYER];			//��ƽ��ע
	LONGLONG						m_lUserBankerScore[GAME_PLAYER];		//��ׯ��ע
	LONGLONG						m_lUserPlayerScore[GAME_PLAYER];		//������ע
	LONGLONG						m_lUserTieSamePointScore[GAME_PLAYER];	//ƽ����ע
	LONGLONG						m_lUserBankerKingScore[GAME_PLAYER];	//ׯ����ע
	LONGLONG						m_lUserPlayerKingScore[GAME_PLAYER];	//������ע
	LONGLONG						m_lUserPlayerTwoPair[GAME_PLAYER];		//�мҶ���
	LONGLONG						m_lUserBankerTwoPair[GAME_PLAYER];		//ׯ�Ҷ���

	//���Ʊ���
protected:
	LONGLONG						m_lAreaLimitScore;						//��������
	LONGLONG						m_lUserLimitScore;						//��������
	LONGLONG						m_lApplyBankerCondition;				//��������
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�

	//��ҳɼ�
protected:
	LONGLONG						m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	LONGLONG						m_lUserReturnScore[GAME_PLAYER];		//������ע
	LONGLONG						m_lUserRevenue[GAME_PLAYER];			//���˰��

	//�˿���Ϣ
protected:
	BYTE							m_cbCardCount[2];						//�˿���Ŀ
    BYTE							m_cbTableCardArray[2][3];				//�����˿�

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��

	//ׯ����Ϣ
protected:
	CArrayTemplate<WORD>			m_ApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	LONGLONG						m_lBankerWinScore;						//�ۼƳɼ�
	LONGLONG						m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ

	//���Ʊ���
protected:
	BYTE							m_cbWinSideControl;					//������Ӯ
	BYTE							m_cbExcuteTimes;					//ִ�д���
	int								m_nSendCardCount;					//���ʹ���

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	CFile							m_FileRecord;							//��¼���
	DWORD							m_dwRecordCount;						//��¼��Ŀ

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	ITableFrameControl				* m_pITableFrameControl;				//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

	//���Ʊ���
protected:
	bool							m_bRefreshCfg;							//ÿ��ˢ��
	TCHAR							m_szGameRoomName[SERVER_LEN];			//��������
	LONGLONG						m_StorageStart;							//��������ÿ���ӵĿ����ֵ����ȡʧ�ܰ� 0 ����
	LONGLONG						m_StorageDeduct;						//ÿ����Ϸ������۳��Ŀ���������ȡʧ�ܰ� 1.00 ����


	//ׯ������
protected:
	//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_lBankerMAX)��
	//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
	//�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�
	LONGLONG						m_lBankerMAX;							//���ׯ����
	LONGLONG						m_lBankerAdd;							//ׯ��������

	//��ҳ���m_lBankerScoreMAX֮�󣬾�����������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
	LONGLONG						m_lBankerScoreMAX;						//ׯ��Ǯ
	LONGLONG						m_lBankerScoreAdd;						//ׯ��Ǯ��ʱ,��ׯ������

	//���ׯ����
	LONGLONG						m_lPlayerBankerMAX;						//������ׯ����

	//��ׯ
	bool							m_bExchangeBanker;						//����ׯ��

	//ʱ������
protected:
	BYTE							m_cbFreeTime;							//����ʱ��
	BYTE							m_cbBetTime;							//��עʱ��
	BYTE							m_cbEndTime;							//����ʱ��

	//�����˿���
	int								m_nMaxChipRobot;						//�����Ŀ (��ע������)
	int								m_nChipRobotCount;						//����ͳ�� (��ע������)

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
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

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
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

#ifdef __SPECIAL___
	//���в���
public:
	virtual bool __cdecl OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem);////
#endif

	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

	//��������
private:
	//�����˿�
	bool DispatchTableCard();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//�ֻ��ж�
	void TakeTurns();
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//������Ϣ
	void SendPlaceJettonFail(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
	//����Ա����
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);
	//��ȡ����
	void ReadConfigInformation(bool bReadFresh);

	//��ע����
private:
	//�����ע
	LONGLONG GetUserMaxJetton(WORD wChairID);
	//�����ע
	LONGLONG GetMaxPlayerScore(WORD wChairID);
	//�����ע
	LONGLONG GetMaxPlayerKingScore(WORD wChairID);
	//�����ע
	LONGLONG GetMaxPlayerTwoPairScore(WORD wChairID);
	//�����ע
	LONGLONG GetMaxBankerTwoPairScore(WORD wChairID);
	//�����ע
	LONGLONG GetMaxBankerScore(WORD wChairID);
	//�����ע
	LONGLONG GetMaxBankerKingScore(WORD wChairID);
	//�����ע
	LONGLONG GetMaxTieScore(WORD wChairID);
	//�����ע
	LONGLONG GetMaxTieKingScore(WORD wChairID);

	//��Ϸͳ��
private:
	//����÷�
    LONGLONG CalculateScore();
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair);
	//��̽���ж�
	bool ProbeJudge();
	//���ƽ��
	void ControlResult();
	private:
	void AddMoney();
};

//////////////////////////////////////////////////////////////////////////

#endif
