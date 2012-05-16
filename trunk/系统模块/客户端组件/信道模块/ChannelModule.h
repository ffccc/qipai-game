#ifndef CHANNEL_MODULE_HEAD_FILE
#define CHANNEL_MODULE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "..\..\ģ���\Template.h"
#include "..\..\�����ļ�\GlobalDef.h"

//���ͷ�ļ�
#include "..\..\�������\��������\ComService.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef CHANNEL_CLASS
	#ifdef  CHANNEL_DLL
		#define CHANNEL_CLASS _declspec(dllexport)
	#else
		#define CHANNEL_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define CHANNEL_MODULE_DLL_NAME	TEXT("ChannelModuleD.dll")			//��� DLL ����
#else
	#define CHANNEL_MODULE_DLL_NAME	TEXT("ChannelModule.dll")			//��� DLL ����
#endif

//�ں���������
#define IPC_MIAN_IPC_KERNEL					0							//�ں�����

//�ں���������
#define IPC_SUB_IPC_CLIENT_CONNECT			1							//����֪ͨ
#define IPC_SUB_IPC_SERVER_ACCEPT			2							//Ӧ��֪ͨ
#define IPC_SUB_IPC_CLIENT_CLOSE			3							//�ر�֪ͨ

//////////////////////////////////////////////////////////////////////////

#define VER_IChannelMessageSink INTERFACE_VERSION(1,1)
static const GUID IID_IChannelMessageSink={0x7a672dca,0x141e,0x45a6,0x95,0xc4,0x43,0x79,0x23,0x7f,0x29,0x73};

//�ŵ���Ϣ���ӽӿ�
interface IChannelMessageSink : public IUnknownEx
{
	//�ŵ����ݴ���
	virtual bool __cdecl OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IChannelEventSink INTERFACE_VERSION(1,1)
static const GUID IID_IChannelEventSink={0xb87334af,0x3f52,0x4e51,0x9d,0xc6,0x51,0x93,0x24,0x66,0x9e,0x2e};

//�ŵ��¼����ӽӿ�
interface IChannelEventSink : public IUnknownEx
{
	//������Ϣ
	virtual bool __cdecl OnChannelConnect(HWND hWndSend)=NULL;
	//Ӧ����Ϣ
	virtual bool __cdecl OnChannelAccept(HWND hWndSend)=NULL;
	//�Ͽ���Ϣ
	virtual bool __cdecl OnChannelClose(HWND hWndSend, bool bInitiative)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IChannelService INTERFACE_VERSION(1,1)
static const GUID IID_IChannelService={0x4d0151e9,0xbd65,0x4a4c,0xa6,0xf8,0x25,0x74,0x68,0x17,0x6e,0x74};

//�ŵ�ģ��ӿ�
interface IChannelService : public IUnknownEx
{
	//�����¼�
	virtual bool __cdecl SetChannelEventSink(IUnknownEx * pIUnknownEx)=NULL;
	//������Ϣ
	virtual bool __cdecl SetChannelMessageSink(IUnknownEx * pIUnknownEx)=NULL;
	//��ȡ�¼�
	virtual void * __cdecl GetChannelEventSink(const IID & Guid, DWORD dwQueryVer)=NULL;
	//��ȡ��Ϣ
	virtual void * __cdecl GetChannelMessageSink(const IID & Guid, DWORD dwQueryVer)=NULL;
	//�Ƿ�����
	virtual bool __cdecl IsConnect(bool bEfficacy)=NULL;
	//��������
	virtual bool __cdecl CreateChannel(HWND hWndServer)=NULL;
	//�رպ���
	virtual bool __cdecl CloseChannel(bool bNotifyServer, bool bNotifySink)=NULL;
	//��������
	virtual bool __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//�ŵ���������
class CChannelServiceHelper : public CTempldateHelper<IChannelService>
{
	//��������
public:
	//���캯��
	CChannelServiceHelper(void) : CTempldateHelper<IChannelService>(IID_IChannelService,
		VER_IChannelService,CHANNEL_MODULE_DLL_NAME,TEXT("CreateChannelService"))
	{
	}
};

//////////////////////////////////////////////////////////////////////////

//��������ļ�
#include "IPCControls.h"

//////////////////////////////////////////////////////////////////////////

#endif