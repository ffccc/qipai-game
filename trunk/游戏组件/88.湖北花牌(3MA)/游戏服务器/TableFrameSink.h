#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//1����������Ϸ�����У���ֵ����صı���
protected:
	WORD				m_wBankerUser;							//ׯ���û�--��ͬ���У�Ҫ������ֵ
	//2����������������Ϸ�����ڲ�ͬ���ж�Ҫ��յı���
	//��Ϸ����
protected:
	BYTE				m_cbLeftCardCount;						//�����ƶ�����ʣ���Ƶĸ���--��ͬ���У����������ֵ
	BYTE				m_cbRepertoryCard[MAX_REPERTORY];		//ÿ���ƵĿ������Ϣ--��ͬ���У����������ֵ
	
	BYTE				m_cbSendCardCount;						//�ܷ��ƴ���--��ͬ���У����������ֵ

	BYTE				m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��˿�--��ͬ���У����������ֵ

	BYTE				m_cbWeaveItemCount[GAME_PLAYER];		//���û��������Ŀ--��ͬ���У����������ֵ
	tagWeaveItem		m_WeaveItemArray[GAME_PLAYER][8];		//���û��������Ϣ--��ͬ���У����������ֵ

	BYTE				m_cbDiscardCount[GAME_PLAYER];			//ÿ���û������Ƶĸ���--��ͬ���У����������ֵ
	BYTE				m_cbDiscardCard[GAME_PLAYER][34];		//ÿ���û������Ƶ���Ϣ--��ͬ���У����������ֵ

	BYTE                m_cbHuaCardIndex[GAME_PLAYER][5];        //�����û����и����Ƶĸ���--��ͬ���У����������ֵ
	BYTE                m_cbHuaWeaveCardIndex[GAME_PLAYER][5];   //�����û�������и����Ƶĸ���--��ͬ���У����������ֵ

	BYTE                m_SiTongCount[GAME_PLAYER];              //ÿ���û�������ʱ���Ĳز�������--��ͬ���У����������ֵ
	BYTE                m_SiTongIndex[GAME_PLAYER][8];           //�Ĳز���ʱ��������

	BYTE                m_WuTongCount[GAME_PLAYER];              //ÿ���û�������ʱ����ز�������--��ͬ���У����������ֵ
	BYTE                m_WuTongIndex[GAME_PLAYER][8];           //��ز���ʱ��������

	bool                m_bGameStart;                            //���ׯ���Ƿ������ʽ����
	bool				b_HuangZhuang;							//�Ƿ��ׯ--��ͬ���У����������ֵ

	//3����������ÿ�ֲ��������еģ�ÿ��֮��Ҫ��յı���
protected:
	BYTE			    m_cbChiHuCard;				             //�����ܺ�������
	tagHuCardResult		m_HuCardResult[GAME_PLAYER];			 //ÿ�η����û��ܷ����ʱ����������

	BYTE               m_bSendCount;                            //ÿ�η��Ƶ�����--ÿ�η���֮ǰ��Ҫȷ����ֵ
	BYTE				m_cbSendCardData[2];					//ÿ�η��Ƶ�����

	//���б���
	WORD				m_wResumeUser;							//���������û�������ʱ���������ĵ�һ���û���Ϣ�����ڻ�ԭ�û�
	WORD				m_wCurrentUser;							//��ǰ�û�

	WORD				m_wProvideUser;							//�ṩ�����Ƶ��û�
	BYTE				m_cbProvideCard;						//���û��ܲ�����ʱ�����б������ܲ�������

	WORD				m_wOutCardUser;							//�����û�--ÿ�γ���ʱ��Ҫ���
	BYTE				m_cbOutCardData;						//�����˿�--ÿ�γ���ʱ��Ҫ���

	//״̬����
	bool				m_bSendStatus;							//����״̬---?????????

	//�û�״̬
	BYTE				m_cbUserAction[GAME_PLAYER];			//�û�����--������û��Ĳ�����ʾ��Ϣ

	BYTE				m_cbOperateCard[GAME_PLAYER];			//�����˿�--�����û�����ִ�в�������Ӧ����
	BYTE				m_cbPerformAction[GAME_PLAYER];			//ִ�ж���--������û�����ִ�еĲ���

	bool				m_bResponse[GAME_PLAYER];				//��Ӧ��־--������û��Ƿ���Ӧ���˲���

	//�����һ�ֵĲ��ƴ�����̶����õı���
	BYTE              m_cbFirstUserAction[GAME_PLAYER+1];     //��˳�򱣴���ĸ��û��ܽ��еĲ����������ء��ء���
	WORD              m_cbFirstUser[GAME_PLAYER+1];          //��˳�򱣴���ĸ��û���ׯ�ң��ϼҡ����ϼҡ�ׯ��
	BYTE              m_cbFirstCurrentIndex;                //��ǰ�������û��������������(0-3)
	BYTE              m_cbFirstCurrentAction;               //��ǰ�û�����Ӧ�Ĳ���
	WORD              m_cbFirstCurrentUser;

	//4���������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						*m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//5�����Ա���
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
	virtual void __cdecl Release() { if (IsValid()) delete this; }
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

	//�û��¼�
public:
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);

	//��������
protected:

	//////////babala 11_15wan

	//��������Ƿ���Բ���
	BYTE EstimateTongCard(WORD wChairID);

	//���Ͳ���
	bool SendOperateNotify();
	
	//�ɷ��˿�
	bool DispatchCardData(WORD wCurrentUser);
	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard);
};

//////////////////////////////////////////////////////////////////////////

#endif