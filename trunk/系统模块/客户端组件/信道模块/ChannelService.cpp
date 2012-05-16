#include "StdAfx.h"
#include "IPCControls.h"
#include "ChannelService.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define IDI_IPC_DATA				1									//IPC ����
#define WM_IPC_MESSAGE				(WM_USER+100)						//IPC ��Ϣ
#define IPCPacketSize(IPCPacket)	(sizeof(tagIPCPacket)-sizeof(IPCPacket.cbBuffer))

//////////////////////////////////////////////////////////////////////////
//�ṹ�嶨��

//IPC ������
struct tagIPCPacket
{
	IPC_Head						Head;								//���ݰ�ͷ
	HWND							hWndSend;							//���ʹ���
	BYTE							cbBuffer[IPC_PACKAGE];				//���ݻ���
};

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChannelService, CWnd)
	ON_WM_COPYDATA()
	ON_MESSAGE(WM_IPC_MESSAGE,OnIPCMessage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CChannelService::CChannelService()
{
	//���ñ���
	m_hWndServer=NULL;
	m_pIChannelEventSink=NULL;
	m_pIChannelMessageSink=NULL;

	//���ýӿ�
	m_IPCRecvCopyData.SetChannelMessageSink(QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//��������
CChannelService::~CChannelService()
{
	CloseChannel(true,false);
	return;
}

//�ӿڲ�ѯ
void * __cdecl CChannelService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IChannelService,Guid,dwQueryVer);
	QUERYINTERFACE(IChannelMessageSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IChannelService,Guid,dwQueryVer);
	return NULL;
}

//IPC ��Ϣ
BOOL CChannelService::OnCopyData(CWnd * pWnd, COPYDATASTRUCT * pCopyDataStruct)
{
	HWND hWndSend=pWnd->m_hWnd;
	if ((hWndSend==m_hWndServer)&&m_IPCRecvCopyData.OnCopyData(hWndSend,pCopyDataStruct)) return TRUE;
	return __super::OnCopyData(pWnd, pCopyDataStruct);
}

//�����¼�
bool __cdecl CChannelService::SetChannelEventSink(IUnknownEx * pIUnknownEx)
{
	if (pIUnknownEx!=NULL)
	{
		m_pIChannelEventSink=(IChannelEventSink *)pIUnknownEx->QueryInterface(IID_IChannelEventSink,VER_IChannelEventSink);
		ASSERT(m_pIChannelEventSink!=NULL);
		return (m_pIChannelEventSink!=NULL);
	}
	else
	{
		m_pIChannelEventSink=NULL;
	}

	return true;
}

//������Ϣ
bool __cdecl CChannelService::SetChannelMessageSink(IUnknownEx * pIUnknownEx)
{
	if (pIUnknownEx!=NULL)
	{
		m_pIChannelMessageSink=(IChannelMessageSink *)pIUnknownEx->QueryInterface(IID_IChannelMessageSink,VER_IChannelMessageSink);
		ASSERT(m_pIChannelMessageSink!=NULL);
		return (m_pIChannelMessageSink!=NULL);
	}
	else
	{
		m_pIChannelMessageSink=NULL;
	}

	return true;
}

//��ȡ�¼�
void * __cdecl CChannelService::GetChannelEventSink(const IID & Guid, DWORD dwQueryVer)
{
	if (m_pIChannelEventSink==NULL) return NULL;
	return m_pIChannelEventSink->QueryInterface(Guid,dwQueryVer);
}

//��ȡ��Ϣ
void * __cdecl CChannelService::GetChannelMessageSink(const IID & Guid, DWORD dwQueryVer)
{
	if (m_pIChannelMessageSink==NULL) return NULL;
	return m_pIChannelMessageSink->QueryInterface(Guid,dwQueryVer);
}

//��ʼ��
bool __cdecl CChannelService::CreateChannel(HWND hWndServer)
{
	//��������
	m_hWndServer=hWndServer;
	m_DataStorage.RemoveData(false);
	if (Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),10)==FALSE) return false;

	//��������֪ͨ
	return SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CONNECT);
}

//�Ƿ�����
bool __cdecl CChannelService::IsConnect(bool bEfficacy)
{
	if (m_hWndServer==NULL) return false;
	if (bEfficacy==true) return IsWindow(m_hWndServer)?true:false;
	return true;
}

//�رպ���
bool __cdecl CChannelService::CloseChannel(bool bNotifyServer, bool bNotifySink)
{
	//�ж�״̬
	if (m_hWndServer==NULL) return false;

	//���͹ر���Ϣ
	if (bNotifyServer==true) SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CLOSE);
	if ((bNotifySink==true)&&(m_pIChannelEventSink!=NULL)) m_pIChannelEventSink->OnChannelClose(m_hWndServer,true);

	//���ñ���
	DestroyWindow();
	m_hWndServer=NULL;
	m_DataStorage.RemoveData(false);

	return true;
}

//��������
bool __cdecl CChannelService::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	if (m_hWndServer==NULL) return false;
	CIPCSendCopyData IPCSendCopyData(m_hWndServer,m_hWnd);
	return IPCSendCopyData.SendData(wMainCmdID,wSubCmdID);
}

//��������
bool __cdecl CChannelService::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	if (m_hWndServer==NULL) return false;
	CIPCSendCopyData IPCSendCopyData(m_hWndServer,m_hWnd);
	return IPCSendCopyData.SendData(wMainCmdID,wSubCmdID,pBuffer,wDataSize);
}

//�ŵ����ݴ���
bool __cdecl CChannelService::OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	//Ч������
	ASSERT(wDataSize<=IPC_PACKAGE);
	if (wDataSize>IPC_PACKAGE) return false;
	if (hWndSend!=m_hWndServer) return false;

	//��������
	tagIPCPacket IPCPacket;
	IPCPacket.hWndSend=hWndSend;
	CopyMemory(&IPCPacket.Head,pHead,sizeof(IPC_Head));
	if (wDataSize>0)
	{
		ASSERT(pIPCBuffer!=NULL);
		CopyMemory(IPCPacket.cbBuffer,pIPCBuffer,wDataSize);
	}

	//��������
	WORD wAddSize=IPCPacketSize(IPCPacket)+wDataSize;
	m_DataStorage.InsertData(IDI_IPC_DATA,&IPCPacket,wAddSize);

	//Ͷ����Ϣ
	PostMessage(WM_IPC_MESSAGE,(WPARAM)wDataSize,(LPARAM)wDataSize);

	return true;
}

//IPC ��Ϣ
LRESULT CChannelService::OnIPCMessage(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	tagDataHead DataHead;
	tagIPCPacket IPCPacket;
	bool bSuccess=m_DataStorage.DistillData(DataHead,&IPCPacket,sizeof(IPCPacket));

	//Ч������
	ASSERT(bSuccess==true);
	ASSERT(DataHead.wDataSize>=IPCPacketSize(IPCPacket));
	ASSERT(DataHead.wDataSize==IPCPacket.Head.wDataSize+IPCPacketSize(IPCPacket));
	if (DataHead.wDataSize<IPCPacketSize(IPCPacket)) return 0;
	if (DataHead.wDataSize!=IPCPacket.Head.wDataSize+IPCPacketSize(IPCPacket)) return 0;

	//��������
	try
	{
		if (IPCPacket.Head.wMainCmdID==IPC_MIAN_IPC_KERNEL)	
		{
			//�ں���Ϣ
			switch (IPCPacket.Head.wSubCmdID)
			{
			case IPC_SUB_IPC_CLIENT_CONNECT:	//������Ϣ
				{
					//����֪ͨ
					if (m_pIChannelEventSink!=NULL) m_pIChannelEventSink->OnChannelConnect(IPCPacket.hWndSend);

					//����֪ͨ
					CIPCSendCopyData SendCopyData;
					SendCopyData.SetClientHwnd(m_hWnd);
					SendCopyData.SetServerHwnd(IPCPacket.hWndSend);
					SendCopyData.SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_SERVER_ACCEPT);

					break;
				}
			case IPC_SUB_IPC_SERVER_ACCEPT:		//Ӧ����Ϣ
				{
					//����֪ͨ
					if (m_pIChannelEventSink!=NULL)	m_pIChannelEventSink->OnChannelAccept(IPCPacket.hWndSend);

					break;
				}
			case IPC_SUB_IPC_CLIENT_CLOSE:		//�ر���Ϣ
				{
					//�ر�����
					CloseChannel(false,true);

					break;
				}
			default: ASSERT(0);
			}
		}
		else	//��Χ��Ϣ
		{
			if (m_pIChannelMessageSink!=NULL)
			{
				WORD wDataSize=IPCPacket.Head.wDataSize;
				m_pIChannelMessageSink->OnChannelMessage(&IPCPacket.Head,IPCPacket.cbBuffer,wDataSize,IPCPacket.hWndSend);
			}
		}
	}
	catch (...)	{ }

	return 0;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateChannelService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CChannelService * pChannelService=NULL;
	try
	{
		pChannelService=new CChannelService();
		if (pChannelService==NULL) throw TEXT("����ʧ��");
		void * pObject=pChannelService->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pChannelService);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
