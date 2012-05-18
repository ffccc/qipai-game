#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϣ����\CMD_Billiards.h"

#define GFF_NORNAL			10				//��������

//////////////////////////////////////////////////////////////////////////

////��Ϸ�߼���
//class CGameLogic
//{
//	//��������
//private:
//
//	//��������
//public:
//	//���캯��
//	CGameLogic();
//	//��������
//	virtual ~CGameLogic();
//
//	//�ڲ�����
//private:
//
//};

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�������
protected:
//	CGameLogic						m_GameLogic;							//��Ϸ�߼�
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
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
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

	//�����¼�
public:
	//����ͬ��
	virtual bool __cdecl OnActionUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�������
	virtual bool __cdecl OnActionUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool __cdecl OnActionUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool __cdecl OnActionUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
	//��������
	virtual bool __cdecl OnActionUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }

	//�����¼�
public:
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);


	//�����Ϣ����
protected:
	BOOL OnHitFinish(WORD bDeskStation, tagGS_HitFinishStruct * pHitFinishInfo);
	//���ð���
	BOOL OnPlaceWhite(WORD bDeskStation, tagCS_PlaceBallStruct * pPlaceWhite);
	//�������Ƿ��к�����
	BOOL IsHaveRedBall();
	BOOL GameFinish(WORD bDeskStation, int iCloseFlag);
	void InitData();
private:
//	IGoldPresent                    * m_pIGoldPresent;                  //�ͽ�ҽ��
	BYTE                    m_GameLayer;
	WORD                    m_GameSetCount;                  //���ù�������
	int						m_iTimeDelay;					//������ʱ
	long int				m_dwMoney;						//��Ϸ���
	int						m_GsqPoint;						//��ʹ�����������ʱ��¼Ҫ�۵ķ֣��Ȳ���źú��ٰ�Ҫ�۵ķַ������
	BOOL					m_AgreeGoOn[2];					

	//��������
	int						m_iUserPoint[2];				//�û�����
	BOOL					m_bHitColor;					
	BYTE					bHitColor;			
	BYTE					m_bTargetBall;					//Ŀ����
	BYTE					m_bFinish[2];					//��Ϣͬ��
	BYTE					m_bNowHitUser;					//���ڻ�����
	//BYTE					m_bNextHitUser;					//��һ��������
	BYTE					m_bInBallIndex[BALL_NUM];				//������������
	BOOL					m_bBallAccess[BALL_NUM];				//�Ƿ���Է���
	double					m_iXBallStation[BALL_NUM];			//��� X λ��
	double					m_iYBallStation[BALL_NUM];			//��� Y λ��
	small					m_bNextColor;					//��һ���ô���һ��������
	BOOL					m_bIsPlayColor;					//�Ƿ�����׶�
	BOOL					m_bWhiteBallOver;				//�Ƿ�������
	int						m_PlaerNum;
	BOOL                    bIsSetRule;
	BYTE                    m_NowSetRule;                   //���ù�������
	BYTE                    m_StarPos;
	BYTE                    m_StarPoint;
	BOOL                    bIsCanAddStar;
	BOOL                    bIsSendTakeWhite;
	tagGS_HitBallStruct     pNowHitInfo;
	BOOL                    bIsHitFinish;
public:
	static const int		m_bPointIndex[BALL_NUM];				//��������
	static const BYTE		m_bColorIndex[BALL_NUM];				//��ɫ����
};

//////////////////////////////////////////////////////////////////////////

#endif