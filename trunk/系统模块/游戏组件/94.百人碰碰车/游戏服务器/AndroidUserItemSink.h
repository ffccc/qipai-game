#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//�����ע����
#define MAX_CHIP_TIME								50
//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	LONGLONG						m_lMaxChipBanker;					//�����ע (ׯ��)
	LONGLONG						m_lMaxChipUser;						//�����ע (����)
	LONGLONG						m_lAreaChip[AREA_COUNT];			//������ע 
	WORD							m_wCurrentBanker;					//ׯ��λ��

	int								m_nChipLimit[2];					//��ע��Χ (0-AREA_COUNT)
	int								m_nChipTime;						//��ע���� (����)
	int								m_nChipTimeCount;					//���´��� (����)
	
	//��ׯ����
protected:
	bool							m_bMeApplyBanker;					//�����ʶ
	int								m_nBankerCount;						//�������˵���ׯ����
	int								m_nWaitBanker;						//�ռ���
	static int						m_stlApplyBanker;					//������

	//���ñ���  (ȫ������)
protected:
	tagRobotInfo					m_RobotInfo;						//ȫ������
	TCHAR							m_szRoomName[32];					//���÷���

	//���ñ���	(��Ϸ����)
protected:
	bool							m_bRefreshCfg;						//ÿ��ˢ��
	LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lUserLimitScore;					//��ע����
	LONGLONG						m_lBankerCondition;					//��ׯ����		

	//���ñ���  (����������)
protected:
	LONGLONG						m_lRobotJettonLimit[2];				//��������	
	int								m_nRobotBetTimeLimit[2];			//��������	
	bool							m_bRobotBanker;						//�Ƿ���ׯ
	int								m_nRobotBankerCount;				//��ׯ����
	int								m_nRobotWaitBanker;					//��������
	int								m_nRobotApplyBanker;				//��ׯ����
	bool							m_bReduceJettonLimit;				//��������

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
public:
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);

	//���ܺ���
public:
	//��ȡ����
	void ReadConfigInformation(TCHAR szFileName[], TCHAR szRoomName[], bool bReadFresh);
	//���㷶Χ
	bool CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[]);
};

//////////////////////////////////////////////////////////////////////////

#endif
