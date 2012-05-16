#ifndef ANDROID_USER_MANAGER_HEAD_FILE
#define ANDROID_USER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "AndroidUserItem.h"
#include "GameServiceExport.h"

//////////////////////////////////////////////////////////////////////////

//��������
typedef CArrayTemplate<CAndroidUserItem *> CAndroidUserItemArray;
typedef CMap<DWORD,DWORD,CAndroidUserItem *, CAndroidUserItem *> CAndroidUserItemMap;

//////////////////////////////////////////////////////////////////////////

//�����˹���
class CAndroidUserManager : public IAndroidUserManager
{
	//��Ԫ����
	friend class CAttemperEngineSink;

	//���ñ���
protected:
	WORD							m_wAutoAndroidCount;				//�Զ���Ŀ

	//������
protected:
	WORD							m_wStockCount;						//�����Ŀ
	DWORD							m_dwAndroidStock[MAX_ANDROID];		//����û�

	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IServerUserManager *			m_pIServerUserManager;				//�û�����
	IGameServiceManager *			m_pIGameServiceManager;				//�������
	ITCPNetworkEngineEvent *		m_pITCPNetworkEngineEvent;			//����ӿ�

	//ϵͳ����
protected:
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//�û�����
protected:
	CAndroidUserItemMap				m_AndroidUserItemMap;				//�û�����
	CAndroidUserItemArray			m_AndroidUserItemFree;				//���ж���
	CAndroidUserItemArray			m_AndroidUserItemStorage;			//�洢����

	//��������
public:
	//���캯��
	CAndroidUserManager();
	//��������
	virtual ~CAndroidUserManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//ֹͣ����
	virtual bool __cdecl StopService();
	//��������
	virtual bool __cdecl StartService();

	//���ýӿ�
public:
	//���ÿ��
	virtual bool __cdecl SetAndroidStock(DWORD dwAndroidStock[], WORD wStockCount);

	//����ӿ�
public:
	//ɾ��������
	virtual bool __cdecl DeleteAndroidUserItem(DWORD dwAndroidID);
	//�л�������
	virtual IAndroidUserItem * __cdecl SwitchAndroidUserItem(IServerUserItem * pIServerUserItem);
	//����������
	virtual IAndroidUserItem * __cdecl CreateAndroidUserItem(DWORD dwUserID, TCHAR szPassword[PASS_LEN]);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool __cdecl OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�������
protected:
	//��ȡ����
	CAndroidUserItem * GetAndroidUserItem(WORD wIndex);
	//�������
	CAndroidUserItem * ActiveAndroidUserItem(DWORD dwUserID);
	//�ͷŶ���
	bool FreeAndroidUserItem(CAndroidUserItem * pAndroidUserItem);
};

//////////////////////////////////////////////////////////////////////////

#endif