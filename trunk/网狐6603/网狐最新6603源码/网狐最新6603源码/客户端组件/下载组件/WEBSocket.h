#ifndef DOWN_LOAD_SOCKET_HEAD_FILE
#define DOWN_LOAD_SOCKET_HEAD_FILE

#pragma once

#include "WinSock2.h"
#include "DownLoadHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class DOWN_LOAD_CLASS CWEBSocket : public CWnd
{
	//״̬����
protected:
	WORD							m_wRequestID;						//�����ʶ
	bool							m_bHeadValid;						//��Ч��־
	BYTE							m_cbSocketStatus;					//����״̬

	//���ݰ�ͷ
protected:
	WORD							m_wHeadSize;						//��ͷ����
	CHAR							m_szHeadData[1024];					//���ݰ�ͷ

	//�������
protected:
	bool							m_bNeedBuffer;						//����״̬
	DWORD							m_dwBufferData;						//��������
	DWORD							m_dwBufferSize;						//�����С
	LPBYTE							m_pcbDataBuffer;					//��������

	//�ں˱���
protected:
	SOCKET							m_hSocket;							//���Ӿ��
	IWEBSocketSink *				m_pIWEBSocketSink;					//�ص��ӿ�

	//������Ϣ
protected:
	BYTE							m_cbProxyType;						//��������
	tagProxyServer					m_ProxyServer;						//������Ϣ

	//��������
public:
	//���캯��
	CWEBSocket();
	//��������
	virtual ~CWEBSocket();

	//���ú���
public:
	//���ýӿ�
	bool SetWEBSocketSink(IUnknownEx * pIUnknownEx);
	//���ô���
	bool SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer);

	//�����ӿ�
public:
	//�ر�����
	VOID CloseSocket(BYTE cbShutReason);
	//���Ӳ���
	BYTE ConnectServer(LPCTSTR pszServer, WORD wPort);
	//��������
	WORD SendRequestData(VOID * pData, WORD wDataSize);

	//��������
protected:
	//���Ӵ���
	BYTE ConnectProxyServer();
	//��������
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);

	//������
protected:
	//�����ȡ
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//���緢��
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//����ر�
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	//��Ϣӳ��
protected:
	//������Ϣ
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif