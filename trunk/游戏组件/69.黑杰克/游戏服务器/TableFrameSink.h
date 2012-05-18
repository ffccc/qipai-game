#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϣ����\CMD_BlackJack.h"
#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wPlayerCount;							//�������	
	WORD							m_wCurrentUser;							//��ǰ�û�
	
	//�������
protected:	
	BYTE							m_cbTableCutCardCount[GAME_PLAYER];		//�����˿�
	BYTE							m_cbTableCutCardArray[GAME_PLAYER][13];	//�����˿�  
	bool                            m_cbCutCard[GAME_PLAYER];               //�Ƿ����

	//��ʼ�˿�
protected:
	BYTE							m_cbTableCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE							m_cbTableCardArray[GAME_PLAYER][13];	//�����˿�  
    
	//���Ʊ���
protected:
	BYTE			            	m_cbRandCardArray[52];					//ɢ���˿�
	BYTE			            	m_cbSendCardCount;				        //�ѷ��˿�

	//��Ϸׯ��
protected:	
	int 						    m_nBankerStation;						//ׯ��λ��
	BOOL                            m_bBankerWin;                           //ׯ�һ�ʤ
	BOOL							m_bBankerGetCard;                       //ׯ��Ҫ��  
	LONG							m_lBankerScore;							//ׯ�ҷ���

	//��Ϸ�м�
protected:
	int								m_nNotBanker;
	LONG							m_lNotBankerScore[3];					//�мҷ���

	//��ʶ��ע
protected:
	BOOL							m_bAddGold[GAME_PLAYER];				//�Ƿ��ע	
	bool                            m_bAddGoldEnd;                          //��ע����	

	//�����Ϣ
protected:
	LONG							m_lMaxGold;								//�����ע
	LONG							m_lTurnBasicGold;						//������ע
	LONG							m_lTableGold[2*GAME_PLAYER];			//��ע���
	LONG                            m_lUserAddGold[GAME_PLAYER];            //�û���ע
	
	//���״̬
protected:
	BYTE							m_cbCardStatus[GAME_PLAYER];		   //��ʼ����
	BYTE							m_cbCutCardStatus[GAME_PLAYER];		   //�����û�

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* pGameServiceOption;					//���ò���

	//���Ա���
protected:
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
	virtual void __cdecl Release() { }
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
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϸ����
protected:
	//��ȡ�˿�
	BYTE GetRandCard();
	//�û�ǿ��
	bool OnUserLeft(bool bAddGoldEnd);
    //�û�ǿ��
	bool OnUserLeft(WORD wChairID, bool bAddGoldEnd);
	//����ɼ�
	bool CalcScore(BYTE cbCardStatus, WORD wChairID, LONG & lGameGold, LONG & lBankerScore, bool bCutCard);
	//����ͣ��
	bool OnSubUserStop(WORD wChairID);
	////�û�����
	//bool OnSubUserCutCard(WORD wChairID)
};

//////////////////////////////////////////////////////////////////////////

#endif