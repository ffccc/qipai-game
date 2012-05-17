#ifndef PROCESS_MANAGER_HEAD_FILE
#define PROCESS_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////////////

//���̹���
class CProcessManager : public CWnd, public IProcessManager, public IProcessControlSink
{
	//�ں˱���
protected:
	PROCESS_INFORMATION				m_ProcessInfo;						//������Ϣ
	IProcessManagerSink *			m_pIProcessManagerSink;				//�ص��ӿ�

	//ͨѶ����
protected:
	HWND							m_hProcessControl;					//ͨѶ����
	CProcessControlHelper			m_ProcessControlModule;				//���̴���

	//��������
public:
	//���캯��
	CProcessManager();
	//��������
	virtual ~CProcessManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ݽӿ�
public:
	//��������
	virtual bool OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend);

	//���ƺ���
public:
	//�������
	bool ActiveGameProcess();
	//�رս���
	bool CloseGameProcess(bool bPerformSink);
	//�������
	bool DetachGameProcess(bool bPerformSink);
	//��������
	bool CreateGameProcess(LPCTSTR pszProcessName);
	//���ýӿ�
	bool SetProcessManagerSink(IUnknownEx * pIUnknownEx);

	//��������
public:
	//��������
	bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID);
	//��������
	bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//ִ������
	bool SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//ϵͳ����
public:
	//�ȼ���Ϣ
	bool SendLevelInfo(tagLevelItem LevelItem[], BYTE cbLevelCount);
	//�б���Ϣ
	bool SendColumnInfo(tagColumnItem ColumnItem[], BYTE cbColumnCount);
	//������Ϣ
	bool SendPropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount);
	//������Ϣ
	bool SendServerInfo(IClientUserItem * pIMyselfUserItem, DWORD dwUserRight, DWORD dwMasterRight, DWORD dwServerRule, WORD wServerType, tagGameServer & GameServer);

	//�û�״̬
public:
	//�û�����
	bool SendUserItem(IClientUserItem * pIClientUserItem);
	//�û�����
	bool SendUserScore(DWORD dwUserID, tagUserScore & UserScore);
	//�û�״̬
	bool SendUserStatus(DWORD dwUserID, tagUserStatus & UserStatus);
	//�û�ͷ��
	bool SendCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo * pCustomFaceInfo);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif