#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

#define IDI_DECLARE_OVERTIME		10				//����ʱ��ʱ��
#define TIMER_BIND_WAITING			101				//�ȴ���ʱ��bind������ʶ
#define TIME_SCORE_STAKE_TIMES		100				//��ע����
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wSiceCount;						//���ӵ���
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbCardIndex[4][MAX_INDEX];		//�û��˿�

	//������Ϣ
protected:
	BYTE							m_bOutCardCount;					//������Ŀ
	WORD							m_wOutCardUser[136];				//�����û�
	BYTE							m_cbOutCardData[136];				//�����˿�

	//������Ϣ
protected:
	BYTE							m_cbSendCardData;					//�����˿�
	BYTE							m_bSendCardCount;					//������Ŀ
	BYTE							m_bLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbRepertoryCard[136];				//����˿�
	BYTE							m_cbKingBrothterPos;				//����ܵ�����λ��

	//�û�״̬
protected:
	bool							m_bReady[4];						//ready�ź�
	bool							m_bChiHu[4];						//�Ժ���־
	bool							m_bResponse[4];						//��Ӧ��־
	BYTE							m_bUserAction[4];					//�û�����
	BYTE							m_bOperateCard[4];					//�����˿�
	BYTE							m_bPerformAction[4];				//ִ�ж���

	//����˿�
protected:
	BYTE							m_bWeaveItemCount[4];				//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[4][4];				//����˿�

	//������Ϣ
protected:
	tagGangInfo						m_LastGangInfo;						//������Ϣ

	//������Ϣ
protected:
	LONG							m_lCellScore;						//��λ����
	bool							m_bCellScoreChange;					//�Ƿ�ı�

	//������Ϣ
protected:
	tagHuCourt						m_HuCourt;							//��������	//��Ϫ�齫ֻ����һ���˺�
	BYTE							m_cbBankerContinuCount[4];			//��ׯ��Ŀ
	LONG							m_lScoreLeft[4];					//ʣ�����	

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame	*					m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption *	m_pGameServiceOption;				//���ò���

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
	virtual VOID __cdecl Release() {  }
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
	virtual bool __cdecl SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

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
	virtual bool __cdecl OnActionUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�������
	virtual bool __cdecl OnActionUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool __cdecl OnActionUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool __cdecl OnActionUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
	//��������
	virtual bool __cdecl OnActionUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }

	//�����¼�
public:
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);
	//ready�ź�
	bool OnUserReady(WORD wChairID);

	//��������
protected:
	//�����¼�
	bool	_OnGameEndNormal(WORD wChairID, IServerUserItem * pIServerUserItem);
	bool	_OnGameEndAbnormal(WORD wChairID, IServerUserItem * pIServerUserItem);
	bool	_OnGameEndDismiss();
	//������
	BYTE 	_CheckAfford(tagHuCourt& HuCourt);
	//�������
	void	_OnFakeOver();
	//��һ����
	BYTE    _DealOneCard(bool bHead = true);
};

//////////////////////////////////////////////////////////////////////////

#endif