#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϣ����\CMD_LLShow.h"
#include "..\��Ϣ����\logic.h"

//����ԭ��
#define GER_NO_PLAYER						0x10						//û�����

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��������
	BYTE							m_bChess[GAME_PLAYER][CHESSCOUNT_H][CHESSCOUNT_W];//ÿ����ҵĵ�ǰ����״̬
	BYTE							m_bPropCnt[GAME_PLAYER][PROP_MAX_CNT];				  //����
protected:		
	BYTE							m_bPlayStatus[GAME_PLAYER];				//��Ϸ״̬
	BYTE							m_bTimerCnt[GAME_PLAYER];				//��ǰ��ҵĶ�ʱ��ʱ��
	BYTE							m_bSeriesLinkCnt[GAME_PLAYER];			//��������ͳ��
	int								m_WinnerBonus;							//Ӯ�߽���
	
	//�������
protected:
	CLogic							m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���

	bool							m_bIsGameEnd;




//////////////////////////////////////////////////////////////////////////////////////
	int								RandNumber(int nMin,int nMax);
	void							InitVariable();							//��ʼ������

	bool							GameEnd(WORD wSeat,BYTE bEndReason);

	// ������Ϣ
	bool							SubGameLost(WORD wSeat, BYTE bEndReason);

	// ������������
	int								AccountChessCount(BYTE chess[CHESSCOUNT_H][CHESSCOUNT_W]);
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { if (IsValid()) delete this; return; }
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
};
//static void RetailOutput(TCHAR *szFormat, ...);
//////////////////////////////////////////////////////////////////////////

#endif