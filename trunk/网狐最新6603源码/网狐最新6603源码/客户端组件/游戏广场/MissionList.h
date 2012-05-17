#ifndef MISSION_LIST_HEAD_FILE
#define MISSION_LIST_HEAD_FILE

#pragma once

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CMap<WORD,WORD,WORD,WORD>	CKindIDMap;							//���ͱ�ʶ

//////////////////////////////////////////////////////////////////////////////////

//�б�����
class CMissionList : public CWnd, public CMissionItem
{
	//��������
protected:
	bool							m_bUpdateOnLine;					//��������
	CKindIDMap						m_KindIDMission;					//��Ϸ��ʶ
	CKindIDMap						m_KindIDPending;					//������ʶ

	//��̬����
protected:
	static CMissionList *			m_pMissionList;						//����ָ��

	//��������
public:
	//���캯��
	CMissionList();
	//��������
	virtual ~CMissionList();

	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	//��������
	VOID UpdateOnLineInfo();
	//��������
	VOID UpdateServerInfo(WORD wKindID);

	//��̬����
public:
	//��ȡ����
	static CMissionList * GetInstance() { return m_pMissionList; }

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif