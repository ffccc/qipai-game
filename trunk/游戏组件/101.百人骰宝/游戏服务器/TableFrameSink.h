#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../��Ϣ����/GameLogic.h"

typedef struct tagPersonInfo
{
	DWORD dwUserID;           // ���ID
	int iPercent;             // ʤ��
}S_PERSONINFO;
typedef CArrayTemplate<S_PERSONINFO,S_PERSONINFO&> CPensonInfoArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	__int64							m_lAllScore[COUNT_AZIMUTH];				// ÿ���������µ���ע
	__int64                         m_lAllAndroidWinScore;                  // ���л�������Ӯ����ע
	//__int64                         m_lMaxAndroidLost;                      // ��������������
	bool                            m_bIsAllAndroid;                        // ���е��˶��ǻ�����
	int								m_iAILostLimitPercent;						//����������ʤ��

	//������ע
protected:
	__int64							m_lUserScore[COUNT_AZIMUTH][GAME_PLAYER];			//�����ÿ���������µ���ע
protected:
	__int64							m_lAreaLimitScore;						//��������
	__int64							m_lUserLimitScore;						//�������
	__int64							m_lApplyBankerCondition;				//��������
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�

	//��ҳɼ�
protected:
	__int64							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	__int64							m_lUserReturnScore[GAME_PLAYER];		//������ע
	__int64							m_lUserRevenue[GAME_PLAYER];			//���˰��

	//�˿���Ϣ
protected:
	E_SICBO_NUMBER					m_enCards[MAX_COUNT_SICBO];             // ���ӵ���
	bool                            m_bWinner[COUNT_AZIMUTH];               // ��Ӯ���
	CPensonInfoArray                m_arPensonInfo;
	INT                             m_iUseWinner;                          // ����ĳһ����Ӯ

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��
	int                             m_iMaxAndroidBanker;                    // ��������ׯ������
	int                             m_iSameCount;                           // ������ͬ�����ļ��
	int                             m_iAndroidRandCount;                    // ���ƻ�����Ӯʹ�õĴ�������


	// ׯ����Ϣ
protected:
	CArrayTemplate<WORD>			m_wApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	__int64							m_lBankerWinScore;						//�ۼƳɼ�
	__int64							m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ
	bool                            m_bChangeSicboType;                     // �Ƿ��Ѿ��ı�ҡ���ӵķ�ʽ
	E_SICBO_TYPE                     m_enSicboType;                         // ҡ���ӵķ�ʽ

	//���Ʊ���
protected:
	BYTE							m_cbWinSideControl;					    // ������Ӯ
	int                             m_iNTWinPercent;
	int								m_nSendCardCount;					    // ���ʹ���
	int                             m_iAndroidNTCount;

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
	CCriticalSection				   m_CriticalSection;				//�߳�����
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
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

	//��������
private:
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);

	//��Ϸͳ��
private:
	//����÷�
    __int64 CalculateScore(void);
	//�ƶ�Ӯ��
	void DeduceWinner(bool bWinner[]);

	// ������Ӯ
	void CtrlWinner(void);
	// ����ϵͳ��ׯ��Ӯ
	void CtrlNTWinner(void);

	// ���ƻ�����ҪӮ
	void CtrlAndroidWinner(void);

	// ��ȡÿ�����������ע��
	__int64 GetMaxPlaceJetton(int iArea, WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif
