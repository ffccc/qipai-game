#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//�˿˱���
protected:
	LONGLONG                            m_lApplyBankerCondition;

	LONGLONG                            m_lMeMaxScore;
	LONGLONG                            m_lMeTieScore;
	LONGLONG                            m_lMeBankerScore;
	LONGLONG                            m_lMePlayerScore;
	INT64								m_lAreaLimitScore;


	int									m_nBetArea;
	LONGLONG							m_nCanBetMoney;
	LONGLONG							m_nCell;
	LONGLONG							m_nMinMoney;
	LONGLONG							m_nMaxMoney;
	LONGLONG							m_nBetMoney;
	LONGLONG							m_nLimitBet;

	int									m_bIsAllowBanker;
	int									m_BankerMinTimes;
	int									m_BankerMaxTimes;

	//ׯ����Ϣ
	protected:
	WORD                          		m_wCurrentBanker;
	LONGLONG                      		m_lCurrentBankerScore;
	bool                          		m_bMeCurrentBanker;
	bool                          		m_bAllowApplyBanker;
	bool                          		m_bMeApplyBanker;
	int                           		m_nBankerTime;

	//�ؼ�����
protected:
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { if (IsValid()) delete this; return ; }
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
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnChangeBanker(const void * pBuffer, WORD wDataSize);
	//���»���
	bool OnChangeUserScore(const void * pBuffer, WORD wDataSize);
	//��Ϸ��¼
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameScore(const void * pBuffer, WORD wDataSize);

private:

	//��ע����
	BYTE RandJettonArea();

	bool RandBoolean() { (rand() % 2) ==0 ? true : false; }
};

//////////////////////////////////////////////////////////////////////////

#endif