#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϣ����\GameLogic.h"
#include "TableFrameSink.h"

//const __int64  money[10] = {100L,100L,1000L,10000L,500L,500L,5000L,1000L,5000L,10000L};
//const  __int64 money[7]={100, 1000, 10000,100000, 1000000 ,5000000,10000000};
//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//�˿˱���
protected:
	__int64							m_lMyMaxStake;						//�����ע
	BYTE							m_cbTurnCardCount;					//��עʱ���ۼ�
	//BYTE							m_cbTurnCardData[MAX_COUNT];		//�����б�

	__int64							m_Money;							//�û�����
	WORD							m_wCurrentBanker;					//��ǰׯ��
	bool							m_first;							//�ս���
	__int64							m_lApplyBankerCondition;			//��ׯ���ٷ���
	INT nOdds[INDEX_MAX]; //��Ӯ����
	bool                            m_ShakeType[3];                     //ҡ���ӵķ�ʽ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�
	BYTE                            cbBankerValue;
	BYTE                            cbPlayerValue;	
	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAndroidUserItemSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool __cdecl InitUserItemSink(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool __cdecl RepositUserItemSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool __cdecl OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool __cdecl OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool __cdecl OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//������Ϣ
	virtual bool __cdecl OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual void __cdecl OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û���λ
	virtual void __cdecl OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//��Ϣ����
protected:
	//�л�ׯ��
	bool OnSubChangeBanker(void * pData, WORD wDataSize);
	//ʱ��״̬
	bool OnSubTimerType(void * pData, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(void * pData, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(void * pData, WORD wDataSize);
	//������ׯ
	bool OnSubApplyBanker(void * pData, WORD wDataSize);

	//����������
	bool OnSubChangeCard(void * pData, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(void * pData, WORD wDataSize);
	int GetNextChair(int chair){ return (chair+GAME_PLAYER)%GAME_PLAYER;};
	int AutoOutCard(bool bOutCard);
public:
	// �����˼�ע
	void OnAddJeetton(void);
	// �Լ�������ׯ
	void OnAppyBanker(void);
};

//////////////////////////////////////////////////////////////////////////

#endif