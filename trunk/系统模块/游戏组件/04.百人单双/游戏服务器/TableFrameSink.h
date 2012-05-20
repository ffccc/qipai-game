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
	RETYPELONG						lUserScore;							//��һ���
};

typedef CArrayTemplate< tagApplyUserInfo > ApplyUserArrary;

//��Ϸ������
#ifndef __SPECIAL___
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
#else
class CTableFrameSink : public ITableFrameSink, public ITableUserAction,public ITableUserActionEX
#endif
{
	//��ע��Ϣ
protected:
	RETYPELONG							m_lOddScore;						//����ע
	RETYPELONG							m_lEvenScore;						//��˫��ע
	RETYPELONG							m_lAreaLimitScore;					//��������
	RETYPELONG							m_lUserLimitScore;

	//��ע��Ϣ
protected:
	RETYPELONG							m_lUserOddScore[GAME_PLAYER];		//����ע
	RETYPELONG							m_lUserEvenScore[GAME_PLAYER];		//��˫��ע
	RETYPELONG							m_UserLeftScore;

	//��ҳɼ�
protected:
	RETYPELONG							m_lUserWinScore[GAME_PLAYER];		//��ҳɼ�
	RETYPELONG							m_lUserReturnScore[GAME_PLAYER];	//������ע
	RETYPELONG							m_lUserRevenue[GAME_PLAYER];		//���˰��

	//�˿���Ϣ
protected:
    BYTE								m_cbDice[2];						//����ɫ��
	WORD								m_nSendCardCount;

	//״̬����
protected:
	DWORD								m_dwJettonTime;						//��ʼʱ��

	//ׯ����Ϣ
protected:
	RETYPELONG							m_lApplyBankerCondition;			//��������
	ApplyUserArrary						m_ApplyUserArrary;					//�����б�
	tagApplyUserInfo					m_CurrentBanker;					//��ǰׯ��
	BYTE								m_cbBankerTimer;					//��ׯ����
	RETYPELONG							m_lBankerWinScore;					//ׯ�ҳɼ�
	TCHAR							    m_szConfigFileName[MAX_PATH];			//�����ļ�

	//��¼����
protected:
	tagServerGameRecord					m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int									m_nRecordFirst;						//��ʼ��¼
	int									m_nRecordLast;						//����¼

	//�������
protected:
	ITableFrame							* m_pITableFrame;					//��ܽӿ�
	ITableFrameControl					* m_pITableFrameControl;			//��ܽӿ�
	const tagGameServiceOption			* m_pGameServiceOption;				//���ò���

	//���Ա���
protected:
	static const WORD					m_wPlayerCount;						//��Ϸ����
	static const enStartMode			m_GameStartMode;					//��ʼģʽ
private:
	BOOL							m_IsHightRoom;
	LONGLONG						m_LowerBigBankerCondition;	
		

private:
	struct TAndroidUser 
	{
		bool blAndroidUser;
		int  PlayCount;
		int  BankCount;
		TAndroidUser()
		{
			memset(this,0,sizeof(*this));
		};
	};
	TAndroidUser						m_AndroidUser[GAME_PLAYER];
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

	#ifdef __SPECIAL___
	//���в���
	virtual bool __cdecl OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem);////

	bool __cdecl IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	#endif
	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, RETYPELONG lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker( tagServerUserData *pUserData, bool bApplyBanker, bool bUserAction=true);
private:
	//����ܹ���ׯ����
	WORD GetBankerTime();
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
	RETYPELONG GetMaxOddScore(WORD wChairID);	
	//�����ע
	RETYPELONG GetMaxEvenScore(WORD wChairID);
	//����÷�
    void CalculateScore();
	//
	LONGLONG CalculateScoreBanker();
	//�ƶ�Ӯ��
	void DeduceWinner(BYTE &cbWinner);

private:
	//�����������ׯ
	void MakeAndroidUserBanker();
	//�����
	void RandList(BYTE cbCardBuffer[], BYTE cbBufferCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
