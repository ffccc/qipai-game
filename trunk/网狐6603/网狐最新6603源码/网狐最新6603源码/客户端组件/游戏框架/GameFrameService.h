#ifndef GAME_FRAME_SERVIE_HEAD_FILE
#define GAME_FRAME_SERVIE_HEAD_FILE

#pragma once

#include "GameFrameWnd.h"
#include "GameFrameView.h"
#include "GameFrameEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//��ܷ���
class GAME_FRAME_CLASS CGameFrameService : public IGameFrameService, public IUserEventSink,
	public IClientKernelSink
{
	//����ӿ�
protected:
	IGameFrameWnd *					m_pIGameFrameWnd;					//��ܽӿ�
	IGameFrameControl *				m_pIGameFrameControl;				//���ƽӿ�

	//����ӿ�
protected:
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	CGameFrameView *				m_pGameFrameView;					//��Ϸ����
	CGameFrameEngine *				m_pGameFrameEngine;					//�������

	//��������
public:
	//���캯��
	CGameFrameService();
	//��������
	virtual ~CGameFrameService();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//������Ϸ
	virtual VOID ResetGameClient();
	//�ر���Ϸ
	virtual VOID CloseGameClient();

	//���ýӿ�
public:
	//�������
	virtual bool OnSchemeFinish();
	//�б�����
	virtual bool SchemeColumnItem(tagColumnItem ColumnItem[], BYTE cbColumnCount);
	//��������
	virtual bool SchemePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount);

	//�ں��¼�
public:
	//��Ϸ��Ϣ
	virtual bool OnEventSocket(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventProcess(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�ͷ��
	virtual VOID OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//���ܺ���
public:
	//��������
	bool CGameFrameService::CreateGameFrameService();
};

//////////////////////////////////////////////////////////////////////////////////

#endif