#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�����뿪�û�
typedef CArrayTemplate<WORD>	RequestLeaveUserAry;

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//�߼�����
protected:
	BYTE							m_cbPackCount;						//�Ƹ���Ŀ
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	BYTE							m_cbMainValue;						//������ֵ

	//������Ϣ
protected:
	BYTE							m_cbCallCard;						//�����˿�
	BYTE							m_cbCallCount;						//������Ŀ
	WORD							m_wCallCardUser;					//�����û�

	//״̬����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�

	//�÷���Ϣ
protected:
	BYTE							m_cbScoreCardCount;					//������Ŀ
	BYTE							m_cbScoreCardData[12*PACK_COUNT];	//�÷��˿�

	//���Ʊ���
protected:
	WORD							m_wFirstOutUser;					//�����û�
	BYTE							m_cbOutCardCount[4];				//������Ŀ
	BYTE							m_cbOutCardData[4][MAX_COUNT];		//�����б�

	//�����˿�
protected:
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];					//�����˿�

	//�����뿪�û�
	RequestLeaveUserAry				m_RequestUserAry;					

	//�û��˿�
protected:
	BYTE							m_cbHandCardCount;					//�˿���Ŀ
	BYTE							m_cbHandCardData[MAX_COUNT];		//�����˿�	

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

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubCallCard(const void * pBuffer, WORD wDataSize);
	//���͵���
	bool OnSubSendConceal(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGamePlay(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//˦�ƽ��
	bool OnSubThrowResult(const void * pBuffer, WORD wDataSize);
	//һ�ֽ���
	bool OnSubTurnBalance(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�����뿪
	bool OnSubRequestLeave(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubBatchCard(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//����
	bool CallCard();
	//�µ���
	bool SendConceal();
	//����
	bool OutCard();
};

//////////////////////////////////////////////////////////////////////////

#endif