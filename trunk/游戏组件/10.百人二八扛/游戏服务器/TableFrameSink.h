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
#ifdef _DEBUG
class CTableFrameSink : public ITableFrameSink, public ITableUserAction, public ITableUserActionEX
#else
		class CTableFrameSink : public ITableFrameSink, public ITableUserAction, public ITableUserActionEX
#endif
#endif

{
	//����Ӯ��
public:
	struct tagWinArea
	{
		int area1;
		int area2;
		int area3;
	};

	static CMD_S_Win_Rate_Info			m_CtrlInfo;
	tagWinArea GetWinArea();
	void SetCard(tagWinArea winArea);

	//����ע��
protected:
	LONGLONG							m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע

	//������ע
protected:
	LONGLONG							m_lUserJettonScore[AREA_COUNT+1][GAME_PLAYER];//������ע

	//���Ʊ���
protected:
	LONGLONG							m_lAreaLimitScore;						//��������
	LONGLONG							m_lUserLimitScore;						//��������
	LONGLONG							m_lApplyBankerCondition;				//��������
	TCHAR							    m_szConfigFileName[MAX_PATH];			//�����ļ�

	//��ҳɼ�
protected:
	LONGLONG							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	LONGLONG							m_lUserReturnScore[GAME_PLAYER];		//������ע
	LONGLONG							m_lUserRevenue[GAME_PLAYER];			//���˰��
	BYTE								m_cbLeftCardCount;						//ʣ���˿���Ŀ
	bool								m_bContiueCard;							//��������

	BYTE								m_bcFirstPostCard[2];					//ҡ���ӵ���

	TAndroidUser						m_AndroidUser[GAME_PLAYER];
	UserPlaceJetton                     m_UserPlaceJetton[GAME_PLAYER];


	//�˿���Ϣ
protected:
	BYTE							m_cbCardCount[CARD_LOCATION_COUNT];			//�˿���Ŀ
	BYTE							m_cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE];				//�����˿�
	BYTE							m_cbTableCard[CARD_COUNT];				//�����˿�

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
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	CFile							m_FileRecord;							//��¼���
	DWORD							m_dwRecordCount;						//��¼��Ŀ
	LONGLONG						m_UserLeftScore;

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
	int                             m_PlayCount;
	int								m_WinCount;
	TCHAR						    m_AndroiUserName[GAME_PLAYER][128];
	INT								m_AndroiUserNameCount ;

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
	virtual bool __cdecl IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CTableFrameSink)) ? true : false;
	}
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
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
	{
		return true;
	}

#ifdef __SPECIAL___
	//���в���
	virtual bool __cdecl OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem);////

	bool __cdecl IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem);
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
	bool DispatchTableCard(bool AndroidUser = false, bool blWin = false);
	//����ׯ��
	void SendApplyUser(IServerUserItem *pServerUserItem);
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

	//��Ϸͳ��
private:
	//����÷�
	LONGLONG CalculateScore();
	//�ƶ�Ӯ��
	void DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan, BYTE &TianMultiple, BYTE &diMultiple, BYTE &TianXuanltiple);
};

//////////////////////////////////////////////////////////////////////////

#endif
