#ifndef TABLE_FRAME_HEAD_FILE
#define TABLE_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

//�û��ӿ�����
typedef CArrayTemplate<IServerUserItem *> CUserItemPtrArray;

//��Ϸ���ӿ��
class CTableFrame : public ITableFrameManager, public ITableFrameControl
{
	//���Ա���
protected:
	WORD							m_wTableID;							//���Ӻ���
	WORD							m_wChairCount;						//������Ŀ

	//״̬����
protected:
	bool							m_bTableLocked;						//������־
	bool							m_bGameStarted;						//��ʼ��־

	//״̬����
protected:
	BYTE							m_bGameStatus;						//��Ϸ״̬
	DWORD							m_dwTimeStart;						//��ʼʱ��
	TCHAR							m_szPassword[PASS_LEN];				//��������

	//������Ϣ
protected:
	LONG							m_lGameTaxScore;					//��Ϸ˰��
	DWORD							m_dwPlayerID[MAX_CHAIR];			//��� I D
	tagScoreInfo					m_ScoreInfo[MAX_CHAIR];				//��Ϸ����

	//�û���Ϣ
protected:
	bool							m_bAllowLookon[MAX_CHAIR];			//�Թ۱�־
	WORD							m_wOffLineCount[MAX_CHAIR];			//���ߴ���
	IServerUserItem	*				m_pIUserItem[MAX_CHAIR];			//�û�ָ��
	CUserItemPtrArray				m_LookonUserItemPtr;				//�Թ��û�

	//��������
protected:
	CMap<DWORD,DWORD,DWORD,DWORD>	m_ClientReadyUser;					//����û�

	//������Ϣ
protected:
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//��������
	const tagGameServiceOption		* m_pGameServiceOption;				//��������

	//����ӿ�
public:
	ITableFrameSink					* m_pITableFrameSink;				//���ӽӿ�
	ITableUserAction				* m_pITableUserAction;				//�¼��ӿ�
	IGameServiceFrame				* m_pIGameServiceFrame;				//���ܽӿ�

	//��������
public:
	//���캯��
	CTableFrame();
	//��������
	virtual ~CTableFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrame(WORD wTableID, tagTableFrameParameter * pTableFrameParameter);

	//��Ϣ�ӿ�
public:
	//���Ӻ���
	virtual WORD __cdecl GetTableID() { return m_wTableID; }
	//��Ϸ����
	virtual WORD __cdecl GetChairCount() { return m_wChairCount; }
	//��Ϸʱ��
	virtual DWORD __cdecl GetPlayTimeCount() { return (DWORD)time(NULL)-m_dwTimeStart; }

	//�û��ӿ�
public:
	//ö���û�
	virtual IServerUserItem * __cdecl EnumLookonUserItem(WORD wIndex);
	//��ȡ�û�
	virtual IServerUserItem * __cdecl GetServerUserItem(WORD wChairID);

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE __cdecl GetGameStatus() { return m_bGameStatus; }
	//����״̬
	virtual void __cdecl SetGameStatus(BYTE bGameStatus) { m_bGameStatus=bGameStatus; }
	//��ȡ����
	virtual const tagGameServiceOption * __cdecl GetGameServiceOption() { return m_pGameServiceOption; }

	//��ʱ���ӿ�
public:
	//���ö�ʱ��
	virtual bool __cdecl SetGameTimer(DWORD DwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam);
	//ɾ����ʱ��
	virtual bool __cdecl KillGameTimer(DWORD DwTimerID);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID);
	//��������
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize);

	//��Ϣ�ӿ�
public:
	//���ͷ�����Ϣ
	virtual bool __cdecl SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//������Ϸ��Ϣ
	virtual bool __cdecl SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);

	//��Ϸ�ӿ�
public:
	//��ɢ��Ϸ
	virtual bool __cdecl DismissGame();
	//������Ϸ
	virtual bool __cdecl ConcludeGame();
	//���ͳ���
	virtual bool __cdecl SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize);

	//д�ֽӿ�
public:
	//д�����
	virtual bool __cdecl WriteUserScore(WORD wChairID, LONG lScore, LONG lRevenue, enScoreKind ScoreKind, LONG lPlayTimeCount=-1);
	//д�����
	virtual bool __cdecl WriteUserScore(IServerUserItem * pIServerUserItem, LONG lScore, LONG lRevenue, enScoreKind ScoreKind, LONG lPlayTimeCount=-1);

	//ִ�нӿ�
public:
	//���¶���
	virtual bool __cdecl PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem,LPCTSTR szPassword);
	//�Թ۶���
	virtual bool __cdecl PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem,LPCTSTR szPassword);
	//�뿪����
	virtual bool __cdecl PerformStandUpAction(IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�����¼�
	virtual bool __cdecl OnUserOffLine(WORD wChairID);
	//�ؽ��¼�
	virtual bool __cdecl OnUserReConnect(WORD wChairID);
	//�������
	virtual bool __cdecl OnUserReqOffLine(WORD wChairID);

	//ϵͳ�¼�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//��Ϸ�¼�����
	virtual bool __cdecl OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//����¼�����
	virtual bool __cdecl OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//��Ƶ�¼�����
	virtual bool __cdecl OnEventSocketVideo(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual bool __cdecl IsTableLocked() { return m_bTableLocked; }
	//��Ϸ״̬
	virtual bool __cdecl IsGameStarted() { return m_bGameStarted; }
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(IServerUserItem * pIServerUserItem);

	//���ƽӿ�
private:
	//��ʼ��Ϸ
	virtual bool __cdecl StartGame();
	//��ʼ�ж�
	virtual bool __cdecl StartVerdict();
	//����λ��
	virtual bool __cdecl SwitchUserChair(WORD wSourceID[], WORD wTargetID[], WORD wSwitchCount);

	//��������
public:
	//��ȡ��λ
	WORD GetNullChairID();
	//�Ƿ����
	bool IsClientReady(IServerUserItem * pIServerUserItem);
	//��������ʧ��
	void SendSitFailedPacket(IServerUserItem * pIServerUserItem, LPCTSTR pszFailedDescribe);
};

//////////////////////////////////////////////////////////////////////////

#endif