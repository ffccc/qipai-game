#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//������Ϣ
protected:
	LONG							m_lMeMaxScore;						//�����ע
	LONG							m_lAreaLimitScore;					//��������
	LONG							m_lApplyBankerCondition;			//��������

	//������ע
protected:
	LONG							m_lMeTieScore;						//��ƽ��ע
	LONG							m_lMeBankerScore;					//��ׯ��ע
	LONG							m_lMePlayerScore;					//������ע
	LONG							m_lMeTieSamePointScore;				//ͬ��ƽע
	LONG							m_lMePlayerKingScore;				//������ע
	LONG							m_lMeBankerKingScore;				//ׯ����ע
	LONG							m_lMePlayerTwoPair;					//������ע
	LONG							m_lMeBankerTwoPair;					//������ע

	//ׯ����Ϣ
protected:
	LONG							m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��

	//״̬����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ


		//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
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
	virtual VOID __cdecl Release() { }
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

	//��������
private:
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount);
	//����ׯ��
	void SetBankerInfo(WORD wBanker,LONG lScore);

	//��Ϸ��Ϣ
private:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);	
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif
