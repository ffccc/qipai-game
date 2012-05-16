#include "StdAfx.h"
#include "TCPNetworkEngine.h"
//////////////////////////////////////////////////////////////////////////
//���캯��
COverLapped::COverLapped(enOperationType OperationType) : m_OperationType(OperationType)
{
	memset(&m_WSABuffer,0,sizeof(m_WSABuffer));
	memset(&m_OverLapped,0,sizeof(m_OverLapped));
}

//��������
COverLapped::~COverLapped()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLappedSend::COverLappedSend() : COverLapped(OperationType_Send)
{
	m_WSABuffer.len=0;
	m_WSABuffer.buf=(char *)m_cbBuffer;
}

//��������
COverLappedSend::~COverLappedSend()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketItem::CServerSocketItem(WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink) 
	: m_wIndex(wIndex), m_pIServerSocketItemSink(pIServerSocketItemSink)
{
	m_wRountID=0;
	m_wRecvSize=0;
	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_bNotify=true;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_bAllowBatch=false;
	m_dwSendXorKey=0;
	m_dwRecvXorKey=0;
	m_dwClientAddr=0;
	m_dwConnectTime=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;
	m_hSocket=INVALID_SOCKET;
	m_OverLappedSendActiveIndex=0;
}

//��������
CServerSocketItem::~CServerSocketItem(void)
{
	//ɾ�������ص� IO
	INT_PTR iCount=m_OverLappedSendFree.GetCount();
	for (INT_PTR i=0;i<iCount;i++) delete m_OverLappedSendFree[i];
	m_OverLappedSendFree.RemoveAll();

	//ɾ����ص� IO
	iCount=m_OverLappedSendActive.GetCount();
	for (INT_PTR i=0;i<iCount;i++) delete m_OverLappedSendActive[i];
	m_OverLappedSendActive.RemoveAll();

	return;
}

//���ӳ��
WORD CServerSocketItem::SeedRandMap(WORD wSeed)
{
	DWORD dwHold=wSeed;
	return (WORD)((dwHold=dwHold*241103L+2533101L)>>16);
}

//ӳ�䷢������
BYTE CServerSocketItem::MapSendByte(BYTE const cbData)
{
	BYTE cbMap=g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
	m_cbSendRound+=3;
	return cbMap;
}

//ӳ���������
BYTE CServerSocketItem::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap=g_RecvByteMap[cbData]-m_cbRecvRound;
	m_cbRecvRound+=3;
	return cbMap;
}

//��ȡ���ͽṹ
COverLappedSend * CServerSocketItem::GetSendOverLapped()
{
	//Ѱ�ҿ��нṹ
	COverLappedSend * pOverLappedSend=NULL;
	INT_PTR nFreeCount=m_OverLappedSendFree.GetCount();
	if (nFreeCount>0)
	{
		pOverLappedSend=m_OverLappedSendFree[nFreeCount-1];
		ASSERT(pOverLappedSend!=NULL);
		m_OverLappedSendFree.RemoveAt(nFreeCount-1);
		m_OverLappedSendActive.Add(pOverLappedSend);
		return pOverLappedSend;
	}

	//�½����ͽṹ
	try
	{
		pOverLappedSend=new COverLappedSend;
		ASSERT(pOverLappedSend!=NULL);
		m_OverLappedSendActive.Add(pOverLappedSend);
		return pOverLappedSend;
	}
	catch (...) { }
	return NULL;
}

//�󶨶���
DWORD CServerSocketItem::Attach(SOCKET hSocket, DWORD dwClientAddr)
{
	//Ч������
	ASSERT(dwClientAddr!=0);
	ASSERT(m_bRecvIng==false);
	ASSERT(IsValidSocket()==false);
	ASSERT(hSocket!=INVALID_SOCKET);

	//���ñ���
	m_bNotify=false;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_hSocket=hSocket;
	m_dwClientAddr=dwClientAddr;
	m_dwRecvTickCount=GetTickCount();
	m_dwConnectTime=(DWORD)time(NULL);

	//����֪ͨ
	m_pIServerSocketItemSink->OnSocketAcceptEvent(this);

	return GetIdentifierID();
}

//���ͺ���
bool CServerSocketItem::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	WORD wSendSize=EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head),sizeof(pOverLappedSend->m_cbBuffer));

	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}

//���ͺ���
bool CServerSocketItem::SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч�����
	ASSERT(wDataSize<=SOCKET_PACKAGE);

	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;
	if (wDataSize>SOCKET_PACKAGE) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		memcpy(pHead+1,pData,wDataSize);
	}
	WORD wSendSize=EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head)+wDataSize,sizeof(pOverLappedSend->m_cbBuffer));
	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}

//Ͷ�ݽ���
bool CServerSocketItem::RecvData()
{
	//Ч�����
	ASSERT(m_bRecvIng==false);
	ASSERT(m_hSocket!=INVALID_SOCKET);

	//�жϹر�
	if (m_bCloseIng==true)
	{
		if (m_OverLappedSendActive.GetCount()==0)
			CloseSocket(m_wRountID);
		return false;
	}

	//��������
	m_bRecvIng=true;
	DWORD dwThancferred=0,dwFlags=0;
	int iRetCode=WSARecv(m_hSocket,&m_OverLappedRecv.m_WSABuffer,1,&dwThancferred,&dwFlags,&m_OverLappedRecv.m_OverLapped,NULL);
	if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
	{
		m_bRecvIng=false;
		CloseSocket(m_wRountID);
		return false;
	}

	return true;
}

//�ر�����
bool CServerSocketItem::CloseSocket
(WORD wRountID)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	//�ر�����
	if (m_hSocket!=INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
	}
	m_bNotify=false;
	//�жϹر�
	if ((m_bRecvIng==false)&&(m_OverLappedSendActive.GetCount()==0)) 
	{
		OnCloseCompleted();
	}
	return true;
}

//���ùر�
bool CServerSocketItem::ShutDownSocket
(WORD wRountID)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;
	m_bNotify=false;

	//���ñ���
	if (m_bCloseIng==false)
	{
		m_bCloseIng=true;
		if (m_OverLappedSendActive.GetCount()==0) 
			CloseSocket(wRountID);
	}

	return true;
}

//����Ⱥ��
bool CServerSocketItem::AllowBatchSend(WORD wRountID, bool bAllowBatch)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//���ñ���
	m_bAllowBatch=bAllowBatch;

	return true;
}

//���ñ���
void CServerSocketItem::ResetSocketData()
{
	//Ч��״̬
	ASSERT(m_hSocket==INVALID_SOCKET);

	//��������
	m_wRountID++;
	m_wRecvSize=0;
	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_dwSendXorKey=0;
	m_dwRecvXorKey=0;
	m_dwClientAddr=0;
	m_dwConnectTime=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;

	//״̬����
	m_bNotify=true;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_bAllowBatch=false;
	m_OverLappedSendFree.Append(m_OverLappedSendActive);
	m_OverLappedSendActive.RemoveAll();

	return;
}

//������ɺ���
bool CServerSocketItem::OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred)
{
	//Ч�����
	ASSERT(pOverLappedSend!=NULL);
	ASSERT(m_OverLappedSendActive.GetCount()>0);
	ASSERT(pOverLappedSend==m_OverLappedSendActive[0]);

	//�ͷŷ��ͽṹ
	m_OverLappedSendActive.RemoveAt(0);
	m_OverLappedSendFree.Add(pOverLappedSend);

	//���ñ���
	if (dwThancferred!=0) m_dwSendTickCount=GetTickCount();

	//�жϹر�
	if (m_hSocket==INVALID_SOCKET)
	{
		m_OverLappedSendFree.Append(m_OverLappedSendActive);
		m_OverLappedSendActive.RemoveAll();
		CloseSocket(m_wRountID);
		return true;
	}

	//������������
	if (m_OverLappedSendActive.GetCount()>0)
	{
		DWORD dwThancferred=0;
		pOverLappedSend=m_OverLappedSendActive[0];
		ASSERT(pOverLappedSend!=NULL);
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			m_OverLappedSendFree.Append(m_OverLappedSendActive);
			m_OverLappedSendActive.RemoveAll();
			CloseSocket(m_wRountID);
			return false;
		}
		return true;
	}

	//�жϹر�
	if (m_bCloseIng==true) 
	{
		CloseSocket(m_wRountID);
	}
	return true;
}

//������ɺ���
bool CServerSocketItem::OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred)
{
	//Ч������
	ASSERT(m_bRecvIng==true);

	//���ñ���
	m_bRecvIng=false;
	m_wRecvSize=dwThancferred;
	m_dwRecvTickCount=GetTickCount();

	//�жϹر�
	if (m_hSocket==INVALID_SOCKET)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//��������
	int iRetCode=recv(m_hSocket,(char *)m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);
	if (iRetCode<=0)
	{
		CloseSocket(m_wRountID);
		return true;
	}
	
	//�������
	m_wRecvSize+=iRetCode;
	BYTE cbBuffer[SOCKET_BUFFER];
	CMD_Head * pHead=(CMD_Head *)m_cbRecvBuf;

	//��������
	try
	{
		while (m_wRecvSize>=sizeof(CMD_Head))
		{
			//Ч������
			WORD wPacketSize=pHead->CmdInfo.wPacketSize;
			//if (wPacketSize>SOCKET_BUFFER) throw TEXT("���ݰ�����");
			if (wPacketSize<sizeof(CMD_Head)) throw TEXT("���ݰ��Ƿ�");
			if (pHead->CmdInfo.cbVersion!=SOCKET_VER) throw TEXT("���ݰ��汾����1");
			if (m_wRecvSize<wPacketSize) break;

			//��ȡ����
			CopyMemory(cbBuffer,m_cbRecvBuf,wPacketSize);
			//todo
			WORD wRealySize=CrevasseBuffer(cbBuffer,wPacketSize);
			ASSERT(wRealySize>=sizeof(CMD_Head));
			m_dwRecvPacketCount++;
			//��������
			WORD wDataSize=wRealySize-sizeof(CMD_Head);
			void * pDataBuffer=cbBuffer+sizeof(CMD_Head);
			CMD_Command Command=((CMD_Head *)cbBuffer)->CommandInfo;

			//�ں�����
			if (Command.wMainCmdID==MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET:	//������
					{
						break;
					}
				default: throw TEXT("�Ƿ�������");
				}
			}
			else 
			{
				//��Ϣ����
				m_pIServerSocketItemSink->OnSocketReadEvent(Command,pDataBuffer,wDataSize,this);			
			}

			//ɾ����������
			m_wRecvSize-=wPacketSize;
			MoveMemory(m_cbRecvBuf,m_cbRecvBuf+wPacketSize,m_wRecvSize);
		}
	}
	catch (LPCTSTR )
	{
		CloseSocket(m_wRountID);
		return false;
	}
	catch (...)
	{ 
		CloseSocket(m_wRountID);
		return false;
	}

	return RecvData();
}

//�ر����֪ͨ
bool CServerSocketItem::OnCloseCompleted()
{
	//Ч��״̬
	ASSERT(m_hSocket==INVALID_SOCKET);
	ASSERT(m_OverLappedSendActive.GetCount()==0);

	////m_bNotify=false;
	////�ر��¼� todo
	//ASSERT(m_bNotify==false);
	//if (m_bNotify==false)
	//{
	//	m_bNotify=true;
		m_pIServerSocketItemSink->OnSocketCloseEvent(this);
	//}

	//״̬����
	ResetSocketData();

	return true;
}

WORD CServerSocketItem::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	WORD i = 0;
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_Head));
	ASSERT(wDataSize<=(sizeof(CMD_Head)+SOCKET_PACKAGE));
	ASSERT(wBufferSize>=(wDataSize+2*sizeof(DWORD)));

	//��������
	WORD wEncryptSize=wDataSize-sizeof(CMD_Info),wSnapCount=0;
	if ((wEncryptSize%sizeof(DWORD))!=0)
	{
		wSnapCount=sizeof(DWORD)-wEncryptSize%sizeof(DWORD);
		memset(pcbDataBuffer+sizeof(CMD_Info)+wEncryptSize,0,wSnapCount);
	}

	//Ч�������ֽ�ӳ��
	BYTE cbCheckCode=0;
	for (i=sizeof(CMD_Info);i<wDataSize;i++) 
	{
		cbCheckCode+=pcbDataBuffer[i];
		pcbDataBuffer[i]=MapSendByte(pcbDataBuffer[i]);
	}

	//��д��Ϣͷ
	CMD_Head * pHead=(CMD_Head *)pcbDataBuffer;
	pHead->CmdInfo.cbCheckCode=~cbCheckCode+1;
	pHead->CmdInfo.wPacketSize=wDataSize;
	pHead->CmdInfo.cbVersion=SOCKET_VER;

	//��������
	DWORD dwXorKey=m_dwSendXorKey;
	WORD * pwSeed=(WORD *)(pcbDataBuffer+sizeof(CMD_Info));
	DWORD * pdwXor=(DWORD *)(pcbDataBuffer+sizeof(CMD_Info));
	WORD wEncrypCount=(wEncryptSize+wSnapCount)/sizeof(DWORD);
	for (i=0;i<wEncrypCount;i++)
	{
		*pdwXor++^=dwXorKey;
		dwXorKey=SeedRandMap(*pwSeed++);
		dwXorKey|=((DWORD)SeedRandMap(*pwSeed++))<<16; 
		dwXorKey^=g_dwPacketKey;
	}

	//���ñ���
	m_dwSendPacketCount++;
	m_dwSendXorKey=dwXorKey;

	return wDataSize;
}

//��������
WORD CServerSocketItem::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	WORD i = 0;
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_Head));
	ASSERT(((CMD_Head *)pcbDataBuffer)->CmdInfo.wPacketSize==wDataSize);

	//��������
	WORD wSnapCount=0;
	if ((wDataSize%sizeof(DWORD))!=0)
	{
		wSnapCount=sizeof(DWORD)-wDataSize%sizeof(DWORD);
		memset(pcbDataBuffer+wDataSize,0,wSnapCount);
	}

	//��ȡ��Կ
	if (m_dwRecvPacketCount==0)
	{
		ASSERT(wDataSize>=(sizeof(CMD_Head)+sizeof(DWORD)));
		if (wDataSize<(sizeof(CMD_Head)+sizeof(DWORD))) throw TEXT("���ݰ����ܳ��ȴ���");
		m_dwRecvXorKey=*(DWORD *)(pcbDataBuffer+sizeof(CMD_Head));
		m_dwSendXorKey=m_dwRecvXorKey;
		MoveMemory(pcbDataBuffer+sizeof(CMD_Head),pcbDataBuffer+sizeof(CMD_Head)+sizeof(DWORD),
			wDataSize-sizeof(CMD_Head)-sizeof(DWORD));
		wDataSize-=sizeof(DWORD);
		((CMD_Head *)pcbDataBuffer)->CmdInfo.wPacketSize-=sizeof(DWORD);
	}

	//��������
	DWORD dwXorKey=m_dwRecvXorKey;
	DWORD * pdwXor=(DWORD *)(pcbDataBuffer+sizeof(CMD_Info));
	WORD  * pwSeed=(WORD *)(pcbDataBuffer+sizeof(CMD_Info));
	WORD wEncrypCount=(wDataSize+wSnapCount-sizeof(CMD_Info))/4;
	for (i=0;i<wEncrypCount;i++)
	{
		if ((i==(wEncrypCount-1))&&(wSnapCount>0))
		{
			BYTE * pcbKey=((BYTE *)&m_dwRecvXorKey)+sizeof(DWORD)-wSnapCount;
			CopyMemory(pcbDataBuffer+wDataSize,pcbKey,wSnapCount);
		}
		dwXorKey=SeedRandMap(*pwSeed++);
		dwXorKey|=((DWORD)SeedRandMap(*pwSeed++))<<16;
		dwXorKey^=g_dwPacketKey;
		*pdwXor++^=m_dwRecvXorKey;
		m_dwRecvXorKey=dwXorKey;
	}

	//Ч�������ֽ�ӳ��
	CMD_Head * pHead=(CMD_Head *)pcbDataBuffer;
	BYTE cbCheckCode=pHead->CmdInfo.cbCheckCode;;
	for (i=sizeof(CMD_Info);i<wDataSize;i++)
	{
		pcbDataBuffer[i]=MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode+=pcbDataBuffer[i];
	}
	if (cbCheckCode!=0) throw TEXT("���ݰ�Ч�������");

	return wDataSize;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketRSThread::CServerSocketRSThread(void)
{
	m_hCompletionPort=NULL;
}

//��������
CServerSocketRSThread::~CServerSocketRSThread(void)
{
}

//���ú���
bool CServerSocketRSThread::InitThread(HANDLE hCompletionPort)
{
	ASSERT(hCompletionPort!=NULL);
	m_hCompletionPort=hCompletionPort;
	return true;
}

//���к���
bool CServerSocketRSThread::RepetitionRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort!=NULL);

	//��������
	DWORD dwThancferred=0;					
	OVERLAPPED * pOverLapped=NULL;
	COverLapped * pSocketLapped=NULL;
	CServerSocketItem * pServerSocketItem=NULL;

	//�ȴ���ɶ˿�
	BOOL bSuccess=GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pServerSocketItem,&pOverLapped,INFINITE);
	if ((bSuccess==FALSE)&&(GetLastError()!=ERROR_NETNAME_DELETED)) return false;
	if ((pServerSocketItem==NULL)&&(pOverLapped==NULL)) return false;

	//�������
	ASSERT(pOverLapped!=NULL);
	ASSERT(pServerSocketItem!=NULL);
	pSocketLapped=CONTAINING_RECORD(pOverLapped,COverLapped,m_OverLapped);
	switch (pSocketLapped->GetOperationType())
	{
	case OperationType_Recv:	//SOCKET ���ݶ�ȡ
		{
			COverLappedRecv * pOverLappedRecv=(COverLappedRecv *)pSocketLapped;
			CThreadLock SocketLockHandle(m_CriticalSection);
			pServerSocketItem->OnRecvCompleted(pOverLappedRecv,dwThancferred);
			break;
		}
	case OperationType_Send:	//SOCKET ���ݷ���
		{
			COverLappedSend * pOverLappedSend=(COverLappedSend *)pSocketLapped;
			CThreadLock SocketLockHandle(m_CriticalSection);
			pServerSocketItem->OnSendCompleted(pOverLappedSend,dwThancferred);
			break;
		}
	}

	return true;
}


//
bool CServerSocketRSThread::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort!=NULL);

	//��������
	DWORD dwThancferred=0;					
	OVERLAPPED * pOverLapped=NULL;
	COverLapped * pSocketLapped=NULL;
	CServerSocketItem * pServerSocketItem=NULL;

	//�ȴ���ɶ˿�
	BOOL bSuccess=GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pServerSocketItem,&pOverLapped,INFINITE);
	if ((bSuccess==FALSE)&&(GetLastError()!=ERROR_NETNAME_DELETED)) return false;
	if ((pServerSocketItem==NULL)&&(pOverLapped==NULL)) return false;

	//�������
	ASSERT(pOverLapped!=NULL);
	ASSERT(pServerSocketItem!=NULL);
	pSocketLapped=CONTAINING_RECORD(pOverLapped,COverLapped,m_OverLapped);
	switch (pSocketLapped->GetOperationType())
	{
	case OperationType_Recv:	//SOCKET ���ݶ�ȡ
		{
			COverLappedRecv * pOverLappedRecv=(COverLappedRecv *)pSocketLapped;
			ASSERT(pOverLappedRecv!=NULL);
			CThreadLock SocketLockHandle(m_CriticalSection);
			pServerSocketItem->OnRecvCompleted(pOverLappedRecv,dwThancferred);
			break;
		}
	case OperationType_Send:	//SOCKET ���ݷ���
		{
			COverLappedSend * pOverLappedSend=(COverLappedSend *)pSocketLapped;
			ASSERT(pOverLappedSend!=NULL);
			CThreadLock SocketLockHandle(m_CriticalSection);
			pServerSocketItem->OnSendCompleted(pOverLappedSend,dwThancferred);
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketAcceptThread::CSocketAcceptThread(void)
{
	m_hCompletionPort=NULL;
	m_pTCPSocketManager=NULL;
	m_hListenSocket=INVALID_SOCKET;
}

//��������
CSocketAcceptThread::~CSocketAcceptThread(void)
{
}

//���ú���
bool CSocketAcceptThread::InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPNetworkEngine * pTCPSocketManager)
{
	ASSERT(hCompletionPort!=NULL);
	ASSERT(pTCPSocketManager!=NULL);
	ASSERT(hListenSocket!=INVALID_SOCKET);
	m_hListenSocket=hListenSocket;
	m_hCompletionPort=hCompletionPort;
	m_pTCPSocketManager=pTCPSocketManager;
	return true;
}

//���к���
bool CSocketAcceptThread::RepetitionRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort!=NULL);
	ASSERT(m_pTCPSocketManager!=NULL);

	//���ñ���
	SOCKADDR_IN	SocketAddr;
	CServerSocketItem * pServerSocketItem=NULL;
	SOCKET hConnectSocket=INVALID_SOCKET;
	int nBufferSize=sizeof(SocketAddr);

	try
	{
		//��������
		hConnectSocket=WSAAccept(m_hListenSocket,(SOCKADDR *)&SocketAddr,&nBufferSize,NULL,NULL);
		if (hConnectSocket==INVALID_SOCKET) return false;

		//��ȡ����
		pServerSocketItem=m_pTCPSocketManager->ActiveSocketItem();
		if (pServerSocketItem==NULL) throw TEXT("�������Ӷ���ʧ��");

		//�������
		CThreadLock SocketLockHandle(m_CriticalSection);
		pServerSocketItem->Attach(hConnectSocket,SocketAddr.sin_addr.S_un.S_addr);
		CreateIoCompletionPort((HANDLE)hConnectSocket,m_hCompletionPort,(ULONG_PTR)pServerSocketItem,0);
		pServerSocketItem->RecvData();
	}
	catch (...)
	{
		//�������
		ASSERT(pServerSocketItem==NULL);
		if (hConnectSocket!=INVALID_SOCKET)	closesocket(hConnectSocket);
	}

	return true;
}
//
bool CSocketAcceptThread::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort!=NULL);
	ASSERT(m_pTCPSocketManager!=NULL);

	//���ñ���
	SOCKADDR_IN	SocketAddr;
	CServerSocketItem * pServerSocketItem=NULL;
	SOCKET hConnectSocket=INVALID_SOCKET;
	int nBufferSize=sizeof(SocketAddr);

	try
	{
		//��������
		hConnectSocket=WSAAccept(m_hListenSocket,(SOCKADDR *)&SocketAddr,&nBufferSize,NULL,NULL);
		if (hConnectSocket==INVALID_SOCKET) return false;

		//��ȡ����
		pServerSocketItem=m_pTCPSocketManager->ActiveSocketItem();
		if (pServerSocketItem==NULL) throw TEXT("�������Ӷ���ʧ��");

		//�������
		CThreadLock SocketLockHandle(m_CriticalSection);
		pServerSocketItem->Attach(hConnectSocket,SocketAddr.sin_addr.S_un.S_addr);
		CreateIoCompletionPort((HANDLE)hConnectSocket,m_hCompletionPort,(ULONG_PTR)pServerSocketItem,0);
		pServerSocketItem->RecvData();
	}
	catch(LPCTSTR )
	{
		//�������
		ASSERT(pServerSocketItem==NULL);
		if (hConnectSocket!=INVALID_SOCKET)	closesocket(hConnectSocket);
	}
	catch (...)
	{
		//�������
		ASSERT(pServerSocketItem==NULL);
		if (hConnectSocket!=INVALID_SOCKET)	closesocket(hConnectSocket);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketDetectThread::CSocketDetectThread(void)
{
	m_dwTickCount=0;;
	m_pTCPSocketManager=NULL;
}

//��������
CSocketDetectThread::~CSocketDetectThread(void)
{
}

//���ú���
bool CSocketDetectThread::InitThread(CTCPNetworkEngine * pTCPSocketManager)
{
	//Ч�����
	ASSERT(pTCPSocketManager!=NULL);

	//���ñ���
	m_dwTickCount=0L;
	m_pTCPSocketManager=pTCPSocketManager;

	return true;
}

//���к���
bool CSocketDetectThread::RepetitionRun()
{
	//Ч�����
	ASSERT(m_pTCPSocketManager!=NULL);

	//���ü��
	Sleep(500);
	m_dwTickCount+=500L;

	//�������
	if (m_dwTickCount>20000L)
	{
		m_dwTickCount=0L;
		m_pTCPSocketManager->DetectSocket();
	}

	return true;
}
//
bool CSocketDetectThread::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_pTCPSocketManager!=NULL);

	//���ü��
	Sleep(500);
	m_dwTickCount+=500L;

	//�������
	if (m_dwTickCount>20000L)
	{
		m_dwTickCount=0L;
		m_pTCPSocketManager->DetectSocket();
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPNetworkEngine::CTCPNetworkEngine(void)
{
	m_bService=false;
	m_wListenPort=0;
	m_dwLastDetect=0;
	m_wMaxSocketItem=0;
	m_hCompletionPort=NULL;
	m_hServerSocket=INVALID_SOCKET;

	m_pITCPNetworkEngineEvent=NULL;
	return;
}

//��������
CTCPNetworkEngine::~CTCPNetworkEngine(void)
{
	//ֹͣ����
	ConcludeService();

	//�ͷŴ洢����
	CServerSocketItem * pSocketItem=NULL;
	for (INT_PTR i=0;i<m_StorageSocketItem.GetCount();i++)
	{
		pSocketItem=m_StorageSocketItem[i];
		ASSERT(pSocketItem!=NULL);
		SafeDelete(pSocketItem);
	}
	m_StorageSocketItem.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CTCPNetworkEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPNetworkEngine,Guid,dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPNetworkEngine,Guid,dwQueryVer);
	return NULL;
}
//
////���ýӿ�
//bool __cdecl CTCPNetworkEngine::SetSocketEngineSink(IUnknownEx * pIUnknownEx)
//{
//	//״̬�ж�
//	if (m_bService==true) 
//	{
//		CTraceService::TraceString(TEXT("�������洦�ڷ���״̬���󶨲�������"),TraceLevel_Exception);
//		
//		return false;
//	}
//
//	//���ýӿ�
//	if (m_AttemperEvent.SetQueueService(pIUnknownEx)==false)
//	{
//		CTraceService::TraceString(TEXT("���������봥�������ʧ��"),TraceLevel_Exception);
//		return false;
//	}
//	
//	return true;
//}
//
////������Ŀ
//bool __cdecl CTCPNetworkEngine::SetMaxSocketItem(WORD wMaxSocketItem)
//{
//	m_wMaxSocketItem=wMaxSocketItem;
//	return true;
//}

//��������
bool __cdecl CTCPNetworkEngine::StartService()
{
	DWORD i = 0;
	//Ч��״̬
	if (m_bService==true) 
	{
		CTraceService::TraceString(TEXT("���������ظ�������������������"),TraceLevel_Warning);
		return true;
	}

	//�ж϶˿�
	if (m_wListenPort==0)
	{
		CTraceService::TraceString(TEXT("������������˿���Ч"),TraceLevel_Exception);
		return false;
	}

	//�󶨶���
	if (m_SendQueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx))==false)
	{
		CTraceService::TraceString(TEXT("���������봥�������ʧ��"),TraceLevel_Exception);
		return false;
	}

	try
	{
		//��ȡϵͳ��Ϣ
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		DWORD dwThreadCount=SystemInfo.dwNumberOfProcessors;

		//������ɶ˿�
		m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,SystemInfo.dwNumberOfProcessors);
		if (m_hCompletionPort==NULL) throw TEXT("����������ɶ˿ڴ���ʧ��");

		//��������SOCKET
		struct sockaddr_in SocketAddr;
		memset(&SocketAddr,0,sizeof(SocketAddr));
		SocketAddr.sin_addr.s_addr=INADDR_ANY;
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(m_wListenPort);
		m_hServerSocket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
		if (m_hServerSocket==INVALID_SOCKET) throw TEXT("����������� SOCKET ����ʧ��");
		int iErrorCode=bind(m_hServerSocket,(SOCKADDR*)&SocketAddr,sizeof(SocketAddr));
		if (iErrorCode==SOCKET_ERROR) throw TEXT("������������˿ڱ�ռ�ã��˿ڰ�ʧ��");
		iErrorCode=listen(m_hServerSocket,200);
		if (iErrorCode==SOCKET_ERROR) throw TEXT("������������˿ڱ�ռ�ã��˿ڼ���ʧ��");

		//�������Ͷ���
		bool bSuccess=m_SendQueueService.StartService();
		if (bSuccess==false) throw TEXT("�������淢�Ͷ��з�������ʧ��");

		//������д�߳�
		for (i=0;i<dwThreadCount;i++)
		{
			CServerSocketRSThread * pServerSocketRSThread=new CServerSocketRSThread();
			if (pServerSocketRSThread==NULL) throw TEXT("���������д�̷߳��񴴽�ʧ��");
			bSuccess=pServerSocketRSThread->InitThread(m_hCompletionPort);
			if (bSuccess==false) throw TEXT("���������д�̷߳�������ʧ��");
			m_SocketRSThreadArray.Add(pServerSocketRSThread);
		}

		//����Ӧ���߳�
		bSuccess=m_SocketAcceptThread.InitThread(m_hCompletionPort,m_hServerSocket,this);
		if (bSuccess==false) throw TEXT("����������������̷߳�������");

		//���ж�д�߳�
		for (i=0;i<dwThreadCount;i++)
		{
			CServerSocketRSThread * pServerSocketRSThread=m_SocketRSThreadArray[i];
			ASSERT(pServerSocketRSThread!=NULL);
			bSuccess=pServerSocketRSThread->StartThread();
				//StartThead();
			if (bSuccess==false) throw TEXT("���������д�̷߳�������ʧ��");
		}

		//�������߳�
		m_SocketDetectThread.InitThread(this);
		bSuccess=m_SocketDetectThread.StartThread();
		if (bSuccess==false) throw TEXT("����������̷߳�������ʧ��");

		//����Ӧ���߳�
		bSuccess=m_SocketAcceptThread.StartThread();
		if (bSuccess==false) throw TEXT("������������̷߳�������ʧ��");

		//���ñ���
		m_bService=true;
	}
	catch (LPCTSTR pszError)
	{
		CTraceService::TraceString(pszError,TraceLevel_Exception);
		return false;
	}

	return true;
}

//ֹͣ����
bool __cdecl CTCPNetworkEngine::ConcludeService()
{
	//���ñ���
	m_bService=false;
	m_dwLastDetect=0L;

	//ֹͣ����߳� 
	m_SocketDetectThread.ConcludeThread(INFINITE);

	//��ֹӦ���߳�
	if (m_hServerSocket!=INVALID_SOCKET)
	{
		CTraceService::TraceString("CTCPNetworkEngine::ConcludeService()()",TraceLevel_Info);
		closesocket(m_hServerSocket);
		m_hServerSocket=INVALID_SOCKET;
	}
	m_SocketAcceptThread.ConcludeThread(INFINITE);

	//ֹͣ���Ͷ���
	m_SendQueueService.StopService();

	//�ͷŶ�д�߳�
	INT_PTR nCount=m_SocketRSThreadArray.GetCount(),i=0;
	if (m_hCompletionPort!=NULL)
	{
		for (i=0;i<nCount;i++) PostQueuedCompletionStatus(m_hCompletionPort,0,NULL,NULL);
	}
	for (i=0;i<nCount;i++)
	{
		CServerSocketRSThread * pSocketThread=m_SocketRSThreadArray[i];
		ASSERT(pSocketThread!=NULL);
		pSocketThread->ConcludeThread(INFINITE);
		SafeDelete(pSocketThread);
	}
	m_SocketRSThreadArray.RemoveAll();

	//�ر���ɶ˿�
	if (m_hCompletionPort!=NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort=NULL;
	}

	//�ر�����
	CServerSocketItem * pServerSocketItem=NULL;
	for (i=0;i<m_ActiveSocketItem.GetCount();i++)
	{
		pServerSocketItem=m_ActiveSocketItem[i];
		ASSERT(pServerSocketItem!=NULL);
		CTraceService::TraceString("CTCPNetworkEngine::ConcludeService()2()",TraceLevel_Info);
		pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
		pServerSocketItem->ResetSocketData();
	}
	m_FreeSocketItem.Append(m_ActiveSocketItem);
	m_ActiveSocketItem.RemoveAll();

	m_pITCPNetworkEngineEvent=NULL;
	return true;
}

//Ӧ����Ϣ
bool CTCPNetworkEngine::OnSocketAcceptEvent(CServerSocketItem * pServerSocketItem)
{
	//Ч������
	ASSERT(pServerSocketItem!=NULL);

	//Ͷ����Ϣ
	if (m_bService==false) return false;

	ASSERT(m_pITCPNetworkEngineEvent!=NULL);
	m_pITCPNetworkEngineEvent->OnEventTCPNetworkBind(pServerSocketItem->GetIdentifierID(), pServerSocketItem->GetClientAddr());

	return true;
}

//�����ȡ��Ϣ
bool CTCPNetworkEngine::OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem)
{
	//Ч������
	ASSERT(pServerSocketItem!=NULL);

	//Ч��״̬
	if (m_bService==false) return false;
	ASSERT(m_pITCPNetworkEngineEvent!=NULL);
	m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(pServerSocketItem->GetIdentifierID(), Command,pBuffer,wDataSize);

	return true;
}

//����ر���Ϣ
bool CTCPNetworkEngine::OnSocketCloseEvent(CServerSocketItem * pServerSocketItem)
{
	//Ч�����
	ASSERT(pServerSocketItem!=NULL);
	try
	{
		//Ч��״̬
		if (m_bService==false) return false;

		//����ʱ��
		WORD wIndex=pServerSocketItem->GetIndex();
		WORD wRountID=pServerSocketItem->GetRountID();
		DWORD dwClientAddr=pServerSocketItem->GetClientAddr();
		DWORD dwConnectTime=pServerSocketItem->GetConnectTime();

		ASSERT(m_pITCPNetworkEngineEvent!=NULL);
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(pServerSocketItem->GetIdentifierID(),pServerSocketItem->GetClientAddr(),pServerSocketItem->GetConnectTime());


		//�ͷ�����
		FreeSocketItem(pServerSocketItem);
	}
	catch (...) {}

	return true;
}

//֪ͨ�ص����������Ͷ����̵߳��ã�
void __cdecl CTCPNetworkEngine::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case QUEUE_SEND_REQUEST:		//��������
		{
			//Ч������
			tagSendDataRequest * pSendDataRequest=(tagSendDataRequest *)pBuffer;
			ASSERT(wDataSize>=(sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuf)));
			ASSERT(wDataSize==(pSendDataRequest->wDataSize+sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuf)));

			//��������
			if (pSendDataRequest->wIndex==INDEX_ALL_SOCKET)
			{
				//��ȡ���
				CThreadLock ItemLockedHandle(m_CriticalSection);
				m_TempSocketItem.Copy(m_ActiveSocketItem);
				ItemLockedHandle.UnLock();

				//ѭ����������
				CServerSocketItem * pServerSocketItem=NULL;
				for (INT_PTR i=0;i<m_TempSocketItem.GetCount();i++)
				{
					pServerSocketItem=m_TempSocketItem[i];
					ASSERT(pServerSocketItem!=NULL);
					CThreadLock SocketLockHandle(m_CriticalSection);
					//if (pServerSocketItem->IsAllowBatch())
					//{
						//pServerSocketItem->SendData(pSendDataRequest->cbSendBuf,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
						//	pSendDataRequest->wSubCmdID,pServerSocketItem->GetRountID());
						pServerSocketItem->SendData(pSendDataRequest->cbSendBuf,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
							pSendDataRequest->wSubCmdID,pServerSocketItem->GetRountID());
					//}
				}
			}
			else
			{
				//�����
				CServerSocketItem * pServerSocketItem=EnumSocketItem(pSendDataRequest->wIndex);
				CThreadLock SocketLockHandle(m_CriticalSection);
				pServerSocketItem->SendData(pSendDataRequest->cbSendBuf,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
					pSendDataRequest->wSubCmdID,pSendDataRequest->wRountID);
			}

			break;
		}
	case QUEUE_SAFE_CLOSE:		//��ȫ�ر�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(tagSafeCloseSocket));
			tagSafeCloseSocket * pSafeCloseSocket=(tagSafeCloseSocket *)pBuffer;

			//��ȫ�ر�
			CServerSocketItem * pServerSocketItem=EnumSocketItem(pSafeCloseSocket->wIndex);
			CThreadLock SocketLockHandle(m_CriticalSection);
			pServerSocketItem->ShutDownSocket(pSafeCloseSocket->wRountID);

			break;
		}
	case QUEUE_ALLOW_BATCH:		//����Ⱥ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(tagAllowBatchSend));
			tagAllowBatchSend * pAllowBatchSend=(tagAllowBatchSend *)pBuffer;

			//����Ⱥ��
			CServerSocketItem * pServerSocketItem=EnumSocketItem(pAllowBatchSend->wIndex);
			CThreadLock SocketLockHandle(m_CriticalSection);
			pServerSocketItem->AllowBatchSend(pAllowBatchSend->wIndex,pAllowBatchSend->bAllow?true:false);

			break;
		}
	case QUEUE_DETECT_SOCKET:	//�������
		{
			//��ȡ���
			CThreadLock ItemLockedHandle(m_CriticalSection);
			m_TempSocketItem.Copy(m_ActiveSocketItem);
			ItemLockedHandle.UnLock();

			//��������
			CMD_KN_DetectSocket DetectSocket;
			ZeroMemory(&DetectSocket,sizeof(DetectSocket));

			//��������
			WORD wRountID=0;
			DWORD dwNowTickCount=GetTickCount();
			DWORD dwBreakTickCount=__max(dwNowTickCount-m_dwLastDetect,TIME_BREAK_READY);

			//���ñ���
			m_dwLastDetect=GetTickCount();

			//�������
			for (INT_PTR i=0;i<m_TempSocketItem.GetCount();i++)
			{
				//��������
				CServerSocketItem * pServerSocketItem=m_TempSocketItem[i];
				DWORD dwRecvTickCount=pServerSocketItem->GetRecvTickCount();
				CThreadLock SocketLockHandle(m_CriticalSection);

				//������
				if (dwRecvTickCount>=dwNowTickCount) continue;

				//�������
				if (pServerSocketItem->IsReadySend()==true)
				{
					if ((dwNowTickCount-dwRecvTickCount)>dwBreakTickCount)
					{
						pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
						continue;
					}
				}
				else
				{
					//todo
					if ((dwNowTickCount-dwRecvTickCount)>TIME_BREAK_CONNECT)
					{
						pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
						continue;
					}
				}

				//��������
				if (pServerSocketItem->IsReadySend()==true)
				{
					wRountID=pServerSocketItem->GetRountID();
					DetectSocket.dwSendTickCount=GetTickCount();
					pServerSocketItem->SendData(&DetectSocket,sizeof(DetectSocket),MDM_KN_COMMAND,SUB_KN_DETECT_SOCKET,wRountID);
				}
			}

			break;
		}
	default: { ASSERT(FALSE); }
	}

	return;
}

//��ȡ���ж���
CServerSocketItem * CTCPNetworkEngine::ActiveSocketItem()
{
	CThreadLock ItemLockedHandle(m_CriticalSection);

	////��ȡ���ж���
	CServerSocketItem * pServerSocketItem=NULL;
	if (m_FreeSocketItem.GetCount()>0)
	{
		INT_PTR nItemPostion=m_FreeSocketItem.GetCount()-1;
		pServerSocketItem=m_FreeSocketItem[nItemPostion];
		ASSERT(pServerSocketItem!=NULL);
		m_FreeSocketItem.RemoveAt(nItemPostion);
		m_ActiveSocketItem.Add(pServerSocketItem);
	}

	//�����¶���
	if (pServerSocketItem==NULL)
	{
		WORD wStorageCount=(WORD)m_StorageSocketItem.GetCount();
		if (wStorageCount<m_wMaxSocketItem)
		{
			try
			{
				pServerSocketItem=new CServerSocketItem(wStorageCount,this);
				if (pServerSocketItem==NULL) return NULL;
				m_StorageSocketItem.Add(pServerSocketItem);
				m_ActiveSocketItem.Add(pServerSocketItem);
			}
			catch (...) { return NULL; }
		}
	}

	return pServerSocketItem;
}

//��ȡ���Ӷ���
CServerSocketItem * CTCPNetworkEngine::EnumSocketItem(WORD wIndex)
{
	CThreadLock ItemLockedHandle(m_CriticalSection);
	if (wIndex<m_StorageSocketItem.GetCount())
	{
		CServerSocketItem * pServerSocketItem=m_StorageSocketItem[wIndex];
		ASSERT(pServerSocketItem!=NULL);
		return pServerSocketItem;
	}
	return NULL;
}

//�ͷ����Ӷ���
bool CTCPNetworkEngine::FreeSocketItem(CServerSocketItem * pServerSocketItem)
{
	//Ч�����
	ASSERT(pServerSocketItem!=NULL);

	//�ͷŶ���
	CThreadLock ItemLockedHandle(m_CriticalSection);
	INT_PTR nActiveCount=m_ActiveSocketItem.GetCount();
	for (int i=0;i<nActiveCount;i++)
	{
		if (pServerSocketItem==m_ActiveSocketItem[i])
		{
			m_ActiveSocketItem.RemoveAt(i);
			m_FreeSocketItem.Add(pServerSocketItem);
			return true;
		}
	}

	//�ͷ�ʧ��
	ASSERT(FALSE);
	return false;
}

//�������
bool __cdecl CTCPNetworkEngine::DetectSocket()
{
	return m_SendQueueService.AddToQueue(QUEUE_DETECT_SOCKET,NULL,0);
}

bool __cdecl CTCPNetworkEngine::SetTCPNetworkEngineEvent(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx!=NULL);
	ASSERT(m_pITCPNetworkEngineEvent==NULL);
	if (pIUnknownEx==NULL) return false;
	if (m_pITCPNetworkEngineEvent!=NULL) return false;

	//��ѯ�ӿ�
	m_pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPNetworkEngineEvent);
	if (m_pITCPNetworkEngineEvent==NULL)
	{
		CTraceService::TraceString(TEXT("�����������ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"),TraceLevel_Exception);
		return false;
	}

	return true;
}

bool __cdecl CTCPNetworkEngine::SetServiceParameter(WORD wServicePort, WORD wMaxConnect)
{
	//Ч��״̬
	if (m_bService==true) 
	{
		CTraceService::TraceString(TEXT("TCPSocketService���ڷ���״̬���˿ڰ󶨲�������"),TraceLevel_Warning);
		return false;
	}

	//�жϲ���
	if (wServicePort==0)
	{
		CTraceService::TraceString(TEXT("����˿ڴ��󣬶˿ڰ󶨲���ʧ��"),TraceLevel_Exception);
		return false;
	}

	m_wListenPort=wServicePort;
	m_wMaxSocketItem=wMaxConnect;

	return true;
}

//
bool __cdecl CTCPNetworkEngine::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч��״̬
	ASSERT(m_bService==true);
	if (m_bService==false) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex= LOWORD(dwSocketID);
	SendRequest.wRountID=HIWORD(dwSocketID);
	SendRequest.wDataSize=0;

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf);
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}
//
bool __cdecl CTCPNetworkEngine::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч��״̬
	ASSERT(m_bService==true);
	if (m_bService==false) return false;

	//Ч������
	ASSERT((wDataSize+sizeof(CMD_Head))<=SOCKET_PACKAGE);
	if ((wDataSize+sizeof(CMD_Head))>SOCKET_PACKAGE) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=LOWORD(dwSocketID);
	SendRequest.wRountID=HIWORD(dwSocketID);
	SendRequest.wDataSize=wDataSize;
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SendRequest.cbSendBuf,pData,wDataSize);
	}

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf)+wDataSize;
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}


//��������
bool __cdecl CTCPNetworkEngine::SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_bService==false) return false;

	//Ч������
	ASSERT((wDataSize+sizeof(CMD_Head))<=SOCKET_PACKET);
	if ((wDataSize+sizeof(CMD_Head))>SOCKET_PACKET) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=INDEX_ALL_SOCKET;
	SendRequest.wRountID=0;
	SendRequest.wDataSize=wDataSize;
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SendRequest.cbSendBuf,pData,wDataSize);
	}

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf)+wDataSize;
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//�ر�����
bool __cdecl CTCPNetworkEngine::CloseSocket(DWORD dwSocketID)
{
	CServerSocketItem * pServerSocketItem=EnumSocketItem(LOWORD(dwSocketID));
	if (pServerSocketItem==NULL) return false;
	CTraceService::TraceString("CTCPNetworkEngine::CloseSocket()2",TraceLevel_Info);
	return pServerSocketItem->CloseSocket(HIWORD(dwSocketID));
}
//����Ⱥ��
bool __cdecl CTCPNetworkEngine::AllowBatchSend(DWORD dwSocketID, bool bAllowBatch)
{
	tagAllowBatchSend AllowBatchSend;
	AllowBatchSend.wIndex=LOWORD(dwSocketID);
	AllowBatchSend.wRountID=HIWORD(dwSocketID);
	AllowBatchSend.bAllow=bAllowBatch;
	return m_SendQueueService.AddToQueue(QUEUE_ALLOW_BATCH,&AllowBatchSend,sizeof(AllowBatchSend));
}
//���ùر�
bool __cdecl CTCPNetworkEngine::ShutDownSocket(DWORD dwSocketID)
{
	tagSafeCloseSocket SafeCloseSocket;
	SafeCloseSocket.wIndex=LOWORD(dwSocketID);
	SafeCloseSocket.wRountID=HIWORD(dwSocketID);
	return m_SendQueueService.AddToQueue(QUEUE_SAFE_CLOSE,&SafeCloseSocket,sizeof(SafeCloseSocket));
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateTCPNetworkEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CTCPNetworkEngine * pTCPNetworkEngine=NULL;
	try
	{
		pTCPNetworkEngine=new CTCPNetworkEngine();
		if (pTCPNetworkEngine==NULL) throw TEXT("����ʧ��");
		void * pObject=pTCPNetworkEngine->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTCPNetworkEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
