#ifndef NETWORK_SERVICE_HEAD_HEAD_FILE
#define NETWORK_SERVICE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨�ļ�
#include "..\..\ģ���\Template.h"
#include "..\..\�����ļ�\Constant.h"
#include "..\..\�����ļ�\GlobalDef.h"

//////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef NETWORK_SERVICE_CLASS
	#ifdef  NETWORK_SERVICE_DLL
		#define NETWORK_SERVICE_CLASS _declspec(dllexport)
	#else
		#define NETWORK_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define NETWORK_SERVICE_DLL_NAME	TEXT("NetworkService.dll")		//��� DLL ����
#else
	#define NETWORK_SERVICE_DLL_NAME	TEXT("NetworkServiceD.dll")		//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////
//��������

//�ر�ԭ��
#define SHUT_REASON_INSIDE			0									//�ڲ�ԭ��
#define SHUT_REASON_NORMAL			1									//�����ر�
#define SHUT_REASON_REMOTE			2									//Զ�̹ر�
#define SHUT_REASON_TIME_OUT		3									//���糬ʱ
#define SHUT_REASON_EXCEPTION		4									//�쳣�ر�

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��������
enum enProxyServerType
{
	ProxyType_None,					//û�д���
	ProxyType_Http,					//HTTP ����
	ProxyType_Socks4,				//SOCKS4 ����
	ProxyType_Socks5,				//SOCKS5 ����
};

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagProxyServerInfo
{
	WORD							wProxyPort;							//����˿�
	TCHAR							szUserName[32];						//�����û�
	TCHAR							szPassword[32];						//��������
	TCHAR							szHttpDomain[32];					//�����û�
	TCHAR							szProxyServer[32];					//�����ַ
};

//��������
struct tagProxyTypeDescribe
{
	TCHAR							szDescribe[32];						//�����ַ�
	enProxyServerType				ProxyServerType;					//��������
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITCPSocket INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocket={0x91446684,0x621c,0x4546,0x86,0x59,0x22,0xa9,0x5a,0x7c,0x20,0xb9};
#else
	#define VER_ITCPSocket INTERFACE_VERSION(1,1)
	static const GUID IID_ITCPSocket={0xc8a0b91a,0xcb6f,0x4749,0xa3,0x44,0xa7,0x06,0x32,0x11,0xbf,0x79};
#endif

//����ӿ�
interface ITCPSocket : public IUnknownEx
{
	//��ʶ�ӿ�
public:
	//��ȡ��ʶ
	virtual WORD __cdecl GetSocketID()=NULL;
	//���ñ�ʶ
	virtual VOID __cdecl SetSocketID(WORD wSocketID)=NULL;

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool __cdecl SetTCPSocketSink(IUnknownEx * pIUnknownEx)=NULL;
	//��ȡ�ӿ�
	virtual VOID * __cdecl GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer)=NULL;

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual BYTE __cdecl GetSocketStatus()=NULL;
	//���ͼ��
	virtual DWORD __cdecl GetLastSendTick()=NULL;
	//���ռ��
	virtual DWORD __cdecl GetLastRecvTick()=NULL;
	//������Ŀ
	virtual DWORD __cdecl GetSendPacketCount()=NULL;
	//������Ŀ
	virtual DWORD __cdecl GetRecvPacketCount()=NULL;

	//����ӿ�
public:
	//�������
	virtual DWORD __cdecl ProxyServerTesting()=NULL;
	//���ô���
	virtual bool __cdecl SetProxyServerInfo(enProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)=NULL;

	//�����ӿ�
public:
	//�ر�����
	virtual VOID __cdecl CloseSocket()=NULL;
	//���Ӳ���
	virtual DWORD __cdecl Connect(DWORD dwServerIP, WORD wPort)=NULL;
	//���Ӳ���
	virtual DWORD __cdecl Connect(LPCTSTR szServerIP, WORD wPort)=NULL;
	//���ͺ���
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual DWORD __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

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
	virtual bool __cdecl OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)=NULL;
	//�ر��¼�
	virtual bool __cdecl OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)=NULL;
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_INetworkManager INTERFACE_VERSION(1,1)
	static const GUID IID_INetworkManager={0x701dfa0e,0x3e66,0x44b7,0x00ae,0x7c,0xf4,0x5e,0x59,0x95,0xfb,0x50};
#else
	#define VER_INetworkManager INTERFACE_VERSION(1,1)
	static const GUID IID_INetworkManager={0xcb58785e,0x7657,0x4d6b,0x0081,0xc0,0x2a,0x82,0x25,0x48,0xf4,0x96};
#endif

//�������ӿ�
interface INetworkManager : public IUnknownEx
{
	//ö�ٴ���
	virtual bool __cdecl EnumProxyServerType(WORD wIndex, tagProxyTypeDescribe & ProxyTypeDescribe)=NULL;
	//�������
	virtual DWORD __cdecl ProxyServerTesting(enProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////
//���������

DECLARE_MODULE_HELPER(TCPSocket,NETWORK_SERVICE_DLL_NAME,"CreateTCPSocket")
DECLARE_MODULE_HELPER(NetworkManager,NETWORK_SERVICE_DLL_NAME,"CreateNetworkManager")

//////////////////////////////////////////////////////////////////////////

#endif