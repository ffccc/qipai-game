#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//ʤ�ʿ����˺���Ϣ
struct ControlAccount	
{
	DWORD dwUserID;
	int	  nOdds;
};

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	__int64								m_iTotalJettonScore[JETTON_AREA_COUNT];				//��������ע�ܶ�����
	__int64								m_iUserAreaScore[GAME_PLAYER][JETTON_AREA_COUNT];	//���������������ע������
	__int64								m_iAreaScoreLimit[JETTON_AREA_COUNT];				//������ǰ���·�
	__int64								m_iSysLimitScore;									//ϵͳ��ע����ϵͳ��ׯʱ����ע�޶�
	static const	int					m_iOddsArray[JETTON_AREA_COUNT];					//����������
	const	int							m_iMaxAreaScore;									//���������������·�
	bool								m_bControlWin;										//�Ƿ����ʤ�ʣ�������
	int									m_nMaxBankerTimes;									//һ��ׯ�������ڿ�������پ֣�������
	int									m_nHasWinTimes;										//ĳׯ����ׯ��������Ӯ�˶��ٴ�

	CArrayTemplate<ControlAccount>		m_CtrlAccountArray;									//�˺ſ�������


	//���Ʊ���
protected:
	__int64							m_iUserLimitScore;						//������ע
	__int64							m_iApplyBankerCondition;				//��ׯ����
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�

	//��ҳɼ�
protected:
	__int64							m_iUserWinScore[GAME_PLAYER];			//��ҳɼ�
	__int64							m_iUserReturnScore[GAME_PLAYER];		//������ע
	__int64							m_iUserRevenue[GAME_PLAYER];			//���˰��

	//�˿���Ϣ
protected:
	BYTE							m_iGoalAnimal;							//������Ϸ�Ὺ���Ķ���
	__int64							m_iGameTimes;							//��ǰ����Ϸ���������ĵڼ���

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��עʱ��

	//ׯ����Ϣ
protected:
	CArrayTemplate<WORD>			m_ApplyUserArray;						//������ׯ����б�
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	__int64							m_iBankerWinScore;						//�ۼƳɼ�
	__int64							m_iBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ��������

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	CFile							m_FileRecord;							//��¼���
	DWORD							m_dwRecordCount;						//��¼��Ŀ

	//�������
protected:
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
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 iJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

	//���ͺ���
private:
	//�����˿�
	bool DispatchTableCard();

	//���淢��
	void NormalDispatchCard();

	//���Ʒ���
	void CtrlDispatchCard(int nCtrlWinOdds = 100 );

	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);

	//ÿ����Ϸ��ʼʱ�̣������������·�ֵ
	bool __cdecl CalculateAreaInitLimite();


	//��Ϸͳ��
private:
	//����÷�
    __int64 CalculateScore();

	//ʤ�ʼ��㣬ϵͳ��ׯ������ʱ�á�����ֵΪ��������ʤ�����򣬷�Χ��0-7
	int	CalculateWinOdds();

	//����ĳ���������Ӯ�����������Ӯ�Ľ����
	bool GetOneAreaWinOdds(int nAreaIndex, __int64 & iResult);
};

//////////////////////////////////////////////////////////////////////////

#endif
