#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////



//�������
#define HORSE_COUNT					8									//�����Ŀ
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	LONGLONG							m_lScore2;							//��ƽ��ע
	LONGLONG							m_lScore3;							//��ׯ��ע
	LONGLONG							m_lScore1;							//������ע
	LONGLONG							m_lScore6;							//ƽ����ע
	LONGLONG							m_lScore5;							//ׯ����ע
	LONGLONG							m_lScore4;							//������ע
	LONGLONG							m_lScore7;							//�мҶ���
	LONGLONG							m_lScore8;							//ׯ�Ҷ���

	//������ע
protected:
	LONGLONG							m_lUserScore2[GAME_PLAYER];			//��ƽ��ע
	LONGLONG							m_lUserScore3[GAME_PLAYER];			//��ׯ��ע
	LONGLONG							m_lUserScore1[GAME_PLAYER];			//������ע
	LONGLONG							m_lUserScore6[GAME_PLAYER];			//ƽ����ע
	LONGLONG							m_lUserScore5[GAME_PLAYER];			//ׯ����ע
	LONGLONG							m_lUserScore4[GAME_PLAYER];			//������ע
	LONGLONG							m_lUserScore7[GAME_PLAYER];			//�мҶ���
	LONGLONG							m_lUserScore8[GAME_PLAYER];			//ׯ�Ҷ���

	//���Ʊ���
protected:
	LONGLONG							m_lAreaLimitScore;						//��������
	LONGLONG							m_lUserLimitScore;						//��������
	LONGLONG							m_lApplyBankerCondition;				//��������
	TCHAR								m_szConfigFileName[MAX_PATH];			//�����ļ�

	//��ҳɼ�
protected:
	LONGLONG							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	LONGLONG							m_lUserReturnScore[GAME_PLAYER];		//������ע
	LONGLONG							m_lUserRevenue[GAME_PLAYER];			//���˰��

	//�˿���Ϣ
protected:
	//BYTE							m_cbCardCount[2];						//�˿���Ŀ
 //   BYTE							m_cbTableCardArray[2][3];				//�����˿�
	BYTE							m_cbHorseSpeed[HORSE_COUNT][HORSE_COUNT];

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
	BYTE							m_cbWinSideControl;						//������Ӯ
	int								m_nSendCardCount;						//���ʹ���

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_S_SCORE_HISTORY];//��Ϸ��¼
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
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return false; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

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
	bool DispatchSpeed();
	//�����˿�
	bool DispatchTableCard();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);

	//��ע����
private:
	//�����ע
	LONGLONG GetUserMaxJetton(WORD wChairID);
	//�����ע
	LONGLONG GetMaxHorse1(WORD wChairID);
	//�����ע
	LONGLONG GetMaxHorse2(WORD wChairID);
	//�����ע
	LONGLONG GetMaxHorse3(WORD wChairID);
	//�����ע
	LONGLONG GetMaxHorse4(WORD wChairID);
	//�����ע
	LONGLONG GetMaxHorse5(WORD wChairID);
	//�����ע
	LONGLONG GetMaxHorse6(WORD wChairID);
	//�����ע
	LONGLONG GetMaxHorse7(WORD wChairID);
	//�����ע
	LONGLONG GetMaxHorse8(WORD wChairID);

	//��Ϸͳ��
private:
	//����÷�
    LONGLONG CalculateScore();
};

//////////////////////////////////////////////////////////////////////////

#endif
