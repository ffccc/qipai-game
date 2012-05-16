#ifndef TCP_SOCKET_HEAD_FILE
#define TCP_SOCKET_HEAD_FILE

#pragma once

#include "WinSock2.h"
#include "NetworkServiceHead.h"

//////////////////////////////////////////////////////////////////////////

//TCP ��������
class CTCPSocket : public CWnd, public ITCPSocket
{
	//��������
protected:
	WORD							m_wSocketID;						//�����ʶ
	BYTE							m_cbSocketStatus;					//����״̬

	//�ں˱���
protected:
	SOCKET							m_hSocket;							//���Ӿ��
	ITCPSocketSink *				m_pITCPSocketSink;					//�ص��ӿ�

	//������Ϣ
protected:
	enProxyServerType				m_ProxyServerType;					//��������
	tagProxyServerInfo				m_ProxyServerInfo;					//������Ϣ

	//���ձ���
protected:
	WORD							m_wRecvSize;						//���ճ���
	BYTE							m_cbRecvBuf[SOCKET_BUFFER*10];		//���ջ���

	//�������
protected:
	bool							m_bNeedBuffer;						//����״̬
	DWORD							m_dwBufferData;						//��������
	DWORD							m_dwBufferSize;						//�����С
	LPBYTE							m_pcbDataBuffer;					//��������

	//��������
protected:
	BYTE							m_cbSendRound;						//�ֽ�ӳ��
	BYTE							m_cbRecvRound;						//�ֽ�ӳ��
	DWORD							m_dwSendXorKey;						//������Կ
	DWORD							m_dwRecvXorKey;						//������Կ

	//��������
protected:
	DWORD							m_dwSendTickCount;					//����ʱ��
	DWORD							m_dwRecvTickCount;					//����ʱ��
	DWORD							m_dwSendPacketCount;				//���ͼ���
	DWORD							m_dwRecvPacketCount;				//���ܼ���

	//��������
public:
	//���캯��
	CTCPSocket();
	//��������
	virtual ~CTCPSocket();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//��ʶ�ӿ�
public:
	//��ȡ��ʶ
	virtual WORD __cdecl GetSocketID() { return m_wSocketID;}
	//���ñ�ʶ
	virtual VOID __cdecl SetSocketID(WORD wSocketID) { m_wSocketID=wSocketID; }

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool __cdecl SetTCPSocketSink(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual VOID * __cdecl GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer);

	//��Ϣ�ӿ�
public:
	//��ȡ״̬
	virtual BYTE __cdecl GetSocketStatus() { return m_cbSocketStatus; }
	//���ͼ��
	virtual DWORD __cdecl GetLastSendTick() { return m_dwSendTickCount; }
	//���ռ��
	virtual DWORD __cdecl GetLastRecvTick() { return m_dwRecvTickCount; }
	//������Ŀ
	virtual DWORD __cdecl GetSendPacketCount() { return m_dwSendPacketCount; }
	//������Ŀ
	virtual DWORD __cdecl GetRecvPacketCount() { return m_dwRecvPacketCount; }

	//����ӿ�
public:
	//�������
	virtual DWORD __cdecl ProxyServerTesting();
	//���ô���
	virtual bool __cdecl SetProxyServerInfo(enProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo);

	//�����ӿ�
public:
	//�ر�����
	virtual VOID __cdecl CloseSocket();
	//���Ӳ���
	virtual DWORD __cdecl Connect(DWORD dwServerIP, WORD wPort);
	//���Ӳ���
	virtual DWORD __cdecl Connect(LPCTSTR szServerIP, WORD wPort);
	//���ͺ���
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	//��������
protected:
	//���Ӵ���
	DWORD ConnectProxyServer();
	//��ַ����
	DWORD TranslateAddr(LPCTSTR pszServerAddr);

	//��������
protected:
	//�ر�����
	VOID CloseSocket(BYTE cbShutReason);
	//��������
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);
	//��������
	DWORD SendDataBuffer(VOID * pBuffer, WORD wSendSize);

	//���ܺ���
protected:
	//��������
	WORD CrevasseBuffer(BYTE cbDataBuffer[], WORD wDataSize);
	//��������
	WORD EncryptBuffer(BYTE cbDataBuffer[], WORD wDataSize, WORD wBufferSize);

	//��������
private:
	//�ֽ�ӳ��
	inline WORD SeedRandMap(WORD wSeed);
	//����ӳ��
	inline BYTE MapSendByte(BYTE cbData);
	//����ӳ��
	inline BYTE MapRecvByte(BYTE cbData);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//������Ϣ
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	//��Ϣ����
protected:
	//�����ȡ
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//���緢��
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//����ر�
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif