#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϣ����\CMD_Mj.h"
#include "GameLogic.h"
#include "CLog.h"
//#include <vector>

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��־
	CLog							log;

	//��������
private:
	bool							m_bZiM;											//�Ƿ�����
	WORD							m_wBankerUser;									//ׯ��
	WORD							m_wCurrentUser;									//��ǰ���
	WORD							m_wLastUser;									//���ֳ������
	WORD							m_wLostUser;									//�������
	long							m_nFang[GAME_PLAYER];							//���Ʒ���

	char							*m_strFang;										//�㷬��
	BYTE							m_wQFeng;										//Ȧ��

	bool							m_bTianHu;										//�������
	bool							m_bDiHu[GAME_PLAYER];							//�غ�����
	bool							m_bRenHu[GAME_PLAYER];							//�˺�����
	//������Ϣ
private:
	WORD							m_wTurnWiner;									//ʤ�����
	BYTE							m_byOutMj;										//������
	int								m_ActionRight[GAME_PLAYER];						//����Ȩ��,6-��,4-��,3-��,																						  2-��,1-�ȴ�, 0-ȡ��

	//�齫��Ϣ
private:
	static  BYTE					m_byMjs[ORG_MJ_MAX];							//�齫������
	BYTE							m_byTouchMjCount;								//�������齫����
	int								m_nTouchMjPos;									//����λ��
	BYTE							m_byHandMjCount[GAME_PLAYER];					//�����������
	BYTE							m_byHandMjs[GAME_PLAYER][HAND_MJ_MAX];			//�����齫
	HU								m_hu[GAME_PLAYER];								//������
	HU								m_tempHu[GAME_PLAYER];							//��ʱ������

	//�������
protected:
	CGameLogic						m_GameLogic;									//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;								//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;							//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;									//��Ϸ����
	static const enStartMode		m_GameStartMode;								//��ʼģʽ

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
	virtual bool __cdecl SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutMj( WORD wChairID, BYTE *byOutMj );
	//�û�����
	bool OnUserTouchMj( WORD wChairID );
	//�û�����
	bool OnUserGiveUp( WORD wChairID, const void *pDataBuffer);
	//�û�����
	bool OnUserPeng( WORD wChairID, const void *pDataBuffer );
	//�û�����
	bool OnUserChi( WORD wChairID, const void *pDataBuffer );
	//�û�����
	bool OnUserGang( WORD wChairID, const void *pDataBuffer );
	//�û�����
	bool OnUserHu( WORD wChairID, const void *pDataBuffer );
	//�û�����ѡ��
	bool OnUserChiSelect( WORD wChairID, int nChiType);
	//�û�ѡ�����
	bool OnUserGangSelect( WORD wChairID, const void *pDataBuffer );

	//����
	bool OnGameEndEmpty();
};

//////////////////////////////////////////////////////////////////////////

#endif