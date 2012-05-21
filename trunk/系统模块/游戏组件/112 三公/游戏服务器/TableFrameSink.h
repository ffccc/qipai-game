#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	LONG							m_lExitScore;							//�˳�ע��
	bool							m_bOffLineGameEnd;						//���߽���
	WORD							m_wBankerUser;							//ׯ���û�
	LONG							m_lGameScore[GAME_PLAYER];				//������Ϣ
	LONG							m_lUserChipIn[GAME_PLAYER];				//��ע��Ϣ
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE							m_cbUserCardData[GAME_PLAYER][3];		//�û��˿�

	WORD							m_wMaxChairID;

	//״̬����
protected:
	BYTE							m_cbSendCard;							//���Ʊ�־
	BYTE							m_cbWaitStart;							//�ȴ���ʼ
	BYTE							m_cbWaitChange;							//�ȴ���ׯ
	BYTE							m_cbWaitContinue;						//�ȴ�����

	//״̬����
protected:
	BYTE							m_cbUserFinish[GAME_PLAYER];			//��ɱ�־
	BYTE							m_cbUserBanker[GAME_PLAYER];			//��ׯ��־
	BYTE							m_cbUserContinue[GAME_PLAYER];			//������־

	//��������
protected:
	LONG							m_lMaxScore;							//�����ע
	LONG							m_lLessScore;							//������ע
	WORD							m_wCurrentUser;							//��ǰ�û�
	DWORD							m_dwBankerStartTime;					//��ʼʱ��

	//AI����
protected:
	LONG							m_dStockScore;							//����Ӯ��

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
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
	//��������
	//virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon);

	//�����¼�
public:
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//���ܺ���
protected:
	//�˿˷���
	void AnalyseCard();
	//����ׯ��
	void SetBankerUser(WORD wBankerUser);
	//ׯ�Ҽ��
	bool IsSuitBankerUser(WORD wChairID);
	//Ѱ��ׯ��
	bool SearceBankerUser();

	//��������
protected:
	//�û���Ŀ
	WORD GetTableUserCount();
	//�����Ŀ
	LONG GetUserMaxChipIn(WORD wChairID);
	//��������
	bool ChangeCardData();
};

//////////////////////////////////////////////////////////////////////////

#endif
