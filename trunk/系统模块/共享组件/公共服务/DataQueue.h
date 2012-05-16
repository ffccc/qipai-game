#ifndef DATA_QUEUE_HEAD_FILE
#define DATA_QUEUE_HEAD_FILE

#pragma once

#include "ComService.h"

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������ͷ
struct tagDataHead
{
	WORD							wDataSize;							//���ݴ�С
	WORD							wIdentifier;						//���ͱ�ʶ
};

//������Ϣ
struct tagBurthenInfo
{
	DWORD							dwDataSize;							//���ݴ�С
	DWORD							dwBufferSize;						//���峤��
	DWORD							dwDataPacketCount;					//���ݰ���
};

//////////////////////////////////////////////////////////////////////////

//���ݶ���
class COM_SERVICE_CLASS CDataQueue
{
	//��ѯ����
protected:
	DWORD							m_dwInsertPos;						//����λ��
	DWORD							m_dwTerminalPos;					//����λ��
	DWORD							m_dwDataQueryPos;					//��ѯλ��

	//���ݱ���
protected:
	DWORD							m_dwDataSize;						//���ݴ�С
	DWORD							m_dwDataPacketCount;				//���ݰ���

	//�������
protected:
	DWORD							m_dwBufferSize;						//���峤��
	LPBYTE							m_pDataQueueBuffer;					//����ָ��

	//��������
public:
	//���캯��
	CDataQueue();
	//��������
	virtual ~CDataQueue();

	//���ܺ���
public:
	//������Ϣ
	bool GetBurthenInfo(tagBurthenInfo & BurthenInfo);

	//���ݹ���
public:
	//ɾ������
	VOID RemoveData(bool bFreeMemroy);
	//��������
	bool InsertData(WORD wIdentifier, VOID * const pBuffer, WORD wDataSize);
	//��ȡ����
	bool DistillData(tagDataHead & DataHead, VOID * pBuffer, WORD wBufferSize);
};

//////////////////////////////////////////////////////////////////////////

#endif