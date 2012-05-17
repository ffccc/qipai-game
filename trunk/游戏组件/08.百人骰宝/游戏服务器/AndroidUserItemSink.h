#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../��Ϣ����/GameLogic.h"

//////////////////////////////////////////////////////////////////////////
typedef CArrayTemplate<DWORD,DWORD&> CApplyBankerArray;

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	int iRandLimit[7];
	//������Ϣ
protected:
	__int64							m_lMeMaxScore;						//�����ע
	__int64                         m_lAllMaxScore;                     // ���ֿ�����ע����ע
	__int64							m_lAreaLimitScore;					//��������
	__int64							m_lApplyBankerCondition;			//��������
	bool                            m_bEnableNote;                      // �Ƿ������ע
	bool m_bEnableSysBanker;  // �Ƿ����ϵͳ��ׯ

	//������ע
protected:
	__int64							m_lMeScore[COUNT_AZIMUTH];          //ÿ����λ��ע
	__int64                         m_lAllScore[COUNT_AZIMUTH];			//ÿ����λ��ע
	//ׯ����Ϣ
protected:
	__int64							m_lBankerScore;						//ׯ�һ���
	WORD							m_wCurrentBanker;					//��ǰׯ��

	CApplyBankerArray               m_arApplyBanker;                    // ����ׯ�Ҷ���
	int m_iMaxAndroidBanker;                                           // ��������������

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

	// ������Ҷ���ʼ��
	virtual void __cdecl OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	// ��ע����
	bool OnPlaceJetton(void);

	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);

	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);

	// �����ע
	__int64 GetMaxAndroidScore(int iArea);
};

//////////////////////////////////////////////////////////////////////////

#endif
