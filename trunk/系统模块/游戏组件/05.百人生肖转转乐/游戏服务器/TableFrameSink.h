#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			16									//��ʷ����
//////////////////////////////////////////////////////////////////////////
//�����Ϣ
struct tagApplyUserInfo
{
	DWORD							dwUserID;							//���ID
	WORD							wChairID;							//���Ӻ���
	LONGLONG						lUserScore;							//��һ���
};

typedef CArrayTemplate< tagApplyUserInfo > ApplyUserArrary;

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��ע��Ϣ
protected:
	INT64						m_lAreaLimitScore;						//��������

	//��ҳɼ�
protected:
	INT64						m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	INT64						m_lUserReturnScore[GAME_PLAYER];		//������ע

	tagScoreInfo				m_ScoreInfo[GAME_PLAYER];
	BOOL						m_bneedWriteTable;

	//״̬����
protected:
	DWORD						m_dwJettonTime;							//��ʼʱ��

	//ׯ����Ϣ
protected:
	INT64						m_lApplyBankerCondition;				//��������
	ApplyUserArrary				m_ApplyUserArrary;						//�����б�
	tagApplyUserInfo			m_CurrentBanker;						//��ǰׯ��
	BYTE						m_cbBankerTimer;						//��ׯ����
	INT64						m_lBankerWinScore;						//ׯ�ҳɼ�
	bool						m_bCancelBanker;						//�����ׯ
	INT64                       m_lMaxBankerTimer;                      //������
	bool						m_bBankerLeft;
	CString						m_strFileName;

//////////////////////////////////////////////////////////////////////////
	LONGLONG					m_nUserBet[GAME_PLAYER][MAX_AREA];
	LONGLONG					m_nMaxCanBet;
	BYTE						m_cbResult;	//���

//////////////////////////////////////////////////////////////////////////

	//�������
protected:
	ITableFrame					* m_pITableFrame;						//��ܽӿ�
	ITableFrameControl			* m_pITableFrameControl;				//��ܽӿ�
	const tagGameServiceOption	* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD			m_wPlayerCount;							//��Ϸ����
	static const enStartMode	m_GameStartMode;						//��ʼģʽ

//	CSystemLog					m_systemLog;
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { if (IsValid()) delete this; return ; }
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
	//���ж���
	virtual bool __cdecl OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbBankType) { return true; }

	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, BYTE cbAnimal, INT64 lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker( tagServerUserData *pUserData, bool bApplyBanker );

	//��������
private:

	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//��Ϸ��¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//����ׯ��
	void ChangeBanker();
	//�ֻ�ׯ��
	void SendChangeBankerMsg();
};

//////////////////////////////////////////////////////////////////////////

#endif