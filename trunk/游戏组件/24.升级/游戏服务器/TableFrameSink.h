#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�߼�����
protected:
	BYTE							m_cbPackCount;						//�Ƹ���Ŀ
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	BYTE							m_cbMainValue;						//������ֵ

	//�뿪����
protected:
	BYTE							m_cbResponses[GAME_PLAYER];			//��Ӧ״̬
	WORD							m_wRequestTimes[GAME_PLAYER];		//�����뿪

	//������Ϣ
protected:
	BYTE							m_cbValueOrder[2];					//��ֵ�ȼ�

	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
	bool							m_bUserStrustee[GAME_PLAYER];		//�û��й�

	//������Ϣ
protected:
	BYTE							m_cbCallCard;						//�����˿�
	BYTE							m_cbCallCount;						//������Ŀ
	WORD							m_wCallCardUser;					//�����û�

	//״̬����
protected:
	bool							m_bCallCard[4];						//���Ʊ�־
	BYTE							m_cbScoreCardCount;					//�˿���Ŀ
	BYTE							m_cbScoreCardData[12*PACK_COUNT];	//�÷��˿�

	//���Ʊ���
protected:
	WORD							m_wTurnWinner;						//ʤ�����
	WORD							m_wFirstOutUser;					//�����û�
	BYTE							m_cbOutCardCount[4];				//������Ŀ
	BYTE							m_cbOutCardData[4][MAX_COUNT];		//�����б�
	BYTE							m_cbLastTurnCard[GAME_PLAYER][MAX_COUNT];		//�ϴγ���
	BYTE							m_cbLastTurnCount[GAME_PLAYER];					//�ϴγ�����Ŀ

	//�����˿�
protected:
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];					//�����˿�

	//�û��˿�
protected:
	BYTE							m_cbHandCardCount[4];				//�˿���Ŀ
	BYTE							m_cbHandCardData[4][MAX_COUNT];		//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
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
	//�û�ͬ��
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem,VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserCallCard(WORD wChairID, BYTE cbCallCard, BYTE cbCallCount);
	//�������
	bool OnUserCallFinish(WORD wChairID);
	//�����˿�
	bool OnUserConcealCard(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);
	//�û��й�
	bool OnUserStrustee(WORD wChairID,bool bStrustee);
	//�����뿪
	bool OnUserRequestLeave(WORD wChairID, LPCTSTR pszLeaveReason);
	//��Ӧ�뿪
	bool OnUserResponsesLeave(WORD wChairID, WORD wTargerChairID, BYTE cbAgreeLeave);

	//��������
protected:
	//�ɷ��˿�
	bool DispatchUserCard();
};

//////////////////////////////////////////////////////////////////////////

#endif