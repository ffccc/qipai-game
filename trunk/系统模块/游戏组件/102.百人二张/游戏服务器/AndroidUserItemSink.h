#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
protected:
	LONGLONG							m_lAreaLimitScore;					//��������
	LONGLONG							m_lUserLimitScore;					//��������
	LONGLONG							m_lApplyBankerCondition;			//��������

	LONGLONG							m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG							m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע

	LONGLONG							m_lMeMaxScore;						//�����ע
	//ׯ����Ϣ
protected:
	LONGLONG							m_lBankerScore;						//ׯ�һ���
	LONGLONG							m_lBankerWinScore;					//ׯ�ҳɼ�
	WORD								m_wBankerUser;						//��ǰׯ��
	WORD								m_wBankerTime;						//��ׯ����

	//BYTE								m_cbLeftCardCount;					//�˿���Ŀ
	bool								m_bEnableSysBanker;					//ϵͳ��ׯ

protected:
	BYTE								m_cbJettonArea;
	WORD								m_wJettonCount;

	bool								m_bMeCurrentBanker;
	bool								m_bApplyingBanker;
	bool								m_bCancelingBanker;
	WORD								m_wRandBankerTime;

	static BOOL							m_bAllowApplyBanker;
	static WORD							m_wMaxBankerTime;
	static INT							m_nMaxJettonRange;

	static CString						m_strConfigFile;
	static BOOL							m_bLoadConfig;

		//�ؼ�����
protected:
	CGameLogic							m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *					m_pIAndroidUserItem;				//�û��ӿ�

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
	virtual void __cdecl OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {};
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {};
	//�û�����
	virtual void __cdecl OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
	//�û���λ
	virtual void __cdecl OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//��Ϸ��¼
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	////�˿���
	//bool OnSubSendCard(const void * pBuffer, WORD wDataSize);

protected:
	//�����ע
	LONGLONG GetUserMaxJetton();

	static void LoadConfig();
};

//////////////////////////////////////////////////////////////////////////

#endif
