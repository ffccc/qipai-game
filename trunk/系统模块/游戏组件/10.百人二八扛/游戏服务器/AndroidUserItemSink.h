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
	LONGLONG							m_lMeMaxScore;						//�����ע
	LONGLONG							m_lAreaLimitScore;					//��������
	LONGLONG							m_lApplyBankerCondition;			//��������

	//��ע��Ϣ
protected:
	//ȫ����ע
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];		//ȫ����ע
	//�����ע
	LONGLONG						m_lMeUserJettonScore[AREA_COUNT+1];		//������ע
	//�˿���Ϣ
	BYTE							m_cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE];				//�����˿�
	BYTE							m_cbLeftCardCount;						//�˿���Ŀ

	//�ɼ�����
	LONGLONG						m_lEndBankerScore;					//ׯ�ҳɼ�
	LONGLONG						m_lEndUserScore;					//��ҳɼ�
	LONGLONG						m_lEndUserReturnScore;				//���ػ���
	LONGLONG						m_lEndRevenue;						//��Ϸ˰��

	//�ɼ�����
	LONGLONG						m_lMeStatisticScore;				//��ҳɼ�

	//��ׯ��Ϣ
protected:
	INT								m_nMeBankerCount;					//��ׯ����
	INT								m_nMePlayerCount;					//�мҴ���

	//ׯ����Ϣ
protected:
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��
	WORD							m_cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						m_lBankerWinScore;					//ׯ�ҳɼ�

	//�����б�
protected:
	//static CArrayTemplate<WORD>		m_AppyBankerAdroidUserArray;		//�������
	int								m_AppyBankerAdroidUserCount;		//��������
	//���Ʊ���
protected:
	bool							m_bEnableAndroidUserBanker;			//������ׯ
	bool							m_bEnableSysBanker;					//����ϵͳ��ׯ
	bool							m_bAppyBanker;						//�Ƿ�������

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
	virtual bool __cdecl IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CAndroidUserItemSink)) ? true : false;
	}
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
	//�����ע
	void AndroidUserPlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//������ׯ
	void RandomApplyBanker();

	//��Ϸ��Ϣ
private:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ������
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//������ׯʧ��
	//bool OnSubUserApplyFail(const void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif
