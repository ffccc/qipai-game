#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "RandomGen.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableFrameEvent
{
protected:
	CRITICAL_SECTION				m_csPlaceJetton;
	//ׯ����Ϣ
protected:
	CArrayTemplate<WORD>			m_ApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	LONG							m_lBankerWinScore;						//�ۼƳɼ�
	LONG							m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ
	bool							m_bChangeBanker;						//ϵͳ��ׯ����
	BYTE							m_bMaxBankerTime;						//��ׯ����
	//���Ʊ���
protected:
	LONG							m_lAreaLimitScore;						//��������
	LONG							m_lUserLimitScore;						//��������
	LONG							m_lApplyBankerCondition;				//��������
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�
	BYTE							m_cbWinSideControl;						//������Ӯ
	BYTE							m_cbMoveEndIndex;						//�ƶ���������
	BYTE							m_cbResultIndex;						//�������
	BYTE							m_cbLastResultIndex;					//�ϰ�����
	BYTE							m_cbEndIndex;							//ֹͣ����
	//ȫ����ע
protected:
	LONG							m_lALLBigSharkScore;						//�Լ�����������
	LONG							m_lALLFeiQinScore;						//�Լ�����������
	LONG							m_lALLZouShouScore;						//�Լ�����������
	LONG							m_lALLYanZiScore;						//�Լ�����������
	LONG							m_lALLTuZiScore;						//����
	LONG							m_lALLGeZiScore;						//����
	LONG							m_lALLXiongMaoScore;					//��è
	LONG							m_lALLKongQueScore;						//��ȸ
	LONG							m_lALLHouZiScore;						//����
	LONG							m_lALLLaoYingScore;						//��ӥ
	LONG							m_lALLShiZiScore;						//ʨ��
	//������ע
protected:
	LONG							m_lUserBigSharkScore[GAME_PLAYER];		//�����
	LONG							m_lUserFeiQinScore[GAME_PLAYER];		//����
	LONG							m_lUserZouShouScore[GAME_PLAYER];		//����
	LONG							m_lUserYanZiScore[GAME_PLAYER];			//�Լ�����������
	LONG							m_lUserTuZiScore[GAME_PLAYER];			//����
	LONG							m_lUserGeZiScore[GAME_PLAYER];			//����
	LONG							m_lUserXiongMaoScore[GAME_PLAYER];		//��è
	LONG							m_lUserKongQueScore[GAME_PLAYER];		//��ȸ
	LONG							m_lUserHouZiScore[GAME_PLAYER];			//����
	LONG							m_lUserLaoYingScore[GAME_PLAYER];		//��ӥ
	LONG							m_lUserShiZiScore[GAME_PLAYER];			//ʨ��
	//��ҳɼ�
protected:
	LONG							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	LONG							m_lUserReturnScore[GAME_PLAYER];		//������ע
	LONG							m_lUserRevenue[GAME_PLAYER];			//���˰��
	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��
	//��ʷ��¼
protected:
	CArrayTemplate<BYTE>			m_cbScoreHistory;						//��ʷ��¼
	//������Ϣ
protected:
	BYTE							m_cbZuoBiIndex;							//��������
	BYTE							m_cbZuoBiCount;							//������������(ͬһ����)
	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���
	ITableFrameControl				* m_pITableFrameControl;				//��ܽӿ�
	CRandomGen						m_RandGen;						
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
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
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
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStation, bool bSendSecret);
	//��������
public:
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//ϵͳ�Զ�������ׯ
	void SysChangeBanker(bool bCancelCurrentBanker);
	//��������
	void RandGameIndex();
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//����÷�
	LONG CalculateScore();
	//���ͼ�¼
	void SendGameRecord();
	//�����ע
	LONG GetMaxScore(WORD wChairID, LONG lJettonScore,BYTE cbArea);
	//�����ע
	LONG GetUserMaxJetton(WORD wChairID);
	//�����Ϣ
	CString DrawScoreString(LONG lNumber);

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
	virtual bool __cdecl OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//�û�����
	virtual bool __cdecl OnEventUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//�û�����
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnEventUserReady(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
};

//////////////////////////////////////////////////////////////////////////

#endif