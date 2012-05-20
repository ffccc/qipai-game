#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϸ�ͻ���\GameLogic.h"
#include "..\��Ϣ����\CMD_Thirteen.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�������
protected:
	CGameLogic							m_GameLogic;										//��Ϸ�߼�
	ITableFrame							* m_pITableFrame;									//��ܽӿ�
	const tagGameServiceOption			* m_pGameServiceOption;								//���ò���
	bool								m_bBaoPai[GAME_PLAYER];								//��Ұ���

	//���Ա���
protected:
	static const WORD					m_wPlayerCount;										//��Ϸ����
	static const enStartMode			m_GameStartMode;									//��ʼģʽ

	//��Ϸ����
protected:
	LONG								m_nChip[GAME_PLAYER];								//��ע��С
	LONG								m_lMaxChip;											//�����ע
	BYTE								m_bUserCardData[GAME_PLAYER][HAND_CARD_COUNT];		//����˿�
	BYTE								m_bSegmentCard[GAME_PLAYER][3][5];					//�ֶ��˿�
	bool								m_bFinishSegment[GAME_PLAYER];						//��ɷֶ�
	WORD								m_wFirstCallBankerUser ;							//��ׯ���
	WORD								m_wBanker;											//ׯ�����
	WORD								m_wCurrentCaller ;									//��ǰ��ׯ
	BYTE								m_bShowCardCount;									//̯����Ŀ
	BYTE								m_bCompleteCompareCount ;							//��ɱȽ�
	BYTE								m_bShowCardTime;									//̯��ʱ��
	BYTE								m_bSetChipCount;									//��ע����

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

	//��Ϣ����
protected:
	//������ע
	bool OnSetChip(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//���÷ֶ�
	bool OnSetSegment(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//���̯��
	bool OnShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//̯��ʱ��
	bool OnShowCardTime(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;





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

	//////////////////////////////////////////////////////////////////////////
	//�����¼�
public:
	//����ͬ��
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//�������
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//��������
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//��������
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true ;}
	//��������
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true ;}

	//�����¼�
public:
		//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){return true ;}
};

//////////////////////////////////////////////////////////////////////////

#endif
