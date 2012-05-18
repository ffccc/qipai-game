#ifndef DOWN_LOAD_HEAD_FILE
#define DOWN_LOAD_HEAD_FILE

#pragma once

#include "WEBSocket.h"
#include "HeadTranslate.h"

//////////////////////////////////////////////////////////////////////////////////

//��վ����
class DOWN_LOAD_CLASS CDownLoad : public IWEBSocketSink
{
	//�ļ���Ϣ
protected:
	DWORD							m_dwTotalSize;						//�ļ���С
	DWORD							m_dwDownLoadSize;					//���ش�С

	//״̬����
protected:
	tagDownLoadInfo					m_DownLoadInfo;						//������Ϣ
	enDownLoadStatus				m_DownLoadStatus;					//����״̬

	//�������
protected:
	CWEBSocket						m_WEBSocket;						//��������
	CHeadTranslate					m_HeadTranslate;					//��ͷ����

	//�ӿڱ���
protected:
	IDownLoadSink *					m_pIDownLoadSink;					//״̬�ص�

	//��������
public:
	//���캯��
	CDownLoad();
	//��������
	virtual ~CDownLoad();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//״̬�ӿ�
protected:
	//������Ϣ
	virtual bool OnSocketLink(INT nErrorCode);
	//�ر���Ϣ
	virtual bool OnSocketShut(BYTE cbShutReason);

	//���ݽӿ�
protected:
	//���ݰ���
	virtual VOID OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize);
	//���ݰ�ͷ
	virtual VOID OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode);

	//��Ϣ����
public:
	//�ļ���С
	DWORD GetTotalSize() { return m_dwTotalSize; }
	//���ش�С
	DWORD GetDownLoadSize() { return m_dwDownLoadSize; }
	//����״̬
	enDownLoadStatus GetDownLoadStatus() { return m_DownLoadStatus; }

	//���ܺ���
public:
	//�ر�����
	bool CloseDownLoad();
	//ִ������
	bool PerformDownLoad(LPCTSTR pszUrl, IUnknownEx * pIUnknownEx);

	//��������
protected:
	//��������
	bool ConnectServer(LPCTSTR pszUrl);
	//��������
	bool SendRequestData(LPCTSTR pszHostName, LPCTSTR pszUrlPath);
	//��ȡ��Ϣ
	INTERNET_SCHEME GetDownLoadInfo(LPCTSTR pszUrl, tagDownLoadInfo & DownLoadInfo);
};

//////////////////////////////////////////////////////////////////////////////////

#endif