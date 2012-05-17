#ifndef WH_SOCKET_HEAD_FILE
#define WH_SOCKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"
#include "..\..\�������\�������\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef WH_SOCKET_CLASS
	#ifdef  WH_SOCKET_DLL
		#define WH_SOCKET_CLASS _declspec(dllexport)
	#else
		#define WH_SOCKET_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define WH_SOCKET_DLL_NAME		TEXT("WHSocket.dll")				//�������
#else
	#define WH_SOCKET_DLL_NAME		TEXT("WHSocketD.dll")				//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//��������

//�ر�ԭ��
#define SHUT_REASON_INSIDE			0									//�ڲ�ԭ��
#define SHUT_REASON_NORMAL			1									//�����ر�
#define SHUT_REASON_REMOTE			2									//Զ�̹ر�
#define SHUT_REASON_TIME_OUT		3									//���糬ʱ
#define SHUT_REASON_EXCEPTION		4									//�쳣�ر�

//////////////////////////////////////////////////////////////////////////////////
//��������

//����״̬
#define SOCKET_STATUS_IDLE			0									//����״̬
#define SOCKET_STATUS_WAIT			1									//�ȴ�״̬
#define SOCKET_STATUS_CONNECT		2									//����״̬

//���Ӵ���
#define CONNECT_SUCCESS				0									//���ӳɹ�
#define CONNECT_FAILURE				1									//����ʧ��
#define CONNECT_EXCEPTION			2									//�����쳣
#define CONNECT_PROXY_FAILURE		3									//���ӳɹ�
#define CONNECT_PROXY_USER_INVALID	4									//�û���Ч

//��������
#define PROXY_NONE					0									//û�д���
#define PROXY_HTTP					1									//��������
#define PROXY_SOCKS4				2									//��������
#define PROXY_SOCKS5				3									//��������

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagProxyDescribe
{
	BYTE							cbProxyType;						//��������
	TCHAR							szDescribe[32];						//�����ַ�
};

//������Ϣ
struct tagProxyServer
{
	WORD							wProxyPort;							//����˿�
	TCHAR							szUserName[32];						//�����û�
	TCHAR							szPassword[32];						//��������
	TCHAR							szProxyServer[32];					//�����ַ
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITCPSocket INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocket={0xa870d592,0x447a,0x40bd,0x00ae,0x4e,0x5e,0x64,0xde,0xce,0x22,0xed};
#else
	#define VER_ITCPSocket INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocket={0x8dfd434a,0xfb21,0x4178,0x00b0,0x3b,0xe6,0x32,0x9f,0x0b,0x5b,0x2e};
#endif

//����ӿ�
interface ITCPSocket : public IUnknownEx
{
	//���ýӿ�
public:
	//���ñ�ʶ
	virtual VOID SetSocketID(WORD wSocketID)=NULL;
	//���ýӿ�
	virtual bool SetTCPSocketSink(IUnknownEx * pIUnknownEx)=NULL;

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual BYTE GetSocketStatus()=NULL;
	//���ͼ��
	virtual DWORD GetLastSendTick()=NULL;
	//���ռ��
	virtual DWORD GetLastRecvTick()=NULL;

	//����ӿ�
public:
	//�������
	virtual BYTE ProxyServerTesting()=NULL;
	//���ô���
	virtual bool SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer)=NULL;

	//�����ӿ�
public:
	//�ر�����
	virtual VOID CloseSocket()=NULL;
	//���Ӳ���
	virtual BYTE Connect(DWORD dwServerIP, WORD wPort)=NULL;
	//���Ӳ���
	virtual BYTE Connect(LPCTSTR pszServerIP, WORD wPort)=NULL;
	//���ͺ���
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITCPSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocketSink={0xdb0f09dd,0x65e4,0x441f,0x0094,0xb1,0xfe,0x80,0x8c,0x1d,0xf2,0x65};
#else
	#define VER_ITCPSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocketSink={0x231416e8,0x74b0,0x472d,0x009e,0xc3,0x4a,0x22,0xe5,0x21,0x09,0x37};
#endif

//����ӿ�
interface ITCPSocketSink : public IUnknownEx
{
	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)=NULL;
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)=NULL;
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUDPSocket INTERFACE_VERSION(1,1)
	static const GUID IID_IUDPSocket={0x11e344d0,0xace1,0x4a72,0x00ab,0xac,0x6f,0x31,0x13,0xbf,0xc9,0x23};
#else
	#define VER_IUDPSocket INTERFACE_VERSION(1,1)
	static const GUID IID_IUDPSocket={0x40173211,0x7389,0x4fde,0x0085,0x4e,0xd2,0xaa,0x83,0xa6,0x0c,0x6d};
#endif

//����ӿ�
interface IUDPSocket : public IUnknownEx
{
	//���ýӿ�
public:
	//���ñ�ʶ
	virtual VOID SetSocketID(WORD wSocketID)=NULL;
	//���ýӿ�
	virtual bool SetUDPSocketSink(IUnknownEx * pIUnknownEx)=NULL;

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual BYTE GetSocketStatus()=NULL;
	//���ͼ��
	virtual DWORD GetLastSendTick()=NULL;
	//���ռ��
	virtual DWORD GetLastRecvTick()=NULL;

	//�����ӿ�
public:
	//�ر�����
	virtual VOID CloseSocket()=NULL;
	//���Ӳ���
	virtual BYTE Connect(DWORD dwServerIP, WORD wPort)=NULL;
	//���Ӳ���
	virtual BYTE Connect(LPCTSTR pszServerIP, WORD wPort)=NULL;
	//���ͺ���
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUDPSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUDPSocketSink={0x28d0360d,0x8bf7,0x4e15,0x00ae,0x94,0x82,0x37,0x97,0x46,0x89,0xfa};
#else
	#define VER_IUDPSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUDPSocketSink={0xed1f538e,0x2d13,0x4003,0x0083,0x54,0x0e,0xf2,0xe2,0x15,0xae,0x08};
#endif

//����ӿ�
interface IUDPSocketSink : public IUnknownEx
{
	//�����¼�
public:
	//�ر��¼�
	virtual bool OnEventUDPSocketShut(WORD wSocketID, BYTE cbShutReason)=NULL;
	//��ȡ�¼�
	virtual bool OnEventUDPSocketRead(WORD wSocketID, DWORD dwSenderIP, UDP_Command Command, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IWHNetwork INTERFACE_VERSION(1,1)
	static const GUID IID_IWHNetwork={0xe500997b,0xe45e,0x474e,0x8a,0xd4,0xbc,0xf3,0x4d,0x89,0x45,0x41};
#else
	#define VER_IWHNetwork INTERFACE_VERSION(1,1)
	static const GUID IID_IWHNetwork={0x32198885,0x0c13,0x4ea8,0x8a,0x36,0x27,0x56,0x51,0x36,0xac,0xc1};
#endif

//����ӿ�
interface IWHNetwork : public IUnknownEx
{
	//���ܺ���
public:
	//��ַת��
	virtual DWORD ConversionAddr(LPCTSTR pszAddress)=NULL;
	//��ȡ��ַ
	virtual DWORD GetLocalNetworkIP(DWORD dwAddrList[], DWORD dwMaxCount)=NULL;

	//����ӿ�
public:
	//ö�ٴ���
	virtual bool EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyDescribe)=NULL;
	//�������
	virtual BYTE ProxyServerTesting(BYTE cbProxyType, const tagProxyServer & ProxyServer)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//���������

DECLARE_MODULE_HELPER(TCPSocket,WH_SOCKET_DLL_NAME,"CreateTCPSocket")
DECLARE_MODULE_HELPER(UDPSocket,WH_SOCKET_DLL_NAME,"CreateUDPSocket")
DECLARE_MODULE_HELPER(WHNetwork,WH_SOCKET_DLL_NAME,"CreateWHNetwork")

//////////////////////////////////////////////////////////////////////////////////

#endif