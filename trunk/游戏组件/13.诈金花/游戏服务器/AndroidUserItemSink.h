#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	WORD							m_wCurrentUser;							//��ǰ�û�

	//�û�״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	LONG							m_lTableScore[GAME_PLAYER];				//��ע��Ŀ

	//��ע��Ϣ
protected:
	LONG							m_lMaxScore;							//�ⶥ��Ŀ
	LONG							m_lCellScore;							//��Ԫ��ע
	LONG							m_lCurrentTimes;						//��ǰ����
	LONG							m_lUserMaxScore;						//������
	bool							m_bMingZhu[GAME_PLAYER];				//���ƶ���

	//�ؼ�����
public:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;					//�û��ӿ�
	
	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
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
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubLookCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubCompareCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//�û�ǿ��
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif
