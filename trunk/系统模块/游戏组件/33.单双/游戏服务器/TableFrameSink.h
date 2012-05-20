#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			16									//��ʷ����
//////////////////////////////////////////////////////////////////////////
//�����Ϣ
struct tagApplyUserInfo
{
	DWORD							dwUserID;							//���ID
	WORD							wChairID;							//���Ӻ���
	LONG							lUserScore;							//��һ���
};

typedef CArrayTemplate< tagApplyUserInfo > ApplyUserArrary;

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��ע��Ϣ
protected:
	LONG							m_lOddScore;						//����ע
	LONG							m_lEvenScore;						//��˫��ע
	LONG							m_lAreaLimitScore;					//��������

	//��ע��Ϣ
protected:
	LONG							m_lUserOddScore[GAME_PLAYER];		//����ע
	LONG							m_lUserEvenScore[GAME_PLAYER];		//��˫��ע

	//��ҳɼ�
protected:
	LONG							m_lUserWinScore[GAME_PLAYER];		//��ҳɼ�
	LONG							m_lUserReturnScore[GAME_PLAYER];	//������ע
	LONG							m_lUserRevenue[GAME_PLAYER];		//���˰��

	//�˿���Ϣ
protected:
    BYTE							m_cbDice[2];						//����ɫ��

	//״̬����
protected:
	DWORD							m_dwJettonTime;						//��ʼʱ��

	//ׯ����Ϣ
protected:
	LONG							m_lApplyBankerCondition;			//��������
	ApplyUserArrary					m_ApplyUserArrary;					//�����б�
	tagApplyUserInfo				m_CurrentBanker;					//��ǰׯ��
	BYTE							m_cbBankerTimer;					//��ׯ����
	LONG							m_lBankerWinScore;					//ׯ�ҳɼ�

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//�������
protected:
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	ITableFrameControl				* m_pITableFrameControl;			//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����
	static const enStartMode		m_GameStartMode;					//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { if (IsValid()) delete this; return; }
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
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker( tagServerUserData *pUserData, bool bApplyBanker, bool bUserAction=true);

	//��������
private:
	//�����˿�
	bool RandDice();
	//ӯ���ж�
	LONGLONG AccountPayoffScore();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	void ChangeBanker();
	//�ֻ�ׯ��
	void SendChangeBankerMsg();

	//�����ע
	LONG GetMaxOddScore(WORD wChairID);	
	//�����ע
	LONG GetMaxEvenScore(WORD wChairID);

	//����÷�
    void CalculateScore();
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE &cbWinner);
};

//////////////////////////////////////////////////////////////////////////

#endif