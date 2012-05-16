#include "StdAfx.h"
#include "IPCControls.h"

//////////////////////////////////////////////////////////////////////////

//��������
CIPCSendCopyData::CIPCSendCopyData(HWND hWndServer, HWND hWndClient)
{
	m_hWndClient=hWndClient;
	SetServerHwnd(hWndServer);
}

//��������
CIPCSendCopyData::~CIPCSendCopyData()
{
}

//�Ƿ�����
bool CIPCSendCopyData::IsConnect()
{
	return IsWindow(m_hWndServer)?true:false;
}

//���÷��ʹ���
bool CIPCSendCopyData::SetClientHwnd(HWND hWndClient)
{
	m_hWndClient=NULL;
	if (IsWindow(hWndClient))
	{
		m_hWndClient=hWndClient;
		return true;
	}
	return false;
}

//���ý��մ���
bool CIPCSendCopyData::SetServerHwnd(HWND hWndServer)
{
	m_hWndServer=NULL;
	if (IsWindow(hWndServer))
	{
		m_hWndServer=hWndServer;
		return true;
	}
	return false;
}

//���ý��մ���
bool CIPCSendCopyData::SetServerHwnd(LPCSTR szClassName)
{
	ASSERT(szClassName!=NULL);
	m_hWndServer=FindWindow(szClassName,NULL);
	return (m_hWndServer!=NULL);
}

//��������
bool CIPCSendCopyData::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	if (!IsWindow(m_hWndServer)) return false;

	//��������
	IPC_Head IPCHead;
	IPCHead.wVersion=IPC_VER;
	IPCHead.wDataSize=sizeof(IPC_Head);
	IPCHead.wMainCmdID=wMainCmdID;
	IPCHead.wSubCmdID=wSubCmdID;
	
	//��������
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=&IPCHead;
	CopyDataStruct.cbData=IPCHead.wDataSize;
	CopyDataStruct.dwData=IPC_IDENTIFIER;
	SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//��������
bool CIPCSendCopyData::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=IPC_PACKAGE);
	if (wDataSize>IPC_PACKAGE) return false;
	if (!IsWindow(m_hWndServer)) return false;

	//��������
	BYTE cbSendBuffer[IPC_PACKAGE+sizeof(IPC_Head)];
	IPC_Head * pIPCHead=(IPC_Head *)cbSendBuffer;
	pIPCHead->wVersion=IPC_VER;
	pIPCHead->wDataSize=sizeof(IPC_Head);
	pIPCHead->wMainCmdID=wMainCmdID;
	pIPCHead->wSubCmdID=wSubCmdID;
	if (pBuffer!=NULL)
	{
		pIPCHead->wDataSize+=wDataSize;
		CopyMemory(cbSendBuffer+sizeof(IPC_Head),pBuffer,wDataSize);
	}

	//��������
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=cbSendBuffer;
	CopyDataStruct.cbData=pIPCHead->wDataSize;
	CopyDataStruct.dwData=IPC_IDENTIFIER;
	SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//��������
bool CIPCSendCopyData::SendIPCData(DWORD dwIdentifier, void * pBuffer, WORD wSize)
{
	if (!IsWindow(m_hWndServer)) return false;

	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData=pBuffer;
	CopyDataStruct.cbData=wSize;
	CopyDataStruct.dwData=dwIdentifier;
	SendMessage(m_hWndServer,WM_COPYDATA,(LPARAM)m_hWndClient,(WPARAM)&CopyDataStruct);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CIPCRecvCopyData::CIPCRecvCopyData()
{
	m_pIChannelMessageSink=NULL;
}

//��������
CIPCRecvCopyData::~CIPCRecvCopyData()
{
}

//���ûص��ӿ�
bool CIPCRecvCopyData::SetChannelMessageSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIChannelMessageSink=(IChannelMessageSink *)pIUnknownEx->QueryInterface(IID_IChannelMessageSink,VER_IChannelMessageSink);
	ASSERT(m_pIChannelMessageSink!=NULL);
	return (m_pIChannelMessageSink!=NULL);
}

//COPYDATA ��Ϣ����
bool CIPCRecvCopyData::OnCopyData(HWND hWndSend, COPYDATASTRUCT * pCopyDataStruct)
{
	//Ч������
	ASSERT(m_pIChannelMessageSink!=NULL);
	if (m_pIChannelMessageSink==NULL) return false;
	if (pCopyDataStruct->dwData!=IPC_IDENTIFIER) return false;

	//��ȡ����
	IPC_Head IPCHead;
	WORD wDataSize=0;
	void * pIPCBuffer=NULL;
	if (!GetIPCData(IPCHead,pIPCBuffer,wDataSize,pCopyDataStruct)) return false;

	//��������
	return m_pIChannelMessageSink->OnChannelMessage(&IPCHead,pIPCBuffer,wDataSize,hWndSend);
}

//��ȡ����
bool CIPCRecvCopyData::GetIPCData(IPC_Head & IPCHead, void * & pIPCBuffer, WORD & wDataSize, PCOPYDATASTRUCT pCopyDataStruct)
{
	//Ч������
	ASSERT(pCopyDataStruct!=NULL);
	if (pCopyDataStruct->dwData!=IPC_IDENTIFIER) return false;
	if (pCopyDataStruct->cbData<sizeof(IPC_Head)) return false;

	//��ȡ����
	CopyMemory(&IPCHead,pCopyDataStruct->lpData,sizeof(IPC_Head));
	if (IPCHead.wVersion!=IPC_VER) return false;
	if (IPCHead.wDataSize!=pCopyDataStruct->cbData) return false;

	//��������
	IPCHead.wDataSize-=sizeof(IPC_Head);
	wDataSize=IPCHead.wDataSize;
	if (wDataSize>0) pIPCBuffer=((IPC_Head *)pCopyDataStruct->lpData)+1;
	else pIPCBuffer=NULL;

	return true;
}

//////////////////////////////////////////////////////////////////////////
