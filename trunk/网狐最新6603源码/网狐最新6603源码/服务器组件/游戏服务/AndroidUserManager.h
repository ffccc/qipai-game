#ifndef ANDROID_USER_MANAGER_HEAD_FILE
#define ANDROID_USER_MANAGER_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "AndroidUserItem.h"
#include "GameServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
typedef CWHArray<CAndroidUserItem *> CAndroidUserItemArray;
typedef CMap<DWORD,DWORD,CAndroidUserItem *, CAndroidUserItem *> CAndroidUserItemMap;

//////////////////////////////////////////////////////////////////////////////////

//�����˹���
class GAME_SERVICE_CLASS CAndroidUserManager : public IAndroidUserManager
{
	//���ñ���
protected:
	WORD							m_wAutoAndroidCount;				//�Զ���Ŀ

	//������
protected:
	WORD							m_wStockCount;						//�����Ŀ
	tagAndroidParameter				m_AndroidParameter[MAX_ANDROID];	//����û�

	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IServerUserManager *			m_pIServerUserManager;				//�û�����
	IGameServiceManager *			m_pIGameServiceManager;				//�������
	ITCPNetworkEngineEvent *		m_pITCPNetworkEngineEvent;			//�¼��ӿ�

	//ϵͳ����
protected:
	tagGameParameter *				m_pGameParameter;					//���ò���
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//�û�����
protected:
	CAndroidUserItemMap				m_AndroidUserItemMap;				//�û�����
	CAndroidUserItemArray			m_AndroidUserItemArray;				//�洢����
	CAndroidUserItemArray			m_AndroidUserItemBuffer;			//���ж���

	//��������
public:
	//���캯��
	CAndroidUserManager();
	//��������
	virtual ~CAndroidUserManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��������
	virtual bool StartService();
	//ֹͣ����
	virtual bool ConcludeService();

	//״̬�ӿ�
public:
	//�û�״��
	virtual WORD GetAndroidUserInfo(tagAndroidUserInfo & AndroidUserInfo);

	//���ýӿ�
public:
	//�������
	virtual bool InitAndroidUser(tagAndroidUserParameter & AndroidUserParameter);
	//���ÿ��
	virtual bool SetAndroidStock(tagAndroidParameter AndroidParameter[], WORD wStockCount);

	//����ӿ�
public:
	//ɾ������
	virtual bool DeleteAndroidUserItem(DWORD dwAndroidID);
	//���һ���
	virtual IAndroidUserItem * SearchAndroidUserItem(DWORD dwUserID, DWORD dwContextID);
	//��������
	virtual IAndroidUserItem * CreateAndroidUserItem(tagAndroidParameter & AndroidParameter);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnEventTimerPulse(DWORD dwTimerID, WPARAM dwBindParameter);

	//����ӿ�
public:
	//��������
	virtual bool SendDataToClient(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
	virtual bool SendDataToClient(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��������
	virtual bool SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�������
protected:
	//�ͷŶ���
	VOID FreeAndroidUserItem(DWORD dwUserID);
	//��ȡ����
	CAndroidUserItem * GetAndroidUserItem(WORD wIndex);
	//�������
	CAndroidUserItem * ActiveAndroidUserItem(tagAndroidParameter & AndroidParameter);
};

//////////////////////////////////////////////////////////////////////////////////

#endif