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
	BYTE                            m_cbScene;
	int								m_nFishGold;						//���
	LONG						m_lCellScore;						//����
	BYTE							m_CannonType;						//�ӵ�����
	BYTE							m_SwapCannonType;					//Ҫ�л����ӵ�����
	float							m_fLastFireRote;					//��һ�ε��ӵ��Ƕ�
	
    //WORD                            m_wSceneSendFishCount;

    Role_Net_Object                m_RoleObjects[GAME_PLAYER];			//��ɫ��¼
    //Fish_Net_Object                m_FishObjects[MAX_FISH_OBJECT];

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
	virtual void __cdecl Release() { return ; }
	//virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }//��
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

//	//��Ϣ����
//protected:
//
//	//��Ϸ��ʼ
//	bool OnSubStart(const void * pBuffer, WORD wDataSize);

	//�ڲ�����
private:
	/// �����ӵ�
	void BuyBullet();
	/// �л��ӵ�
	void ChangeCannon();
	/// �л��ӵ�
	void ChangeCannonOnce();
	/// �����ӵ�
	void Fire();
};

//////////////////////////////////////////////////////////////////////////

#endif