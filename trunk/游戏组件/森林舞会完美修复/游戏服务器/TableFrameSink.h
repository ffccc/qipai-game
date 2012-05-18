#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϣ����\GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�����Ϣ


struct tagApplyBanker
{
	DWORD							dwUserID;							//���ID
	WORD							wChairID;							//���Ӻ���
	INT64							lUserScore;							//��һ���
};

struct tagCurrentBanker
{
	WORD							wBankerCount;						//��ׯ����
	INT64							lBankerWinScore;					//ׯ�ҳɼ�
	bool							bCancel;							//������ׯ��־
	bool							bAI;							    //�����˱�־
	DWORD							dwUserID;							//���ID
	WORD							wChairID;							//���Ӻ���
	INT64							lUserScore;							//��һ���
	tagServerUserData*				pUserData;							//�û�����ָ��

	//����=������
	tagCurrentBanker& operator = (tagApplyBanker& ApplyBanker)
	{
		dwUserID = ApplyBanker.dwUserID;
		wChairID = ApplyBanker.wChairID;
		lUserScore = ApplyBanker.lUserScore;
		return (*this);
	};

	tagCurrentBanker& operator = (tagApplyBanker* pApplyBanker)
	{
		dwUserID = pApplyBanker->dwUserID;
		wChairID = pApplyBanker->wChairID;
		lUserScore = pApplyBanker->lUserScore;
		return (*this);
	};
};

typedef CArrayTemplate< tagApplyBanker > ApplyUserArrary;

const  LONG money[15]={100, 100,100,1000,1000, 1000, 10000, 10000, 10000,100000, 1000000, 1000000,100000,5000000,10000000};

const BYTE plu[][15]={	6,7,8,10,11,13,17,20,23,33,40,46,3,3,3,
	5,7,8,8,11,13,14,20,23,28,40,46,3,3,3,
    4,7,8,7,11,13,12,20,23,25,40,46,3,3,3,
	5,6,7,8,10,11,14,17,20,33,35,40,3,3,3,
	4,5,6,7,8,10,11,14,17,25,28,35,3,3,3,
};

const BYTE pColoct[][24]=
{
	0,0,7,3,3,4,1,0,6,3,0,0,6,2,0,8,9,3,9,10,6,5,3,11,
	1,0,7,3,5,4,1,0,6,3,0,1,7,2,2,6,11,4,9,10,8,5,3,11,
	0,1,7,4,4,4,1,0,7,4,1,1,6,2,0,8,11,5,10,9,6,5,3,10,
	0,1,8,4,3,3,1,0,8,3,1,1,6,2,0,7,10,5,11,9,6,5,4,10,
	2,1,8,4,3,3,0,0,8,3,1,1,6,1,0,7,9,5,9,11,6,5,3,10,
	1,2,8,4,3,3,2,1,7,3,1,2,6,1,0,8,9,5,9,11,8,6,4,10,
	0,2,8,4,3,3,0,1,6,3,2,2,6,0,0,8,11,5,9,10,7,7,4,10,
	0,0,7,4,3,4,0,1,6,3,2,0,6,0,0,8,9,5,11,10,6,8,4,9
};


//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��ע��Ϣ
protected:
	CArrayTemplate<tagHistory>		m_HistoryArray;						//���������ʷ��¼

	//��ע��Ϣ
protected:
	//��עͳ�ƣ�����ע������ע������ע����1->��9����1->��9����10����עλ��
	
	LONG							m_lDetailStakes[GAME_PLAYER][INDEX_MAX];	//���û�����עλ��ͳ�Ƶ���ע���
	LONG							m_lPlayerStakes[GAME_PLAYER];			//ÿ���û��Ľ�Ҷ�
	LONG							m_lPlayerWin[GAME_PLAYER][INDEX_MAX];			//ÿ���û�ÿ������Ӯ
	LONG							m_lAreaStakes[INDEX_MAX];				//ֻ����עλ��ͳ�Ƶ���ע���

	INT64							m_lWinAmount[GAME_PLAYER];				//�����Ӯ
	LONG                             m_lCellGold;                            //��λ���
	BYTE							m_CardCount[15];						//����
	BYTE							m_CardColoct[24];						//��ɫ        
	BYTE							m_Card;	                                //ɫֵ
	BYTE							m_Cardsite;	                            //λ��
	bool                            m_Winning;                              //�Ƿ񷢷Ųʽ�

	int                             nActStatus;                             //ʱ��״̬
	int                             m_nTimerTime;                           //��ʱ��
    int                             PlayGame;
	int                             m_numr;                                 //��������
	int                             m_bjlnp;




	//�������
protected:
	LONG							m_lApplyBankerCondition;			//������ׯ����ͷ���

	BYTE							m_cbWinPercent;						//ׯ��ʤ��

	WORD							m_lBankerCountMax;					//������ׯ��������

	WORD							m_wApplyCountMax;					//������ׯ���������

	BYTE                           	PlayerBankerlow;                    //�����ׯ����   

	BYTE                           	m_BSIDSL;                       //��ʤIDʤ�� 

	DWORD							BSID1;							//���ID
	DWORD							BSID2;							//���ID
	DWORD							BSID3;							//���ID
	DWORD							BSID4;							//���ID
	DWORD							BSID5;							//���ID

    //�ʽ�
	LONG                            m_Prizemoney;

	//�ѷ��ŵĲʽ�
	LONG                            m_PrizeGame;
    //��ӮǮ��
	INT64                        	m_WinMoney;
    //����Ǯ��
	INT64                        	m_LoseMoney;
    //��Ӯ����
	BYTE                            m_Winratio;

    int                             m_ratio[12];
	int                             m_ratioCount;

	DWORD							m_dwUserID;							//���ID	
	BOOL                            ISbanker;                           //����
   int                              m_char;
	ApplyUserArrary					m_ApplyUserArrary;					//�����б�
	tagCurrentBanker				m_CurrentBanker;					//��ǰׯ��

	INT64                            m_lBanker;                         // ׯ�ҳɼ�



	//״̬����
protected:
	DWORD							m_dwJettonTime;						//��ʼʱ��

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	ITableFrameControl				* m_pITableFrameControl;
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���
	ITableFrameManager              * m_TableFrameManager;

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
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

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


	DWORD WINAPI GetVerInfo();
	void ShutDown(DWORD verInfo);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();
   // ��ʼ����
	void OnGameDispatch();
	// ����ʱ��״̬
	void SendTimerTyper(WORD type,WORD num);
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
public:
	//�û��뿪-hlz
	virtual bool __cdecl OnEventUserLeave(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//����ͬ��
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//�������
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//��������
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//��������
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true;};
	//��������
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true;};

	//�����¼�
public:

	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);


	public:
		//�û�ͬ��
		virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbBetArea, INT64 lBetAmount);

	//����ׯ��
	bool OnUserApplyBanker( tagServerUserData *pUserData, bool bApplyBanker , bool bApplybAi);

	//��������
private:
	//�����˿�
	bool DispatchTableCard();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	void ChangeBanker();
	//�ֻ�ׯ��
	void SendChangeBankerMsg();
	//�жϻ���û�����
	bool HavePlayer();
	//д��
	void WriteWinAcount();

	void MakeRobotMessage();
};

//////////////////////////////////////////////////////////////////////////

#endif